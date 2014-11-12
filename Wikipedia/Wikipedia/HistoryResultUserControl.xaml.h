//
// HistoryResultUserControl.xaml.h
// Declaration of the HistoryResultUserControl class
//

#pragma once

#include "HistoryResultUserControl.g.h"
#include "HistoryPage.xaml.h"

namespace Wikipedia
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class HistoryResultUserControl sealed
	{
	public:
		HistoryResultUserControl();
		HistoryResultUserControl(Windows::Data::Json::JsonObject^ revision, Wikipedia::Article^ article, Wikipedia::HistoryPage^ contentDialog);

	private:
		void HistoryResultUserControl_Clicked();
		Wikipedia::Article^ article;
		Wikipedia::HistoryPage^ contentDialog;
	};
}
