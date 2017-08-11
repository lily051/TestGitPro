#pragma once
#include "ui/Picture.h"

// CDlgPic 对话框

class CDlgPic : public CDialog
{
	DECLARE_DYNAMIC(CDlgPic)
private:
    CPicture m_pic;
public:
    void SetPic(CString & sPicPath);
	CDlgPic(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPic();

// 对话框数据
	enum { IDD = IDD_DLG_PIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnDestroy();
};
