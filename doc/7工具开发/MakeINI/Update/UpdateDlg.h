
// UpdateDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CUpdateDlg 对话框
class CUpdateDlg : public CDialog
{
// 构造
public:
	CUpdateDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UPDATE_DIALOG };

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
	CListCtrl m_StoreItems;
	queue<CString> download_list;
	void InitCtr(void);
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnDownload();
	void PushList(void);
	void WriteConfig(int i,struct fileStruct &fileInfo);
	void DeleteConfig( );
	void FindFiles(void);
	void GetAllFileInfo(void);
	void WriteCommon(void);
	BOOL GetFileVersion(LPCTSTR strFile, CString& strVersion,CString &filename,CString &description);   
	bool StartConnect(void);

	//afx_msg void OnBnClickedButtonCon();
	CString GeTPath(void);
};

typedef struct fileStruct
{
	CString Name;//=replace2.dll
//# 文件大小
	ULONGLONG filesize;//=246
//# 文件下载地址
	CString	URL;//=http://127.0.0.1/download/AutoUpdate/replace2.dll
//# 文件压缩方式
	CString CompressMethod;//=None
//# 文件摘要码，用标准MD5算法得到
	CString	Hash;//=E2DC119208AB13BB254E0DD7DEC04466
//# 文件安装子路径，相对于安装目录
	CString Subcatalog;//=1\//
}fileStruct;

