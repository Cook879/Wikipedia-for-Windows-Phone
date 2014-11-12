//
// SearchContentDialog.xaml.h
// Declaration of the SearchContentDialog class
//

#pragma once

#include "SearchContentDialog.g.h"
#include "MainPage.xaml.h"

namespace Wikipedia
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SearchContentDialog sealed
	{
	public:
		SearchContentDialog();
		SearchContentDialog(Platform::String^ lang, Wikipedia::MainPage^ page);

		Wikipedia::MainPage^ GetPage();
	private:
		Windows::Web::Http::HttpClient^ httpClient;
		Platform::String^ lang;
		Wikipedia::MainPage^ page;
		int resultsPerPage;
		int totalHits;
		int offset;

		//Add parameters?
		void SearchBox_TextChanged();
		void MoreSearchResults();
		void GetSearchResults(Windows::Foundation::Uri^ uri);
	};
}
