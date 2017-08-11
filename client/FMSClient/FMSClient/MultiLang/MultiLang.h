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


// ���÷�����
/*
    �ӿ�˵�������������ļ�
    ����˵����strModule��ģ������
             nLangType  ��Ҫʹ�õ��������ͣ�0���������ģ�1��Ӣ�ģ������ٶ��壩
    ����˵����true�ɹ��� falseʧ��
*/
bool LoadLangFile(const std::string& strModule, int nLangType);

/*
    �ӿ�˵��������keyword���ض�Ӧ���ַ���
    ����˵����strKeyWord���ؼ��֣�ȫ�ļ�Ψһ��
    ����˵�����ַ���
*/
const TCHAR* GetStr(const std::string& strKeyWord);