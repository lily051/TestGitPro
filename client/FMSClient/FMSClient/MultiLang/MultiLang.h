#pragma once

#include <string>
#include <map>
#include "fcTchar.h"
#include "tinyxml/tinyxml.h"

class CMultiLang
{
public:
    CMultiLang(void);
    ~CMultiLang(void);

private:
    std::map<std::string, std::map<std::string, std::_tstring>> m_mapString; // <_keyword, <type, string>>
    std::string m_strLangType;
    std::map<int, std::string> m_mapLangType;
    std::map<std::string, std::map<int, std::string>> m_mapLangFile;

    std::_tstring m_strDefault;
public:
    bool LoadConfig();

    bool LoadLangFile(const std::string& strModule, int nLangType);

    const TCHAR* GetString(const std::string& strKeyWord);

    std::string GetLangType(int nLangType);
};

extern CMultiLang theMultiLang;


// 调用方法：
/*
    接口说明：加载语言文件
    参数说明：strModule：模块名称
             nLangType  ：要使用的语言类型（0：简体中文，1：英文，其他再定义）
    返回说明：true成功， false失败
*/
bool LoadLangFile(const std::string& strModule, int nLangType);

/*
    接口说明：根据keyword返回对应的字符串
    参数说明：strKeyWord：关键字（全文件唯一）
    返回说明：字符串
*/
const TCHAR* GetStr(const std::string& strKeyWord);