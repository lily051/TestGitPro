#pragma once
#include <atlimage.h>

class CResourceLoader :
	public CBitmap
{
public:
	CResourceLoader(void);
	~CResourceLoader(void);

	BOOL Load(UINT uID);
	//ljg 2010/03/08 add [[
	//加载PNG
	HBITMAP LoadPNG(UINT uID);
	Image * LoadPNGToImage(UINT nID);
	HBITMAP ResourceLoad_PNG_EX_HBMP(UINT nID,Color crBak);
	HCURSOR ResourceLoad_Cursor(UINT nID);
	HBITMAP ResourceLoad_Bmp(UINT nID);

	HICON CResourceLoader::LoadIcon(UINT nID);

	//]]end
private:
	BOOL LoadFromBuffer (LPBYTE lpBuffer, UINT uiSize);
	class CImage * m_pImage;
};
extern CResourceLoader g_ResourceLoader;
#define RESOURCELOAD_PNG(id)               g_ResourceLoader.LoadPNG(id)
#define RESOURCELOAD_PNG_EX_HBMP(id,cr)    g_ResourceLoader.ResourceLoad_PNG_EX_HBMP(id,cr)
#define RESOURCELOAD_PNG_EX_IMG(id)        g_ResourceLoader.LoadPNGToImage(id)
#define RESOURCELOAD_CURSOR(id)            g_ResourceLoader.ResourceLoad_Cursor(id)
#define RESOURCELOAD_BMP(id)               g_ResourceLoader.ResourceLoad_Bmp(id)
#define FROM_PNG(id)   g_ResourceLoader.LoadPNG(id)//ljg add  电子地图使用
#define RESOURCELOAD_ICON(id)	           g_ResourceLoader.LoadIcon(id)