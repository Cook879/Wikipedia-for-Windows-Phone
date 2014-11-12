//
// Article.h
// Declaration of the Article class
// 
// The Article class stores information about an article on the wiki
//

#ifndef ARTICLE_H
#define ARTICLE_H

namespace Wikipedia {
	public ref class Section sealed {
	public:
		property int id;
		property Platform::String^ line;
		property Platform::String^ text;
		property int level;
		property Section^ parent;

		Section(int id, Platform::String^ line, Platform::String^ text, int level, Section^ parent);
	};

	public ref class Article sealed {
		public:
			// Constructors
			Article(Platform::String^ ns, Platform::String^ name, Platform::String^ lang);
			Article(Platform::String^ title, Platform::String^ lang);

			// Title stuff
			Platform::String^ GetTitle(void);
			Platform::String^ GetNamespace(void);
			Platform::String^ GetName(void);
			Platform::String^ GetLang(void);

			void SetTitle(Platform::String^ title);

			// Content stuff
			void SetContent(Platform::String^ content);
			Platform::String^ GetContent(void);
			bool HasContent(void);

			void AddSection(Wikipedia::Section^ section);
			Windows::Foundation::Collections::IVector<Wikipedia::Section^>^ GetSections();

			bool HasLanguages(void);
			void AddLanguage(Wikipedia::Article^ lang);
			Windows::Foundation::Collections::IVector<Wikipedia::Article^>^ GetLanguages();
			void FoundLanguages();
		private:
			// Required variables
			Platform::String^ ns;
			Platform::String^ name;
			Platform::String^ lang;

			// Variables required later
			Platform::String^ content;
			Windows::Foundation::Collections::IVector<Wikipedia::Section^>^ sections;
			Windows::Foundation::Collections::IVector<Wikipedia::Article^>^ languages;
			bool foundLanguages;
	};

	public ref class Edit sealed {
	public:
		Edit(Article^ article, int sectionId, Platform::String^ text, Platform::String^ token, Windows::Web::Http::HttpClient^ httpClient);
		Article^ GetArticle();
		int GetSectionId();
		Platform::String^ GetText();
		Platform::String^ GetToken();
		Windows::Web::Http::HttpClient^ GetHttpClient();
	private:
		Article^ article;
		int sectionId;
		Platform::String^ text;
		Platform::String^ token;
		Windows::Web::Http::HttpClient^ httpClient;
	};

}

#endif