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
#include <stdlib.h>
#include <tchar.h>
#include <Dbghelp.h>    // for MakeSureDirectoryPathExists
#pragma comment(lib, "Dbghelp.lib")

/**************************************************************************
*   Function:		LoadImageFromResource
*   Description:    从资源中读取PNG图片，返回Image *.
*   Input:          UINT uID:资源id
*   Output:         (null)
*   Return:			 成功返回Image *,失败返回NULL.
**************************************************************************/
Image * LoadImageFromResource(UINT uID)
{
	Image * pImage = NULL;
	
	// find resource handle
	LPVOID lpResource = NULL;
	HGLOBAL hResource = NULL;
	
	// 打开资源操作句柄
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(uID), _T("PNG"));
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(uID), _T("PNG"));
	
	if (hRsrc != NULL)
	{
		// load it
		hResource = LoadResource(hInst, hRsrc);
		
		if (hResource == NULL)
		{
			return NULL;
		}
		// lock it
		lpResource = LockResource(hResource);
		
		ASSERT(lpResource != NULL);
	}

	DWORD dwlen = SizeofResource(hInst, hRsrc); 

	//Allocate   global   memory   on   which   to   create   stream 
	HGLOBAL hGlobal = GlobalAlloc(GMEM_FIXED, dwlen); 
	BYTE * pMem = (BYTE*)GlobalLock(hGlobal); 
    if (NULL != lpResource)
    {
        memcpy(pMem, lpResource, dwlen); 
    }
	IStream* pStream = NULL;
	
	if (::CreateStreamOnHGlobal(hGlobal, FALSE, &pStream) == S_OK)
	{
		pImage = Image::FromStream(pStream);
		pStream->Release();

		ASSERT(pImage != NULL);
		ASSERT(pImage->GetLastStatus() == Gdiplus::Ok);
	}

	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);

	// cleanup
	if (lpResource != NULL && hResource != NULL)
	{
		UnlockResource(hResource);
		FreeResource(hResource);
	}
	
	return pImage;
}

/**************************************************************************
*   Function:		LoadImageFromResource
*   Description:    从资源中读取PNG图片，返回Bitmap *.
*   Input:          UINT uID:资源id
*   Output:         (null)
*   Return:			 成功返回Image *,失败返回NULL.
**************************************************************************/
Bitmap * LoadBitmapFromResource(UINT uID)
{
	HINSTANCE hInst = AfxGetResourceHandle(); 

	Bitmap * pBitmap = NULL;

	HRSRC hResource = ::FindResource(hInst, MAKEINTRESOURCE(uID), _T("PNG"));   //   type 
	if (!hResource)
	{
		return NULL;
	}

	DWORD imageSize = ::SizeofResource(hInst, hResource);
	if (!imageSize)
	{
		return NULL;
	}

	const void* pResourceData = ::LockResource(::LoadResource(hInst, hResource));
	if (!pResourceData)
	{
		return NULL;
	}

	void * hBuffer  = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
	
	if (hBuffer)
	{
		void* pBuffer = ::GlobalLock(hBuffer);
		
		if (pBuffer)
		{
			CopyMemory(pBuffer, pResourceData, imageSize);

			IStream* pStream = NULL;
			if (::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream) == S_OK)
			{
				pBitmap = Gdiplus::Bitmap::FromStream(pStream);
				pStream->Release();
				if (pBitmap)
				{ 
					if (pBitmap->GetLastStatus() != Gdiplus::Ok)
					{
						delete pBitmap;
						return NULL;
					}
				}
			}
			::GlobalUnlock(hBuffer);
		}
		
		::GlobalFree(hBuffer);
		hBuffer = NULL;
	}

	return pBitmap;
}


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
	HINSTANCE hinstRes = AfxFindResourceHandle(MAKEINTRESOURCE(nID),_T("PNG"));

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

HANDLE CreateMyFile(const CString &strFileName)
{
	HANDLE hFileHandle = NULL;
	CString strError;
	DWORD dwDirattr = 0;

	hFileHandle = CreateFile(strFileName,
		GENERIC_WRITE | GENERIC_READ,                
		FILE_SHARE_READ | FILE_SHARE_WRITE,             
		NULL,                        
		CREATE_ALWAYS,               
		FILE_ATTRIBUTE_NORMAL,       
		NULL	
		);

	strError.Format(_T("\n@lrh -- CreateFile-- ErrorCode = %u\n"), ::GetLastError());
	OutputDebugString(strError);

	if(INVALID_HANDLE_VALUE == hFileHandle)
	{
		strError.Format(_T("\n@lrh -- CreateFile failed -- ErrorCode = %u\n"), ::GetLastError());
		OutputDebugString(strError);
		return NULL;
	}

	return hFileHandle;
}

/** @fn         SaveBinaryFile
 *  @brief      保存数据到指定文件
 *  @param[IN]  pData: 缓冲数据
 *  @param[IN]  nDataSize: 数据大小
 *  @param[IN]  strFilePath: 文件名
 *  @return     TRUE: 成功，FALSE: 失败
 */
BOOL SaveBinaryFile(PBYTE pData, DWORD nDataSize, const CString &strFilePath)
{
    if (pData == NULL || nDataSize <= 0)
    {
        return FALSE;
    }

    CString strError;
    DWORD dwResult = 0;
    HANDLE hFile = CreateMyFile(strFilePath);
    if (hFile == NULL)
    {
        return FALSE;
    }

    if(!WriteFile(hFile, pData, nDataSize, &dwResult, NULL))
    {
        strError.Format(_T("Could not write to file %s (error %d)\n"), strFilePath, ::GetLastError());
        OutputDebugString(strError);
        CloseHandle(hFile);
        return FALSE;
    }

    CloseHandle(hFile);
    return TRUE;
}

/** @fn         SuperMakeDir
 *  @brief      检查指定目录是否存在，如果不存在则创建整个strFile所表示的整个目
 *  @param[IN]  strFilePath: 文件的绝对路径
 *  @return     TRUE: 成功，FALSE: 失败
 */
BOOL SuperMakeDir(PCSTR strFilePath)
{
    BOOL bRet = MakeSureDirectoryPathExists(strFilePath);
    if (!bRet)
    {
        PV_ERROR("...[%s]Directory NotExist and CreateFailed...", strFilePath);
    }
    return bRet;
}

HANDLE Create_File(TCHAR *csFileName)
{
	HANDLE hFileHandle = NULL;
	CString strError = _T("");
	DWORD dwDirattr = 0;
	CFileFind tempFind;

	dwDirattr = GetFileAttributes(csFileName);

	//dsx 注释掉if(dwDirattr == 0xFFFFFFFF)
	//可以覆盖保存

	//if(dwDirattr == 0xFFFFFFFF)
	{
		hFileHandle = CreateFile(csFileName,
			GENERIC_WRITE,                
			FILE_SHARE_READ,             
			NULL,                        
			OPEN_ALWAYS,               
			FILE_ATTRIBUTE_NORMAL,       
			NULL	
			);

		if(INVALID_HANDLE_VALUE == hFileHandle)
		{
			strError.Format(_T("打开文件 %s 失败"),csFileName);
			//OutputDebugString(strError);
			return NULL;
		}
	}


	return hFileHandle;
}

BOOL CreateFileDir(TCHAR *csFilePath)
{
	CString strPath     = _T("");
	CString strPathTemp = _T("");
	strPath.Format(_T("%s"), csFilePath);

	CString strTemp = _T("");
	DWORD dwDirAttr = GetFileAttributes(strPath);

	//若是根目录或文件夹(若是根目录：如C:\ 这个字符的长度的3)
	if(strPath.GetLength()  <= 3 || \
		(0xFFFFFFFF != dwDirAttr&&FILE_ATTRIBUTE_DIRECTORY == (FILE_ATTRIBUTE_DIRECTORY&dwDirAttr)))
	{
		return TRUE;
	}

	if (strPath.GetAt(strPath.GetLength()-1) == '\\')
	{
		strPathTemp.Format(_T("%s"),strPath);
	}
	else
	{
		strPathTemp.Format(_T("%s\\"),strPath);
	}

	int iPos = strPathTemp.Find(_T("\\"), 0);

	while (iPos > 0)
	{
		strTemp.Format(_T("%s"), strPathTemp.Left(iPos+1));
		dwDirAttr = GetFileAttributes(strTemp);

		if (strTemp.GetLength()  > 3 &&  \
			(0xFFFFFFFF == dwDirAttr || FILE_ATTRIBUTE_DIRECTORY != (FILE_ATTRIBUTE_DIRECTORY&dwDirAttr))) 
		{
			if (!CreateDirectory(strTemp, NULL))
			{
				return FALSE;
			}
		}

		iPos = strPathTemp.Find(_T("\\"), iPos+1);
	}

	strPath.Format(_T("%s"), strPathTemp);

	return TRUE;
}

/** @fn         CheckDiskSpace
 *  @brief      检查指定磁盘是否有可用空间
 *  @param[IN]  strDisk: 指定盘符
 *  @param[IN]  dwFreeSpace: 可用空间下限(MB为单位)
 *  @return     TRUE: 有，FALSE: 无
 */
BOOL CheckDiskSpace(const CString &strDisk, DWORD dwFreeSpace)
{
    BOOL bRet = FALSE;
    DWORD dwFreeDiskSpace = 0;
    ULARGE_INTEGER uiFreeBytesAvailableToCaller;
    ULARGE_INTEGER uiTotalNumberOfBytes;
    ULARGE_INTEGER uiTotalNumberOfFreeBytes;

    if(GetDiskFreeSpaceEx(strDisk, &uiFreeBytesAvailableToCaller, &uiTotalNumberOfBytes, &uiTotalNumberOfFreeBytes))
    {
        dwFreeDiskSpace = (DWORD)(uiFreeBytesAvailableToCaller.QuadPart >> 20);
        if (dwFreeDiskSpace > dwFreeSpace)
        {
            bRet = TRUE;
        }
    }
    return bRet;
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
		
		if (NULL != pdwError)
		{
			*pdwError = dwError;
		}            
		return 0;
	}

	lpWideCharStr = (LPWSTR)::malloc(nRequired * sizeof(WCHAR));
	if (!lpWideCharStr)
	{		
		if (NULL != pdwError)
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
		
		if (NULL != pdwError)
		{
			*pdwError = dwError;
		} 
		::free(lpWideCharStr);
		lpWideCharStr = NULL;
		return 0;
	}

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
		
		if (NULL != pdwError)
		{
			*pdwError = dwError;
		} 
		return 0;
	}

	lpMultiByteStr = (LPSTR)::malloc(nRequired * sizeof(CHAR));
	if (!lpMultiByteStr)
	{	
		if (NULL != pdwError)
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
		
		if (NULL != pdwError)
		{
			*pdwError = dwError;
		} 
		::free(lpMultiByteStr);
		lpMultiByteStr = NULL;
		return 0;
	}

	return nRequired;
}

bool CP936_A2W(LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, DWORD* pdwError)
{
	cchWideChar = A2W_Internal(936, 0, lpMultiByteStr, cbMultiByte, lpWideCharStr, pdwError);
	if (!cchWideChar)
	{
		PV_ERROR("a2wi failed");

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
	::free(p);
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

HWND MyCreateToolTip(int toolID, HWND hDlg, TCHAR* pText)
{
	// toolID:  the resource ID of the control.
	// hDlg:    the handle of the dialog box.
	// pText:   the text that appears in the tooltip.
	// hInst:   the global instance handle.

	if (!toolID || !hDlg || !pText)
	{
		return NULL;
	}
	// Get the window of the tool.
	HWND hwndTool = ::GetDlgItem(hDlg, toolID);
	HINSTANCE hInst = GetModuleHandle(NULL);
	// Create the tooltip.
#ifndef TTS_BALLOON
#define TTS_BALLOON             0x40
#endif

	HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
		//WS_POPUP |TTS_ALWAYSTIP | TTS_BALLOON,//泡泡型
		WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,//普通方块型
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hDlg, NULL, 
		hInst, NULL);

	if (!hwndTool || !hwndTip)
	{
		return NULL;
	}                              

	// Associate the tooltip with the tool.
	TOOLINFO toolInfo = { 0 };
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = hDlg;
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.uId = (UINT_PTR)hwndTool;
	toolInfo.lpszText = pText;
	::SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
	
	return hwndTip;
}