﻿#include "pch.h"
#include "Pools.h"
#if __has_include("Pools.g.cpp")
#include "Pools.g.cpp"
#endif

#include "Globals.h"

#include <winrt/Windows.UI.Xaml.Media.Animation.h>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media::Animation;
using namespace winrt::Windows::UI::Xaml::Controls;

namespace winrt::HotelManagement::implementation
{
    int32_t Pools::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Pools::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }


    void Pools::HomeTitle_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        auto t = SlideNavigationTransitionInfo();
        t.Effect(SlideNavigationTransitionEffect::FromLeft);

        Frame().Navigate(winrt::xaml_typename<Home>(), nullptr, t);
    }

    void Pools::Page_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        parallax1().Source(mainFlipView());
        parallax2().Source(mainFlipView());
        parallax3().Source(mainFlipView());
        parallax4().Source(mainFlipView());
    }

    winrt::Windows::Foundation::IAsyncAction Pools::btnReserve_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        if (isLoggedIn())
            Frame().Navigate(winrt::xaml_typename<Booking>(), nullptr, DrillInNavigationTransitionInfo());
        else {

            auto res = co_await Dialog(
                L"Not logged in",
                L"No account has logged in this computer.\nWould you like to login or create an account instead?",
                L"No",
                L"Log In",
                L"Sign Up");


            if (res == ContentDialogResult::Primary)
                Frame().Navigate(winrt::xaml_typename<LoginScreen>(), nullptr, DrillInNavigationTransitionInfo());
            else if (res == ContentDialogResult::Secondary)
                Frame().Navigate(winrt::xaml_typename<SignUpForm>(), nullptr, DrillInNavigationTransitionInfo());


        }
    }


}


