#pragma once

#include "Support.g.h"

namespace winrt::HotelManagement::implementation
{
    struct Support : SupportT<Support>
    {
        Support() 
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
    struct Support : SupportT<Support, implementation::Support>
    {
    };
}
