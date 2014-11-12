﻿

#pragma once
//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------

namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Controls {
                ref class Grid;
                ref class TextBlock;
            }
        }
    }
}
namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Documents {
                ref class Hyperlink;
                ref class Run;
            }
        }
    }
}

namespace Wikipedia
{
    partial ref class AboutPage : public ::Windows::UI::Xaml::Controls::Page, 
        public ::Windows::UI::Xaml::Markup::IComponentConnector
    {
    public:
        void InitializeComponent();
        virtual void Connect(int connectionId, ::Platform::Object^ target);
    
    private:
        bool _contentLoaded;
    
        private: ::Windows::UI::Xaml::Controls::Grid^ LayoutRoot;
        private: ::Windows::UI::Xaml::Controls::Grid^ ContentRoot;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ Copyright;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ WikipediaContent;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ Feedback;
        private: ::Windows::UI::Xaml::Documents::Hyperlink^ email;
        private: ::Windows::UI::Xaml::Documents::Run^ ByUsingThis;
        private: ::Windows::UI::Xaml::Documents::Run^ and;
        private: ::Windows::UI::Xaml::Documents::Run^ PrivacyPolicy;
        private: ::Windows::UI::Xaml::Documents::Run^ TermsOfUse;
        private: ::Windows::UI::Xaml::Documents::Run^ CreativeCommons;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ AppTitle;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ AboutTitle;
    };
}

