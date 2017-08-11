/**   @file PlayWndView.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief    ��طŻ��洰����.
 *
 *    @author   zhoupeiqiang
 *    @date     2012/10/29
 *
 *    @note 
 *    @note 
 *    @note ��ʷ��¼��
 *    @note V1.0.0  
 *
 *    @warning 
 */

#pragma once
#include "./PlaybackWnd/PlaybackWnd.h"

// CPlayWndDlg �Ի���

class CPlayWndView : public CWnd
{
	DECLARE_DYNAMIC(CPlayWndView)
public:
	CPlayWndView(HANDLE hDlg, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlayWndView();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CPlaybackWnd* m_pPlayWnd[MAX_PLAYWND_NUM];
	BOOL m_bMove;    //�Ƿ������ӿؼ��ƶ�
	PLAY_LAYOUT_WND m_curLayWnd;
	//CWnd* m_pFullScreenWnd;  //ȫ����������
	HANDLE m_hDlg;	 // �Ի�����.
public:
	UINT m_unSelectWnd;    //��ǰѡ�еĴ���
	UINT m_unFullWndID;    //˫���Ŵ�Ĵ���
	BOOL m_bSynPlay;       //��ǰ�Ƿ�ͬ���ط���
	BOOL m_bReversePlay;   // �Ƿ����ڵ���.
	BOOL m_bSynTimer;      //ͬ����ʱ���Ƿ���
private:
	//�ƶ��Ӵ���
	void MoveWnd();

	//���յ�������ѡ�е���Ϣ
	LRESULT OnMsgWndSelect(WPARAM wParam, LPARAM lParam);
	//���յ������ڵ�˫����Ϣ
	LRESULT OnMsgWndDbClick(WPARAM wParam, LPARAM lParam);
	//��������ȫ����Ϣ
	LRESULT OnMsgFullScreen(WPARAM wParam, LPARAM lParam);
	//�������ڻط�״̬�ı���Ϣ
	LRESULT OnMsgPlayStateUpdate(WPARAM wParam, LPARAM lParam);
	// ������һ���ļ�.
	LRESULT OnMsgPlayNextFile(WPARAM wParam, LPARAM lParam);
	// �˵��¼�.
	LRESULT OnMenuCallBack(WPARAM wParam, LPARAM lParam);
	// ��Ӧ�����ļ��ɹ���Ϣ.
	LRESULT OnMsgFindSuccess(WPARAM wParam, LPARAM lParam);
	// ��Ӧ�����ļ�ʧ����Ϣ.
	LRESULT OnMsgFindFail(WPARAM wParam, LPARAM lParam);
	// �ر��������ڵ���Ƶ.
	//LRESULT OnMsgMiniToolbarOpenSound(WPARAM wParam, LPARAM lParam);
	// ��ʱ������.
	LRESULT OnMsgMiniToolbarDownload(WPARAM wParam, LPARAM lParam);
	// �����ٶȸı�.
	LRESULT OnMsgPlaybackSpeedChange(WPARAM wParam, LPARAM lParam);
	// ��֡����.
	LRESULT OnMsgPlaybackSingleFrame(WPARAM wParam, LPARAM lParam);

public:
    // �ر��������ڵ���Ƶ.
    void OnMsgMiniToolbarOpenSound(int nWndID);

	//ʱ��ط�
	BOOL PlaybackTime(
		const PB_LOGININFO& stLoginInfo, 
		const PLAYBACK_TIME_INFO& stPlaybackParam, 
		const FINDFILE_PARAM& stFindParam,
		CString strChanName, 
		int nCameraID, 
		int nWndID = -1, 
		DWORD dwRight = PBSD_RIGHT_ALL,
		DWORD dwRev = 0);
	//�ļ��ط�
	BOOL PlaybackFile(
		const PB_LOGININFO& stLoginInfo, 
		const PLAYBACK_FILE_INFO& stPlaybackParam, 
		CString strChanName, 
		int nCameraID, 
		int nWndID = -1, 
		DWORD dwRight = PBSD_RIGHT_ALL,
		DWORD dwRev = 0);
	//ֹͣ�ط�
	void StopPlay(int nWndID = -1);
	// ֹͣ�طż�ص�.
	void StopPlayCam(int nCameraID);
	// ֹͣ�طż�ص�.
	void StopPlayCam();
	//ֹͣȫ��
	void StopAll();
    //ȫ��ץͼ
    void CaptureAll();
	// ��ȡ�ط�״̬��Ϣ.
	BOOL GetPlaybackInfo(int nWndNo, PlaybackStatData& stData);
	//�طŶ�λ
	BOOL SetPlayTime(const PB_TIME& stCurTime, const PB_TIME& stStartTime, const PB_TIME& stStopTime, int nWndID);
	//��ť����¼�
	void OnButtonClick(int nBtnID, LONG lValue);

	//���ô��ڷ���
	void SetWndLayout(PLAY_LAYOUT_WND emLayoutWnd);
	//������ʾ�ַ�
	void ShowWndInfo(CString strInfo, int nWndID = -1);
	//����OSD�ص�����
	void SetOsdCallback(pFunMsgOsdTime pCallbackFun, LPVOID lpvoid);
	//��ȡ��ǰѡ�д���������С
	BOOL GetCurSelWndVolumn(BOOL & bVoice, int& nVolumn);
	//��ȡ��ǰ������ڷ���
	int GetCursorWnd();
	// ��ȡ��ǰѡ�еķ���.
	int GetCurSelectedWnd();
	// ��ȡ�ļ��б�.
	std::vector<PB_FILEINFO>& GetFileInfo(int nWndID);
	// ��ȡ�����ļ���Ϣ.
	BOOL GetFileInfo(int nWndID, int nIndex, PB_FILEINFO& stFileInfo);
	// ��ȡ��ص�ID.
	int GetCameraID(int nWndID);
	// ��ȡ������Ϣ.
	DWORD GetRevInfo(int nWndID);
	// ��ȡ��ص�����.
	CString& GetCamName(int nWndID);
	// ��ȡͨ����.
	LONG GetChannel(int nWndID);
	// ��ȡȨ��.
	DWORD GetRight(int nWndID);
	// ��ȡ�ط�����.
	ENUM_PLAY_TYPE GetPlayType(int nWndID);
	// ��ȡ��¼��Ϣ.
	PB_LOGININFO& GetLoginInfo(int nWndID);
	// ���°�ťtips.
	void UpdateButtonTips();
	// ���ô��ڱ���.
	void Invalidate(int nWndID);
	// �Ƿ��ǵ���״̬.
	BOOL IsReversePlay(int nWndID);
	// ��ȡ��ǰ�����ļ����.
	BOOL GetCurPlayFileIndex(int nWndID, int& nPlayIndex);
	// ���õ�ǰ�����ļ����.
	void SetCurPlayFileIndex(int nWndID, int nPlayIndex);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
