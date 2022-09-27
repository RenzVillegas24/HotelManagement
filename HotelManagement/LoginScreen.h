#pragma once

#include "LoginScreen.g.h"



namespace winrt::HotelManagement::implementation
{
    struct LoginScreen : LoginScreenT<LoginScreen>
    {
        LoginScreen() 
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);


        winrt::Windows::Foundation::IAsyncAction btnLogin_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void btnSignUp_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void btnForgot_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void Page_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::HotelManagement::factory_implementation
{
    struct LoginScreen : LoginScreenT<LoginScreen, implementation::LoginScreen>
    {
    };
}
