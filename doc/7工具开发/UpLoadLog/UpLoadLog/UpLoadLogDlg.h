// UpLoadLogDlg.h : 头文件
//

#pragma once


// CUpLoadLogDlg 对话框
class CUpLoadLogDlg : public CDialog
{
// 构造
public:
	UploadThread *m_upthread;
	char zipfile[MAX_PATH];
	char file[MAX_PATH];
public:
	CUpLoadLogDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UPLOADLOG_DIALOG };

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
	afx_msg void OnNcPaint();
	afx_msg LRESULT UserMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	void UpLoad();        //上传文件
	void SetUploadInfo(); //设置上传信息
	void GetExePath();    //获取程序本地路径
protected:
	virtual void OnCancel();
};
