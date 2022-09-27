#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"
#include "winrt/Windows.ApplicationModel.Core.h"
#include "winrt/Windows.UI.ViewManagement.h"
#include "../../../../../../Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/winrt/wrl/internal.h"
#include "winrt/Windows.UI.Xaml.Input.h"
#include <iostream>

#include "winrt/Windows.Storage.h"

#include "GlobalVars.h"

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


    // List of ValueTuple holding the Navigation Tag and the relative Navigation Page
    vector<pair<wstring, Windows::UI::Xaml::Interop::TypeName>> m_pages;


    void MainPage::MainPivot_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
    {

        m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName>
            (L"home", winrt::xaml_typename<Home>()));
        m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName>
            (L"bookings", winrt::xaml_typename<Booking>()));
        m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName>
            (L"payment", winrt::xaml_typename<Payment>()));
        m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName>
            (L"support", winrt::xaml_typename<Support>()));
        m_pages.push_back(std::make_pair<std::wstring, Interop::TypeName>
            (L"accounts", winrt::xaml_typename<Account>()));

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



        // sqlite
        
        GlobalVars().dbPath(to_string(Windows::Storage::ApplicationData::Current().LocalFolder().Path()) + "\\data.db");

        

        // open a connection to the database or create the file if it doesn't exist
        // default is READWRITE | CREATE
        database db(GlobalVars().dbPath());


        // create a new table, if needed
        db << "CREATE TABLE IF NOT EXISTS accounts ("
            "   id            INTEGER PRIMARY KEY AUTOINCREMENT,"
            "   username      TEXT     NOT NULL,"
            "   password      TEXT     NOT NULL,"
            "   icon          TEXT     NOT NULL,"
            "   civil         TEXT     NOT NULL,"
            "   nationality   TEXT     NOT NULL,"
            //name            
            "   firstname     TEXT     NOT NULL,"
            "   lastname      TEXT     NOT NULL,"
            "   middlename    TEXT     NOT NULL,"
            "   suffix        TEXT     NOT NULL,"
            //--              
            "   gender        INTEGER  NOT NULL,"
            "   birthday      TEXT     NOT NULL,"
            //home address    
            "   unit          TEXT     NOT NULL,"
            "   block_nm      TEXT     NOT NULL,"
            "   street        TEXT     NOT NULL,"
            "   subdivision   TEXT     NOT NULL,"
            "   barangay      TEXT     NOT NULL,"
            "   city          TEXT     NOT NULL,"
            "   province      TEXT     NOT NULL,"
            "   region        TEXT     NOT NULL,"
            "   country       TEXT     NOT NULL,"
            //--              
            "   zip           TEXT     NOT NULL,"
            "   email         TEXT     NOT NULL,"
            "   tele_nm       INTEGER  NOT NULL"
            ");";

        db << "CREATE TABLE IF NOT EXISTS loggedin ("
            "   username      TEXT     NOT NULL,"
            "   password      TEXT     NOT NULL"
            ");";

            

    }



    void MainPage::navView_ItemInvoked(NavigationView const& sender, NavigationViewItemInvokedEventArgs const& args)
    {
        if (args.IsSettingsInvoked()) {
            navView_Navigate(L"settings", args.RecommendedNavigationTransitionInfo());
            
        }
        else if (args.InvokedItemContainer())
        {
            navView_Navigate(unbox_value_or<hstring>( args.InvokedItemContainer().Tag(), L"").c_str(), args.RecommendedNavigationTransitionInfo());
        }
    }


    void MainPage::navView_Navigate(std::wstring navItemTag, Windows::UI::Xaml::Media::Animation::NavigationTransitionInfo const& transitionInfo)
    {
        Windows::UI::Xaml::Interop::TypeName pageTypeName;



        if (navItemTag == L"accounts") {
            if (GlobalVars().isLoggedIn())
                m_pages.at(4) = std::make_pair<std::wstring, Interop::TypeName>(L"accounts", winrt::xaml_typename<Account>());
            else 
                m_pages.at(4) = std::make_pair<std::wstring, Interop::TypeName>(L"accounts", winrt::xaml_typename<LoginScreen>());
     
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

  
    bool MainPage::TryGoBack()
    {
        if (!ContentFrame().CanGoBack()) return false;
        
        ContentFrame().GoBack();
        return true;
        
    }


    void MainPage::navView_BackRequested(NavigationView const& sender, NavigationViewBackRequestedEventArgs const& args)
    {
        TryGoBack();

    }

	void MainPage::System_BackRequested( Windows::Foundation::IInspectable const& /* sender */, Windows::UI::Core::BackRequestedEventArgs const& args)
	{
		if (!args.Handled())
		{
			args.Handled(TryGoBack());
		}
	}


    void MainPage::On_Navigated(Windows::Foundation::IInspectable const& /* sender */, Windows::UI::Xaml::Navigation::NavigationEventArgs const& args)
    {
        navView().IsBackEnabled(ContentFrame().CanGoBack());

            // SettingsItem is not part of NavView.MenuItems, and doesn't have a Tag.
        if (ContentFrame().SourcePageType().Name == winrt::xaml_typename<Settings>().Name)
        {
            navView().SelectedItem(navView().SettingsItem().as<NavigationViewItem>());
            //navView().Header(winrt::box_value(L"Settings"));
        }

        else if (ContentFrame().SourcePageType().Name != L"")
            for (auto&& eachPage : m_pages)
                if (eachPage.second.Name == args.SourcePageType().Name)
                {
                    for (auto&& eachMenuItem : navView().MenuItems())
                    {
                        auto navigationViewItem = eachMenuItem.try_as<NavigationViewItem>();
                        if (navigationViewItem)
                        {
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




    void MainPage::navView_DisplayModeChanged(NavigationView const& sender, NavigationViewDisplayModeChangedEventArgs const& args)
    {
        Thickness* m = new Thickness();
        m->Right = 0;
        m->Bottom = 0;
        m->Top = 0;

        if (args.DisplayMode() == NavigationViewDisplayMode::Minimal)
            m->Left = 95;
        else
            m->Left = 42;

        AppTitle().Margin(*m);

    }

    void MainPage::navView_PaneOpening(NavigationView const& sender, winrt::Windows::Foundation::IInspectable const& args)
    {
        Thickness* m = new Thickness();
        m->Right = 0;
        m->Bottom = 0;
        m->Top = 0;
        m->Left = 42;

        AppTitle().Margin(*m);
    }


    void MainPage::navView_PaneClosing(NavigationView const& sender, winrt::Windows::Foundation::IInspectable const& args)
    {
        Thickness* m = new Thickness();
        m->Right = 0;
        m->Bottom = 0;
        m->Top = 0;


        if (navView().DisplayMode() == NavigationViewDisplayMode::Minimal)
            m->Left = 95;
        else
            m->Left = 42;

        AppTitle().Margin(*m);

    }
}










