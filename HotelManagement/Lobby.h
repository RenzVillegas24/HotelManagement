#pragma once

#include "Lobby.g.h"

namespace winrt::HotelManagement::implementation
{
    struct Lobby : LobbyT<Lobby>
    {
        Lobby() 
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
    struct Lobby : LobbyT<Lobby, implementation::Lobby>
    {
    };
}
