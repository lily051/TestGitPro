#pragma once
#include "afxcmn.h"
#include "PlayVedioDlg.h"
#include "DownloadDlg.h"
// #include "TreeParamDlg.h"
// #include "LabelMgr.h"
// #include "SubSerachDlg.h"
// #include "PlaybackCfg.h"
#include "LocalPlayback.h"
#include "../ui/uiBase/SkinTabCtrl.h"


// CPlaybackMain �Ի���

class CPlaybackMain : public CDialog
{
	DECLARE_DYNAMIC(CPlaybackMain)

public:
	CPlaybackMain(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlaybackMain();

// �Ի�������
	enum { IDD = IDD_DIALOG_PLAYBACK_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bMove;
	MAIN_TAB_ITEM m_emShowDlg;

	// �طŻص�����.
	PBSD_PlaybackCB m_pfnPlayBackCB;
	// �˵��ص�����.
	PBSD_MenuCB m_pfnMenuCB;
	// �ط��Զ�������.
	LPVOID m_pPlaybackUserData;
	// �û��Զ�������.
	LPVOID m_pUserData;
	// д������־�û��Զ�������.
	LPVOID m_pWorkLogUserData;
	BOOL m_bIsLocalPlayback;

public:
	CPlayVedioDlg* m_pDlgPlayVedio;
	CDownloadDlg* m_pDownload;
	//CTreeParamDlg* m_pDlgTreeParam;
	//CLabelMgr* m_pDlgLabelMgr;
	//CSubSerachDlg* m_pDlgSubSerach;
	//CPlaybackCfg* m_pDlgPlaybackCfg;
	CLocalPlayback* m_pDlgLocalPlayback;

private:
	//�Ӵ��ڴ���
	void CreateChildWnd();
	//�ƶ��ؼ�
	void MoveCtrl();
	//tab�ؼ���ʼ��
	BOOL InitTab();
	// �ı���ʾ�ı�ǩҳ.
	void ChangeTab(int nItem);
	// ��ص�ֹͣ�ط�.
	void StopPlayCam(int nResID);
	// ����XML����.
	BOOL BuildFileXML(int nWnd, const std::vector<PB_FILEINFO>& vecFileList, CString& strXML);

public:
	// ��ص�ط�.
	BOOL PlaybackCam(LPPBSDLoginParam pLoginParam, LPPBSDPlayParam pPlayParam, int nWnd = -1, DWORD dwRight = PBSD_RIGHT_ALL);
	// ��ص�ط�.
	BOOL LocalPlayback(const int nVideoType, const CString csFilePath);
	// ָֹͣ���Ĵ���.
	void StopPlayByWndNo(int nWndNo = -1);
	// ֹͣȫ������.
	void StopAll();
	// ���ûطŻص�����.
	void SetPlaybackCB(PBSD_PlaybackCB pfnPlaybackCB, LPVOID pUserData);
	// ���ò˵��ص�����.
	void SetMenuCB(PBSD_MenuCB pfnMenuCB, LPVOID pUserData);
	// ����д������־�ص�.
	void SetWorkLogCallBack(PBSD_WorkLogCB pfnWorkLogCB, LPVOID pUserData);
	// ��ȡ�ط�״̬��Ϣ.
	BOOL GetPlaybackInfo(int nWndNo, PlaybackStatData& stData);
	// ��ص�ֹͣ�ط�.
	void StopPlayCam();
	// ˢ�´���.
	void Invalidate();
public:

	//�����ڼ�ص�ȷ��/˫����Ϣ
	LRESULT OnMsgCameraPlay(WPARAM wParam, LPARAM lParam);
	//�����ڼ�ص���ק��Ϣ
	LRESULT OnMsgCameraDrag(WPARAM wParam, LPARAM lParam);
	//�ļ�������Ӧ��Ϣ
	LRESULT OnMsgFileDownload(WPARAM wParam, LPARAM lParam);
	//�¼�֪ͨ������Ϣ
	LRESULT OnMsgDownloadNotify(WPARAM wParam, LPARAM lParam);
	// �������֪ͨ��Ϣ��ֹͣ�ط�.
	LRESULT OnMsgBWControlStopPlay(WPARAM wParam, LPARAM lParam);
	// ˢ�»طŷ���.
	LRESULT OnUpdateGroup(WPARAM wParam, LPARAM lParam);
	// �˵��ص�.
	LRESULT OnMenuCallBack(WPARAM wParam, LPARAM lParam);
	// �����ļ��ɹ�.
	LRESULT OnMsgFindSuccess(WPARAM wParam, LPARAM lParam);
    // �ļ��б��л���ʾ.
    LRESULT OnFileListChange(WPARAM wParam, LPARAM lParam);
public:
	CSkinTabCtrl m_tabPlay;
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTabPlay(NMHDR *pNMHDR, LRESULT *pResult);
	LRESULT OnDownloadFilePlay(WPARAM wParam, LPARAM lParam);
};
