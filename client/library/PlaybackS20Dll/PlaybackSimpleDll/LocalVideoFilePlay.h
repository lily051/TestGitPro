#pragma once
#include "LocalWndTitle.h"
#include "LocalWndView.h"
#include "LocalWndToolBar.h"
#include "CaptureTipDlg.h"


// CLocalVideoFilePlay �Ի���

class CLocalVideoFilePlay : public CDialog
{
	DECLARE_DYNAMIC(CLocalVideoFilePlay)

public:
	CLocalVideoFilePlay(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLocalVideoFilePlay();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOCALVIDEOFILE_PLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	CLocalWndTitle *m_pLocalWndTitle;
	CLocalWndView *m_pLocalWndView;
	CLocalWndToolBar *m_pLocalWndToolBar;
	CCaptureTipDlg *m_pCaptureTip;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	LRESULT OnMsgToolBarOpera(WPARAM wParam, LPARAM lParam);

public:
	// ��ʼ����
	void StartPlay(const int nVideoType, const CString csFilePath);
	// ֹͣ����
	void StopPlay();

protected:
	// �����Ӵ���
	void CreateWnd(void);
	// ��ʼ���ؼ�λ��
	void MoveWnd(void);
	// ������תΪ00:00:00���͵�ʱ��
	CString SecondToStrTime(DWORD seconds);

	// ץͼ
	BOOL OnCapture();
	// ȫ��
	void OnFullScreen(BOOL bFull);
	// �޸Ĵ��ڱ���
	void OnUpdateWndTitle();

public:
	// ���Ŷ˿�
	LONG m_lPlayPort;
	//�Ƿ���Ҫֹͣ����
	BOOL m_bStopPlay; 
private:
	// ��ʱ��
	CString m_strTotalTime;
	// �ļ�����
	CString m_csFileName;
	// ��ʱ������
	DWORD m_dwTatalSeconds;
	// �Ƿ����ڲ���
	BOOL m_bIsPlaying;
	// �Ƿ�������ͣ
	BOOL m_bIsPausing;
	// �Ƿ���Ƶ������
	BOOL m_bIsSounding;
	// �Ƿ�ȫ����
	BOOL m_bIsFullScreening;
	// ����
	PLAYSPEED m_nSpeed;


	// ���ڴ�С
	CRect m_rcWndSize;
	// ȫ�����ھ��
	CWnd* m_pFullScreenWnd;
	// ȫ�����ڸ����ھ��
	CWnd* m_pFullScreenParentWnd;
	// �ļ�·��
	CString m_csFilePath;
	
};
