//lint -library
#include "StdAfx.h"
#include "ResourceLoader.h"
#include "AutoExchangeModuleState.h"
#include "../PreviewS20Dll.h"

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
    AutoExchangeModuleState AEMS(theApp.m_hResInstance);

	HINSTANCE hinstRes = AfxFindResourceHandle(MAKEINTRESOURCE(uID),_T("PNG"));

	HRSRC hRsrc = ::FindResource(hinstRes,MAKEINTRESOURCE(uID),_T("PNG"));
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
    AutoExchangeModuleState AEMS(theApp.m_hResInstance);

	HINSTANCE hinstRes = AfxFindResourceHandle(MAKEINTRESOURCE(nID),_T("PNG"));

	Image *pRetImg = NULL;

	HGLOBAL        hGlobal = NULL;
	HRSRC        hSource = NULL;
	LPVOID        lpVoid  = NULL;
	int            nSize   = 0;

	hSource = FindResource(hinstRes, MAKEINTRESOURCE(nID), _T("PNG"));

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

	return pRetImg;
}

HBITMAP CResourceLoader::ResourceLoad_PNG_EX_HBMP(UINT nID,Color crBak)
{
    AutoExchangeModuleState AEMS(theApp.m_hResInstance);

	HINSTANCE hinstRes = AfxFindResourceHandle(MAKEINTRESOURCE(nID),_T("PNG"));

	Bitmap * pBitmap = NULL;
	HBITMAP hBitmap = NULL;

	HRSRC hResource = ::FindResource(hinstRes, MAKEINTRESOURCE(nID), _T("PNG"));   //   type 
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

	if(pBitmap)
	{
		pBitmap->GetHBITMAP(crBak, &hBitmap);
		delete pBitmap;
	}

	return hBitmap;
}

HCURSOR CResourceLoader::ResourceLoad_Cursor(UINT nID)
{
    AutoExchangeModuleState AEMS(theApp.m_hResInstance);

	HINSTANCE hinstRes = AfxFindResourceHandle(MAKEINTRESOURCE(nID),_T("Cursor"));

	HCURSOR hCursor =  ::LoadCursor(hinstRes,MAKEINTRESOURCE(nID));  

	return hCursor;

	return HCURSOR();
}

HICON CResourceLoader::LoadIcon(UINT nID)
{
    AutoExchangeModuleState AEMS(theApp.m_hResInstance);

	HINSTANCE hinstRes = AfxFindResourceHandle(MAKEINTRESOURCE(nID),_T("Ico"));

	HICON   hIcon   =   ::LoadIcon(hinstRes,MAKEINTRESOURCE(nID)); 

	return hIcon;
}
HBITMAP CResourceLoader::ResourceLoad_Bmp(UINT nID)
{
    AutoExchangeModuleState AEMS(theApp.m_hResInstance);

	HBITMAP hBmp = NULL;

	HINSTANCE hinstRes = AfxFindResourceHandle(MAKEINTRESOURCE(nID),_T("Bitmap"));

	hBmp =  ::LoadBitmap(hinstRes,MAKEINTRESOURCE(nID)); 

	if(hBmp == NULL)
	{
		DWORD dwLastError = GetLastError();
	}

	return hBmp;
}
