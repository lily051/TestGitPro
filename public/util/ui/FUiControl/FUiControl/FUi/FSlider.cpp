// FSlider.cpp : 实现文件
//

#include "stdafx.h"
#include "FSlider.h"
#include "FMemDCEx.h"
#include "fuilib.h"


// CFSlider

IMPLEMENT_DYNAMIC(CFSlider, CSliderCtrl)

CFSlider::CFSlider()
: m_pImgChannel(NULL)
, m_pImgThumb(NULL)
, m_clrBkg(FUi::White)
, m_bHover(TRUE)
{
}

CFSlider::~CFSlider()
{
    m_pImgThumb = NULL;
    m_pImgChannel = NULL;
}


BEGIN_MESSAGE_MAP(CFSlider, CSliderCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CFSlider::OnNMCustomdraw)
    ON_WM_MOUSEMOVE()
    ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CFSlider 消息处理程序
void CFSlider::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    
    // 绘制进度
    DWORD dwStage = pNMCD->dwDrawStage;
    // 绘制项
    DWORD itemSpec = pNMCD->dwItemSpec;

    CDC* pDC = CDC::FromHandle( pNMCD->hdc);
    if (NULL == pDC)
    {
        *pResult = 0;
    }

    switch (dwStage)   // 采用switch - case结构来区分各个阶段
    {
    case CDDS_PREPAINT:       // 在程序绘制刚开始时
        // 这里如果没有对返回的值pResult赋值
        // CDRF_NOTIFYITEMDRAW消息则，图像的自绘制会失败
        // 程序将无法收到item的绘制消息
        // 而CDRF_NOTIFYPOSTPAINT则是为了绘制channel
        // 绘制循环时循环进行的
        {
            *pResult = CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
            break;
        }
    case CDDS_ITEMPREPAINT:
        {
            switch (itemSpec)
            {
                // 如果不想做操作可以赋值CDRF_DODEFAULT
            case TBCD_CHANNEL:
                {
                    DrawChannel(pNMCD->rc, GetPos(), pNMCD->hdc);
                    *pResult = CDRF_SKIPDEFAULT;
                    break;
                }
            case TBCD_THUMB:
                {
                    CRect rc_(pNMCD->rc);
                    CPoint pt = rc_.CenterPoint();
                    DrawThumb(pt, pNMCD->hdc);

                    // 忽略默认的绘制如果没有这句
                    // 之前的工作就白费了
                    *pResult = CDRF_SKIPDEFAULT;
                break;
                }
            default:
                break;
            }
            break;
        }
    case CDDS_ITEMPOSTPAINT:
        switch (itemSpec)
        {
        case TBCD_CHANNEL:
            DrawChannel(pNMCD->rc, GetPos(), pNMCD->hdc);
            *pResult = CDRF_SKIPDEFAULT;
            break;
        }
        break;
    default:
        break;
    
    }
}

void CFSlider::SetObjectName(LPCTSTR szObjectName)
{
    CString strTemp(szObjectName);
    Image* pImgC = CResMgr::GetInstance()->GetRes(strTemp + _T("_c"));
    if (NULL == pImgC)
    {
        return ;
    }
    Image* pImgT = CResMgr::GetInstance()->GetRes(strTemp + _T("_t"));
    if (NULL == pImgT)
    {
        return ;
    }

    m_strObjectName = szObjectName;
    m_pImgChannel = pImgC;
    m_pImgThumb = pImgT;
}

void CFSlider::DrawThumb(CPoint& pt, HDC hDC)
{
    if (NULL == m_pImgThumb)
    {
        return ;
    }
    CRect rc;
    rc.left = pt.x - 7;
    rc.right = pt.x + 7;
    rc.top = pt.y - 7;
    rc.bottom = pt.y + 7;

    RectF rcfDst;
    Rect2RectF(rc, rcfDst);

    int nOffset = 0;
    if (m_bHover)
    {
        nOffset = 1;
    }
    if (!IsWindowEnabled())
    {
        nOffset = 2;
    }

    CDC *pDC = CDC::FromHandle(hDC);
    RenderEngine->DrawImageX(pDC, m_pImgThumb, rcfDst, 0, (REAL)nOffset * m_pImgThumb->GetHeight() / 3,
        (REAL)m_pImgThumb->GetWidth(), (REAL)m_pImgThumb->GetHeight() / 3);

}

void CFSlider::DrawChannel(CRect rc, int nPos, HDC hDC)
{
    if (NULL == m_pImgChannel)
    {
        return ;
    }
    // 背景
//     CRect rcClient;
//     GetClientRect(rcClient);
// 
     CDC* pDC = CDC::FromHandle(hDC);
// 
//     RenderEngine->DrawColor(pDC, rcClient, m_clrBkg);

    // 
    int nMin = 0, nMax = 100;
    GetRange(nMin, nMax);
    // 总长度、左长度
    // 状态：normal、disable

    CPoint pt = rc.CenterPoint();

    int nOffset = 0;
    if (!IsWindowEnabled())
    {
        nOffset = 1; 
    }

    RectF rcfLeft, rcfRight;
    if (GetStyle() & TBS_VERT)
    {
        rcfLeft = RectF((REAL)pt.x - 1, (REAL)rc.top, 2, (REAL)rc.Height() * nPos / nMax);
        rcfRight = RectF(rcfLeft.X, rcfLeft.Y + rcfLeft.Height, 2, rc.Height()  - rcfLeft.Height);
    }
    else
    {
        rcfLeft = RectF((REAL)rc.left, (REAL)pt.y - 1, (REAL)rc.Width() * nPos / nMax, 2);
        rcfRight = RectF(rcfLeft.X + rcfLeft.Width, (REAL)pt.y - 1, rc.Width() - rcfLeft.Width, 2);
    }

    RenderEngine->DrawImageX(pDC, m_pImgChannel, rcfLeft, 0, (REAL)nOffset * m_pImgChannel->GetHeight() / 4, 
        (REAL)m_pImgChannel->GetWidth(), (REAL)m_pImgChannel->GetHeight() / 4 - 1);

    RenderEngine->DrawImageX(pDC, m_pImgChannel, rcfRight, 0, (REAL)(nOffset + 2) * m_pImgChannel->GetHeight() / 4, 
        (REAL)m_pImgChannel->GetWidth(), (REAL)m_pImgChannel->GetHeight() / 4 - 1);
}

void CFSlider::PreSubclassWindow()
{
    ModifyStyle(0, TBS_FIXEDLENGTH);
    SetThumbLength(28);

    CSliderCtrl::PreSubclassWindow();
}

void CFSlider::OnMouseMove(UINT nFlags, CPoint point)
{
    // 由于Thumb的大小为15，但是贴图为14，迅速滑动时会有界面刷新异常的地方（部分channel没有刷新）
    // 所以，这里考虑在鼠标移动后补充刷新一下
    Invalidate();

    CSliderCtrl::OnMouseMove(nFlags, point);
}

BOOL CFSlider::OnEraseBkgnd(CDC* /*pDC*/)
{
    // 不调用系统自己的背景，避免闪烁
    return TRUE;
}

void CFSlider::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 按键在滑动块区域内，不做操作
	CRect tRect;
	GetThumbRect(&tRect);
	if (tRect.PtInRect(point))
	{
		CSliderCtrl::OnLButtonDown(nFlags, point);
		return;
	}

	// 实现滑动块精确定位   

	CRect rcSlider;

	GetChannelRect(&rcSlider);
	int nMax = GetRangeMax();
	float fInterval = float(rcSlider.Width())/nMax;
	float fPos = (point.x-rcSlider.left+fInterval-1)/fInterval;

	CSliderCtrl::SetPos((int)fPos);

	//CSliderCtrl::OnLButtonDown(nFlags, point);
}
