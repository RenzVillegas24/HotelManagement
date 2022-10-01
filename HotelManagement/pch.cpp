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

    bool initDatabase() {


        // sqlite
        dbPath(to_string(localPath().Path()) + "\\data.db");

        try {
            // open a connection to the database or create the file if it doesn't exist
            // default is READWRITE | CREATE
            sqlite::database db(dbPath());


            // create a new table, if needed
            db << "CREATE TABLE IF NOT EXISTS accounts ("
                "   id            INTEGER PRIMARY KEY AUTOINCREMENT,"
                "   username      TEXT     NOT NULL,"
                "   password      TEXT     NOT NULL,"
                "   icon          TEXT     NOT NULL,"
                "   civil         TEXT     NOT NULL,"
                "   nationality   TEXT     NOT NULL,"
                //name            
                "   firstName     TEXT     NOT NULL,"
                "   lastName      TEXT     NOT NULL,"
                "   middleName    TEXT     NOT NULL,"
                "   suffix        TEXT     NOT NULL,"
                //--              
                "   gender        INTEGER  NOT NULL,"
                "   birthday      TEXT     NOT NULL,"
                //home address    
                "   untBlck_etc   TEXT     NOT NULL,"
                "   barangay      TEXT     NOT NULL,"
                "   city          TEXT     NOT NULL,"
                "   province      TEXT     NOT NULL,"
                "   region        TEXT     NOT NULL,"
                "   country       TEXT     NOT NULL,"
                //code
                "   barangayCode  TEXT     NOT NULL,"
                "   cityCode      TEXT     NOT NULL,"
                "   provinceCode  TEXT     NOT NULL,"
                "   regionCode    TEXT     NOT NULL,"
                //--              
                "   zip           TEXT     NOT NULL,"
                "   email         TEXT     NOT NULL,"
                "   teleNum       TEXT     NOT NULL,"
                "   isAdmin       INTEGER  NOT NULL"
                ");";

            db << "CREATE TABLE IF NOT EXISTS loggedin ("
                "   username      TEXT     NOT NULL,"
                "   password      TEXT     NOT NULL,"
                "   isRemembered  INTEGER  NOT NULL"
                ");";


        } catch(std::exception& e) {
            return false;
        }
        return true;
    }

}