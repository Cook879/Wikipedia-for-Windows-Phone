//
// PreviewPage.xaml.cpp
// Implementation of the PreviewPage class
//

#include "pch.h"
#include "PreviewPage.xaml.h"
#include "Article.h"
#include "HelperFunctions.h"

using namespace Wikipedia;

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Networking::Connectivity;
using namespace Windows::Phone::UI::Input;
using namespace Windows::UI::Popups;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Web::Http;

static Windows::UI::Xaml::Interop::TypeName contentPageType = { "Wikipedia.MainPage", Windows::UI::Xaml::Interop::TypeKind::Metadata };
static Windows::UI::Xaml::Interop::TypeName savePageType = { "Wikipedia.SavePage", Windows::UI::Xaml::Interop::TypeKind::Metadata };
static Windows::UI::Xaml::Interop::TypeName editPageType = { "Wikipedia.EditPage", Windows::UI::Xaml::Interop::TypeKind::Metadata };

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=390556

PreviewPage::PreviewPage()
{
	InitializeComponent();
	loader = ref new Windows::ApplicationModel::Resources::ResourceLoader();
	_backPressedToken = HardwareButtons::BackPressed += ref new EventHandler<BackPressedEventArgs^>(this, &PreviewPage::PreviewPage_BackPressed);

	firstTime = true;
}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.
/// This parameter is typically used to configure the page.</param>
void PreviewPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	ConnectionProfile^ profile = NetworkInformation::GetInternetConnectionProfile();
	if (profile != nullptr)	{
		if (profile->NetworkAdapter != nullptr) {

			edit = (Edit^)e->Parameter;
			
			try {
				Uri^ uri = ref new Uri("https://" + edit->GetArticle()->GetLang() + ".wikipedia.org/w/api.php?format=json&action=parse");
			
				Map<String^, String^>^ properties = ref new Map<String^, String^>();
				properties->Insert("title", edit->GetArticle()->GetTitle());
				properties->Insert("text", edit->GetText());
				properties->Insert("preview", "mobileformat");

				HttpFormUrlEncodedContent^ propertiesEncoded = ref new HttpFormUrlEncodedContent(properties);

				create_task(edit->GetHttpClient()->PostAsync(uri, propertiesEncoded)).then([=](HttpResponseMessage^ response) {
					response->EnsureSuccessStatusCode();

					return create_task(response->Content->ReadAsStringAsync());
				}).then([=](String^ responseString) {
					try {
						JsonObject^ json = JsonValue::Parse(responseString)->GetObject();
						
						if (json->HasKey("parse")) {
							json = json->GetNamedObject("parse");
							
							if (json->HasKey("text")) {
								json = json->GetNamedObject("text");
								
								if (json->HasKey("*")) {
									text = json->GetNamedString("*");

									text = HelperFunctions::ProcessContent(text, edit->GetArticle()->GetTitle());
									text = HelperFunctions::ProcessImages(text);
									WebViewControl->NavigateToString(text);

								} else {
									HelperFunctions::ErrorMessage(loader->GetString("standardError"));
								}
							} else {
								HelperFunctions::ErrorMessage(loader->GetString("standardError"));
							}
						} else {
							HelperFunctions::ErrorMessage(loader->GetString("standardError"));
						}
					} catch (Exception^ e) {
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

// Intercept the URI and stop it from being followed
void PreviewPage::WebViewControl_ContentLoading(Object^ sender, WebViewNavigationStartingEventArgs^ args)
{
	// Seems like silly code - actually very important or page won't load for the first time!
	if (firstTime == false) {
		WebViewControl->Stop();
		HelperFunctions::ErrorMessage("You can't follow links off of the preview page!");
		firstTime = true;
		WebViewControl->NavigateToString(text);
	}
	else {
		firstTime = false;
	}
}

void PreviewPage::EditAppBarButton_Click() {
	this->Frame->Navigate(editPageType, edit);
}

void PreviewPage::CancelAppBarButton_Click() {
	MessageDialog^ md = ref new MessageDialog(loader->GetString("cancelEditing"));
	UICommand^ yesCommand = ref new UICommand(loader->GetString("yes"), ref new UICommandInvokedHandler(this, &PreviewPage::Cancel));
	UICommand^ noCommand = ref new UICommand(loader->GetString("no"), ref new UICommandInvokedHandler(&HelperFunctions::DoNothing));

	md->Commands->Append(yesCommand);
	md->Commands->Append(noCommand);

	md->DefaultCommandIndex = 0;
	md->CancelCommandIndex = 1;
	md->ShowAsync();
}

void PreviewPage::Cancel(IUICommand^ command){
	this->Frame->Navigate(contentPageType, edit->GetArticle());
}

void PreviewPage::SaveAppBarButton_Click() {
	this->Frame->Navigate(savePageType, edit);
}

void PreviewPage::PreviewPage_BackPressed(Object^ sender, BackPressedEventArgs^ e) {
	this->Frame->Navigate(editPageType, edit);
	e->Handled = true;
}

void PreviewPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	HardwareButtons::BackPressed -= _backPressedToken;
}