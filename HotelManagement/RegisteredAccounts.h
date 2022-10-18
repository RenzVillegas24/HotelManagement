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
        void scrollMain_ViewChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs const& e);
        void stickyHeader_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::SizeChangedEventArgs const& e);
        void headerShow_Completed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        void headerHide_Completed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        Windows::Foundation::IAsyncAction RegisteredAccounts::appendDataToList(int isAdmin, int id, std::string lastName, std::string firstName, std::string middleName, std::string suffix, std::string birthday, int age, std::string icon, std::string province, std::string city, std::string barangay, std::string untBlck_etc);
        void cmbxFilter_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e);
        void cmbxSort_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e);
        void Page_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::SizeChangedEventArgs const& e);
       

    private:
        double previousOffset;
        double stoppedOffset;
        double downOffset;
        double upOffset;

    };
}

namespace winrt::HotelManagement::factory_implementation
{
    struct RegisteredAccounts : RegisteredAccountsT<RegisteredAccounts, implementation::RegisteredAccounts>
    {
    };
}
