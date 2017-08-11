#pragma once
#include "LocalVideoFile.h"
#include "LocalVideoFilePlay.h"


// CLocalPlayback 对话框

class CLocalPlayback : public CDialog
{
	DECLARE_DYNAMIC(CLocalPlayback)

public:
	CLocalPlayback(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLocalPlayback();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOCALPLAYBACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	// 文件选择对话框.
	CLocalVideoFile* m_pLocalVedioFile;
	// 文件播放对话框.
	CLocalVideoFilePlay* m_pLocalVedioFilePlay;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT OnMsgLocalVideoPlay(WPARAM wParam, LPARAM lParam);
	// 本地回放.
	BOOL LocalPlayback(const int nVideoType, const CString csFilePath);
	BOOL StopPlayback();

protected:
	// 创建子窗口
	void CreateChildWnd(void);
	// 初始化控件位置
	void MoveCtrl(void);
    int m_vidoWndshow; //是否显示列表窗口
};
