//
// HelperFunctions.h
// Declaration of the HelperFunctions class
//
// A collection of random functions which are used in multiple places
//

#ifndef HELPER_H
#define HELPER_H
namespace Wikipedia {
	namespace HelperFunctions {
			Windows::Web::Http::HttpClient^ CreateHttpClient(void);
			void ErrorMessage(Platform::String^ message);
			std::wstring UnderscoreToSpace(std::wstring s);
			void DoNothing(Windows::UI::Popups::IUICommand^ command);
			Platform::String^ UnderscoreToSpace(Platform::String^ s);
			void Sleep(uint32 ms);

			Platform::String^ ProcessSection(Platform::String^ sectionStr, int id, int level);
			Platform::String^ ProcessContent(Platform::String^ contentStr, Platform::String^ title);
			Platform::String^ ProcessImages(Platform::String^ sectionStr);
	}
}

#endif