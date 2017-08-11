#include "StdAfx.h"
#include "RenderMgr.h"
#include "fuilib.h"

#define CHECK_DC(p) {if(NULL==p)return;}

// 简单单例
CRenderMgr* CRenderMgr::GetInstance()
{
    static CRenderMgr _Instance;

    return &_Instance;
}

CRenderMgr::CRenderMgr(void)
{
	// 宋体 12 常规
    LOGFONT lf1 = {0};
    _tcscpy_s(lf1.lfFaceName, _T("宋体"));
    lf1.lfHeight = 12;
    lf1.lfWeight = FW_NORMAL;
    AddFont(SONG_12_N, lf1);

    // 宋体 12 中粗
    _tcscpy_s(lf1.lfFaceName, _T("宋体"));
    lf1.lfHeight = 12;
    lf1.lfWeight = FW_SEMIBOLD;
    AddFont(SONG_12_SB, lf1);

    // 宋体 16 粗
    _tcscpy_s(lf1.lfFaceName, _T("宋体"));
    lf1.lfHeight = 16;
    lf1.lfWeight = FW_BOLD;
    AddFont(SONG_16_B, lf1);

	// Arial 15 常规
	_tcscpy_s(lf1.lfFaceName, _T("Arial"));
	lf1.lfHeight = 15;
	lf1.lfWeight = FW_NORMAL;
	AddFont(ARIAL_15_N, lf1);   

	// 雅黑 18 常规
	_tcscpy_s(lf1.lfFaceName, _T("微软雅黑"));
	lf1.lfHeight = 18;
	lf1.lfWeight = FW_NORMAL;
	AddFont(YAHEI_18_N, lf1);


	// 雅黑 18 常规
	_tcscpy_s(lf1.lfFaceName, _T("微软雅黑"));
	lf1.lfHeight = 18;
	lf1.lfWeight = FW_BOLD;
	AddFont(YAHEI_18_B, lf1);


	// 雅黑 18 中粗
	_tcscpy_s(lf1.lfFaceName, _T("微软雅黑"));
	lf1.lfHeight = 18;
	lf1.lfWeight = FW_SEMIBOLD;
	AddFont(YAHEI_18_SB, lf1);

	// 雅黑 12 常规
	_tcscpy_s(lf1.lfFaceName, _T("微软雅黑"));
	lf1.lfHeight = 20;
	lf1.lfWeight = FW_NORMAL;
	AddFont(YAHEI_20_N, lf1);

    // 雅黑 20 中粗
    _tcscpy_s(lf1.lfFaceName, _T("微软雅黑"));
    lf1.lfHeight = 20;
    lf1.lfWeight = FW_SEMIBOLD;
    AddFont(YAHEI_20_SB, lf1);

	// 雅黑 20 粗
	_tcscpy_s(lf1.lfFaceName, _T("微软雅黑"));
	lf1.lfHeight = 20;
	lf1.lfWeight = FW_BOLD;
	AddFont(YAHEI_20_B, lf1);


	// 雅黑 26 中粗
	_tcscpy_s(lf1.lfFaceName, _T("微软雅黑"));
	lf1.lfHeight = 26;
	lf1.lfWeight = FW_SEMIBOLD;
	AddFont(YAHEI_26_SB, lf1);

	// 雅黑 23 粗
	_tcscpy_s(lf1.lfFaceName, _T("微软雅黑"));
	lf1.lfHeight = 24;
	lf1.lfWeight = FW_BOLD;
	AddFont(YAHEI_24_B, lf1);

	// 雅黑 24 中粗
	_tcscpy_s(lf1.lfFaceName, _T("微软雅黑"));
	lf1.lfHeight = 24;
	lf1.lfWeight = FW_SEMIBOLD;
	AddFont(YAHEI_24_SB, lf1);
}

CRenderMgr::~CRenderMgr(void)
{
    for (std::map<CString, CFont*>::iterator itr = m_mapResFont.begin();
        itr != m_mapResFont.end(); ++itr)
    {
        CFont* pFont = itr->second;
        if (pFont->m_hObject)
        {
            pFont->DeleteObject();
        }
        delete pFont;
    }
    m_mapResFont.clear();
}

void CRenderMgr::AddFont(const CString& strName, LOGFONT lfFont)
{
    if (m_mapResFont.find(strName) != m_mapResFont.end())
    {
        return ;
    }

    CFont *pFont = new CFont;
    if (pFont->CreateFontIndirect(&lfFont))
    {
        m_mapResFont[strName] = pFont;
    }

}

CFont* CRenderMgr::GetFont(const CString& strName)
{
    if (m_mapResFont.find(strName) != m_mapResFont.end())
    {
        return m_mapResFont[strName];
    }
    return NULL;
}


void CRenderMgr::DrawColor(CDC *pDC, const RECT& rc, COLORREF color)
{
    CHECK_DC(pDC);

    pDC->SetBkColor(color);
    pDC->ExtTextOut(0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
}

void CRenderMgr::DrawLine(CDC *pDC, const RECT& rc, int nWidth, COLORREF dwPenColor,int nStyle /*= PS_SOLID*/)
{
    CHECK_DC(pDC);

    CPen pen;
    pen.CreatePen(nStyle, nWidth, dwPenColor);

    CPen *pOldPen = pDC->SelectObject(&pen);
    pDC->MoveTo(rc.left, rc.top);
    pDC->LineTo(rc.right, rc.bottom);
    pDC->SelectObject(pOldPen);
    pen.DeleteObject();
}

void CRenderMgr::DrawRect(CDC *pDC, const RECT& rc, int nWidth, COLORREF dwPenColor)
{
    CHECK_DC(pDC);

    CPen pen;
    pen.CreatePen(PS_SOLID | PS_INSIDEFRAME, nWidth, dwPenColor);
    CPen *pOldPen = pDC->SelectObject(&pen);
    pDC->SelectObject(::GetStockObject(HOLLOW_BRUSH));
    pDC->Rectangle(rc.left, rc.top, rc.right, rc.bottom);
    pDC->SelectObject(pOldPen);
    pen.DeleteObject();
}

void CRenderMgr::DrawImageX(CDC *pDC, Image* pImage, REAL dstx, REAL dsty, REAL dstw, 
                           REAL dsth, REAL srcx, REAL srcy, REAL srcw, REAL srch)
{
    //构造位置
    RectF rcDst(dstx, dsty, dstw, dsth);

    DrawImageX(pDC, pImage, rcDst, srcx, srcy, srcw, srch);
}

void CRenderMgr::DrawImageX(CDC *pDC, Image* pImage, const RectF& rcf, REAL srcx, REAL srcy, REAL srcw, REAL srch, const int nRenderMode)
{
    CHECK_DC(pDC);

    if (NULL == pImage)
    {
        return ;
    }

	if (nRenderMode == RENDER_GDIPLUS)
	{
		// 创建屏幕
		Graphics graphics(pDC->GetSafeHdc());

		// 绘图属性
		ImageAttributes imgAttr;
		imgAttr.SetWrapMode(WrapModeTileFlipXY);

		//绘画图像
		graphics.DrawImage(pImage, rcf, srcx, srcy, srcw, srch, UnitPixel, &imgAttr);
	}
	else if (nRenderMode == RENDER_GDI)
	{	
		CBitmap *pCBitmap = CResMgr::GetInstance()->GetResCBitmap(pImage);
		if (NULL == pCBitmap)
		{
			return ;
		}
		
		CDC	dc;
		dc.CreateCompatibleDC(NULL);
		dc.SelectObject(pCBitmap);

		pDC->SetStretchBltMode(COLORONCOLOR);
		pDC->StretchBlt(rcf.X, rcf.Y, rcf.Width, rcf.Height, &dc, srcx, srcy, srcw, srch, SRCCOPY);

		dc.DeleteDC();
	}
	else
	{

	}
}

void CRenderMgr::DrawImageX(CDC *pDC, Image* pImage, const CRect& rcDst, int nMargin)
{
    if (NULL == pImage)
    {
        return;
    }

    std::vector<int> vecMargin;
    SetMargin(vecMargin, nMargin);

    CRect rcImg(0, 0, pImage->GetWidth(), pImage->GetHeight());

    // 切割图片
    std::vector<RectF> vecfImg;
    SepRect(rcImg, vecfImg, vecMargin);

    // 切割矩形
    std::vector<RectF> vecfDst;
    SepRect((CRect&)rcDst, vecfDst, vecMargin);

    for (size_t i = 0; i < vecfImg.size(); ++i)
    {
        RenderEngine->DrawImageX(pDC, pImage, vecfDst[i], vecfImg[i].X, vecfImg[i].Y, 
            vecfImg[i].Width, vecfImg[i].Height);
    }
}

void CRenderMgr::DrawTextX(CDC *pDC, const CString& strText, LPRECT lpRect, COLORREF clrText, UINT uFormat)
{
    CHECK_DC(pDC);

    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(clrText);
    pDC->DrawText(strText, lpRect, uFormat);
}

void CRenderMgr::DrawTextX(CDC *pDC, const CString& strText, LPRECT lpRect, COLORREF clrText, int nRowDis, UINT uFormat)
{
    if( strText.IsEmpty())
    {
        return;
    }

    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(clrText);

    CString strTmp = strText;
    TCHAR* pText = strTmp.GetBuffer();
    int nCount = strTmp.GetLength();
    CRect rtChar;
    CSize size = pDC->GetTextExtent(pText + 0, 1);
    int nRowHeight = size.cx + nRowDis;
    rtChar.top = lpRect->top;
    rtChar.left = lpRect->left;
    rtChar.bottom	= rtChar.top + nRowDis + size.cy;
    rtChar.right	= rtChar.left + size.cx;	
    CString strChar;
    for (int nCharIndex = 0; nCharIndex < nCount; nCharIndex++)
    {
        if( rtChar.right > lpRect->right )
        {
            rtChar.top = rtChar.bottom;
            rtChar.bottom += nRowHeight;
            size = pDC->GetTextExtent(pText + nCharIndex, 1);
            rtChar.left = lpRect->left;
            rtChar.right = rtChar.left + size.cx;
            if( rtChar.bottom > lpRect->bottom )
                break;
        }
        strChar = pText[nCharIndex];
        pDC->DrawText(strChar, rtChar, uFormat);
        size = pDC->GetTextExtent(pText + nCharIndex + 1, 1);
        rtChar.left = rtChar.right;
        rtChar.right += size.cx;
    }
}

BOOL CRenderMgr::DrawTransparentBmp(HDC hdcDest, int nXoriginDest, int nYoriginDest , int nWidthDest, int nHeightDest
									, HDC hdcSrc, int nXoriginSrc, int nYoriginSrc , int nWidthSrc, int nHeightSrc, UINT crTransparent)
{
	if (nWidthDest < 1 || nWidthSrc < 1 || nHeightDest < 1 || nHeightSrc < 1)
	{
		return FALSE;
	}

	HDC hMaskDC, hImageDC;

	HBITMAP hMaskBitmap,hOldMaskBitmap;
	HBITMAP hImageBitmap, hOldImageBitmap;

	//创建mask DC 和临时存源文件的 DC 
	hMaskDC = CreateCompatibleDC(hdcDest);
	hImageDC = CreateCompatibleDC(hdcDest);

	//创建mask 位图和image 兼容位图 
	hMaskBitmap = CreateBitmap(nWidthSrc, nHeightSrc, 1, 1, NULL);				//单色位图 
	hImageBitmap = CreateCompatibleBitmap(hdcDest, nWidthSrc, nHeightSrc);		//彩色位图 

	//将位图选进DC中
	hOldImageBitmap = (HBITMAP) SelectObject(hImageDC, hImageBitmap);
	hOldMaskBitmap = (HBITMAP) SelectObject(hMaskDC, hMaskBitmap);

	//将src位图拷入临时的image DC中
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
	{
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXoriginSrc, nYoriginSrc, SRCCOPY);
	}
	else
	{
		StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXoriginSrc, nYoriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);
	}

	SetBkColor(hImageDC, crTransparent);	
	//此步生成掩码位图(mask bitmap),注意整个位图中白为1，黑为0
	//整个mask 位图中透明色的地方是白色，其他为黑色 
	BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, nXoriginSrc, nYoriginSrc, SRCCOPY);
	COLORREF crOldBack = SetBkColor(hdcDest, RGB(255, 255, 255));
	COLORREF crOldText = SetTextColor(hdcDest, RGB(0, 0, 0));

	BitBlt(hdcDest, 0, 0, nWidthDest, nHeightDest, hImageDC, nXoriginSrc, nYoriginSrc, SRCINVERT);
	BitBlt(hdcDest, 0, 0, nWidthDest, nHeightDest, hMaskDC, nXoriginSrc, nYoriginSrc, SRCAND);
	BitBlt(hdcDest, 0, 0, nWidthDest, nHeightDest, hImageDC, nXoriginSrc, nYoriginSrc, SRCINVERT);

	SetBkColor(hdcDest, crOldBack);
	SetTextColor(hdcDest, crOldText);

	DeleteObject(SelectObject(hMaskDC, hOldMaskBitmap));
	DeleteDC(hMaskDC);
	DeleteObject(SelectObject(hImageDC, hOldImageBitmap));
	DeleteDC(hImageDC);
	
	return TRUE;
}
