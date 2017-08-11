// PasswordDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ResPacker.h"
#include "PasswordDlg.h"


// CPasswordDlg �Ի���

IMPLEMENT_DYNAMIC(CPasswordDlg, CDialog)

CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPasswordDlg::IDD, pParent)
{

}

CPasswordDlg::~CPasswordDlg()
{
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


CString CPasswordDlg::LoadPassword()
{
	TCHAR szPassword[64] = {0};
	GetProfileString(_T("ResPacker"), _T("Password"), _T("Hik_Financial_8100"), szPassword, 64);
	CString strPassword(szPassword);
	return strPassword;
}

BEGIN_MESSAGE_MAP(CPasswordDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_OK, &CPasswordDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_RESET, &CPasswordDlg::OnBnClickedBtnReset)
END_MESSAGE_MAP()


// CPasswordDlg ��Ϣ�������


void CPasswordDlg::OnBnClickedBtnOk()
{
	CString strPassword;
	GetDlgItemText(IDC_EDIT_PASSWORD, strPassword);
	if (strPassword.GetLength() > 64)
	{
		MessageBox(_T("Password max length:64"));
		return ;
	}
	WriteProfileString(_T("ResPacker"), _T("Password"), strPassword.GetBuffer());

	OnOK();
}

BOOL CPasswordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_EDIT_PASSWORD, LoadPassword());

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPasswordDlg::OnBnClickedBtnReset()
{
	SetDlgItemText(IDC_EDIT_PASSWORD, _T("Hik_Financial_8100"));
	WriteProfileString(_T("ResPacker"), _T("Password"),  _T("Hik_Financial_8100"));
}
