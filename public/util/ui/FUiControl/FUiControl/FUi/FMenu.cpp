// FListCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FMenu.h"
#include "fuilib.h"


#define ICON_SIZE           24  // ͼ��Ĵ�С
#define MENU_PADDING        10  // �˵��������յĴ�С
#define SEPARATOR_HEIGHT    1   // �ָ���ĸ߶�
#define MENU_BORDER         1   // �˵��߿�

HHOOK g_hMenuHook;

// �ɵĴ��ڹ���
WNDPROC g_OldWndProc = NULL;

// �Զ���Ĵ��ڹ���
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
    switch(message) 
    { 
    case WM_CREATE: 
        { 
            // ����Ҫȥ���˵����ڵ� WS_BORDER ���� WS_EX_DLGMODALFRAME, WS_EX_WINDOWEDGE ��չ���
            DWORD dwStyle = ::GetWindowLong(hWnd ,GWL_STYLE); 
            DWORD dwNewStyle =   (dwStyle & ~WS_BORDER); 
            ::SetWindowLong(hWnd ,GWL_STYLE ,dwNewStyle); 
            DWORD dwExStyle = ::GetWindowLong(hWnd ,GWL_EXSTYLE); 
            DWORD dwNewExStyle = (dwExStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE)); 
            ::SetWindowLong(hWnd ,GWL_EXSTYLE, dwNewExStyle); 

            break;
        }
    case WM_PRINT:   
        {
            // ������ֹ�ǿͻ����Ļ���
            return CallWindowProc(g_OldWndProc, hWnd, WM_PRINTCLIENT, wParam,lParam); 
        }
    case WM_ERASEBKGND:
        return TRUE;
    case WM_WINDOWPOSCHANGING: 
        { 
            // ���, ��Ϊ����MeasureItem��ָ���Ĳ˵���С��ϵͳ���Զ�������ϱ߿����Ǳ���ȥ���ⲿ�ֶ���ĳߴ�
            LPWINDOWPOS lpPos = (LPWINDOWPOS)lParam; 

            lpPos->cx -= 2 * GetSystemMetrics(SM_CYBORDER) + 4; 
            lpPos->cy -= 2 * GetSystemMetrics(SM_CYBORDER) + 4; 

            break;
        }
    }

    return CallWindowProc(g_OldWndProc, hWnd, message, wParam, lParam); 
}
// ���Ӻ���
LRESULT CALLBACK WindowsHook(int code, WPARAM wParam, LPARAM lParam)
{
    PCWPSTRUCT lpCwp = (PCWPSTRUCT)lParam;

    // ��׽�˵����� WM_CREATE, 0x01E2
    while(code == HC_ACTION && lpCwp->message == WM_CREATE) 
    { 
        TCHAR szClassName[10] = {0}; 
        int Count =::GetClassName(lpCwp->hwnd, szClassName, sizeof(szClassName) / sizeof(szClassName[0]));

        // ����Ƿ�˵����� 
        if (Count == 6 && _tcscmp(szClassName, _T("#32768")) == 0) 
        { 
            WNDPROC lastWndProc = (WNDPROC)GetWindowLong(lpCwp->hwnd, GWL_WNDPROC); 
            if(lastWndProc != WndProc) 
            { 
#pragma warning(disable:4311)
                // �滻�˵����ڹ��̺���
                SetWindowLong(lpCwp->hwnd, GWL_WNDPROC, (LONG)WndProc);
                // �����ɵĴ��ڹ���
                g_OldWndProc = lastWndProc; 
                break;
            } 
        } 
        break; 
    }

    return CallNextHookEx(g_hMenuHook, code, wParam, lParam); 
}


IMPLEMENT_DYNAMIC(CFMenu, CMenu)

CFMenu::CFMenu()
: m_nItemHeight(28)
, m_clrText(FUi::RGB_102)
, m_clrGrayText(FUi::Red)
, m_clrHoverText(FUi::White)
, m_clrBorder(FUi::RGB_213)
, m_pImgBkg(NULL)
, m_strFontName(YAHEI_18_N)
{
}

CFMenu::~CFMenu()
{
    try
    {
        DestoryMenu();

        for (std::map<UINT_PTR, ITEM_DATA*>::iterator itr = m_mapItemData.begin();
            itr != m_mapItemData.end(); ++itr)
        {
            ITEM_DATA *pItemData = itr->second;
            if (pItemData)
            {
                delete pItemData;
            }
        }
        m_mapItemData.clear();

        m_pImgBkg = NULL;
    }
    catch (...)
    {
    }
}

// CFMenu ��Ϣ�������
void CFMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
    int nItemWidth = 100;
    int nItemHeight = m_nItemHeight;
    ITEM_DATA *pItemData = (ITEM_DATA*)lpMIS->itemData;
    if (pItemData)
    {
        // ����Item�Ŀ��
        if (pItemData->nFlags & MF_SEPARATOR)
        {
            nItemHeight = SEPARATOR_HEIGHT;
        }
        else
        {
            CString strText = pItemData->strText;
            CDC *pDC = CWnd::GetDesktopWindow()->GetDC();
            if (pDC)
            {
                CRect rcText;
                CFont *pOld = pDC->SelectObject(RenderEngine->GetFont(m_strFontName));
                pDC->DrawText(strText, rcText, DT_CALCRECT | DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                nItemWidth = rcText.Width();

                pDC->SelectObject(pOld);
                CWnd::GetDesktopWindow()->ReleaseDC(pDC);
            }
        }
    }

    lpMIS->itemHeight = nItemHeight;
    lpMIS->itemWidth = nItemWidth + ICON_SIZE + 2 * MENU_PADDING + 12;
}

void CFMenu::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
    if (NULL == lpDIS || ODT_MENU != lpDIS->CtlType)
    {
        return ;
    }
    CDC *pDC = CDC::FromHandle(lpDIS->hDC);
    if (NULL == pDC)
    {
        return ;
    }
    ITEM_DATA *pItemData = (ITEM_DATA*)lpDIS->itemData;
    if (NULL == pItemData)
    {
        return ;
    }

    /*
    itemAction
    Defines the drawing action required. This will be one or more of the following bits:

    ODA_DRAWENTIRE   This bit is set when the entire control needs to be drawn.
    ODA_FOCUS   This bit is set when the control gains or loses input focus. 
                The itemState member should be checked to determine whether the control has focus.
    ODA_SELECT  This bit is set when only the selection status has changed. 
                The itemState member should be checked to determine the new selection state.
    */

    /*
    itemState
    Specifies the visual state of the item after the current drawing action takes place. 
    That is, if a menu item is to be dimmed, the state flag ODS_GRAYED will be set. The state flags are as follows:

    -ODS_CHECKED   This bit is set if the menu item is to be checked. This bit is used only in a menu.
    -ODS_DISABLED   This bit is set if the item is to be drawn as disabled.
    -ODS_FOCUS   This bit is set if the item has input focus.
    ODS_GRAYED   This bit is set if the item is to be dimmed. This bit is used only in a menu.
    ODS_SELECTED   This bit is set if the item's status is selected.
    -ODS_COMBOBOXEDIT   The drawing takes place in the selection field (edit control) of an ownerdrawn combo box.
    -ODS_DEFAULT   The item is the default item.

    ǰ��� - �ı�ʾ����û�д���
    */

    CRect rcItem(lpDIS->rcItem);
    RectF rcfItem;
    Rect2RectF(lpDIS->rcItem, rcfItem);

    COLORREF clrText(m_clrText);
    int nOffset = 0;

    // ��ͼ��������ɫ
    if (lpDIS->itemState & ODS_SELECTED)
    {
        nOffset = 1;
        clrText = m_clrHoverText;
    }
    else if (lpDIS->itemState & ODS_GRAYED)
    {
        // �Ƿ��û�
        clrText = m_clrGrayText;
    }
    // �ų���ͷ���򣬷�ֹϵͳ���ƺ󸲸ǵ�
    //pDC->ExcludeClipRect(rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);

    // ����
    //lint -e613
    RenderEngine->DrawImageX(pDC, m_pImgBkg, rcfItem, 0, (REAL)nOffset * m_pImgBkg->GetHeight() / 2,
        (REAL)m_pImgBkg->GetWidth(), (REAL)m_pImgBkg->GetHeight() / 2 - 1);
    //lint +e613

    // ��ͷ
    if (pItemData->nFlags & MF_POPUP)
    {
        CRect rcArrow(rcItem);
        rcArrow.left = rcArrow.right - MENU_PADDING - 6;
        rcArrow.right -= MENU_BORDER;
    }

    // �ָ���
    if (pItemData->nFlags & MF_SEPARATOR)
    {
        CPen penSeparator;
        penSeparator.CreatePen(PS_DOT, 1, m_clrBorder);
        pDC->SelectObject(&penSeparator);
        pDC->MoveTo(rcItem.left + 3, rcItem.top);
        pDC->LineTo(rcItem.right - 3, rcItem.top);
        penSeparator.DeleteObject();
    }

    // ͼ��
    if (!pItemData->strIcon.IsEmpty())
    {
        CString strIconName = pItemData->strIcon;
        Image *pImg = CResMgr::GetInstance()->GetRes(strIconName);
        // ���û���ҵ� _hover����Դ����ô��ʾĬ�ϵ��Ǹ�

        if (pImg)
        {
            nOffset = (lpDIS->itemState & ODS_SELECTED)?1:0;
            int nTop = rcItem.top + (rcItem.Height() - pImg->GetHeight() / 2) / 2;
            RectF rcfImg((REAL)MENU_PADDING, (REAL)nTop, (REAL)pImg->GetWidth(),(REAL)pImg->GetHeight() / 2);

            RenderEngine->DrawImageX(pDC, pImg, rcfImg, 0, (REAL)nOffset * pImg->GetHeight() / 2, 
                (REAL)pImg->GetWidth(), (REAL)pImg->GetHeight() / 2);
        }
    }
    
    // ����
    CRect rc(lpDIS->rcItem);
    rc.left += (MENU_PADDING + ICON_SIZE + 6);

    CFont *pOld = pDC->SelectObject(RenderEngine->GetFont(m_strFontName));

    int nFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
    RenderEngine->DrawTextX(pDC, pItemData->strText, rc, clrText, nFormat);
    pDC->SelectObject(pOld);

    // �߿�
    int nSeparatorCount = GetSeparatorCount();
    CRect rcClient(0, 0, rcItem.right, 0);
    rcClient.bottom = m_nItemHeight * (GetMenuItemCount() - nSeparatorCount) + SEPARATOR_HEIGHT * nSeparatorCount;

    CBrush brBorder;
    brBorder.CreateSolidBrush(m_clrBorder);
    pDC->FrameRect(rcClient, &brBorder);
    brBorder.DeleteObject();
}

void CFMenu::SetObjectName(LPCTSTR szObjectName)
{
    Image* pImgBkg = CResMgr::GetInstance()->GetRes(szObjectName);
    if (NULL == pImgBkg)
    {
        return ;
    }

    m_strObjectName = szObjectName;
    m_pImgBkg = pImgBkg;
}

void CFMenu::SetItemHeight(int nItemHeight)
{
    m_nItemHeight = nItemHeight;
}

BOOL CFMenu::AddMenu(UINT nFlags, UINT_PTR nIDNewItem, LPCTSTR lpszNewItem, LPCTSTR lpszIcon)
{
    ITEM_DATA *pItemData = new ITEM_DATA;
    pItemData->strText = lpszNewItem;
    pItemData->strIcon = lpszIcon;
    pItemData->nFlags = nFlags;

    // ����ظ������ˣ���ɾ��ԭ�ȵ�����
    if (m_mapItemData.find(nIDNewItem) != m_mapItemData.end())
    {
        delete m_mapItemData[nIDNewItem];
    }

    m_mapItemData[nIDNewItem] = pItemData;

    return AppendMenu(nFlags | MF_OWNERDRAW, nIDNewItem, (LPCTSTR)pItemData);
}

BOOL CFMenu::CreatePopupMenu()
{
    // ʹ�� Hook �Ƴ��˵��߿�
    g_hMenuHook = ::SetWindowsHookEx(WH_CALLWNDPROC, WindowsHook, AfxGetInstanceHandle(), ::GetCurrentThreadId());
    OutputDebugString(_T("Fui:Menu Hook\n"));

    return CMenu::CreatePopupMenu();
}

BOOL CFMenu::DestoryMenu()
{
    BOOL bRet = CMenu::DestroyMenu();

    UnhookWindowsHookEx(g_hMenuHook);
    OutputDebugString(_T("Fui:Menu UnHook\n"));

    return bRet;
}


int CFMenu::GetSeparatorCount()
{
    int nSeparatorCount = 0;
    for (std::map<UINT_PTR, ITEM_DATA*>::const_iterator itr = m_mapItemData.begin();
        itr != m_mapItemData.end(); ++itr)
    {
        ITEM_DATA *pItemData = itr->second;
        if (NULL == pItemData)
        {
            continue;
        }
        if (pItemData->nFlags & MF_SEPARATOR)
        {
            ++nSeparatorCount;
        }
    }
    return nSeparatorCount;
}

void CFMenu::SetTextFont(const CString& strFontName)
{
    m_strFontName = strFontName;
}

void CFMenu::SetTextColor(COLORREF clrText, COLORREF clrGrayText, COLORREF clrHoverText)
{
    m_clrText = clrText;
    m_clrGrayText = clrGrayText;
	m_clrHoverText = clrHoverText;
}

void CFMenu::SetBorderColor(COLORREF clrBorder)
{
    m_clrBorder = clrBorder;
}
