#include "pch.h"
#include "Payment.h"
#if __has_include("Payment.g.cpp")
#include "Payment.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::HotelManagement::implementation
{
    int32_t Payment::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Payment::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

   
}
