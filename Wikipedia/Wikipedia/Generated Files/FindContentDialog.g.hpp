﻿

//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"
#include "FindContentDialog.xaml.h"




void ::Wikipedia::FindContentDialog::InitializeComponent()
{
    if (_contentLoaded)
        return;

    _contentLoaded = true;

    // Call LoadComponent on ms-appx:///FindContentDialog.xaml
    ::Windows::UI::Xaml::Application::LoadComponent(this, ref new ::Windows::Foundation::Uri(L"ms-appx:///FindContentDialog.xaml"), ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);

    // Get the TextBox named 'searchField'
    searchField = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"searchField"));
}

void ::Wikipedia::FindContentDialog::Connect(int connectionId, Platform::Object^ target)
{
    switch (connectionId)
    {
    case 1:
        (safe_cast<::Windows::UI::Xaml::Controls::ContentDialog^>(target))->PrimaryButtonClick +=
            ref new ::Windows::Foundation::TypedEventHandler<::Windows::UI::Xaml::Controls::ContentDialog^, ::Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^>(this, (void (::Wikipedia::FindContentDialog::*)(Windows::UI::Xaml::Controls::ContentDialog^, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^))&FindContentDialog::ContentDialog_PrimaryButtonClick);
        (safe_cast<::Windows::UI::Xaml::Controls::ContentDialog^>(target))->SecondaryButtonClick +=
            ref new ::Windows::Foundation::TypedEventHandler<::Windows::UI::Xaml::Controls::ContentDialog^, ::Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^>(this, (void (::Wikipedia::FindContentDialog::*)(Windows::UI::Xaml::Controls::ContentDialog^, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^))&FindContentDialog::ContentDialog_SecondaryButtonClick);
        break;
    }
    (void)connectionId; // Unused parameter
    (void)target; // Unused parameter
    _contentLoaded = true;
}
