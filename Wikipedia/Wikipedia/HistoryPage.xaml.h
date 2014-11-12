//
// HistoryPage.xaml.h
// Declaration of the HistoryPage class
//

#pragma once

#include "HistoryPage.g.h"
#include "Article.h"
#include "MainPage.xaml.h"

namespace Wikipedia
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class HistoryPage sealed
	{
	public:
		HistoryPage();
		HistoryPage(Wikipedia::Article^ article, Wikipedia::MainPage^ page);
		Wikipedia::MainPage^ GetPage();

	private:
		void MoreResults();

		Wikipedia::Article^ article;
		Wikipedia::MainPage^ page;
		int rvcontinue;
	};
}
