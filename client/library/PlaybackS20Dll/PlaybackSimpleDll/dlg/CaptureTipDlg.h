#pragma once
#include "Picture.h"
#include "afxwin.h"
#include "../ui/uiCtrl/UiDialog.h"



// CCaptureTipDlg �Ի���

class CCaptureTipDlg : public UIDialog
{
	DECLARE_DYNAMIC(CCaptureTipDlg)

public:
	CCaptureTipDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCaptureTipDlg();

// �Ի�������
	enum { IDD = IDD_DLG_CAPTURE_TIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CBrush m_bkBrush;

	CStatic m_sttPath;
	BOOL m_bIsInit;
	CPicture m_pic;
	CRect m_rcPic;
	CRect m_rcPath;

	BOOL m_bTimer;     //��ʱ���Ƿ�������
	BOOL m_bCount;	   // �Ƿ���Ҫ��ʱ.
	int m_nLeaveTime;  //����뿪ʱ��

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
	void LoadPic(TCHAR *szPicPath);	
	void InitPos();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
};
