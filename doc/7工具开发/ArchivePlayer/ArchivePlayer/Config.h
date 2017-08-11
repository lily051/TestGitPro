#pragma once

#include "GeneralDef.h"

class Config
{
public:
    Config(std::wstring &pathname);
    ~Config(void);
    int GetConfig(mysqlInfo& mysql);
    int GetConfig();
    int initItem();
private:
    std::wstring m_pathName;
    mysqlInfo m_sql;
};
