#pragma once


// CDlgHelp �Ի���

class CDlgHelp : public CDialog
{
	DECLARE_DYNAMIC(CDlgHelp)

public:
	CDlgHelp(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgHelp();

// �Ի�������
	enum { IDD = IDD_DLG_HELP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
