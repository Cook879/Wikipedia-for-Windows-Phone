//
// SearchResultUserControl.xaml.cpp
// Implementation of the SearchResultUserControl class
//

#include "pch.h"
#include "SearchResultUserControl.xaml.h"
#include "Article.h"
#include "SearchContentDialog.xaml.h"

using namespace Wikipedia;

using namespace std;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

SearchResultUserControl::SearchResultUserControl()
{
	InitializeComponent();
}

SearchResultUserControl::SearchResultUserControl(String^ title, String^ lang, SearchContentDialog^ contentDialog)
{
	InitializeComponent();

	article = ref new Article(title, lang);

	titleTextBlock->Text = title;
	this->contentDialog = contentDialog;
}

void SearchResultUserControl::SearchResultUserControl_Clicked(){
	contentDialog->GetPage()->ChangeArticle(article);
	contentDialog->Hide();
}

