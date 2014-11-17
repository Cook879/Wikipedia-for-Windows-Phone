//
// Article.cpp
// Implementation of the Article class
// 
// See Article.h for details
//

#include "pch.h"
#include "Article.h"
#include "HelperFunctions.h"

using namespace Wikipedia;

using namespace concurrency;
using namespace std;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Web::Http;

Article::Article(String^ ns, String^ name, String^ lang) {
	this->ns = ns;
	this->name = name;
	this->lang = lang;

	sections = ref new Vector<Section^>();
	languages = ref new Vector<Article^>();
	foundLanguages = false;
}

//
// Constructor that takes a title, complete with namespace
// 
Article::Article(String^ title, String^ lang) {

	// Convert to a wstring so we can manipulate
	wstring titleStr = title->Data();
	titleStr = HelperFunctions::UnderscoreToSpace(titleStr);

	// Try and find a colon
	int pos = titleStr.find(':');

	// No colon = main namespace, else other namespace
	if (pos == string::npos) {
		this->ns = "main";
		this->name = title;
	} else {
		this->ns = ref new String(titleStr.substr(0, pos).c_str());
		this->name = ref new String(titleStr.substr(pos + 1).c_str());
	}

	this->lang = lang;
	sections = ref new Vector<Section^>();
	languages = ref new Vector<Article^>();
	foundLanguages = false;
}

String^ Article::GetTitle() {
	String^ title = ref new String();

	if (!ns->Equals("main"))
		title += ns + ":";
	title += name;

	return title;
}

void Article::SetTitle(String^ title) {
	// Convert to a wstring so we can manipulate
	wstring titleStr = title->Data();
	titleStr = HelperFunctions::UnderscoreToSpace(titleStr);

	// Try and find a colon
	int pos = titleStr.find(':');

	// No colon = main namespace, else other namespace
	if (pos == string::npos) {
		this->ns = "main";
		this->name = title;
	}
	else {
		this->ns = ref new String(titleStr.substr(0, pos).c_str());
		this->name = ref new String(titleStr.substr(pos + 1).c_str());
	}
}


String^ Article::GetNamespace() {
	return ns;
}

String^ Article::GetName() {
	return name;
}

String^ Article::GetLang() {
	return lang;
}

bool Article::HasContent() {
	return !content->IsEmpty();
}

void Article::SetContent(String^ content) {
	this->content = content;
}

String^ Article::GetContent() {
	return content;
}

void Article::AddSection(Section^ section) {
	sections->Append(section);
}

IVector<Section^>^ Article::GetSections() {
	return sections;
}

Section^ Article::GetSection(int id) {
	return sections->GetAt(id);
}

bool Article::HasLanguages(void) {
	return foundLanguages;
}

void Article::FoundLanguages() {
	foundLanguages = true;
}
void Article::AddLanguage(Article^ lang) {
	languages->Append(lang);
}

IVector<Article^>^ Article::GetLanguages() {
	return languages;
}

// 
// Section class
//
Section::Section(int id, String^ line, String^ text, int level, Section^ parent) {
	this->id = id;
	this->line = line;
	this->text = text;
	this->level = level;
	this->parent = parent;
}


// 
// Edit class
//
Edit::Edit(Article^ article, int sectionId, String^ text, String^ token, HttpClient^ httpClient) {
	this->article = article;
	this->sectionId = sectionId;
	this->text = text;
	this->token = token;
	this->httpClient = httpClient;
}

Article^ Edit::GetArticle() {
	return article;
}
int Edit::GetSectionId() {
	return sectionId;
}

String^ Edit::GetText() {
	return text;
}
String^ Edit::GetToken() {
	return token;
}
HttpClient^ Edit::GetHttpClient() {
	return httpClient;
}

void Edit::SetHttpClient(HttpClient^ httpClient) {
	this->httpClient = httpClient;
}

void Edit::SetToken(Platform::String^ token) {
	this->token = token;
}

void Edit::SetText(Platform::String^ text) {
	this->text = text;
}