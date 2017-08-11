#pragma once


// CPlaybackCtrlDlg �Ի���

class CPlaybackCtrlDlg : public CFDialog
{
	DECLARE_DYNAMIC(CPlaybackCtrlDlg)

public:
	CPlaybackCtrlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlaybackCtrlDlg();

	// �Ի�������
	enum { IDD = IDD_DLG_PLAYBACK_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void InitCtrl(void);
	virtual BOOL OnInitDialog();
	void OnCancel();
	HANDLE m_hPlayback;
	void MoveCtrl(void);
	afx_msg void OnDestroy();
	BOOL StartPlayBack(int nFlag, CString strRecName);
};
