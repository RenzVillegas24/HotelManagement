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

        void btnMicaTheme_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void toggleMicaTheme_Toggled(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);


    private:
        static Windows::UI::Xaml::DependencyProperty m_isMicaEnable;
    public:
        void Page_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::HotelManagement::factory_implementation
{
    struct Settings : SettingsT<Settings, implementation::Settings>
    {
    };
}
