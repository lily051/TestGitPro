#include "stdafx.h"
#include "../StdAfx.h"
//lint -library

#include "ResourceLoader.h"

CResourceLoader::CResourceLoader(void)
{
	m_pImage = new CImage;
}

CResourceLoader::~CResourceLoader(void)
{
	delete m_pImage;
}

BOOL CResourceLoader::Load(UINT uID)
{
	HINSTANCE hinstRes = AfxFindResourceHandle(MAKEINTRESOURCE(uID), L"PNG");

	HRSRC hRsrc = ::FindResource(hinstRes,MAKEINTRESOURCE(uID), L"PNG");
	if (hRsrc == NULL)
	{
		return FALSE;
	}

	HGLOBAL hGlobal = LoadResource (hinstRes, hRsrc);
	if (hGlobal == NULL)
	{
		return FALSE;
	}

	LPVOID lpBuffer = ::LockResource(hGlobal);
	if (lpBuffer == NULL)
	{
		FreeResource(hGlobal);
		return FALSE;
	}

	BOOL bRes = LoadFromBuffer ((LPBYTE) lpBuffer, (UINT) ::SizeofResource (hinstRes, hRsrc));

	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return bRes;
}

BOOL CResourceLoader::LoadFromBuffer (LPBYTE lpBuffer, UINT uiSize)
{
	ASSERT(lpBuffer != NULL);

	HGLOBAL hRes = ::GlobalAlloc (GMEM_MOVEABLE, uiSize);
	if (hRes == NULL)
	{
		return FALSE;
	}

	IStream* pStream = NULL;
	LPVOID lpResBuffer = ::GlobalLock (hRes);
	ASSERT (lpResBuffer != NULL);

	memcpy (lpResBuffer, lpBuffer, uiSize);

	HRESULT hResult = ::CreateStreamOnHGlobal (hRes, FALSE, &pStream);

	if (hResult != S_OK)
	{
		return FALSE;
	}

	ENSURE(m_pImage != NULL);

	m_pImage->Load(pStream);
	pStream->Release();

	BOOL bRes = Attach(m_pImage->Detach());

	return bRes;
}

/*************************************************
Function:    	LoadPNG
Description:	加载PNG
Input:			uID资源ID
Output:      	
Return:			位图或者NULL
*************************************************/
HBITMAP CResourceLoader::LoadPNG(UINT uID)
{
	if (this->Load(uID))
	{
		return (HBITMAP)this->Detach();
	}
	return NULL;
}

/*************************************************
Function:    	LoadPNGToImage
Description:	加载PNG变为Image*
Input:			uID资源ID
Output:      	
Return:			
*************************************************/
Image * CResourceLoader::LoadPNGToImage(UINT nID)
{
	HINSTANCE hinstRes = AfxFindResourceHandle(MAKEINTRESOURCE(nID), L"PNG");
	//AfxSetResourceHandle(hinstRes); 

	Image *pRetImg = NULL;

	HGLOBAL        hGlobal = NULL;
	HRSRC        hSource = NULL;
	LPVOID        lpVoid  = NULL;
	int            nSize   = 0;

	hSource = FindResource(hinstRes, MAKEINTRESOURCE(nID), L"PNG");

	if(hSource == NULL)
	{
		goto Exit;
	}

	hGlobal = LoadResource(hinstRes, hSource);
	if(hGlobal == NULL)
	{    
		goto Exit;
	}

	lpVoid = LockResource(hGlobal);
	if(lpVoid == NULL)
	{
		goto Exit;
	}

	nSize = (UINT)SizeofResource(AfxGetResourceHandle(), hSource);

	HGLOBAL hGlobal2 = GlobalAlloc(GMEM_MOVEABLE, nSize);
	if(hGlobal2 == NULL)
	{
		goto Exit;
	}

	void* pData = GlobalLock(hGlobal2);
	memcpy(pData, (void *)hGlobal, nSize);
	GlobalUnlock(hGlobal2);

	IStream* pStream = NULL;

	if(CreateStreamOnHGlobal(hGlobal2, TRUE, &pStream) == S_OK)
	{			
		pRetImg = Image::FromStream(pStream, FALSE);
		pStream->Release();
	}

	FreeResource(hGlobal2); 			

Exit:
	if(hGlobal)
	{
		UnlockResource(hGlobal);   
		FreeResource(hGlobal); 
	}

	HINSTANCE hInstance = ::AfxGetInstanceHandle();
	//AfxSetResourceHandle(hInstance);
	return pRetImg;
}

HBITMAP CResourceLoader::ResourceLoad_PNG_EX_HBMP(UINT nID,Color crBak)
{
	HINSTANCE hinstRes = AfxFindResourceHandle(MAKEINTRESOURCE(nID), L"PNG");

	//AfxSetResourceHandle(hinstRes);

	Bitmap * pBitmap = NULL;
	HBITMAP hBitmap = NULL;

	HRSRC hResource = ::FindResource(hinstRes, MAKEINTRESOURCE(nID), L"PNG");   //   type 
	if (!hResource)
	{
		goto Exit;
	}

	DWORD imageSize = ::SizeofResource(hinstRes, hResource);
	if (!imageSize)
	{
		goto Exit;
	}

	const void* pResourceData = ::LockResource(::LoadResource(hinstRes, hResource));
	if (!pResourceData)
	{
		goto Exit;
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
						goto Exit;
					}
				}
			}
			::GlobalUnlock(hBuffer);
		}

		::GlobalFree(hBuffer);
		hBuffer = NULL;
	}

Exit:
	HINSTANCE hInstance = ::AfxGetInstanceHandle();
	//AfxSetResourceHandle(hInstance);

	if(pBitmap)
	{
		pBitmap->GetHBITMAP(crBak, &hBitmap);
		delete pBitmap;
	}

	return hBitmap;
}

HCURSOR CResourceLoader::ResourceLoad_Cursor(UINT nID)
{
	HINSTANCE hinstRes = AfxFindResourceHandle(MAKEINTRESOURCE(nID), L"Cursor");

	//AfxSetResourceHandle(hinstRes); 

	HCURSOR hCursor =  ::LoadCursor(hinstRes,MAKEINTRESOURCE(nID));  

	HINSTANCE hInstance = ::AfxGetInstanceHandle();
	//AfxSetResourceHandle(hInstance);
	return hCursor;

	return HCURSOR();
}

HICON CResourceLoader::LoadIcon(UINT nID)
{
	HINSTANCE hinstRes = AfxFindResourceHandle(MAKEINTRESOURCE(nID), L"Ico");
	//AfxSetResourceHandle(hinstRes);   

	HICON   hIcon   =   ::LoadIcon(hinstRes,MAKEINTRESOURCE(nID)); 
	HINSTANCE hInstance = ::AfxGetInstanceHandle();
	//AfxSetResourceHandle(hInstance);

	return hIcon;
}
HBITMAP CResourceLoader::ResourceLoad_Bmp(UINT nID)
{
	HBITMAP hBmp = NULL;

	HINSTANCE hinstRes = AfxFindResourceHandle(MAKEINTRESOURCE(nID), L"Bitmap");

	//AfxSetResourceHandle(hinstRes);

	hBmp =  ::LoadBitmap(hinstRes,MAKEINTRESOURCE(nID)); 


	HINSTANCE hInstance = ::AfxGetInstanceHandle();
	//AfxSetResourceHandle(hInstance);

	if(hBmp == NULL)
	{
		DWORD dwLastError = GetLastError();
	}

	return hBmp;
}

COLORREF CResourceLoader::ResourceLoad_Color(UINT nID)
{
	// 在颜色表中查找.
	std::map<UINT, COLORREF>::iterator ite = m_mColor.find(nID);
	// 如果查找到则直接返回颜色.
	if (ite != m_mColor.end())
	{
		return (*ite).second;
	}

	COLORREF clr = RGB(0, 0, 0);
	CString strClr;
	if (0 == strClr.LoadString(nID))
	{
		return clr;
	}

	CString strTemp = _T("RGB(");

	int nIndex = strClr.Find(strTemp);
	if (-1 == nIndex)
	{
		return clr;
	}

	int r = 0;
	int g = 0;
	int b = 0;

	strClr = strClr.Right(strClr.GetLength() - strTemp.GetLength());
	nIndex = strClr.Find(',');
	if (-1 == nIndex)
	{
		return clr;
	}

	strTemp = strClr.Left(nIndex);
	strTemp.Trim();
	r = _wtoi(strTemp);
	strClr = strClr.Right(strClr.GetLength() - nIndex - 1);

	nIndex = strClr.Find(',');
	if (-1 == nIndex)
	{
		return clr;
	}
	strTemp = strClr.Left(nIndex);
	strTemp.Trim();
	g = _wtoi(strTemp);
	strClr = strClr.Right(strClr.GetLength() - nIndex - 1);

	nIndex = strClr.Find(')');
	if (-1 == nIndex)
	{
		return clr;
	}
	strTemp = strClr.Left(nIndex);
	strTemp.Trim();
	b = _wtoi(strTemp);
	
	clr = RGB(r, g, b);

	// 将颜色插入表中.
	m_mColor.insert(std::pair<UINT, COLORREF>(nID, clr));
	
	return clr;
}