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
                ref class Button;
                ref class TextBox;
                ref class CheckBox;
                ref class StackPanel;
                ref class TextBlock;
            }
        }
    }
}
namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Documents {
                ref class Run;
            }
        }
    }
}

namespace Wikipedia
{
    partial ref class SavePage : public ::Windows::UI::Xaml::Controls::Page, 
        public ::Windows::UI::Xaml::Markup::IComponentConnector
    {
    public:
        void InitializeComponent();
        virtual void Connect(int connectionId, ::Platform::Object^ target);
    
    private:
        bool _contentLoaded;
    
        private: ::Windows::UI::Xaml::Controls::Grid^ ContentRoot;
        private: ::Windows::UI::Xaml::Controls::Button^ SaveButton;
        private: ::Windows::UI::Xaml::Controls::TextBox^ SummaryBox;
        private: ::Windows::UI::Xaml::Controls::CheckBox^ MinorEditBox;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ Captcha;
        private: ::Windows::UI::Xaml::Controls::TextBox^ CAPTCHABox;
        private: ::Windows::UI::Xaml::Controls::Button^ SubmitButton;
        private: ::Windows::UI::Xaml::Documents::Run^ TextA;
        private: ::Windows::UI::Xaml::Documents::Run^ Summary;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ SavingChangesTo;
    };
}

