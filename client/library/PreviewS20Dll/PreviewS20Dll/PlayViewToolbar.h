#pragma once
#include "ui\DrawStripFun.h"
#include "ui\ButtonST_Skin.h"
#include "ui\CustomDrawBlackWnd.h"
#include "util\CToolTips.h"
#include "ui/Scope.h"

#define PLAY_TOOLBAR_TIPS_MAXNUM    14

static const UINT RECORD_TIMER = 1; ///< 录像时长定时器

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

	CWnd *		m_pParrentWnd;					//父窗口指针
	BOOL		m_bIsInit;						//窗口创建完毕标志

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
	// button点击事件
	afx_msg void OnBnClickedPlayviewtoolbarFullScreen();
	afx_msg void OnBnClickedPlayviewtoolbarRecord();
	afx_msg void OnBnClickedPlayviewtoolbarPreview();
	afx_msg void OnBnClickedPlayviewtoolbarCapture();
	afx_msg void OnBnClickedPlayviewtoolbarSound();
	afx_msg void OnBnClickedPlayviewtoolbarStream();
	afx_msg void OnBnClickedPlayviewtoolbarOSD();

public:
	// 创建子窗口
	void CreateChildWnd(void);
	// 初始化位置
	void InitPose(void);
	// 初始化控件
	void InitCtrl(void);
	// 启用全屏
	void SetFullScreenON(void);
	// 禁止全屏
	void SetFullScreenOff(void);
	// 检查button
    void CheckShowingBtns();
	// 添加按钮提示
	void SetButtonToolTip();
    // 更新按钮提示
    void UpdateToolTips();
	// 重置录像
	void ResetVideo(void);
	// 设置预览状态
	void SetPreviewStatus(BOOL bPreviewing = TRUE, int nStreamType = 0);
	// 设置录像状态
	void SetRecordStatus(BOOL bPreviewing = TRUE);

	// 更新按钮提示
	void UpdateRecordTips(BOOL bRecord);
	// 更新声音按钮
	void SetSoundStatus(BOOL bSounding);
	// 更新主子码流按钮
	void SetStreamStatus(BOOL bMainStreaming);
	// 更新OSD按钮
	void SetOsdStatus(BOOL bShowOsding);
	// 更新预览按钮提示
	void UpdatePreviewTips(BOOL bPreviewing);
	// 更新声音按钮提示
    void UpdateSoundTips(BOOL bSounding);
	// 更新码流类型
	void UpdateStreamTips(BOOL bStream);
	// 更新OSD类型
	void UpdateOsdTips(BOOL bOsd);

	// 点击预览录像操作
	void OnBnClickedPlayviewtoolbarRecordRef();
	// 启/禁用录像按钮
	void OnBnRecordPlayviewtoolbarEnableRef(BOOL bEnale, const CString& strMsg);
	// 刷新按钮状态
	void UpdateBtnState(int nMenuID, int nState);
};


