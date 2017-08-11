#pragma once
//lint -library

#include "afxwin.h"
#include "atlimage.h"
#include <map>

class CResourceLoader :
	public CBitmap
{
public:
	CResourceLoader(void);
	~CResourceLoader(void);

	BOOL Load(UINT uID);
	//ljg 2010/03/08 add [[
	//º”‘ÿPNG
	HBITMAP LoadPNG(UINT uID);
	Image * LoadPNGToImage(UINT nID);
	HBITMAP ResourceLoad_PNG_EX_HBMP(UINT nID,Color crBak = Color(0, 255, 255, 255));
	HCURSOR ResourceLoad_Cursor(UINT nID);
	HBITMAP ResourceLoad_Bmp(UINT nID);
	COLORREF ResourceLoad_Color(UINT nID);
	HICON CResourceLoader::LoadIcon(UINT nID);

	//]]end
private:
	BOOL LoadFromBuffer (LPBYTE lpBuffer, UINT uiSize);
	class CImage * m_pImage;
	std::map<UINT, COLORREF> m_mColor;
};
