// MsgBoxDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MsgBoxDlg.h"
#include "fuilib.h"

// 提示文字区域的最大宽度
#define MAX_TEXT_WIDTH  400
// 两边留空大小
#define MARGIN_SIZE     50

// CMsgBoxDlg 对话框

IMPLEMENT_DYNAMIC(CMsgBoxDlg, CFDialog)

CMsgBoxDlg::CMsgBoxDlg(CWnd* pParent /*=NULL*/)
	: CFDialog(CMsgBoxDlg::IDD, pParent)
    , m_pImgIcon(NULL)
    , m_pImgBtnBkg(NULL)
    , m_uBtnType(MB_OK)
    , m_strTextFont(YAHEI_18_N)
{
    m_brBkg.CreateSolidBrush(FUi::White);
}

CMsgBoxDlg::~CMsgBoxDlg()
{
    SAFE_DELETE_BRUSH(m_brBkg);
}

void CMsgBoxDlg::DoDataExchange(CDataExchange* pDX)
{
    CFDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
    DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
    DDX_Control(pDX, IDC_BTN_YES, m_btnYes);
    DDX_Control(pDX, IDC_BTN_NO, m_btnNo);
    DDX_Control(pDX, IDC_BTN_RETRY, m_btnRetry);
}


BEGIN_MESSAGE_MAP(CMsgBoxDlg, CFDialog)
    ON_WM_PAINT()
    ON_COMMAND_RANGE(IDC_BTN_OK, IDC_BTN_RETRY, &CMsgBoxDlg::OnBtnClicked)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CMsgBoxDlg 消息处理程序

BOOL CMsgBoxDlg::OnInitDialog()
{
    CFDialog::OnInitDialog();

    InitDlg();

    return TRUE;
}

// 初始化对话框
void CMsgBoxDlg::InitDlg()
{
    SetWindowText(m_strCaption);

    m_pImgIcon = CResMgr::GetInstance()->GetRes(_T("msgbox_icon"));
    m_pImgBtnBkg = CResMgr::GetInstance()->GetRes(_T("msgbox_bo"));

    SetObjectName(_T("msgbox"));

    std::vector<int> vecMargin;
    vecMargin.push_back(1);
    vecMargin.push_back(34);
    vecMargin.push_back(1);
    vecMargin.push_back(1);
    SetMargin(vecMargin);

    // 尺寸
    CalcSize();

    // 按钮
    InitBtn();
    
    // 居中
    CenterWindow();
}

// 初始化按钮
void CMsgBoxDlg::InitBtn()
{
    int nCtrlState = FUi::Normal | FUi::Hovered | FUi::Pressed;

    m_btnYes.SetObjectName(_T("ok"));
    m_btnYes.SetWindowText(_T("是"));
    m_btnYes.SetTextAlign(DT_CENTER);
    m_btnYes.SetTextFont(nCtrlState, YAHEI_20_SB);
    m_btnYes.SetTextColor(nCtrlState, FUi::White);

    m_btnNo.SetObjectName(_T("cancel"));
    m_btnNo.SetWindowText(_T("否"));
    m_btnNo.SetTextAlign(DT_CENTER);
    m_btnNo.SetTextFont(nCtrlState, YAHEI_20_SB);
    m_btnNo.SetTextColor(nCtrlState, FUi::RGB_102);

    m_btnOK.SetObjectName(_T("ok"));
    m_btnOK.SetWindowText(_T("确定"));
    m_btnOK.SetTextAlign(DT_CENTER);
    m_btnOK.SetTextFont(nCtrlState, YAHEI_20_SB);
    m_btnOK.SetTextColor(nCtrlState, FUi::White);

    m_btnCancel.SetObjectName(_T("cancel"));
    m_btnCancel.SetWindowText(_T("取消"));
    m_btnCancel.SetTextAlign(DT_CENTER);
    m_btnCancel.SetTextFont(nCtrlState, YAHEI_20_SB);
    m_btnCancel.SetTextColor(nCtrlState, FUi::RGB_102);

    m_btnRetry.SetObjectName(_T("ok"));
    m_btnRetry.SetWindowText(_T("重试"));
    m_btnRetry.SetTextAlign(DT_CENTER);
    m_btnRetry.SetTextFont(nCtrlState, YAHEI_20_SB);
    m_btnRetry.SetTextColor(nCtrlState, FUi::White);

    // 按钮位置
    CRect rcClient;
    GetClientRect(rcClient);

    CRect rcBtn;
    m_btnOK.GetClientRect(rcBtn);

    if ((m_uBtnType & 0xF) == MB_YESNO)
    {
        m_btnYes.ShowWindow(SW_SHOW);
        m_btnNo.ShowWindow(SW_SHOW);
        m_btnOK.ShowWindow(SW_HIDE);
        m_btnCancel.ShowWindow(SW_HIDE);
        m_btnRetry.ShowWindow(SW_HIDE);

        m_btnNo.SetWindowPos(NULL, rcClient.right - (10 + rcBtn.Width()), 
            rcClient.bottom - 10 - rcBtn.Height(), 0, 0, SWP_NOSIZE);
        m_btnYes.SetWindowPos(NULL, rcClient.right - 2 * (10 + rcBtn.Width()), 
            rcClient.bottom - 10 - rcBtn.Height(), 0, 0, SWP_NOSIZE);
    }
    else if ((m_uBtnType & 0xF) == MB_OK)
    {
        m_btnYes.ShowWindow(SW_HIDE);
        m_btnNo.ShowWindow(SW_HIDE);
        m_btnOK.ShowWindow(SW_SHOW);
        m_btnCancel.ShowWindow(SW_HIDE);
        m_btnRetry.ShowWindow(SW_HIDE);

        m_btnOK.SetWindowPos(NULL, rcClient.right - (10 + rcBtn.Width()), 
            rcClient.bottom - 10 - rcBtn.Height(), 0, 0, SWP_NOSIZE);
    }
    else if ((m_uBtnType & 0xF) == MB_OKCANCEL)
    {
        m_btnYes.ShowWindow(SW_HIDE);
        m_btnNo.ShowWindow(SW_HIDE);
        m_btnOK.ShowWindow(SW_SHOW);
        m_btnCancel.ShowWindow(SW_SHOW);
        m_btnRetry.ShowWindow(SW_HIDE);

        m_btnCancel.SetWindowPos(NULL, rcClient.right - (10 + rcBtn.Width()), 
            rcClient.bottom - 10 - rcBtn.Height(), 0, 0, SWP_NOSIZE);
        m_btnOK.SetWindowPos(NULL, rcClient.right - 2 * (10 + rcBtn.Width()), 
            rcClient.bottom - 10 - rcBtn.Height(), 0, 0, SWP_NOSIZE);
    }
    else if ((m_uBtnType & 0xF) == MB_YESNOCANCEL)
    {
        m_btnYes.ShowWindow(SW_SHOW);
        m_btnNo.ShowWindow(SW_SHOW);
        m_btnOK.ShowWindow(SW_HIDE);
        m_btnCancel.ShowWindow(SW_SHOW);
        m_btnRetry.ShowWindow(SW_HIDE);

        m_btnCancel.SetWindowPos(NULL, rcClient.right - (10 + rcBtn.Width()), 
            rcClient.bottom - 10 - rcBtn.Height(), 0, 0, SWP_NOSIZE);
        m_btnNo.SetWindowPos(NULL, rcClient.right - 2 * (10 + rcBtn.Width()), 
            rcClient.bottom - 10 - rcBtn.Height(), 0, 0, SWP_NOSIZE);
        m_btnYes.SetWindowPos(NULL, rcClient.right - 3 * (10 + rcBtn.Width()), 
            rcClient.bottom - 10 - rcBtn.Height(), 0, 0, SWP_NOSIZE); 
    }
    else if ((m_uBtnType & 0xF) == MB_RETRYCANCEL)
    {
        m_btnYes.ShowWindow(SW_HIDE);
        m_btnNo.ShowWindow(SW_HIDE);
        m_btnOK.ShowWindow(SW_HIDE);
        m_btnCancel.ShowWindow(SW_SHOW);
        m_btnRetry.ShowWindow(SW_SHOW);

        m_btnCancel.SetWindowPos(NULL, rcClient.right - (10 + rcBtn.Width()), 
            rcClient.bottom - 10 - rcBtn.Height(), 0, 0, SWP_NOSIZE);
        m_btnRetry.SetWindowPos(NULL, rcClient.right - 2 * (10 + rcBtn.Width()), 
            rcClient.bottom - 10 - rcBtn.Height(), 0, 0, SWP_NOSIZE);
    }
    else
    {
        m_btnYes.ShowWindow(SW_HIDE);
        m_btnNo.ShowWindow(SW_HIDE);
        m_btnOK.ShowWindow(SW_SHOW);
        m_btnCancel.ShowWindow(SW_HIDE);
        m_btnRetry.ShowWindow(SW_HIDE);

        m_btnOK.SetWindowPos(NULL, rcClient.right - (10 + rcBtn.Width()), 
            rcClient.bottom - 10 - rcBtn.Height(), 0, 0, SWP_NOSIZE);
    }
}

// 对话框的大小
void CMsgBoxDlg::CalcSize()
{
    CDC *pDC = GetDC();
    CFont *pOld = pDC->SelectObject(RenderEngine->GetFont(m_strTextFont));

    // 文字单行显示时的尺寸
    CRect rcText(0, 0, 0, 0);
    pDC->DrawText(m_strText, &rcText, /*DT_SINGLELINE |*/ DT_CALCRECT);
    pDC->SelectObject(pOld);
    ReleaseDC(pDC);

    // 超过 MAX_TEXT_WIDTH 时文字进行分行
    int nTmp = rcText.Width();
    int nTextWidth = min(nTmp, MAX_TEXT_WIDTH);     // 调整后的文字区域宽度
    int nTextHeight = (rcText.Height() + 10) * (rcText.Width() / MAX_TEXT_WIDTH + 1);
    //int nTextHeight = max(36, nTmp);                // 调整后的文字区域的高度
	TRACE("TextHeight:%d - LineHeight:%d\n", nTmp, rcText.Height() + 10);
    
    // 根据文字区域的尺寸调整对话框的大小
    CRect rcClient(0, 0, 0, 0);
    rcClient.right = nTextWidth + 2 * MARGIN_SIZE + 56;
    rcClient.bottom = nTextHeight + 3 * MARGIN_SIZE;

	rcClient.right = max(350, rcClient.right);
	rcClient.bottom = max(210, rcClient.bottom);
    MoveWindow(rcClient);
}

void CMsgBoxDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    //CFDialog::OnPaint();

    // 按钮区的背景
    DrawBkg(&dc);

    DrawImg(&dc);

    DrawMsg(&dc);
}

void CMsgBoxDlg::DrawImg(CDC *pDC)
{
    if (NULL == pDC)
    {
        return ;
    }
    
    if (NULL == m_pImgIcon)
    {
        return ;
    }
    
    // 根据设置显示对应的图标
    int nOffSet = 0;
    if (m_uBtnType & MB_ICONERROR)
    {
        nOffSet = 1;
    }
    else if (m_uBtnType & MB_ICONQUESTION)
    {
        nOffSet = 2;
    }
    else if (m_uBtnType & MB_ICONINFORMATION)
    {
        nOffSet = 3;
    }
    else if (m_uBtnType & MB_ICONWARNING)
    {
        nOffSet = 4;
    }

    CRect rcClient;
    GetClientRect(rcClient);

    int nIconW = m_pImgIcon->GetWidth();
    int nIconH = m_pImgIcon->GetHeight() / 5;
    int nTop = (rcClient.Height() - MARGIN_SIZE - nIconH) / 2 ;
    RectF rcfDst((REAL)MARGIN_SIZE, (REAL)nTop, (REAL)nIconW, (REAL)nIconH);

    RenderEngine->DrawImageX(pDC, m_pImgIcon, rcfDst, 0, (REAL)nOffSet * nIconH, (REAL)nIconW, (REAL)nIconH);
}

void CMsgBoxDlg::DrawMsg(CDC *pDC)
{
    if (NULL == pDC)
    {
        return ;
    }

    CRect rcClient;
    GetClientRect(rcClient);

	pDC->SetBkMode(TRANSPARENT);
	CFont *pOld = pDC->SelectObject(RenderEngine->GetFont(m_strTextFont));

	// 文字显示区域的大小
	CRect rcTemp;	
	rcTemp.left = rcClient.left + MARGIN_SIZE * 2;
	rcTemp.right = rcClient.right - MARGIN_SIZE;
	rcTemp.top = 0;
	rcTemp.bottom = rcClient.bottom;
	DrawMultLineText(pDC, rcTemp, 10, DT_LEFT | DT_WORDBREAK | DT_CALCRECT, m_strText);

	// 文字在对话框中的显示位置
	CRect rcText;
	rcText.left = rcClient.left + MARGIN_SIZE * 2;
	rcText.right = rcClient.right - MARGIN_SIZE;
    rcText.top = (rcClient.Height() - rcTemp.Height() - 50) / 2;
    rcText.bottom = rcClient.bottom;

    // 使用扩展函数，支持行间距设置
    DrawMultLineText(pDC, rcText, 10, DT_LEFT | DT_WORDBREAK, m_strText);
    pDC->SelectObject(pOld);
}

// 按钮区域的背景
void CMsgBoxDlg::DrawBkg(CDC *pDC)
{
    if (NULL == pDC)
    {
        return ;
    }

    if (NULL == m_pImgBtnBkg)
    {
        return ;
    }

    CRect rcClient;
    GetClientRect(rcClient);

    RectF rcfDst(0, (REAL)rcClient.bottom - 50, (REAL)rcClient.Width(), (REAL)50);

    RenderEngine->DrawImageX(pDC, m_pImgBtnBkg, rcfDst, 0, 0, (REAL)m_pImgBtnBkg->GetWidth(), (REAL)m_pImgBtnBkg->GetHeight());
}

// 扩展接口，支持设置行间距
void CMsgBoxDlg::DrawMultLineText(CDC* pDC ,CRect& rect, int nRowDis, UINT nFromat, CString strText) 
{
    if( strText.GetLength() <= 0 )
    {
        return;
    }

    WCHAR* pText = strText.GetBuffer();
    int nCount = strText.GetLength();
    CRect rtChar;
    CSize size = pDC->GetTextExtent(pText + 0, 1);
    int nRowHeight = size.cx + nRowDis;
    rtChar.top = rect.top;
    rtChar.left = rect.left;
    rtChar.bottom	= rtChar.top + nRowDis + size.cy;
    rtChar.right	= rtChar.left + size.cx;	
    CString strChar;
	int nLine = 1;
    for (int nCharIndex = 0; nCharIndex < nCount; nCharIndex++)
    {
		// 换行
        if( rtChar.right > rect.right )
        {
			++nLine;
            rtChar.top = rtChar.bottom;
            rtChar.bottom += nRowHeight;
            size = pDC->GetTextExtent(pText + nCharIndex, 1);
            rtChar.left = rect.left;
            rtChar.right = rtChar.left + size.cx;
            if( rtChar.bottom > rect.bottom )
                break;
        }

		//遇到\r\n换行显示
		if (nCharIndex < nCount - 1)
		{
			WCHAR strChar1 = pText[nCharIndex];
			WCHAR strChar2 = pText[nCharIndex+1];

			if (('\r') == strChar1  && ('\n') == strChar2)
			{
				++nLine;
				rtChar.top = rtChar.bottom;
				rtChar.bottom += nRowHeight;
				size = pDC->GetTextExtent(pText + nCharIndex, 1);
				rtChar.left = rect.left;
				rtChar.right = rtChar.left + size.cx;
				if( rtChar.bottom > rect.bottom )
					break;
			}
		}

		// 显示&符号时，需要用&&才能显示
		strChar = pText[nCharIndex];
        if (0 == strChar.Compare(_T("&")))
        {
            strChar = _T("&&");
        }

		pDC->DrawText(strChar, rtChar, nFromat);
		size = pDC->GetTextExtent(pText + nCharIndex + 1, 1);
        rtChar.left = rtChar.right;
        rtChar.right += size.cx;
    }

	if (nFromat & DT_CALCRECT)
	{
		CRect rcTemp(rect);
		rect.top = 0;
		rect.bottom = nLine * nRowHeight;
		rect.left = 0;
		rect.right = rcTemp.Width();
	}
	TRACE("Multiline Height:%d - LineHeight:%d\n", nLine * nRowHeight, nRowHeight);
}

// 根据点击按钮，返回对应的结果
void CMsgBoxDlg::OnBtnClicked(UINT nID)
{
    switch (nID)
    {
    case IDC_BTN_OK:
        EndDialog(IDOK);
        break;
    case IDC_BTN_CANCEL:
        EndDialog(IDCANCEL);
        break;
    case IDC_BTN_YES:
        EndDialog(IDYES);
        break;
    case IDC_BTN_NO:
        EndDialog(IDNO);
        break;
    case IDC_BTN_RETRY:
        EndDialog(IDRETRY);
        break;
    default:
        EndDialog(IDOK);
    }
}

// 设置对话框的基本属性
void CMsgBoxDlg::SetText(const CString& strCaption, const CString& strText, UINT uType)
{
    m_strCaption = strCaption;
    m_strText = strText;
    m_uBtnType = uType;
}
HBRUSH CMsgBoxDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CFDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    if (nCtlColor == CTLCOLOR_DLG)
    {
        return m_brBkg;
    }

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}
