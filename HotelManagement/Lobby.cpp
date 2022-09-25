#include "pch.h"
#include "Lobby.h"
#if __has_include("Lobby.g.cpp")
#include "Lobby.g.cpp"
#endif

#include <winrt/Windows.UI.Xaml.Media.Animation.h>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media::Animation;

namespace winrt::HotelManagement::implementation
{
    int32_t Lobby::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Lobby::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void Lobby::HomeTitle_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        auto t = SlideNavigationTransitionInfo();
        t.Effect(SlideNavigationTransitionEffect::FromLeft);

        Frame().Navigate(winrt::xaml_typename<Home>(), nullptr, t);
    }
}
