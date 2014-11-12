//
// SavePage.xaml.h
// Declaration of the SavePage class
//

#pragma once

#include "SavePage.g.h"
#include "Article.h"

namespace Wikipedia
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SavePage sealed
	{
	public:
		SavePage();

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		Wikipedia::Edit^ edit;
		Platform::String^ captchaId;

		void SaveButton_Click(void);
		void SubmitButton_Click(void);
	};
}
