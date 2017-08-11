#pragma once


// CPreviewCtrlDlg �Ի���

class CPreviewCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CPreviewCtrlDlg)

public:
	CPreviewCtrlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPreviewCtrlDlg();

// �Ի�������
	enum { IDD = IDD_DLG_PREVIEW_CRTL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void InitCtrl(void);
	void MoveCtrl(void);
	void UnInitCtrl(void);
	BOOL StartPreview(PREVIEW_CAMINFO& stCamInfo);
	BOOL StartSound(void);
	HANDLE m_hPreview;
	afx_msg void OnDestroy();
};
