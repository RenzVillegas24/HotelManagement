#include "pch.h"
#include "Booking.h"
#if __has_include("Booking.g.cpp")
#include "Booking.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::HotelManagement::implementation
{
    int32_t Booking::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Booking::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

 
}
