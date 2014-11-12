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
	//TocEntryUserControl^ teuc = ref new TocEntryUserControl(sections->GetAt(i)->line, "sectionLink" + i, sections->GetAt(i)->level, WebViewControl, this);

	textBlock->Text = section->line;

	this->section = section;

	this->WebViewControl = WebViewControl;
	this->contentDialog = contentDialog;
}

void TocEntryUserControl::TocEntryUserControl_Clicked() {
	contentDialog->GetContent();

	Vector<String^>^ param = ref new Vector<String^>();
	param->Append("sectionLink" + section->id);

	//DEBUG
	//std::wstring s(("sectionLink" + section->id + "")->Data());
	//std::wstring stemp = std::wstring(s.begin(), s.end());
//LPCWSTR sw = stemp.c_str();
//OutputDebugString(sw);

	if(section->parent != nullptr)
		OpenSection(section);

	create_task(WebViewControl->InvokeScriptAsync("anchorNew", param));
	contentDialog->Hide();
}

void TocEntryUserControl::OpenSection(Section^ parent) {
	//create_task(WebViewControl->InvokeScriptAsync("refresh", nullptr));

	WebViewControl->NavigateToString(contentDialog->GetContent());

	while (parent->parent != nullptr) {
		OpenSection(parent->parent);
	}
	Vector<String^>^ param = ref new Vector<String^>();
	param->Append("section" + section->id);

	create_task(WebViewControl->InvokeScriptAsync("openSection", param));
}