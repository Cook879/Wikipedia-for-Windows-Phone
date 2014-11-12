﻿//
// EditPage.xaml.cpp
// Implementation of the EditPage class
//

#include "pch.h"
#include "EditPage.xaml.h"
#include "Article.h"
#include "HelperFunctions.h"

using namespace Wikipedia;

using namespace concurrency;
using namespace Platform;
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
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Web::Http;

static TypeName savePageType = { "Wikipedia.SavePage", TypeKind::Metadata };
static TypeName previewPageType = { "Wikipedia.PreviewPage", TypeKind::Metadata };
static TypeName contentPageType = { "Wikipedia.MainPage", TypeKind::Metadata };

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=390556

EditPage::EditPage()
{
	InitializeComponent();
	loader = ref new Windows::ApplicationModel::Resources::ResourceLoader();
}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.
/// This parameter is typically used to configure the page.</param>
void EditPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	IVector<Object^>^ params = (IVector<Object^>^)e->Parameter;
	article = (Article^)params->GetAt(0);
	section = (int)params->GetAt(1);
	mainPage = (MainPage^)params->GetAt(2);

	ConnectionProfile^ profile = NetworkInformation::GetInternetConnectionProfile();

	if (profile != nullptr)	{
		if (profile->NetworkAdapter != nullptr)	{
			try {
				httpClient = HelperFunctions::CreateHttpClient();

				Uri^ uri;
				if (section == 0)
					uri = ref new Uri("https://" + article->GetLang() + ".wikipedia.org/w/api.php?format=json&action=query&titles=" + article->GetTitle() + "&prop=info|revisions&rvprop=content&intoken=edit");
				else
					uri = ref new Uri("https://" + article->GetLang() + ".wikipedia.org/w/api.php?format=json&action=query&titles=" + article->GetTitle() + "&prop=info|revisions&rvprop=content&intoken=edit&rvsection=" + section);
				
				create_task(httpClient->GetAsync(uri)).then([=](HttpResponseMessage^ response) {
					response->EnsureSuccessStatusCode();

					return create_task(response->Content->ReadAsStringAsync());
				}).then([=](String^ responseString) {
					JsonObject^ json = JsonValue::Parse(responseString)->GetObject();

					// Navigate through the objects
					if (json->HasKey("query")){
						JsonObject^ query = json->GetObject()->GetNamedObject("query");

						if (query->HasKey("pages")) {
							JsonObject^ pages = query->GetNamedObject("pages");

							/* Start hack-y way to get the json for the page as it doesn't like GetObject() */
							std::wstring pageIdString(pages->Stringify()->Data());
							int endPos = pageIdString.find(':');
							pageIdString = pageIdString.erase(0, endPos + 1);
							pageIdString = pageIdString.erase(pageIdString.length() - 1, 1);
							JsonValue^ pageId = JsonValue::Parse(ref new String(pageIdString.c_str()));
							/* End hack-y code */

							JsonObject^ pageData = pageId->GetObject();

							if (pageData->HasKey("title") && pageData->HasKey("edittoken") && pageData->HasKey("revisions")) {
								Title->Text = L"Editing " + pageData->GetNamedString("title");
								token = pageData->GetNamedString("edittoken");

								JsonArray^ contentArray = pageData->GetNamedArray("revisions");
								JsonObject^ contentObject = contentArray->GetObjectAt(0);
								EditBox->Text = contentObject->GetNamedString("*");
							}
							else {
								HelperFunctions::ErrorMessage(loader->GetString("standardError"));
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

void EditPage::SaveAppBarButton_Click() {
	Edit^ e = CreateEdit();
	this->Frame->Navigate(savePageType, e);
}

void EditPage::PreviewAppBarButton_Click() {
	Edit^ e = CreateEdit();
	this->Frame->Navigate(previewPageType, e);
}

void EditPage::CancelAppBarButton_Click() {
	MessageDialog^ md = ref new MessageDialog(loader->GetString("cancelEditing"));
	UICommand^ yesCommand = ref new UICommand(loader->GetString("yes"), ref new UICommandInvokedHandler(this, &EditPage::GoBack));
	UICommand^ noCommand = ref new UICommand(loader->GetString("no"), ref new UICommandInvokedHandler(&HelperFunctions::DoNothing));

	md->Commands->Append(yesCommand);
	md->Commands->Append(noCommand);

	md->DefaultCommandIndex = 0;
	md->CancelCommandIndex = 1;
	md->ShowAsync();
}


Edit^ EditPage::CreateEdit() {
	return  ref new Edit(article, section, EditBox->Text, token, httpClient);
}

void EditPage::GoBack(IUICommand^ command){
	this->Frame->Navigate(contentPageType, article);
}
