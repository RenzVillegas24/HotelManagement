#include "pch.h"
#include "Account.h"
#if __has_include("Account.g.cpp")
#include "Account.g.cpp"
#endif

#include "Globals.h"
#include "winrt/Windows.UI.Popups.h"
#include <winrt/Windows.Globalization.DateTimeFormatting.h>


using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::Foundation;
using namespace winrt::Windows::UI::Popups;

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

        db << "SELECT username FROM loggedin;"
            >> userLogged;

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

                    txtZip().Text(to_hstring(zip));

                    txtContact().Text(to_hstring(teleNum));
                    txtEmail().Text(to_hstring(email));

                    txtUsername().Text(to_hstring(username));
                    txtPassword().Text(to_hstring(string(password.length(), '*')));

        };

        auto img = co_await Base64ToImage(imgStr);
        imgID().ImageSource(img);
    }

}




