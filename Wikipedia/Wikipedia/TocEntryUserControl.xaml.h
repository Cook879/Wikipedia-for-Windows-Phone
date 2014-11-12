//
// TocEntryUserControl.xaml.h
// Declaration of the TocEntryUserControl class
//

#pragma once

#include "TocEntryUserControl.g.h"
#include "TocContentDialog.xaml.h"

namespace Wikipedia
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class TocEntryUserControl sealed
	{
	public:
		TocEntryUserControl();
		TocEntryUserControl(Wikipedia::Section^ section, Windows::UI::Xaml::Controls::WebView^ WebViewControl, Wikipedia::TocContentDialog^ contentDialog);
	private:
		Windows::UI::Xaml::Controls::WebView^ WebViewControl;
		Wikipedia::TocContentDialog^ contentDialog;
		Wikipedia::Section^ section;

		void TocEntryUserControl_Clicked();
		void OpenSection(Wikipedia::Section^ parent);
	};
}
