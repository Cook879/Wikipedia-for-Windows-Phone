//
// TocContetDialog.xaml.h
// Declaration of the TocContetDialog class
//

#pragma once

#include "TocContentDialog.g.h"
#include "Article.h"

namespace Wikipedia
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class TocContentDialog sealed
	{
	public:
		TocContentDialog();
		TocContentDialog(Wikipedia::Article^ article, Windows::UI::Xaml::Controls::WebView^ WebViewControl);
		Platform::String^ GetContent();

	private:
		Wikipedia::Article^ article;
		Windows::UI::Xaml::Controls::WebView^ WebViewControl;
	};

}

