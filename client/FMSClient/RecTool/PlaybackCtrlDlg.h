#pragma once


// CPlaybackCtrlDlg 对话框

class CPlaybackCtrlDlg : public CFDialog
{
	DECLARE_DYNAMIC(CPlaybackCtrlDlg)

public:
	CPlaybackCtrlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlaybackCtrlDlg();

	// 对话框数据
	enum { IDD = IDD_DLG_PLAYBACK_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
