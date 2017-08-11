/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       utils.cpp
*   Description:    实用函数
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-10-20 Monday
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-10-20      JiangTianjing   created
**************************************************************************/
//lint -library
#include "stdafx.h"
#include "utils.h"
#include <afxpriv2.h>
#include <xstring>
#include <regex>
#include <sstream>

#define WINDOW_ROUND_SIZE   6

#define COLOR_TRANSPANTE	RGB(255, 0, 255)

/**************************************************************************
*   Function:		SetRegion
*   Description:    按图片设置窗口的形状区域
*   Input:          CWnd * pWnd:窗口指针
*					CBitmap & bitmap:位图
*   Output:         (null)
*   Return:			 成功返回Image *,失败返回NULL.
**************************************************************************/
void SetRegion(CWnd * pWnd, CBitmap & bitmap)
{
    CDC * pDC = pWnd->GetDC();

    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    CBitmap * pOldBitmap = memDC.SelectObject(&bitmap);

    CRect wndRect;
    pWnd->GetWindowRect(&wndRect);

    CRgn wndRgn;
    wndRgn.CreateRectRgn(0, 0, wndRect.Width(), wndRect.Height());

	// 按图片去掉四个圆角

    // four corner
    for (int cx = 0; cx <= WINDOW_ROUND_SIZE; ++cx)
    {
        for (int cy = 0; cy <= WINDOW_ROUND_SIZE; ++cy)
        {
            if (memDC.GetPixel(cx, cy) == COLOR_TRANSPANTE)
            {
                CRgn rgnTemp;
                rgnTemp.CreateRectRgn(cx, cy, cx + 1, cy + 1);
                wndRgn.CombineRgn(&wndRgn, &rgnTemp, RGN_XOR);
            }
        }
    }

    for (int cx = wndRect.Width() - WINDOW_ROUND_SIZE; cx <= wndRect.Width(); ++cx)
    {
        for (int cy = 0; cy <= WINDOW_ROUND_SIZE; ++cy)
        {
            if (memDC.GetPixel(cx, cy) == COLOR_TRANSPANTE)
            {
                CRgn rgnTemp;
                rgnTemp.CreateRectRgn(cx, cy, cx + 1, cy + 1);
                wndRgn.CombineRgn(&wndRgn, &rgnTemp, RGN_XOR);
            }
        }
    }

    for (int cx = 0; cx <= WINDOW_ROUND_SIZE; ++cx)
    {
        for (int cy = wndRect.Height() - WINDOW_ROUND_SIZE; cy <= wndRect.Height(); ++cy)
        {
            if (memDC.GetPixel(cx, cy) == COLOR_TRANSPANTE)
            {
                CRgn rgnTemp;
                rgnTemp.CreateRectRgn(cx, cy, cx + 1, cy + 1);
                wndRgn.CombineRgn(&wndRgn, &rgnTemp, RGN_XOR);
            }
        }
    }

    for (int cx = wndRect.Width() - WINDOW_ROUND_SIZE; cx <= wndRect.Width(); ++cx)
    {
        for (int cy = wndRect.Height() - WINDOW_ROUND_SIZE; cy <= wndRect.Height(); ++cy)
        {
            if (memDC.GetPixel(cx, cy) == COLOR_TRANSPANTE)
            {
                CRgn rgnTemp;
                rgnTemp.CreateRectRgn(cx, cy, cx + 1, cy + 1);
                wndRgn.CombineRgn(&wndRgn, &rgnTemp, RGN_XOR);
            }
        }
    }

    memDC.SelectObject(pOldBitmap);

    pWnd->SetWindowRgn(wndRgn, TRUE);

    ReleaseDC(pWnd->GetSafeHwnd(), pDC->m_hDC);
}

HBITMAP ResourceLoad_Bmp(UINT nID)
{
	HBITMAP hBmp = NULL;
	HINSTANCE hinstRes = AfxFindResourceHandle(MAKEINTRESOURCE(nID), _T("PNG"));

	AfxSetResourceHandle(hinstRes);   

	hBmp =  ::LoadBitmap(hinstRes,MAKEINTRESOURCE(nID));  

	HINSTANCE hInstance = ::AfxGetInstanceHandle();
	AfxSetResourceHandle(hInstance);

	if(hBmp == NULL)
	{
	}

	return hBmp;
}

//////////////////////////////////////////////////////////////////////////
int GetFontSize(int nPixelSize)
{
	HDC hDC = ::GetDC(NULL);
	int iRet = -MulDiv(nPixelSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	::ReleaseDC(NULL,hDC);
	return iRet;
}

int GetFontSize0()
{
	CString clString;
	int nSize = 8;
	return nSize;
}
CString GetFontName()
{
	CString clString(_T("Tahoma"));
	return clString;
}

int GetFontCharset()
{
	int nCharSet = 0;
	CString clString(_T("0"));
	return nCharSet;
}

/*************************************************
Function:    	CheckStrLegality
Description:	查看CString是否合法(% ％　')
Input:          CString str,被检查的字符串
Output:      	
Return:			无
祁梦星 2009年10月26日8:55:25 添加
*************************************************/
BOOL AppFun_CheckStrLegality(CString str)
{
	str.Trim();

	if (_tcscmp(str, _T(""))==0)
	{
		return FALSE;
	}
	else if (str.Find(_T("%"))>=0 || str.Find(_T("'"))>=0 || str.Find(_T("％"))>=0)
	{
		return FALSE;
	}
	if (_tcscmp(str, _T(" ")) == 0)
	{
		return FALSE;
	}

	return TRUE;
}

//获取程序运行的路径
void AppFun_GetAppPath(CString &strPathName)
{
	TCHAR szFileName[MAX_PATH];

	if (0 != GetModuleFileName(NULL, szFileName, MAX_PATH - 1))
	{
		szFileName[MAX_PATH - 1] = '\0';
	}

	TCHAR * p = _tcsrchr(szFileName, '\\') + 1;

	if (p)
	{
		*p = '\0';

		strPathName = szFileName;
	}
}


/*************************************************
Function:    	AppFun_GetProcessIDByName
Description:	根据进程的名称获取进程的ID
Input:			pName进程名称
Output:      	
Return:			-1获取失败，非-1获取成功
*************************************************/
DWORD AppFun_GetProcessIDByName(LPCTSTR pName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	DWORD dwPID = -1;
	if (INVALID_HANDLE_VALUE == hSnapshot) 
	{
		return -1;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL fOk;
	for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe)) 
	{
		if (!_tcscmp(pe.szExeFile, pName)) 
		{
			CloseHandle(hSnapshot);
			dwPID = pe.th32ProcessID;
			return dwPID;
		}
	}
	return -1;
}

/*************************************************
Function:	 AppFun_GetDirectoryPath
Description: 路径选择
Input:       hWnd,要获取路径的窗口句柄, strPath指定展开的位置[目前还没有实现]
Output:      无
Return:		 路径 Str
*************************************************/
CString AppFun_GetDirectoryPath(HWND hWnd, CString strPath)
{
	CString szTemp;
	LPITEMIDLIST pidlRoot = NULL;
	SHGetSpecialFolderLocation(hWnd,CSIDL_DRIVES,&pidlRoot);
	BROWSEINFO bi;								  //必须传入的参数,下面就是这个结构的参数的初始化
	CString strDisplayName;						  //用来得到,你选择的活页夹路径,相当于提供一个缓冲区
	bi.hwndOwner = hWnd;				  //得到父窗口Handle值
	bi.pidlRoot = pidlRoot;						  //这个变量就是我们在上面得到的.
	bi.pszDisplayName = strDisplayName.GetBuffer(MAX_PATH + 1);		 //得到缓冲区指针
	szTemp = _T("");
	bi.lpszTitle = szTemp.GetBuffer(szTemp.GetLength() + 1);//设置标题
	szTemp.ReleaseBuffer();
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;			 //设置标志
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;				
	LPITEMIDLIST lpIDList = SHBrowseForFolder(&bi);					//打开对话框
	strDisplayName.ReleaseBuffer();									//和上面的GetBuffer()相对应
	TCHAR pPath[MAX_PATH];
	CString Str;
	if(lpIDList)
	{
		SHGetPathFromIDList(lpIDList, pPath);
		Str = pPath;
	}
	return Str;
}

//获取硬盘可用信息
CString AppFun_GetDiskAvailableInfo()
{
	DWORD dwLogical = GetLogicalDriveStrings(0,NULL);
	LPTSTR pAllDrivers=new wchar_t[dwLogical];
	::GetLogicalDriveStrings(dwLogical,pAllDrivers);

	LPTSTR pDriver=pAllDrivers;
	wchar_t chTempDriver[26] = L"";


	int  iDriverNum = 0;
	while(pDriver[0]!=0)
	{
		chTempDriver[iDriverNum++]=*pDriver;
		pDriver=_tcschr(pDriver,0)+1;          //定位到下一个盘符
	}

	int iDiverCount = 0;
	CStringW strRet;
	strRet = _T("磁盘可用情况:   ");
	for(int i = 0;i < iDriverNum;i++)
	{
		CStringW strRootPath = L"";
		strRootPath.Format(_T("%c%s"),chTempDriver[i], _T(":\\"));
		int nType = GetDriveType(strRootPath);
		if(DRIVE_FIXED == nType || DRIVE_REMOVABLE == nType)
		{
			ULARGE_INTEGER ulAvailable, ulTotal, ulTotalFree;
			if (GetDiskFreeSpaceEx(strRootPath, &ulAvailable, &ulTotal, &ulTotalFree))
			{
				CStringW strText;
				strText.Format(_T("%c: %.2fG;  "), chTempDriver[i], (float)ulAvailable.QuadPart/(1024*1024*1024));
				strRet += strText;
			}
		}
	}
	delete[] pAllDrivers;
	return strRet;
}

/** @fn AppFun_GeLocalIPAddr
*   @brief 或者本机IP
*   @param[out] strIPAddr: ip
*   @param NULL
*   @return NULL
*/
BOOL AppFun_GetLocalIPAddr(CString & strIPAddr)
{
	WORD   wVersionRequested;   
	WSADATA   wsaData;   
	char   name[255];   
	char   szIp[16];
	PHOSTENT   hostinfo;   
	wVersionRequested = MAKEWORD(2,0);  
 
 	if(WSAStartup(wVersionRequested,&wsaData) == 0)
 	{   
 		if(gethostname(name, sizeof(name)) == 0)
 		{   
			if((hostinfo = gethostbyname(name)) != NULL)   
			{   
				sprintf_s(szIp, 16, inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list));
			}   
 		}   
		WSACleanup();
	}  
	int nLength;
	wchar_t* wszIp = NULL;
	CP936_A2W(szIp, strlen(szIp)+1, wszIp, nLength);
	strIPAddr.Format(_T("%s"), wszIp);
	CodeSet_Free(wszIp);
	if (!strIPAddr.IsEmpty())
	{
		return TRUE;
	}
	return FALSE;
}

void AppFun_GetAllLocalIPAddr(std::vector<CString>& vecIP)   
{       
    PMIB_IPADDRTABLE pIPAddrTable;
    DWORD dwSize = 0;
    pIPAddrTable = (MIB_IPADDRTABLE*) GlobalAlloc(GPTR, sizeof(MIB_IPADDRTABLE));
    if ( pIPAddrTable ) 
    {
        if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) 
        {
            GlobalFree( pIPAddrTable );
            pIPAddrTable = (MIB_IPADDRTABLE *) GlobalAlloc (GPTR, dwSize);
        }
    }
    else
    {
        OutputDebugString(_T("@ljg-Memory allocation failed."));
    }

    if ( pIPAddrTable ) 
    {
        // Make a second call to GetIpAddrTable to get the
        // actual data we want
        if ( GetIpAddrTable( pIPAddrTable, &dwSize, 0 ) == NO_ERROR ) 
        { 
            for (unsigned int i=0;i<dwSize/sizeof(MIB_IPADDRTABLE);++i)
            {
                if (pIPAddrTable->table[i].dwAddr != 0)
                {
                    in_addr in;
                    in.S_un.S_addr = pIPAddrTable->table[i].dwAddr;
                    CString tempIP;
                    tempIP = ::inet_ntoa(in);
                    vecIP.push_back(tempIP);
                }
            }
        }
        else 
        {
            OutputDebugString(_T("@ljg-Call to GetIpAddrTable failed."));
        }
    }

    if (pIPAddrTable)
    {
        GlobalFree( pIPAddrTable );
    }
}

/*************************************************
Function:    	CheckStrLegality
Description:	查看输入是否合法(% ％　')
Input:          CString csString,被检查的字符串
int nMaxLen,路径最大长度
Output:      	
Return:			返回枚举类型 CHECKPATH_SUCESS成功 否则失败
add by yudan at 2010-08-20 15:25
*************************************************/
ENUM_CHECKPATH_LEAGL AppFun_CheckStrLegality(CString csString, int nMaxLen)
{
	csString.Trim();

	if (0 == _tcscmp(csString, L""))
	{
		return CHECKPATH_EMPTY;
	}

	if (csString.Find(_T("%"))>=0 || csString.Find(_T("'"))>=0 || csString.Find(_T("％"))>=0)
	{
		return CHECKPATH_ILLEAGLCHAR;
	}

	if (0 == _tcscmp(csString, _T(" ")))
	{
		return CHECKPATH_EMPTY;
	}

	if (csString.GetLength() > nMaxLen)
	{
		return CHECKPATH_OVERLENGTH;
	}

	return CHECKPATH_SUCCESS;
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

		if (!pdwError)
		{
			*pdwError = dwError;
		}            
		return 0;
	}

	lpWideCharStr = (LPWSTR)::malloc(nRequired * sizeof(WCHAR)/* + 1*/);
	if (!lpWideCharStr)
	{		
		if (!pdwError)
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

		if (!pdwError)
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

		if (!pdwError)
		{
			*pdwError = dwError;
		} 
		return 0;
	}

	lpMultiByteStr = (LPSTR)::malloc(nRequired * sizeof(CHAR)/* + 1*/);
	if (!lpMultiByteStr)
	{	
		if (!pdwError)
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

		if (!pdwError)
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
	pTemp = NULL;

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

CStringA CStringW2A(CStringW strText)
{
	int nLenght;
	char *pTemp = NULL;

	CP936_W2A(strText, strText.GetLength() + 1, pTemp, nLenght);

	CStringA strData;
	strData.Format("%s", pTemp);

	CodeSet_Free(pTemp);
	pTemp = NULL;

	return strData;
}


bool ip_valid(const char* ip)
{
    std::tr1::regex reg("((?:(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d))))");
    bool ret = std::tr1::regex_match(ip, reg);
    return ret;
}

//lint -efunc(438, DebugMsg)
void DebugMsg(LPCTSTR pszFormat, ...) throw()
{
    TCHAR buf[4096] = {0};
    va_list arglist;
    va_start(arglist, pszFormat);
    _vsntprintf_s(buf, sizeof(buf) / sizeof(*buf) - 1, pszFormat, arglist); //lint !e534
    va_end(arglist);

    OutputDebugString(buf);
}
//lint +efunc(438, DebugMsg)

//////////////////////////////////////////////////////////////////////////