#pragma once

#include "Settings.g.h"

namespace winrt::HotelManagement::implementation
{
    struct Settings : SettingsT<Settings>
    {
        Settings() 
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

    };
}

namespace winrt::HotelManagement::factory_implementation
{
    struct Settings : SettingsT<Settings, implementation::Settings>
    {
    };
}
