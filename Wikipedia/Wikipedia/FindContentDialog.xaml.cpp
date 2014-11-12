//
// FindContentDialog.xaml.cpp
// Implementation of the FindContentDialog class
//

#include "pch.h"
#include "FindContentDialog.xaml.h"

using namespace Wikipedia;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Content Dialog item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

Wikipedia::FindContentDialog::FindContentDialog()
{
	InitializeComponent();
}

Wikipedia::FindContentDialog::FindContentDialog(MainPage^ page)
{
	InitializeComponent();

	this->page = page;
}

void Wikipedia::FindContentDialog::ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args)
{
	page->FindInPage(searchField->Text);
	this->Hide();
}

void Wikipedia::FindContentDialog::ContentDialog_SecondaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args)
{
	this->Hide();
}
