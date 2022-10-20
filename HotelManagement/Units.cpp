#include "pch.h"
#include "Units.h"
#if __has_include("Units.g.cpp")
#include "Units.g.cpp"
#endif

#include <winrt/Windows.UI.Xaml.Media.Animation.h>
#include <Globals.h>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Media::Animation;

namespace winrt::HotelManagement::implementation
{
    int32_t Units::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Units::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }


    void winrt::HotelManagement::implementation::Units::HomeTitle_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        auto t = SlideNavigationTransitionInfo();
        t.Effect(SlideNavigationTransitionEffect::FromLeft);

        Frame().Navigate(winrt::xaml_typename<Home>(), nullptr, t);
    }

 

    void Units::mainFlipView_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        parallax1().Source(mainFlipView());
        parallax2().Source(mainFlipView());
        parallax3().Source(mainFlipView());
        parallax4().Source(mainFlipView());
        parallax5().Source(mainFlipView());
        parallax6().Source(mainFlipView());
        parallax7().Source(mainFlipView());
        parallax9().Source(mainFlipView());

        Focus(FocusState::Programmatic);

    }

    winrt::Windows::Foundation::IAsyncAction Units::btnReserve_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
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


    void Units::fade_anim(winrt::Windows::UI::Xaml::DependencyObject objIn, winrt::Windows::UI::Xaml::DependencyObject objOut, int millis) {
        auto timeSpn = Windows::Foundation::TimeSpan(std::chrono::milliseconds(millis));
        auto dur = DurationHelper::FromTimeSpan(timeSpn);
        auto ky = KeyTimeHelper::FromTimeSpan(timeSpn);

        auto new_storyboard = Storyboard();

        auto in_anim = DoubleAnimation();
        in_anim.To(0.0);
        in_anim.Duration(dur);
        Storyboard::SetTarget(in_anim, objIn);
        Storyboard::SetTargetProperty(in_anim, L"Opacity");

        auto in_animVis = ObjectAnimationUsingKeyFrames();
        auto in_animVis_key = DiscreteObjectKeyFrame();
        in_animVis_key.KeyTime(ky);
        in_animVis_key.Value(box_value(L"Collapsed"));
        in_animVis.KeyFrames().Append(in_animVis_key);

        Storyboard::SetTarget(in_animVis, objIn);
        Storyboard::SetTargetProperty(in_animVis, L"(Panel.Visibility)");


        auto out_anim = DoubleAnimation();
        out_anim.To(1.0);
        out_anim.Duration(dur);
        Storyboard::SetTarget(out_anim, objOut);
        Storyboard::SetTargetProperty(out_anim, L"Opacity");

        auto out_animVis = ObjectAnimationUsingKeyFrames();
        auto out_animVis_key = DiscreteObjectKeyFrame();
        out_animVis_key.Value(box_value(L"Visible"));
        out_animVis.KeyFrames().Append(out_animVis_key);
         
        Storyboard::SetTarget(out_animVis, objOut);
        Storyboard::SetTargetProperty(out_animVis, L"(Panel.Visibility)");


        new_storyboard.Children().Append(in_anim);
        new_storyboard.Children().Append(in_animVis);
        new_storyboard.Children().Append(out_anim);
        new_storyboard.Children().Append(out_animVis);

        new_storyboard.Begin();
    }


    void Units::Page_KeyDown(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::KeyRoutedEventArgs const& e)
    {
        if (e.Key() == winrt::Windows::System::VirtualKey::Escape) {

            if (P1_content().Opacity() == 1)
                fade_anim(P1_content(), P1_contentShort(), 400);
            else if (P2_content().Opacity() == 1)
                fade_anim(P2_content(), P2_contentShort(), 400);
            else if (P3_content().Opacity() == 1)
                fade_anim(P3_content(), P3_contentShort(), 400);
            else if (P4_content().Opacity() == 1)
                fade_anim(P4_content(), P4_contentShort(), 400);
            else if (P5_content().Opacity() == 1)
                fade_anim(P5_content(), P5_contentShort(), 400);
            else if (P6_content().Opacity() == 1)
                fade_anim(P6_content(), P6_contentShort(), 400);
            else if (P7_content().Opacity() == 1)
                fade_anim(P7_content(), P7_contentShort(), 400); 
            
            mainFlipView().IsEnabled(true); 
        }
    }

    void Units::btnShowP1_details_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        fade_anim(P1_contentShort(), P1_content(), 150);
        mainFlipView().IsEnabled(false);
    }

    void Units::btnHideP1_details_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        fade_anim(P1_content(), P1_contentShort(), 400);
        mainFlipView().IsEnabled(true);
    }

    void Units::btnShowP2_details_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        fade_anim(P2_contentShort(), P2_content(), 150);
        mainFlipView().IsEnabled(false);
    }

    void Units::btnHideP2_details_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        fade_anim(P2_content(), P2_contentShort(), 400);
        mainFlipView().IsEnabled(true);
    }



    void Units::btnShowP3_details_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        fade_anim(P3_contentShort(), P3_content(), 150);
        mainFlipView().IsEnabled(false);
    }


    void Units::btnHideP3_details_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        fade_anim(P3_content(), P3_contentShort(), 400);
        mainFlipView().IsEnabled(true);
    }

    void Units::btnShowP4_details_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        fade_anim(P4_contentShort(), P4_content(), 150);
        mainFlipView().IsEnabled(false);
    }

    void Units::btnHideP4_details_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        fade_anim(P4_content(), P4_contentShort(), 400);
        mainFlipView().IsEnabled(true);
    }

    void Units::btnShowP5_details_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        fade_anim(P5_contentShort(), P5_content(), 150);
        mainFlipView().IsEnabled(false);
    }


    void Units::btnHideP5_details_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        fade_anim(P5_content(), P5_contentShort(), 400);
        mainFlipView().IsEnabled(true);
    }

    void Units::btnShowP6_details_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        fade_anim(P6_contentShort(), P6_content(), 150);
        mainFlipView().IsEnabled(false);
    }

    void Units::btnHideP6_details_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        fade_anim(P6_content(), P6_contentShort(), 400);
        mainFlipView().IsEnabled(true);
    }

    void Units::btnShowP7_details_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        fade_anim(P7_contentShort(), P7_content(), 150);
        mainFlipView().IsEnabled(false);
    }

    void Units::btnHideP7_details_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        fade_anim(P7_content(), P7_contentShort(), 400);
        mainFlipView().IsEnabled(true);
    }

}





