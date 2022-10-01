#include "pch.h"
#include "SignUpForm.h"
#if __has_include("SignUpForm.g.cpp")
#include "SignUpForm.g.cpp"
#endif

#include "Globals.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Globalization.DateTimeFormatting.h>

#include <winrt/Windows.ApplicationModel.DataTransfer.h>

#include <winrt/Windows.UI.Xaml.Media.Imaging.h>

using namespace winrt;
using namespace Windows;
using namespace UI::Xaml;
using namespace UI::Popups;
using namespace Controls;
using namespace Security::Cryptography;


using namespace sqlite;
using namespace std;


namespace winrt::HotelManagement::implementation
{
    int32_t SignUpForm::MyProperty()
    {
        throw hresult_not_implemented();
       
    }

    void  SignUpForm::MyProperty(int32_t /* value */)
    {
              
        throw hresult_not_implemented();
       
    }


    void SignUpForm::cmbxCountry_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {
        

        if (cmbxCountry().SelectedItem() != nullptr) {
            sqlite_config config;
            config.flags = OpenFlags::READONLY;

            database db(to_string(localPath().Path() + L"\\refRegion.db"), config);
            cmbxRegion().Items().Clear();
            regionS.clear();
            cmbxProvince().Items().Clear();
            provinceS.clear();
            cmbxCityMunicipal().Items().Clear();
            cityMunicipalS.clear();
            cmbxBarangay().Items().Clear();
            barangayS.clear();

            if (cmbxCountry().SelectedItem().as< UI::Xaml::Controls::TextBlock>().Text() == L"Philippines")
            try
            {

                db << "SELECT regDesc, regCode FROM refregion;"
                    >> [&](string region, string code) {

                    auto txt = Controls::TextBlock();
                    txt.Text(to_hstring(region));
                    

                    cmbxRegion().Items().Append(txt);
                    array<string, 2> data = { region, code };
                    regionS.push_back(data);
                    
                    

                };

           

            }
            catch (exception& e)
            {
                MessageDialog dialog(to_hstring(e.what()), L"Database Error");
                dialog.ShowAsync();
            }
        }
    }


    void SignUpForm::cmbxRegion_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {
        if (cmbxRegion().SelectedItem() != nullptr) {
            sqlite_config config;
            config.flags = OpenFlags::READONLY;

            cmbxProvince().Items().Clear();
            provinceS.clear();
            cmbxCityMunicipal().Items().Clear();
            cityMunicipalS.clear();
            cmbxBarangay().Items().Clear();
            barangayS.clear();


            database db(to_string(localPath().Path() + L"\\refProvince.db"), config);
            try
            {
            
                db << "SELECT provDesc, provCode FROM refprovince WHERE regCode = '" + regionS.at(cmbxRegion().SelectedIndex()).at(1) + "';"
                    >> [&](string province, string code) {

                    auto txt = Controls::TextBlock();
                    txt.Text(to_hstring(province));


                    cmbxProvince().Items().Append(txt);
                    array<string, 2> data = { province, code };
                    provinceS.push_back(data);

                };


            }
            catch (exception& e)
            {
                MessageDialog dialog(to_hstring(e.what()), L"Database Error");
                dialog.ShowAsync();
            }
        }
    }


    void SignUpForm::cmbxProvince_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {
        if (cmbxProvince().SelectedItem() != nullptr) {
            sqlite_config config;
            config.flags = OpenFlags::READONLY;

            cmbxCityMunicipal().Items().Clear();
            cityMunicipalS.clear();
            cmbxBarangay().Items().Clear();
            barangayS.clear();

            database db(to_string(localPath().Path() + L"\\refCitymun.db"), config);
            try
            {

                db << "SELECT citymunDesc, citymunCode FROM refcitymun WHERE provCode = '" + provinceS.at(cmbxProvince().SelectedIndex()).at(1) + "';"
                    >> [&](string city, string code) {

                    auto txt = Controls::TextBlock();
                    txt.Text(to_hstring(city));


                    cmbxCityMunicipal().Items().Append(txt);
                    array<string, 2> data = { city, code };
                    cityMunicipalS.push_back(data);

                };

            }
            catch (exception& e)
            {
                MessageDialog dialog(to_hstring(e.what()), L"Database Error");
                dialog.ShowAsync();
            }


        }
    }


    void SignUpForm::cmbxCityMunicipal_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {
        if (cmbxCityMunicipal().SelectedItem() != nullptr) {
            sqlite_config config;
            config.flags = OpenFlags::READONLY;


            database db(to_string(localPath().Path() + L"\\refBrgy.db"), config);    
            cmbxBarangay().Items().Clear();
            barangayS.clear();

            try
            {

                db << "SELECT brgyDesc, brgyCode FROM refbrgy WHERE citymunCode = '" + cityMunicipalS.at(cmbxCityMunicipal().SelectedIndex()).at(1) + "';"
                    >> [&](string barangay, string code) {

                    auto txt = Controls::TextBlock();
                    txt.Text(to_hstring(barangay));


                    cmbxBarangay().Items().Append(txt);
                    array<string, 2> data = { barangay, code };
                    barangayS.push_back(data);

                };

            }
            catch (exception& e)
            {
                MessageDialog dialog(to_hstring(e.what()), L"Database Error");
                dialog.ShowAsync();
            }

        }
    }


    void SignUpForm::cmbxBarangay_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {
        if (cmbxBarangay().SelectedItem() != nullptr) {
        
            sqlite_config config;
            config.flags = OpenFlags::READONLY;


            database db(to_string(localPath().Path() + L"\\refBrgy.db"), config);

            try
            {

                db << "SELECT brgyCode FROM refbrgy WHERE "
                    " brgyDesc = '" + to_string(cmbxBarangay().SelectedItem().as<TextBlock>().Text()) + 
                    "' AND regCode = '" + regionS.at(cmbxRegion().SelectedIndex()).at(1) +
                    "' AND provCode = '" + provinceS.at(cmbxProvince().SelectedIndex()).at(1) +
                    "' AND citymunCode = '" + cityMunicipalS.at(cmbxCityMunicipal().SelectedIndex()).at(1) +
                    "';"
                    >> brgySelCode;

            }
            catch (exception& e)
            {
                MessageDialog dialog(to_hstring(e.what()), L"Database Error");
                dialog.ShowAsync();
            }
        }
    }


    void SignUpForm::cmbxCountry_DropDownOpened(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e)
    {
        cmbxCountry().SelectedItem(nullptr);

    }


    void SignUpForm::cmbxRegion_DropDownOpened(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e)
    {
        cmbxRegion().SelectedItem(nullptr);

    }


    void SignUpForm::cmbxProvince_DropDownOpened(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e)
    {
        cmbxProvince().SelectedItem(nullptr);

    }


    void SignUpForm::cmbxCityMunicipal_DropDownOpened(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e)
    {
        cmbxCityMunicipal().SelectedItem(nullptr);

    }


    void SignUpForm::cmbxBarangay_DropDownOpened(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e)
    {
        cmbxBarangay().SelectedItem(nullptr);

    }


/*

Villarica
Ponsho
Pilato
Jr.
Male
June 27, 1980
Single
Filipino
Philippines
Region V
Albay
Manito
Balasbas
324
2345
09364738534
VillaricaPonshoPilato@gmail.com
VillaricaPonsho
#123Pass


*/


    Windows::Foundation::IAsyncAction SignUpForm::btnConfirm_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        if (
            txtbxLastName().Text().empty() ||
            txtbxFirstName().Text().empty() ||
            txtbxMiddleName().Text().empty() ||
            cmbxGender().SelectedItem() == nullptr ||
            txtbxBirthDate().SelectedDate() == nullptr ||
            cmbxCivil().SelectedItem() == nullptr ||
            txtbxNationality().Text().empty() ||
            cmbxCountry().SelectedItem() == nullptr ||
            cmbxRegion().SelectedItem() == nullptr ||
            cmbxProvince().SelectedItem() == nullptr ||
            cmbxCityMunicipal().SelectedItem() == nullptr ||
            cmbxBarangay().SelectedItem() == nullptr ||
            txtbxUnitBuildingEtc().Text().empty() ||
            txtbxZipCode().Text().empty() ||
            txtbxContact().Text().empty() ||
            txtbxEmail().Text().empty() ||
            txtbxUsername().Text().empty() ||
            txtbxPassword().Password().empty() ||
            txtbxConfirmPassword().Password().empty() ||
            imageIDURI.empty()
            ) {
            MessageDialog dialog(L"Some items are empty,\nplease try again..." , L"Input Error");
            dialog.ShowAsync();
        }
        else {
            hstring errors = L"";


            if (txtbxContact().Text().size() != 11) 
                errors = errors + L"\n● Contact numbers should be 11 numbers only.";
            else 
                if (to_string(txtbxContact().Text()).find_first_not_of("0123456789") != string::npos)
                    errors = errors + L"\n● Contact numbers should contains numbers only.";

            if (txtbxZipCode().Text().size() != 4)
                errors = errors + L"\n● Zip code should be 4 numbers only.";
            
            if (txtbxEmail().Text().size() < 10) 
                errors = errors + L"\n● Email should be atleast 10 characters.";
            else 
                if (txtbxPassword().Password() != txtbxConfirmPassword().Password())
                    errors = errors + L"\n● Password doesn't match with password confirmation.";

            if (txtbxPassword().Password().size() <= 6) 
                errors = errors + L"\n● Password should be atleast 7 characters.";
            else 
                if (to_string(txtbxPassword().Password()).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._-?#@*$&!") != string::npos)
                    errors = errors + L"\n● Password should contains alphabets, numbers and special characters (._-?#@*$&!) only.";

            
            if (txtbxUsername().Text().size() <= 6) 
                errors = errors + L"\n● Username should be atleast 6 characters.";
            else 
                if (to_string(txtbxUsername().Text()).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._-") != string::npos)
                    errors = errors + L"\n● Username should contains alphabets, numbers and specific characters (._-) only.";

            

            if (errors.empty()) {

                dbPath(to_string(localPath().Path()) + "\\data.db");
                database db(dbPath());

                bool isAccountAlready = false;
                try {
                    string temp;
                    db << "SELECT username FROM accounts WHERE username = '" + to_string(txtbxUsername().Text()) + "';"
                        >> temp;


                }
                catch (exception& e) {
                    isAccountAlready = true;
                }

                if (isAccountAlready) {
                    initDatabase();

                    try {
                   
                        auto bdayFmt = Globalization::DateTimeFormatting::DateTimeFormatter(L"{month.integer(2)}/{day.integer(2)}/{year.full}");


                        db << "INSERT INTO accounts ("
                            "   username      ,"
                            "   password      ,"
                            "   icon          ,"
                            "   civil         ,"
                            "   nationality   ,"
                            //name            
                            "   firstName     ,"
                            "   lastName      ,"
                            "   middleName    ,"
                            "   suffix        ,"
                            //--              
                            "   gender        ,"
                            "   birthday      ,"
                            //home address    
                            "   untBlck_etc   ,"
                            "   barangay      ,"
                            "   city          ,"
                            "   province      ,"
                            "   region        ,"
                            "   country       ,"
                            //code
                            "   barangayCode  ,"
                            "   cityCode      ,"
                            "   provinceCode  ,"
                            "   regionCode    ,"
                            //--              
                            "   zip           ,"
                            "   email         ,"
                            "   teleNum       ,"
                            "   isAdmin       "
                            ")"
                     

                            "VALUES("
                            " '" +
                                      to_string(txtbxUsername().Text())                                        + "', " 
                            " '" +    to_string(txtbxPassword().Password())                                    + "', " 
                            " '" +    to_string(co_await ImageToBase64(imgIDFile, 250)) +  "', " +
                            " '" +    to_string(cmbxCivil().SelectedItem().as<TextBlock>().Text())             + "', " 
                            " '" +    to_string(txtbxNationality().Text())                                     + "', " 
                                      
                            " '" +    to_string(txtbxFirstName().Text())                                       + "', " 
                            " '" +    to_string(txtbxLastName().Text())                                        + "', " 
                            " '" +    to_string(txtbxMiddleName().Text())                                      + "', " 
                            " '" +    to_string(txtbxSuffixName().Text())                                      + "', " 
                                      
                            " '" +    std::to_string(cmbxGender().SelectedIndex())                             + "', " 
                            " '" +    to_string(bdayFmt.Format(txtbxBirthDate().Date()))                       + "', " 
                                      
                            " '" +    to_string(txtbxUnitBuildingEtc().Text())                                 + "', " 
                            " '" +    to_string(cmbxBarangay().SelectedItem().as<TextBlock>().Text())          + "', " 
                            " '" +    to_string(cmbxCityMunicipal().SelectedItem().as<TextBlock>().Text())     + "', " 
                            " '" +    to_string(cmbxProvince().SelectedItem().as<TextBlock>().Text())          + "', " 
                            " '" +    to_string(cmbxRegion().SelectedItem().as<TextBlock>().Text())            + "', " 
                            " '" +    to_string(cmbxCountry().SelectedItem().as<TextBlock>().Text())           + "', " 
                                      
                            " '" +    brgySelCode                                                              + "', " 
                            " '" +    cityMunicipalS.at(cmbxCityMunicipal().SelectedIndex()).at(1)             + "', " 
                            " '" +    provinceS.at(cmbxProvince().SelectedIndex()).at(1)                       + "', " 
                            " '" +    regionS.at(cmbxRegion().SelectedIndex()).at(1)                           + "', " 
                                      
                            " '" +    to_string(txtbxZipCode().Text())                                         + "', " 
                            " '" +    to_string(txtbxEmail().Text())                                           + "', " 
                            " '" +    to_string(txtbxContact().Text())                                         + "', "
                            " '0' );";




                        MessageDialog dialog(L"The account was successfully created!", L"Successfully added account");
                        auto& res = co_await dialog.ShowAsync();

                        if (res.Label() == L"Yes") {
                            Frame().GoBack();
                        }


                    }
                    catch (exception& e) {
                        MessageDialog dialog(to_hstring(e.what()), L"Database Error");
                        dialog.ShowAsync();
                    }

                }
                else {

                    MessageDialog dialog(L"The account you are using are already used,\nplease try again...", L"Account already exists");
                    dialog.ShowAsync();
                }

            }
            else {

                MessageDialog dialog(L"There are errors on your inputs:" + errors + L"\nplease try again...", L"Input Error");
                dialog.ShowAsync();
            }
        }
    }

  
    Windows::Foundation::IAsyncAction SignUpForm::btnCancel_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {

        MessageDialog dialog(L"Are you want to cancel your registration?\nOnce you leave the page, the inserted info will reset.", L"Leaving Confirmation");
        

        dialog.Commands().Append(UICommand(L"Yes", nullptr));
        dialog.Commands().Append(UICommand(L"No", nullptr));
        auto& result = co_await dialog.ShowAsync();


        if (result.Label() == L"Yes")
            Frame().GoBack();

    }

    Windows::Foundation::IAsyncAction SignUpForm::btnChooseImage_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        auto picker = Storage::Pickers::FileOpenPicker();
        picker.ViewMode(Storage::Pickers::PickerViewMode::Thumbnail);
        picker.SuggestedStartLocation(Storage::Pickers::PickerLocationId::PicturesLibrary);

        picker.FileTypeFilter().Append(L".jpg");
        picker.FileTypeFilter().Append(L".jpeg");
        picker.FileTypeFilter().Append(L".png");
        picker.FileTypeFilter().Append(L".bmp");
        picker.FileTypeFilter().Append(L".gif");


        imgIDFile = co_await picker.PickSingleFileAsync();
        
        
        if (imgIDFile != nullptr)
        {
            auto stream = co_await imgIDFile.OpenAsync(Storage::FileAccessMode::Read);
        
            auto bitmapImg = UI::Xaml::Media::Imaging::BitmapImage();
            co_await bitmapImg.SetSourceAsync(stream);

        
            auto decoder = Graphics::Imaging::BitmapDecoder::CreateAsync(stream);
            imgID().ImageSource(bitmapImg);
            imageIDURI = imgIDFile.Path();


            auto hhh = co_await ImageToBase64(imgIDFile, 250);
           
            winrt::Windows::ApplicationModel::DataTransfer::DataPackage dataPackage =  winrt::Windows::ApplicationModel::DataTransfer::DataPackage::DataPackage();
            dataPackage.SetText(hhh);

            winrt::Windows::ApplicationModel::DataTransfer::Clipboard::SetContent(dataPackage);
            
            
            const char* s = to_string(hhh).c_str();
            size_t start = 0;
            size_t end = 100; 

            auto nnn = to_string(hhh).substr(start, end - start);

            MessageDialog dialog(to_hstring(nnn) + L"...", L"Copied");
            co_await dialog.ShowAsync();
        }

        
    }


    void SignUpForm::Page_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {


    }

  

}





