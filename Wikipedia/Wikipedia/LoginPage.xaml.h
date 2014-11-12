//
// LoginPage.xaml.h
// Declaration of the LoginPage class
//

#pragma once

#include "LoginPage.g.h"
#include "MainPage.xaml.h"

namespace Wikipedia
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class LoginPage sealed
	{
	public:
		LoginPage();
		LoginPage(Wikipedia::MainPage^ mainPage);

	private:
		void LoginButton_Click();
		void CancelButton_Click();

		Wikipedia::MainPage^ mainPage;
	};
}
