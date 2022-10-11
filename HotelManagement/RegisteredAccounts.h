#pragma once

#include "RegisteredAccounts.g.h"

namespace winrt::HotelManagement::implementation
{
    struct RegisteredAccounts : RegisteredAccountsT<RegisteredAccounts>
    {
        RegisteredAccounts() 
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void Page_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::HotelManagement::factory_implementation
{
    struct RegisteredAccounts : RegisteredAccountsT<RegisteredAccounts, implementation::RegisteredAccounts>
    {
    };
}
