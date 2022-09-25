#include "pch.h"
#include "Units.h"
#if __has_include("Units.g.cpp")
#include "Units.g.cpp"
#endif

#include <winrt/Windows.UI.Xaml.Media.Animation.h>

using namespace winrt;
using namespace Windows::UI::Xaml;
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


    void Units::HomeTitle_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        auto t = SlideNavigationTransitionInfo();
        t.Effect(SlideNavigationTransitionEffect::FromLeft);

        Frame().Navigate(winrt::xaml_typename<Home>(), nullptr, t);
    }


}
