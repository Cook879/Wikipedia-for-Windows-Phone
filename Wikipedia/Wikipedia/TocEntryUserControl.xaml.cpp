//
// TocEntryUserControl.xaml.cpp
// Implementation of the TocEntryUserControl class
//

#include "pch.h"
#include "TocEntryUserControl.xaml.h"

using namespace Wikipedia;

using namespace std;
using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
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

TocEntryUserControl::TocEntryUserControl()
{
	InitializeComponent();
}

TocEntryUserControl::TocEntryUserControl(Section^ section, WebView^ WebViewControl, Wikipedia::TocContentDialog^ contentDialog) {
	InitializeComponent();

	textBlock->Text = section->line;

	this->section = section;

	this->WebViewControl = WebViewControl;
	this->contentDialog = contentDialog;
}

void TocEntryUserControl::TocEntryUserControl_Clicked() {
	contentDialog->GetContent();

	Vector<String^>^ param = ref new Vector<String^>();
	param->Append("sectionLink" + section->id);

	if(section->parent != nullptr)
		OpenSection(section->parent);

	create_task(WebViewControl->InvokeScriptAsync("anchorNew", param));
	contentDialog->Hide();
}

void TocEntryUserControl::OpenSection(Section^ section) {
	//create_task(WebViewControl->InvokeScriptAsync("refresh", nullptr));

	//WebViewControl->NavigateToString(contentDialog->GetContent());

	//OutputDebugString(("OpenSection" + section->id + "")->Data());

	Section^ parent = section;

	while (parent->parent != nullptr) {
		OpenSection(parent->parent);
		parent = parent->parent;
	}
	Vector<String^>^ param = ref new Vector<String^>();
	param->Append("section" + section->id);

	create_task(WebViewControl->InvokeScriptAsync("openSection", param));
}