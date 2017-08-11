#pragma once
#include "../ui/uiBase/ButtonST_Skin.h"

//���ϰ�ť����
#define PVWND_TOOLBAR_BTN_MAXNUM    8

// CRealWndToolBar �Ի���

class CPlayMiniToolBarWnd : public CWnd
{
	DECLARE_DYNAMIC(CPlayMiniToolBarWnd)

public:
	CPlayMiniToolBarWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlayMiniToolBarWnd();

protected:
    DECLARE_MESSAGE_MAP()
	afx_msg void OnToolBarBtnClkPlay();
	afx_msg void OnToolBarBtnClkStop();
	afx_msg void OnToolBarBtnClkSlow();
	afx_msg void OnToolBarBtnClkFast();
	afx_msg void OnToolBarBtnClkFrame();
    afx_msg void OnToolBarBtnClkCapture();
    afx_msg void OnToolBarBtnClkSave();
    afx_msg void OnToolBarBtnClkSound();
	afx_msg void OnToolBarBtnClkDownload();
	afx_msg void OnToolBarBtnClkExpand();
    
	virtual void PreSubclassWindow();

private:
	CButtonST_Skin m_btnPlay;
	CButtonST_Skin m_btnStop;
	CButtonST_Skin m_btnSlow;
	CButtonST_Skin m_btnFast;
	CButtonST_Skin m_btnFrame;
	CButtonST_Skin m_btnDownload;
    CButtonST_Skin m_btnCapture;
    CButtonST_Skin m_btnSave;
    CButtonST_Skin m_btnSound;
	CButtonST_Skin m_btnExpand;
   
    CBitmap m_hBkImage;                 ///< ����ͼ
    CDC m_dcBuffer;
	BOOL m_bReverse;					// �Ƿ��ǵ���״̬.
	BOOL m_bPause;						// �Ƿ�����ͣ״̬.
	BOOL m_bOpenSound;					// �Ƿ��������.
	BOOL m_bSave;						// �Ƿ����ڼ���.
	BOOL m_bIsSingle;					// �Ƿ��ǵ�֡�ط�.

public:
	afx_msg void OnPaint();	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void CreateChildWnd();
	void InitCtrl();
	void InitPos();
	void OnBtnClick(UINT nID);
    void SetToolBarBtnTips();

    /** @fn         DrawBkImage
     *  @brief      ���Ʊ���ͼƬ����OnPaint����
     *  @return     void
     */
    void DrawBkImage();

	// ��ȡ��Ƶ��ť�Ĵ���λ��.
	void GetSoundBtnRect(CRect& rcSound);
	// ���°�ť״̬.
	void UpdateBtnState(BOOL bPause, BOOL bOpenSound, BOOL bSave, BOOL bIsSingleFrame, BOOL bSingleSound = FALSE);
	// ���°�ťtips.
	void UpdateButtonTips();
	// ���õ���״̬.
	void SetReverseState(BOOL bReverse);


public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();

public:
	HRESULT OnMsgMouseLeaveMiniToolbar(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
};
