// RealWaveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PreviewS20Dll.h"
#include "RealWaveDlg.h"


// CRealWaveDlg 对话框

IMPLEMENT_DYNAMIC(CRealWaveDlg, CChildDlgOwerDraw)

CRealWaveDlg::CRealWaveDlg(CWnd* pParent /*=NULL*/)
	: CChildDlgOwerDraw(CRealWaveDlg::IDD, pParent)
    , m_bInitFlag(FALSE)
{

}

CRealWaveDlg::~CRealWaveDlg()
{
}

void CRealWaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDlgOwerDraw::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_REAL_WAVE, m_scope);
	DDX_Control(pDX, IDC_STC_PHONE_WAVE, m_scopePhone);
}


BEGIN_MESSAGE_MAP(CRealWaveDlg, CChildDlgOwerDraw)
    ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CRealWaveDlg 消息处理程序

BOOL CRealWaveDlg::OnInitDialog()
{
    CChildDlgOwerDraw::OnInitDialog();

    SetTitleBar_Image(RESOURCELOAD_PNG_EX_IMG(IDR_PNG_CHILDCUSTOMDLG_TITLE));
    SetTitleBar_MinBtnImage(RESOURCELOAD_PNG_EX_IMG(IDR_PNG_CHILDCUSTOMDLG_CLOSE),
        RESOURCELOAD_PNG_EX_IMG(IDR_PNG_CHILDCUSTOMDLG_OPEN),
        RESOURCELOAD_PNG_EX_IMG(IDR_PNG_CHILDCUSTOMDLG_BTNSEL));

    //SetBakeColor(theApp.GetUIColor(UI_COLOR_TYPE_PANEL_BKG));

    m_bInitFlag = TRUE;

    return TRUE;
}

void CRealWaveDlg::MoveCtrl()
{
    CRect clientRect;
    GetClientRect(clientRect);
    //clientRect.top = clientRect.top;
    //clientRect.left = clientRect.left;
    //clientRect.right = clientRect.right;
    //clientRect.bottom = clientRect.bottom;

    //波形
	CRect rcVideoWave;
	rcVideoWave.top = clientRect.top;
	rcVideoWave.left = clientRect.left;
	rcVideoWave.right = clientRect.right/2 - 1;
	rcVideoWave.bottom = clientRect.bottom;
    m_scope.MoveWindow(&rcVideoWave);

	CRect rcPhoneWave;
	rcPhoneWave.top = clientRect.top;
	rcPhoneWave.left = rcVideoWave.right + 2;
	rcPhoneWave.right = clientRect.right;
	rcPhoneWave.bottom = clientRect.bottom;
	m_scopePhone.MoveWindow(rcPhoneWave);
}

void CRealWaveDlg::OnSize(UINT nType, int cx, int cy)
{
    CChildDlgOwerDraw::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    if (m_bInitFlag)
    {
        MoveCtrl();
    }
}

void CRealWaveDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    CRect rcClient;
    GetClientRect(rcClient);

    CBitmap bmpBake;
    bmpBake.Attach(g_ResourceLoader.ResourceLoad_Bmp(IDB_BMP_REALWND_TOOLBAR));

    BITMAP bm;
    bmpBake.GetBitmap(&bm);

    CDC dcMem;
    dcMem.CreateCompatibleDC(&dc);
    dcMem.SelectObject(bmpBake);

    dc.StretchBlt(0,0,rcClient.Width(),rcClient.Height(),
        &dcMem,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
}
