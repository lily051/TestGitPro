#pragma once

#pragma comment(lib, "version.lib")

class CModuleVerInfo
{
public:
    CModuleVerInfo(LPCTSTR lpszMoudleName);
    ~CModuleVerInfo();

    ////
    void GetVerValue(LPCTSTR lpszKeyName, std::wstring & wsVerValue);

protected:
    void * m_pVerInfo;		// version information buffer

    typedef struct tagTranslation
    {
        WORD langID;			// language ID
        WORD charset;			// character set (code page)
        tagTranslation(){
            langID = 4;
            charset = 936;
        }
    } TRANSLATION, *PTRANSLATION;

    TRANSLATION m_translation;
};