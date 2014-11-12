//
// LanguagesEntryUserControl.xaml.cpp
// Implementation of the LanguagesEntryUserControl class
//

#include "pch.h"
#include "LanguagesEntryUserControl.xaml.h"

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

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

LanguagesEntryUserControl::LanguagesEntryUserControl()
{
	InitializeComponent();
}

LanguagesEntryUserControl::LanguagesEntryUserControl(Article^ article, LanguagesContentDialog^ contentDialog) {
	InitializeComponent();

	this->article = article;

	textBlock->Text = article->GetLang();

	this->contentDialog = contentDialog;
}

void LanguagesEntryUserControl::LanguagesEntryUserControl_Clicked() {
	contentDialog->GetPage()->ChangeArticle(article);
	contentDialog->Hide();
}