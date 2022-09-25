#include "pch.h"
#include "Pools.h"
#if __has_include("Pools.g.cpp")
#include "Pools.g.cpp"
#endif


#include <winrt/Windows.UI.Xaml.Media.Animation.h>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media::Animation;

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
}
