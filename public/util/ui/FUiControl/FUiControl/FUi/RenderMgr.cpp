#include "StdAfx.h"
#include "RenderMgr.h"
#include "fuilib.h"

#define CHECK_DC(p) {if(NULL==p)return;}

// �򵥵���
CRenderMgr* CRenderMgr::GetInstance()
{
    static CRenderMgr _Instance;

    return &_Instance;
}

CRenderMgr::CRenderMgr(void)
{
	// ���� 12 ����
    LOGFONT lf1 = {0};
    _tcscpy_s(lf1.lfFaceName, _T("����"));
    lf1.lfHeight = 12;
    lf1.lfWeight = FW_NORMAL;
    AddFont(SONG_12_N, lf1);

    // ���� 12 �д�
    _tcscpy_s(lf1.lfFaceName, _T("����"));
    lf1.lfHeight = 12;
    lf1.lfWeight = FW_SEMIBOLD;
    AddFont(SONG_12_SB, lf1);

    // ���� 16 ��
    _tcscpy_s(lf1.lfFaceName, _T("����"));
    lf1.lfHeight = 16;
    lf1.lfWeight = FW_BOLD;
    AddFont(SONG_16_B, lf1);

	// Arial 15 ����
	_tcscpy_s(lf1.lfFaceName, _T("Arial"));
	lf1.lfHeight = 15;
	lf1.lfWeight = FW_NORMAL;
	AddFont(ARIAL_15_N, lf1);   

	// �ź� 18 ����
	_tcscpy_s(lf1.lfFaceName, _T("΢���ź�"));
	lf1.lfHeight = 18;
	lf1.lfWeight = FW_NORMAL;
	AddFont(YAHEI_18_N, lf1);


	// �ź� 18 ����
	_tcscpy_s(lf1.lfFaceName, _T("΢���ź�"));
	lf1.lfHeight = 18;
	lf1.lfWeight = FW_BOLD;
	AddFont(YAHEI_18_B, lf1);


	// �ź� 18 �д�
	_tcscpy_s(lf1.lfFaceName, _T("΢���ź�"));
	lf1.lfHeight = 18;
	lf1.lfWeight = FW_SEMIBOLD;
	AddFont(YAHEI_18_SB, lf1);

	// �ź� 12 ����
	_tcscpy_s(lf1.lfFaceName, _T("΢���ź�"));
	lf1.lfHeight = 20;
	lf1.lfWeight = FW_NORMAL;
	AddFont(YAHEI_20_N, lf1);

    // �ź� 20 �д�
    _tcscpy_s(lf1.lfFaceName, _T("΢���ź�"));
    lf1.lfHeight = 20;
    lf1.lfWeight = FW_SEMIBOLD;
    AddFont(YAHEI_20_SB, lf1);

	// �ź� 20 ��
	_tcscpy_s(lf1.lfFaceName, _T("΢���ź�"));
	lf1.lfHeight = 20;
	lf1.lfWeight = FW_BOLD;
	AddFont(YAHEI_20_B, lf1);


	// �ź� 26 �д�
	_tcscpy_s(lf1.lfFaceName, _T("΢���ź�"));
	lf1.lfHeight = 26;
	lf1.lfWeight = FW_SEMIBOLD;
	AddFont(YAHEI_26_SB, lf1);

	// �ź� 23 ��
	_tcscpy_s(lf1.lfFaceName, _T("΢���ź�"));
	lf1.lfHeight = 24;
	lf1.lfWeight = FW_BOLD;
	AddFont(YAHEI_24_B, lf1);

	// �ź� 24 �д�
	_tcscpy_s(lf1.lfFaceName, _T("΢���ź�"));
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
    //����λ��
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
		// ������Ļ
		Graphics graphics(pDC->GetSafeHdc());

		// ��ͼ����
		ImageAttributes imgAttr;
		imgAttr.SetWrapMode(WrapModeTileFlipXY);

		//�滭ͼ��
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

    // �и�ͼƬ
    std::vector<RectF> vecfImg;
    SepRect(rcImg, vecfImg, vecMargin);

    // �и����
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

	//����mask DC ����ʱ��Դ�ļ��� DC 
	hMaskDC = CreateCompatibleDC(hdcDest);
	hImageDC = CreateCompatibleDC(hdcDest);

	//����mask λͼ��image ����λͼ 
	hMaskBitmap = CreateBitmap(nWidthSrc, nHeightSrc, 1, 1, NULL);				//��ɫλͼ 
	hImageBitmap = CreateCompatibleBitmap(hdcDest, nWidthSrc, nHeightSrc);		//��ɫλͼ 

	//��λͼѡ��DC��
	hOldImageBitmap = (HBITMAP) SelectObject(hImageDC, hImageBitmap);
	hOldMaskBitmap = (HBITMAP) SelectObject(hMaskDC, hMaskBitmap);

	//��srcλͼ������ʱ��image DC��
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
	{
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXoriginSrc, nYoriginSrc, SRCCOPY);
	}
	else
	{
		StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXoriginSrc, nYoriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);
	}

	SetBkColor(hImageDC, crTransparent);	
	//�˲���������λͼ(mask bitmap),ע������λͼ�а�Ϊ1����Ϊ0
	//����mask λͼ��͸��ɫ�ĵط��ǰ�ɫ������Ϊ��ɫ 
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
