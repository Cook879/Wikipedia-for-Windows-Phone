#include "pch.h"
#include "MainPage.xaml.h"
#include "HelperFunctions.h"

#include "Article.h"
#include "LanguagesContentDialog.xaml.h"
#include "TocContentDialog.xaml.h"
#include "SearchContentDialog.xaml.h"
#include "LoginPage.xaml.h"

using namespace Wikipedia;

using namespace concurrency;
using namespace std;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::Media::SpeechRecognition;
using namespace Windows::Networking::Connectivity;
using namespace Windows::Phone::UI::Input;
using namespace Windows::Storage;
using namespace Windows::System;
using namespace Windows::UI::Popups;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Web::Http;
using namespace Windows::Web::Http::Filters;

static TypeName editPageType = { "Wikipedia.EditPage", TypeKind::Metadata };
static TypeName aboutPageType = { "Wikipedia.AboutPage", TypeKind::Metadata };

MainPage::MainPage()
{
	InitializeComponent();

	this->NavigationCacheMode = Navigation::NavigationCacheMode::Required;

	httpClient = HelperFunctions::CreateHttpClient();

	loader = ref new Windows::ApplicationModel::Resources::ResourceLoader();

	// TODO change to main page
	//article = ref new Article("User:Cook879/sandbox", loader->GetString("langCode"));
	article = ref new Article("Main Page", loader->GetString("langCode"));
	//article = ref new Article("Frank Sinatra", "de");

	// Set up back button
	history = ref new Vector<String^>();
	historyPos = 0;
	_backPressedToken = HardwareButtons::BackPressed += ref new EventHandler<BackPressedEventArgs^>(this, &MainPage::MainPage_BackPressed);

}

void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	// Set up voice controls
	if (e->NavigationMode == NavigationMode::New)
	{
		Uri^ uri = ref new Uri("ms-appx:///VoiceCommands.xml");

		create_task(StorageFile::GetFileFromApplicationUriAsync(uri)).then([](task<StorageFile^> t)
		{
			VoiceCommandManager::InstallCommandSetsFromStorageFileAsync(t.get());
		});
	}

	// Check if we have a page we want
	if (e->Parameter != nullptr) {
		if (e->Parameter->GetType()->ToString()->Equals("String"))
			article = (Article^)e->Parameter;
		else {
			OutputDebugString(L"\n\nvoice search\n\n");
		}
	}
	WebViewControl->NavigationStarting += ref new TypedEventHandler<WebView^, WebViewNavigationStartingEventArgs^>(this, &MainPage::WebViewControl_NavigationStarting);

	// Check if we need to get content or not
	String^ content;
	if (article->HasContent()) {
		content = article->GetContent();
		NavigateToString(content);
	} 
	else {
		LoadContent();
	}

	// Get username and login the user
	auto values = ApplicationData::Current->LocalSettings->Values;
	String^ username = (String^)values->Lookup("username");
	if (!username->IsEmpty()) {
		Windows::Security::Credentials::PasswordVault^ vault = ref new Windows::Security::Credentials::PasswordVault();
		Windows::Security::Credentials::PasswordCredential^ cred = vault->Retrieve("Wikipedia", username);
		LogIn(cred);
	}
}

void MainPage::NavigateToString(String^ content) {
	WebViewControl->NavigateToString(content);

	history->Append(content);
	historyPos = history->Size - 1;
}

void MainPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	HardwareButtons::BackPressed -= _backPressedToken;
}


void MainPage::Browser_NavigationCompleted(Object^ sender, WebViewNavigationCompletedEventArgs^ e)
{}

void MainPage::MainPage_BackPressed(Object^ sender, BackPressedEventArgs^ e)
{
	if (historyPos > 0) {
		historyPos -= 1;
		WebViewControl->NavigateToString(history->GetAt(historyPos));

		history->RemoveAtEnd();

		e->Handled = true;
	}
}

/// <summary>
/// Navigates to the initial home page.
/// </summary>
void MainPage::HomeAppBarButton_Click(Object^ sender, RoutedEventArgs^ e)
{
	OutputDebugString(L"Method: homeappbarbuttonclick");
	(sender);	// Unused parameter
	(e);		// Unused parameter

	Article^ articleNew = ref new Article("Main Page", article->GetLang());
	ChangeArticle(articleNew);
}

// Obtains the content of the article from the server and displays it
void MainPage::LoadContent() {
	// Check internet connection
	ConnectionProfile^ profile = NetworkInformation::GetInternetConnectionProfile();

	if (profile != nullptr)	{
		if (profile->NetworkAdapter != nullptr)	{

			try {
				Uri^ uri = ref new Uri("https://" + article->GetLang() + ".wikipedia.org/w/api.php?action=mobileview&format=json&page=" + article->GetTitle() + "&sections=all&sectionprop=level|line|number");

				create_task(httpClient->GetAsync(uri)).then([=](HttpResponseMessage^ response) {
					response->EnsureSuccessStatusCode();

					return create_task(response->Content->ReadAsStringAsync());
				}).then([=](String^ responseString) {
					JsonObject^ json = JsonValue::Parse(responseString)->GetObject();

					if (json->HasKey("mobileview")) {
						json = json->GetNamedObject("mobileview");
						
						if (json->HasKey("sections")) {
							JsonArray^ sections = json->GetNamedArray("sections");

							if (sections->GetAt(0)->GetObject()->HasKey("text")) {
								int arraySize = sections->Size;
								String^ content = ref new String();

								// Section 0
								IJsonValue^ sectionJson = sections->GetAt(0);
								if (sectionJson->ValueType == JsonValueType::Object) {
									JsonObject^ sectionObj = sectionJson->GetObject();

									content += sectionObj->GetNamedString("text");

									Section^ section = ref new Section(0, loader->GetString("Introduction"), sectionObj->GetNamedString("text"), 2, nullptr);
									article->AddSection(section);
								}

								// Get the text for each section of the article
								int defaultLevel = 2;

								Section^ parent = nullptr;

								for (int i = 1; i < arraySize; i++) {

									IJsonValue^ sectionJson = sections->GetAt(i);

									if (sectionJson->ValueType == JsonValueType::Object) {
										JsonObject^ sectionObj = sectionJson->GetObject();

										int curLevel = _wtol(sectionObj->GetNamedString("level")->Data());

										int nextLevel;
										if (i == arraySize - 1)
											nextLevel = defaultLevel;
										else
											nextLevel = _wtol(sections->GetAt(i + 1)->GetObject()->GetNamedString("level")->Data());

										String^ sectionText = HelperFunctions::ProcessSection(sectionObj->GetNamedString("text"), i, curLevel);

										content += sectionText;

										int diff = curLevel - nextLevel;

										if (diff < 0) {
											// Ain't gonna do a thing
										}
										else {
											diff++;
											for (int i = 0; i < diff; i++) {
												content += "</div>";
											}
										}

										String^ title = sectionObj->GetNamedString("number") + ". " + sectionObj->GetNamedString("line");

										Section^ section = ref new Section(i, title, sectionObj->GetNamedString("text"), curLevel, parent);
										article->AddSection(section);


										//DEBUG
										String^ parentStr;
										if (section->parent == nullptr)
											parentStr = "null";
										else
											parentStr = section->parent->id + "";
										

										diff--;

										if (diff < 0) {
											OutputDebugString(L"diff < 0 so parent=section\n");

											parent = section;
										}
										else if (diff > 0) {
											OutputDebugString(L"diff > 0 so parent=parent->parent \n");
											parent = parent->parent;
										}
									}
								}

								if (json->HasKey("normalizedtitle")){
									article->SetTitle(json->GetNamedString("normalizedtitle"));
								}

								content = HelperFunctions::ProcessContent(content, article->GetTitle());
								content = HelperFunctions::ProcessImages(content);

								NavigateToString(content);

								article->SetContent(content);

								ObtainLanguageData(article);
							} else {
								HelperFunctions::ErrorMessage(loader->GetString("standardError"));
							}
					} else {
						HelperFunctions::ErrorMessage(loader->GetString("standardError"));
					}
				} else {
					HelperFunctions::ErrorMessage(loader->GetString("pageNotExist"));
				}

				}).then([=](task<void> prevTask) {
					try {
						prevTask.get();
					}
					catch (Exception ^ex) {
						return;
					}
				});
			}
			catch (Exception^ e) {
				HelperFunctions::ErrorMessage(loader->GetString("standardError"));
			}
		} else {
			HelperFunctions::ErrorMessage(loader->GetString("noInternet"));
		}
	} else {
		HelperFunctions::ErrorMessage(loader->GetString("noInternet"));
	}
}


// Intercept the URI and make sure to direct it to its intended destination
void MainPage::WebViewControl_NavigationStarting(WebView^ sender, WebViewNavigationStartingEventArgs^ args)
{
	wstring uri = args->Uri->ToString()->Data();

	if (!uri.empty()) {

		wstring about = L"about:/wiki";
		if (uri.find(about) == 0)  {

			NavigateToString(article->GetContent());
			wstring pageName = uri.substr(about.size() + 1);

			//article = ref new Article(ref new String(pageName.c_str()), article->GetLang());

			ChangeArticle(ref new Article(ref new String(pageName.c_str()), article->GetLang()));
			//LoadContent();
		}
		else if ((uri.find(L"about:blank#editor") == 0)) {
			wstring text = L"about:blank#editor/";
			wstring sectionID = uri.substr(text.size());
			int section = _wtof(sectionID.c_str());

			EditSection(section);
		}

		else if (uri.find(L"about:/w/index.php?title=") == 0 && uri.find(L"action=edit&section=") != wstring::npos) {

			wstring text = L"action=edit&section=";
			int pos = uri.find(text);
			
			wstring sectionID = uri.substr(pos+text.size()).c_str();
			int section = _wtof(sectionID.c_str());
			EditSection(section);
		}
		else if (uri.find(L"about:blank") == 0) {
			// Do nothing
		}
		else {
			Launcher::LaunchUriAsync(args->Uri);
		}
	}
}

void MainPage::TocAppBarButton_Click(Object^ sender, RoutedEventArgs^ e){
	TocContentDialog^ tcd = ref new TocContentDialog(article, WebViewControl);
	tcd->ShowAsync();
}

void MainPage::EditAppBarButton_Click(Object^ sender, RoutedEventArgs^ e)
{
	//IVector<Object^>^ params = ref new Vector<Object^>();
	Edit^ edit = ref new Edit(article, 0, nullptr, nullptr, nullptr);
	//params->Append(edit);
	//params->Append(this);

	this->Frame->Navigate(editPageType, edit);
}

void MainPage::SearchAppBarButton_Click(Object^ sender, RoutedEventArgs^ e){
	SearchContentDialog^ sp = ref new SearchContentDialog(article->GetLang(), this);
	sp->ShowAsync();
}

//
// Calls the random page api and directs the user to that new page.
//
void MainPage::RandomAppBarButton_Click(Object^ sender, RoutedEventArgs^ e) {
	ConnectionProfile^ profile = NetworkInformation::GetInternetConnectionProfile();
	if (profile != nullptr)	{
		if (profile->NetworkAdapter != nullptr) {

			try {
				Uri^ uri = ref new Uri("https://" + article->GetLang() + ".wikipedia.org/w/api.php?action=query&format=json&list=random&rnnamespace=0");

				create_task(httpClient->GetAsync(uri)).then([=](HttpResponseMessage^ response) {
					response->EnsureSuccessStatusCode();

					return create_task(response->Content->ReadAsStringAsync());
				}).then([=](String^ responseString) {
					JsonObject^ json = JsonValue::Parse(responseString)->GetObject();

					if (json->HasKey("query")) {
						json = json->GetNamedObject("query");

						if (json->HasKey("random")) {
							json = json->GetNamedArray("random")->GetObjectAt(0);

							String^ title = json->GetNamedString("title");

							ChangeArticle(ref new Article(title, article->GetLang()));
						} else {
							HelperFunctions::ErrorMessage(loader->GetString("standardError"));
						}
					} else {
						HelperFunctions::ErrorMessage(loader->GetString("standardError"));
					}
				}).then([=](task<void> prevTask) {
					try {
						prevTask.get();
					} catch (Exception ^ex) {
						return;
					}
				});
			} catch (Exception^ e) {
				HelperFunctions::ErrorMessage(loader->GetString("standardError"));
			}
		} else {
			HelperFunctions::ErrorMessage(loader->GetString("noInternet"));
		}
	} else {
		HelperFunctions::ErrorMessage(loader->GetString("noInternet"));
	}
}

void MainPage::LanguageAppBarButton_Click(Object^ sender, RoutedEventArgs^ e) {
	LanguagesContentDialog^ lcd = ref new LanguagesContentDialog(article, this);
	lcd->ShowAsync();
}

void MainPage::LoginAppBarButton_Click(Object^ sender, RoutedEventArgs^ e) {
	LoginPage^ lp = ref new LoginPage(this);
	lp->ShowAsync();
}

void MainPage::LogoutAppBarButton_Click(Object^ sender, RoutedEventArgs^ e) {
	// Check internet connection
	ConnectionProfile^ profile = NetworkInformation::GetInternetConnectionProfile();

	if (profile != nullptr)	{
		if (profile->NetworkAdapter != nullptr)	{

			try {
				Uri^ uri = ref new Uri("https://" + article->GetLang() + ".wikipedia.org/w/api.php?action=logout");

				create_task(httpClient->GetAsync(uri)).then([=](HttpResponseMessage^ response) {
					response->EnsureSuccessStatusCode();

					LoginAppBarButton->Label = loader->GetString("login");

					LoginAppBarButton->IsEnabled = true;
					LogoutAppBarButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

					auto values = ApplicationData::Current->LocalSettings->Values;
					values->Remove("cookie");
					values->Remove("username");

					HttpBaseProtocolFilter^ filter = ref new HttpBaseProtocolFilter();
					HttpCookieCollection^ cookieCollection = filter->CookieManager->GetCookies(uri);

					IIterator<HttpCookie^>^ iterator = cookieCollection->First();
					while (iterator->HasCurrent)
					{
						HttpCookie^ cookie = iterator->Current;
						filter->CookieManager->DeleteCookie(cookie);						
						iterator->MoveNext();
					}

					HelperFunctions::ErrorMessage(loader->GetString("logout"));

					return create_task(response->Content->ReadAsStringAsync());
				});
			} catch (Exception^ e) {
				HelperFunctions::ErrorMessage(loader->GetString("standardError"));
			}
		} else {
			HelperFunctions::ErrorMessage(loader->GetString("noInternet"));
		}
	} else {
		HelperFunctions::ErrorMessage(loader->GetString("noInternet"));
	}
}

void MainPage::AboutAppBarButton_Click(Object^ sender, RoutedEventArgs^ e) {
	this->Frame->Navigate(aboutPageType);
}

void MainPage::ObtainLanguageData(Article^ article) {
	ConnectionProfile^ profile = NetworkInformation::GetInternetConnectionProfile();
	
	if (profile != nullptr)	{
		if (profile->NetworkAdapter != nullptr) {

			// There are 285 Wikipedias - set to 300 allows no issues for future additions
			Uri^ uri = ref new Uri("https://" + article->GetLang() + ".wikipedia.org/w/api.php?action=query&prop=langlinks&format=json&titles=" + article->GetTitle() + "&lllimit=300");

			create_task(httpClient->GetAsync(uri)).then([=](HttpResponseMessage^ response) {
				response->EnsureSuccessStatusCode();

				return create_task(response->Content->ReadAsStringAsync());
			}).then([=](String^ responseString) {
				JsonObject^ json = JsonValue::Parse(responseString)->GetObject();

				if (json->HasKey("query")) {
					json = json->GetNamedObject("query");
				
					if (json->HasKey("pages")) {
						JsonObject^ pages = json->GetNamedObject("pages");

						/* Start hack-y way to get the json for the page as it doesn't like GetObject() */
						std::wstring pageIdString(pages->Stringify()->Data());
						int endPos = pageIdString.find(':');
						pageIdString = pageIdString.erase(0, endPos + 1);
						pageIdString = pageIdString.erase(pageIdString.length() - 1, 1);
						JsonValue^ pageId = JsonValue::Parse(ref new String(pageIdString.c_str()));
						/* End hack-y code */

						JsonObject^ pageData = pageId->GetObject();
						if (pageData->HasKey("langlinks")) {
							JsonArray^ langlinks = pageData->GetNamedArray("langlinks");

							for (int i = 0; i < langlinks->Size; i++) {
								JsonObject^ data = langlinks->GetObjectAt(i);
								String^ title = data->GetNamedString("*");
								if (title->IsEmpty())
									title = "Main Page";
								Article^ lang = ref new Article(title, data->GetNamedString("lang"));
								article->AddLanguage(lang);
							}

							article->FoundLanguages();
							LanguageAppBarButton->IsEnabled = true;
						} else {
							LanguageAppBarButton->IsEnabled = false;
						}
					} else {
						HelperFunctions::ErrorMessage(loader->GetString("standardError"));
					}
				} else {
				HelperFunctions::ErrorMessage(loader->GetString("standardError"));
			}

			}).then([=](task<void> prevTask) {
				try {
					prevTask.get();
				}
				catch (Exception ^ex) {
					return;
				}
			});
		} else {
			HelperFunctions::ErrorMessage(loader->GetString("noInternet"));
		}
	} else {
		HelperFunctions::ErrorMessage(loader->GetString("noInternet"));
	}
}

void MainPage::ChangeArticle(Article^ article){
	this->article = article;

	LoadContent();
}

void MainPage::CheckLoggedIn() {
	ConnectionProfile^ profile = NetworkInformation::GetInternetConnectionProfile();

	if (profile != nullptr)	{
		if (profile->NetworkAdapter != nullptr) {

			Uri^ uri = ref new Uri("https://" + article->GetLang() + ".wikipedia.org/w/api.php?action=query&meta=userinfo&format=json");

			create_task(httpClient->GetAsync(uri)).then([=](HttpResponseMessage^ response) {
				response->EnsureSuccessStatusCode();

				return create_task(response->Content->ReadAsStringAsync());
			}).then([=](String^ responseString) {
				JsonObject^ json = JsonValue::Parse(responseString)->GetObject();

				if (json->HasKey("query")) {
					json = json->GetNamedObject("query");

					if (json->HasKey("userinfo")) {
						JsonObject^ userinfo = json->GetNamedObject("userinfo");

						if (userinfo->HasKey("anon")) {
							LoginAppBarButton->Label = loader->GetString("login");

							LoginAppBarButton->IsEnabled = true;
							LogoutAppBarButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

							auto values = ApplicationData::Current->LocalSettings->Values;
							values->Remove("cookie");
							values->Remove("username");
						}
						else {
							ShowLoggedIn(userinfo->GetNamedString("username"));
						}
					}
					else {
						HelperFunctions::ErrorMessage(loader->GetString("standardError"));
					}
				}
				else {
					HelperFunctions::ErrorMessage(loader->GetString("standardError"));
				}

			}).then([=](task<void> prevTask) {
				try {
					prevTask.get();
				}
				catch (Exception ^ex) {
					return;
				}
			});
		}
		else {
			HelperFunctions::ErrorMessage(loader->GetString("noInternet"));
		}
	}
	else {
		HelperFunctions::ErrorMessage(loader->GetString("noInternet"));
	}

}

void MainPage::ShowLoggedIn(String^ username) {
	LoginAppBarButton->IsEnabled = false;
	LogoutAppBarButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
	LoginAppBarButton->Label = loader->GetString("loggedIn") + " " + username;
	httpClient = HelperFunctions::CreateHttpClient();
}

void MainPage::EditSection(int section) {
	//IVector<Object^>^ params = ref new Vector<Object^>();
	Edit^ edit = ref new Edit(article, section, nullptr, nullptr, nullptr);
	//params->Append(edit);
	//params->Append(this);

	this->Frame->Navigate(editPageType, edit);

}

void MainPage::LogIn(Windows::Security::Credentials::PasswordCredential^ cred){
	auto loader = ref new Windows::ApplicationModel::Resources::ResourceLoader();

	ConnectionProfile^ profile = NetworkInformation::GetInternetConnectionProfile();
	if (profile != nullptr)	{
		if (profile->NetworkAdapter != nullptr) {

			String^ username = cred->UserName;			
			try {
				HttpClient^ httpClient = HelperFunctions::CreateHttpClient();

				Uri^ uri = ref new Uri("https://en.wikipedia.org/w/api.php?format=json&action=login");

				Map<String^, String^>^ properties = ref new Map<String^, String^>();

				properties->Insert("lgname", username);
				properties->Insert("lgpassword", cred->Password);

				HttpFormUrlEncodedContent^ propertiesEncoded = ref new HttpFormUrlEncodedContent(properties);

				create_task(httpClient->PostAsync(uri, propertiesEncoded)).then([=](HttpResponseMessage^ response) {
					response->EnsureSuccessStatusCode();

					return create_task(response->Content->ReadAsStringAsync());
				}).then([=](String^ responseString) {
					JsonObject^ json = JsonValue::Parse(responseString)->GetObject();
					if (json->HasKey("login")) {
						json = json->GetNamedObject("login");

						if (json->HasKey("token")) {
							String^ token = json->GetNamedString("token");

							properties->Insert("lgtoken", token);
							HttpFormUrlEncodedContent^ propertiesEncoded = ref new HttpFormUrlEncodedContent(properties);

							create_task(httpClient->PostAsync(uri, propertiesEncoded)).then([=](HttpResponseMessage^ response) {
								response->EnsureSuccessStatusCode();


								return create_task(response->Content->ReadAsStringAsync());
							}).then([=](String^ responseString) {
								JsonObject^ json = JsonValue::Parse(responseString)->GetObject();
								if (json->HasKey("login")) {
									json = json->GetNamedObject("login");
									String^ result = json->GetNamedString("result");

									if (result == "Success") {
										String^ cookiePrefix = json->GetNamedString("cookieprefix");
										String^ sessionId = json->GetNamedString("sessionid");

										ApplicationData::Current->LocalSettings->Values->Insert("username", dynamic_cast<PropertyValue^>(PropertyValue::CreateString(username)));

										Windows::Web::Http::Filters::HttpBaseProtocolFilter^ filter = ref new Windows::Web::Http::Filters::HttpBaseProtocolFilter();
										HttpCookieCollection^ cookieCollection = filter->CookieManager->GetCookies(uri);

										/*OutputDebugString((cookieCollection->Size + " cookies found.\r\n")->Data());
										IIterator<HttpCookie^>^ iterator = cookieCollection->First();
										while (iterator->HasCurrent)
										{
											HttpCookie^ cookie = iterator->Current;


											OutputDebugString(L"--------------------\r\n");
											OutputDebugString(("Name: " + cookie->Name + "\r\n")->Data());
											OutputDebugString(("Domain: " + cookie->Domain + "\r\n")->Data());
											OutputDebugString(("Path: " + cookie->Path + "\r\n")->Data());
											OutputDebugString(("Value: " + cookie->Value + "\r\n")->Data());
											if (cookie->Expires)
											{
												OutputDebugString(("Expires: " + cookie->Expires->Value.UniversalTime + "\r\n")->Data());
											}
											else
											{
												OutputDebugString(L"Expires:\r\n");
											}
											OutputDebugString(("Secure: " + cookie->Secure + "\r\n")->Data());
											OutputDebugString(("HttpOnly: " + cookie->HttpOnly + "\r\n")->Data());

											String^ nameStr = cookie->Name;
											wstring name = nameStr->Data();
											if (name.find(L"Session") != wstring::npos) {
												ApplicationData::Current->LocalSettings->Values->Insert("cookieName", cookie->Name);
												ApplicationData::Current->LocalSettings->Values->Insert("cookieDomain", cookie->Domain);
												ApplicationData::Current->LocalSettings->Values->Insert("cookiePath", cookie->Path);
												ApplicationData::Current->LocalSettings->Values->Insert("cookieValue", cookie->Value);

												break;
											}

											iterator->MoveNext();
										}*/

										this->ShowLoggedIn(username);

										Uri^ uri = ref new Uri("https://en.wikipedia.org/w/api.php?action=query&meta=userinfo&format=json");

										create_task(httpClient->GetAsync(uri)).then([=](HttpResponseMessage^ response) {
											response->EnsureSuccessStatusCode();


											return create_task(response->Content->ReadAsStringAsync());
										}).then([=](String^ responseString) {
											OutputDebugString(responseString->Data());

										}).then([=](task<void> prevTask) {
											try {
												prevTask.get();
											}
											catch (Exception ^ex) {
												return;
											}
										});
									}
								}

							}).then([=](task<void> prevTask) {
								try {
									prevTask.get();
								}
								catch (Exception ^ex) {
									return;
								}
							});
						}
					}
				}).then([=](task<void> prevTask) {
					try {
						prevTask.get();
					}
					catch (Exception ^ex) {
						return;
					}
				});
			}
			catch (Exception^ e) {
				
			}
		}
		else {
			HelperFunctions::ErrorMessage(loader->GetString("noInternet"));
		}
	}	else {
		HelperFunctions::ErrorMessage(loader->GetString("noInternet"));
	}
}