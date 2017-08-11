// RecNameInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "RecTool.h"
#include "RecNameInfoDlg.h"


// CRecNameInfoDlg 对话框

IMPLEMENT_DYNAMIC(CRecNameInfoDlg, CDialog)

CRecNameInfoDlg::CRecNameInfoDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRecNameInfoDlg::IDD, pParent)
{

}

CRecNameInfoDlg::~CRecNameInfoDlg()
{
}

void CRecNameInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RECNAME, m_staticRecName);
}


BEGIN_MESSAGE_MAP(CRecNameInfoDlg, CDialog)
	ON_WM_ERASEBKGND()
	//	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_PV_STARTRECORD, &CRecNameInfoDlg::OnStartRecord)
	ON_MESSAGE(WM_PV_STOPRECORD, &CRecNameInfoDlg::OnStopRecord)
END_MESSAGE_MAP()


// CRecNameInfoDlg 消息处理程序

BOOL CRecNameInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitCtrl();

	MoveCtrl();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CRecNameInfoDlg::InitCtrl(void)
{

	//COLORREF clrRef()
	m_staticRecName.SetTextColor(RGB(255, 255, 255));
	//m_btnEditRecName.SetObjectName(_T("btn"));
	//m_btnEditRecName.SetIcon(_T("edit-(video)"));
}

void CRecNameInfoDlg::MoveCtrl(void)
{
	CRect rcClient(0, 0, 400, 30);
	MoveWindow(rcClient);

	// 	CRect rcClient;
	// 	GetClientRect(&rcClient);

	m_staticRecName.MoveWindow(5, 5, 200, 30);
	m_staticRecName.ShowWindow(SW_SHOW);
	//m_staticRecName.ShowWindow(SW_HIDE);

}

BOOL CRecNameInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillSolidRect(rcClient, RGB(70, 70, 70));

	return TRUE;
}


//void CRecNameInfoDlg::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// 判断鼠标点击是否在编辑框内
//	CString strOldRecName;
//	m_staticRecName.GetWindowText(strOldRecName);
//	CRect rcEdit;
//	m_editRecName.GetClientRect(rcEdit);
//	if (!PtInRect(rcEdit, point))
//	{
//		m_staticRecName.ShowWindow(SW_SHOW);
//		m_btnEditRecName.ShowWindow(SW_HIDE);
//		m_editRecName.ShowWindow(SW_HIDE);
//	}
//
//	//设置新录像文件名
//	CString strNewRecName = _T("");
//	m_editRecName.GetWindowText(strNewRecName);
//	if (_T("") == strNewRecName)
//	{
//		strNewRecName = strOldRecName;
//	}
//	CString strPostfix = strNewRecName.Mid(strNewRecName.ReverseFind('.'), 4);
//	if (0 != strPostfix.CompareNoCase(_T(".mp4")))
//	{
//		strNewRecName.Append(_T(".mp4"));
//	}
//	m_staticRecName.SetWindowText(strNewRecName);
//	UpdateData(FALSE);	
//
//	CDialog::OnLButtonDown(nFlags, point);
//}

LRESULT CRecNameInfoDlg::OnStartRecord(WPARAM wparam, LPARAM lparam)
{
	m_staticRecName.ShowWindow(SW_SHOW);
	const RECOED_FILE_INFO * pRecordFileInfo = (const RECOED_FILE_INFO*)wparam;
	CString strFileName = pRecordFileInfo->strFileName.Mid(pRecordFileInfo->strFileName.ReverseFind('\\') + 1);
	m_staticRecName.SetText(strFileName);

	//delete pRecordFileInfo;
	return 0;
}

LRESULT CRecNameInfoDlg::OnStopRecord(WPARAM wparam, LPARAM lparam)
{
	// 	CString strRecName;
	// 	m_staticRecName.GetWindowText(strRecName);
	m_staticRecName.SetText(_T(""));
	UpdateData(FALSE);
	CString title;
	m_staticRecName.GetWindowText(title);
	m_staticRecName.ShowWindow(SW_HIDE);

	const RECOED_FILE_INFO * pRecordFileInfo = (const RECOED_FILE_INFO*)wparam;
	// 	CString strFileName = pRecordFileInfo->strFileName.Mid(pRecordFileInfo->strFileName.ReverseFind('\\') + 1);
	// 	m_staticRecName.SetWindowText(strFileName);

	//delete pRecordFileInfo;
	return 0;
}

BOOL CRecNameInfoDlg::StopRecord(CString & strRecFileName)
{
	// 	CString strRecName;
	// 	m_staticRecName.GetWindowText(strRecName);
	// 	strRecFileName = theApp.m_strAppCapPath + _T("\\");
	// 	strRecFileName.Append(strRecName);
	m_staticRecName.SetWindowText(_T(""));
	//m_staticRecName.ShowWindow(SW_HIDE);

	return TRUE;
}



BOOL CRecNameInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_NCLBUTTONDBLCLK)  ////屏蔽标题栏双击事件(最大化/还原)
	{
		return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN)  
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:  //屏蔽Enter
			return TRUE;
		case VK_ESCAPE:   //屏蔽ESC键
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
