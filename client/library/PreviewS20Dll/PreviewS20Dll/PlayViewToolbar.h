#pragma once
#include "ui\DrawStripFun.h"
#include "ui\ButtonST_Skin.h"
#include "ui\CustomDrawBlackWnd.h"
#include "util\CToolTips.h"
#include "ui/Scope.h"

#define PLAY_TOOLBAR_TIPS_MAXNUM    14

static const UINT RECORD_TIMER = 1; ///< ¼��ʱ����ʱ��

// CPlayViewToolbar

class CPlayViewToolbar : public CWnd
{
	DECLARE_DYNAMIC(CPlayViewToolbar)

public:
	CPlayViewToolbar();
	virtual ~CPlayViewToolbar();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CButtonST_Skin m_btnRecord;
	CButtonST_Skin m_btnFullScreen;
	CButtonST_Skin m_btnPreview;
	CButtonST_Skin m_btnCapture;
	CButtonST_Skin m_btnSound;
	CButtonST_Skin m_btnStream;
	CButtonST_Skin m_btnOSD;
	CButtonST_Skin m_btnPhoneWave;

	BOOL m_bVideoUser;
	CString m_strVideoMsg;

	CWnd *		m_pParrentWnd;					//������ָ��
	BOOL		m_bIsInit;						//���ڴ�����ϱ�־

	BOOL m_bRecording;
	BOOL m_bPreviewing;
    BOOL m_bSounding;
	BOOL m_bMainStreaming;
	BOOL m_bShowOsding;
public:
	BOOL CreateWnd(const RECT & rect, CWnd * pParent, unsigned int uiID);

	CScope	m_scpVideoWav;
	CScope	m_scpAudioWav;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	LRESULT OnClkToolBarBtn(WPARAM wparam, LPARAM lparam); 
	afx_msg void OnClickSetPlayPortion(); 
	// button����¼�
	afx_msg void OnBnClickedPlayviewtoolbarFullScreen();
	afx_msg void OnBnClickedPlayviewtoolbarRecord();
	afx_msg void OnBnClickedPlayviewtoolbarPreview();
	afx_msg void OnBnClickedPlayviewtoolbarCapture();
	afx_msg void OnBnClickedPlayviewtoolbarSound();
	afx_msg void OnBnClickedPlayviewtoolbarStream();
	afx_msg void OnBnClickedPlayviewtoolbarOSD();

public:
	// �����Ӵ���
	void CreateChildWnd(void);
	// ��ʼ��λ��
	void InitPose(void);
	// ��ʼ���ؼ�
	void InitCtrl(void);
	// ����ȫ��
	void SetFullScreenON(void);
	// ��ֹȫ��
	void SetFullScreenOff(void);
	// ���button
    void CheckShowingBtns();
	// ��Ӱ�ť��ʾ
	void SetButtonToolTip();
    // ���°�ť��ʾ
    void UpdateToolTips();
	// ����¼��
	void ResetVideo(void);
	// ����Ԥ��״̬
	void SetPreviewStatus(BOOL bPreviewing = TRUE, int nStreamType = 0);
	// ����¼��״̬
	void SetRecordStatus(BOOL bPreviewing = TRUE);

	// ���°�ť��ʾ
	void UpdateRecordTips(BOOL bRecord);
	// ����������ť
	void SetSoundStatus(BOOL bSounding);
	// ��������������ť
	void SetStreamStatus(BOOL bMainStreaming);
	// ����OSD��ť
	void SetOsdStatus(BOOL bShowOsding);
	// ����Ԥ����ť��ʾ
	void UpdatePreviewTips(BOOL bPreviewing);
	// ����������ť��ʾ
    void UpdateSoundTips(BOOL bSounding);
	// ������������
	void UpdateStreamTips(BOOL bStream);
	// ����OSD����
	void UpdateOsdTips(BOOL bOsd);

	// ���Ԥ��¼�����
	void OnBnClickedPlayviewtoolbarRecordRef();
	// ��/����¼��ť
	void OnBnRecordPlayviewtoolbarEnableRef(BOOL bEnale, const CString& strMsg);
	// ˢ�°�ť״̬
	void UpdateBtnState(int nMenuID, int nState);
};


