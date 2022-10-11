#include "pch.h"
#include "Settings.h"
#if __has_include("Settings.g.cpp")
#include "Settings.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

namespace winrt::HotelManagement::implementation
{
    int32_t Settings::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Settings::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void Settings::btnMicaTheme_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        toggleMicaTheme().IsOn(!toggleMicaTheme().IsOn());
    }


    void Settings::toggleMicaTheme_Toggled(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        auto mainPage = Frame().Parent().as<NavigationView>().Parent().as<winrt::Windows::UI::Xaml::Controls::Grid>().Parent().as<MainPage>();
        
        
        BackdropMaterial::SetApplyToRootOrPageBackground(mainPage, toggleMicaTheme().IsOn());
        if (!toggleMicaTheme().IsOn())
        {
            mainPage.Background(
                winrt::Windows::UI::Xaml::Media::SolidColorBrush(
                    winrt::unbox_value<Windows::UI::Color>(
                        Resources().Lookup(
                            winrt::box_value(L"SystemAccentColor")
                        )
                    )
                )
            );

        }

    }

    void Settings::Page_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        auto mainPage = Frame().Parent().as<NavigationView>().Parent().as<winrt::Windows::UI::Xaml::Controls::Grid>().Parent().as<MainPage>();
        toggleMicaTheme().IsOn(BackdropMaterial::GetApplyToRootOrPageBackground(mainPage));
    }


}


