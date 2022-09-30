#pragma once

#include "MainPage.g.h"

namespace muxc
{
    using namespace winrt::Microsoft::UI::Xaml::Controls;
};

namespace wuxc
{
    using namespace winrt::Windows::UI::Xaml::Controls;
};

namespace winrt::HotelManagement::implementation
{


    struct MainPage : MainPageT<MainPage>
    {
        MainPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/Windows/cppwinrt/tree/master/nuget#initializecomponent

        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        Windows::Foundation::IAsyncAction SendFileToLocal(hstring, hstring);

        void MainPivot_Loaded(IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        void navView_BackRequested(muxc::NavigationView const& sender, muxc::NavigationViewBackRequestedEventArgs const& args);
        void navView_DisplayModeChanged(muxc::NavigationView const& sender, muxc::NavigationViewDisplayModeChangedEventArgs const& args);
        void navView_PaneOpening(muxc::NavigationView const& sender, winrt::Windows::Foundation::IInspectable const& args);
        void navView_PaneClosing(muxc::NavigationView const& sender, winrt::Windows::Foundation::IInspectable const& args);
        void navView_ItemInvoked(muxc::NavigationView const& sender, muxc::NavigationViewItemInvokedEventArgs const& args);
        void navView_Navigate(std::wstring navItemTag, Windows::UI::Xaml::Media::Animation::NavigationTransitionInfo const& transitionInfo);
    
        void On_Navigated(Windows::Foundation::IInspectable const& /* sender */, Windows::UI::Xaml::Navigation::NavigationEventArgs const& args);
        void System_BackRequested(Windows::Foundation::IInspectable const& /* sender */, Windows::UI::Core::BackRequestedEventArgs const& args);
        
        bool TryGoBack();

    private:
        // Vector of std::pair holding the Navigation Tag and the relative Navigation Page.
        std::vector<std::pair<std::wstring, Windows::UI::Xaml::Interop::TypeName>> m_pages;
    };
}

namespace winrt::HotelManagement::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {

    };
}
