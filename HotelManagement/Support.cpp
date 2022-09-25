#include "pch.h"
#include "Support.h"
#if __has_include("Support.g.cpp")
#include "Support.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::HotelManagement::implementation
{
    int32_t Support::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Support::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }


}
