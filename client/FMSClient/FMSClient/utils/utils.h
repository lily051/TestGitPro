#pragma once

void DebugMsg(LPCSTR pszFormat, ...);

void GetNetDelta(__int64& ullIn, __int64& ullOut);

CString FormatNetDelta(__int64& i);

void RebootApp(const CString& strAppPath, BOOL isForceLogout = FALSE);

void OpenUrl(const CString& strUrl);

void GetDirectoryPath(HWND hWnd, CString& strInitPath);

void GetFilePath(HWND hWnd, CString& strInitPath);

void GetVerValue(LPCTSTR lpszKeyName, CString& strVerValue);

// URL编码，用于支持Web,调
// pszEncode的长度建议为 使用WideCharToMultiByte函数判断输出字符的长度 * 3
// 全是中文时，最小长度需要 （使用WideCharToMultiByte函数判断输出字符的长度 - 1） * 3
int URLEncode(LPCTSTR pszUrl, LPTSTR pszEncode, int nEncodeLen);

// 对字符串中的&符号进行转换
// nMode=0，在&前加上一个&；nMode=1，双&&转化成单&
CString ConvertChar(const CString& strSrc, int nMode);


// dsx unicode utf8 cp936 字符编码和解码相关的函数---begin
int A2W_Internal(UINT CodePage,DWORD dwFlags,LPCSTR lpMultiByteStr,int cbMultiByte,LPWSTR& lpWideCharStr,DWORD* pdwError);
int W2A_Internal(UINT CodePage,DWORD dwFlags,LPCWSTR lpWideCharStr,int cchWideChar,LPSTR& lpMultiByteStr,LPCSTR lpDefaultChar,LPBOOL lpUsedDefaultChar,DWORD* pdwError);
bool CP936_A2W(LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, DWORD* pdwError = NULL);
bool CP936_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError = NULL);
bool UTF8_A2W(LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, DWORD* pdwError = NULL);
bool UTF8_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError = NULL);
std::wstring CP936_A2W(const std::string& strA);
std::string CP936_W2A(const std::wstring& strW);
CStringA CStringW2A(CStringW strText);
std::string UTF8_MFCSTRW2A(const CStringW& strW);
CStringW UTF8_A2MFCSTRW(const std::string& strA);

void CodeSet_Free(void* p);
int My_atoi(CString strNum);
std::string My_itoa(int i);
CString My_itot(int i);

bool ip_valid(const char* ip);

#pragma comment(lib, "version.lib")


// 获取版本
class CModuleVerInfo
{
public:
    CModuleVerInfo(LPCTSTR lpszMoudleName)
        : m_pVerInfo(NULL)
    {
        // read file version info
        DWORD dwHandle; // a variable that the function sets to zero. 
        DWORD dwSize = GetFileVersionInfoSize(lpszMoudleName, &dwHandle);

        if (dwSize > 0)
        {
            m_pVerInfo = malloc(dwSize); // allocate version info

            if (::GetFileVersionInfo(lpszMoudleName, 0, dwSize, m_pVerInfo))
            {
                LPVOID lpvi;
                UINT uLen;

                if (VerQueryValue(m_pVerInfo, _T("\\"), &lpvi, &uLen))
                {
                    //*(VS_FIXEDFILEINFO*)this = *(VS_FIXEDFILEINFO*)lpvi;

                    // Get translation info
                    if (VerQueryValue(m_pVerInfo, _T("\\VarFileInfo\\Translation"), &lpvi, &uLen) && uLen >= 4) 
                    {
                        m_translation = *(TRANSLATION *)lpvi;
                    }
                }
            }
        }
    }
    ~CModuleVerInfo()
    {
        if (m_pVerInfo)
        {
            free(m_pVerInfo);
        }
    }

    ////
    void GetVerValue(LPCTSTR lpszKeyName, CString & strVerValue)
    {
        if (m_pVerInfo)
        {
            // To get a string value must pass query in the form
            //
            //    "\StringFileInfo\<langID><codepage>\keyname"
            //
            // where <lang-codepage> is the languageID concatenated with the
            // code page, in hex.
            //
            CString query;
            query.Format(_T("\\StringFileInfo\\%04x%04x\\%s"), m_translation.langID, m_translation.charset, lpszKeyName);

            LPCTSTR pVal;
            UINT iLenVal;

            if (VerQueryValue(m_pVerInfo, (LPTSTR)(LPCTSTR)query, (LPVOID*)&pVal, &iLenVal)) 
            {
                strVerValue = pVal;
            }
        }
    }

protected:
    void * m_pVerInfo;		// version information buffer

    typedef struct tagTranslation
    {
        WORD langID;			// language ID
        WORD charset;			// character set (code page)

        tagTranslation()
        {
            langID = 0;
            charset= 0;
        }
    } TRANSLATION, *PTRANSLATION;

    TRANSLATION m_translation;
};


BOOL AppShowSaveFileDlg(const CString& sFileName,const CString& sTitle,const CString& sFilter,CString& OutputFileName,CString& strFileExt);
