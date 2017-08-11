
// ResPackerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "zlibHelper.h"
#include "PicPreviewDlg.h"
#include "ColorListCtrl.h"


// CResPackerDlg 对话框
class CResPackerDlg : public CDialog
{
// 构造
public:
	CResPackerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RESPACKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	BOOL PackWithzlib(CString& strPath);


	//////////////////////////////////////////////////////////////////////////
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CTreeCtrl m_trRes;
	CPicPreviewDlg m_dlgPicPreview;
	CColorListCtrl m_lcLog;

	CString m_strResPath;
	CString m_strZipRootPath;	// zip文件的保存路径
	CImageList m_imageList;

	zlibHelper m_zlib;
	CString m_strPassword;

	RC_INFO m_rcInfoList;

    std::vector<CString> m_vecTempResPath;

	// 菜单项：打开文件、保存资源、设置密码
	CMenu m_menu;
	afx_msg void OnMenuOpen();
    afx_msg void OnMenuNew();
	afx_msg void OnMenuSaveRed();
    afx_msg void OnMenuSaveBlue();
    afx_msg void OnMenuExtract();
	afx_msg void OnMenuPassword();

	BOOL m_bInit;
	void InitCtrl();
	void MoveCtrl();
	void AddLog(const CString& strMsg, int nLevel = 0);
	// 创建资源树
	void BuildRcTree(const RC_INFO& rcInfo, HTREEITEM hItem = TVI_ROOT);
	//
	void ListRes(RC_INFO& rcInfo);
	// 
	BOOL SaveRc(const int nSkinType = 0);
	// 
	BOOL AddRes2Zip(zlibHelper& zlib, RC_INFO& rcInfo);

	// 自定义消息：打开资源文件
	afx_msg LRESULT OnMsgOpenRc(WPARAM, LPARAM);
	// 自定义消息：添加资源文件夹
	afx_msg LRESULT OnMsgAddRes(WPARAM, LPARAM);
	// 
	afx_msg void OnTvnSelchangedTreeRes(NMHDR *pNMHDR, LRESULT *pResult);
    
    afx_msg void OnClose();
};
