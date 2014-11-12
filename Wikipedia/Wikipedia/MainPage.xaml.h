//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "Article.h"

namespace Wikipedia
{
	public ref class MainPage sealed
	{
		//Windows::Foundation::Uri^ GetHomeUri() const
		//{
			// TODO: Replace with your URL here.
		//	return ref new Windows::Foundation::Uri(L"ms-appx-web:///Html//test.htm");
		//}

	public:
		MainPage();
		void ChangeArticle(Article^ article);
		void ShowLoggedIn(Platform::String^ username);
	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
		virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
		virtual void OnActivated(Windows::ApplicationModel::Activation::IActivatedEventArgs^ e) override;

	private:
		// Default stuff
		Windows::Foundation::EventRegistrationToken _backPressedToken;

		void Browser_NavigationCompleted(Platform::Object^ sender, Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs^ e);
		void MainPage_BackPressed(Platform::Object^ sender, Windows::Phone::UI::Input::BackPressedEventArgs^ e);

		// My stuff
		Wikipedia::Article^ article;
		Windows::Web::Http::HttpClient^ httpClient;
		Windows::ApplicationModel::Resources::ResourceLoader^ loader;
		Platform::Collections::Vector<Platform::String^>^ history;
		int historyPos;

		void LoadContent(void);
		void WebViewControl_NavigationStarting(Windows::UI::Xaml::Controls::WebView^ sender, Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs^ args);
		void MainPage::NavigateToString(Platform::String^ content);

		void TocAppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void EditAppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void SearchAppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void HomeAppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void RandomAppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void LanguageAppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void LoginAppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void LogoutAppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void AboutAppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		
		void ObtainLanguageData(Wikipedia::Article^ article);
		
		void DoNothing();
		void ExternalLink();

		void CheckLoggedIn();

		void EditSection(int section);

		void LogIn(Windows::Security::Credentials::PasswordCredential^ cred);
	};
}
