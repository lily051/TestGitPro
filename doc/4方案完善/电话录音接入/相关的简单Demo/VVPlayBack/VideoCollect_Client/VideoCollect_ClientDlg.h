
// VideoCollect_ClientDlg.h : ͷ�ļ�
//
#pragma once
#include "DevInfor.h"
#include "BrowseDir.h"
#include "MySqlDBLayer.h"
#include "afxwin.h"
#include "fcConfigParser.h"
#include "HPR_FileSystem.h"
#include "HPR_Error.h"
#include "CWMPPlayer4.h"    //����ͷ�ļ�  
#include "CWMPControls.h"   //����ͷ�ļ� 
// �ط�Ԥ��
#define PLAY_STOP   0
#define PLAY_NORMAL 1
#define PLAY_PAUSE  2
#define PLAY_FAST   3
#define PLAY_SLOW   4

#define CHECK_INTERVAL 10 * 1000
#define CONFIG_FILE                _T("DeviceParam.ini")
#define INI_SEC_AUTH               _T("DeviceParam")
#define KEY_DEVICE_IP              _T("DeviceIp")
#define KEY_DEVICE_PORT            _T("DevicePort")
#define KEY_DEVICE_USERNAME        _T("DeviceUsername")
#define KEY_DEVICE_PASSWORD        _T("DevicePassword")

#define DEFAULT_DEVICE_IP          _T("127.0.0.1")
#define DEFAULT_DEVICE_PORT        _T("8000")
#define DEFAULT_DEVICE_USERNAME    _T("admin")
#define DEFAULT_DEVICE_PASSWORD    _T("12345")
namespace my{
    TCHAR const INI_CFG_FILE_NAME[] = _T("DeviceParam.ini");
    TCHAR const INI_SEC_DEVICE_PARAM[] = _T("DeviceParam");
    TCHAR const INI_SEC_DEVICE_IP[] = _T("DeviceIp");
    TCHAR const INI_SEC_DEVICE_PORT[] = _T("DevicePort");
    TCHAR const INI_SEC_DEVICE_USERNAME[] = _T("DeviceUsername");
    TCHAR const INI_SEC_DEVICE_PASSWORD[] = _T("DevicePassword");
    TCHAR const DEFAULT_SEC_DEVICE_IP[] = _T("127.0.0.1");
    TCHAR const DEFAULT_SEC_DEVICE_PORT[] = _T("8000");
    TCHAR const DEFAULT_SEC_DEVICE_USERNAME[] = _T("admin");
    TCHAR const DEFAULT_SEC_DEVICE_PASSWORD[] = _T("12345");
}
// CVideoCollect_ClientDlg �Ի���
class CVideoCollect_ClientDlg : public CDialog
{
// ����
public:
	CVideoCollect_ClientDlg(CWnd* pParent = NULL);	// ��׼���캯��
    ~CVideoCollect_ClientDlg();
    void OnOK();
    void OnCancel();
// �Ի�������
	enum { IDD = IDD_VIDEOCOLLECT_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    virtual BOOL PreTranslateMessage(MSG* pMsg);  // PreTranslateMessage����Ϣ���͸�TranslateMessage����֮ǰ�����õ�
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonLogin();
    afx_msg void OnBnClickedButtonPlay();                                   //���Ű�ť
    afx_msg void OnClinkSelectTreeChan(NMHDR *pNMHDR, LRESULT *pResult);    //����ѡ��ͨ��
    afx_msg void OnDoubleHitSelectTreeChan(NMHDR *pNMHDR, LRESULT *pResult);//˫��ѡ��ͨ���Ҳ���
    //
public:
    bool DoLogin();                              //��¼����
    void DoGetDeviceResoureCfg();                //�豸��Դ��ȡ
    void CreateDeviceTree();                     //����ͨ����
    void StartPlay(int& iChanIndex);             //��ʼԤ��
    void StopPlay(LONG& m_lPlayHandle);          //ֹͣԤ��
    void StartPlayBack();                        //��ʼ�ط�
    void StopPlayBack(LONG& m_lPlayBackHandle);  //ֹͣ�ط�

    void StartPlayBackVoice();
    void SetPlayState(int iState);
    void StartVideoLinkCheck();
    void GetStartStopTime(NET_DVR_TIME& StartSearchTime, NET_DVR_TIME& StopSearchTime);
    static void DelayBySleep(DWORD dwMilliseconds, volatile bool &stopFlag, DWORD dwInterval = 500);

    void InitConfig();
    void SaveParamLogin();
    void BuildDefaultFile();
private:
    CIPAddressCtrl	m_ctrlDevIp;           //IP
    CEdit m_editDevPort;                   //port
    CEdit m_editDevUserName;               //username
    CEdit m_editDevPassWord;               //password

    CString m_strDevIP;
    CString m_strDevPort;
    CString m_strDevUser;
    CString m_strDevPassword;

    LOCAL_DEVICE_INFO m_struDeviceInfo;    //��¼�ɹ���Ϣ����

    HTREEITEM m_hDevItem;
    int m_iCurChanIndex;                   //��ǰͨ��������������
    CTreeCtrl	m_ctrlTreeChan;            //�ڵ�����ͨ����

    volatile bool m_bLogin;                //��¼�߼�
    volatile bool m_bPlaying;
    volatile bool m_bPlayingBack;
    volatile bool m_bPlayingBackPause;     //��ͣ�ط�
    LONG m_lPlayHandle;                    //Ԥ�����
    LONG m_lPlayBackHandle;                //�طž��

    CDateTimeCtrl	m_TimeStart;
    CDateTimeCtrl	m_TimeEnd;
    CDateTimeCtrl	m_DateEnd;
    CDateTimeCtrl	m_DateStart;

    HANDLE m_hThreadVideoPlayerBack;
    HANDLE m_hThreadVoicePlayerBack;
    HICON m_hStopEnableIcon;
    HICON m_hStopDisableIcon;

    CWMPPlayer4 m_Player;
    CWMPControls m_control;  //���ư�ť����
public:
    afx_msg void OnBnClickedButtonPlayback();
    afx_msg void OnBnClickedButtonPlaybackOut();
};
