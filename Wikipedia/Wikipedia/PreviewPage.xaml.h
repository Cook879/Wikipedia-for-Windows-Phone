//
// PreviewPage.xaml.h
// Declaration of the PreviewPage class
//

#pragma once

#include "PreviewPage.g.h"
#include "Article.h"

namespace Wikipedia
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PreviewPage sealed
	{
	public:
		PreviewPage();

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		Wikipedia::Edit^ edit;
		Windows::ApplicationModel::Resources::ResourceLoader^ loader;

		Windows::Foundation::EventRegistrationToken _backPressedToken;

		void Browser_NavigationCompleted(Platform::Object^ sender, Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs^ e);
		void PreviewPage_BackPressed(Platform::Object^ sender, Windows::Phone::UI::Input::BackPressedEventArgs^ e);
		//void WebViewControl_NavigationStarting(Windows::UI::Xaml::Controls::WebView^ sender, Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs^ args);
		void WebViewControl_ContentLoading(Platform::Object^ sender, Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs^ args);

		void EditAppBarButton_Click();
		void CancelAppBarButton_Click();
		void SaveAppBarButton_Click();
		void Cancel(Windows::UI::Popups::IUICommand^ command);

		bool firstTime;
		Platform::String^ text;
	};
}
