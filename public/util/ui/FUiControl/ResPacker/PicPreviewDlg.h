#pragma once


// CPicPreviewDlg �Ի���

class CPicPreviewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPicPreviewDlg)

public:
	CPicPreviewDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPicPreviewDlg();

// �Ի�������
	enum { IDD = IDD_DLG_PIC_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	Gdiplus::Image* m_pImage;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CFont m_font;
};
