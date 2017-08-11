#pragma once

// PlaybackOCX3.0Ctrl.h : CPlaybackOCX30Ctrl ActiveX 控件类的声明。

#include "PlaybackMainWnd.h"
// CPlaybackOCX30Ctrl : 有关实现的信息，请参阅 PlaybackOCX3.0Ctrl.cpp。

class CPlaybackOCX30Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CPlaybackOCX30Ctrl)

// 构造函数
public:
	CPlaybackOCX30Ctrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

private:
	CPlaybackMainWnd* m_pPlaybackMainWnd;

// 实现
protected:
	~CPlaybackOCX30Ctrl();

	DECLARE_OLECREATE_EX(CPlaybackOCX30Ctrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CPlaybackOCX30Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CPlaybackOCX30Ctrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CPlaybackOCX30Ctrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
		dispidlpstrBtnRights = 11,
		dispidPB_LocalPlayback = 10L,
		dispidPB_GetVersion = 9L,
		dispidlModuleStyle = 8,
		dispidlPicType = 7,
		dispidlpstrPath = 6,
		dispidPB_StopAll = 5L,
		dispidPB_StopPlayback = 4L,
		dispidPB_Playback = 3L,
		dispidlSkinType = 2,
		dispidlLayout = 1
	};
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	void OnlLayoutChanged(void);
	LONG m_lLayout;
	void OnlSkinTypeChanged(void);
	LONG m_lSkinType;
	LONG PB_Playback(LPCTSTR lpstrXML);
	LONG PB_LocalPlayback(LPCTSTR lpstrXML);
	BSTR PB_GetRecordFile(LPCTSTR lpstrXML);

	LONG PB_StopPlayback(LONG lWndNo);
	LONG PB_StopAll(void);
	LONG PB_SetParam(LPCTSTR lpstrParam);
	void OnlpstrPathChanged(void);
	CString m_lpstrPath;
	void OnlPicTypeChanged(void);
	LONG m_lPicType;

	void OnlModuleStyleChanged(void);
	LONG m_lModuleStyle;

	void OnlBtnRights(void);
	CString m_lBtnRights;

	void OnlpstrPicPathChanged(void);
	CString m_lpstrPicPath;
	BSTR PB_GetPlaybackInfo(LONG lWndNo);
	void OnlpstrBwInfoChanged(void);
	CString m_lpstrBwInfo;
	
	void OnlUserLevelChanged(void);
	LONG m_lUserLevel;
	void OnlpstrIPChanged(void);
	CString m_lpstrIP;
	void OnlPortChanged(void);
	LONG m_lPort;
	LONG PB_EnableWindow(LONG lEnable);
	void GetFileVersion(const CString& strFileName, CString& strVersion);
	BSTR PB_GetVersion(void);
public:
	virtual BOOL IsInvokeAllowed(DISPID /*dispid*/);
    void CheckCurrentDirectory();
};

extern CPlaybackOCX30Ctrl* g_pCtrl;