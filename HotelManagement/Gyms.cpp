#include "pch.h"
#include "Gyms.h"
#if __has_include("Gyms.g.cpp")
#include "Gyms.g.cpp"
#endif

#include <winrt/Windows.UI.Xaml.Media.Animation.h>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media::Animation;

namespace winrt::HotelManagement::implementation
{
    int32_t Gyms::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Gyms::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void Gyms::HomeTitle_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        auto t = SlideNavigationTransitionInfo();
        t.Effect(SlideNavigationTransitionEffect::FromLeft);

        Frame().Navigate(winrt::xaml_typename<Home>(), nullptr, t);
    }
}
