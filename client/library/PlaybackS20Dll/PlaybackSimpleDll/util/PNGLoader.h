#pragma once
//lint -library
#include "afxwin.h"

class CPNGLoader :
	public CBitmap
{
public:
	CPNGLoader(void);
	~CPNGLoader(void);

	BOOL Load(UINT uID);
	//ljg 2010/03/08 add [[
	//º”‘ÿPNG
	HBITMAP LoadPNG(UINT uID);
	Image * LoadPNGToImage(UINT nID);
	HBITMAP ResourceLoad_PNG_EX_HBMP(UINT nID,Color crBak);
	//]]end
private:
	BOOL LoadFromBuffer (LPBYTE lpBuffer, UINT uiSize);
	class CImage * m_pImage;
};
