/**   @file PlayVedioDlg.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief    �طŸ�������.
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

#include "PlayWndView.h"
#include "PlayToolBarWnd.h"
#include "../ui/PlaybackTimeBar/PlayBarTimeCtrl_For9000.h"
#include "./VolumnSetWnd.h"
#include <vector>
#include "DownloadDlg.h"

// CPlayVedioDlg �Ի���

class CPlayVedioDlg : public CDialog
{
	DECLARE_DYNAMIC(CPlayVedioDlg)

public:
	CPlayVedioDlg(HANDLE hDlg, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlayVedioDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PLAYVEDIO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	HANDLE m_hDlg;
	BOOL m_bMove;  //�Ӵ����Ƿ������ƶ���ʶ
	BOOL m_bFullScreen; //�Ƿ�ȫ��
	CRect m_rcWndPos;  //���ڴ�С
	CDownloadDlg * m_pDownloadDlg;  //ָ��Ψһ�����ش���
	PB_TIME m_stSpanStart;
	PB_TIME m_stSpanStop;

public:
	CPlayWndView * m_pPlayViewWnd;
	CPlayToolBarWnd * m_pToolBarWnd;
    CPlayBarTimeCtrl_For9000 * m_pPlayBarTimeCtrl;
	CVolumnSetWnd * m_pVolumnSetWnd;
	CWnd* m_pFullScreenWnd;
	CWnd* m_pParentWnd;
	time_t m_tMin;
	time_t m_tMax;

private:
	//�����Ӵ���
	void CreateChildWnd();
	//�����ƶ�
	void MoveCtrl();
	//��ʼ��TimeBar
	void InitBarTimeCtrl_For9000();
	//����ȫ���л�
	void WndFullScreenSet();

public:
	// �������ضԻ���ָ��.
	void SetDownloadDlgPoint(CDownloadDlg * pDownloadDlg);
	// ��ȡ������ڴ��ں�.
	int GetCursorWnd();
	// ��ȡѡ�еĴ��ں�.
	int GetSelectedWnd();
	//��ص�ط�
	BOOL Playback(
		int nCameraID, 
		const CString& strCamName, 
		const PLAY_PARAM& stPlayParam, 
		PB_LOGININFO& stLoginInfo, 
		int nWndID = -1, 
		DWORD dwRight = PBSD_RIGHT_ALL,
		DWORD dwRev = 0);
	// ֹͣ�طż�ص�.
	BOOL StopPlayback(int nCameraID);
	// ֹͣ�طż�ص�.
	BOOL StopPlayback();
	// ֹͣ�ط�ָ������.
	void StopPlaybackByWndNo(int nWndNo = -1);
	// ֹͣȫ���طŴ���.
	void StopAll();
	// ��ȡ�ط�״̬��Ϣ.
	BOOL GetPlaybackInfo(int nWndNo, PlaybackStatData& stData);
	//�ط��ļ�
	BOOL PlaybackFile(
		int nCameraID, 
		const CString& strCamName, 
		int nChannel, 
		const char* pStreamID,
		const PB_FILEINFO& stFileInfo, 
		const PB_LOGININFO& stLoginInfo, 
		int nWndID = -1, 
		DWORD dwRight = PBSD_RIGHT_ALL,
		DWORD dwRev = 0);
	//�طŷ���ط�
	BOOL PlayGroup(int nGroupID, const PLAY_PARAM& stPlaybackParam);
	// ���¼�����.
	void Clear(int nWndID);
	//����¼�����
	void BuildRecordGraph(const std::vector<PB_FILEINFO>& vecFileInfo, UINT unElementID = -1);
	//¼������ת��
	int GetGraphRecordType(int nRecordType);
	// ��ȡ�ļ��б�.
	std::vector<PB_FILEINFO>& GetFileInfo(int nWndID);
	// ��ȡ�����ļ���Ϣ.
	BOOL GetFileInfo(int nWndID, int nIndex, PB_FILEINFO& stFileInfo);
	// ��ȡ������ڴ���.
	int GetCurSelectedWnd();
	// ˢ�´���.
	void Invalidate();

	//��������ť�����Ӧ��Ϣ
	LRESULT OnMsgToolButtonClick(WPARAM wParam, LPARAM lParam);
	//¼������Ķ�λ��Ϣ
	LRESULT OnMsgTimeBarSetPos(WPARAM wParam, LPARAM lParam);
	//��������ť״̬ˢ����Ϣ
	LRESULT OnMsgToolBarUpdate(WPARAM wParam, LPARAM lParam); 
	//ȫ���л���Ϣ
	LRESULT OnMsgFullScreen(WPARAM wParam, LPARAM lParam);
	//�����������ڹر���Ϣ
	LRESULT OnMsgVolumnWndClose(WPARAM wParam, LPARAM lParam);
	// ������һ���ļ�.
	LRESULT OnMsgPlayNextFile(WPARAM wParam, LPARAM lParam); 
	// ˢ�·�������.
	LRESULT OnMsgUpdateGroup(WPARAM wParam, LPARAM lParam);
	// �˵��ص�.
	LRESULT OnMenuCallBack(WPARAM wParam, LPARAM lParam);
	// �����ļ��ɹ�.
	LRESULT OnMsgFindSuccess(WPARAM wParam, LPARAM lParam);
	// �����ļ�ʧ��.
	LRESULT OnMsgFindFail(WPARAM wParam, LPARAM lParam);
	// �ļ��б��л�.
	LRESULT OnFileListChange(WPARAM wParam, LPARAM lParam);
	// ��ʱ������.
	LRESULT OnMsgMiniToolbarDownload(WPARAM wParam, LPARAM lParam);
	// �ط�״̬
	LRESULT OnMsgPlayBackState(WPARAM wParam, LPARAM lParam);
	

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
