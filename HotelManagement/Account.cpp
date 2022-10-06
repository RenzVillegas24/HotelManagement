#include "pch.h"
#include "Account.h"
#if __has_include("Account.g.cpp")
#include "Account.g.cpp"
#endif

#include "Globals.h"
#include "winrt/Windows.UI.Popups.h"
#include <winrt/Windows.Globalization.DateTimeFormatting.h>
#include <regex>
#include <winrt/Windows.Storage.Pickers.h>


using namespace winrt;
using namespace Windows;
using namespace UI::Xaml;
using namespace Storage;
using namespace Foundation;
using namespace UI::Popups;
using namespace Controls;
using namespace Graphics::Imaging;
using namespace UI::Xaml::Media::Imaging;
using namespace UI::Xaml::Media::Animation;

using namespace sqlite;
using namespace std;



namespace winrt::HotelManagement::implementation
{
    int32_t Account::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Account::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
        
     
    }


    Windows::Foundation::IAsyncAction Account::Page_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
    {
        dbPath(to_string(localPath().Path()) + "\\data.db");
        database db(dbPath());

        string userLogged;
        int isRemembered = 0;

        db << "SELECT username, isRemembered FROM loggedin;"
            >> tie(userLogged, isRemembered);

        toggleStaySigned().IsOn(isRemembered);
        hstring imgStr;


        db << "SELECT * FROM accounts WHERE username = ?;"
            << userLogged
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
                int isAdmin) {

                    
                    imgStr = to_hstring(icon);

                    txtName().Text(
                        to_hstring(
                            lastName + ", " + firstName + (middleName.empty() ? "" : " " + middleName) + (suffix.empty() ? "" : " " + suffix)
                        )
                    );

                    //source: https://stackoverflow.com/questions/21021388/how-to-parse-a-date-string-into-a-c11-stdchrono-time-point-or-similar
                    std::tm t{};

                    sscanf_s(birthday.c_str(), "%d/%d/%d",
                        &t.tm_mon,
                        &t.tm_mday,
                        &t.tm_year
                    );
                    t.tm_year -= 1900;
                    --t.tm_mon;
                    t.tm_isdst = -1;  // guess if DST should be in effect when calling mktime

                    errno = 0;
                    auto tt = std::mktime(&t);

                    std::tm tm;
                    localtime_s(&tm, &tt);
                    char buff[70];
                    if (strftime(buff, sizeof buff, "%B %e, %Y", &tm)) {
                        puts(buff);
                    }
                    else {
                        puts("strftime failed");
                    }
                  

                    txtBirthday().Text(to_hstring(buff));
                    txtGender().Text(gender == 0 ? L"Male" : gender == 1 ? L"Female" : L"Others");
                    txtCivil().Text(to_hstring(civil));
                    txtNationality().Text(to_hstring(nationality));

                    //source: https://stackoverflow.com/questions/13879718/remove-characters-from-stdstring-from-to-with-erase
                    txtAddress().Text(
                        to_hstring(
                            untBlck_etc + ", " +
                            std::regex_replace(barangay, std::regex("([^(]*)\\([^)]*\\)(.*)"), "$1$2") + ", " +
                            std::regex_replace(city, std::regex("([^(]*)\\([^)]*\\)(.*)"), "$1$2") + ", " +
                            std::regex_replace(province, std::regex("([^(]*)\\([^)]*\\)(.*)"), "$1$2") 
                        )
                    );

                    txtZip().Text(to_hstring(zip));

                    txtContact().Text(to_hstring(teleNum));
                    txtEmail().Text(to_hstring(email));

                    txtUsername().Text(to_hstring(username));
                    txtPassword().Text(to_hstring(string(password.length(), '*')));

                    txtAdmin().Text(isAdmin ? L"Unregister this account from admin" : L"Register this account as admin");


        };

        auto img = co_await Base64ToImage(imgStr);
        imgID().ImageSource(img);
    }

    Windows::Foundation::IAsyncAction Account::btnLogout_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        auto res = co_await Dialog(
            L"Logging out",
            L"You are about to log out your account.\nWould you like to continue logging out?",
            L"No",
            L"Yes");


        if (res == ContentDialogResult::Primary)
        {
            Frame().Navigate(winrt::xaml_typename<Home>(), nullptr, DrillInNavigationTransitionInfo());
            int isRemembered;
            sqlite::database db(dbPath());

            db << "DELETE FROM loggedin;";
        }


    }


    void Account::btnStaySigned_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        toggleStaySigned().IsOn(!toggleStaySigned().IsOn());
    }


    void Account::toggleStaySigned_Toggled(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        database db(dbPath());

        string userLogged;

        db << "UPDATE loggedin SET isRemembered = ? WHERE username = ?;"
            << toggleStaySigned().IsOn()
            << to_string(txtUsername().Text());

    }


    Windows::Foundation::IAsyncAction Account::btnAdmin_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        
        database db(dbPath());
        int isAdmin;
        db << "SELECT isAdmin FROM accounts;"
            >> isAdmin;

        #pragma region "Generate Dialog"
        
            auto title = TextBlock(); 
            title.Text(isAdmin ? L"Downgrade account" : L"Upgrade account");

            auto marg = Thickness();
            marg.Bottom = 15; marg.Top = 5;

            auto subtitle = TextBlock(); 
            if (isAdmin)
                subtitle.Text(L"To downgrade please confirm your password then insert the admin password.\n(Note: This doesn't replace your password into admin password)");
            else 
                subtitle.Text(L"To register please confirm your password then insert the admin password.\n(Note: This doesn't replace your password into admin password)");
            subtitle.Width(400);
            subtitle.TextWrapping(TextWrapping::WrapWholeWords);
            subtitle.Margin(marg);

            marg.Bottom = 5;

            auto acctPass = PasswordBox(); 
            acctPass.PlaceholderText(L"Account Password");
            acctPass.Margin(marg);

            auto adminPass = PasswordBox(); 
            adminPass.PlaceholderText(L"Admin Password");
            adminPass.Margin(marg);

            auto stack = StackPanel(); 
            stack.Children().Append(subtitle);
            stack.Children().Append(acctPass);
            stack.Children().Append(adminPass);


            winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
            dialog.Title(title);

            dialog.Content(stack);
            dialog.CloseButtonText(L"Cancel");
            dialog.PrimaryButtonText(isAdmin? L"Downgrade Account" : L"Register Account");

        #pragma endregion
        
        auto res = co_await dialog.ShowAsync();


        if (res == ContentDialogResult::Primary) {

            string password, user;

            db << "SELECT username, password FROM loggedin;"
                >> tie(user, password);

            
            bool adminPassed = L"#qWeR3" == adminPass.Password(),
                acctPassed = to_hstring(password) == acctPass.Password();


            if (adminPassed && acctPassed) {
                

                db << "UPDATE accounts SET isAdmin = ? WHERE username = ?;"
                    << !isAdmin
                    << user;

                if (isAdmin) {
                    co_await Dialog(
                        L"Account downgraded successfully",
                        L"Your may now proceed.",
                        L"Ok"
                    );
                    txtAdmin().Text(L"Register this account as admin");
                }
                else {
                    co_await Dialog(
                        L"Account upgraded successfully",
                        L"Your account now has admin abilities.",
                        L"Ok"
                    );
                    txtAdmin().Text(L"Unregister this account from admin");
                }

            }
            else
            {
                hstring errCon = L"";
                if (!adminPassed && !acctPassed)
                    errCon = L"Account and admin password not matched.";

                else if (!adminPassed)
                    errCon = L"Admin password not matched.";

                else if (!acctPassed)
                    errCon = L"Account password not matched.";

                co_await Dialog(
                    L"Password error",
                    errCon,
                    L"Ok"
                );

            }
        }

       
    }


    Windows::Foundation::IAsyncAction Account::btnPassword_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {

        #pragma region "Generate Dialog"

            auto title = TextBlock();
            title.Text(L"Change password");

            auto marg = Thickness();
            marg.Bottom = 15; marg.Top = 5;

            auto subtitle = TextBlock();
            subtitle.Text(L"To change the pass please enter the old password then insert the new password.");
            subtitle.Width(400);
            subtitle.TextWrapping(TextWrapping::WrapWholeWords);
            subtitle.Margin(marg);

            marg.Bottom = 5;

            auto acctPass = PasswordBox();
            acctPass.PlaceholderText(L"Old Password");
            acctPass.Margin(marg);

            auto newPass = PasswordBox();
            newPass.PlaceholderText(L"New Password");
            newPass.Margin(marg);

            auto stack = StackPanel();
            stack.Children().Append(subtitle);
            stack.Children().Append(acctPass);
            stack.Children().Append(newPass);


            winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
            dialog.Title(title);
            

            dialog.Content(stack);
            dialog.CloseButtonText(L"Cancel");
            dialog.PrimaryButtonText(L"Confirm changes");

        #pragma endregion

        auto res = co_await dialog.ShowAsync();
        
        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string password, user;

            db << "SELECT username, password FROM loggedin;"
                >> tie(user, password);

            
            if (to_hstring(password) == acctPass.Password()) {

                if (newPass.Password().size() <= 6)
                    co_await Dialog(
                        L"Password error",
                        L"Password should be atleast 7 characters.",
                        L"Ok"
                    );
                else if (to_string(newPass.Password()).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._-?#@*$&!") != string::npos)
                    co_await Dialog(
                        L"Password error",
                        L"Password should contains alphabets, numbers and special characters (._-?#@*$&!) only.",
                        L"Ok"
                    );
                else {

                    db << "UPDATE accounts SET password = ? WHERE username = ?;"
                        << to_string(newPass.Password())
                        << user;

                    db << "UPDATE loggedin SET password = ? WHERE username = ?;"
                        << to_string(newPass.Password())
                        << user;

                    co_await Dialog(
                        L"Account updated successfully",
                        L"Your may now proceed.",
                        L"Ok"
                    );
                    txtPassword().Text(to_hstring(string('*', newPass.Password().size())));
                }
            }
            else
            {
                if (acctPass == newPass)
                    co_await Dialog(
                        L"Password error",
                        L"Old and new password matched.\nNo changes performed.",
                        L"Ok"
                    );
                else
                    co_await Dialog(
                        L"Password error",
                        L"Account password not matched.",
                        L"Ok"
                    );

            }
        }
    }


    Windows::Foundation::IAsyncAction Account::btnUsername_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        
        #pragma region "Generate Dialog"

            auto title = TextBlock();
            title.Text(L"Change username");

            auto marg = Thickness();
            marg.Bottom = 15; marg.Top = 5;

            auto subtitle = TextBlock();
            subtitle.Text(L"Please enter your updated username below.");
            subtitle.Width(400);
            subtitle.TextWrapping(TextWrapping::WrapWholeWords);
            subtitle.Margin(marg);

            marg.Bottom = 5;

            auto newUsername = TextBox();
            newUsername.PlaceholderText(L"New Username");
            newUsername.Margin(marg);

            auto stack = StackPanel();
            stack.Children().Append(subtitle);
            stack.Children().Append(newUsername);


            winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
            dialog.Title(title);

            dialog.Content(stack);
            dialog.CloseButtonText(L"Cancel");
            dialog.PrimaryButtonText(L"Confirm changes");

        #pragma endregion

        auto res = co_await dialog.ShowAsync();
        
        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user;

            db << "SELECT username FROM loggedin;"
                >> user;

            
            if (to_hstring(user) != newUsername.Text()) {
                if (newUsername.Text().size() <= 6) 
                    co_await Dialog(
                        L"Username error",
                        L"Username should be atleast 6 characters.",
                        L"Ok"
                    );
                else if (to_string(newUsername.Text()).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._-") != string::npos)
                    co_await Dialog(
                        L"Username error",
                        L"Username should contains alphabets, numbers and specific characters (._-) only.",
                        L"Ok"
                    );
                else {
                    db << "UPDATE accounts SET username = ? WHERE username = ?;"
                        << to_string(newUsername.Text())
                        << user;

                    db << "UPDATE loggedin SET username = ? WHERE username = ?;"
                        << to_string(newUsername.Text())
                        << user;
             
                    co_await Dialog(
                        L"Account updated successfully",
                        L"Your may now proceed.",
                        L"Ok"
                    );
                    txtUsername().Text(to_hstring(newUsername.Text()));
                }
            }
            else
            {
                
                co_await Dialog(
                    L"Username error",
                    L"Old and new username matched. \nNo changes performed.",
                    L"Ok"
                );
              

            }
        }
    }


    Windows::Foundation::IAsyncAction Account::btnEmail_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
#pragma region "Generate Dialog"

        auto title = TextBlock();
        title.Text(L"Change email");

        auto marg = Thickness();
        marg.Bottom = 15; marg.Top = 5;

        auto subtitle = TextBlock();
        subtitle.Text(L"Please enter your updated email below.");
        subtitle.Width(400);
        subtitle.TextWrapping(TextWrapping::WrapWholeWords);
        subtitle.Margin(marg);

        marg.Bottom = 5;

        auto newEmail = TextBox();
        newEmail.PlaceholderText(L"New Email");
        newEmail.Margin(marg);

        auto stack = StackPanel();
        stack.Children().Append(subtitle);
        stack.Children().Append(newEmail);


        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);

        dialog.Content(stack);
        dialog.CloseButtonText(L"Cancel");
        dialog.PrimaryButtonText(L"Confirm changes");

#pragma endregion

        auto res = co_await dialog.ShowAsync();

        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user, email;

            db << "SELECT username FROM loggedin;"
                >> user;


            db << "SELECT email FROM accounts WHERE username = ?;"
                << user
                >> email;

            if (to_hstring(user) != newEmail.Text()) {
                if (newEmail.Text().size() < 10)
                    co_await Dialog(
                        L"Email error",
                        L"Email should be atleast 10 characters.",
                        L"Ok"
                    );
                else if (to_string(newEmail.Text()).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._-?#@*$&!") != string::npos)
                    co_await Dialog(
                        L"Email error",
                        L"Email should contains alphabets, numbers and special characters (._-?#@*$&!) only.",
                        L"Ok"
                    );
                else {
                    db << "UPDATE accounts SET email = ? WHERE username = ?;"
                        << to_string(newEmail.Text())
                        << user;

                    co_await Dialog(
                        L"Account updated successfully",
                        L"Your may now proceed.",
                        L"Ok"
                    );
                    txtEmail().Text(to_hstring(newEmail.Text()));
                }
            }
            else
            {

                co_await Dialog(
                    L"Email error",
                    L"Old and new email matched. \nNo changes performed.",
                    L"Ok"
                );


            }
        }
    }


    Windows::Foundation::IAsyncAction Account::btnContact_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {

#pragma region "Generate Dialog"

        auto title = TextBlock();
        title.Text(L"Change contact number");

        auto marg = Thickness();
        marg.Bottom = 15; marg.Top = 5;

        auto subtitle = TextBlock();
        subtitle.Text(L"Please enter your updated contact number below.");
        subtitle.Width(400);
        subtitle.TextWrapping(TextWrapping::WrapWholeWords);
        subtitle.Margin(marg);

        marg.Bottom = 5;

        auto newContact = TextBox();
        newContact.PlaceholderText(L"New Contact Number");
        newContact.Margin(marg);

        auto stack = StackPanel();
        stack.Children().Append(subtitle);
        stack.Children().Append(newContact);


        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);

        dialog.Content(stack);
        dialog.CloseButtonText(L"Cancel");
        dialog.PrimaryButtonText(L"Confirm changes");

#pragma endregion

        auto res = co_await dialog.ShowAsync();

        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user, contact;

            db << "SELECT username FROM loggedin;"
                >> user;

            db << "SELECT teleNum FROM accounts WHERE username = ?;"
                << user
                >> contact;


            if (to_hstring(contact) != newContact.Text()) {
                if (newContact.Text().size() != 11)
                    co_await Dialog(
                        L"Contact numbers error",
                        L"Contact numbers should be 11 numbers only.",
                        L"Ok"
                    );
                else if (to_string(newContact.Text()).find_first_not_of("0123456789") != string::npos)
                    co_await Dialog(
                        L"Contact numbers error",
                        L"Contact number should contains numbers only.",
                        L"Ok"
                    );
                else {
                    db << "UPDATE accounts SET teleNum = ? WHERE username = ?;"
                        << to_string(newContact.Text())
                        << user;

                    co_await Dialog(
                        L"Account updated successfully",
                        L"Your may now proceed.",
                        L"Ok"
                    );
                    txtContact().Text(to_hstring(newContact.Text()));
                }
            }
            else
            {

                co_await Dialog(
                    L"Contact number error",
                    L"Old and new email matched. \nNo changes performed.",
                    L"Ok"
                );


            }
        }
    }


    Windows::Foundation::IAsyncAction Account::btnZip_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {

#pragma region "Generate Dialog"

        auto title = TextBlock();
        title.Text(L"Change zip code");

        auto marg = Thickness();
        marg.Bottom = 15; marg.Top = 5;

        auto subtitle = TextBlock();
        subtitle.Text(L"Please enter your updated zip code below.");
        subtitle.Width(400);
        subtitle.TextWrapping(TextWrapping::WrapWholeWords);
        subtitle.Margin(marg);

        marg.Bottom = 5;

        auto newZip = TextBox();
        newZip.PlaceholderText(L"New Zip Code");
        newZip.Margin(marg);

        auto stack = StackPanel();
        stack.Children().Append(subtitle);
        stack.Children().Append(newZip);


        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);

        dialog.Content(stack);
        dialog.CloseButtonText(L"Cancel");
        dialog.PrimaryButtonText(L"Confirm changes");

#pragma endregion

        auto res = co_await dialog.ShowAsync();

        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user, zip;

            db << "SELECT username FROM loggedin;"
                >> user;

            db << "SELECT zip FROM accounts WHERE username = ?;"
                << user
                >> zip;


            if (to_hstring(zip) != newZip.Text()) {
                if (newZip.Text().size() != 4)
                    co_await Dialog(
                        L"Zip code error",
                        L"Zip code should be 4 numbers only.",
                        L"Ok"
                    );
                else if (to_string(newZip.Text()).find_first_not_of("0123456789") != string::npos)
                    co_await Dialog(
                        L"Zip code error",
                        L"Zip code should contains numbers only.",
                        L"Ok"
                    );
                else {
                    db << "UPDATE accounts SET zip = ? WHERE username = ?;"
                        << to_string(newZip.Text())
                        << user;

                    co_await Dialog(
                        L"Account updated successfully",
                        L"Your may now proceed.",
                        L"Ok"
                    );
                    txtZip().Text(to_hstring(newZip.Text()));
                }
            }
            else
            {

                co_await Dialog(
                    L"Zip code error",
                    L"Old and new zip code matched. \nNo changes performed.",
                    L"Ok"
                );


            }
        }
    }


    Windows::Foundation::IAsyncAction Account::btnAddress_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
#pragma region "Generate Dialog"

        auto title = TextBlock();
        title.Text(L"Change address");

        auto marg = Thickness();
        marg.Bottom = 15; marg.Top = 5;

        auto subtitle = TextBlock();
        subtitle.Text(L"Please enter your updated address below.");
        subtitle.Width(400);
        subtitle.TextWrapping(TextWrapping::WrapWholeWords);
        subtitle.Margin(marg);
        marg.Bottom = 5;

        std::vector<std::array<std::string, 2>> regionS;
        std::vector<std::array<std::string, 2>> provinceS;
        std::vector<std::array<std::string, 2>> cityMunicipalS;
        std::vector<std::array<std::string, 2>> barangayS;
        std::string brgySelCode;


        //source: https://stackoverflow.com/questions/3745594/how-to-set-grid-row-and-column-positions-programmatically
        
        marg.Right = 5; marg.Left = 0;
        auto newCountry = ComboBox();
        Grid::SetColumn(newCountry, 0);
        newCountry.HorizontalAlignment(HorizontalAlignment::Stretch);
        newCountry.PlaceholderText(L"Country");

        auto txtPhil = TextBlock(); txtPhil.Text(L"Philippines");
        newCountry.Items().Append(txtPhil);
        newCountry.Margin(marg);


        marg.Right = 0; marg.Left = 5;
        auto newRegion = ComboBox();
        Grid::SetColumn(newRegion, 1);
        newRegion.HorizontalAlignment(HorizontalAlignment::Stretch);
        newRegion.PlaceholderText(L"Region");
        newRegion.Margin(marg);

        //source: https://learn.microsoft.com/en-us/windows/windows-app-sdk/api/winrt/microsoft.ui.xaml.gridlength?view=windows-app-sdk-1.1
        auto stack1 = Grid();
        auto rwdef1 = ColumnDefinition(); rwdef1.Width(GridLengthHelper::FromPixels(200));
        auto rwdef2 = ColumnDefinition(); rwdef2.Width(GridLengthHelper::FromPixels(200));
        stack1.ColumnDefinitions().Append(rwdef1);
        stack1.ColumnDefinitions().Append(rwdef2);
        stack1.Children().Append(newCountry);
        stack1.Children().Append(newRegion);



        marg.Right = 5; marg.Left = 0;
        auto newProvince = ComboBox();
        Grid::SetColumn(newProvince, 0);
        newProvince.HorizontalAlignment(HorizontalAlignment::Stretch);
        newProvince.PlaceholderText(L"Province");
        newProvince.Margin(marg);

        marg.Right = 0; marg.Left = 5;
        auto newCity = ComboBox();
        Grid::SetColumn(newCity, 1);
        newCity.HorizontalAlignment(HorizontalAlignment::Stretch);
        newCity.PlaceholderText(L"Region");
        newCity.Margin(marg);


        auto stack2  = Grid();
        auto rwdef3 = ColumnDefinition(); rwdef3.Width(GridLengthHelper::FromPixels(200));
        auto rwdef4 = ColumnDefinition(); rwdef4.Width(GridLengthHelper::FromPixels(200));
        stack2.ColumnDefinitions().Append(rwdef3);
        stack2.ColumnDefinitions().Append(rwdef4);
        stack2.Children().Append(newProvince);
        stack2.Children().Append(newCity);


        marg.Right = 0; marg.Left = 0;
        
        auto newBarangay = ComboBox();
        newBarangay.HorizontalAlignment(HorizontalAlignment::Stretch);
        newBarangay.PlaceholderText(L"Barangay");
        newBarangay.Margin(marg);


        auto newUnitBlck = TextBox();
        newUnitBlck.PlaceholderText(L"Unit/Building/Block etc.");
        newUnitBlck.Margin(marg);


#pragma region "Events"

        newCountry.SelectionChanged([&](winrt::Windows::Foundation::IInspectable const& sendr, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& f) {
            if (newCountry.SelectedItem() != nullptr) {
                sqlite_config config;
                config.flags = OpenFlags::READONLY;

                database db(to_string(localPath().Path() + L"\\refRegion.db"), config);
                newRegion.Items().Clear();
                regionS.clear();
                newProvince.Items().Clear();
                provinceS.clear();
                newCity.Items().Clear();
                cityMunicipalS.clear();
                newBarangay.Items().Clear();
                barangayS.clear();

                if (newCountry.SelectedItem().as<TextBlock>().Text() == L"Philippines")
                try
                {
                    db << "SELECT regDesc, regCode FROM refregion;"
                        >> [&](string region, string code) {

                        auto txt = Controls::TextBlock();
                        txt.Text(to_hstring(region));


                        newRegion.Items().Append(txt);
                        array<string, 2> data = { region, code };
                        regionS.push_back(data);
                    };
                }
                catch (exception& ef)
                {
                    Dialog(
                        L"SQL Error",
                        to_hstring(ef.what()),
                        L"Ok");
                }
            }
        });
        newRegion.SelectionChanged([&](winrt::Windows::Foundation::IInspectable const& sendr, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& f) {
            if (newRegion.SelectedItem() != nullptr) {
                sqlite_config config;
                config.flags = OpenFlags::READONLY;

                database db(to_string(localPath().Path() + L"\\refProvince.db"), config);
                newProvince.Items().Clear();
                provinceS.clear();
                newCity.Items().Clear();
                cityMunicipalS.clear();
                newBarangay.Items().Clear();
                barangayS.clear();

                try
                {
                    db << "SELECT provDesc, provCode FROM refprovince WHERE regCode = '" + regionS.at(newRegion.SelectedIndex()).at(1) + "';"
                        >> [&](string province, string code) {

                        auto txt = Controls::TextBlock();
                        txt.Text(to_hstring(province));


                        newProvince.Items().Append(txt);
                        array<string, 2> data = { province, code };
                        provinceS.push_back(data);

                    };
                }
                catch (exception& ef)
                {
                    Dialog(
                        L"SQL Error",
                        to_hstring(ef.what()),
                        L"Ok");
                }
            }
        });
        newProvince.SelectionChanged([&](winrt::Windows::Foundation::IInspectable const& sendr, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& f) {
            if (newProvince.SelectedItem() != nullptr) {
                sqlite_config config;
                config.flags = OpenFlags::READONLY;

                database db(to_string(localPath().Path() + L"\\refCitymun.db"), config);
                newCity.Items().Clear();
                cityMunicipalS.clear();
                newBarangay.Items().Clear();
                barangayS.clear();

                try
                {
                    db << "SELECT citymunDesc, citymunCode FROM refcitymun WHERE provCode = '" + provinceS.at(newProvince.SelectedIndex()).at(1) + "';"
                        >> [&](string city, string code) {

                        auto txt = Controls::TextBlock();
                        txt.Text(to_hstring(city));


                        newCity.Items().Append(txt);
                        array<string, 2> data = { city, code };
                        cityMunicipalS.push_back(data);

                    };
                }
                catch (exception& ef)
                {
                    Dialog(
                        L"SQL Error",
                        to_hstring(ef.what()),
                        L"Ok");
                }
            }
        });
        newCity.SelectionChanged([&](winrt::Windows::Foundation::IInspectable const& sendr, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& f) {
            if (newCity.SelectedItem() != nullptr) {
                sqlite_config config;
                config.flags = OpenFlags::READONLY;

                database db(to_string(localPath().Path() + L"\\refBrgy.db"), config);
                
                newBarangay.Items().Clear();
                barangayS.clear();

                try
                {
                    db << "SELECT brgyDesc, brgyCode FROM refbrgy WHERE citymunCode = '" + cityMunicipalS.at(newCity.SelectedIndex()).at(1) + "';"
                        >> [&](string barangay, string code) {

                        auto txt = Controls::TextBlock();
                        txt.Text(to_hstring(barangay));


                        newBarangay.Items().Append(txt);
                        array<string, 2> data = { barangay, code };
                        barangayS.push_back(data);

                    };
                }
                catch (exception& ef)
                {
                    Dialog(
                        L"SQL Error",
                        to_hstring(ef.what()),
                        L"Ok");
                }
            }
        });
        newBarangay.SelectionChanged([&](winrt::Windows::Foundation::IInspectable const& sendr, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& f) {
            if (newBarangay.SelectedItem() != nullptr) {
                sqlite_config config;
                config.flags = OpenFlags::READONLY;

                database db(to_string(localPath().Path() + L"\\refBrgy.db"), config);

                try
                {
                    db << "SELECT brgyCode FROM refbrgy WHERE "
                        " brgyDesc = '" + to_string(newBarangay.SelectedItem().as<TextBlock>().Text()) +
                        "' AND regCode = '" + regionS.at(newRegion.SelectedIndex()).at(1) +
                        "' AND provCode = '" + provinceS.at(newProvince.SelectedIndex()).at(1) +
                        "' AND citymunCode = '" + cityMunicipalS.at(newCity.SelectedIndex()).at(1) +
                        "';"
                        >> brgySelCode;

                }
                catch (exception& ef)
                {
                    Dialog(
                        L"SQL Error",
                        to_hstring(ef.what()),
                        L"Ok");
                }
            }
         });


        newCountry.DropDownOpened([&](winrt::Windows::Foundation::IInspectable const& sendr, winrt::Windows::Foundation::IInspectable const& f) {
            newCountry.SelectedItem(nullptr);
        });
        newRegion.DropDownOpened([&](winrt::Windows::Foundation::IInspectable const& sendr, winrt::Windows::Foundation::IInspectable const& f) {
            newRegion.SelectedItem(nullptr);
        });
        newProvince.DropDownOpened([&](winrt::Windows::Foundation::IInspectable const& sendr, winrt::Windows::Foundation::IInspectable const& f) {
            newProvince.SelectedItem(nullptr);
        });
        newCity.DropDownOpened([&](winrt::Windows::Foundation::IInspectable const& sendr, winrt::Windows::Foundation::IInspectable const& f) {
            newCity.SelectedItem(nullptr);
        });
        newBarangay.DropDownOpened([&](winrt::Windows::Foundation::IInspectable const& sendr, winrt::Windows::Foundation::IInspectable const& f) {
            newBarangay.SelectedItem(nullptr);
        });

#pragma endregion


        auto stack = StackPanel();
        stack.Children().Append(subtitle);
        stack.Children().Append(stack1);
        stack.Children().Append(stack2);
        stack.Children().Append(newBarangay);
        stack.Children().Append(newUnitBlck);


        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);

        dialog.Content(stack);
        dialog.CloseButtonText(L"Cancel");
        dialog.PrimaryButtonText(L"Confirm changes");

#pragma endregion

        auto res = co_await dialog.ShowAsync();

        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user;

            db << "SELECT username FROM loggedin;"
                >> user;


            if (newCountry.SelectedItem() == nullptr ||
                newRegion.SelectedItem() == nullptr || 
                newProvince.SelectedItem() == nullptr || 
                newCity.SelectedItem() == nullptr || 
                newBarangay.SelectedItem() == nullptr || 
                newUnitBlck.Text().empty())
                co_await Dialog(
                    L"Address error",
                    L"Complete the address below.",
                    L"Ok"
                );
            else {
                db << "UPDATE accounts SET "
                        "untBlck_etc = ?, "
                        "barangay = ?, "
                        "city = ?, "
                        "province = ?, "
                        "region = ?, "
                        "country = ?, "
                        "barangayCode = ?, "
                        "cityCode = ?, "
                        "provinceCode = ?, "
                        "regionCode = ? "
                        "WHERE username = ?;"
                    << to_string(newUnitBlck.Text())
                    << to_string(newBarangay.SelectedItem().as<TextBlock>().Text())
                    << to_string(newCity.SelectedItem().as<TextBlock>().Text())
                    << to_string(newProvince.SelectedItem().as<TextBlock>().Text())
                    << to_string(newRegion.SelectedItem().as<TextBlock>().Text())
                    << to_string(newCountry.SelectedItem().as<TextBlock>().Text())
                    << brgySelCode
                    << cityMunicipalS.at(newCity.SelectedIndex()).at(1)
                    << provinceS.at(newProvince.SelectedIndex()).at(1)
                    << regionS.at(newRegion.SelectedIndex()).at(1)
                    << user;

                co_await Dialog(
                    L"Account updated successfully",
                    L"Your may now proceed.",
                    L"Ok"
                );


                txtAddress().Text(
                    to_hstring(
                        to_string(newUnitBlck.Text()) + ", " +
                        std::regex_replace(to_string(newBarangay.SelectedItem().as<TextBlock>().Text()), std::regex("([^(]*)\\([^)]*\\)(.*)"), "$1$2") + ", " +
                        std::regex_replace(to_string(newCity.SelectedItem().as<TextBlock>().Text()), std::regex("([^(]*)\\([^)]*\\)(.*)"), "$1$2") + ", " +
                        std::regex_replace(to_string(newProvince.SelectedItem().as<TextBlock>().Text()), std::regex("([^(]*)\\([^)]*\\)(.*)"), "$1$2")
                    )
                );

            }
        }
    }


    Windows::Foundation::IAsyncAction Account::btnNationality_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {


#pragma region "Generate Dialog"

        auto title = TextBlock();
        title.Text(L"Change nationality");

        auto marg = Thickness();
        marg.Bottom = 15; marg.Top = 5;

        auto subtitle = TextBlock();
        subtitle.Text(L"Please enter your updated nationality below.");
        subtitle.Width(400);
        subtitle.TextWrapping(TextWrapping::WrapWholeWords);
        subtitle.Margin(marg);

        marg.Bottom = 5;

        auto newNationality = TextBox();
        newNationality.PlaceholderText(L"New Nationality");
        newNationality.Margin(marg);

        auto stack = StackPanel();
        stack.Children().Append(subtitle);
        stack.Children().Append(newNationality);


        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);

        dialog.Content(stack);
        dialog.CloseButtonText(L"Cancel");
        dialog.PrimaryButtonText(L"Confirm changes");

#pragma endregion

        auto res = co_await dialog.ShowAsync();

        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user, nationality;

            db << "SELECT username FROM loggedin;"
                >> user;


            db << "SELECT nationality FROM accounts WHERE username = ?;"
                << user
                >> nationality;

            if (to_hstring(user) != newNationality.Text()) {
              
                if (newNationality.Text().size() >= 3) {
                    db << "UPDATE accounts SET nationality = ? WHERE username = ?;"
                        << to_string(newNationality.Text())
                        << user;

                    co_await Dialog(
                        L"Account updated successfully",
                        L"Your may now proceed.",
                        L"Ok"
                    );
                    txtNationality().Text(to_hstring(newNationality.Text()));
                } else
                    co_await Dialog(
                        L"Nationality error",
                        L"Nationality characters should be 3 characters above.",
                        L"Ok"
                    );
            }
            else
            {
                co_await Dialog(
                    L"Nationality error",
                    L"Old and new nationality matched. \nNo changes performed.",
                    L"Ok"
                );


            }
        }
    }


    Windows::Foundation::IAsyncAction Account::btnCivil_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {

#pragma region "Generate Dialog"

        auto title = TextBlock();
        title.Text(L"Change civil status");

        auto marg = Thickness();
        marg.Bottom = 15; marg.Top = 5;

        auto subtitle = TextBlock();
        subtitle.Text(L"Please enter your updated civil status below.");
        subtitle.Width(400);
        subtitle.TextWrapping(TextWrapping::WrapWholeWords);
        subtitle.Margin(marg);
        marg.Bottom = 5;


        marg.Right = 5; marg.Left = 0;
        auto newCivil = ComboBox();
        newCivil.HorizontalAlignment(HorizontalAlignment::Stretch);
        newCivil.PlaceholderText(L"Civil status");

        auto item1 = TextBlock(); title.Text(L"Single");
        newCivil.Items().Append(item1);
        auto item2 = TextBlock(); title.Text(L"Married");
        newCivil.Items().Append(item2);
        auto item3 = TextBlock(); title.Text(L"Widow");
        newCivil.Items().Append(item3);
        auto item4 = TextBlock(); title.Text(L"Legally Seperated");
        newCivil.Items().Append(item4);



        auto stack = StackPanel();
        stack.Children().Append(subtitle);
        stack.Children().Append(newCivil);


        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);

        dialog.Content(stack);
        dialog.CloseButtonText(L"Cancel");
        dialog.PrimaryButtonText(L"Confirm changes");

#pragma endregion

        auto res = co_await dialog.ShowAsync();

        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user;

            db << "SELECT username FROM loggedin;"
                >> user;


            if (newCivil.SelectedItem() == nullptr)
                co_await Dialog(
                    L"Civil status error",
                    L"Civil status not yet selected.",
                    L"Ok"
                );
            else {
                db << "UPDATE accounts SET civil = ? WHERE username = ?;"
                    << to_string(newCivil.SelectedItem().as<TextBlock>().Text())
                    << user;

                co_await Dialog(
                    L"Account updated successfully",
                    L"Your may now proceed.",
                    L"Ok"
                );

                txtCivil().Text(newCivil.Text());

            }
        }
    }


    Windows::Foundation::IAsyncAction Account::btnGender_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {

#pragma region "Generate Dialog"

        auto title = TextBlock();
        title.Text(L"Change gender");

        auto marg = Thickness();
        marg.Bottom = 15; marg.Top = 5;

        auto subtitle = TextBlock();
        subtitle.Text(L"Please enter your updated gender below.");
        subtitle.Width(400);
        subtitle.TextWrapping(TextWrapping::WrapWholeWords);
        subtitle.Margin(marg);
        marg.Bottom = 5;


        marg.Right = 5; marg.Left = 0;
        auto newGender = ComboBox();
        newGender.HorizontalAlignment(HorizontalAlignment::Stretch);
        newGender.PlaceholderText(L"Gender");

        auto item1 = TextBlock(); title.Text(L"Male");
        newGender.Items().Append(item1);
        auto item2 = TextBlock(); title.Text(L"Female");
        newGender.Items().Append(item2);
        auto item3 = TextBlock(); title.Text(L"Other");
        newGender.Items().Append(item3);



        auto stack = StackPanel();
        stack.Children().Append(subtitle);
        stack.Children().Append(newGender);


        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);

        dialog.Content(stack);
        dialog.CloseButtonText(L"Cancel");
        dialog.PrimaryButtonText(L"Confirm changes");

#pragma endregion

        auto res = co_await dialog.ShowAsync();

        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user;

            db << "SELECT username FROM loggedin;"
                >> user;


            if (newGender.SelectedItem() == nullptr)
                co_await Dialog(
                    L"Civil status error",
                    L"Civil status not yet selected.",
                    L"Ok"
                );
            else {
                db << "UPDATE accounts SET civil = ? WHERE username = ?;"
                    << to_string(newGender.SelectedItem().as<TextBlock>().Text())
                    << user;

                co_await Dialog(
                    L"Account updated successfully",
                    L"Your may now proceed.",
                    L"Ok"
                );

                txtCivil().Text(newGender.Text());

            }
        }
    }


    Windows::Foundation::IAsyncAction Account::btnBirthday_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {

#pragma region "Generate Dialog"

        auto title = TextBlock();
        title.Text(L"Change gender");

        auto marg = Thickness();
        marg.Bottom = 15; marg.Top = 5;

        auto subtitle = TextBlock();
        subtitle.Text(L"Please enter your updated gender below.");
        subtitle.Width(400);
        subtitle.TextWrapping(TextWrapping::WrapWholeWords);
        subtitle.Margin(marg);
        marg.Bottom = 5;


        marg.Right = 5; marg.Left = 0;
        auto newBirthday = DatePicker();
        newBirthday.HorizontalAlignment(HorizontalAlignment::Stretch);


        auto stack = StackPanel();
        stack.Children().Append(subtitle);
        stack.Children().Append(newBirthday);


        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);

        dialog.Content(stack);
        dialog.CloseButtonText(L"Cancel");
        dialog.PrimaryButtonText(L"Confirm changes");

#pragma endregion

        auto res = co_await dialog.ShowAsync();

        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user;

            db << "SELECT username FROM loggedin;"
                >> user;


            if (newBirthday.SelectedDate() == nullptr)
                co_await Dialog(
                    L"Birthday error",
                    L"Birthday not yet selected.",
                    L"Ok"
                );
            else {
                auto bdayFmt = winrt::Windows::Globalization::DateTimeFormatting::DateTimeFormatter::DateTimeFormatter(L"{month.integer(2)}/{day.integer(2)}/{year.full}");
                db << "BEGIN;";

                db << "UPDATE accounts SET birthday = ? WHERE username = ?;"
                    << to_string(bdayFmt.Format(newBirthday.Date()))
                    << user;

                db << "COMMIT;";

                co_await Dialog(
                    L"Account updated successfully",
                    L"Your may now proceed.",
                    L"Ok"
                );

                bdayFmt = winrt::Windows::Globalization::DateTimeFormatting::DateTimeFormatter::DateTimeFormatter(L"{month.full} {day.integer}, {year.full}");
                txtBirthday().Text(bdayFmt.Format(newBirthday.Date()));

            }
        }
    }


    Windows::Foundation::IAsyncAction Account::btnName_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {

#pragma region "Generate Dialog"

        auto title = TextBlock();
        title.Text(L"Change name");

        auto marg = Thickness();
        marg.Bottom = 15; marg.Top = 5;

        auto subtitle = TextBlock();
        subtitle.Text(L"Please enter your updated name details below.");
        subtitle.Width(400);
        subtitle.TextWrapping(TextWrapping::WrapWholeWords);
        subtitle.Margin(marg);

        marg.Bottom = 5;

        auto newLastname = TextBox();
        newLastname.PlaceholderText(L"Last name");
        newLastname.Margin(marg);

        auto newFirstname = TextBox();
        newFirstname.PlaceholderText(L"First name");
        newFirstname.Margin(marg);



        marg.Right = 5; marg.Left = 0;
        auto newMiddlename = TextBox();
        Grid::SetColumn(newMiddlename, 0);
        newMiddlename.PlaceholderText(L"Middle name");
        newMiddlename.Margin(marg);

        marg.Right = 0; marg.Left = 5;
        auto newSuffix = TextBox();
        Grid::SetColumn(newSuffix, 1);
        newSuffix.PlaceholderText(L"Suffix");
        newSuffix.Margin(marg);


        auto stack1 = Grid();
        auto rwdef1 = ColumnDefinition(); rwdef1.Width(GridLengthHelper::FromValueAndType(3, GridUnitType::Star));
        auto rwdef2 = ColumnDefinition(); rwdef2.Width(GridLengthHelper::FromValueAndType(1, GridUnitType::Star));
        stack1.ColumnDefinitions().Append(rwdef1);
        stack1.ColumnDefinitions().Append(rwdef2);
        stack1.Children().Append(newMiddlename);
        stack1.Children().Append(newSuffix);


        auto stack = StackPanel();
        stack.Children().Append(subtitle);
        stack.Children().Append(newLastname);
        stack.Children().Append(newFirstname);
        stack.Children().Append(stack1);


        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);

        dialog.Content(stack);
        dialog.CloseButtonText(L"Cancel");
        dialog.PrimaryButtonText(L"Confirm changes");

#pragma endregion

        auto res = co_await dialog.ShowAsync();

        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user, lastName, firstName, middleName, suffix;

            db << "SELECT username FROM loggedin;"
               >> user;
            
            db << "SELECT lastName, firstName, middleName, suffix FROM accounts WHERE username = ?;"
               << user
               >> tie(lastName, firstName, middleName, suffix);

            if (to_hstring(lastName) != newLastname.Text() ||
                to_hstring(firstName) != newFirstname.Text() ||
                to_hstring(middleName) != newMiddlename.Text() ||
                to_hstring(suffix) != newSuffix.Text()
                ) {

                if (newLastname.Text().empty() ||
                    newFirstname.Text().empty() ||
                    newMiddlename.Text().empty() ||
                    newSuffix.Text().empty()
                    )

                    co_await Dialog(
                        L"Name details error",
                        L"Some details are empty.",
                        L"Ok"
                    );
                else
                {

                    db << "UPDATE accounts SET lastName = ?, firstName = ?, middleName = ?, suffix = ? WHERE username = ?;"
                        << to_string(newLastname.Text())
                        << to_string(newFirstname.Text())
                        << to_string(newMiddlename.Text())
                        << to_string(newSuffix.Text())
                        << user;

                    co_await Dialog(
                        L"Account updated successfully",
                        L"Your may now proceed.",
                        L"Ok"
                    );

                    txtName().Text(
                        newLastname.Text() + L", " + newFirstname.Text() + (newMiddlename.Text().empty() ? L"" : L" " + newMiddlename.Text()) + (newSuffix.Text().empty() ? L"" : L" " + newSuffix.Text())
                    );
                }


            }
            else
            {
                co_await Dialog(
                    L"Name details error",
                    L"Old and new name matched. \nNo changes performed.",
                    L"Ok"
                );


            }
        }
    }






    void Account::gridImg_PointerExited(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
    {
        if (!isImgLoading)
            imgHover().Begin();

    }


    void Account::gridImg_PointerEntered(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
    {
        if (!isImgLoading)
            imgOut().Begin();

    }


    Windows::Foundation::IAsyncAction Account::gridImg_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        isImgLoading = true;

        auto picker = Pickers::FileOpenPicker();
        picker.ViewMode(Pickers::PickerViewMode::Thumbnail);
        picker.SuggestedStartLocation(Pickers::PickerLocationId::PicturesLibrary);

        picker.FileTypeFilter().Append(L".jpg");
        picker.FileTypeFilter().Append(L".jpeg");
        picker.FileTypeFilter().Append(L".png");
        picker.FileTypeFilter().Append(L".bmp");
        picker.FileTypeFilter().Append(L".gif");


        imgLoading().Begin();
        auto imgIDFile = co_await picker.PickSingleFileAsync();

        if (imgIDFile != nullptr)
        {
            auto stream = co_await imgIDFile.OpenAsync(FileAccessMode::Read);

            auto bitmapImg = BitmapImage();
            co_await bitmapImg.SetSourceAsync(stream);

#pragma region "Generate Dialog"

            auto title = TextBlock();
            title.Text(L"Changing profile image");

            auto marg = Thickness();
            marg.Bottom = 15; marg.Top = 5;

            auto subtitle = TextBlock();
            subtitle.Text(L"This image will be used as your new profile picture.");
            subtitle.Width(400);
            subtitle.TextWrapping(TextWrapping::WrapWholeWords);
            subtitle.Margin(marg);
            marg.Bottom = 5;


            auto newImg = Border();
            newImg.HorizontalAlignment(HorizontalAlignment::Center);
            newImg.Width(200);
            newImg.Height(200);

            auto imgBrush = winrt::Windows::UI::Xaml::Media::ImageBrush::ImageBrush();
            imgBrush.Stretch(winrt::Windows::UI::Xaml::Media::Stretch::UniformToFill);
            imgBrush.ImageSource(bitmapImg);
            newImg.Background(imgBrush);

            auto corners = CornerRadius(); 
            corners.BottomLeft = corners.BottomRight = corners.TopLeft = corners.TopRight = 5;
            newImg.CornerRadius(corners);


            auto stack = StackPanel();
            stack.Children().Append(subtitle);
            stack.Children().Append(newImg);


            winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
            dialog.Title(title);

            dialog.Content(stack);
            dialog.CloseButtonText(L"Cancel");
            dialog.PrimaryButtonText(L"Confirm changes");

#pragma endregion

            auto res = co_await dialog.ShowAsync();

            if (res == ContentDialogResult::Primary)
            {
                database db(dbPath());




                auto decoder = co_await BitmapDecoder::CreateAsync(stream);
                auto imgOut = co_await ImageToBase64(imgIDFile, 250);

                string user;

                db << "SELECT username FROM loggedin;"
                    >> user;


                db << "UPDATE accounts SET icon = ? WHERE username = ?;"
                    << to_string(imgOut)
                    << user;

                imgID().ImageSource(bitmapImg);

                co_await Dialog(
                    L"Account updated successfully",
                    L"Your may now proceed.",
                    L"Ok"
                );


            }



#ifdef DEBUG

            auto hhh = co_await ImageToBase64(imgIDFile, 250);

            winrt::Windows::ApplicationModel::DataTransfer::DataPackage dataPackage = winrt::Windows::ApplicationModel::DataTransfer::DataPackage::DataPackage();
            dataPackage.SetText(hhh);
            winrt::Windows::ApplicationModel::DataTransfer::Clipboard::SetContent(dataPackage);

            const char* s = to_string(hhh).c_str();
            size_t start = 0;
            size_t end = 50;

            auto nnn = to_string(hhh).substr(start, end - start);

            co_await Dialog(
                L"Copied",
                to_hstring(nnn) + L"...",
                L"Ok");


            auto img = co_await Base64ToImage(hhh);
            imgID().ImageSource(img);


#endif // DEBUG


        }

        imgUnloading().Begin();
        isImgLoading = false;

    }


}





