#pragma once

#define IDD_PLAYBACKMAINWND 1002 
#define WM_WRITE_WORKLOG WM_USER + 1003
// CPlaybackMainWnd

class CPlaybackMainWnd : public CWnd
{
	DECLARE_DYNAMIC(CPlaybackMainWnd)

public:
	CPlaybackMainWnd(PBSDLayout emLayout, PBSDSkinType emSkinType, DWORD dwStyle, LPCTSTR lpstrIP, LONG LPort, LONG lUserLevel);
	virtual ~CPlaybackMainWnd();

protected:
	DECLARE_MESSAGE_MAP()

private:
	// 画面分割模式.
	PBSDLayout m_emLayout;
	// 皮肤类型.
	PBSDSkinType m_emSkinType;
	// 组件风格.
	DWORD m_dwStyle;
	// PBSD句柄.
	HANDLE m_hDlg;
	// 是否带宽控制.
	LONG m_lBwControl;
	// IP.
	CString m_lpstrIP;
	// 端口.
	LONG m_lPort;
	// 用户级别.
	LONG m_lUserLevel;


private:
	// 解析回放参数字符串.
	BOOL ParsePlaybackXML(CString strXML, int& nWndNo, DWORD& dwRight, PBSDLoginParam& rLoginInfo, PBSDPlayParam& rPlayParam);
	// 解析参数设置字符串.
	BOOL ParseParamXML(LPCTSTR lpstrXML, CString& strPicPath, CString& strDownloadPath, int& nPicType, int& nShowTitile);
public:
	// 开始回放.
	LONG Playback(LPCTSTR lpstrXML);
	// 开始本地回放.
	LONG LocalPlayback(LPCTSTR lpstrXML);
	// 停止回放.
	LONG StopPlayback(LONG lWndNo);
	// 全部停止.
	LONG StopAll();
	// 设置参数.
	LONG SetParameter(LPCTSTR lpstrXML);
	// 获取回放状态信息.
	CString GetPlaybackInfo(LONG lWndNo);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


