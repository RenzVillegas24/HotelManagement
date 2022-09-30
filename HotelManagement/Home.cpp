#include "pch.h"
#include "Home.h"


#include "winrt/Windows.UI.Xaml.h"
#include "winrt/Windows.UI.Xaml.Media.h"
#include "winrt/Windows.UI.Xaml.Input.h"

#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.ApplicationModel.Core.h>


#if __has_include("Home.g.cpp")
#include "Home.g.cpp"
#endif
#include "winrt/Windows.UI.Popups.h"
#include <winrt/Windows.UI.Xaml.Media.Animation.h>

using namespace winrt;
using namespace winrt::Windows::UI::Xaml::Controls;
using namespace winrt::Windows::UI::Xaml::Media;

using namespace Windows::Foundation;
using namespace Windows::Devices::Enumeration;

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media::Animation;
using namespace Windows::UI::Popups;

namespace winrt::HotelManagement::implementation
{
    int32_t Home::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Home::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }


    void Home::OnTick(IInspectable const& sender, IInspectable const& event)
    {
        OnTickDiscover();
        OnTickUnits();
        OnTickGym();
        OnTickRestaurant();
        OnTickPool();
        OnTickLobby();
        OnTickEvents();
        OnTickAll();

        tickTime++;
    }


    void Home::OnTickDiscover()
    {
        int tick = tickTime;


        if (tick % 8 == 0) {
            if (flipView_Discover().SelectedIndex() == flipView_Discover().Items().Size() - 1)
                directionD = -1;
            else if (flipView_Discover().SelectedIndex() == 0)
                directionD = 1;


            flipView_Discover().SelectedIndex(flipView_Discover().SelectedIndex() + directionD);
        }
    }

    void Home::OnTickUnits()
    {
        int tick = tickTime - 1;


        if (tick % 10 == 0) {
            if (flipView_Units().SelectedIndex() == flipView_Units().Items().Size() - 1)
                directionU = -1;
            else if (flipView_Units().SelectedIndex() == 0)
                directionU = 1;


            flipView_Units().SelectedIndex(flipView_Units().SelectedIndex() + directionU);

            for (int i = 0; i < 5; i++)
            {
                double sz = flipView_Units().SelectedIndex() == i ? 10 : 5;

                auto ellp = indiUnits().Children().GetAt(i).as<Shapes::Ellipse>();
                ellp.Width(sz);
                ellp.Height(sz);
            }
        }
    }

    void Home::OnTickGym()
    {

        int tick = tickTime - 2;

        if (tick % 10 == 0) {
            if (flipView_Gym().SelectedIndex() == flipView_Gym().Items().Size() - 1)
                directionG = -1;
            else if (flipView_Gym().SelectedIndex() == 0)
                directionG = 1;


            flipView_Gym().SelectedIndex(flipView_Gym().SelectedIndex() + directionG);

            for (int i = 0; i < 5; i++)
            {
                double sz = flipView_Gym().SelectedIndex() == i ? 10 : 5;

                auto ellp = indiGym().Children().GetAt(i).as<Shapes::Ellipse>();
                ellp.Width(sz);
                ellp.Height(sz);
            }

        }
        
    }

    void Home::OnTickRestaurant()
    {
        int tick = tickTime - 3;

        if (tick % 10 == 0) {
            if (flipView_Restaurant().SelectedIndex() == flipView_Restaurant().Items().Size() - 1)
                directionR = -1;
            else if (flipView_Restaurant().SelectedIndex() == 0)
                directionR = 1;


            flipView_Restaurant().SelectedIndex(flipView_Restaurant().SelectedIndex() + directionR);

            for (int i = 0; i < 5; i++)
            {
                double sz = flipView_Restaurant().SelectedIndex() == i ? 10 : 5;

                auto ellp = indiRestaurant().Children().GetAt(i).as<Shapes::Ellipse>();
                ellp.Width(sz);
                ellp.Height(sz);
            }
        }
    }

    void Home::OnTickPool()
    {
        int tick = tickTime - 4;

        if (tick % 10 == 0) {
            if (flipView_Pool().SelectedIndex() == flipView_Pool().Items().Size() - 1)
                directionP = -1;
            else if (flipView_Pool().SelectedIndex() == 0)
                directionP = 1;


            flipView_Pool().SelectedIndex(flipView_Pool().SelectedIndex() + directionP);

            for (int i = 0; i < 5; i++)
            {
                double sz = flipView_Pool().SelectedIndex() == i ? 10 : 5;

                auto ellp = indiPool().Children().GetAt(i).as<Shapes::Ellipse>();
                ellp.Width(sz);
                ellp.Height(sz);
            }
        }
    }

    void Home::OnTickLobby()
    {
        int tick = tickTime - 5;

        if (tick % 10 == 0) {
            if (flipView_Lobby().SelectedIndex() == flipView_Lobby().Items().Size() - 1)
                directionL = -1;
            else if (flipView_Lobby().SelectedIndex() == 0)
                directionL = 1;


            flipView_Lobby().SelectedIndex(flipView_Lobby().SelectedIndex() + directionL);

            for (int i = 0; i < 5; i++)
            {
                double sz = flipView_Lobby().SelectedIndex() == i ? 10 : 5;

                auto ellp = indiLobby().Children().GetAt(i).as<Shapes::Ellipse>();
                ellp.Width(sz);
                ellp.Height(sz);
            }
        }
    }

    void Home::OnTickEvents()
    {
        int tick = tickTime - 6;

        if (tick % 10 == 0) {
            if (flipView_Events().SelectedIndex() == flipView_Events().Items().Size() - 1)
                directionE = -1;
            else if (flipView_Events().SelectedIndex() == 0)
                directionE = 1;


            flipView_Events().SelectedIndex(flipView_Events().SelectedIndex() + directionE);

            for (int i = 0; i < 5; i++)
            {
                double sz = flipView_Events().SelectedIndex() == i ? 10 : 5;

                auto ellp = indiEvents().Children().GetAt(i).as<Shapes::Ellipse>();
                ellp.Width(sz);
                ellp.Height(sz);
            }
        }
    }

    void Home::OnTickAll()
    {
        int tick = tickTime - 7;

        if (tick % 6 == 0) {
            if (flipView_All().SelectedIndex() == flipView_All().Items().Size() - 1)
                directionA = -1;
            else if (flipView_All().SelectedIndex() == 0)
                directionA = 1;


            flipView_All().SelectedIndex(flipView_All().SelectedIndex() + directionA);
        }
    }

    void Home::Page_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
    {
        flipTimer.Interval(std::chrono::milliseconds{500});
        flipTimer.Start();

        auto registrationtoken = flipTimer.Tick({ this, &Home::OnTick });
    }

    void Home::Page_Unloaded(IInspectable const& sender, RoutedEventArgs const& e)
    {
        flipTimer.Stop();
    }

    void Home::btnScrollDown_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        flipView_Main().SelectedIndex(flipView_Main().SelectedIndex() + 1);
    }

    void Home::btnRooms_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        auto t = SlideNavigationTransitionInfo();
        t.Effect(SlideNavigationTransitionEffect::FromRight);

        Frame().Navigate(winrt::xaml_typename<Units>(), nullptr, t);
    }

    void Home::btnGyms_Click(IInspectable const& sender,RoutedEventArgs const& e)
    {
        auto t = SlideNavigationTransitionInfo();
        t.Effect(SlideNavigationTransitionEffect::FromRight);

        Frame().Navigate(winrt::xaml_typename<Gyms>(), nullptr, t);
    }

    void Home::btnRestaurant_Click(IInspectable const& sender,RoutedEventArgs const& e)
    {
        auto t = SlideNavigationTransitionInfo();
        t.Effect(SlideNavigationTransitionEffect::FromRight);

        Frame().Navigate(winrt::xaml_typename<Restaurant>(), nullptr, t);
    }


    void Home::btnPools_Click(IInspectable const& sender,RoutedEventArgs const& e)
    {
        auto t = SlideNavigationTransitionInfo();
        t.Effect(SlideNavigationTransitionEffect::FromRight);

        Frame().Navigate(winrt::xaml_typename<Pools>(), nullptr, t);
    }


    void Home::btnLobby_Click(IInspectable const& sender,RoutedEventArgs const& e)
    {
        auto t = SlideNavigationTransitionInfo();
        t.Effect(SlideNavigationTransitionEffect::FromRight);

        Frame().Navigate(winrt::xaml_typename<Lobby>(), nullptr, t);
    }


    void Home::btnEvents_Click(IInspectable const& sender,RoutedEventArgs const& e)
    {
        auto t = SlideNavigationTransitionInfo();
        t.Effect(SlideNavigationTransitionEffect::FromRight);

        Frame().Navigate(winrt::xaml_typename<Events>(), nullptr, t);
    }

    void Home::btnHelp_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        Frame().Navigate(winrt::xaml_typename<Support>(), nullptr, DrillInNavigationTransitionInfo());

    }


    IAsyncAction Home::btnReserve_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        if(isLoggedIn())
            Frame().Navigate(winrt::xaml_typename<Account>(), nullptr, DrillInNavigationTransitionInfo());
        else {
            MessageDialog dialog(L"No account has logged in this computer, would you like to login or create an account?", L"Not logged in");
            dialog.Commands().Append(UICommand(L"Login", nullptr));
            dialog.Commands().Append(UICommand(L"Create an account", nullptr));
            dialog.Commands().Append(UICommand(L"Cancel", nullptr));
            auto& result = co_await dialog.ShowAsync();


            auto t = SlideNavigationTransitionInfo();
            t.Effect(SlideNavigationTransitionEffect::FromRight);

            if (result.Label() == L"Login")
                Frame().Navigate(winrt::xaml_typename<LoginScreen>(), nullptr, t);
            else if (result.Label() == L"Create an account")
                Frame().Navigate(winrt::xaml_typename<SignUpForm>(), nullptr, t);
            

        }
        

    }


}





