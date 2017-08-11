#pragma once
#include "atltypes.h"


// CSplashDlg 对话框

class CSplashDlg : public CDialog
{
	DECLARE_DYNAMIC(CSplashDlg)

public:
	CSplashDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSplashDlg();

// 对话框数据
	enum { IDD = IDD_DLG_SPLASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

	//设置初始文字
	void SetText(const CString& strTitle, const CString& strVer, const CString& strText);
	//设置动态文字
	void SetText(const CString& strText);
	//显示界面
	void Show();
	//隐藏界面
	void Hide();
	//刷新界面
	void SplashStep(CDC* pDC);
	//画背景图
	void DrawBkg(CDC* pDC);
	//刷新界面线程函数
	void ThrobberProc();

private:
	Image *m_pImgBkg;
	CRect m_rcWnd;
	HANDLE m_hExitThread;
	HANDLE m_hThread;
	CString m_strTitle;
	CString m_strText;
	CString m_strVer;
	
};
