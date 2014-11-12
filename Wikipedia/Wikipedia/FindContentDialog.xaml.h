//
// FindContentDialog.xaml.h
// Declaration of the FindContentDialog class
//

#pragma once

#include "FindContentDialog.g.h"
#include "MainPage.xaml.h"

namespace Wikipedia
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class FindContentDialog sealed
	{
	public:
		FindContentDialog();
		FindContentDialog(Wikipedia::MainPage^ page);
	private:
		Wikipedia::MainPage^ page;
			
		void ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args);
		void ContentDialog_SecondaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args);
	};
}
