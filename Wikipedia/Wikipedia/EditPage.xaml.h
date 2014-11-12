//
// EditPage.xaml.h
// Declaration of the EditPage class
//

#pragma once

#include "EditPage.g.h"
#include "Article.h"
#include "MainPage.xaml.h"

namespace Wikipedia
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class EditPage sealed
	{
	public:
		EditPage();

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		Platform::String^ token;
		Windows::Web::Http::HttpClient^ httpClient;
		Wikipedia::Article^ article;
		Windows::ApplicationModel::Resources::ResourceLoader^ loader;
		Wikipedia::MainPage^ mainPage;
		int section;

		void SaveAppBarButton_Click();
		void PreviewAppBarButton_Click();
		void CancelAppBarButton_Click();
		void GoBack(Windows::UI::Popups::IUICommand^ command);
		void DoNothing(Windows::UI::Popups::IUICommand^ command);
		Wikipedia::Edit^ CreateEdit(void);
	};
}
