#include "pch.h"
#include "LoginScreen.h"
#if __has_include("LoginScreen.g.cpp")
#include "LoginScreen.g.cpp"
#endif

#include "Globals.h"



#include "winrt/Windows.UI.Popups.h"
#include <winrt/Windows.UI.Xaml.Media.Animation.h>


using namespace winrt;
using namespace Windows::UI::Xaml;

using namespace winrt;

using namespace Windows::UI::Xaml::Media::Animation;

using namespace sqlite;
using namespace std;

using namespace Windows;
using namespace Foundation;
using namespace UI::Popups;
using namespace Controls;

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
					Content = L"The username and password textbox was empty,\nPlease try again.";
				else
					Content = L"The username textbox was empty,\nPlease try again.";
			else if (txtbxPassword().Password().empty())
				Content = L"The password textbox was empty,\nPlease try again.";




			co_await Dialog(TitleErr, Content, L"Ok");

			co_return;
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
					co_await Dialog(TitleErr, L"Password not matched,\nPlease try again.", L"Ok");

				}
				else
				{
					co_await Dialog(
						L"Logged in successfully!", 
						L"Welcome back " + txtbxUsername().Text() + L"\nYou may now proceed.", 
						L"Ok");


					db << "INSERT INTO loggedin ( "
						"username, "
						"password, "
						"isRemembered "
						")"
						"VALUES (" 
						" '" + to_string(txtbxUsername().Text()) + "'," +
						" '" + to_string(txtbxPassword().Password()) + "'," +
						" '" + std::to_string(Chk_Remember().IsChecked().GetBoolean() ? 1 : 0) + "'" +
						");";

					
					auto t = SlideNavigationTransitionInfo();
					t.Effect(SlideNavigationTransitionEffect::FromRight);

					Frame().Navigate(winrt::xaml_typename<Account>(), nullptr, t);

					Frame().Parent().as<Microsoft::UI::Xaml::Controls::NavigationView>().MenuItems().GetAt(4).as<Microsoft::UI::Xaml::Controls::NavigationViewItem>().MenuItems().GetAt(1)
						.as<Microsoft::UI::Xaml::Controls::NavigationViewItem>().Visibility(Visibility::Visible);

					Frame().Parent().as<Microsoft::UI::Xaml::Controls::NavigationView>().MenuItems().GetAt(1).as<Microsoft::UI::Xaml::Controls::NavigationViewItem>().Visibility(Visibility::Visible);


					int isAdmin;

					db << "SELECT isAdmin FROM accounts WHERE username = ?;"
						<< to_string(txtbxUsername().Text().c_str())
						>> isAdmin;

					if (isAdmin)
						Frame().Parent().as<Microsoft::UI::Xaml::Controls::NavigationView>().MenuItems().GetAt(3).as<Microsoft::UI::Xaml::Controls::NavigationViewItem>().Visibility(Visibility::Visible);
				}
			}
			else {
				auto t = SlideNavigationTransitionInfo();
				t.Effect(SlideNavigationTransitionEffect::FromRight);

				auto res = co_await Dialog(
					TitleErr,
					L"Account not found!\nWould you like to create account instead?",
					L"No",
					L"Yes");

				if (res == ContentDialogResult::Primary)
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
#ifdef DEBUG
			winrt::Windows::UI::Popups::MessageDialog dialog(dbPathH(), L"Local");
			dialog.ShowAsync();
#endif // DEBUG

	}

	void LoginScreen::Page_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
	{

	}

}
