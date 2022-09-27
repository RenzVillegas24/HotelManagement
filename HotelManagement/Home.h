#pragma once

#include "Home.g.h"
#include "winrt/Windows.UI.Xaml.h"

namespace winrt::HotelManagement::implementation
{
    struct Home : HomeT<Home>
    {
        Home() 
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
            Home::flipTimer;
            Home::directionG = Home::directionP = Home::directionR = Home::directionU = 1;

        }

        int32_t MyProperty();
        Windows::UI::Xaml::DispatcherTimer Home::flipTimer;
        int directionD, 
            directionU,
            directionG,
            directionR,
            directionP,
            directionL,
            directionE,
            directionA;

        int tickTime = 0;


        void MyProperty(int32_t value);


        void Page_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void OnTick(IInspectable const& sender, IInspectable const& event);

        void OnTickDiscover();
        void OnTickUnits();
        void OnTickGym();
        void OnTickRestaurant();
        void OnTickPool();
        void OnTickLobby();
        void OnTickEvents();
        void OnTickAll();

        void Page_Unloaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void btnScrollDown_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void btnRooms_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void btnGyms_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void btnRestaurant_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void btnPools_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void btnLobby_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void btnEvents_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void btnHelp_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        winrt::Windows::Foundation::IAsyncAction btnReserve_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::HotelManagement::factory_implementation
{
    struct Home : HomeT<Home, implementation::Home>
    {
    };
}
