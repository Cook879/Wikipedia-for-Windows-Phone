//
// LanguagesContentDialog.xaml.cpp
// Implementation of the LanguagesContentDialog class
//

#include "pch.h"
#include "LanguagesContentDialog.xaml.h"
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

// The Content Dialog item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

Wikipedia::LanguagesContentDialog::LanguagesContentDialog()
{
	InitializeComponent();
}

Wikipedia::LanguagesContentDialog::LanguagesContentDialog(Article^ article, MainPage^ page) {
	InitializeComponent();

	this->article = article;
	this->page = page;

	IVector<Article^>^ languages = article->GetLanguages();
	
	for (int i = 0; i < languages->Size; i++) {
		LanguagesEntryUserControl^ leuc = ref new LanguagesEntryUserControl(languages->GetAt(i), this);
		LanguagesStackPanel->Children->Append(leuc);
	}
}
Wikipedia::MainPage^ LanguagesContentDialog::GetPage() {
	return page;
}
Wikipedia::Article^ LanguagesContentDialog::GetArticle() {
	return article;
}