#ifndef GUARD_FCCONFIGPARSER_H
#define GUARD_FCCONFIGPARSER_H
/**	@file    fcConfigParser.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   ini 文件解析工具,不是线程安全的
 *
 *	@author	 xuezhihu
 *	@date	 2013/3/23
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V0.0.1  创建
 *
 *	@warning 这里填写本文件相关的警告信息
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
        if (std::_tstring::npos == m_cfgFileName.find(_T(":"))) // 是个相对路径
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
