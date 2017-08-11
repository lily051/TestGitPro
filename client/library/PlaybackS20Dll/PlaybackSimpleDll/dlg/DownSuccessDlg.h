#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "../ui/uiBase/TransButton.h"
#include "../ui/uiBase/SkinListCtrl.h"


// CDownSuccessDlg 对话框
#define WM_MENU_OPENFILE 1000
#define WM_MENU_DELETETASK 1001
#define WM_MENU_OPENDIR 1002

class CDownSuccessDlg : public CDialog
{
	DECLARE_DYNAMIC(CDownSuccessDlg)

public:
	CDownSuccessDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDownSuccessDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DOWNSUCCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bMove;

	CRect m_rcTitle;

	CDC m_dcMem;
	HBITMAP m_hBitmap;
	int m_nBitmapWidth;  //背景图片宽度
	int m_nBitmapHeight;  //背景图片高度

public:
	std::map<LONG, DOWNLOAD_INFO> m_mapDownInfo;  //下载信息

private:
	/** @fn InitCtrl
	*   @brief 初始化控件
	*   @param[in]  NULL
	*   @param[out] NULL
	*   @return NULL
	*/
	void InitCtrl();

	/** @fn MoveCtrl
	*   @brief 移动控件
	*   @param[in]  NULL
	*   @param[out] NULL
	*   @return NULL
	*/
	void MoveCtrl();

public:

	//增加一条记录
	BOOL AddDownloadInfo(LONG lSeq, DOWNLOAD_INFO& stDownloadInfo);


	/** @fn TransparentBk
	*   @透明绘图 
	*   @param[in] pDc: 子控件pDc
	*   @param[in] pWnd: 子控件句柄 
	*   @return NULL
	*/
	void TransparentBk(CDC * pDc, CWnd * pWnd);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CTransButton m_btnOpenFile;
	CTransButton m_btnDelete;
	CTransButton m_btnClear;
	CTransButton m_btnOpenDir;
	CSkinListCtrl m_listDownloadSuc;
	afx_msg void OnBnClickedButtonOpenfile();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonOpenDir();
	afx_msg void OnNMRClickListDownloadsuc(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnItemchangedListDownloadsuc(NMHDR *pNMHDR, LRESULT *pResult);
};
