#pragma once

#include "Gyms.g.h"

namespace winrt::HotelManagement::implementation
{
    struct Gyms : GymsT<Gyms>
    {
        Gyms() 
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);
        void HomeTitle_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

    };
}

namespace winrt::HotelManagement::factory_implementation
{
    struct Gyms : GymsT<Gyms, implementation::Gyms>
    {
    };
}
