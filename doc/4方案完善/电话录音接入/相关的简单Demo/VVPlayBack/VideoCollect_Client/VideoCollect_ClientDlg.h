
// VideoCollect_ClientDlg.h : 头文件
//
#pragma once
#include "DevInfor.h"
#include "BrowseDir.h"
#include "MySqlDBLayer.h"
#include "afxwin.h"
#include "fcConfigParser.h"
#include "HPR_FileSystem.h"
#include "HPR_Error.h"
#include "CWMPPlayer4.h"    //播放头文件  
#include "CWMPControls.h"   //控制头文件 
// 回放预设
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
// CVideoCollect_ClientDlg 对话框
class CVideoCollect_ClientDlg : public CDialog
{
// 构造
public:
	CVideoCollect_ClientDlg(CWnd* pParent = NULL);	// 标准构造函数
    ~CVideoCollect_ClientDlg();
    void OnOK();
    void OnCancel();
// 对话框数据
	enum { IDD = IDD_VIDEOCOLLECT_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    virtual BOOL PreTranslateMessage(MSG* pMsg);  // PreTranslateMessage是消息在送给TranslateMessage函数之前被调用的
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonLogin();
    afx_msg void OnBnClickedButtonPlay();                                   //播放按钮
    afx_msg void OnClinkSelectTreeChan(NMHDR *pNMHDR, LRESULT *pResult);    //单击选中通道
    afx_msg void OnDoubleHitSelectTreeChan(NMHDR *pNMHDR, LRESULT *pResult);//双击选中通道且播放
    //
public:
    bool DoLogin();                              //登录函数
    void DoGetDeviceResoureCfg();                //设备资源获取
    void CreateDeviceTree();                     //创建通道树
    void StartPlay(int& iChanIndex);             //开始预览
    void StopPlay(LONG& m_lPlayHandle);          //停止预览
    void StartPlayBack();                        //开始回放
    void StopPlayBack(LONG& m_lPlayBackHandle);  //停止回放

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

    LOCAL_DEVICE_INFO m_struDeviceInfo;    //登录成功信息保存

    HTREEITEM m_hDevItem;
    int m_iCurChanIndex;                   //当前通道在数组中索引
    CTreeCtrl	m_ctrlTreeChan;            //节点数里通道树

    volatile bool m_bLogin;                //登录逻辑
    volatile bool m_bPlaying;
    volatile bool m_bPlayingBack;
    volatile bool m_bPlayingBackPause;     //暂停回放
    LONG m_lPlayHandle;                    //预览句柄
    LONG m_lPlayBackHandle;                //回放句柄

    CDateTimeCtrl	m_TimeStart;
    CDateTimeCtrl	m_TimeEnd;
    CDateTimeCtrl	m_DateEnd;
    CDateTimeCtrl	m_DateStart;

    HANDLE m_hThreadVideoPlayerBack;
    HANDLE m_hThreadVoicePlayerBack;
    HICON m_hStopEnableIcon;
    HICON m_hStopDisableIcon;

    CWMPPlayer4 m_Player;
    CWMPControls m_control;  //控制按钮关联
public:
    afx_msg void OnBnClickedButtonPlayback();
    afx_msg void OnBnClickedButtonPlaybackOut();
};
