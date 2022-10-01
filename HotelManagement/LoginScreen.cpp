#include "pch.h"
#include "LoginScreen.h"
#if __has_include("LoginScreen.g.cpp")
#include "LoginScreen.g.cpp"
#endif



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
			txtbxPassword().Password().empty()) {
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
		if (!isLoggedIn())
		{
			bool isAccountPresent = true;
			database db(dbPath());

			try {

				db << "SELECT password FROM accounts WHERE username = ?;"
					<< to_string(txtbxUsername().Text().c_str())
					>> password;

			}
			catch (exception& e) {
				isAccountPresent = false;
			}

			if (isAccountPresent) {
				if (password != to_string(txtbxPassword().Password().c_str()))
				{
					MessageDialog dialog(L"Password not matched, please try again.", TitleErr);
					co_await dialog.ShowAsync();
				}
				else
				{

					MessageDialog dialog(L"Welcome back " + txtbxUsername().Text() + L"!\nYou have successfully logged in your account. You may now proceed.", L"Logged in successfully!");
					co_await dialog.ShowAsync();

					db << "INSERT INTO loggedin ( "
						"username, "
						"password, "
						"isRemembered "
						")"
						"VALUES (" 
						" '" + to_string(txtbxUsername().Text()) + " '," +
						" '" + to_string(txtbxPassword().Password()) + " '," +
						" '" + std::to_string(Chk_Remember().IsChecked() ?  1: 0) + " '" +
						");";

					
					auto t = SlideNavigationTransitionInfo();
					t.Effect(SlideNavigationTransitionEffect::FromRight);

					Frame().Navigate(winrt::xaml_typename<Account>(), nullptr, t);
				}
			}
			else {
				MessageDialog dialog(L"Account not found!\nWould you like to create account using this username instead?", TitleErr);
				dialog.Commands().Append(UICommand(L"Yes", nullptr));
				dialog.Commands().Append(UICommand(L"No", nullptr));
				auto& result = co_await dialog.ShowAsync();


				auto t = SlideNavigationTransitionInfo();
				t.Effect(SlideNavigationTransitionEffect::FromRight);

				if (result.Label() == L"Yes")
					Frame().Navigate(winrt::xaml_typename<SignUpForm>(), nullptr, t);
			}
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
		winrt::Windows::UI::Popups::MessageDialog dialog(dbPathH(), L"Local");

		dialog.ShowAsync();
	}

	void LoginScreen::Page_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
	{

	}

}
