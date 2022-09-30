#include "pch.h"


namespace winrt::HotelManagement::implementation
{
    std::string dbPath(std::string newPath) {
        dataPath = newPath;
        return dataPath;
    }
    std::string dbPath() {
        return dataPath;
    }

    winrt::Windows::Storage::IStorageFolder localPath() {
        return winrt::Windows::Storage::ApplicationData::Current().LocalFolder();
    }

   
    winrt::hstring dbPathH() {
        return winrt::to_hstring(dataPath);
        

    }

    bool isLoggedIn(void)
    {
        sqlite::database db(dbPath());

        try
        {
            bool data_cnt = false;
            db << "SELECT username FROM loggedin;"
                >> [&](std::string usr_name, std::string usr_pass) {
                data_cnt = true;
            };

            if (!data_cnt)
                return false;
            else
                return true;
        }
        catch (const std::exception&)
        {
            return false;
        }
    }

}