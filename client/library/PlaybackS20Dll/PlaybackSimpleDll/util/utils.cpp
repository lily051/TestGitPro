/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       utils.cpp
*   Description:    ʵ�ú���
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-10-20 Monday
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-10-20      JiangTianjing   created
**************************************************************************/
//lint -library
#include "stdafx.h"
#include "../stdafx.h"
#include "utils.h"
#include <afxpriv2.h>
using namespace std;

/**************************************************************************
*   Function:		LoadImageFromResource
*   Description:    ����Դ�ж�ȡPNGͼƬ������Image *.
*   Input:          UINT uID:��Դid
*   Output:         (null)
*   Return:			 �ɹ�����Image *,ʧ�ܷ���NULL.
**************************************************************************/
Image * LoadImageFromResource(UINT uID)
{
	Image * pImage = NULL;
	
	// find resource handle
	LPVOID lpResource = NULL;
	HGLOBAL hResource = NULL;
	
	// ����Դ�������
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
*   Description:    ����Դ�ж�ȡPNGͼƬ������Bitmap *.
*   Input:          UINT uID:��Դid
*   Output:         (null)
*   Return:			 �ɹ�����Image *,ʧ�ܷ���NULL.
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
*   Description:    ��ͼƬ���ô��ڵ���״����
*   Input:          CWnd * pWnd:����ָ��
*					CBitmap & bitmap:λͼ
*   Output:         (null)
*   Return:			 �ɹ�����Image *,ʧ�ܷ���NULL.
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

	// ��ͼƬȥ���ĸ�Բ��

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

	//AfxSetResourceHandle(hinstRes);   

	hBmp =  ::LoadBitmap(hinstRes,MAKEINTRESOURCE(nID));  

	HINSTANCE hInstance = ::AfxGetInstanceHandle();
	//AfxSetResourceHandle(hInstance);

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
Description:	�鿴CString�Ƿ�Ϸ�(% ����')
Input:          CString str,�������ַ���
Output:      	
Return:			��
������ 2009��10��26��8:55:25 ���
*************************************************/
BOOL AppFun_CheckStrLegality(CString str)
{
	str.Trim();

	if (_tcscmp(str, _T(""))==0)
	{
		return FALSE;
	}
	else if (str.Find(_T("%"))>=0 || str.Find(_T("'"))>=0 || str.Find(_T("��"))>=0)
	{
		return FALSE;
	}
	if (_tcscmp(str, _T(" ")) == 0)
	{
		return FALSE;
	}

	return TRUE;
}

//��ȡ�������е�·��
void AppFun_GetAppPath(CString &strPathName)
{
	TCHAR szFileName[MAX_PATH];

	if (0 != GetModuleFileName(AfxGetInstanceHandle(), szFileName, MAX_PATH - 1))
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
Description:	���ݽ��̵����ƻ�ȡ���̵�ID
Input:			pName��������
Output:      	
Return:			-1��ȡʧ�ܣ���-1��ȡ�ɹ�
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
Description: ·��ѡ��
Input:       hWnd,Ҫ��ȡ·���Ĵ��ھ��, strPathָ��չ����λ��[Ŀǰ��û��ʵ��]
Output:      ��
Return:		 ·�� Str
*************************************************/
CString AppFun_GetDirectoryPath(HWND hWnd, CString strPath)
{
	CString szTemp;
	LPITEMIDLIST pidlRoot = NULL;
	SHGetSpecialFolderLocation(hWnd,CSIDL_DRIVES,&pidlRoot);
	BROWSEINFO bi;								  //���봫��Ĳ���,�����������ṹ�Ĳ����ĳ�ʼ��
	CString strDisplayName;						  //�����õ�,��ѡ��Ļ�ҳ��·��,�൱���ṩһ��������
	bi.hwndOwner = hWnd;				  //�õ�������Handleֵ
	bi.pidlRoot = pidlRoot;						  //���������������������õ���.
	bi.pszDisplayName = strDisplayName.GetBuffer(MAX_PATH + 1);		 //�õ�������ָ��
	szTemp = _T("ѡ���ļ���");
	bi.lpszTitle = szTemp.GetBuffer(szTemp.GetLength() + 1);//���ñ���
	szTemp.ReleaseBuffer();
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;			 //���ñ�־
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;				
	LPITEMIDLIST lpIDList = SHBrowseForFolder(&bi);					//�򿪶Ի���
	strDisplayName.ReleaseBuffer();									//�������GetBuffer()���Ӧ
	TCHAR pPath[MAX_PATH];
	CString Str;
	if(lpIDList)
	{
		SHGetPathFromIDList(lpIDList, pPath);
		Str = pPath;
	}
	return Str;
}

//��ȡӲ�̿�����Ϣ
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
		pDriver=_tcschr(pDriver,0)+1;          //��λ����һ���̷�
	}

	int iDiverCount = 0;
	CStringW strRet;
	strRet = _T("���̿������:   ");
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
*   @brief ���߱���IP
*   @param[out] strIPAddr: ip
*   @param NULL
*   @return NULL
*/
BOOL AppFun_GetLocalIPAddr(CString& strIPAddr)
{
	WORD   wVersionRequested;   
	WSADATA   wsaData;   
	char   name[255];   
	PHOSTENT   hostinfo;   
	wVersionRequested = MAKEWORD(2,0);   

	if(WSAStartup(wVersionRequested,&wsaData) == 0)   
	{   
		if(gethostname(name,sizeof(name)) == 0)   
		{   
			if((hostinfo = gethostbyname(name)) != NULL)   
			{   
				strIPAddr = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
			}   
		}   
		WSACleanup();
	}     
	if (!strIPAddr.IsEmpty())
	{
		return TRUE;
	}
	return FALSE;
}


/*************************************************
Function:    	CheckStrLegality
Description:	�鿴�����Ƿ�Ϸ�(% ����')
Input:          CString csString,�������ַ���
int nMaxLen,·����󳤶�
Output:      	
Return:			����ö������ CHECKPATH_SUCESS�ɹ� ����ʧ��
add by yudan at 2010-08-20 15:25
*************************************************/
ENUM_CHECKPATH_LEAGL AppFun_CheckStrLegality(CString csString, int nMaxLen)
{
	csString.Trim();

	if (0 == _tcscmp(csString, L""))
	{
		return CHECKPATH_EMPTY;
	}

	if (csString.Find(_T("%"))>=0 || csString.Find(_T("'"))>=0 || csString.Find(_T("��"))>=0)
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
        if (NULL != pdwError)
        {
            *pdwError = dwError;
        }

		return 0;
	}

	lpWideCharStr = (LPWSTR)::malloc(nRequired * sizeof(WCHAR)/* + 1*/);
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

		if (NULL != pdwError)
		{
			*pdwError = dwError;
		} 
		return 0;
	}

	lpMultiByteStr = (LPSTR)::malloc(nRequired * sizeof(CHAR)/* + 1*/);
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

//////////////////////////////////////////////////////////////////////////


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
		//WS_POPUP |TTS_ALWAYSTIP | TTS_BALLOON,//������
		WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,//��ͨ������
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

/** @fn SaveBmpTofile
*   @brief ����bmp���ݵ��ļ���
*   @param[in]  hbmp: bmp�ڴ�����
*   @param[in]  path: ���浽��ַ
*   @return TRUE�ɹ�
*/
BOOL SaveBmpTofile(HBITMAP hbmp, CString path) 
{
	//����˵��: hbmp :�豣���ͼ��ľ��         path :����·��    
	//�����ļ�ͷ�ṹ 
	BITMAPFILEHEADER fileHead;                                     
	int fileHeadLen = sizeof( BITMAPFILEHEADER );
	//����ͼ����Ϣ�ṹ
	BITMAPINFOHEADER bmpHead;
	int bmpHeadLen =sizeof( BITMAPINFOHEADER ); 
	BITMAP bmpObj; GetObject( hbmp, sizeof(BITMAP), &bmpObj );
	DWORD fileSizeInByte;               
	//�ļ��ܵ��ֽڴ�С
	//��ȡϵͳ��ɫ���,��ÿ�������ö���λ��ʾ
	DWORD PixelSizeInBit;              
	CDC srcDC;                      
	//ϵͳ��Ļ�豸������        
	srcDC.CreateDC( _T("DISPLAY"), NULL, NULL, NULL); 
	PixelSizeInBit=srcDC.GetDeviceCaps( BITSPIXEL ) * srcDC.GetDeviceCaps( PLANES ); 
	fileSizeInByte = fileHeadLen + bmpHeadLen + bmpObj.bmWidth*bmpObj.bmHeight*PixelSizeInBit/8;
	//��ʼ���ļ�ͷ�ṹ     
	fileHead.bfOffBits = fileHeadLen + bmpHeadLen;
	fileHead.bfReserved1=0; 
	fileHead.bfReserved2=0; 
	fileHead.bfSize = fileSizeInByte;
	fileHead.bfType = 0x4D42; 
	///��ʼͼ����Ϣ�ṹ 
	bmpHead.biBitCount = (WORD)PixelSizeInBit;
	bmpHead.biCompression = BI_RGB; 
	bmpHead.biPlanes = 1; 
	bmpHead.biHeight = bmpObj.bmHeight; 
	bmpHead.biWidth = bmpObj.bmWidth; 
	bmpHead.biSize = bmpHeadLen;  
    bmpHead.biSizeImage = bmpHead.biWidth * bmpHead.biHeight * bmpHead.biBitCount;
	//Ϊ�ļ�����ռ�
	PBYTE pFile=new byte[ fileSizeInByte ];
	memset( pFile, 0, fileSizeInByte ); 
	//����ļ�ͷ�� 
	memcpy( pFile, (PBYTE)&fileHead, fileHeadLen);    
	//����ļ���Ϣͷ���ṹ
	memcpy( pFile+fileHeadLen, (PBYTE)&bmpHead, bmpHeadLen);     
	//������ز���
	GetDIBits( srcDC.m_hDC, hbmp, 0, bmpObj.bmHeight, pFile+fileHeadLen+bmpHeadLen, (LPBITMAPINFO)(pFile+fileHeadLen), DIB_RGB_COLORS);  
	//���ļ���д������
	HANDLE hFile; 
	hFile=CreateFile( path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if( hFile==INVALID_HANDLE_VALUE ) 
	{       
		return FALSE;
	}   
	DWORD nByteTransfered; 
	WriteFile( hFile, pFile, fileSizeInByte, &nByteTransfered, NULL); 
	CloseHandle( hFile );
	//����
	delete []pFile; 
	srcDC.DeleteDC(); 

	return TRUE;
}

/**   @fn      void WaitForWindowThread(HANDLE hThread)
 *    @brief   �ȴ������߳̽���.
 *    @param   hThread:�߳̾��.
 *    @param   
 *    @return  
 */
void WaitForWindowThread(HANDLE hThread)
{	
	while(WAIT_OBJECT_0 != MsgWaitForMultipleObjects(1, &hThread, FALSE, INFINITE, QS_ALLINPUT)) 
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);   
			DispatchMessage(&msg);   
		}
	}
}

/**   @fn          CheckFilePathSpace
 *    @brief   	   �����̿ռ�.
 *    @param[in]   strFilePath:�ļ�·��.
 *    @param[in]   uMinSpace:��С���̿ռ�.
 *    @return      
 */
BOOL CheckFilePathSpace(CString strFilePath, ULONGLONG uMinSpace)
{
	strFilePath.Trim();
	if(strFilePath.IsEmpty())
	{
		return FALSE;
	}

	CString strDisk = strFilePath.Left(1);
	strDisk += _T(":\\");

	ULARGE_INTEGER FreeSpace,CallerSpace,TotalSpace;

	if(GetDiskFreeSpaceEx(strDisk,&CallerSpace,&TotalSpace,&FreeSpace))
	{
		if(FreeSpace.QuadPart > uMinSpace)
		{
			return TRUE;
		}
	}

	return FALSE;
}

CString GetFileSize(DWORD dwFileSize)
{
	TRACE("@z filesize:%ld", dwFileSize);
	CString strFileSize = _T("");

	if (dwFileSize <= 0)
	{
		strFileSize = _T("0");
		return strFileSize;
	}

	// MB.
	float fMB = dwFileSize / ((float)1024 * (float)1024);
	if (fMB > 1)
	{
		strFileSize.Format(_T("%.02fMB"), fMB);
		return strFileSize;
	}

	// KB.
	float fKB = dwFileSize / ((float)1024);
	if (fKB > 1)
	{
		strFileSize.Format(_T("%.02fKB"), fKB);
		return strFileSize;
	}

	strFileSize.Format(_T("%ldB"), dwFileSize);
	return strFileSize;

}

BOOL MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize)
{
	// Get the required size of the buffer that receives the Unicode
	// string.
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, NULL, 0);
	if(dwSize < dwMinSize)
	{
		return FALSE;
	}

	// Convert headers from ASCII to Unicode.
	MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, lpwszStr, dwMinSize);  

	return TRUE;
}

BOOL WCharToMByte(LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize)
{
	memset(lpszStr,0,dwSize * sizeof(char));
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,NULL,0,NULL,FALSE);
	if(dwSize < dwMinSize)
	{
		return FALSE;
	}

	WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,lpszStr,dwSize,NULL,FALSE);

	return TRUE;
}

void GetErrorMsg(DWORD dwError, CString& strMsg)
{
	switch (dwError)
	{
	case PB_UNSUPPORT_OPERATION:
		{	
			strMsg = _T("��֧�ָò���");
		}
		break;

	default:
		break;
	}
}