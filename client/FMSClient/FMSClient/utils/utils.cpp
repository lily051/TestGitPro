#include "StdAfx.h"
#include "utils.h"
#include <regex>
#include <sstream>
using namespace std;


#pragma comment(lib, "Iphlpapi.lib")

//lint -efunc(438, DebugMsg)
void DebugMsg(LPCSTR pszFormat, ...)
{
    char buf[4096] = {0};
    va_list arglist;
    va_start(arglist, pszFormat);
    _vsnprintf_s(buf, sizeof(buf) / sizeof(*buf) - 1, pszFormat, arglist); //lint !e534
    va_end(arglist);

    OutputDebugStringA(buf);
}
//lint +efunc(438, DebugMsg)


typedef struct _tagNETADAPTER
{
    DWORD dwPreInBytes;		    // 下载总流量
    DWORD dwPreOutBytes;        // 上传总流量
    DWORD dwInBytesDelt;		// 下载速度
    DWORD dwOutBytesDelt;		// 上传速度

    _tagNETADAPTER()
    {
        dwPreInBytes = 0;
        dwPreOutBytes = 0;
        dwInBytesDelt = 0;
        dwOutBytesDelt = 0;
    }
}NETADAPTER;

std::vector<NETADAPTER> g_vecNetInfo;
void GetNetDelta(__int64& ullIn, __int64& ullOut)
{
    using namespace std;
    ullIn = 0;
    ullOut = 0;

    DWORD dwSize = sizeof(MIB_IFTABLE);
    MIB_IFTABLE	*pIfTable = (MIB_IFTABLE *)malloc(dwSize);
    if (pIfTable == NULL)
    {
        return;
    }

    if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER)
    {
        free(pIfTable);
        pIfTable = (MIB_IFTABLE *)malloc(dwSize);
        if (pIfTable == NULL)
        {
            return;
        }
    }

    if (GetIfTable(pIfTable, &dwSize, FALSE) == NO_ERROR)
    {
        g_vecNetInfo.resize(pIfTable->dwNumEntries);

        std::map<string, DWORD> mpIFs;
        unsigned __int64 ullTotalInDelt = 0;
        unsigned __int64 ullTotalOutDelt = 0;
        for (int i = 0; i < (int) pIfTable->dwNumEntries; i++)
        {
            // Retrieve the transmission of each network card
            MIB_IFROW *pIfRow = (MIB_IFROW *) & pIfTable->table[i];

            if (pIfRow->dwType == IF_TYPE_SOFTWARE_LOOPBACK)
            {
                // loopback doesn't count
                g_vecNetInfo[i].dwInBytesDelt = 0;
                g_vecNetInfo[i].dwOutBytesDelt = 0;
            }else
            {
                // first call doesn't count
                if (g_vecNetInfo[i].dwPreInBytes == 0 && g_vecNetInfo[i].dwPreOutBytes == 0)
                {
                    g_vecNetInfo[i].dwInBytesDelt = 0;
                    g_vecNetInfo[i].dwOutBytesDelt = 0;
                }else
                {
                    // Deal with the overflow case
                    g_vecNetInfo[i].dwInBytesDelt = (g_vecNetInfo[i].dwPreInBytes > pIfTable->table[i].dwInOctets)
                        ? 0xFFFFFFFF - g_vecNetInfo[i].dwPreInBytes + pIfTable->table[i].dwInOctets
                        : pIfTable->table[i].dwInOctets - g_vecNetInfo[i].dwPreInBytes;
                    g_vecNetInfo[i].dwOutBytesDelt = (g_vecNetInfo[i].dwPreOutBytes > pIfTable->table[i].dwOutOctets)
                        ?0xFFFFFFFF - g_vecNetInfo[i].dwPreOutBytes + pIfTable->table[i].dwOutOctets
                        : pIfTable->table[i].dwOutOctets - g_vecNetInfo[i].dwPreOutBytes;
                }
            }

            // 根据bPhysAddr信息过滤重复的网卡信息
            string sPhysic;
            /*if (pIfRow->bPhysAddr == NULL)
            {
                sPhysic = "NULL";
            }else
            {
                sPhysic.assign((char*)pIfRow->bPhysAddr, pIfRow->dwPhysAddrLen);
            }*/
			sPhysic.assign((char*)pIfRow->bPhysAddr, pIfRow->dwPhysAddrLen);
            map<string, DWORD>::const_iterator it = mpIFs.find(sPhysic);
            if (it == mpIFs.end())
            {
                mpIFs[sPhysic] = pIfRow->dwInOctets;
                ullTotalInDelt += g_vecNetInfo[i].dwInBytesDelt;
                ullTotalOutDelt += g_vecNetInfo[i].dwOutBytesDelt;
            }            

            // save for next time
            g_vecNetInfo[i].dwPreInBytes = pIfTable->table[i].dwInOctets;
            g_vecNetInfo[i].dwPreOutBytes = pIfTable->table[i].dwOutOctets;
        }

        ullIn = ullTotalInDelt;
        ullOut = ullTotalOutDelt;
    }

    free(pIfTable);
}

CString FormatNetDelta(__int64& ullTotal)
{
    CString sInfo;
    unsigned __int64 ullSpeed = ullTotal;
    if (ullSpeed / 1024 / 1024 / 1024 > 0) //GB
    {
        float fSpeed = (float)ullSpeed / 1024.0f / 1024.0f / 1024.0f;
        sInfo.Format(_T("%.1fGB/S"), fSpeed);
    }else if (ullSpeed / 1024 / 1024 > 0) //MB
    {
        float fSpeed = (float)ullSpeed / 1024.0f / 1024.0f;
        sInfo.Format(_T("%.1fMB/s"), fSpeed);
    }
    else // KB
    {
        float fSpeed = (float)ullSpeed / 1024.0f;
        sInfo.Format(_T("%.1fKB/S"), fSpeed);
    }

    return sInfo;
}


// 重启按钮响应函数
void RebootApp(const CString& strAppPath, BOOL isForceLogout)
{
    if (!PathFileExists(strAppPath))
    {
        return ;
    }

    // 重启程序不使用自动登录
    ::WriteProfileString(_T("reboot"), _T("switch"), _T("0"));

    ::PostMessage(AfxGetMainWnd()->m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 1);

    // 被强制下线需要提示用户，使用命令行区别
    CString strCmd;
    if (isForceLogout)
    {
        strCmd = _T("forcelogoff");
    }
    else
    {
        strCmd = _T("reboot");
    }

    // 重启程序，reboot参数用于防止进程唯一性检查导致重启失败
    STARTUPINFO StartInfo;
    PROCESS_INFORMATION procStruct;
    memset(&StartInfo, 0, sizeof(STARTUPINFO));
    StartInfo.cb = sizeof(STARTUPINFO);
    ::CreateProcess(
        (LPCTSTR)strAppPath,
        strCmd.GetBuffer(),
        NULL,
        NULL,
        FALSE,
        NORMAL_PRIORITY_CLASS,
        NULL,
        NULL,
        &StartInfo,
        &procStruct);
}

void OpenUrl(const CString& strUrl)
{
    ShellExecute(NULL, _T("open"), strUrl, NULL, NULL, SW_SHOWNORMAL);
}

void GetDirectoryPath(HWND hWnd, CString& strInitPath)
{
	TCHAR szDir[MAX_PATH] = {0};

	BROWSEINFO bi;					//必须传入的参数,下面就是这个结构的参数的初始化
	bi.hwndOwner = hWnd;			//得到父窗口Handle值
	bi.pidlRoot = NULL;				//这个变量就是我们在上面得到的.
	bi.pszDisplayName = szDir;		//得到缓冲区指针
	bi.lpszTitle = NULL;
	bi.ulFlags = BIF_NEWDIALOGSTYLE/*BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE*/;			//设置标志
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;				
	LPITEMIDLIST lpIDList = SHBrowseForFolder(&bi);					//打开对话框

	TCHAR pPath[MAX_PATH];
	if(lpIDList)
	{
		SHGetPathFromIDList(lpIDList, pPath);
		strInitPath = pPath;
	}
}

void GetFilePath(HWND hWnd, CString& strInitPath)
{
    CFileDialog dlgFile(TRUE, NULL, strInitPath, OFN_HIDEREADONLY,
        _T("All files (*.*)|*.*||"), NULL);

    if (IDOK == dlgFile.DoModal())
    {
        strInitPath = dlgFile.GetPathName();
    }
    else
    {
        strInitPath = _T("");
    }
}

void GetVerValue(LPCTSTR lpszKeyName, CString& strVerValue)
{
    typedef struct tagTranslation
    {
        WORD langID;            // language ID
        WORD charset;           // character set (code page)
        tagTranslation()
        {
            langID = 0;
            charset = 0;
        }
    } TRANSLATION;

    TCHAR szPath[MAX_PATH] = {0};
    GetModuleFileName(NULL, szPath, MAX_PATH);

    DWORD dwHandle;
    DWORD dwSize = GetFileVersionInfoSize(szPath, &dwHandle);

    LPVOID m_pVerInfo = NULL;
    TRANSLATION m_translation;
    if (dwSize > 0)
    {
        m_pVerInfo = malloc(dwSize);
        if (GetFileVersionInfo(szPath, 0, dwSize, m_pVerInfo))
        {
            LPVOID lpvi;
            UINT uLen;
            if (VerQueryValue(m_pVerInfo, _T("\\"), &lpvi, &uLen))
            {
                if (VerQueryValue(m_pVerInfo, _T("\\VarFileInfo\\Translation"), &lpvi, &uLen) && uLen >= 4) 
                {
                    m_translation = *(TRANSLATION *)lpvi;
                }
            }
        }
    }

    if (m_pVerInfo)
    {
        CString query;
        query.Format(_T("\\StringFileInfo\\%04x%04x\\%s"),
            m_translation.langID, m_translation.charset, lpszKeyName);

        LPCTSTR pVal;
        UINT iLenVal;
        if (VerQueryValue(m_pVerInfo, (LPTSTR)(LPCTSTR)query, (LPVOID*)&pVal, &iLenVal)) 
        {
            strVerValue = pVal;
        }
    }

    free(m_pVerInfo);
}

bool UTF8_A2W(LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, DWORD* pdwError)
{
	cchWideChar = A2W_Internal(CP_UTF8, 0, lpMultiByteStr, cbMultiByte, lpWideCharStr, pdwError);
	if (!cchWideChar)
	{
		return false;
	}

	return true;
}

bool UTF8_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError)
{
	cbMultiByte = W2A_Internal(CP_UTF8, 0, lpWideCharStr, cchWideChar, lpMultiByteStr, NULL, NULL, pdwError);
	if (!cbMultiByte)
	{
		return false;
	}

	return true;
}

//lint -e413
int A2W_Internal(
				 UINT CodePage, 
				 DWORD dwFlags,         
				 LPCSTR lpMultiByteStr, 
				 int cbMultiByte,       
				 LPWSTR& lpWideCharStr,
				 DWORD* pdwError)
{
	int nRequired = ::MultiByteToWideChar(CodePage, dwFlags,
		lpMultiByteStr, cbMultiByte, lpWideCharStr, 0);
	if (!nRequired)
	{
		DWORD dwError = ::GetLastError();

		if (pdwError)
		{
			*pdwError = dwError;
		}
		return 0;
	}

	lpWideCharStr = (LPWSTR)::malloc(nRequired * sizeof(WCHAR)/* + 1*/);
	if (!lpWideCharStr)
	{		
		if (pdwError)
		{
			*pdwError = ERROR_INSUFFICIENT_BUFFER;
		} 
		return 0;
	}

	nRequired = ::MultiByteToWideChar(CodePage, dwFlags,
		lpMultiByteStr, cbMultiByte, lpWideCharStr, nRequired);
	if (!nRequired)
	{
		DWORD dwError = ::GetLastError();

		if (pdwError)
		{
			*pdwError = dwError;
		} 
		::free(lpWideCharStr);
		lpWideCharStr = NULL;
		return 0;
	}

	//lpWideCharStr[nRequired * sizeof(WCHAR)] = '\0';

	return nRequired;
}
//lint +e413

//lint -e413
int W2A_Internal(
				 UINT CodePage, 
				 DWORD dwFlags, 
				 LPCWSTR lpWideCharStr,
				 int cchWideChar, 
				 LPSTR& lpMultiByteStr, 
				 LPCSTR lpDefaultChar,    
				 LPBOOL lpUsedDefaultChar,
				 DWORD* pdwError)
{
	int nRequired = ::WideCharToMultiByte(CodePage, dwFlags,
		lpWideCharStr, cchWideChar, lpMultiByteStr, 0, lpDefaultChar, lpUsedDefaultChar);
	if (!nRequired)
	{
		DWORD dwError = ::GetLastError();

		if (pdwError)
		{
			*pdwError = dwError;
		} 
		return 0;
	}

	lpMultiByteStr = (LPSTR)::malloc(nRequired * sizeof(CHAR)/* + 1*/);
	if (!lpMultiByteStr)
	{	
		if (pdwError)
		{
			*pdwError = ERROR_INSUFFICIENT_BUFFER;
		} 
		return 0;
	}

	nRequired = ::WideCharToMultiByte(CodePage, dwFlags,
		lpWideCharStr, cchWideChar, lpMultiByteStr, nRequired, lpDefaultChar, lpUsedDefaultChar);
	if (!nRequired)
	{
		DWORD dwError = ::GetLastError();

		if (pdwError)
		{
			*pdwError = dwError;
		} 
		::free(lpMultiByteStr);
		lpMultiByteStr = NULL;
		return 0;
	}

	//lpMultiByteStr[nRequired * sizeof(CHAR)] = '\0';

	return nRequired;
}
//lint +e413

bool CP936_A2W(LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, DWORD* pdwError)
{
	cchWideChar = A2W_Internal(936, 0, lpMultiByteStr, cbMultiByte, lpWideCharStr, pdwError);
	if (!cchWideChar)
	{
		OutputDebugString(_T("a2wi failed"));

		return false;
	}

	return true;
}

bool CP936_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError)
{
	cbMultiByte = W2A_Internal(936, 0, lpWideCharStr, cchWideChar, lpMultiByteStr, NULL, NULL, pdwError);
	if (!cbMultiByte)
	{
		//MYUTILS_ERROR("w2ai failed");

		return false;
	}

	return true;
}

wstring CP936_A2W(const string& strA)
{
	wstring strW;
	WCHAR* pTemp = 0;
	int cwchar = 0;
	if (CP936_A2W(strA.c_str(), strA.size() + 1, pTemp, cwchar))
	{
		strW = pTemp;
		CodeSet_Free(pTemp);
	}
	else
	{
		//MYUTILS_ERROR("ca2w failed");
	}

	return strW;
}
string CP936_W2A(const wstring& strW)
{
	string strA;
	CHAR* pTemp = 0;
	int cchar = 0;
	if (CP936_W2A(strW.c_str(), strW.size() + 1, pTemp, cchar))
	{
		strA = pTemp;
		CodeSet_Free(pTemp);
	}
	else
	{
		//MYUTILS_ERROR("cw2a failed");
	}

	return strA;
}

string UTF8_MFCSTRW2A(const CStringW& strW)
{
	string strA;
	CHAR* pTemp = 0;
	int cchar = 0;
	if (UTF8_W2A((LPCWSTR)strW, strW.GetLength() + 1, pTemp, cchar))
	{
		strA = pTemp;
		CodeSet_Free(pTemp);
	}
	else
	{
	}

	return strA;
}

CStringW UTF8_A2MFCSTRW(const string& strA)
{
	CStringW strW;
	WCHAR* pTemp = 0;
	int cwchar = 0;
	if (UTF8_A2W(strA.c_str(), strA.size() + 1, pTemp, cwchar))
	{
		strW = pTemp;
		CodeSet_Free(pTemp);
	}
	else
	{
	}

	return strW;
}

void CodeSet_Free(void* p)
{
    if (p)
    {
        ::free(p);
    }
}

int My_atoi(CString strNum)
{
    char *pTemp = NULL;
    int nLength;
    CP936_W2A(strNum, strNum.GetLength() + 1, pTemp, nLength);

    int nNum = atoi(pTemp);

    CodeSet_Free(pTemp);

    return nNum;
}

std::string My_itoa(int i)
{
    std::stringstream ss;
    ss << i;
    std::string strTemp = ss.str();
    return strTemp;
}

CString My_itot(int i)
{
    CString strTemp;
    strTemp.Format(_T("%d"), i);
    return strTemp;
}

// 检查是否是IP地址
bool ip_valid(const char* ip)
{
	std::tr1::regex reg("((?:(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d))))");
	bool ret = std::tr1::regex_match(ip, reg);
	return ret;
}

int URLEncode(LPCTSTR pszUrl, LPTSTR pszEncode, int nEncodeLen)
{
    if(pszUrl == NULL)
        return 0;

    if(pszEncode == NULL || nEncodeLen == 0)
        return 0;

    //定义变量
    int nLength = 0;
    char* pString = NULL;

    nLength = WideCharToMultiByte(CP_UTF8, 0, pszUrl, -1, NULL, 0, NULL, NULL);
    pString = new char[nLength];  

    //Unicode转到UTF-8
    nLength = WideCharToMultiByte(CP_UTF8, 0, pszUrl, -1, pString, nLength, NULL, NULL);  

    static char hex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    memset(pszEncode, 0, nEncodeLen * sizeof(TCHAR));

    for( int i = 0; i < nLength - 1; i++ )
    {
        unsigned char c = pString[i];
        if(isalnum(c))    // 数字或字母
        {
            *pszEncode++ = c;
        }
        else if(isspace(c))        // 包含空格
        {
            *pszEncode++ = '+';
        }
        else                        // 进行编码
        {
            *pszEncode++ = '%';  
            *pszEncode++ = hex[c / 16];
            *pszEncode++ = hex[c % 16];
        }
    }

    //删除内存
    delete[] pString;
    return nLength;
}

CString ConvertChar(const CString& strSrc, int nMode)
{
    CString strTmp = strSrc;
    if (0 == nMode)
    {
        int nRet = strTmp.Find(_T('&'), 0);

        while (-1 != nRet)
        {
            strTmp.Insert(nRet, _T('&'));

            nRet = strTmp.Find(_T('&'), nRet + 2);
        }
    }
    else if (1 == nMode)
    {
        // 对‘&’进行转换
        int nRet = strTmp.Find(_T('&'), 0);

        while (-1 != nRet)
        {
            strTmp.Delete(nRet + 1);

            nRet = strTmp.Find(_T('&'), nRet + 1);
        }
    }

    return strTmp;
}

BOOL AppShowSaveFileDlg(const CString& sFileName,const CString& sTitle,const CString& sFilter,
						CString& OutputFileName,CString& strFileExt)
{
	CFileDialog dlgFile(FALSE,NULL,sFileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,sFilter);

	dlgFile.m_ofn.lpstrTitle  = sTitle;
	if (dlgFile.DoModal() == IDOK)
	{
		OutputFileName = dlgFile.GetPathName();
		strFileExt = dlgFile.GetFileExt();

		CFileFind finder;   
		if(finder.FindFile(OutputFileName))   
		{   
			if(!DeleteFile(OutputFileName))//删除已存在文件
			{  
				CString szTemp;
				szTemp = "删除文件失败！";
				AfxMessageBox(szTemp,NULL,MB_OK|MB_ICONEXCLAMATION);
				return FALSE;
			}
		}

		return TRUE;
	}
	return FALSE;
}
