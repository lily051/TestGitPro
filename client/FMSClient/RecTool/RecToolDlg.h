
// RecToolDlg.h : 头文件
//

#pragma once
#include <set>
#include "FUi/FDialog.h"
#include "PreviewCtrlDlg.h"
#include "Fui/flistctrl.h"
#include "PlaybackCtrlDlg.h"
#include "RecNameInfoDlg.h"
#include "BottomDlg.h"
#include "EditFileNameDlg.h"
#include "Fui/fstatic.h"
#include "ListInfoDlg.h"


// CRecToolDlg 对话框
class CRecToolDlg : public CFDialog
{
// 构造
public:
	CRecToolDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CRecToolDlg();

// 对话框数据
	enum { IDD = IDD_RECTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void LoadSkin(void);

private:
	//预览窗口
	CPreviewCtrlDlg * m_lpPreviewCtrlDlg;
	//录像名称窗口
	CRecNameInfoDlg * m_lpRecNameInfoDlg;
	//回放窗口
	CPlaybackCtrlDlg *m_lpPlaybackCtrlDlg;
	//状态栏
	CBottomDlg * m_lpBottomDlg;
	//文件列表
	CListInfoDlg * m_lpListInfoDlg;
public:
	void InitCtrl(void);
	void MoveCtrl(void);

	//处理录像提示通知信息
	void NotifyRecordFileInfo(const RECOED_FILE_INFO &recoedFile);

	BOOL StartPreview(void);

	bool m_bIsRecording;

	typedef struct _tagItemData
	{
		CString strFileName;
	}ITEMDATA, *LPITEMDATA;

	BOOL InsertRec(const RECOED_FILE_INFO &recordFile);
	BOOL InsertPic(const RECOED_FILE_INFO &recordFile);
	CFListCtrl m_listRecords;
	CArray<RECOED_FILE_INFO> m_PicFileArray;  //图片列表
	CArray<RECOED_FILE_INFO> m_RecFileArray; //录像列表
	CStringArray m_ArrayFileList;	//文件列表
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

public:
    CFStatic m_staticBlank1;
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

extern CRecToolDlg * g_pRecMgrDlg;
