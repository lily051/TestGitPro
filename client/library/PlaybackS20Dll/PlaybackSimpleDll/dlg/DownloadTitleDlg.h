#pragma once
#include "afxwin.h"
#include "../ui/uiBase/TitleBtn.h"
#include "../ui/uiBase/IconStatic.h"


// CDownloadTitleDlg 对话框

class CDownloadTitleDlg : public CDialog
{
	DECLARE_DYNAMIC(CDownloadTitleDlg)

public:
	CDownloadTitleDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDownloadTitleDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DOWNLOADTITLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bMove;
	CBrush m_bkBrush;

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
	/** @fn SetStaText
	*   @brief 设置static控件文字
	*   @param[in]  NULL
	*   @param[out] NULL
	*   @return NULL
	*/
	void SetStaText();

public:
	int m_nDowningCount;      //正在下载的文件数量
	int m_nDownSuccessCount;  //下载成功的文件数量
	int m_nDownFailedCount;   //下载失败的文件数量

public:
	CTitleButton m_btnTitle;
	CIconStatic m_staDownloading;
	CIconStatic m_staDownSuccess;
	CIconStatic m_staDownFailed;

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedStaticDownloading();
	afx_msg void OnStnClickedStaticDownsuccess();
	afx_msg void OnStnClickedStaticDownfailed();
	afx_msg void OnPaint();
};
