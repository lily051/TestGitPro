// BottomDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RecTool.h"
#include "BottomDlg.h"


// CBottomDlg �Ի���

IMPLEMENT_DYNAMIC(CBottomDlg, CDialog)

CBottomDlg::CBottomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBottomDlg::IDD, pParent)
{

}

CBottomDlg::~CBottomDlg()
{
}

void CBottomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBottomDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CBottomDlg ��Ϣ�������

BOOL CBottomDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillSolidRect(rcClient, RGB(31, 31, 31));  //���ñ���ɫ

	return TRUE;
}

void CBottomDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void CBottomDlg::InitCtrl(void)
{

}
BOOL CBottomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
