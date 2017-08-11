#pragma once
#include "ui/UIDialog.h"
#include "util/Guard.h"
#include "Picture.h"


// CCaptureTipDlg �Ի���

class CCaptureTipDlg : public UIDialog
{
	DECLARE_DYNAMIC(CCaptureTipDlg)

public:
	CCaptureTipDlg(CString& strPath, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCaptureTipDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PREVIEW_CAPTIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CStatic m_sttPath;
	BOOL m_bIsInit;
	CPicture m_pic;
	CRect m_rcPic;
	CRect m_rcPath;
    CBrush m_brushBK;				//������ˢ
    CString m_strPath;

	HANDLE m_hDisappear;

	BOOL m_bMouseTrack;

public:

	int m_nDisAppearCount;		//������ʧ����
	CRITICAL_SECTION m_csDisAppearCount;
	BOOL m_bMouseHover;

	static DWORD WINAPI AutoDisappear(LPVOID pParam);
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();

	
	void LoadPic(PBYTE pImage,DWORD dwImageSize);	
	void InitPos();
	void ResetTimeInterVal();
};
