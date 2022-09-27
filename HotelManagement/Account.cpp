#include "pch.h"
#include "Account.h"
#if __has_include("Account.g.cpp")
#include "Account.g.cpp"
#endif

#include "GlobalVars.h"
#include "winrt/Windows.UI.Popups.h"

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::Foundation;
using namespace winrt::Windows::UI::Popups;

using namespace sqlite;
using namespace std;



namespace winrt::HotelManagement::implementation
{
    int32_t Account::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Account::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
        
     
    }

    void Account::Page_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
    {
       
    }

}




