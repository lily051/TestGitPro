/**   @file		RenderMgr.h
 *    @note		HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief 
 *
 *    @author	jddd
 *    @date		2015/09/12
 *
 *    @note 	管理绘图相关的接口：图片、颜色
 *    @note 	管理资源：字体
 *    @note 	
 *
 *    @warning 	
 */

#pragma once
#include <map>
#include "FuiBase.h"

// 绘图模式
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

    //创建单例
public:
    static CRenderMgr* GetInstance();

private:
    std::map<CString, CFont*> m_mapResFont;

public:
    // 字体管理
    void AddFont(const CString& strName, LOGFONT lfFont);

    CFont* GetFont(const CString& strName);

    void DrawTextX(CDC *pDC, const CString& strText, LPRECT lpRect, COLORREF clrText, UINT uFormat);

    void DrawTextX(CDC *pDC, const CString& strText, LPRECT lpRect, COLORREF clrText, int nRowDis, UINT uFormat);

    // 绘制颜色
    void DrawColor(CDC *pDC, const RECT& rc, COLORREF color);

    // 绘制线条
    void DrawLine(CDC *pDC,  const RECT& rc, int nWidth, COLORREF dwPenColor,int nStyle = PS_SOLID);

    // 绘制矩形
    void DrawRect(CDC *pDC, const RECT& rc, int nWidth, COLORREF dwPenColor);

    // 绘制图片
    void DrawImageX(CDC *pDC, Image* pImage, REAL dstx, REAL dsty, REAL dstw, 
        REAL dsth, REAL srcx, REAL srcy, REAL srcw, REAL srch);

    void DrawImageX(CDC *pDC, Image* pImage, const RectF& rcf, REAL srcx, REAL srcy, REAL srcw, REAL srch, const int nRenderMode = RENDER_GDIPLUS);

	BOOL DrawTransparentBmp(HDC hdcDest, int nXoriginDest, int nYoriginDest 
		, int nWidthDest, int nHeightDest, HDC hdcSrc, int nXoriginSrc, int nYoriginSrc
		, int nWidthSrc, int nHeightSrc, UINT crTransparent);

    // 9宫格绘图
    void DrawImageX(CDC *pDC, Image* pImage, const CRect& rcDst, int nMargin);

    // 绘制透明背景
    void DrawTransparentBkg();

    // 混合绘图
};

#define RenderEngine	CRenderMgr::GetInstance()