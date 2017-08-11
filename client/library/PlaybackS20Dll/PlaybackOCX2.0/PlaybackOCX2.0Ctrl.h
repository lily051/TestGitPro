#pragma once

// PlaybackOCX3.0Ctrl.h : CPlaybackOCX30Ctrl ActiveX �ؼ����������

#include "PlaybackMainWnd.h"
// CPlaybackOCX30Ctrl : �й�ʵ�ֵ���Ϣ������� PlaybackOCX3.0Ctrl.cpp��

class CPlaybackOCX30Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CPlaybackOCX30Ctrl)

// ���캯��
public:
	CPlaybackOCX30Ctrl();

// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

private:
	CPlaybackMainWnd* m_pPlaybackMainWnd;

// ʵ��
protected:
	~CPlaybackOCX30Ctrl();

	DECLARE_OLECREATE_EX(CPlaybackOCX30Ctrl)    // �๤���� guid
	DECLARE_OLETYPELIB(CPlaybackOCX30Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CPlaybackOCX30Ctrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(CPlaybackOCX30Ctrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
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