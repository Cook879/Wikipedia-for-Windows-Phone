//
// SearchContentDialog.xaml.cpp
// Implementation of the SearchContentDialog class
//

#include "pch.h"
#include "SearchContentDialog.xaml.h"
#include "HelperFunctions.h"
#include "SearchResultUserControl.xaml.h"

using namespace Wikipedia;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Networking::Connectivity;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Web::Http;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=390556

SearchContentDialog::SearchContentDialog()
{
	InitializeComponent();
}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.
/// This parameter is typically used to configure the page.</param>
SearchContentDialog::SearchContentDialog(String^ lang, MainPage^ page)
{
	InitializeComponent();

	this->lang = lang;
	this->page = page;

	httpClient = HelperFunctions::CreateHttpClient();

	resultsPerPage = 10;
}

// Called when the search box is used
void SearchContentDialog::SearchBox_TextChanged() {
	String^ searchString = SearchBox->Text;

	if (!searchString->IsEmpty()) {
		Uri^ uri = ref new Uri("https://" + lang + ".wikipedia.org/w/api.php?action=query&list=search&format=json&srwhat=text&srlimit=" + resultsPerPage + "&srsearch=" + searchString);
		GetSearchResults(uri);
	}
	else {
		ResultsPanel->Children->Clear();
	}
}

// Called when more button is clicked
void SearchContentDialog::MoreSearchResults() {
	if (offset > -1) {
		String^ searchString = SearchBox->Text;

		Uri^ uri = ref new Uri("https://" + lang + ".wikipedia.org/w/api.php?action=query&list=search&format=json&srwhat=text&srlimit=" + resultsPerPage + "&sroffset=" + offset + "&srsearch=" + searchString);
		GetSearchResults(uri);
	}
}

// Requests from the server, processes results, displays them
void SearchContentDialog::GetSearchResults(Uri^ uri) {
	auto loader = ref new Windows::ApplicationModel::Resources::ResourceLoader();

	ConnectionProfile^ profile = NetworkInformation::GetInternetConnectionProfile();

	if (profile != nullptr)	{
		if (profile->NetworkAdapter != nullptr) {
			try {
				create_task(httpClient->GetAsync(uri)).then([=](HttpResponseMessage^ response) {
					response->EnsureSuccessStatusCode();

					return create_task(response->Content->ReadAsStringAsync());
				}).then([=](String^ responseString) {
					ResultsPanel->Children->Clear();

					JsonObject^ json = JsonValue::Parse(responseString)->GetObject();

					if (json->HasKey("query")) {
						JsonObject^ jsonQuery = json->GetNamedObject("query");

						if (jsonQuery->HasKey("searchinfo")) {
							JsonObject^ info = jsonQuery->GetNamedObject("searchinfo");

							totalHits = info->GetNamedNumber("totalhits");
							if (totalHits > 0) {

								if (jsonQuery->HasKey("search")) {
									JsonArray^ results = jsonQuery->GetNamedArray("search");
									ResultsPanel->Children->Clear();
									for (int i = 0; i < results->Size; i++) {
										JsonObject^ resultData = results->GetObjectAt(i);
										SearchResultUserControl^ sruc = ref new SearchResultUserControl(resultData->GetNamedString("title"), lang, this);
										ResultsPanel->Children->Append(sruc);
									}

									if (json->HasKey("query-continue")) {
										JsonObject^ queryContinue = json->GetNamedObject("query-continue")->GetNamedObject("search");
										offset = queryContinue->GetNamedNumber("sroffset");
										ContinueButton->IsEnabled = true;
									}
									else {
										offset = -1;
										ContinueButton->IsEnabled = false;
									}
								}
								else {
									HelperFunctions::ErrorMessage(loader->GetString("standardError"));
								}
							}
							else {
								ResultsPanel->Children->Clear();
								TextBlock^ tb = ref new TextBlock();
								tb->Text = loader->GetString("noResults");
								ResultsPanel->Children->Append(tb);
							}
						}
						else {
							HelperFunctions::ErrorMessage(loader->GetString("standardError"));
							ResultsPanel->Children->Clear();
						}
					}
					else {
						HelperFunctions::ErrorMessage(loader->GetString("standardError"));
						ResultsPanel->Children->Clear();
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
				ResultsPanel->Children->Clear();
			}
		}
		else {
			HelperFunctions::ErrorMessage(loader->GetString("noInternet"));
		}
	}
	else {
		HelperFunctions::ErrorMessage(loader->GetString("noInternet"));
	}
}

Wikipedia::MainPage^ SearchContentDialog::GetPage() {
	return page;
}