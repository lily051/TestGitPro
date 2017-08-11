// FListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "FMemDCEx.h"
#include "FuiBase.h"
#include "FListCtrl.h"

//理财业务状态和审批状态
#define BUSINESSPASS 	_T("审批通过")
#define RECORDPASS	    _T("合规")
#define UPLOADSUCCESS	_T("上传成功")

#define BUSINESSERROR	_T("审批退回")
#define RECORDERROR	    _T("不合规")
#define UPLOADFAILED	_T("上传失败")

// 列表排序所需信息结构体
typedef struct sort_info
{
    CFListCtrl* pListCtrl;  // 列表指针
    int nSubItem;           // 排序的列
    int nSortMode;          // 排序的方式
    sort_info()
    {
        pListCtrl = NULL;
        nSubItem = -1;
        nSortMode = LIST_SORT_NO;
    }
}STRUCT_SORT_INFO;

// CFListCtrl

IMPLEMENT_DYNAMIC(CFListCtrl, CListCtrl)

CFListCtrl::CFListCtrl()
	: m_nMultiBtnHeight(MULTI_BUTTON_HEIGHT)
	, m_nMultiBtnWidth(MULTI_BUTTON_HEIGHT)
{
    m_isDataBegin = false;
    m_isCheckBox = false;
    m_isHeadCreateBox = false;
    m_nStyle = FLS_ZEBRA;
    m_nHoverItem = -1;
    m_nPreHoverItem = -1;
    m_nMultiBtnSub = -1;
    m_nMultiBtnShow = 0;
    m_nHeight = 34;
    m_ptMultiBtnHover = CPoint(-1, -1);
    m_ptMultiBtnClick = CPoint(-1, -1);
	m_ptLastHoverBtn = CPoint(-1, -1);
    m_nScrollPos = 0;
    m_nScrollPage = 0;
    m_imgList.Create(BMPIMAGE_SIZE, BMPIMAGE_SIZE, ILC_COLOR32 | ILC_MASK, 0, 20);
	m_bEnableToolTip = false;
	m_nItem = -1;
	m_nSubItem = -1;
    m_pActrueParent = NULL;

    m_mapFont[FUi::Normal] = YAHEI_18_N;
    m_mapFont[FUi::Pressed] = YAHEI_18_N;

    m_mapTextColor[FUi::Normal] = FUi::Black;
    m_mapTextColor[FUi::Pressed] = FUi::Black;
	m_toolTip.Create(this);
}

//lint -e1551
CFListCtrl::~CFListCtrl()
{
    m_imgList.DeleteImageList();

    if (m_listCheckBox.size())
    {
        std::list<CFButton*>::iterator itor = m_listCheckBox.begin();
        while(itor != m_listCheckBox.end())
        {
            delete (*itor);
            itor++;
        }
        std::list<CFButton*>().swap(m_listCheckBox);
    }

    if (m_mapButton.size())
    {
        std::map<std::pair<int, int>, CFButton*>::iterator itor = m_mapButton.begin();
        while(itor != m_mapButton.end())
        {
            delete itor->second;
            itor++;
        }
        std::map<std::pair<int, int>, CFButton*>().swap(m_mapButton);
    }
}
//lint +e1551

//lint -e648
BEGIN_MESSAGE_MAP(CFListCtrl, CListCtrl)
    ON_WM_CREATE()
    ON_WM_MEASUREITEM()
    ON_WM_MEASUREITEM_REFLECT()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_ERASEBKGND()
    ON_WM_WINDOWPOSCHANGING()
    ON_NOTIFY_REFLECT(NM_HOVER, &CFListCtrl::OnNMHover)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CFListCtrl::OnNMCustomdraw)
    ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CFListCtrl::OnLvnItemchanged)
    ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CFListCtrl::OnLvnColumnclick)
    ON_MESSAGE(WM_HEADCTRL_CHECKBOX, &CFListCtrl::OnHeadCtrlCheckBox)
    ON_MESSAGE(MSG_CHECKBOX_CLICK, &CFListCtrl::OnListItemCheckBox)
    ON_CONTROL_RANGE(BN_CLICKED, IDC_LIST_BUTTON_BEGIN, IDC_LIST_BUTTON_END, &CFListCtrl::OnBtnClick)
    ON_NOTIFY(HDN_ITEMCHANGINGA, 0, &CFListCtrl::OnHdnItemchanging)
    ON_NOTIFY(HDN_ITEMCHANGINGW, 0, &CFListCtrl::OnHdnItemchanging)
    ON_NOTIFY(HDN_BEGINTRACKA, 0, &CFListCtrl::OnHdnBegintrack)
    ON_NOTIFY(HDN_BEGINTRACKW, 0, &CFListCtrl::OnHdnBegintrack)
    ON_NOTIFY(HDN_ITEMCHANGEDA, 0, &CFListCtrl::OnHdnItemchanged)
    ON_NOTIFY(HDN_ITEMCHANGEDW, 0, &CFListCtrl::OnHdnItemchanged)
    ON_NOTIFY(HDN_ENDTRACKA, 0, &CFListCtrl::OnHdnEndtrack)
    ON_NOTIFY(HDN_ENDTRACKW, 0, &CFListCtrl::OnHdnEndtrack)
END_MESSAGE_MAP()
//lint +e648

// CFListCtrl 消息处理程序

void CFListCtrl::PreSubclassWindow()
{
    SetWindowLong(this->m_hWnd, GWL_STYLE, GetStyle() | WS_CLIPCHILDREN |
        WS_CLIPSIBLINGS | LVS_OWNERDRAWFIXED | LVS_REPORT);
    ModifyStyle(WS_HSCROLL | WS_VSCROLL, 0);
    SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT |
        LVS_EX_TRACKSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_DOUBLEBUFFER);
    SetHoverTime(10);

    CHeaderCtrl* pHeader = GetHeaderCtrl();
    m_listHeader.SubclassWindow(pHeader->GetSafeHwnd());
    m_listHeader.SubclassDlgItem(0, this);
    m_listHeader.ModifyStyle(0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

    if (m_nStyle == FLS_BLACK)
    {
        m_pImgNoData = CResMgr::GetInstance()->GetRes(_T("nodata_black"));
    }
    else
    {
        m_pImgNoData = CResMgr::GetInstance()->GetRes(_T("nodata"));
    }

	

    // 解决设置列表头高度开始时无效的问题
    InsertItem(0, _T(""));
    DeleteItem(0);

    CListCtrl::PreSubclassWindow();
}

int CFListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CListCtrl::OnCreate(lpCreateStruct) == -1)
        return -1;

    SetWindowLong(this->m_hWnd, GWL_STYLE, GetStyle() | WS_CLIPCHILDREN |
        WS_CLIPSIBLINGS | LVS_OWNERDRAWFIXED | LVS_REPORT);
    SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT |
        LVS_EX_TRACKSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_DOUBLEBUFFER);
    CHeaderCtrl* pHeader = GetHeaderCtrl();
    m_listHeader.SubclassWindow(pHeader->GetSafeHwnd());
    m_listHeader.ModifyStyle(0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

    // 解决设置列表头高度开始时无效的问题
    InsertItem(0, _T(""));
    DeleteItem(0);

    return 0;
}

void CFListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // 获取DC和列表尺寸
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

    // 获取滚动条信息
    SCROLLINFO scrollInfo;
    GetScrollInfo(SB_VERT, &scrollInfo);
    m_nScrollPos = scrollInfo.nPos;
    m_nScrollPage = scrollInfo.nPage;

    CRect rcItem, rcClient;
    GetClientRect(&rcClient);
    GetItemRect(lpDrawItemStruct->itemID, rcItem, LVIR_BOUNDS);
    rcItem.right = rcClient.right;

    DrawBg(pDC, rcItem, lpDrawItemStruct);

    DrawListItem(pDC, lpDrawItemStruct);
}

void CFListCtrl::DrawBg(CDC* pDC, CRect& rcItem, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // 画背景
    if (FLS_LINE == m_nStyle)
    {
        rcItem.bottom -= 1;
    }
    if ((lpDrawItemStruct->itemState & ODS_SELECTED) && (FLS_LINE == m_nStyle || FLS_ZEBRA == m_nStyle))
    {
        RenderEngine->DrawColor(pDC, rcItem, FUi::RGB_197);
    }
    else if(((int)lpDrawItemStruct->itemID == m_nHoverItem) && (FLS_LINE == m_nStyle || FLS_ZEBRA == m_nStyle))
    {
        RenderEngine->DrawColor(pDC, rcItem, FUi::RGB_222);
    }
    else if ((lpDrawItemStruct->itemState & ODS_SELECTED) && (FLS_BLACK == m_nStyle))
    {
        RenderEngine->DrawColor(pDC, rcItem, RGB(0x36, 0x36, 0x36));
    }
    else if(((int)lpDrawItemStruct->itemID == m_nHoverItem) && (FLS_BLACK == m_nStyle))
    {
        RenderEngine->DrawColor(pDC, rcItem, RGB(60, 60, 60));
    }
    else if (FLS_ZEBRA == m_nStyle) // 斑马纹
    {
        if (0 == lpDrawItemStruct->itemID % 2)
        {
            RenderEngine->DrawColor(pDC, rcItem, FUi::White);
        }
        else
        {
            RenderEngine->DrawColor(pDC, rcItem, FUi::RGB_242);
        }
    }
    else if (FLS_LINE == m_nStyle)  // 线性
    {
        RenderEngine->DrawColor(pDC, rcItem, FUi::White);
        RenderEngine->DrawLine(pDC, CRect(rcItem.left, rcItem.bottom, rcItem.right, rcItem.bottom), 1, FUi::RGB_234);
    }
	else if (FLS_BLACK == m_nStyle)	//黑色调
	{
		RenderEngine->DrawColor(pDC, rcItem, RGB(24, 24, 24));
	}
}

void CFListCtrl::DrawListItem(CDC* pDC, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    LV_COLUMN lvColumn;
    ZeroMemory(&lvColumn, sizeof(lvColumn));
    lvColumn.mask = LVCF_WIDTH | LVCF_FMT;

    for (int i = 0; GetColumn(i, &lvColumn); i++)
    {
        // 获取列表项尺寸,全部LVIR_BOUNDS
        CRect rcSubItem;
        GetSubItemRect(lpDrawItemStruct->itemID, i, LVIR_BOUNDS, rcSubItem);

        // 画checkbox
        if (m_isCheckBox && (0 == i))
        {
            std::list<CFButton*>::iterator itor = m_listCheckBox.begin();
            advance(itor, lpDrawItemStruct->itemID);
            if (itor != m_listCheckBox.end() && NULL != *itor)
            {
                // 通过客户区和表头的大小控制checkbox的位置
                (*itor)->MoveWindow(rcSubItem.left + (lvColumn.cx - CHECKBOX_SIZE) / 2,
                    rcSubItem.top + (m_nHeight - CHECKBOX_SIZE) / 2 , CHECKBOX_SIZE, CHECKBOX_SIZE);
                (*itor)->ShowWindow(SW_SHOW);
                (*itor)->Invalidate();
            }
            continue;
        }

        // 初始化列表项, 获取列表项文字
        LV_ITEM lvItem;
        WCHAR lpBuffer[256] = {0};
        ZeroMemory(&lvItem, sizeof(lvItem));
        lvItem.iItem = lpDrawItemStruct->itemID;
        lvItem.mask = LVIF_IMAGE | LVIF_TEXT | LVIF_PARAM;
        lvItem.pszText = lpBuffer;
        lvItem.cchTextMax = sizeof(lpBuffer);
        lvItem.lParam = NULL;
        lvItem.iImage = -1;
        if (m_isCheckBox)
            lvItem.iSubItem = i - 1;
        else
            lvItem.iSubItem = i;

        GetItem(&lvItem);

        // 画图像
        if (-1 != lvItem.iImage)
        {
            m_imgList.Draw(pDC, lvItem.iImage, CPoint(rcSubItem.left + SUBITEM_MARGIN,
                rcSubItem.top + (m_nHeight - BMPIMAGE_SIZE) / 2 - 1), ILD_TRANSPARENT);
        }

        // 获取文字，对齐方式以及画图区域
        CString strText(lpBuffer);
        int textAlign = DT_LEFT;
        if (lvColumn.fmt & LVCFMT_CENTER)
            textAlign = DT_CENTER;
        else if (lvColumn.fmt & LVCFMT_RIGHT)
            textAlign = DT_RIGHT;
        UINT uFormat = DT_VCENTER | DT_SINGLELINE | DT_EDITCONTROL | DT_END_ELLIPSIS | textAlign;

        // 获取列表项尺寸,文字部分LVIR_LABEL
        GetSubItemRect(lpDrawItemStruct->itemID, i, LVIR_LABEL, rcSubItem);
        if (-1 == lvItem.iImage)
        {
            rcSubItem.left += SUBITEM_MARGIN;
        }
        else
        {
            rcSubItem.left += BMPIMAGE_SIZE + SUBITEM_MARGIN + 5;
        }

		if (FLS_LINE == m_nStyle || FLS_ZEBRA == m_nStyle)
		{
			// 画字
			CFont* pOldFont;
			if (lpDrawItemStruct->itemState & ODS_SELECTED)
			{
				pOldFont = pDC->SelectObject(RenderEngine->GetFont(m_mapFont[FUi::Pressed]));
				if (1 == i)
				{
					//RenderEngine->DrawTextX(pDC, strText, rcSubItem, RGB(0x3F, 0x85, 0xE4), uFormat);
					RenderEngine->DrawTextX(pDC, strText, rcSubItem, m_mapTextColor[FUi::Pressed], uFormat);
				}
				else if (7 == i)
				{
					if (strText == BUSINESSPASS || strText == RECORDPASS)
					{
						//RenderEngine->DrawTextX(pDC, strText, rcSubItem, RGB(49, 154, 38), uFormat);
						RenderEngine->DrawTextX(pDC, strText, rcSubItem, m_mapTextColor[FUi::Pressed], uFormat);
					}
					else if (strText == BUSINESSERROR || strText == RECORDERROR)
					{
						RenderEngine->DrawTextX(pDC, strText, rcSubItem, FUi::Red, uFormat);
					}
					else
					{
						RenderEngine->DrawTextX(pDC, strText, rcSubItem, m_mapTextColor[FUi::Pressed], uFormat);
					}
				}
				else if (8 == i)
				{
					if (strText == UPLOADSUCCESS)
					{
						//RenderEngine->DrawTextX(pDC, strText, rcSubItem, RGB(49, 154, 38), uFormat);
						RenderEngine->DrawTextX(pDC, strText, rcSubItem, m_mapTextColor[FUi::Pressed], uFormat);
					}
					else if (strText == UPLOADFAILED)
					{
						RenderEngine->DrawTextX(pDC, strText, rcSubItem, FUi::Red, uFormat);
					}
					else
					{
						RenderEngine->DrawTextX(pDC, strText, rcSubItem, m_mapTextColor[FUi::Pressed], uFormat);
					}
				}
				else
				{
					RenderEngine->DrawTextX(pDC, strText, rcSubItem, m_mapTextColor[FUi::Pressed], uFormat);
				}
			}
			else
			{
				pOldFont = pDC->SelectObject(RenderEngine->GetFont(m_mapFont[FUi::Normal]));
				if (1 == i)
				{
					//RenderEngine->DrawTextX(pDC, strText, rcSubItem, RGB(0x3F, 0x85, 0xE4), uFormat);
					RenderEngine->DrawTextX(pDC, strText, rcSubItem, m_mapTextColor[FUi::Normal], uFormat);
				}
				else if (7 == i)
				{
					if (strText == (_T("审批通过")) || strText == (_T("合规")))
					{
						//RenderEngine->DrawTextX(pDC, strText, rcSubItem, RGB(49, 154, 38), uFormat);
						RenderEngine->DrawTextX(pDC, strText, rcSubItem, m_mapTextColor[FUi::Pressed], uFormat);
					}
					else if (strText == (_T("审批退回")) || strText == (_T("不合规")))
					{
						RenderEngine->DrawTextX(pDC, strText, rcSubItem, FUi::Red, uFormat);
					}
					else
					{
						RenderEngine->DrawTextX(pDC, strText, rcSubItem, m_mapTextColor[FUi::Normal], uFormat);
					}
				}
				else if (8 == i)
				{
					if (strText == (_T("上传成功")))
					{
						//RenderEngine->DrawTextX(pDC, strText, rcSubItem, RGB(49, 154, 38), uFormat);
						RenderEngine->DrawTextX(pDC, strText, rcSubItem, m_mapTextColor[FUi::Pressed], uFormat);
					}
					else if (strText == (_T("上传失败")))
					{
						RenderEngine->DrawTextX(pDC, strText, rcSubItem, FUi::Red, uFormat);
					}
					else
					{
						RenderEngine->DrawTextX(pDC, strText, rcSubItem, m_mapTextColor[FUi::Normal], uFormat);
					}
				}
				else
				{
					RenderEngine->DrawTextX(pDC, strText, rcSubItem, m_mapTextColor[FUi::Normal], uFormat);
				}
			}
			pDC->SelectObject(pOldFont);
		}
		else
		{
			// 画字
			CFont* pOldFont;
			if (lpDrawItemStruct->itemState & ODS_SELECTED)
			{
				pOldFont = pDC->SelectObject(RenderEngine->GetFont(m_mapFont[FUi::Pressed]));
				RenderEngine->DrawTextX(pDC, strText, rcSubItem, RGB(0xC0, 0xC0, 0xC0), uFormat);
			}
			else
			{
				pOldFont = pDC->SelectObject(RenderEngine->GetFont(m_mapFont[FUi::Normal]));
				RenderEngine->DrawTextX(pDC, strText, rcSubItem, RGB(0xC0, 0xC0, 0xC0), uFormat);
			}
			pDC->SelectObject(pOldFont);
		}


        DrawBtn(pDC, lvColumn, lvItem, rcSubItem);
    }
}

void CFListCtrl::DrawBtn(CDC* pDC, LV_COLUMN& lvColumn, LV_ITEM& lvItem, CRect& rcSubItem)
{
    // 画按钮
    std::map<std::pair<int, int>, CFButton*>::iterator itFind =
        m_mapButton.find(std::make_pair(lvItem.iItem, lvItem.iSubItem));
    if (itFind != m_mapButton.end())
    {
        // 当表头宽度小于按钮宽度时，按钮随表头变化
        int buttonWidth = 0;
        if (lvColumn.cx < BUTTON_WIDTH + SUBITEM_MARGIN)
        {
            buttonWidth = lvColumn.cx - SUBITEM_MARGIN;
        }
        else
        {
            buttonWidth = BUTTON_WIDTH;
        }

        itFind->second->MoveWindow(rcSubItem.left, rcSubItem.top +
            (m_nHeight - BUTTON_HEIGHT) / 2, buttonWidth, BUTTON_HEIGHT);

        itFind->second->ShowWindow(SW_SHOW);
        itFind->second->Invalidate();
    }

    // 画多按钮
    if (m_nMultiBtnSub == lvItem.iSubItem)
    {
        CRect rcMutiBtn(rcSubItem);
        rcMutiBtn.top = rcSubItem.top + (m_nHeight - m_nMultiBtnHeight) / 2;

        m_nMultiBtnShow = 0;
        for (int i = 0; i < (int)m_vecBtnImage.size(); i++)
        {
            int drawWidth = lvColumn.cx - (rcMutiBtn.left + m_nMultiBtnWidth - (rcSubItem.left - SUBITEM_MARGIN));
            if (drawWidth >= 0)
            {
                drawWidth = m_nMultiBtnWidth;
                m_nMultiBtnShow++;
            }
            else
            {
                drawWidth = 0;
            }

            RectF rcfDst((REAL)rcMutiBtn.left, (REAL)rcMutiBtn.top, (REAL)drawWidth, (REAL)m_nMultiBtnHeight);

            if (CPoint(lvItem.iItem, i) == m_ptMultiBtnClick)
            {
                RenderEngine->DrawImageX(pDC, m_vecBtnImage[i], rcfDst, 0, (REAL)m_vecBtnImage[i]->GetHeight() * 2 / 4,
                    (REAL)m_vecBtnImage[i]->GetWidth(), (REAL)m_vecBtnImage[i]->GetHeight() / 4);
            }
            else if (CPoint(lvItem.iItem, i) == m_ptMultiBtnHover)
            {
                RenderEngine->DrawImageX(pDC, m_vecBtnImage[i], rcfDst, 0, (REAL)m_vecBtnImage[i]->GetHeight() / 4,
                    (REAL)m_vecBtnImage[i]->GetWidth(), (REAL)m_vecBtnImage[i]->GetHeight() / 4);
            }
            else
            {
                RenderEngine->DrawImageX(pDC, m_vecBtnImage[i], rcfDst, 0, 0, 
                    (REAL)m_vecBtnImage[i]->GetWidth(), (REAL)m_vecBtnImage[i]->GetHeight() / 4);
            }

            rcMutiBtn.left += m_nMultiBtnWidth + 5;
        }
    }
}

void CFListCtrl::HideBtn()
{
    // 隐藏不在客户区显示的checkbox
    for (int i = 0; i < (int)m_listCheckBox.size(); i++)
    {
        if (i < m_nScrollPos || i > m_nScrollPos + m_nScrollPage)
        {
            std::list<CFButton*>::iterator itor = m_listCheckBox.begin();
            advance(itor, i);
            (*itor)->ShowWindow(SW_HIDE);
        }
    }

    // 隐藏不在客户区显示的按钮
    for (int i = 0; i < GetItemCount(); i++)
    {
        if (i >= m_nScrollPos && i <= m_nScrollPos + m_nScrollPage)
            continue;

        for(int j = 0; j < GetHeaderCtrl()->GetItemCount(); j++)
        {
            std::map<std::pair<int, int>, CFButton*>::iterator itFind =
                m_mapButton.find(std::make_pair(i, j));
            if (itFind != m_mapButton.end())
            {
                itFind->second->ShowWindow(SW_HIDE);
            }
        }
    }
}

void CFListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    // MOUSE_LEAVE消息需要经过注册才能捕捉
    if (-1 != m_nHoverItem)
    {
        // 注册TME_LEAVE消息
        TRACKMOUSEEVENT mouseEvent;
        mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
        mouseEvent.dwFlags = TME_LEAVE;
        mouseEvent.hwndTrack = GetSafeHwnd();
        _TrackMouseEvent(&mouseEvent);
    }
	if (m_bEnableToolTip)
	{
		CString str;
		LVHITTESTINFO lvhti;

		// 判断鼠标当前所在的位置(行, 列)
		lvhti.pt = point;    
		SubItemHitTest(&lvhti);
		if (m_isCheckBox)
		{
			lvhti.iSubItem--;
		}

		//判断鼠标在多按钮上
		for (UINT i = 0; i< m_vecBtnImage.size(); ++i)
		{
			CPoint ptCurrentBtn(lvhti.iItem, i);
			if ((ptCurrentBtn == m_ptMultiBtnClick || ptCurrentBtn == m_ptMultiBtnHover) && ptCurrentBtn != m_ptLastHoverBtn)
			{
				m_ptLastHoverBtn = ptCurrentBtn;
				if (m_vecBtnToolTip.size() > i)
				{
					m_toolTip.AddTool(this, m_vecBtnToolTip[i]);
					m_toolTip.Pop();
				}
			}
		}

		//如果鼠标移动到另一个单元格内, 则进行处理; 否则, 不做处理
		if((lvhti.iItem != m_nItem) || (lvhti.iSubItem != m_nSubItem))
		{
			// 保存当前鼠标所在的(行,列)
			m_nItem = lvhti.iItem;
			m_nSubItem = lvhti.iSubItem;

			// 如果鼠标移动到一个合法的单元格内,则显示新的提示信息
			// 否则, 不显示提示
			if((m_nItem != -1) && (m_nSubItem != -1))
			{
				// @@@@@@@@ 在这里修改要显示的提示信息
				// 这里仅仅是一个例子---获得当前单元格的文字信息, 并设置为新的提示信息
				str = GetItemText(m_nItem ,m_nSubItem);   
				m_toolTip.AddTool(this, str); 
				// 显示提示框
				m_toolTip.Pop();
			}
		}
	}

    CListCtrl::OnMouseMove(nFlags, point);
}

void CFListCtrl::OnMouseLeave()
{
    // 鼠标离开列表区域，刷新悬停项区域，让它恢复正常
    if (m_nHoverItem < GetItemCount())
    {
        CRect rcItem, rcClient;
        GetClientRect(&rcClient);

        GetItemRect(m_nHoverItem, &rcItem, LVIR_BOUNDS);
        rcItem.right = rcClient.right;
        InvalidateRect(&rcItem);
    }

    m_nHoverItem = -1;
    m_nPreHoverItem = m_nHoverItem;
    CListCtrl::OnMouseLeave();
}

void CFListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // 计算多按钮点击
    int subItem = m_nMultiBtnSub;
    CRect rcSubItem;
    if (m_isCheckBox)
    {
        subItem++;
    }

    // 获取列表项尺寸,文字部分LVIR_LABEL
    GetSubItemRect(m_nHoverItem, subItem, LVIR_LABEL, rcSubItem);

    // 鼠标双击击在多按钮项上
    if (rcSubItem.PtInRect(point))
    {
        // 双击变成单击
        CFListCtrl::OnLButtonDown(nFlags, point);
        return;
    }

    CListCtrl::OnLButtonDblClk(nFlags, point);
}

void CFListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_nHoverItem >= GetItemCount() || m_nHoverItem < 0)
    {
        CListCtrl::OnLButtonDown(nFlags, point);
        return;
    }

	bool bChecked = IsItemChecked(m_nHoverItem);
	if (!bChecked)
	{
		SetItemChecked(m_nHoverItem, !bChecked);
	}

    // 计算多按钮点击
    int subItem = m_nMultiBtnSub;
    CRect rcSubItem;
    if (m_isCheckBox)
    {
        subItem++;
    }

    // 获取列表项尺寸,文字部分LVIR_LABEL
    GetSubItemRect(m_nHoverItem, subItem, LVIR_LABEL, rcSubItem);

    // 多按钮理想状态为没有图标，防止可能有
    LV_ITEM lvItem;
    ZeroMemory(&lvItem, sizeof(lvItem));
    lvItem.iItem = m_nHoverItem;
    lvItem.mask = LVIF_IMAGE;
    lvItem.iImage = -1;
    lvItem.iSubItem = subItem;

    GetItem(&lvItem);
    if (-1 == lvItem.iImage)
    {
        rcSubItem.left += SUBITEM_MARGIN;
    }
    else
    {
        rcSubItem.left += BMPIMAGE_SIZE + SUBITEM_MARGIN + 5;
    }

    // 鼠标点击在多按钮项上
    if (rcSubItem.PtInRect(point))
    {
        CRect rcMutiBtn(rcSubItem);
        rcMutiBtn.top = rcSubItem.top + (m_nHeight - m_nMultiBtnHeight) / 2;
        rcMutiBtn.bottom = rcMutiBtn.top + m_nMultiBtnHeight;
        rcMutiBtn.right = rcMutiBtn.left + m_nMultiBtnWidth;

        for (int i = 0; i < (int)m_vecBtnImage.size(); i++)
        {
            // 记录在多按钮上点击的项
            if (rcMutiBtn.PtInRect(point) && i < m_nMultiBtnShow)
            {
                m_ptMultiBtnClick = CPoint(m_nHoverItem, i);
                InvalidateRect(rcMutiBtn);
                return;
            }

            rcMutiBtn.left += m_nMultiBtnWidth + 5;
            rcMutiBtn.right = rcMutiBtn.left + m_nMultiBtnWidth;
        }
    }

	//通知父窗口
    CWnd* pParent = GetParent();
    if (m_pActrueParent)
    {
        pParent = m_pActrueParent;
    }
	::PostMessage(pParent->GetSafeHwnd(), WM_LBUTTONDOWN, 0, 0);

    CListCtrl::OnLButtonDown(nFlags, point);
}

void CFListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_ptMultiBtnClick != CPoint(-1, -1))
    {
        int subItem = m_nMultiBtnSub;
        if (m_isCheckBox)
        {
            subItem++;
        }

        if (m_ptMultiBtnHover != CPoint(-1, -1))
        {
            CRect rcPreHover;
            GetSubItemRect(m_ptMultiBtnHover.x, subItem, LVIR_LABEL, rcPreHover);
            m_ptMultiBtnHover = CPoint(-1, -1);

            InvalidateRect(rcPreHover);
        }

        STRU_MULTIBTN_INFO* multibtnInfo = new STRU_MULTIBTN_INFO;
        multibtnInfo->nItem = m_ptMultiBtnClick.x;
        multibtnInfo->nNumber = m_ptMultiBtnClick.y;

        CWnd* pParent = GetParent();
        if (m_pActrueParent)
        {
            pParent = m_pActrueParent;
        }
        ::PostMessage(pParent->GetSafeHwnd(), WM_COMMAND, MSG_MULTIBTN_CLICK, (LPARAM)multibtnInfo);

        CRect rcSubItem;
        GetSubItemRect(m_ptMultiBtnClick.x, subItem, LVIR_LABEL, rcSubItem);
        m_ptMultiBtnClick = CPoint(-1, -1);
        InvalidateRect(rcSubItem);
    }

    CListCtrl::OnLButtonUp(nFlags, point);
}

void CFListCtrl::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
    CListCtrl::OnWindowPosChanging(lpwndpos);
    // override only if control is resized
    if ((lpwndpos->flags & SWP_NOSIZE) != 0)
        return;
    // get current size of the control
    RECT rect;
    GetWindowRect(&rect);
    // calculate control width change
    int deltaX = lpwndpos->cx - (rect.right - rect.left);
    // if control is narrowed, correct the width of the last column 
    // to prevent horizontal scroll bar to appear
    if (deltaX < 0)
    {
        int lastColumnIndex = GetLastColumnIndex();
        int columnWidth = GetColumnWidth(lastColumnIndex);
        SetColumnWidth(lastColumnIndex, columnWidth + deltaX);
    }
}

void CFListCtrl::OnNMHover(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
    // 获取鼠标在列表中时的坐标
    CPoint point;
    GetCursorPos(&point);
    ScreenToClient(&point);

    CRect rcClient;
    GetClientRect(&rcClient);

    // 根据列表项的高度、列表头的高度，以及滑动条的位移，计算悬停的列表项
    CRect rcHeaderCtrl;
    GetHeaderCtrl()->GetClientRect(&rcHeaderCtrl);
    m_nHoverItem = (point.y - rcHeaderCtrl.Height()) / m_nHeight + m_nScrollPos;

    // 为减少闪烁，缩小刷新的区域和条件
    // 悬停项小于实际列表项数 且 原悬停项和现悬停项不相等，对原悬停项和现悬停项进行刷新
    // 悬停项大于实际列表项数 且 原悬停项小于实际列表项数，对原悬停项进行刷新
    if (m_nHoverItem < GetItemCount() && m_nPreHoverItem != m_nHoverItem)
    {
        CRect rcItem;
        GetItemRect(m_nHoverItem, &rcItem, LVIR_BOUNDS);
        rcItem.right = rcClient.right;
        InvalidateRect(&rcItem);

        GetItemRect(m_nPreHoverItem, &rcItem, LVIR_BOUNDS);
        rcItem.right = rcClient.right;
        InvalidateRect(&rcItem);
    }
    else if (m_nHoverItem >= GetItemCount() && m_nPreHoverItem < GetItemCount())
    {
        CRect rcItem;
        GetItemRect(m_nPreHoverItem, &rcItem, LVIR_BOUNDS);
        rcItem.right = rcClient.right;
        InvalidateRect(&rcItem);
    }

    m_nPreHoverItem = m_nHoverItem;

    if (m_nHoverItem >= GetItemCount())
    {
        *pResult = TRUE;
        return;
    }

    // 计算多按钮悬停
    int subItem = m_nMultiBtnSub;
    CRect rcSubItem;
    if (m_isCheckBox)
    {
        subItem++;
    }

    // 获取列表项尺寸,文字部分LVIR_LABEL
    GetSubItemRect(m_nHoverItem, subItem, LVIR_LABEL, rcSubItem);

    // 多按钮理想状态为没有图标，防止可能有
    LV_ITEM lvItem;
    ZeroMemory(&lvItem, sizeof(lvItem));
    lvItem.iItem = m_nHoverItem;
    lvItem.mask = LVIF_IMAGE;
    lvItem.iImage = -1;
    lvItem.iSubItem = subItem;

    GetItem(&lvItem);
    if (-1 == lvItem.iImage)
    {
        rcSubItem.left += SUBITEM_MARGIN;
    }
    else
    {
        rcSubItem.left += BMPIMAGE_SIZE + SUBITEM_MARGIN + 5;
    }

    bool isHovermulBtn = false;
    // 鼠标在多按钮项中
    if (rcSubItem.PtInRect(point))
    {
        CRect rcMutiBtn(rcSubItem);
        rcMutiBtn.top = rcSubItem.top + (m_nHeight - m_nMultiBtnHeight) / 2;
        rcMutiBtn.bottom = rcMutiBtn.top + m_nMultiBtnHeight;
        rcMutiBtn.right = rcMutiBtn.left + m_nMultiBtnWidth;

        for (int i = 0; i < (int)m_vecBtnImage.size(); i++)
        {
            // 记录在多按钮上停留的项
            if (rcMutiBtn.PtInRect(point))
            {
                if (m_ptMultiBtnHover != CPoint(m_nHoverItem, i))
                {
                    CRect rcPreHover;
                    GetSubItemRect(m_ptMultiBtnHover.x, subItem, LVIR_LABEL, rcPreHover);
                    m_ptMultiBtnHover = CPoint(m_nHoverItem, i);

                    InvalidateRect(rcPreHover);
                    InvalidateRect(rcMutiBtn);
                }
                isHovermulBtn = true;
                break;
            }

            rcMutiBtn.left += m_nMultiBtnWidth + 5;
            rcMutiBtn.right = rcMutiBtn.left + m_nMultiBtnWidth;
        }
    }

    // 没有悬停时，取消悬停状态
    if (!isHovermulBtn)
    {
        CRect rcPreHover;
        GetSubItemRect(m_ptMultiBtnHover.x, subItem, LVIR_LABEL, rcPreHover);
        m_ptMultiBtnHover = CPoint(-1, -1);

        InvalidateRect(rcPreHover);
    }

    // 鼠标离开点击项时取消点击
    if (m_ptMultiBtnClick != CPoint(-1, -1))
    {
        CRect rcSelBtn(rcSubItem);
        rcSelBtn.top = rcSelBtn.top + (m_nHeight - m_nMultiBtnHeight) / 2;
        rcSelBtn.bottom = rcSelBtn.top + m_nMultiBtnHeight;
        rcSelBtn.left += m_ptMultiBtnClick.y * (m_nMultiBtnWidth + 5);
        rcSelBtn.right = rcSelBtn.left + m_nMultiBtnWidth;

        if (!rcSelBtn.PtInRect(point))
        {
            m_ptMultiBtnClick = CPoint(-1, -1);
            InvalidateRect(rcSelBtn);
        }
    }

    *pResult = TRUE;
}

void CFListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    CDC* pDC = CDC::FromHandle(pNMCD->hdc);

    if (pNMCD->dwDrawStage == CDDS_PREPAINT)
    {   // 准备绘制列表
        HideBtn();

        *pResult =  CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
    }
    else if (pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)
    {   // 绘制列表
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
    }
    else if (pNMCD->dwDrawStage == CDDS_POSTPAINT)
    {   // 绘制结束
        int remainTop = 0;// 底部剩余区域的顶部
        int nCount = GetItemCount();

        // 计算底部剩余区域的顶部
        if(nCount > 0)
        {
            CRect rcItem;
            GetItemRect(nCount - 1, &rcItem, LVIR_BOUNDS);
            remainTop = rcItem.bottom;
        }
        else
        {
            CRect rcClient;
            GetClientRect(&rcClient);
            remainTop = rcClient.top + GetHeadHeight();
        }

        // 绘制底部剩余非Item区域
        if (0 == GetItemCount() && Ok == m_pImgNoData->GetLastStatus()) // 没有数据
        {
            // 列表区域完整的大小
            CRect rcClient;
            GetClientRect(rcClient);
            rcClient.top += m_listHeader.GetHeight();

            REAL dstX = (REAL)rcClient.left + (rcClient.Width() - m_pImgNoData->GetWidth()) / 2;
            REAL dstY = (REAL)rcClient.top + (rcClient.Height() - m_pImgNoData->GetHeight()) / 2;
            // 计算中间位置

            if (m_nStyle == FLS_BLACK)
            {
                //m_pImgNoData = CResMgr::GetInstance()->GetRes(_T("nodata_black"));
                pDC->FillSolidRect(rcClient, RGB(24, 24, 24));
            }
            else
            {
                m_pImgNoData = CResMgr::GetInstance()->GetRes(_T("nodata"));
            }

            // 绘图
            RenderEngine->DrawImageX(pDC, m_pImgNoData, dstX, dstY, m_pImgNoData->GetWidth(), m_pImgNoData->GetHeight(), 
                0.0, 0.0, m_pImgNoData->GetWidth(), m_pImgNoData->GetHeight());

            m_isDataBegin = false;
        }
        else	// 有数据
        {
            CRect rcItem, rcHead, rcClient;
            GetClientRect(&rcClient);
            GetHeaderCtrl()->GetClientRect(&rcHead);

            for (int i = remainTop; i < rcClient.bottom; i += m_nHeight)
            {
                rcItem.top = i;
                rcItem.bottom = rcItem.top + m_nHeight;
                rcItem.left = 0;
                rcItem.right = rcHead.right;

                if (FLS_ZEBRA == m_nStyle)
                {
                    if (0 == (nCount + (i - remainTop) / m_nHeight) % 2)
                    {
                        RenderEngine->DrawColor(pDC, rcItem, FUi::White);
                    }
                    else
                    {
                        RenderEngine->DrawColor(pDC, rcItem, FUi::RGB_242);
                    }
                }
                else if (FLS_LINE == m_nStyle)
                {
                    RenderEngine->DrawColor(pDC, rcItem, FUi::White);
                    RenderEngine->DrawLine(pDC, CRect(rcItem.left, rcItem.bottom -1, rcItem.right, rcItem.bottom -1), 1, FUi::RGB_234);
                }
				else if (FLS_BLACK == m_nStyle)
				{
					 RenderEngine->DrawColor(pDC, rcItem, RGB(24, 24, 24));
				}
            }

            // 第一次有数据需要全部刷新，保证条纹能够正常显示
            if (!m_isDataBegin)
            {
                Invalidate();
                m_isDataBegin = true;
            }
        }

        *pResult =  CDRF_SKIPDEFAULT;
    }
}

void CFListCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CFListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    // 设置行高
    lpMeasureItemStruct->itemHeight = m_nHeight;
}

void CFListCtrl::OnLvnItemchanged(NMHDR* pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if (pNMListView->uChanged & LVIF_STATE)
    {
        BOOL bSelectChanged = (pNMListView->uOldState & LVIS_SELECTED) != (pNMListView->uNewState & LVIS_SELECTED);
        if (bSelectChanged)
        {
            BOOL bSelected = pNMListView->uNewState & LVIS_SELECTED;
            if (!bSelected)
                ::PostMessage(m_listHeader.GetSafeHwnd(), WM_HEADCTRL_CHECKBOX_CHANGED, 0, 0);
            SetItemChecked(pNMListView->iItem, bSelected ? 1 : 0);
        }
    }

    // 列表项改变时刷新选中和前选中区域
    CRect rcItem, rcClient;
    GetClientRect(&rcClient);

    GetItemRect(pNMCD->dwDrawStage, &rcItem, LVIR_BOUNDS);
    rcItem.right = rcClient.right;
    InvalidateRect(&rcItem);

    *pResult = 0;
}

// 排序回调函数
int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    STRUCT_SORT_INFO* pSortInfo = (STRUCT_SORT_INFO*)lParamSort;
    CFListCtrl* pListCtrl = pSortInfo->pListCtrl;
    int subItem = pSortInfo->nSubItem;
    int sortStyle = pSortInfo->nSortMode;

    if (subItem < 0)
    {
        return 0;
    }

    CString strItem1 = pListCtrl->GetItemText(lParam1, subItem);
    CString strItem2 = pListCtrl->GetItemText(lParam2, subItem);

    if (LIST_SORT_DESCENDING == sortStyle || LIST_SORT_YES == sortStyle)
    {
        return strItem2.Compare(strItem1);
        
    }
    else if (LIST_SORT_ASCENDING == sortStyle)
    {
        return strItem1.Compare(strItem2);
    }

    return 0;
}

// 点击排序
void CFListCtrl::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // 不支持排序
    if (LIST_SORT_NO == m_nSortVector[pNMLV->iSubItem])
    {
        return;
    }
    // 设置排序方式
    if (LIST_SORT_YES == m_nSortVector[pNMLV->iSubItem])
    {
        m_listHeader.SetSort(pNMLV->iSubItem, LIST_SORT_DESCENDING);
        m_nSortVector[pNMLV->iSubItem] = LIST_SORT_DESCENDING;
    }
    else if (LIST_SORT_DESCENDING == m_nSortVector[pNMLV->iSubItem])
    {
        m_listHeader.SetSort(pNMLV->iSubItem, LIST_SORT_ASCENDING);
        m_nSortVector[pNMLV->iSubItem] = LIST_SORT_ASCENDING;
    }
    else if (LIST_SORT_ASCENDING == m_nSortVector[pNMLV->iSubItem])
    {
        m_listHeader.SetSort(pNMLV->iSubItem, LIST_SORT_DESCENDING);
        m_nSortVector[pNMLV->iSubItem] = LIST_SORT_DESCENDING;
    }

    // 需要setitemdata才能排序
    for (int i = 0; i < GetItemCount(); i++)
    {
        SetItemData(i, i);
    }

    // 把列表指针和点击的表头项传入排序回调函数
    STRUCT_SORT_INFO sortInfo;
    sortInfo.pListCtrl = this;
    sortInfo.nSubItem = pNMLV->iSubItem;
    sortInfo.nSortMode = m_nSortVector[pNMLV->iSubItem];

    // 存在checkbox，表头项减1
    if (m_isCheckBox)
    {
        sortInfo.nSubItem -= 1;
    }
    SortItems(CompareFunc, (DWORD)&sortInfo);

    // 排序完成后，调整checkbox的位置
    std::list<CFButton*> listTmp(m_listCheckBox);
    for (int i = 0; i < (int)m_listCheckBox.size(); i++)
    {
        // itemData和项数一样则不需要调整
        int itemData = GetItemData(i);
        if(i == itemData)
        {
            continue;
        }

        // 根据itemData对应的checkbox，重新设置checkbox列表
        std::list<CFButton*>::iterator itor = m_listCheckBox.begin();
        std::list<CFButton*>::iterator itorTmp = listTmp.begin();
        advance(itor, i);
        advance(itorTmp, itemData);
        *itor = *itorTmp;
    }

    // 排序完成后，调整按钮的位置
    std::map<std::pair<int, int>, CFButton*> mapTmp(m_mapButton);
    std::map<std::pair<int, int>, CFButton*>().swap(m_mapButton);
    for (int i = 0; i < GetItemCount(); i++)
    {
        int itemData = GetItemData(i);

        for(int j = 0; j < GetHeaderCtrl()->GetItemCount(); j++)
        {
            // 根据itemData重新设置按钮的索引（原来的map已清空）
            std::map<std::pair<int, int>, CFButton*>::iterator itFind =
                mapTmp.find(std::make_pair(itemData, j));
            if (itFind == mapTmp.end())
            {
                continue;
            }

            CFButton* temp = itFind->second;
            m_mapButton.insert(std::make_pair(std::make_pair(i, j), temp));
        }
    }

    *pResult = 0;
}

// stores current column width on the beginning of divider dragging process
// and sets a flag that indicates that dragging process is in progress
void CFListCtrl::OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    if ((phdr->pitem) != 0 && (phdr->pitem->mask & HDI_WIDTH) != 0) {
        // prevent resizing the last column
        if (phdr->iItem == GetLastColumnIndex()) {
            *pResult = 1;
            return;
        }
        // save current width of the column being resized
        m_oldColumnWidth = phdr->pitem->cxy;
        m_trackingHeaderDivider = TRUE;
    }
    *pResult = 0;
}

// prevents flickering scrollbar during divider dragging and adjusts last column width
void CFListCtrl::OnHdnItemchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    if ((phdr->pitem) != 0 && (phdr->pitem->mask & HDI_WIDTH) != 0 && m_trackingHeaderDivider)
    {
        int lastColumnIndex = GetLastColumnIndex();
        // if resizing any column except the last one...
        if (phdr->iItem < lastColumnIndex)
        {
            SetRedraw(FALSE);
            int newWidth = phdr->pitem->cxy;
            // if column is being widened, correct width of the last column
            // to avoid flickering horizontal scrollbar
            if (newWidth > m_oldColumnWidth)
            {
                int lastColumnWidth = GetColumnWidth(lastColumnIndex) - newWidth + m_oldColumnWidth;
                SetColumnWidth(lastColumnIndex, lastColumnWidth);
            }
            // if column is narrowed, set LVSCW_AUTOSIZE_USEHEADER for the last column
            else
                SetColumnWidth(lastColumnIndex, LVSCW_AUTOSIZE_USEHEADER);
            // store new width of the column
            m_oldColumnWidth = newWidth;
        }
        else
        {
            // all columns have been resized, so redraw the control
            SetRedraw(TRUE);
            Invalidate();
            UpdateWindow();
        }
    }
    *pResult = 0;
}

// readjust column widths if HDS_FULLDARG style is not set
void CFListCtrl::OnHdnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    if ((phdr->pitem) != 0 && (phdr->pitem->mask & HDI_WIDTH) != 0 && m_trackingHeaderDivider == FALSE)
    {
        int lastColumnIndex = GetLastColumnIndex();
        // if any column except the last one was resized
        if (phdr->iItem < lastColumnIndex)
        {
            SetRedraw(FALSE);
            SetColumnWidth(lastColumnIndex, LVSCW_AUTOSIZE_USEHEADER);
            SetRedraw(TRUE);
            Invalidate();
            UpdateWindow();
        }
    }
    *pResult = 0;
}

// resets flag that indicates divider is going on
void CFListCtrl::OnHdnEndtrack(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

    // 控制表头最小宽度
    if (phdr->pitem->cxy < HEAD_WIDTH_MIN)
    {
        phdr->pitem->cxy = HEAD_WIDTH_MIN;
    }

    m_trackingHeaderDivider = FALSE;
    *pResult = 0;
}

BOOL CFListCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
    // 当表头宽度小于按钮宽度时，刷新按钮所在项
    for (int i = 0; i < GetItemCount(); i++)
    {
        if (i < m_nScrollPos && i >= m_nScrollPos + m_nScrollPage)
            continue;

        for(int j = 0; j < GetHeaderCtrl()->GetItemCount(); j++)
        {
            std::map<std::pair<int, int>, CFButton*>::iterator itFind =
                m_mapButton.find(std::make_pair(i, j));
            if (itFind != m_mapButton.end())
            {
                CRect rcButton;
                itFind->second->GetClientRect(rcButton);

                static int buttonPreWidth = 0;
                if (rcButton.Width() < BUTTON_WIDTH && buttonPreWidth != rcButton.Width())
                {
                    CRect rcItem, rcClient;
                    GetClientRect(&rcClient);
                    if (m_isCheckBox)
                    {
                        GetSubItemRect(i, j + 1, LVIR_BOUNDS, rcItem);
                    }
                    else
                    {
                        GetSubItemRect(i, j, LVIR_BOUNDS, rcItem);
                    }
                    rcItem.right = rcClient.right;

                    buttonPreWidth = rcButton.Width();
                    InvalidateRect(rcItem);
                }
            }
        }
    }

    // 禁止擦除背景，减少闪烁
    return TRUE;
}

int CFListCtrl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat /*= LVCFMT_LEFT*/, int nWidth /*= -1*/, int nSubItem /*= -1*/)
{
    // 添加锁定和排序开关
    m_listHeader.AddLock();
    m_listHeader.AddSort();
    m_nSortVector.push_back(LIST_SORT_NO);
    if (m_isCheckBox)
        nCol++;

    return CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
}

void CFListCtrl::BeforeInsert()
{
    CRect rcClient, rcSelItem;
    GetClientRect(&rcClient);

    // 刷新所有选中项和前一项，解决右边非客户区不重绘的问题
    POSITION nSelPos = GetFirstSelectedItemPosition();
    while(nSelPos)
    {
        int nSelItem = GetNextSelectedItem(nSelPos);

        GetItemRect(nSelItem, rcSelItem, LVIR_BOUNDS);
        rcSelItem.right = rcClient.right;
        InvalidateRect(rcSelItem);

        GetItemRect(nSelItem + 1, rcSelItem, LVIR_BOUNDS);
        rcSelItem.right = rcClient.right;
        InvalidateRect(rcSelItem);
    }
}

int  CFListCtrl::InsertItem(const LVITEM* pItem)
{
    return CListCtrl::InsertItem(pItem);
}

int CFListCtrl::InsertItem(int nItem, LPCTSTR lpszItem)
{
    if (m_isCheckBox)
    {
        InsertCheckBox(nItem);
    }

    BeforeInsert();

    return CListCtrl::InsertItem(nItem, lpszItem, -1);
}

int CFListCtrl::InsertItem(int nItem, LPCTSTR lpszItem, int nImage)
{
    if (m_isCheckBox)
    {
        InsertCheckBox(nItem);
    }

    BeforeInsert();

    return CListCtrl::InsertItem(nItem, lpszItem, nImage);
}

void CFListCtrl::InsertCheckBox(int nItem)
{
    if (nItem > (int)m_listCheckBox.size())
    {
        nItem = m_listCheckBox.size();
    }

    // 添加checkbox插入list
    CFButton* pCheckBox;
    pCheckBox = new CFButton;
    pCheckBox->Create(_T(""), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        CRect(0, 0, 0, 0), this, IDC_LIST_CHECKBOX_BEGIN);
    std::list<CFButton*>::iterator itor = m_listCheckBox.begin();
    pCheckBox->SetObjectName(_T("cbtn"));
    pCheckBox->SetCheckBox(TRUE);
    pCheckBox->SetInListCtrl(TRUE);
    pCheckBox->MoveWindow(0,0,0,0);
    advance(itor, nItem);
    m_listCheckBox.insert(itor, pCheckBox);
}

int CFListCtrl::DeleteItem(int nItem)
{
    if (nItem >= GetItemCount())
    {
        return -1;
    }

    if (m_isCheckBox && nItem < (int)m_listCheckBox.size())
    {
        // 释放checkbox
        if (m_isCheckBox)
        {
            std::list<CFButton*>::iterator itor = m_listCheckBox.begin();
            advance(itor, nItem);
            if (*itor)
                delete (*itor);
            m_listCheckBox.erase(itor);
        }
    }

    for (int i = 0; i < GetHeaderCtrl()->GetItemCount(); i++)
    {
        // 释放按钮
        std::map<std::pair<int, int>, CFButton*>::iterator itLast =
            m_mapButton.find(std::make_pair(nItem, i));
        if (itLast != m_mapButton.end())
        {
            if (itLast->second)
                delete (itLast->second);
            m_mapButton.erase(itLast);
        }
    }

    std::map<std::pair<int, int>, CFButton*>::iterator itor = m_mapButton.begin();
    while(itor != m_mapButton.end())
    {
        if( (itor->first).first > nItem)
        {
            m_mapButton.insert(itor, std::make_pair(std::make_pair
                (itor->first.first - 1, itor->first.second), itor->second));
            itor = m_mapButton.erase(itor);
        }
		else
		{
			itor++;
		}
    }

    // 刷新删除项以下的所有项
    CRect rcInval;
    GetClientRect(&rcInval);
    rcInval.top = nItem * m_nHeight + GetHeadHeight();
    InvalidateRect(&rcInval);

    return CListCtrl::DeleteItem(nItem);
}

BOOL CFListCtrl::DeleteAllItems()
{
    if (m_listCheckBox.size())
    {
        std::list<CFButton*>::iterator itor = m_listCheckBox.begin();
        while(itor != m_listCheckBox.end())
        {
            delete (*itor);
            itor = m_listCheckBox.erase(itor);
        }
    }

    if (m_mapButton.size())
    {
        std::map<std::pair<int, int>, CFButton*>::iterator itor = m_mapButton.begin();
        while(itor != m_mapButton.end())
        {
            delete itor->second;
            itor = m_mapButton.erase(itor);
        }
    }

    return CListCtrl::DeleteAllItems();
}


// 设置checkbox
void CFListCtrl::SetCheckBox(bool bCreate /*= true*/)
{
    if (bCreate && !m_isHeadCreateBox)
    {
        InsertColumn(0, _T("CheckBox"), LVCFMT_CENTER, ITEM_CHECKBOX_WIDTH);
        SetHeadLock(0, true);
    }
    m_isCheckBox = bCreate;
    m_listHeader.SetCheckBox(bCreate);
}

// 设置是否选中checkbox
void CFListCtrl::SetItemChecked(int nItem, int nChecked)
{
    if (!m_isCheckBox || m_listCheckBox.size() < nItem + 1)
    {
        return ;
    }

    std::list<CFButton*>::iterator itor = m_listCheckBox.begin();
    advance(itor, nItem);
    if (*itor && (*itor)->GetCheck() != nChecked)
    {
        (*itor)->SetCheck(nChecked);
    }
}

bool CFListCtrl::IsItemChecked(int nItem)
{
    if (!m_isCheckBox)
    {
        return false;
    }

    std::list<CFButton*>::iterator itor = m_listCheckBox.begin();
    advance(itor, nItem);
    if (*itor)
    {
        return ((*itor)->GetCheck() == 0 ? false : true);
    }

    return false;
}

// 是否锁定列表头, nItem为-1时对所有项生效
void CFListCtrl::SetHeadLock(int nItem, bool isLock /*= true*/)
{
    if (-1 == nItem)
    {
        m_listHeader.SetLock(nItem , isLock);
        return;
    }

    if (m_isCheckBox)
    {
        nItem++;
    }
    m_listHeader.SetLock(nItem , isLock);
}

// 列表头能否排序
void CFListCtrl::SetHeadSort(int nItem, bool bSort /*= true*/)
{
    if (m_isCheckBox)
    {
        nItem++;
    }

    if (bSort)
    {
        m_listHeader.SetSort(nItem, LIST_SORT_YES);
        m_nSortVector[nItem] = LIST_SORT_YES;
    }
    else
    {
        m_listHeader.SetSort(nItem, LIST_SORT_NO);
        m_nSortVector[nItem] = LIST_SORT_NO;
    }
}

// 设置图标列表
int CFListCtrl::SetImageList(LPCTSTR lpszImagePath)
{
    Image* pImage = CResMgr::GetInstance()->GetRes(lpszImagePath);
    if (!pImage)
        return -1;

    RectF RectDest(0, 0, BMPIMAGE_SIZE, BMPIMAGE_SIZE);

    // 新建位图
    Bitmap* pBitmap = new Bitmap(BMPIMAGE_SIZE, BMPIMAGE_SIZE);
    if (!pBitmap)
        return -1;

    Graphics* graphics = Graphics::FromImage(pBitmap);
    if (!graphics)
    {
        delete pBitmap;
        return -1;
    }

    // 画位图
    graphics->SetInterpolationMode(InterpolationModeHighQuality);
    graphics->SetSmoothingMode(SmoothingModeHighQuality);
    graphics->DrawImage(pImage, RectDest, 0, 0, (REAL)pImage->GetWidth(),
        (REAL)pImage->GetHeight(), UnitPixel, NULL, NULL, NULL);

    // 把图添加到图标列表中
    HBITMAP hBitmap = NULL;
    CBitmap bmp;
    pBitmap->GetHBITMAP(Color(0, 0, 0), &hBitmap);
    bmp.Attach(hBitmap);
    int nImage = m_imgList.Add(&bmp, RGB(0, 0, 0));

    bmp.Detach();
    delete pBitmap;

    return nImage;
}

// 设置项
BOOL CFListCtrl::SetItem(const LVITEM* pItem)
{
    return CListCtrl::SetItem(pItem);
}

BOOL CFListCtrl::SetItem(int nItem, int nSubItem, LPCTSTR lpszItem, int nImage)
{
    return CListCtrl::SetItem(nItem, nSubItem, LVIF_IMAGE | LVIF_TEXT,
        lpszItem, nImage, 0, 0, NULL);
}

BOOL CFListCtrl::SetButtonText(int nItem, int nSubItem, LPCTSTR lpszButton)
{
    if (nItem >= GetItemCount())
    {
        return FALSE;
    }

    std::map<std::pair<int, int>, CFButton*>::iterator itFind =
        m_mapButton.find(std::make_pair(nItem, nSubItem));
    if (itFind == m_mapButton.end())
    {
        return FALSE;
    }
    CFButton* pButton = m_mapButton[std::make_pair(nItem, nSubItem)];
    if (NULL == pButton)
    {
        return FALSE;
    }

    pButton->SetWindowText(lpszButton);
    return TRUE;
}

// 设置按钮, nID用于响应按钮点击
BOOL CFListCtrl::SetButton(int nItem, int nSubItem, LPCTSTR lpszButton, UINT nID)
{
    if (nItem >= GetItemCount())
    {
        return FALSE;
    }

    // 添加按钮插入map
    CFButton* pButton;
    pButton = new CFButton;
    pButton->Create(lpszButton, WS_CHILD | WS_VISIBLE,
        CRect(0, 0, 0, 0), this, IDC_LIST_BUTTON_BEGIN + nID);
    pButton->SetObjectName(_T("btn"));
    pButton->SetTextAlign(DT_CENTER);
    pButton->SetHoverCursor(LoadCursor(NULL, IDC_HAND));
    pButton->SetTextColor(FUi::Normal, FUi::White);
    pButton->MoveWindow(0,0,0,0);

    // 判断该位置是否已经存在按钮
    // 不存在直接插入
    std::map<std::pair<int, int>, CFButton*>::iterator itFind =
        m_mapButton.find(std::make_pair(nItem, nSubItem));
    if (itFind == m_mapButton.end())
    {
        m_mapButton.insert(std::make_pair(std::make_pair(nItem, nSubItem), pButton));
        return TRUE;
    }

    // 存在则把后面的所有按钮推后,然后插入
    std::map<std::pair<int, int>, CFButton*>::iterator itorBegin = m_mapButton.begin();
    std::map<std::pair<int, int>, CFButton*>::iterator itor = m_mapButton.end();
    advance(itorBegin, nItem);

    while (itor != itorBegin)
    {
        itor--;
        m_mapButton[std::make_pair(itor->first.first + 1, nSubItem)] =
            m_mapButton[std::make_pair(itor->first.first, nSubItem)];
    }
    m_mapButton[std::make_pair(nItem, nSubItem)] = pButton;

    return TRUE;
}

// 多按钮无文字
BOOL CFListCtrl::SetButton(int nSubItem, int nBtnNum, LPCTSTR lpszImage, ...)
{
    if (nSubItem >= m_listHeader.GetItemCount())
    {
        return FALSE;
    }
    m_nMultiBtnSub = nSubItem;

    va_list btnList;
    LPCTSTR lpszBtn = lpszImage;
    va_start(btnList, lpszImage);
    for (int i = 0; i < nBtnNum; i++)
    {
        Image* pImg = CResMgr::GetInstance()->GetRes(lpszBtn);
        if (pImg)
        {
            m_vecBtnImage.push_back(pImg);
        }
        lpszBtn = va_arg(btnList, LPCTSTR);
    }
    va_end(btnList);

    return TRUE;
}

void CFListCtrl::SetStyle(int nStyle)
{
    // 列表样式：FLS_ZEBRA-斑马纹，FLS_LINE-线性
    if (FLS_ZEBRA != nStyle && FLS_LINE != nStyle && FLS_BLACK != nStyle)
    {
        return;
    }

    m_nStyle = nStyle;
}

// 响应表头选中checkbox的消息，wParam为checkbox状态
LRESULT CFListCtrl::OnHeadCtrlCheckBox(WPARAM wParam, LPARAM /*lParam*/)
{
    // 改变全部checkbox
    for (int i = 0; i < (int)m_listCheckBox.size(); i++)
    {
        std::list<CFButton*>::iterator itor = m_listCheckBox.begin();
        advance(itor, i);
        (*itor)->SetCheck(int(wParam));

        if (int(wParam))
        {
            SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
        }
        else
        {
            SetItemState(i, ~LVIS_SELECTED, LVIS_SELECTED);
        }

        SetSelectionMark(i);
    }

    return TRUE;
}

LRESULT CFListCtrl::OnListItemCheckBox(WPARAM wParam, LPARAM /*lParam*/)
{
    if (int(wParam))
    {
        SetItemState(m_nItem, LVIS_SELECTED, LVIS_SELECTED);
    }
    else
    {
        SetItemState(m_nItem, ~LVIS_SELECTED, LVIS_SELECTED);
    }

    SetSelectionMark(m_nItem);

    return TRUE;
}

void CFListCtrl::OnBtnClick(UINT nID)
{
    // 模拟按钮点击消息
    CWnd* pParent = GetParent();
    if (m_pActrueParent)
    {
        pParent = m_pActrueParent;
    }
    ::PostMessage(pParent->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(nID - IDC_LIST_BUTTON_BEGIN, BN_CLICKED), NULL);
}

void CFListCtrl::SetHeadHeight(int nHeight)
{
    m_listHeader.SetHeight(nHeight);
}

int CFListCtrl::GetHeadHeight()
{
    return m_listHeader.GetHeight();
}

void CFListCtrl::SetHeadBgColor(COLORREF colorBg)
{
    m_listHeader.SetBgColor(colorBg);
}

void CFListCtrl::SetHeight(int nHeight)
{
    m_nHeight = nHeight;

    CRect rcWin;
    GetWindowRect(&rcWin);
    WINDOWPOS wp;
    wp.hwnd = m_hWnd;
    wp.cx = rcWin.Width();
    wp.cy = rcWin.Height();
    wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
    SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}

LRESULT CFListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        {
            // 因为按键消息 暂时没有找到好的办法移动控件窗口 就关闭按键吧
            return 0;
        }
        break;
    case WM_MOUSEMOVE:
        {
            if((m_nItem != -1) && (m_nSubItem != -1))
            {
                MSG msg;  
                memset(&msg, 0, sizeof(msg));
                msg.hwnd = GetSafeHwnd();  
                msg.message = message;  
                msg.wParam = wParam;  
                msg.lParam = lParam;  
                m_toolTip.RelayEvent(&msg);
            }
        }
        break;
    case WM_VSCROLL:
    case WM_HSCROLL:
        {
            WORD sbCode = LOWORD(wParam);
            int nPos = HIWORD(wParam);

            if (SB_THUMBTRACK == sbCode || SB_THUMBPOSITION == sbCode)
            {
                SCROLLINFO siv = {0};
                siv.cbSize = sizeof(SCROLLINFO);
                siv.fMask = SIF_ALL;
                SCROLLINFO sih = siv;

                CRect rcClient;
                GetClientRect(&rcClient);
                GetScrollInfo(SB_VERT,&siv);
                GetScrollInfo(SB_HORZ,&sih);

                SIZE sizeAll;
                if (0 == sih.nPage)
                    sizeAll.cx = rcClient.right;
                else
                    sizeAll.cx = rcClient.right * (sih.nMax + 1) / sih.nPage;

                if (0 == siv.nPage)
                    sizeAll.cy = rcClient.bottom;
                else
                    sizeAll.cy = rcClient.bottom * (siv.nMax + 1) / siv.nPage ;

                SIZE size = {0,0};
                if (WM_VSCROLL == message)
                {
                    size.cx = sizeAll.cx * sih.nPos / (sih.nMax + 1);
                    size.cy = sizeAll.cy * (nPos - siv.nPos) / (siv.nMax + 1);
                }
                else
                {
                    size.cx = sizeAll.cx * (nPos - sih.nPos) / (sih.nMax + 1);
                    size.cy = sizeAll.cy * siv.nPos / (siv.nMax + 1);
                }

                Scroll(size);
                return 1;
            }
        }
        break;
    default:
        break;
    }

	return CListCtrl::WindowProc(message, wParam, lParam);
}

void CFListCtrl::SetToolTipEnable(bool bValue)
{
	m_bEnableToolTip = bValue;
}

BOOL CFListCtrl::SetButtonToolTip(int nBtnNum, LPCTSTR strToolTip, ...)
{
	va_list btnToolTipList;
	LPCTSTR strBtn = strToolTip;
	va_start(btnToolTipList, strToolTip);
	for (int i = 0; i < nBtnNum; i++)
	{
		CString strToolTip = strBtn;
		m_vecBtnToolTip.push_back(strToolTip);
		strBtn = va_arg(btnToolTipList, LPCTSTR);
	}
	va_end(btnToolTipList);

	return TRUE;
}

void CFListCtrl::SetMutibtnSize(int nHeight, int nWidth)
{
	m_nMultiBtnHeight = nHeight;
	if (-1 == nWidth)
	{
		m_nMultiBtnWidth = m_nMultiBtnHeight;
	}
	else
	{
		m_nMultiBtnWidth = nWidth;
	}
}