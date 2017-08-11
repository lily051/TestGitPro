/**   @file		RenderMgr.h
 *    @note		HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief 
 *
 *    @author	jddd
 *    @date		2015/09/12
 *
 *    @note 	�����ͼ��صĽӿڣ�ͼƬ����ɫ
 *    @note 	������Դ������
 *    @note 	
 *
 *    @warning 	
 */

#pragma once
#include <map>
#include "FuiBase.h"

// ��ͼģʽ
enum
{
	RENDER_GDIPLUS = 101,
	RENDER_GDI,
	RENDER_DIRECTDRAW,
};

class AFX_EXT_CLASS CRenderMgr
{
public:
    CRenderMgr(void);
    ~CRenderMgr(void);

    //��������
public:
    static CRenderMgr* GetInstance();

private:
    std::map<CString, CFont*> m_mapResFont;

public:
    // �������
    void AddFont(const CString& strName, LOGFONT lfFont);

    CFont* GetFont(const CString& strName);

    void DrawTextX(CDC *pDC, const CString& strText, LPRECT lpRect, COLORREF clrText, UINT uFormat);

    void DrawTextX(CDC *pDC, const CString& strText, LPRECT lpRect, COLORREF clrText, int nRowDis, UINT uFormat);

    // ������ɫ
    void DrawColor(CDC *pDC, const RECT& rc, COLORREF color);

    // ��������
    void DrawLine(CDC *pDC,  const RECT& rc, int nWidth, COLORREF dwPenColor,int nStyle = PS_SOLID);

    // ���ƾ���
    void DrawRect(CDC *pDC, const RECT& rc, int nWidth, COLORREF dwPenColor);

    // ����ͼƬ
    void DrawImageX(CDC *pDC, Image* pImage, REAL dstx, REAL dsty, REAL dstw, 
        REAL dsth, REAL srcx, REAL srcy, REAL srcw, REAL srch);

    void DrawImageX(CDC *pDC, Image* pImage, const RectF& rcf, REAL srcx, REAL srcy, REAL srcw, REAL srch, const int nRenderMode = RENDER_GDIPLUS);

	BOOL DrawTransparentBmp(HDC hdcDest, int nXoriginDest, int nYoriginDest 
		, int nWidthDest, int nHeightDest, HDC hdcSrc, int nXoriginSrc, int nYoriginSrc
		, int nWidthSrc, int nHeightSrc, UINT crTransparent);

    // 9�����ͼ
    void DrawImageX(CDC *pDC, Image* pImage, const CRect& rcDst, int nMargin);

    // ����͸������
    void DrawTransparentBkg();

    // ��ϻ�ͼ
};

#define RenderEngine	CRenderMgr::GetInstance()