#include "pch.h"
#include "Account.h"
#if __has_include("Account.g.cpp")
#include "Account.g.cpp"
#endif

#include "winrt/Windows.Storage.h"

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::Foundation;




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

    void Account::flipView_Main_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
    {
      

    }

    void Account::btnLogin_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {

    }


    void Account::btnSignUp_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        dataPath = Windows::Storage::ApplicationData::Current().LocalFolder().Path();
        // path for the temporary database file
        hstring path{ dataPath + L"\\data.db" };


        // store the file name for use later
        std::string fname = to_string(path.c_str());


        // open a connection to the database or create the file if it doesn't exist
        sqlite::database db(fname); // default is READWRITE | CREATE

        // create a new table, if needed
        db << "CREATE TABLE IF NOT EXISTS accounts ("
            "   id            INTEGER PRIMARY KEY AUTOINCREMENT,"
            "   username      TEXT     NOT NULL,"
            "   password      TEXT     NOT NULL,"
            "   icon          TEXT     NOT NULL,"
            "   civil         TEXT     NOT NULL,"
            "   nationality   TEXT     NOT NULL,"
            //name            
            "   firstname     TEXT     NOT NULL,"
            "   lastname      TEXT     NOT NULL,"
            "   middlename    TEXT     NOT NULL,"
            "   suffix        TEXT     NOT NULL,"
            //--              
            "   gender        INTEGER  NOT NULL,"
            "   birthday      TEXT     NOT NULL,"
            //home address    
            "   unit          TEXT     NOT NULL,"
            "   block_nm      TEXT     NOT NULL,"
            "   street        TEXT     NOT NULL,"
            "   subdivision   TEXT     NOT NULL,"
            "   barangas      TEXT     NOT NULL,"
            "   city          TEXT     NOT NULL,"
            "   province      TEXT     NOT NULL,"
            "   country       TEXT     NOT NULL,"
            //--              
            "   zip           TEXT     NOT NULL,"
            "   email         TEXT     NOT NULL,"
            "   tele_nm       INTEGER  NOT NULL"
            ");";

        
    }

}


