
// Preview_HIKDlg.h : ͷ�ļ�
//

#pragma once
#include "DlgPic.h"


// CPreview_HIKDlg �Ի���
class CPreview_HIKDlg : public CDialog
{
// ����
public:
	CPreview_HIKDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PREVIEW_HIK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
    CDlgPic * m_pPicDlg;
    CBrush m_bkBrush;
    NET_DVR_DEVICEINFO_V30 m_devInfo;
    LONG m_lLoginID;
    LONG m_lPlayHandle;
private:
    void StartPlay(int nChannel);
    void StopPlay();
    void CapturePicture();
    void SetPicDlgShowPos();
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
    virtual void OnCancel();
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
};
