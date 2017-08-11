#pragma once
#include "Picture.h"
#include "afxwin.h"
#include "../ui/uiCtrl/UiDialog.h"


// CSaveDataDlg �Ի���

class CSaveDataDlg : public UIDialog
{
	DECLARE_DYNAMIC(CSaveDataDlg)

public:
	CSaveDataDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSaveDataDlg();

// �Ի�������
	enum { IDD = IDD_DLG_CAPTURE_TIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CBrush m_bkBrush;

	CStatic m_sttPath;
	BOOL m_bIsInit;
	CRect m_rcPath;
    CString m_strPath;

	BOOL m_bTimer;     //��ʱ���Ƿ�������
	int m_nLeaveTime;  //����뿪ʱ��
	BOOL m_bCount;	   // �Ƿ���Ҫ��ʱ.

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();

	void LoadFile(TCHAR *szPicPath);	
	void InitPos();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
    void DrawTextInRect(CString strText, CRect rcTemp);
};
