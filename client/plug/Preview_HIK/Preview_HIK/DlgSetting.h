#pragma once


// CDlgSetting �Ի���

class CDlgSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgSetting)

public:
	CDlgSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSetting();

// �Ի�������
	enum { IDD = IDD_DLG_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
