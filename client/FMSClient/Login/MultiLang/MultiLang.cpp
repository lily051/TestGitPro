#include "StdAfx.h"
#include "MultiLang.h"

#define LANG_TYPE_DEFAUTL 999


CMultiLang theMultiLang;

bool LoadLangFile(const std::string& strModule, int nLangType)
{
    return theMultiLang.LoadLangFile(strModule, nLangType);
}

const TCHAR* GetStr(const std::string& strKeyWord)
{
    return theMultiLang.GetString(strKeyWord);
}

CMultiLang::CMultiLang(void)
: m_strLangType("chs")
, m_strDefault(_T("Undefined"))
{
    //m_mapLangType[LANG_TYPE_DEFAUTL] = "chs";
    LoadConfig();
}

CMultiLang::~CMultiLang(void)
{
}

bool CMultiLang::LoadLangFile(const std::string& strModule, int nLangType)
{
    m_mapString.clear();
    if (m_mapLangFile.find(strModule) == m_mapLangFile.end())
    {
        return false;
    }
    if (m_mapLangFile[strModule].find(nLangType) == m_mapLangFile[strModule].end())
    {
        return false;
    }

    // 多语言文件绝对路径
    TCHAR szPath[MAX_PATH] = {0};
    GetModuleFileName(NULL, szPath, MAX_PATH);
    (_tcsrchr(szPath, _T('\\')))[1] = 0;
    std::string strLangPath = fcT2A(szPath);
    std::string strLangFile = m_mapLangFile[strModule][nLangType];
    strLangPath.append("\\");
    strLangPath.append(strLangFile);

    TiXmlDocument doc;
    if (!doc.LoadFile(strLangPath))
    {
        return false;
    }
    TiXmlElement* pLangNode = doc.FirstChildElement();
    if (!pLangNode)
    {
        return false;
    }

    TiXmlElement* pTypeNode = pLangNode->FirstChildElement("type");
    while (pTypeNode)
    {
        int nType = atoi(pTypeNode->Attribute("key"));
        m_mapLangType[nType] = pTypeNode->Attribute("value");

        pTypeNode = pTypeNode->NextSiblingElement("type");
    }

    TiXmlElement* pResNode = pLangNode->FirstChildElement("resource");
    std::map<std::string, std::_tstring> mapElement;
    TiXmlElement* pStringNode = pResNode->FirstChildElement();
    while (pStringNode)
    {
        std::string strKeyword = pStringNode->Attribute("keyword");

        // every attribute
        for (std::map<int, std::string>::iterator itr = m_mapLangType.begin();
            itr != m_mapLangType.end(); ++itr)
        {
            const char* pStr = pStringNode->Attribute(itr->second.c_str());
            if (pStr)
            {
                mapElement[itr->second] = fcU2T(pStr);
            }
            else
            {
                mapElement[itr->second] = m_strDefault;
            }
        }
        m_mapString[strKeyword] = mapElement;

        pStringNode = pStringNode->NextSiblingElement();
    }

    m_strLangType = GetLangType(nLangType);

    return true;
}

std::string CMultiLang::GetLangType(int nLangType)
{
    if (m_mapLangType.find(nLangType) != m_mapLangType.end())
    {
        return m_mapLangType[nLangType];
    }
    return "chs";
}

const TCHAR* CMultiLang::GetString(const std::string& strKeyWord)
{
    std::string strTemp;
    if (m_mapString.find(strKeyWord) != m_mapString.end())
    {
        return m_mapString[strKeyWord][m_strLangType].c_str();
    }

    return m_strDefault.c_str();
}

bool CMultiLang::LoadConfig()
{
    m_mapLangFile.clear();

    // 多语言配置绝对路径
    TCHAR szPath[MAX_PATH] = {0};
    GetModuleFileName(NULL, szPath, MAX_PATH);
    (_tcsrchr(szPath, _T('\\')))[1] = 0;
    CString strLangPath(szPath);
    strLangPath.Format(_T("%s\\LangConfig.xml"), szPath);

    TiXmlDocument doc;
    if (!doc.LoadFile(fcT2A((LPCTSTR)strLangPath)))
    {
        return false;
    }
    TiXmlElement* pLangRoot = doc.FirstChildElement();
    if (!pLangRoot)
    {
        return false;
    }
    TiXmlElement* pLangNode = pLangRoot->FirstChildElement();
    while (pLangNode)
    {
        std::string strModule = pLangNode->Value();
        TiXmlElement* pModuleNode = pLangNode->FirstChildElement();
        while(pModuleNode)
        {
            int nType = atoi(pModuleNode->Attribute("key"));
            m_mapLangFile[strModule][nType] = pModuleNode->Attribute("value");

            pModuleNode = pModuleNode->NextSiblingElement();
        }

        pLangNode = pLangNode->NextSiblingElement();
    }

    return true;
}