#include "pch.h"
#include "Blank.h"
#if __has_include("Blank.g.cpp")
#include "Blank.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::HotelManagement::implementation
{
    int32_t Blank::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Blank::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

}
