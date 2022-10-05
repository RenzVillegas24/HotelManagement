#include "pch.h"
#include "Account.h"
#if __has_include("Account.g.cpp")
#include "Account.g.cpp"
#endif

#include "Globals.h"
#include "winrt/Windows.UI.Popups.h"
#include <winrt/Windows.Globalization.DateTimeFormatting.h>
#include <regex>


using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::Foundation;
using namespace winrt::Windows::UI::Popups;
using namespace Controls;
using namespace Media::Animation;

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
                            std::regex_replace(province, std::regex("([^(]*)\\([^)]*\\)(.*)"), "$1$2") +
                            std::regex_replace(city, std::regex("([^(]*)\\([^)]*\\)(.*)"), "$1$2") +
                            std::regex_replace(barangay, std::regex("([^(]*)\\([^)]*\\)(.*)"), "$1$2") 
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
            dialog.PrimaryButtonText(isAdmin? L"Downgrade" : L"Register");

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
                        L"Account downgrade successful",
                        L"Your may now proceed.",
                        L"Ok"
                    );
                    txtAdmin().Text(L"Register this account as admin");
                }
                else {
                    co_await Dialog(
                        L"Account upgrade successful",
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
            subtitle.Margin(marg);

            marg.Bottom = 5;

            auto acctPass = PasswordBox();
            acctPass.PlaceholderText(L"Old Account Password");
            acctPass.Margin(marg);

            auto newPass = PasswordBox();
            newPass.PlaceholderText(L"New Account Password");
            newPass.Margin(marg);

            auto stack = StackPanel();
            stack.Children().Append(subtitle);
            stack.Children().Append(acctPass);
            stack.Children().Append(newPass);


            winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
            dialog.Title(title);
            

            dialog.Content(stack);
            dialog.CloseButtonText(L"Cancel");
            dialog.PrimaryButtonText(L"Change password");

        #pragma endregion

        auto res = co_await dialog.ShowAsync();
        
        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string password, user;

            db << "SELECT username, password FROM loggedin;"
                >> tie(user, password);

            
            if (to_hstring(password) == acctPass.Password()) {

                db << "UPDATE accounts SET password = ? WHERE username = ?;"
                    << to_string(newPass.Password())
                    << user;

                db << "UPDATE loggedin SET password = ? WHERE username = ?;"
                    << to_string(newPass.Password())
                    << user;
             
                co_await Dialog(
                    L"Account password updated successful",
                    L"Your may now proceed.",
                    L"Ok"
                );

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
            subtitle.Text(L"Please enter the new username below.");
            subtitle.Margin(marg);

            marg.Bottom = 5;

            auto newUsername = TextBox();
            newUsername.PlaceholderText(L"New Account Password");
            newUsername.Margin(marg);

            auto stack = StackPanel();
            stack.Children().Append(subtitle);
            stack.Children().Append(newUsername);


            winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
            dialog.Title(title);

            dialog.Content(stack);
            dialog.CloseButtonText(L"Cancel");
            dialog.PrimaryButtonText(L"Change username");

        #pragma endregion

        auto res = co_await dialog.ShowAsync();
        
        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user;

            db << "SELECT username FROM loggedin;"
                >> user;

            
            if (to_hstring(user) != newUsername.Text()) {

                db << "UPDATE accounts SET username = ? WHERE username = ?;"
                    << to_string(newUsername.Text())
                    << user;

                db << "UPDATE loggedin SET password = ? WHERE username = ?;"
                    << to_string(newUsername.Text())
                    << user;
             
                co_await Dialog(
                    L"Account username updated successful",
                    L"Your may now proceed.",
                    L"Ok"
                );

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
}





