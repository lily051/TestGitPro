#pragma once
#include "LocalVideoFile.h"
#include "LocalVideoFilePlay.h"


// CLocalPlayback �Ի���

class CLocalPlayback : public CDialog
{
	DECLARE_DYNAMIC(CLocalPlayback)

public:
	CLocalPlayback(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLocalPlayback();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOCALPLAYBACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	// �ļ�ѡ��Ի���.
	CLocalVideoFile* m_pLocalVedioFile;
	// �ļ����ŶԻ���.
	CLocalVideoFilePlay* m_pLocalVedioFilePlay;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT OnMsgLocalVideoPlay(WPARAM wParam, LPARAM lParam);
	// ���ػط�.
	BOOL LocalPlayback(const int nVideoType, const CString csFilePath);
	BOOL StopPlayback();

protected:
	// �����Ӵ���
	void CreateChildWnd(void);
	// ��ʼ���ؼ�λ��
	void MoveCtrl(void);
    int m_vidoWndshow; //�Ƿ���ʾ�б���
};
