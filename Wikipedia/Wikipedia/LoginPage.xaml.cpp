//
// LoginPage.xaml.cpp
// Implementation of the LoginPage class
//

#include "pch.h"
#include "LoginPage.xaml.h"
#include "Article.h"
#include "HelperFunctions.h"

using namespace Wikipedia;

using namespace std;
using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Networking::Connectivity;
using namespace Windows::Storage;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Web::Http;

//static Windows::UI::Xaml::Interop::TypeName contentPageType = { "Wikipedia.MainPage", Windows::UI::Xaml::Interop::TypeKind::Metadata };

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=390556

LoginPage::LoginPage()
{
	InitializeComponent();
}

LoginPage::LoginPage(MainPage^ mainPage) {
	InitializeComponent();
	this->mainPage = mainPage;
}

void LoginPage::LoginButton_Click() {
	auto loader = ref new Windows::ApplicationModel::Resources::ResourceLoader();

	ConnectionProfile^ profile = NetworkInformation::GetInternetConnectionProfile();
	if (profile != nullptr)	{
		if (profile->NetworkAdapter != nullptr) {

			String^ username = usernameBox->Text;
			String^ password = passwordBox->Password;

			if (username->IsEmpty()) {
				HelperFunctions::ErrorMessage(loader->GetString("noUsername"));
			} else if (password->IsEmpty()) {
				HelperFunctions::ErrorMessage(loader->GetString("noPassword"));
			} else {

				try {

					HttpClient^ httpClient = HelperFunctions::CreateHttpClient();

					Uri^ uri = ref new Uri("https://en.wikipedia.org/w/api.php?format=json&action=login");

					Map<String^, String^>^ properties = ref new Map<String^, String^>();

					properties->Insert("lgname", username);
					properties->Insert("lgpassword", password);

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
											// Not the best way to do it
											//String^ cookiePrefix = json->GetNamedString("cookieprefix");
											//String^ sessionId = json->GetNamedString("sessionid");

											//ApplicationData::Current->LocalSettings->Values->Insert("username", dynamic_cast<PropertyValue^>(PropertyValue::CreateString(username)));
											
											Windows::Security::Credentials::PasswordVault^ vault = ref new Windows::Security::Credentials::PasswordVault();
											Windows::Security::Credentials::PasswordCredential^ c = ref new Windows::Security::Credentials::PasswordCredential("Wikipedia", username, password);
											vault->Add(c);

											Windows::Web::Http::Filters::HttpBaseProtocolFilter^ filter = ref new Windows::Web::Http::Filters::HttpBaseProtocolFilter();
											HttpCookieCollection^ cookieCollection = filter->CookieManager->GetCookies(uri);

											mainPage->ShowLoggedIn(username);

											HelperFunctions::ErrorMessage("Successful login");

											// Should take the page argument
											//mainPage->Frame->Navigate(contentPageType, ref new Article("Main Page", "en"));
											this->Hide();
										} else if (result == "WrongPass") {
											HelperFunctions::ErrorMessage(loader->GetString("wrongPassword"));
										} else if (result == "Illegal") {
											HelperFunctions::ErrorMessage(loader->GetString("illegalUsername"));
										} else if (result == "NotExists") {
											HelperFunctions::ErrorMessage(loader->GetString("usernameNotExist"));
										} else if (result == "Blocked") {
											HelperFunctions::ErrorMessage(loader->GetString("blocked"));
										} else if (result == "Throttled") {
											HelperFunctions::ErrorMessage(loader->GetString("throttled") + json->GetNamedNumber("wait") + " " + loader->GetString("seconds"));
										} else {
											HelperFunctions::ErrorMessage(loader->GetString("loginUnsuc"));
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
			}
		} else {
			 HelperFunctions::ErrorMessage(loader->GetString("noInternet"));
		}
	} else {
	 HelperFunctions::ErrorMessage(loader->GetString("noInternet"));
	}
}
void LoginPage::CancelButton_Click() {
	this->Hide();
}
