#pragma once


// CPicPreviewDlg 对话框

class CPicPreviewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPicPreviewDlg)

public:
	CPicPreviewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPicPreviewDlg();

// 对话框数据
	enum { IDD = IDD_DLG_PIC_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	Gdiplus::Image* m_pImage;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CFont m_font;
};
