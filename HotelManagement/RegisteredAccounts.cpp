#include "pch.h"
#include "RegisteredAccounts.h"
#if __has_include("RegisteredAccounts.g.cpp")
#include "RegisteredAccounts.g.cpp"
#endif

#include "Globals.h"
#include <regex>

using namespace winrt;
using namespace Windows::UI;
using namespace Xaml;
using namespace Controls;
using namespace Media;
using namespace sqlite;
using namespace std;
using namespace winrt::Windows::UI::Composition;

namespace winrt::HotelManagement::implementation
{
    int32_t RegisteredAccounts::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void RegisteredAccounts::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }


    void RegisteredAccounts::scrollMain_ViewChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs const& e)
    {
        if (!e.IsIntermediate())
        {
            /*
                
            if (stickyHeader().Height() >= 25)
                headerShow().Begin();
            else
                headerHide().Begin();
                */
        }
    }

    void RegisteredAccounts::stickyHeader_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::SizeChangedEventArgs const& e)
    {
        scrollMain().Margin(ThicknessHelper::FromLengths(0, stickyHeader().Height(), 0, 0));

    }


    void RegisteredAccounts::Page_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        previousOffset = stoppedOffset = 0;
        upOffset = 50;


        scrollMain().RegisterPropertyChangedCallback(ScrollViewer().VerticalOffsetProperty(), [&](DependencyObject s, DependencyProperty dp) {
            int h = 0;
            tempxt().Text(L"");
           

                if (scrollMain().VerticalOffset() > previousOffset)
                {
                    tempxt().Text(L"U ");
                    if (stickyHeader().Height() > 0 )
                    {
                        h = int(min(50, max(upOffset -scrollMain().VerticalOffset() + stoppedOffset, 0)));
                        stickyHeader().Height(h);
                        downOffset = 50;
                    }
                    else if (stickyHeader().Height() == 0)
                    {
                        stickyHeader().Height(0);
                        stoppedOffset = int(scrollMain().VerticalOffset());
                        downOffset = 0;
                    }


                }
                else {
                    tempxt().Text(L"D ");
                    if (stickyHeader().Height() < 50)
                    {
                        h = int(min(50, max(downOffset-scrollMain().VerticalOffset() + stoppedOffset, 0)));
                        stickyHeader().Height(h);
                        upOffset = 0;

                    }
                    else if (stickyHeader().Height() == 50)
                    {
                        stickyHeader().Height(50);
                        stoppedOffset = int(scrollMain().VerticalOffset());
                        upOffset = 50;
                    }

                }



            tempxt().Text(tempxt().Text() +
                to_hstring(int(scrollMain().VerticalOffset() - previousOffset)) + L"; " 
                + to_hstring(int(scrollMain().VerticalOffset())) + L"; " 
                + to_hstring(int(stoppedOffset)) + L"; "
                + to_hstring(int(previousOffset)) + L"; "
                + to_hstring(int(h)) + L"; "
                + to_hstring(int(stickyHeader().Height())) + L"; "
                + to_hstring(int(upOffset)) + L"; "
                + to_hstring(int(downOffset)) + L"; "
            
            );


          

            previousOffset = scrollMain().VerticalOffset();
          

        });

  

        database db(dbPath());

        db << "SELECT * FROM accounts;"
            >> [&](
                int id,
                string username,
                string password,
                string icon,
                string civil,
                string nationality,
                string firstName,
                string lastName,
                string middleName,
                string suffix,
                int gender,
                string birthday,
                string untBlck_etc,
                string barangay,
                string city,
                string province,
                string region,
                string country,
                string barangayCode,
                string cityCode,
                string provinceCode,
                string regionCode,
                string zip,
                string email,
                string teleNum,
                int isAdmin) -> winrt::fire_and_forget {    
                    // Source: https://stackoverflow.com/questions/68122223/how-can-you-co-await-in-a-c-winrt-lambda

                    auto stack = Grid();
                    auto rwdef1 = RowDefinition(); rwdef1.Height(GridLengthHelper::FromValueAndType(1, GridUnitType::Star));
                    auto rwdef2 = RowDefinition(); rwdef2.Height(GridLengthHelper::FromValueAndType(1, GridUnitType::Star));
                    auto coldef1 = ColumnDefinition(); coldef1.Width(GridLengthHelper::FromPixels(30));
                    auto coldef2 = ColumnDefinition(); coldef2.Width(GridLengthHelper::FromPixels(90));
                    auto coldef3 = ColumnDefinition(); coldef3.Width(GridLengthHelper::FromValueAndType(1, GridUnitType::Star));
                    stack.RowDefinitions().Append(rwdef1);
                    stack.RowDefinitions().Append(rwdef2);
                    stack.ColumnDefinitions().Append(coldef1);
                    stack.ColumnDefinitions().Append(coldef2);
                    stack.ColumnDefinitions().Append(coldef3);

                    if (isAdmin) {
                        auto coldef4 = ColumnDefinition(); coldef4.Width(GridLengthHelper::FromPixels(90));
                        stack.ColumnDefinitions().Append(coldef4);
                    }


                    auto txtNum = TextBlock();
                    txtNum.VerticalAlignment(VerticalAlignment::Center);
                    txtNum.HorizontalAlignment(HorizontalAlignment::Center);
                    txtNum.TextWrapping(TextWrapping::Wrap);
                    txtNum.Text(to_hstring(id));
                    Grid::SetColumn(txtNum, 0);
                    Grid::SetRowSpan(txtNum, 2);
                    stack.Children().Append(txtNum);


                    auto imgBitmap = ImageBrush();
                    imgBitmap.ImageSource(co_await Base64ToImage(to_hstring(icon)));

                    auto imgIcon = Border();
                    imgIcon.Height(65);
                    imgIcon.Width(65);
                    imgIcon.Margin(ThicknessHelper::FromLengths(5,20,5,20));
                    imgIcon.CornerRadius(CornerRadiusHelper::FromUniformRadius(5));
                    imgIcon.Background(imgBitmap);
                    Grid::SetColumn(imgIcon, 1);
                    Grid::SetRowSpan(imgIcon, 2);
                    stack.Children().Append(imgIcon);
                    
                    auto txtName = TextBlock();
                    txtName.FontFamily(
                        winrt::unbox_value<Windows::UI::Xaml::Media::FontFamily>(
                            Application::Current().Resources().Lookup(
                                winrt::box_value(L"Bold")
                            )
                        )
                    );
                    txtName.FontSize(17);
                    txtName.VerticalAlignment(VerticalAlignment::Bottom);
                    txtName.HorizontalAlignment(HorizontalAlignment::Stretch);
                    txtName.TextTrimming(TextTrimming::CharacterEllipsis);
                    txtName.Margin(ThicknessHelper::FromLengths(8, 2, 8, 2));
                    txtName.Text(
                        to_hstring(
                        lastName + ", " + firstName + (middleName.empty() ? "" : " " + middleName) + (suffix.empty() ? "" : " " + suffix)
                        )
                    );
                    Grid::SetColumn(txtName, 2);
                    Grid::SetRow(txtName, 0);
                    stack.Children().Append(txtName);

                    auto txtAddress = TextBlock();
                    txtAddress.Opacity(0.75);
                    txtAddress.VerticalAlignment(VerticalAlignment::Top);
                    txtAddress.HorizontalAlignment(HorizontalAlignment::Stretch);
                    txtAddress.TextTrimming(TextTrimming::CharacterEllipsis);
                    txtAddress.Margin(ThicknessHelper::FromLengths(8, 2, 15, 2));
                    txtAddress.Text(
                        to_hstring(
                            untBlck_etc + ", " +
                            std::regex_replace(barangay, std::regex("([^(]*)\\([^)]*\\)(.*)"), "$1$2") + ", " +
                            std::regex_replace(city, std::regex("([^(]*)\\([^)]*\\)(.*)"), "$1$2") + ", " +
                            std::regex_replace(province, std::regex("([^(]*)\\([^)]*\\)(.*)"), "$1$2")
                        )
                    );
                    Grid::SetColumn(txtAddress, 2);
                    Grid::SetRow(txtAddress, 1);
                    stack.Children().Append(txtAddress);


                    if (isAdmin) {
                        auto txtBrdrAdmin = Border();
                        txtBrdrAdmin.Background(SolidColorBrush(ColorHelper::FromArgb(150, 255,153,7)));
                        txtBrdrAdmin.CornerRadius(CornerRadiusHelper::FromUniformRadius(5));
                        txtBrdrAdmin.VerticalAlignment(VerticalAlignment::Center);
                        txtBrdrAdmin.HorizontalAlignment(HorizontalAlignment::Center);
                        txtBrdrAdmin.Margin(ThicknessHelper::FromLengths(8, 2, 12, 2));
                        txtBrdrAdmin.Padding(ThicknessHelper::FromLengths(8, 4, 8, 4));
                        auto txtAdmin = TextBlock();
                        txtAdmin.Text(L"ADMIN");

                        txtBrdrAdmin.Child(txtAdmin);
                        Grid::SetColumn(txtBrdrAdmin, 3);
                        Grid::SetRowSpan(txtBrdrAdmin, 2);
                        stack.Children().Append(txtBrdrAdmin);
                    }



                    accountList().Items().Append(stack);

        };

    }


    
}







void winrt::HotelManagement::implementation::RegisteredAccounts::headerShow_Completed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e)
{
    upOffset = 50;

}


void winrt::HotelManagement::implementation::RegisteredAccounts::headerHide_Completed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e)
{
    upOffset = 0;

}
