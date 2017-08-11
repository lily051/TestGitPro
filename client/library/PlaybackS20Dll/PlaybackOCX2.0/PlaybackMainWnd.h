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
	// ����ָ�ģʽ.
	PBSDLayout m_emLayout;
	// Ƥ������.
	PBSDSkinType m_emSkinType;
	// ������.
	DWORD m_dwStyle;
	// PBSD���.
	HANDLE m_hDlg;
	// �Ƿ�������.
	LONG m_lBwControl;
	// IP.
	CString m_lpstrIP;
	// �˿�.
	LONG m_lPort;
	// �û�����.
	LONG m_lUserLevel;


private:
	// �����طŲ����ַ���.
	BOOL ParsePlaybackXML(CString strXML, int& nWndNo, DWORD& dwRight, PBSDLoginParam& rLoginInfo, PBSDPlayParam& rPlayParam);
	// �������������ַ���.
	BOOL ParseParamXML(LPCTSTR lpstrXML, CString& strPicPath, CString& strDownloadPath, int& nPicType, int& nShowTitile);
public:
	// ��ʼ�ط�.
	LONG Playback(LPCTSTR lpstrXML);
	// ��ʼ���ػط�.
	LONG LocalPlayback(LPCTSTR lpstrXML);
	// ֹͣ�ط�.
	LONG StopPlayback(LONG lWndNo);
	// ȫ��ֹͣ.
	LONG StopAll();
	// ���ò���.
	LONG SetParameter(LPCTSTR lpstrXML);
	// ��ȡ�ط�״̬��Ϣ.
	CString GetPlaybackInfo(LONG lWndNo);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


