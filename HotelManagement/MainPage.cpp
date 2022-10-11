#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"
#include "winrt/Windows.ApplicationModel.Core.h"
#include "winrt/Windows.UI.ViewManagement.h"
#include "../../../../../../Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/winrt/wrl/internal.h"
#include "winrt/Windows.UI.Xaml.Input.h"
#include <iostream>

#include "winrt/Windows.UI.Popups.h"
#include "Globals.h"


using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Input;

//titlebar
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::ApplicationModel::Core;
using namespace Animation;

using namespace sqlite;
using namespace std;

namespace winrt::HotelManagement::implementation
{



    int32_t MainPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();

    }

    //https://stackoverflow.com/questions/57139554/how-to-copy-a-file-from-visual-studio-project-to-uwp-applications-localfolder-o
    Windows::Foundation::IAsyncAction MainPage::SendFileToLocal(hstring fromFolder, hstring fileName)
    {
        Windows::Storage::StorageFile file{
             co_await Windows::Storage::StorageFile::GetFileFromApplicationUriAsync(Windows::Foundation::Uri{L"ms-appx:///Assets/" + fromFolder + L"/" + fileName})
        };
        co_await file.CopyAsync(localPath(), fileName, Windows::Storage::NameCollisionOption::ReplaceExisting);
    }



    // List of ValueTuple holding the Navigation Tag and the relative Navigation Page
    vector<pair<wstring, Windows::UI::Xaml::Interop::TypeName>> m_pages;


    void MainPage::MainPivot_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
    {
        dbPath(to_string(localPath().Path()) + "\\data.db");

        //Send assets to local files
        SendFileToLocal(L"Database", L"refBrgy.db");
        SendFileToLocal(L"Database", L"refCitymun.db");
        SendFileToLocal(L"Database", L"refProvince.db");
        SendFileToLocal(L"Database", L"refRegion.db");

   

        m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName>
            (L"home", winrt::xaml_typename<Home>()));
        m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName>
            (L"home_events", winrt::xaml_typename<Events>()));
        m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName>
            (L"home_gyms", winrt::xaml_typename<Gyms>()));
        m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName>
            (L"home_lobby", winrt::xaml_typename<Lobby>()));
        m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName>
            (L"home_pools", winrt::xaml_typename<Pools>()));
        m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName>
            (L"home_restaurant", winrt::xaml_typename<Restaurant>()));
        m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName>
            (L"home_units", winrt::xaml_typename<Units>()));
        m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName>
            (L"bookings", winrt::xaml_typename<Booking>()));

        m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName>
            (L"support", winrt::xaml_typename<Support>()));
        m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName>
            (L"registered", winrt::xaml_typename<RegisteredAccounts>()));
        m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName>
            (L"accounts", winrt::xaml_typename<Account>()));

        m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName>
            (L"account_createAcct", winrt::xaml_typename<SignUpForm>()));


        //m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName> (L"registered", winrt::xaml_typename<SignUpForm>()));
        //m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName> (L"payment", winrt::xaml_typename<Payment>()));
        

        //minimum size
        ApplicationView::GetForCurrentView().SetPreferredMinSize(Size(500, 500));

        //extend titlebar
        Window::Current().SetTitleBar(AppTitle());
        CoreApplication::GetCurrentView().TitleBar().ExtendViewIntoTitleBar(true);
        ApplicationViewTitleBar titleBar = ApplicationView::GetForCurrentView().TitleBar();

        titleBar.ButtonBackgroundColor(Colors::Transparent());
        titleBar.ButtonInactiveBackgroundColor(Colors::Transparent());

        // Add handler for ContentFrame navigation.
        ContentFrame().Navigated({ this, &MainPage::On_Navigated });
        Windows::UI::Core::SystemNavigationManager::GetForCurrentView().BackRequested({ this, &MainPage::System_BackRequested });

        //load home page
        navView().SelectedItem(navView().MenuItems().GetAt(0));
        navView_Navigate(L"home", Animation::EntranceNavigationTransitionInfo());


        //NavigationCacheMode(Navigation::NavigationCacheMode::Enabled);


        if (isLoggedIn()) {
            database db(dbPath());

            string userLogged;
            int isAdmin;

            db << "SELECT username, isRemembered FROM loggedin;"
                >> userLogged;

            db << "SELECT isAdmin FROM accounts WHERE username = ?;"
                << userLogged
                >> isAdmin;

            navView().MenuItems().GetAt(3).as<Microsoft::UI::Xaml::Controls::NavigationViewItem>().Visibility(isAdmin ? Visibility::Visible : Visibility::Collapsed);
        }

        navView().MenuItems().GetAt(4).as<Microsoft::UI::Xaml::Controls::NavigationViewItem>().MenuItems().GetAt(1)
            .as<Microsoft::UI::Xaml::Controls::NavigationViewItem>().Visibility(isLoggedIn() ? Visibility::Visible : Visibility::Collapsed);

        initDatabase();

    }



    void MainPage::navView_ItemInvoked(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args)
    {
        if (args.IsSettingsInvoked()) {
            navView_Navigate(L"settings", args.RecommendedNavigationTransitionInfo());
            
        }
        else if (args.InvokedItemContainer())
        { 


            navView_Navigate(unbox_value_or<hstring>( args.InvokedItemContainer().Tag(), L"").c_str(), args.RecommendedNavigationTransitionInfo());
        }
    }


    Windows::Foundation::IAsyncAction MainPage::navView_Navigate(std::wstring navItemTag, Windows::UI::Xaml::Media::Animation::NavigationTransitionInfo const& transitionInfo)
    {
        Windows::UI::Xaml::Interop::TypeName pageTypeName;


        if (navItemTag == L"accounts") {
            if (isLoggedIn())
                m_pages.at(m_pages.size() - 2) = std::make_pair<std::wstring, Interop::TypeName>(L"accounts", winrt::xaml_typename<Account>());
            else 
                m_pages.at(m_pages.size() - 2) = std::make_pair<std::wstring, Interop::TypeName>(L"accounts", winrt::xaml_typename<LoginScreen>());
     
        }
        else if (navItemTag == L"account_logout")
        {
            ContentFrame().Navigate(winrt::xaml_typename<Blank>(), nullptr, DrillInNavigationTransitionInfo());
            auto res = co_await Dialog(
                L"Logging out",
                L"You are about to log out your account.\nWould you like to continue logging out?",
                L"No",
                L"Yes");


            if (res == ContentDialogResult::Primary)
            {
                ContentFrame().Navigate(winrt::xaml_typename<Home>(), nullptr, DrillInNavigationTransitionInfo());
                database db(dbPath());
                db << "DELETE FROM loggedin;";
                ContentFrame().BackStack().Clear();

                navView().MenuItems().GetAt(3).as<Microsoft::UI::Xaml::Controls::NavigationViewItem>().Visibility(Visibility::Collapsed);

                navView().MenuItems().GetAt(4).as<Microsoft::UI::Xaml::Controls::NavigationViewItem>().MenuItems().GetAt(1)
                    .as<Microsoft::UI::Xaml::Controls::NavigationViewItem>().Visibility(Visibility::Collapsed);
            }
            else
                TryGoBack();
        }



        if (navItemTag == L"settings")
            pageTypeName = winrt::xaml_typename<Settings>();
        
        else
            for (auto&& eachPage : m_pages)
                if (eachPage.first == navItemTag)
                {
                    pageTypeName = eachPage.second;
                    break;
                }

        // Get the page type before navigation so you can prevent duplicate
        // entries in the backstack.
        Interop::TypeName preNavPageType = ContentFrame().CurrentSourcePageType();

        // Navigate only if the selected page isn't currently loaded.
        if (pageTypeName.Name != L"" && preNavPageType.Name != pageTypeName.Name)
            ContentFrame().Navigate(pageTypeName, nullptr, transitionInfo);



       

    }

  
    Windows::Foundation::IAsyncOperation<bool> MainPage::TryGoBack()
    {

         if (!ContentFrame().CanGoBack()) co_return false;


         // Leave confirmation while signing up
         if (ContentFrame().SourcePageType().Name != winrt::xaml_typename<Settings>().Name)
         {

             auto curTag = winrt::unbox_value_or<winrt::hstring>(navView().SelectedItem().as<muxc::NavigationViewItem>().Tag(), L"");

             if (curTag == L"account_createAcct") {
                 auto res = co_await Dialog(
                     L"Leaving Confirmation",
                     L"Do you want to cancel the registration?\nLeaving will be reset the changes.",
                     L"No",
                     L"Yes");

                 if (res == ContentDialogResult::Primary)
                     ContentFrame().GoBack();
             }
             else {

                ContentFrame().GoBack();
                co_return true;
             }
        
         }
         else {

             ContentFrame().GoBack();
             co_return true;
         }
         


        
    }


    Windows::Foundation::IAsyncAction MainPage::navView_BackRequested(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewBackRequestedEventArgs const& args)
    {
        co_await TryGoBack();

    }

    Windows::Foundation::IAsyncAction MainPage::System_BackRequested( Windows::Foundation::IInspectable const& /* sender */, Windows::UI::Core::BackRequestedEventArgs const& args)
	{
		if (!args.Handled())
		{
			args.Handled(co_await TryGoBack());
		}
	}


    void MainPage::On_Navigated(Windows::Foundation::IInspectable const& /* sender */, Windows::UI::Xaml::Navigation::NavigationEventArgs const& args)
    {
        navView().IsBackEnabled(ContentFrame().CanGoBack());

            // SettingsItem is not part of NavView.MenuItems, and doesn't have a Tag.
        if (ContentFrame().SourcePageType().Name == winrt::xaml_typename<Settings>().Name)
        {
            navView().SelectedItem(navView().SettingsItem().as<muxc::NavigationViewItem>());
            //navView().Header(winrt::box_value(L"Settings"));
        
        } 
        else if (ContentFrame().SourcePageType().Name != L"")
            for (auto&& eachPage : m_pages)
                if (eachPage.second.Name == args.SourcePageType().Name)
                {
                    for (auto&& eachMenuItem : navView().MenuItems())
                    {
                        auto navigationViewItem = eachMenuItem.try_as<muxc::NavigationViewItem>();
                        if (navigationViewItem)
                        {
                            for (auto&& eachMenuItemLow : navigationViewItem.MenuItems())
                            {
                                auto navigationViewItemLow = eachMenuItemLow.try_as<muxc::NavigationViewItem>();
                                if (navigationViewItemLow) {
                                    winrt::hstring hstringValueLow = winrt::unbox_value_or<winrt::hstring>(navigationViewItemLow.Tag(), L"");
                                    if (hstringValueLow == eachPage.first)
                                    {
                                        navView().SelectedItem(navigationViewItemLow);
                                        //navView().Header(navigationViewItem.Content());

                                    }
                                }

                            }

                            winrt::hstring hstringValue = winrt::unbox_value_or<winrt::hstring>(navigationViewItem.Tag(), L"");
                            if (hstringValue == eachPage.first)
                            {
                                navView().SelectedItem(navigationViewItem);
                                //navView().Header(navigationViewItem.Content());
                            }
                        }
                    }
                    break;
                }

    }




    void MainPage::navView_DisplayModeChanged(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewDisplayModeChangedEventArgs const& args)
    {
        Thickness* m = new Thickness();
        m->Right = 0;
        m->Bottom = 0;
        m->Top = 0;

        if (args.DisplayMode() == winrt::Microsoft::UI::Xaml::Controls::NavigationViewDisplayMode::Minimal)
            m->Left = 95;
        else
            m->Left = 42;

        AppTitle().Margin(*m);

    }

    void MainPage::navView_PaneOpening(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Windows::Foundation::IInspectable const& args)
    {
   
    }


    void MainPage::navView_PaneClosing(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Windows::Foundation::IInspectable const& args)
    {

    }
}










