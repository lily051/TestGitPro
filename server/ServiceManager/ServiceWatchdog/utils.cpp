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

#include "stdafx.h"
#include "utils.h"
#include <afxpriv2.h>

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
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(uID), "PNG");
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(uID), "PNG");
	
	if (hRsrc != NULL)
	{
		// load it
		hResource = LoadResource(hInst, hRsrc);
		
		if (NULL == hResource)
		{
			return NULL;
		}
		// lock it
		lpResource = LockResource(hResource);
		
		ASSERT(lpResource != NULL);
	}

    // 无资源
    if (NULL == lpResource)
    {
        if (hResource != NULL)
        {
            UnlockResource(hResource);
            FreeResource(hResource);
        }
        return NULL;
    }

	DWORD dwlen = SizeofResource(hInst, hRsrc); 

	//Allocate   global   memory   on   which   to   create   stream 
	HGLOBAL hGlobal = GlobalAlloc(GMEM_FIXED, dwlen); 
	BYTE * pMem = (BYTE*)GlobalLock(hGlobal); 

	memcpy(pMem, lpResource, dwlen); 

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
		//hBuffer = NULL;
	}

	return pBitmap;
}


#define WINDOW_ROUND_SIZE   7

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

/**************************************************************************
*   Function:		TestRemoteHost
*   Description:    test whether the remote host is online
*   Input:          const char *szIP, remote host IP
*                   USHORT nPort, remote host port
*   Output:         (null)
*   Return:			BOOL, return TRUE if online
**************************************************************************/
BOOL TestRemoteHost(const char *szIP, USHORT nPort)
{
	class CInitSocket{
	public:
		CInitSocket():m_bInited(false){}
		bool Init()
		{
			WSADATA wsaData;
			int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
			if( iResult == NO_ERROR )
			{
				m_bInited = true;
			}
			return m_bInited;
		}
		~CInitSocket()
		{
			if (!m_bInited)
			{
				WSACleanup();
			}

		}
	private:
		bool m_bInited;
	};
	CInitSocket initSock;
	initSock.Init();

	timeval timeout = {5,0};
	fd_set fdset;
	bool bConnected = FALSE;
	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);
	int iRet = -1;
	int iMode=1;//非阻塞模式
	SOCKET iSockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( iSockfd == INVALID_SOCKET )
	{
		/*int i = */WSAGetLastError();
		return FALSE;
	}
	if(setsockopt(iSockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&bOptVal, bOptLen) == SOCKET_ERROR)
	{
		closesocket(iSockfd);
		return FALSE;
	}
	linger m_sLinger;
	m_sLinger.l_onoff  = 1;
	m_sLinger.l_linger = 0;//m_sLinger.l_linger=5;
	setsockopt(iSockfd, SOL_SOCKET, SO_LINGER, (char *)&m_sLinger, sizeof(linger));//防止出现time_wait状态
	struct sockaddr_in remoteaddr;
	remoteaddr.sin_port = htons((u_short)nPort);
	remoteaddr.sin_addr.s_addr = inet_addr(szIP);
	remoteaddr.sin_family = AF_INET;
	//////////////////////////////////////////////////////////
	iRet = ioctlsocket(iSockfd, FIONBIO, (u_long FAR*) &iMode);//lint !e569
	if(iRet == SOCKET_ERROR)
	{
		closesocket(iSockfd);
		return FALSE;
	}
	iRet = connect(iSockfd,(LPSOCKADDR)&remoteaddr, sizeof(remoteaddr));
	if(iRet < 0)
	{
		FD_ZERO(&fdset);
		FD_SET((unsigned)iSockfd, &fdset);
		int iSelect = select((int)iSockfd + 1, 0, &fdset, 0, &timeout);//等待连接返回
		if(iSelect > 0)
		{
			int iErr = -1;
			int iLen = sizeof(iErr);
			getsockopt(iSockfd, SOL_SOCKET, SO_ERROR, (char*)&iErr, &iLen);
			if(iErr == 0)
			{
				bConnected = TRUE;//连接成功
			}
		}
	}
	closesocket(iSockfd);
	return bConnected;
}
