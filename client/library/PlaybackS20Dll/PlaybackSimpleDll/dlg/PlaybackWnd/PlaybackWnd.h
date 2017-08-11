#pragma once
#include "PlayTitleWnd.h"
#include "PlayVedioWnd.h"
#include "../PlaybackOper.h"
#include "PlayMiniToolBarWnd.h"
#include "PlayMiniToolBarExpandWnd.h"
#include "../CaptureTipDlg.h"
#include "../SaveDataDlg.h"
#include <vector>
#include "../OrderPlay.h"

/*****************************************************************
*   Copyright 2011-2013 Hikvision Digital Technology Co., Ltd.
*   FileName:       PlaybackWnd.h
*   Description:    �طſؼ������࣬���༴��Ϊһ���طŵĴ���
*   Author:         yudan
*   Date:			2011-1-27 16:32
*   Modification History:
*    <version>      <time>          <author>        <desc>
      v1.1          2011-1-27 16:33   yudan          create
	  v1.2          2012-9-28 11:01   yudan  �ټ����� modify
	  v1.3			2013-05-14 09:27  zpz			 modify
*****************************************************************/

//��ʱ�����
#define TIMER_GET_PLAYPOS 1905
#define TIMER_GET_PLAYOSDTIME 1907
#define GET_PLAYPOS_TIME 1000
#define GET_PLAYOSDTIME_TIME 1000
//menu�˵�ID
enum
{
	MENU_ID_SHOWTITLE = WM_USER + 1800,
	MENU_ID_ORIGINAL = WM_USER + 1801,			// ԭʼ����.
	MENU_ID_DOWNLOADLIST = WM_USER + 1803,
	MENU_ID_PLAYAGAIN = WM_USER + 1804,
	MENU_ID_4_3 = WM_USER + 1805,				// 4:3.
	MENU_ID_16_9 = WM_USER + 1806,
	MENU_ID_PLAY = WM_USER + 1807,
	MENU_ID_STOP = WM_USER + 1808,
	MENU_ID_SLOW = WM_USER + 1809,
	MENU_ID_FAST = WM_USER + 1810,
	MENU_ID_SINGLEFRAME = WM_USER + 1811,
	MENU_ID_DOWNLOAD_BY_TIME = WM_USER + 1812,
	MENU_ID_CAPTURE = WM_USER + 1813,
	MENU_ID_SAVE = WM_USER + 1814,
	MENU_ID_OPENSOUND = WM_USER + 1815,
    MENU_ID_CONVERTPLAY = WM_USER + 1816,
	MENU_ID_FULLSCREEN = WM_USER + 1817,
};

enum
{
	OPERA_ID_CAPTURE			= 0,
	OPERA_ID_SLOW ,
	OPERA_ID_PLAY,
	OPERA_ID_FAST,
	OPERA_ID_SOUND,
	OPERA_ID_FULLSCREEN,
	OPERA_ID_STOP,
	OPERA_ID_SLIDER,
	OPERA_ID_SOUNDVALUE,
	OPERA_ID_NOTICE
};

// ��Ӧ�߳��޸Ľ�����Ϣ.
#define WM_UPDATE_MSG (WM_USER + 1900)


//��ɫ����
#define COLOR_BORDER_SELECT UI_PLAYWNDSEL_FRAME  //ѡ��״̬��ɫ
#define COLOR_BORDER_NOSELECT UI_PLAYWND_SPACE  //��ѡ��״̬����ɫ
//���ݶ���
#define TITLE_WND_HEIGHT 16      //�������߶�


typedef enum _tagPlayType
{
	PLAY_TYPE_TIME = 1,  //��ʱ��ط�
	PLAY_TYPE_FILE,      //���ļ��ط�
}PLAY_TYPE;

/** @fn pOnMsgNeedRsp
*   @brief osdʱ��
*   @param[in] lOsdTime: osdʱ��
*   @param[in] unWndID: ����ID
*/
typedef void (CALLBACK *pFunMsgOsdTime)(LONG lOsdTime, UINT unWndID, LPVOID lpvoid);


class CPlaybackWnd : public CWnd
{
	DECLARE_DYNAMIC(CPlaybackWnd)
public:
	CPlaybackWnd(HANDLE hDLg, void* pParent = NULL);
	~CPlaybackWnd();

private:
    void* m_pParent;				 //������ָ��
	CPlaybackOper m_opePB;			 // �طſ�����.
	COrderPlay m_OrderPlay;			 // �����ط��ļ�������.
	COLORREF m_clrBorder;            //�߿���ɫ
	BOOL m_bIsMax;                   //�ؼ��Ƿ����
	BOOL m_bMove;                    //�Ƿ������ƶ��ؼ�

	HANDLE m_hDlg;					 // �Ի�����.
	CPlayTitleWnd* m_pPlayTitleWnd;  //�طŴ��ڱ���ؼ�
	CPlayVedioWnd* m_pPlayVedioWnd;  //�طŴ�����Ƶ�ؼ�
	CPlayMiniToolBarWnd* m_pToolBarWnd; // �طŴ��ڹ�����.
	CPlayMiniToolBarExpandWnd* m_pToolBarExpandWnd;	// �ط���չ������.
	CCaptureTipDlg * m_pCaptureTip;  //ͼƬչʾ����
	CSaveDataDlg* m_pSaveData;		// �طż�����Ϣ����.
	CButtonST_Skin *m_pbtnClose;		 //�رմ��ڰ�ť
	BOOL m_bShowTitle;               //�Ƿ���ʾ����
	CRect m_rcClient;                //���ڴ�С
	ShowProportion m_emShowPro;      // ��ʾ����.
	BOOL m_bIsInitTask;			     // �Ƿ��ʼ��������.
public:
	UINT m_unWndID;                  //���ڰ����
	HWND m_hRecvMsg;                 //��Ϣ���մ��ھ��
	BOOL m_bFullScreen;              //�Ƿ�ȫ����
	BOOL m_bSynPlay;                 //�Ƿ�ͬ���ط���
	BOOL m_bReversePlay;			 // �Ƿ��ǵ���ģʽ.
	BOOL m_bSelect;                  //��ǰ�����Ƿ�ѡ��
	BOOL m_bUsed;                    //��ǰ�����Ƿ�ʹ�ù�

    //��ȡ�豸¼���������
    CString m_strAbilityXml;
    void GetDeviceRecordAbility();
    void ParseDeviceRecordAbility();

public:
	BOOL m_bPause;                   //�Ƿ���ͣ��
	BOOL m_bSoundOpen;               //�����Ƿ��
	BOOL m_bSaveData;                //�Ƿ񱣴�������
	BOOL m_bSingleSound;             //�Ƿ��ǵ���Ƶ����
 	ENUM_CONTROL_TYPE m_emPlaySpeed; //�ط��ٶ�
	ENUM_CONTROL_TYPE m_emSynPlaySpeed;	// ͬ���ط��ٶ�.
	HANDLE m_hPlayback;              //�طŷ���ID
	PB_TIME m_tStartTime;            //��ǰ�طſ�ʼʱ��(��ʱ��ط���Ч)
	PB_TIME m_tEndTime;              //��ǰ�طŽ���ʱ��(��ʱ��ط���Ч)
	LONG m_lEndTime;                 //��ǰ�طŽ���ʱ��
	LONG m_lBeginTime;				 // ��ǰ�طſ�ʼʱ��.
	CString m_strPlayFile;           //��ǰ�ط��ļ���(���ļ��ط���Ч)
	CString m_strChanName;           //��ǰ�طŵļ�ص�����
	LONG m_lPlayPos;                 //�طŽ���
	int m_nVolumn;                   //��ǰ����
	PB_LOGININFO m_stLoginInfo;      //�豸��¼����
	PLAYBACK_TIME_INFO m_stPlayTimeParam;  //ʱ��طŲ���
	PLAYBACK_FILE_INFO m_stPlayFileParam;  //�ļ��طŲ���
	FINDFILE_PARAM m_stFindParam;	 // �����ļ�����.
	int m_nCameraID;                 //��ǰ�ط���Ϣ�ļ�ص�ID
	LONG m_lChannel;					 // ��ǽ�ط���Ϣ��ͨ����.
	PLAY_TYPE m_emPbType;          //�طŲ���
	ENUM_PLAY_TYPE m_emPlayType;   // �ط�����.
	pFunMsgOsdTime m_pSetOsdTime;    //����OSDʱ��ص�
	LPVOID m_lpSetOsdTime;           //����OSD�ص��Զ������
	LONG m_lOsdTime;                 //��ǰ���ŵ�osdʱ��
	BOOL m_bOperating;				// �Ƿ����ڲ�����.
	BOOL m_bIsSingleFrame;			// �Ƿ����ڽ��е�֡����.
	DWORD m_dwRight;				// Ȩ�޿���.
	DWORD m_dwRev;					// Ԥ���ֶ�.
	CString m_strSaveDataPath;		// IE����ģʽ�µļ�����ʱ·��.
	HANDLE m_hSaveDataState;		// IE����ģʽ�µļ����ļ��Ի�����.
	CString m_strFilePath;			// �طż����ļ�·��.
	HANDLE m_hCheckSpaceThread;		 // �����̿ռ��߳�.
	HANDLE m_hStopCheck;			 // ֹͣ�����̿ռ�.
	ThreadDataCheckSpace m_stCheckData;	// ���̼���̲߳���.
	BOOL m_bPlayNextFile;			// �Զ�������һ���ļ�.
//�طŴ�����
public:
	static void CALLBACK DrawOnWnd(LONG nPort, HDC hDC, LONG nUser);
	//���ڲ�����ʼ��
	void InitWnd(UINT unWndID, PLAY_LAYOUT_WND emLayoutWnd);
	//���������ͷ�
	void clear();
 	//��ʱ��ط�
 	BOOL PlaybackByTime(
		const PB_LOGININFO& stLoginInfo, 
		const PLAYBACK_TIME_INFO& stPlaybackParam, 
		const FINDFILE_PARAM& stFindParam, 
		CString strChanName, 
		int nCameraID = -1, 
		DWORD dwRight = PBSD_RIGHT_ALL,
		DWORD dwRev = 0);
 	//���ļ��ط�
 	BOOL PlaybackByFile(
		const PB_LOGININFO& stLoginInfo, 
		const PLAYBACK_FILE_INFO& stPlaybackParam, 
		CString strChanName, 
		int nCameraID = -1, 
		DWORD dwRight = PBSD_RIGHT_ALL,
		DWORD dwRev = 0);
 	//�طŸı����
 	BOOL PlaybackSetPos(const PB_TIME& stCurTime, const PB_TIME& stStartTime, const PB_TIME& stStopTime);
 	//ֹͣ�ط�
 	BOOL StopPlayback(BOOL bAutoStop = FALSE, BWControlType emBwControl = BWCONTROL_NOCONTROL);
	// �����˳�ʱֹͣ�ط�.
	BOOL DestroyStopPlayback();
 	//��ͣ�ط�
 	BOOL PausePlayback();
 	//�ָ��ط�
 	BOOL RePlayback();
 	//�ı�ط��ٶ�
 	BOOL SetPlaySpeed(ENUM_CONTROL_TYPE enumPlaySpeed, BOOL bSynPlay = FALSE);
	// ��ȡ�ط��ٶ�.
	ENUM_CONTROL_TYPE GetSynPlaySpeed() const;
 	//������
 	BOOL OpenPlaySound(DWORD dwSound = 50);
 	//�ر�����
 	BOOL ClosePlaySound();
 	//�ı�������С
 	BOOL SetPlayVolume(DWORD dwSound);
	//��ͼ
	BOOL CapturePic(CString& strPic);
 	//��������
 	int SaveData(CString& strPath);
 	//ֹͣ��������
 	BOOL StopSaveData();
	// ��ȡ�ط�״̬��Ϣ.
	BOOL GetPlaybackInfo(PlaybackStatData& stData);
	//����osd�ص���Ϣ
	void SetOsdCallback(pFunMsgOsdTime pSetOsdTime, LPVOID lpvoid);
	//�����Ƿ�ͬ��
	void SetSynPlay(BOOL bSynPlay);
	// ��֡����.
	void PlaySingleFrame(BOOL bPlay, BOOL bSynPlay = FALSE);
	// �Ƿ����ڵ�֡�ط�.
	BOOL IsSingleFrame() const;
	// �Ƿ����ڽ��лط�.
	BOOL IsPlayback();
	// �Ƿ��Ѿ�ʹ�ù��ô���.
	BOOL IsUsed();
	// ��ȡ��ص�ID.
	int GetCameraID();
	// ��ȡ�����ֶ���Ϣ.
	DWORD GetRevInfo();
	// ��ȡ�ļ��б�.
	std::vector<PB_FILEINFO>& GetFileInfo();
	// �����ļ��б�.
	BOOL SetFileInfo(const std::vector<PB_FILEINFO>& vFileInfo);
	// ��ȡ�����ļ���Ϣ
	BOOL GetFileInfo(int nIndex, PB_FILEINFO& stFileInfo);
	// ��ȡȨ��.
	DWORD GetRight();
	// ��ȡʱ����Ϣ.
	PLAYBACK_TIME_INFO& GetPlayTimeInfo();
	// ��ȡ�ļ���Ϣ.
	PLAYBACK_FILE_INFO& GetPlayFileInfo();
	// ��ȡ�ļ�������Ϣ.
	FINDFILE_PARAM& GetFindInf();
	// ��ȡ��ص�����.
	CString& GetCamName();
	// ��ȡͨ����.
	LONG GetChannel();
	// ��ȡ�ط�����.
	ENUM_PLAY_TYPE GetPlayType();
	// ��ȡ��¼��Ϣ.
	PB_LOGININFO& GetLoginInfo();
	// ���°�ťtips.
	void UpdateButtonTips();
	// �ظ��ط�.
	void PlayAgain();
	// ���ûط�.
	void Invalidate();
	// �Ƿ��ǵ���״̬.
	BOOL IsReversePlay();
	// ��ȡ��ǰ�����ļ����.
	BOOL GetCurPlayFileIndex(int& nPlayIndex);
	// ���õ�ǰ�����ļ����.
	void SetCurPlayFileIndex(int nPlayIndex);
	// ���ٲ���.
	void PlaySlow();
	// ���ٲ���.
	void PlayFast();

private:
	//��ʼ�ط�
	void StartPlay(BOOL bFindFile);
 	//��ȡ�ط�OSDʱ��
    LONG GetOsdTime();
 	//��ȡ�طŽ���
 	LONG GetPlayPos();
	//���Ͱ�ť״̬ˢ����Ϣ
	void PostUpdateMsg();
	// ���ع�����.
	void HideToolBar();
	// �����̿ռ��̺߳���.
	static DWORD WINAPI ThreadFuncCheckSpace(LPVOID pParam);
	// ��ʾ������Ϣ.
	void ShowErrorMsg(const CString& strDefMsg);


//�ؼ�������
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClosePlayWindow();//ֹͣ����
	LRESULT OnUpdateMsg(WPARAM wParam, LPARAM lParam);
	// �����ļ�ʧ��.
	LRESULT OnMsgFindFail(WPARAM wParam, LPARAM lParam);
	// �����ļ��ɹ�.
	LRESULT OnMsgFindSuccess(WPARAM wParam, LPARAM lParam);
	// �طųɹ�.
	LRESULT OnMsgPlaySuccess(WPARAM wParam, LPARAM lParam);
	// �ط�ʧ��.
	LRESULT OnMsgPlayFail(WPARAM wParam, LPARAM lParam);
	// ������������.
	LRESULT OnMsgBWLimit(WPARAM wParam, LPARAM lParam);
	// ֹͣ�ط�.
	LRESULT OnMsgStopPlay(WPARAM wParam, LPARAM lParam);
	// ���������豸.
	LRESULT OnMsgPlaying(WPARAM wParam, LPARAM lParam);
	// ���ûطŽ��ȳɹ�.
	LRESULT OnMsgLocatePlaySuccess(WPARAM wParam, LPARAM lParam);
	// ���ûطŽ���ʧ��.
	LRESULT OnMsgLocatePlayFail(WPARAM wParam, LPARAM lParam);
	// ������Ʒ���������Ͽ��ط�.
	LRESULT OnMsgBwStopPlay(WPARAM wParam, LPARAM lParam);
	// ����뿪��Ƶ������Ϣ.
	LRESULT OnMsgMouseLeaveMiniToolbar(WPARAM wParam, LPARAM lParam);
	// ��������Ƶ������Ϣ.
	LRESULT OnMsgMouseMoveMiniToolbar(WPARAM wParam, LPARAM lParam);
	// ��������ť�����Ϣ.
	LRESULT OnMsgToolButtonClick(WPARAM wParam, LPARAM lParam);
	// ��չ�������ر���Ϣ.
	LRESULT OnMsgToolBarExpandClose(WPARAM wParam, LPARAM lParam);
	// ���̿ռ䲻��.
	LRESULT OnMsgCheckSpaceNotEnough(WPARAM wParam, LPARAM lParam);
	// û���ҵ������������ļ�����.
	LRESULT OnMsgFindNoFile(WPARAM wParam, LPARAM lParam);
	// ˢ�´�����Ϣ��Ӧ.
	LRESULT OnMsgInvalidateWindow(WPARAM wParam, LPARAM lParam);
	
public:
	//������ʾ
	void SetTitleText(CString strTitleText);
	//�ƶ��ؼ�
	void MoveCtrl();
	//���ݻط�״̬��ʾ����
	void SetTitleText();
	//��ȡ����
	CString GetTitleText();
	//��ʾȡ��������
	void ShowTitle(BOOL bShowTitle = TRUE);
	//��ʾ�����İ���Ϣ
	void ShowWndInfo(CString strMsg);
	//���ô���ID
	void SetWndID(UINT unWndID);
    //���ô��ڱ߿���ɫ
	void SetBorderClr(BOOL bSet);
	//�ı���Ϣ���վ��
	BOOL SetMsgRecvHwnd(HWND hWnd);
	//�ı���Ƶ���ڱ���
	void SetVedioOriginal();
	// ���ûطž��.
	void SetPlayHandle(HANDLE hPlayHandle);

private:
	//�߿����
	void DrawBorder(COLORREF clrBorder);
	// ����ֹͣ����.
	void SetStopTextShow(CString strVideo, BOOL bErase = TRUE);
	// ���ô��ڵ�������.
	void SetTextOnWnd(CString strVideo);
	// ����������.
	void OnMiniToolbarControlPlay();
	// ��������ʱ������.
	void OnMiniToolbarDownload();
	// ��������������.
	void OnMiniToolbarControlSave();
	// ��������չ.
	void OnMiniToolbarControlExpand();

public:
    // ��������ͼ.
    void OnMiniToolbarControlCapture();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
