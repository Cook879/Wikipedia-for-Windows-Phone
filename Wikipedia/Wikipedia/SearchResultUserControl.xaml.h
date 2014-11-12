//
// SearchResultUserControl.xaml.h
// Declaration of the SearchResultUserControl class
//

#pragma once

#include "SearchResultUserControl.g.h"
#include "SearchContentDialog.xaml.h"

namespace Wikipedia
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SearchResultUserControl sealed
	{
	public:
		SearchResultUserControl();
		SearchResultUserControl(Platform::String^ title, Platform::String^ lang, Wikipedia::SearchContentDialog^ contentDialog);

	private:
		Wikipedia::Article^ article;
		Wikipedia::SearchContentDialog^ contentDialog;

		void SearchResultUserControl_Clicked();
	};
}
