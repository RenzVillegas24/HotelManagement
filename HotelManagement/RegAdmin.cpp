#include "pch.h"
#include "RegAdmin.h"
#if __has_include("RegAdmin.g.cpp")
#include "RegAdmin.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::HotelManagement::implementation
{
    int32_t RegAdmin::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void RegAdmin::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

   
}
