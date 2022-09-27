#pragma once
#include "pch.h"
#include "String.h"
#include "sqlite_modern_cpp.h"


class GlobalVars {
public:
	inline std::string dbPath(std::string);
	inline std::string dbPath();
	inline bool isLoggedIn(void);
private:
	std::string dataPath;
};

inline std::string GlobalVars::dbPath(std::string newPath) {
    dataPath = newPath;
    return dataPath;
}
inline std::string GlobalVars::dbPath() {
    return dataPath;
}

inline bool GlobalVars::isLoggedIn(void)
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

