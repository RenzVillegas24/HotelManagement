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
        if (isHeaderSticky()) {
            scrollMain().Margin(ThicknessHelper::FromLengths(0, stickyHeader().Height(), 0, 0));
        }
    }
    
    Windows::Foundation::IAsyncAction RegisteredAccounts::appendDataToList(int isAdmin, int id, string lastName, string firstName, string middleName, string suffix, string birthday, int age, string icon, string province, string city, string barangay, string untBlck_etc ) {

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
        imgIcon.Margin(ThicknessHelper::FromLengths(5, 20, 5, 20));
        imgIcon.CornerRadius(CornerRadiusHelper::FromUniformRadius(5));
        imgIcon.Background(imgBitmap);
        Grid::SetColumn(imgIcon, 1);
        Grid::SetRowSpan(imgIcon, 2);
        stack.Children().Append(imgIcon);

        auto txtName = TextBlock();
        txtName.VerticalAlignment(VerticalAlignment::Bottom);
        txtName.HorizontalAlignment(HorizontalAlignment::Stretch);
        txtName.TextTrimming(TextTrimming::CharacterEllipsis);
        txtName.Margin(ThicknessHelper::FromLengths(8, 2, 8, 2));

        auto txtInlineName = Documents::Run();
        txtInlineName.FontSize(17);
        txtInlineName.FontFamily(
            winrt::unbox_value<Windows::UI::Xaml::Media::FontFamily>(
                Application::Current().Resources().Lookup(
                    winrt::box_value(L"Bold")
                )
            )
        );
        txtInlineName.Text(
            to_hstring(
                lastName + ", " + firstName + (middleName.empty() ? "" : " " + middleName) + (suffix.empty() ? "" : " " + suffix) 
            )
        );

        auto txtInlineAge = Documents::Run();
        txtInlineAge.FontSize(13);
        txtInlineAge.FontFamily(
            winrt::unbox_value<Windows::UI::Xaml::Media::FontFamily>(
                Application::Current().Resources().Lookup(
                    winrt::box_value(L"Light")
                )
            )
        );
        txtInlineAge.Text(
            to_hstring(
                to_hstring(
                    "    " + std::to_string(age) + " years old"
                )
            )
        );

        txtName.Inlines().Append(txtInlineName);
        txtName.Inlines().Append(txtInlineAge);




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
            txtBrdrAdmin.Background(SolidColorBrush(ColorHelper::FromArgb(150, 255, 153, 7)));
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
    }
    


    void RegisteredAccounts::Page_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::SizeChangedEventArgs const& e)
    {
        if (isHeaderSticky()) {

            stickyHeader().Height(50);
            upOffset = 50;
            downOffset = 0;
            if (scrollMain().VerticalOffset() == scrollMain().ScrollableHeight())
                scrollMain().ScrollToVerticalOffset(scrollMain().VerticalOffset() - 1);
        }
    }

    void RegisteredAccounts::Page_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        previousOffset = stoppedOffset = 0;
        upOffset = 50;

        if (isHeaderSticky()) {

            scrollMain().RegisterPropertyChangedCallback(ScrollViewer().VerticalOffsetProperty(), [&](DependencyObject s, DependencyProperty dp) {
                int h = 0;

    #ifdef DEBUG
                tempxt().Text(L"");
    #endif
                    if (scrollMain().VerticalOffset() > previousOffset)
                    {
    #ifdef DEBUG
                        tempxt().Text(L"U ");
    #endif

                        if (stickyHeader().Height() > 0)
                        {
                            h = int(min(50, max(upOffset - scrollMain().VerticalOffset() + stoppedOffset, 0)));
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
    #ifdef DEBUG
                        tempxt().Text(L"D ");
    #endif
                        if (stickyHeader().Height() < 50)
                        {
                            h = int(min(50, max(downOffset - scrollMain().VerticalOffset() + stoppedOffset, 0)));
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

    #ifdef DEBUG
                    tempxt().Text(tempxt().Text() +
                        to_hstring(int(scrollMain().VerticalOffset() - previousOffset)) + L"; "
                        + to_hstring(int(scrollMain().VerticalOffset())) + L"; "
                        + to_hstring(int(stoppedOffset)) + L"; "
                        + to_hstring(int(previousOffset)) + L"; "
                        + to_hstring(int(h)) + L"; "
                        + to_hstring(int(stickyHeader().Height())) + L"; "
                        + to_hstring(int(upOffset)) + L"; "
                        + to_hstring(int(downOffset)) + L"; "
                        + to_hstring(int(scrollMain().ScrollableHeight())) + L"; "
                    );
    #endif

                    previousOffset = scrollMain().VerticalOffset();
            });

        }
        else {
            accountList().Padding(ThicknessHelper::FromLengths(20, 140, 20, 20));
        }


        cmbxFilter().SelectedIndex(0);
        cmbxSort().SelectedIndex(0);
    }





    void RegisteredAccounts::headerShow_Completed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e)
    {

        if (isHeaderSticky()) {
            upOffset = 50;
        }

    }


    void RegisteredAccounts::headerHide_Completed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e)
    {
        if (isHeaderSticky()) {
            upOffset = 0;
        }
    }




    void RegisteredAccounts::cmbxFilter_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {
        accountList().Items().Clear();
        hstring result = cmbxFilter().SelectedItem().as<TextBlock>().Text(),
                result_sort = L"";

        if (cmbxSort().SelectedIndex() > -1)
            result_sort = cmbxSort().SelectedItem().as<Grid>().Tag().as<hstring>();
    
        string command = string(
            R"(
                WITH dates AS (
	                SELECT 
	                id,
	                substr(birthday,7,4)||'-'||substr(birthday,1,2)||'-'||substr(birthday,4,2) AS dateconv
	                FROM accounts 
                )
                SELECT
	                *
                FROM accounts INNER JOIN (
	                SELECT
		                date(
			                strftime('%Y-%m-%d', 'now'),
			                '-'||(strftime('%m',dates.dateconv)-1)||' months',
			                '-'||(strftime('%d',dates.dateconv)-1)||' days')-dates.dateconv AS age,
		                id
	                FROM dates
                ) t 
                ON accounts.id = t.id )");

        if (result == L"Admin") 
            command += " WHERE isAdmin = 1";
        else if (result == L"Non-Admin") 
            command += " WHERE isAdmin = 0";
        
        if (result_sort == L"asc_Creation")
            command += " ORDER BY id ASC";
        else if (result_sort == L"dsc_Creation")
            command += " ORDER BY id DESC";
        else if (result_sort == L"asc_Surname")
            command += " ORDER BY lastName ASC";
        else if (result_sort == L"dsc_Surname")
            command += " ORDER BY lastName DESC";
        else if (result_sort == L"asc_Age")
            command += " ORDER BY age ASC";
        else if (result_sort == L"dsc_Age")
            command += " ORDER BY age DESC";

        command += ";";


        database db(dbPath());
        int i = 0;

        db << command
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
                int isAdmin,
                int age) -> winrt::fire_and_forget {
                    // Source: https://stackoverflow.com/questions/68122223/how-can-you-co-await-in-a-c-winrt-lambda
                    co_await appendDataToList(isAdmin, i+=1, lastName, firstName, middleName, suffix, birthday, age, icon, province, city, barangay, untBlck_etc);

        };
        
    }


    void RegisteredAccounts::cmbxSort_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {

            accountList().Items().Clear();
            hstring result = cmbxSort().SelectedItem().as<Grid>().Tag().as<hstring>(),
                    result_filter = L"";
            
            if (cmbxFilter().SelectedIndex() > -1)
                result_filter = cmbxFilter().SelectedItem().as<TextBlock>().Text();

            string command = string(
                R"(
                WITH dates AS (
	                SELECT 
	                id,
	                substr(birthday,7,4)||'-'||substr(birthday,1,2)||'-'||substr(birthday,4,2) AS dateconv
	                FROM accounts 
                )
                SELECT
	                *
                FROM accounts INNER JOIN (
	                SELECT
		                date(
			                strftime('%Y-%m-%d', 'now'),
			                '-'||(strftime('%m',dates.dateconv)-1)||' months',
			                '-'||(strftime('%d',dates.dateconv)-1)||' days')-dates.dateconv AS age,
		                id
	                FROM dates
                ) t 
                ON accounts.id = t.id )");

            if (result_filter == L"Admin")
                command += " WHERE isAdmin = 1";
            else if (result_filter == L"Non-Admin")
                command += " WHERE isAdmin = 0";

            if (result == L"asc_Creation") 
                command += " ORDER BY id ASC";
            else if (result == L"dsc_Creation") 
                command += " ORDER BY id DESC";
            else if (result == L"asc_Surname")
                command += " ORDER BY lastName ASC";
            else if (result == L"dsc_Surname") 
                command += " ORDER BY lastName DESC";
            else if (result == L"asc_Age") 
                command += " ORDER BY age ASC";
            else if (result == L"dsc_Age") 
                command += " ORDER BY age DESC";

            command += ";";


            database db(dbPath());
            int i = 0;

            db << command
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
                    int isAdmin,
                    int age) -> winrt::fire_and_forget {
                        // Source: https://stackoverflow.com/questions/68122223/how-can-you-co-await-in-a-c-winrt-lambda
                        co_await appendDataToList(isAdmin, i += 1, lastName, firstName, middleName, suffix, birthday, age, icon, province, city, barangay, untBlck_etc);

            };
    }

    
}


