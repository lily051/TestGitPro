
// FixToolDlg.h : ͷ�ļ�
//

#pragma once
#include "Tinyxml/tinyxml.h"
#include "Tinyxml/tinyxmlutils.h"
#include <WinSvc.h>
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

// CFixToolDlg �Ի���
class CFixToolDlg : public CDialog
{
// ����
public:
	CFixToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FIXTOOL_DIALOG };
	CComboBox	m_comProduct;
	CEdit		m_editPackPath;
	CString		m_sPackPath;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnSelectPack();

public:
	BOOL GetProcessPath(const CString & ProcessName, CString & ProcessPath);
};
