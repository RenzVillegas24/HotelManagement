#include "pch.h"
#include "Settings.h"
#if __has_include("Settings.g.cpp")
#include "Settings.g.cpp"
#endif

#include "winrt/Windows.ApplicationModel.Resources.Core.h"

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
        auto mainPage = Frame().Parent().as<NavigationView>().Parent().as<winrt::Windows::UI::Xaml::Controls::Grid>().Parent().as<MainPage>();


    }


    void Settings::toggleMicaTheme_Toggled(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        auto mainPage = Frame().Parent().as<NavigationView>().Parent().as<winrt::Windows::UI::Xaml::Controls::Grid>().Parent().as<MainPage>();
        auto root = Window::Current().Content().as<FrameworkElement>();

        
        
        BackdropMaterial::SetApplyToRootOrPageBackground(mainPage, toggleMicaTheme().IsOn());
        if (!toggleMicaTheme().IsOn())
        {
            if (root.RequestedTheme() == ElementTheme::Light)
                mainPage.Background(
                    winrt::unbox_value<Windows::UI::Xaml::Media::AcrylicBrush>(
                        Application::Current().Resources().Lookup(
                            winrt::box_value(L"LightAcrylic")
                        )
                    )
                );
            else if (root.RequestedTheme() == ElementTheme::Dark)
                mainPage.Background(
                    winrt::unbox_value<Windows::UI::Xaml::Media::AcrylicBrush>(
                        Application::Current().Resources().Lookup(
                            winrt::box_value(L"DarkAcrylic")
                        )
                    )
                );
            else
                mainPage.Background(
                    winrt::unbox_value<Windows::UI::Xaml::Media::AcrylicBrush>(
                        Application::Current().Resources().Lookup(
                            winrt::box_value(L"SystemControlAltHighAcrylicWindowBrush")
                        )
                    )
                );

        }

    }

    void Settings::Page_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        auto mainPage = Frame().Parent().as<NavigationView>().Parent().as<winrt::Windows::UI::Xaml::Controls::Grid>().Parent().as<MainPage>();
        toggleMicaTheme().IsOn(BackdropMaterial::GetApplyToRootOrPageBackground(mainPage));

        auto root = Window::Current().Content().as<FrameworkElement>();

        if (root.RequestedTheme() == ElementTheme::Default)
            cmbxTheme().SelectedIndex(0);
        else if (root.RequestedTheme() == ElementTheme::Light)
            cmbxTheme().SelectedIndex(1);
        else if (root.RequestedTheme() == ElementTheme::Dark)
            cmbxTheme().SelectedIndex(2);
        
        toggleStickyHeader().IsOn(!isHeaderSticky());

    
    }


    void Settings::btnTheme_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        cmbxTheme().IsDropDownOpen(!cmbxTheme().IsDropDownOpen());
    }

    void Settings::cmbxTheme_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {
        auto root = Window::Current().Content().as<FrameworkElement>();
        auto selected = cmbxTheme().SelectedItem().as<winrt::Windows::UI::Xaml::Controls::TextBlock>().Text();
        auto mainPage = Frame().Parent().as<NavigationView>().Parent().as<winrt::Windows::UI::Xaml::Controls::Grid>().Parent().as<MainPage>();

        if (selected == L"System Default")
            root.RequestedTheme(ElementTheme::Default);
        else if (selected == L"Light")
        {
            root.RequestedTheme(ElementTheme::Light);
            mainPage.Background(
                winrt::unbox_value<Windows::UI::Xaml::Media::AcrylicBrush>(
                    Application::Current().Resources().Lookup(
                        winrt::box_value(L"LightAcrylic")
                    )
                )
            );
        }
        else if (selected == L"Dark")
        {
            root.RequestedTheme(ElementTheme::Dark);
            mainPage.Background(
                winrt::unbox_value<Windows::UI::Xaml::Media::AcrylicBrush>(
                    Application::Current().Resources().Lookup(
                        winrt::box_value(L"DarkAcrylic")
                    )
                    )
            );
        }

    }
    void Settings::toggleStickyHeader_Toggled(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        isHeaderSticky(!toggleStickyHeader().IsOn());
    }


    void Settings::btnStickyHeader_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        toggleStickyHeader().IsOn(!toggleStickyHeader().IsOn());
    }

}





