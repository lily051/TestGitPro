
// TimeConverterDlg.h : 头文件
//

#pragma once


// CTimeConverterDlg 对话框
class CTimeConverterDlg : public CDialog
{
// 构造
public:
	CTimeConverterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TIMECONVERTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CString m_Date;
    ULONGLONG m_TimeStamp;
    afx_msg void OnEnKillfocusDateEdit();
    afx_msg void OnEnKillfocusTimestampEdit();
};
