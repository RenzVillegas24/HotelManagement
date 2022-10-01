#pragma once

#include "SignUpForm.g.h"

#include "string";
#include "array";
#include "vector";


using namespace std;

namespace winrt::HotelManagement::implementation
{
    struct SignUpForm : SignUpFormT<SignUpForm>
    {
        SignUpForm() 
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);


        void cmbxCountry_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e);
        void cmbxRegion_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e);
        void cmbxProvince_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e);
        void cmbxCityMunicipal_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e);
        void cmbxBarangay_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e);
        
        Windows::Foundation::IAsyncAction btnConfirm_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction btnCancel_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);



        void cmbxCountry_DropDownOpened(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        void cmbxRegion_DropDownOpened(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        void cmbxProvince_DropDownOpened(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        void cmbxCityMunicipal_DropDownOpened(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        void cmbxBarangay_DropDownOpened(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);

        Windows::Foundation::IAsyncAction btnChooseImage_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void Page_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

    private:


        std::vector<std::array<std::string, 2>> regionS;
        std::vector<std::array<std::string, 2>> provinceS;
        std::vector<std::array<std::string, 2>> cityMunicipalS;
        std::vector<std::array<std::string, 2>> barangayS;
        std::string brgySelCode;
        winrt::hstring imageIDURI;
        winrt::Windows::Storage::StorageFile imgIDFile = winrt::Windows::Storage::StorageFile(nullptr);
    };
}

namespace winrt::HotelManagement::factory_implementation
{
    struct SignUpForm : SignUpFormT<SignUpForm, implementation::SignUpForm>
    {

    };
}
