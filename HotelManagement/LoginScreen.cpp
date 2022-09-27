#include "pch.h"
#include "LoginScreen.h"
#if __has_include("LoginScreen.g.cpp")
#include "LoginScreen.g.cpp"
#endif


#include "GlobalVars.h"
#include "winrt/Windows.UI.Popups.h"
#include <winrt/Windows.UI.Xaml.Media.Animation.h>


using namespace winrt;
using namespace Windows::UI::Xaml;

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Popups;

using namespace Windows::UI::Xaml::Media::Animation;

using namespace sqlite;
using namespace std;

namespace winrt::HotelManagement::implementation
{
    int32_t LoginScreen::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void LoginScreen::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }



    IAsyncAction LoginScreen::btnLogin_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {

        hstring TitleErr = L"Login Error";

        if (txtbxUsername().Text().empty() ||
            txtbxPassword().Password().empty() ) {
            hstring Content = L"";
            if (txtbxUsername().Text().empty())
                if (txtbxPassword().Password().empty())
                    Content = L"The username and password textbox was empty, please try again.";
                else
                    Content = L"The username textbox was empty, please try again.";
            else if (txtbxPassword().Password().empty())
                Content = L"The password textbox was empty, please try again.";



            MessageDialog dialog(Content, TitleErr);
            dialog.ShowAsync();

            return;
        }

        string password;
        if (GlobalVars().isLoggedIn())
        {
             database db(GlobalVars().dbPath());

             db << "select password from accounts where username = ?;"
                << to_string(txtbxUsername().Text().c_str())
                >> password;


            if (password != to_string(txtbxPassword().Password().c_str()))
            {
                hstring Content = L"Password not matched, please try again.";
                MessageDialog dialog(Content, TitleErr);
                co_await dialog.ShowAsync();
            }
            else 
            {
                auto t = SlideNavigationTransitionInfo();
                t.Effect(SlideNavigationTransitionEffect::FromRight);

                Frame().Navigate(winrt::xaml_typename<Account>(), nullptr, t);
            }
        }
        else {
            hstring Content = L"Account not found!\nWould you like to create account using this username instead?";
            MessageDialog dialog(Content, TitleErr);
            dialog.Commands().Append(UICommand(L"Yes", nullptr));
            dialog.Commands().Append(UICommand(L"No", nullptr));
            auto& result = co_await dialog.ShowAsync();


            auto t = SlideNavigationTransitionInfo();
            t.Effect(SlideNavigationTransitionEffect::FromRight);

            if (result.Label() == L"Yes")
                Frame().Navigate(winrt::xaml_typename<SignUpForm>(), nullptr, t);
        }
    }


    void LoginScreen::btnSignUp_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {

        auto t = SlideNavigationTransitionInfo();
        t.Effect(SlideNavigationTransitionEffect::FromRight);

        Frame().Navigate(winrt::xaml_typename<SignUpForm>(), nullptr, t);

    }

    void LoginScreen::btnForgot_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {

    }

    void LoginScreen::Page_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
    {

    }

}
