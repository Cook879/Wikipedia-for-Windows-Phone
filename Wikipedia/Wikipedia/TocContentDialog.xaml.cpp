#include "pch.h"
#include "TocContentDialog.xaml.h"
#include "TocEntryUserControl.xaml.h"

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

TocContentDialog::TocContentDialog()
{
	InitializeComponent();
}

TocContentDialog::TocContentDialog(Article^ article, WebView^ WebViewControl)
{
	InitializeComponent();

	this->article = article;
	this->WebViewControl = WebViewControl;
	IVector<Section^>^ sections = article->GetSections();

	for (int i = 0; i < sections->Size; i++) {
		TocEntryUserControl^ teuc = ref new TocEntryUserControl(sections->GetAt(i), WebViewControl, this);
		TocStackPanel->Children->Append(teuc);
	}

}

String^ TocContentDialog::GetContent() {
	return article->GetContent();
}