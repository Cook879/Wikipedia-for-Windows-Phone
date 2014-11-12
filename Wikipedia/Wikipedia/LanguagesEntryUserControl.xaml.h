//
// LanguagesEntryUserControl.xaml.h
// Declaration of the LanguagesEntryUserControl class
//

#pragma once

#include "LanguagesEntryUserControl.g.h"
#include "LanguagesContentDialog.xaml.h"

namespace Wikipedia
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class LanguagesEntryUserControl sealed
	{
	public:
		LanguagesEntryUserControl();
		LanguagesEntryUserControl(Wikipedia::Article^ article, Wikipedia::LanguagesContentDialog^ contentDialog);

	private:
		Wikipedia::LanguagesContentDialog^ contentDialog;
		Wikipedia::Article^ article;

		void LanguagesEntryUserControl_Clicked();
	};
}
