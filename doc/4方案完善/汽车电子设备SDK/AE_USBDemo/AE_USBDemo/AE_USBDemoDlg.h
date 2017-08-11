
// AE_USBDemoDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "GeneralPlayerAPI.h"


// CAE_USBDemoDlg 对话框
class CAE_USBDemoDlg : public CDialog
{
// 构造
public:
	CAE_USBDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_AE_USBDEMO_DIALOG };

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
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnDestroy();
    CComboBox m_comboDevList;
    afx_msg void OnBnClickedBtnLogin();
    USB_DEVICE m_hDevice;   //当前设备
    USB_HANDLE m_hLoginHandle;  //登录句柄
    STREAM_HANDLE m_nStreamHandle; //实时预览句柄
    LONG m_lPort;   //GPAPI播放库播放端口

    //设备状态回调函数
    static void OnDevStatus(USB_HANDLE pHandle, int nStatus, void* pUser);
    afx_msg void OnBnClickedBtnStartPreview();
    //实时预览回调函数
     static void OnRecvRealPlay(STREAM_HANDLE hRealPlayHandle, int nDataType, char* pBuffer, int nRetLen, void* pUser);
     afx_msg void OnBnClickedBtnLogout();
     afx_msg void OnBnClickedBtnStopPreview();
     BOOL StartPlay(PLAYERTYPE playerType, PBYTE pBuffer, DWORD dwBufferSize, CString strCamName /*= _T("")*/);
     BOOL DealDecodeData(long nPort,char * pBuf,long nSize,void * pFrameInfo);
     //播放画面dc回调函数
     static void CALLBACK DrawFunMsgCB(long nPort,HDC hDC,long lUser);
     void StopPlay();
     BOOL InputData(PBYTE pBuffer,DWORD dwSize,long lFrameType);

     CStatic m_prewiewWnd;
     BOOL m_bRecord;
     afx_msg void OnBnClickedBtnStartRecord();
     afx_msg void OnBnClickedBtnStopRecord();
     afx_msg void OnBnClickedBtnCapture();
     CEdit m_editUser;
     CEdit m_editPwd;
};
