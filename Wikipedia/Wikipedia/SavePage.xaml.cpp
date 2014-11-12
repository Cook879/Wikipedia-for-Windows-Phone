//
// SavePage.xaml.cpp
// Implementation of the SavePage class
//

#include "pch.h"
#include "SavePage.xaml.h"
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
using namespace Windows::UI::Popups;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Web::Http;

static Windows::UI::Xaml::Interop::TypeName contentPageType = { "Wikipedia.MainPage", Windows::UI::Xaml::Interop::TypeKind::Metadata };

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=390556

SavePage::SavePage()
{
	InitializeComponent();

}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.
/// This parameter is typically used to configure the page.</param>
void SavePage::OnNavigatedTo(NavigationEventArgs^ e)
{
	edit = (Edit^)e->Parameter;
}

void SavePage::SaveButton_Click() {
	auto loader = ref new Windows::ApplicationModel::Resources::ResourceLoader();

	ConnectionProfile^ profile = NetworkInformation::GetInternetConnectionProfile();
	
	if (profile != nullptr)	{
		if (profile->NetworkAdapter != nullptr) {
			
			try {
				Uri^ uri = ref new Uri("https://" + edit->GetArticle()->GetLang() + ".wikipedia.org/w/api.php?format=json&action=edit");

				Map<String^, String^>^ properties = ref new Map<String^, String^>();
				properties->Insert("title", edit->GetArticle()->GetTitle());
				properties->Insert("text", edit->GetText());
				properties->Insert("summary", SummaryBox->Text);
				if (MinorEditBox->IsChecked->Value == true){
					properties->Insert("minor", "");
				}
				if (edit->GetSectionId() != 0)
					properties->Insert("section", ""+edit->GetSectionId()+"");

				properties->Insert("token", edit->GetToken());

				HttpFormUrlEncodedContent^ propertiesEncoded = ref new HttpFormUrlEncodedContent(properties);
				
				//DEBUG
				OutputDebugString(L"\nAAAAAAA\n");
				std::wstring s(propertiesEncoded->ToString()->Data());
				std::wstring stemp = std::wstring(s.begin(), s.end());
				LPCWSTR sw = stemp.c_str();
				OutputDebugString(sw);
				OutputDebugString(L"\n");

				create_task(edit->GetHttpClient()->PostAsync(uri, propertiesEncoded)).then([=](HttpResponseMessage^ response) {
					response->EnsureSuccessStatusCode();

					return create_task(response->Content->ReadAsStringAsync());
				}).then([=](String^ responseString) {

					//DEBUG
					OutputDebugString(L"\nBBBBBB\n");
					std::wstring s(responseString->Data());
					std::wstring stemp = std::wstring(s.begin(), s.end());
					LPCWSTR sw = stemp.c_str();
					OutputDebugString(sw);
					OutputDebugString(L"\n");

					JsonObject^ json = JsonValue::Parse(responseString)->GetObject();
					if (json->HasKey("edit")){
						json = json->GetNamedObject("edit");

						if (json->HasKey("result")){
							String^ result = json->GetNamedString("result");

							if (result == "Success"){
								HelperFunctions::ErrorMessage(loader->GetString("editSuccess"));

								Article^ oldArticle = edit->GetArticle();
								Article^ newArticle = ref new Article(oldArticle->GetTitle(), oldArticle->GetLang());

								this->Frame->Navigate(contentPageType, newArticle);
							}

							else if (json->HasKey("captcha")) {
								JsonObject^ captcha = json->GetNamedObject("captcha");
								captchaId = captcha->GetNamedString("id");
								String^ url = "https://en.wikipedia.org" + captcha->GetNamedString("url");
								Uri^ imageUri = ref new Uri(url);
								BitmapImage^ bmp = ref new BitmapImage(imageUri);

								Image^ img = ref new Image();
								img->Source = bmp;
								Captcha->Children->Append(img);

								CAPTCHABox->Visibility = Windows::UI::Xaml::Visibility::Visible;
								SubmitButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
								SaveButton->IsEnabled = false;
							}
							else {
								HelperFunctions::ErrorMessage(loader->GetString("standardError"));
							}
						} else if (json->HasKey("error")) {
							json = json->GetNamedObject("error");
							HelperFunctions::ErrorMessage(json->GetNamedString("info"));
						} else {
							HelperFunctions::ErrorMessage(loader->GetString("standardError"));
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

void SavePage::SubmitButton_Click() {
	auto loader = ref new Windows::ApplicationModel::Resources::ResourceLoader();

	Uri^ uri = ref new Uri("https://" + edit->GetArticle()->GetLang() + ".wikipedia.org/w/api.php?format=json&action=edit");

	Map<String^, String^>^ properties = ref new Map<String^, String^>();
	properties->Insert("title", edit->GetArticle()->GetTitle());
	properties->Insert("text", edit->GetText());
	properties->Insert("summary", SummaryBox->Text);
	if (MinorEditBox->IsChecked)
		properties->Insert("minor", "");

	properties->Insert("token", edit->GetToken());

	properties->Insert("captchaid", captchaId);
	properties->Insert("captchaword", CAPTCHABox->Text);


	HttpFormUrlEncodedContent^ propertiesEncoded = ref new HttpFormUrlEncodedContent(properties);
	//DEBUG
	OutputDebugString(L"\nCCCCCCCCCCC\n");
	std::wstring s(propertiesEncoded->ToString()->Data());
	std::wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR sw = stemp.c_str();
	OutputDebugString(sw);
	OutputDebugString(L"\n");

	create_task(edit->GetHttpClient()->PostAsync(uri, propertiesEncoded)).then([=](HttpResponseMessage^ response) {
		response->EnsureSuccessStatusCode();

		return create_task(response->Content->ReadAsStringAsync());
	}).then([=](String^ responseString) {

		//DEBUG
		OutputDebugString(L"\nDDDDDDDD\n");
		std::wstring s(responseString->Data());
		std::wstring stemp = std::wstring(s.begin(), s.end());
		LPCWSTR sw = stemp.c_str();
		OutputDebugString(sw);
		OutputDebugString(L"\n");

		JsonObject^ json = JsonValue::Parse(responseString)->GetObject();
		if (json->HasKey("edit")){
			json = json->GetNamedObject("edit");

			if (json->HasKey("result")){
				String^ result = json->GetNamedString("result");

				if (result == "Success"){
					HelperFunctions::ErrorMessage(loader->GetString("editSuccess"));

					Article^ oldArticle = edit->GetArticle();
					Article^ newArticle = ref new Article(oldArticle->GetTitle(), oldArticle->GetLang());

					this->Frame->Navigate(contentPageType, newArticle);
				}
				else if (json->HasKey("captcha")) {
					JsonObject^ captcha = json->GetNamedObject("captcha");
					captchaId = captcha->GetNamedString("id");
					String^ url = "https://en.wikipedia.org" + captcha->GetNamedString("url");
					Uri^ imageUri = ref new Uri(url);
					BitmapImage^ bmp = ref new BitmapImage(imageUri);

					Captcha->Children->Clear();
					Image^ img = ref new Image();
					img->Source = bmp;
					Captcha->Children->Append(img);

					CAPTCHABox->Visibility = Windows::UI::Xaml::Visibility::Visible;
					SubmitButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
					SaveButton->IsEnabled = false;
				}
				else {
					HelperFunctions::ErrorMessage(loader->GetString("standardError"));
				}
			}
			else if (json->HasKey("error")) {
				json = json->GetNamedObject("error");
				HelperFunctions::ErrorMessage(json->GetNamedString("info"));
			}
			else {
				HelperFunctions::ErrorMessage(loader->GetString("standardError"));
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