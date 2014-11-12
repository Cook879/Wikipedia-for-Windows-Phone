//
// LanguagesContentDialog.xaml.h
// Declaration of the LanguagesContentDialog class
//

#pragma once

#include "LanguagesContentDialog.g.h"
#include "Article.h"
#include "MainPage.xaml.h"

namespace Wikipedia
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class LanguagesContentDialog sealed
	{
	public:
		LanguagesContentDialog();
		LanguagesContentDialog(Wikipedia::Article^ article, Wikipedia::MainPage^ page);
		Wikipedia::MainPage^ GetPage();
		Wikipedia::Article^ GetArticle();

	private:
		Wikipedia::MainPage^ page;
		Wikipedia::Article^ article;
	};
}
