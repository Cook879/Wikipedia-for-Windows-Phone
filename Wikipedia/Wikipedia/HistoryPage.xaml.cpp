//
// HistoryPage.xaml.cpp
// Implementation of the HistoryPage class
//

#include "pch.h"
#include "HistoryPage.xaml.h"
#include "HelperFunctions.h"
#include "HistoryResultUserControl.xaml.h"

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

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=390556

HistoryPage::HistoryPage()
{
	InitializeComponent();
}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.
/// This parameter is typically used to configure the page.</param>
HistoryPage::HistoryPage(Article^ article, MainPage^ page)
{
	InitializeComponent();

	this->article = article;
	this->page = page;

	try {
		HttpClient^ httpClient = HelperFunctions::CreateHttpClient();

		Uri^ uri = ref new Uri("http://" + article->GetLang() + ".wikipedia.org/w/api.php?action=query&format=json&prop=revisions&titles=" + article->GetTitle() + "&rvlimit=6");

		create_task(httpClient->GetAsync(uri)).then([=](HttpResponseMessage^ response) {
			response->EnsureSuccessStatusCode();

			return create_task(response->Content->ReadAsStringAsync());
		}).then([=](String^ responseString) {
			std::wstring s(responseString->Data());
			std::wstring stemp = std::wstring(s.begin(), s.end());
			LPCWSTR sw = stemp.c_str();
			OutputDebugString(sw);

			ResultsPanel->Children->Clear();

			// Error handling
			JsonObject^ json = JsonValue::Parse(responseString)->GetObject();
			if (json->HasKey("query")) {
				JsonObject^ query = json->GetNamedObject("query");

				if (query->HasKey("pages")) {
					query = query->GetNamedObject("pages");
					/* Start hack-y way to get the json for the page as it doesn't like GetObject() */
					std::wstring pageIdString(query->Stringify()->Data());
					int endPos = pageIdString.find(':');
					pageIdString = pageIdString.erase(0, endPos + 1);
					pageIdString = pageIdString.erase(pageIdString.length() - 1, 1);
					JsonValue^ pageId = JsonValue::Parse(ref new String(pageIdString.c_str()));
					/* End hack-y code */
					JsonObject^ pageData = pageId->GetObject();

					if (pageData->HasKey("revisions")) {
						JsonArray^ revisions = pageData->GetNamedArray("revisions");
						for (int i = 0; i < revisions->Size; i++) {
							HistoryResultUserControl^ hruc = ref new HistoryResultUserControl(revisions->GetObjectAt(i), article, this);
							ResultsPanel->Children->Append(hruc);
						}

						JsonObject^ queryContinue = json->GetNamedObject("query-continue")->GetNamedObject("revisions");
						rvcontinue = queryContinue->GetNamedNumber("rvcontinue");
					}
					else {
						HelperFunctions::ErrorMessage("Sorry - an error has occured.");
					}
				}
				else {
					HelperFunctions::ErrorMessage("Sorry - an error has occured.");
				}
			}
			else {
				HelperFunctions::ErrorMessage("Sorry - an error has occured.");
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
		HelperFunctions::ErrorMessage("Sorry - an error has occured.");
	}
}

MainPage^ HistoryPage::GetPage() {
	return page;
}

/*void HistoryPage::MoreResults() {
	try {
		HttpClient^ httpClient = HelperFunctions::CreateHttpClient();

		Uri^ uri = ref new Uri("http://" + article->GetLang() + ".wikipedia.org/w/api.php?action=query&format=json&prop=revisions&titles=" + article->GetTitle() + "&rvlimit=6&rvcontinue=" + rvcontinue);

		create_task(httpClient->GetAsync(uri)).then([=](HttpResponseMessage^ response) {
			response->EnsureSuccessStatusCode();

			return create_task(response->Content->ReadAsStringAsync());
		}).then([=](String^ responseString) {
			std::wstring s(responseString->Data());
			std::wstring stemp = std::wstring(s.begin(), s.end());
			LPCWSTR sw = stemp.c_str();
			OutputDebugString(sw);

			ResultsPanel->Children->Clear();

			// Error handling
			JsonObject^ json = JsonValue::Parse(responseString)->GetObject();
			if (json->HasKey("query")) {
				JsonObject^ query = json->GetNamedObject("query");

				if (query->HasKey("pages")) {
					query = query->GetNamedObject("pages");
					/* Start hack-y way to get the json for the page as it doesn't like GetObject() *//*
					std::wstring pageIdString(query->Stringify()->Data());
					int endPos = pageIdString.find(':');
					pageIdString = pageIdString.erase(0, endPos + 1);
					pageIdString = pageIdString.erase(pageIdString.length() - 1, 1);
					JsonValue^ pageId = JsonValue::Parse(ref new String(pageIdString.c_str()));
					/* End hack-y code *//*
					JsonObject^ pageData = pageId->GetObject();

					if (pageData->HasKey("revisions")) {
						JsonArray^ revisions = pageData->GetNamedArray("revisions");
						for (int i = 0; i < revisions->Size; i++) {
							HistoryResultUserControl^ hruc = ref new HistoryResultUserControl(revisions->GetObjectAt(i), article, this);
							ResultsPanel->Children->Append(hruc);
						}

						JsonObject^ queryContinue = json->GetNamedObject("query-continue")->GetNamedObject("revisions");
						rvcontinue = queryContinue->GetNamedNumber("rvcontinue");
					}
					else {
						HelperFunctions::ErrorMessage("Sorry - an error has occured.");
					}
				}
				else {
					HelperFunctions::ErrorMessage("Sorry - an error has occured.");
				}
			}
			else {
				HelperFunctions::ErrorMessage("Sorry - an error has occured.");
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
		HelperFunctions::ErrorMessage("Sorry - an error has occured.");
	}
}*/