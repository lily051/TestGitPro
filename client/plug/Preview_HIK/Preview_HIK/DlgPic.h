#pragma once
#include "ui/Picture.h"

// CDlgPic �Ի���

class CDlgPic : public CDialog
{
	DECLARE_DYNAMIC(CDlgPic)
private:
    CPicture m_pic;
public:
    void SetPic(CString & sPicPath);
	CDlgPic(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPic();

// �Ի�������
	enum { IDD = IDD_DLG_PIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnDestroy();
};
