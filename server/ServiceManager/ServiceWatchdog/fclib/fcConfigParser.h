#ifndef GUARD_FCCONFIGPARSER_H
#define GUARD_FCCONFIGPARSER_H
/**	@file    fcConfigParser.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   ini �ļ���������,�����̰߳�ȫ��
 *
 *	@author	 xuezhihu
 *	@date	 2013/3/23
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V0.0.1  ����
 *
 *	@warning ������д���ļ���صľ�����Ϣ
 */
#include <cassert>
#include "fcTchar.h"
class CConfigParser
{
public:
    CConfigParser(LPCTSTR lpCfgFileName)
        : m_cfgFileName(lpCfgFileName)
    {
        ::memset(m_szTempValue, 0, sizeof(m_szTempValue));
        if (std::_tstring::npos == m_cfgFileName.find(_T(":"))) // �Ǹ����·��
        {
            TCHAR szPath[MAX_PATH + 1] = {0};
            _tfullpath(szPath, m_cfgFileName.c_str(), _countof(szPath) - 1);
            m_cfgFileName = szPath;
        }
    }

    bool IsExist(void)
    {
        static int const MODE_EXIST = 0;
        return NO_ERROR == _taccess(m_cfgFileName.c_str(), MODE_EXIST);
    }

    void SetSection(LPCTSTR lpSectionName)
    {
        assert(NULL != lpSectionName);
        m_sectionName = lpSectionName;
    }

    bool GetBooleanValue(LPCTSTR keyName, bool defaultValue = false)
    {
        assert(!m_sectionName.empty());
        if (0 != ::GetPrivateProfileInt(m_sectionName.c_str(),
            keyName, 0, m_cfgFileName.c_str()))
        {
            return true;
        }

        return defaultValue;
    }

    INT32 GetInt32Value(LPCTSTR keyName, INT32 defaultValue = 0)
    {
        assert(!m_sectionName.empty());
        return ::GetPrivateProfileInt(m_sectionName.c_str(),
            keyName, defaultValue, m_cfgFileName.c_str());
    }

    LPCTSTR GetStringValue(LPCTSTR keyName, LPCTSTR defaultValue = _T(""))
    {
        assert(!m_sectionName.empty());
        ::GetPrivateProfileString(m_sectionName.c_str(), keyName,
            defaultValue, m_szTempValue, _countof(m_szTempValue),
            m_cfgFileName.c_str());

        return m_szTempValue;
    }
    bool SetStringValue(LPCTSTR keyName, LPCTSTR value = _T(""))
    {
        assert(!m_sectionName.empty());
        return FALSE != ::WritePrivateProfileString(m_sectionName.c_str(),
            keyName, value, m_cfgFileName.c_str());
    }

private:
    std::_tstring m_cfgFileName;
    TCHAR m_szTempValue[2048];
    std::_tstring m_sectionName;
};

#endif // end of GUARD_FCCONFIGPARSER_H
