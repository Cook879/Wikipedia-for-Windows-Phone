//
// HistoryResultUserControl.xaml.cpp
// Implementation of the HistoryResultUserControl class
//

#include "pch.h"
#include "HistoryResultUserControl.xaml.h"
#include "HelperFunctions.h"

using namespace Wikipedia;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Web::Http;

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

HistoryResultUserControl::HistoryResultUserControl()
{
	InitializeComponent();
}

HistoryResultUserControl::HistoryResultUserControl(JsonObject^ revision, Article^ article, HistoryPage^ contentDialog) {
	InitializeComponent();

	userTextBlock->Text = revision->GetNamedString("user");
	timestampTextBlock->Text = revision->GetNamedString("timestamp");
	commentTextBlock->Text = revision->GetNamedString("comment");

	this->contentDialog = contentDialog;
	this->article = article;
}

void HistoryResultUserControl::HistoryResultUserControl_Clicked() {
	/*	HttpClient^ httpClient = HelperFunctions::CreateHttpClient();

	Uri^ uri = ref new Uri("http://" + article->GetLang() + ".wikipedia.org/w/api.php?action=mobileview&format=json&page=" + article->GetTitle() + "&sections=all");
	//std::wstring s(uri->ToString()->Data());
	//std::wstring stemp = std::wstring(s.begin(), s.end());
	//LPCWSTR sw = stemp.c_str();
	//OutputDebugString(sw);

	create_task(httpClient->GetAsync(uri)).then([=](HttpResponseMessage^ response) {
		response->EnsureSuccessStatusCode();

		//std::wstring s(response->ToString()->Data());
		//std::wstring stemp = std::wstring(s.begin(), s.end());
		//LPCWSTR sw = stemp.c_str();
		//OutputDebugString(sw);

		return create_task(response->Content->ReadAsStringAsync());
	}).then([=](String^ responseString) {
		//std::wstring s(responseString->Data());
		//std::wstring stemp = std::wstring(s.begin(), s.end());
		//LPCWSTR sw = stemp.c_str();
		//OutputDebugString(sw);

		JsonObject^ json = JsonValue::Parse(responseString)->GetObject();
		if (json->HasKey("mobileview")) {
			json = json->GetNamedObject("mobileview");
			if (json->HasKey("sections")) {
				JsonArray^ sections = json->GetNamedArray("sections");

				int arraySize = sections->Size;
				String^ content = ref new String();

				// Section 0
				IJsonValue^ sectionJson = sections->GetAt(0);
				if (sectionJson->ValueType == JsonValueType::Object) {
					JsonObject^ sectionObj = sectionJson->GetObject();
					content += sectionObj->GetNamedString("text");

					// Should really be language specific
					Section^ section = ref new Section(0, "Introduction", sectionObj->GetNamedString("text"));
					article->AddSection(section);
				}
				// Get the text for each section of the article
				for (int i = 1; i < arraySize; i++) {
					IJsonValue^ sectionJson = sections->GetAt(i);
					if (sectionJson->ValueType == JsonValueType::Object) {
						JsonObject^ sectionObj = sectionJson->GetObject();

						// Don't really want this line in the future
						String^ sectionText = ProcessSection(sectionObj->GetNamedString("text"), i);

						content += sectionText;

						Section^ section = ref new Section(i, sectionObj->GetNamedString("line"), sectionText);
						article->AddSection(section);
					}
				}
				content = ProcessContent(content);

				//std::wstring s(content->Data());
				//std::wstring stemp = std::wstring(s.begin(), s.end());
				//LPCWSTR sw = stemp.c_str();
				//OutputDebugString(sw);

				WebViewControl->NavigateToString(content);
				article->SetContent(content);
				ObtainLanguageData(article);
			}
			else {
				HelperFunctions::ErrorMessage("Sorry - an error has occured.");
			}
		}
		else {
			HelperFunctions::ErrorMessage("This page doesn't exist. Create it?");
		}

	}).then([=](task<void> prevTask) {
		try {
			prevTask.get();
		}
		catch (Exception ^ex) {
			return;
		}
	});


	contentDialog->Hide();*/
}
