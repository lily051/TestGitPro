#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "../ui/uiBase/TransButton.h"
#include "../ui/uiBase/SkinListCtrl.h"
#include "DownloadOper.h"

#define TIMER_ID_GETDOWNLOADPOS WM_USER + 1101
#define MSG_UPDATEBTN (WM_USER + 1102)

class CDownloadingDlg : public CDialog
{
	DECLARE_DYNAMIC(CDownloadingDlg)

public:
	CDownloadingDlg(HANDLE hDlg, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDownloadingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DOWNLOADING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bMove;
	HANDLE m_hDlg;
	CRect m_rcTitle;

	CDC m_dcMem;
	HBITMAP m_hBitmap;
	int m_nBitmapWidth;  //背景图片宽度
	int m_nBitmapHeight;  //背景图片高度

	LONG m_lDowningSeq;          //当前正在下载的信息序列
	std::map<LONG, DOWNLOAD_INFO> m_mapDownInfo;  //下载信息
	CRITICAL_SECTION m_csDownlist;  //下载信息信号量
	HANDLE m_hDownload;          //当前下载句柄
	int m_nCamID;				 // 当前监控点ID.
	DWORD m_dwTimeBegin;		 // 当前下载录像的开始时间.
	DWORD m_dwTimeEnd;			 // 当前下载录像的结束时间.
	DWORD m_dwRev;				 // 预留字段.
	CDownloadOper m_opeDownload; // 下载控制类.
public:
	BOOL m_bDownloading;         //正在下载
	BOOL m_bWorking;             //下载线程正在工作
	HANDLE m_hDownloadThread;    //下载线程

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
	/** @fn TransparentBk
	*   @透明绘图 
	*   @param[in] pDc: 子控件pDc
	*   @param[in] pWnd: 子控件句柄 
	*   @return NULL
	*/
	void TransparentBk(CDC * pDc, CWnd * pWnd);

	//下载线程
	static void DownloadThreadFun(LPVOID lpvoid);

	//开始下载
	BOOL StartDownload();
	//下载单个任务
	BOOL DownloadSingleTask(LONG lSeq, DOWNLOAD_INFO& stDownloadInfo);
	//停止下载
	void StopDownload();
	//事件通知下载
	BOOL CDownloadingDlg::NotifyStartDownload();

	//增加一条记录
	BOOL AddDownloadInfo(LONG lSeq, DOWNLOAD_INFO &stDownloadInfo);
	//删除选中的记录
	void DelSelectInfo();
	//清空列表
	void ClearList();
	// 获取下载状态信息.
	BOOL GetPlaybackInfo(PlaybackStatData& stData);
	// 磁盘空间不足.
	HRESULT OnMsgCheckSpaceNotEnough(WPARAM wParam, LPARAM lParam);
    //更新菜单栏按钮状态
    LRESULT UpdateMenuButtonState(WPARAM wParam, LPARAM lParam);
    //下载失败消息
    LRESULT OnMsgDownloadFailed(WPARAM wParam, LPARAM lParam);
    //序号重排
    void ReSetListIndex();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CTransButton m_btnStartDownload;
	CTransButton m_btnStopDownload;
	CTransButton m_btnDelete;
	CTransButton m_btnClear;
	CSkinListCtrl m_listDownloading;
	afx_msg void OnBnClickedButtonStartdownload();
	afx_msg void OnBnClickedButtonStopdownload();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonClear();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnLvnItemchangedListDownloading(NMHDR *pNMHDR, LRESULT *pResult);
};
