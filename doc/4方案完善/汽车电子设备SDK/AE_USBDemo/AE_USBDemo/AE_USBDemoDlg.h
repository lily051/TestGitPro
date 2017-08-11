
// AE_USBDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "GeneralPlayerAPI.h"


// CAE_USBDemoDlg �Ի���
class CAE_USBDemoDlg : public CDialog
{
// ����
public:
	CAE_USBDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_AE_USBDEMO_DIALOG };

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
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnDestroy();
    CComboBox m_comboDevList;
    afx_msg void OnBnClickedBtnLogin();
    USB_DEVICE m_hDevice;   //��ǰ�豸
    USB_HANDLE m_hLoginHandle;  //��¼���
    STREAM_HANDLE m_nStreamHandle; //ʵʱԤ�����
    LONG m_lPort;   //GPAPI���ſⲥ�Ŷ˿�

    //�豸״̬�ص�����
    static void OnDevStatus(USB_HANDLE pHandle, int nStatus, void* pUser);
    afx_msg void OnBnClickedBtnStartPreview();
    //ʵʱԤ���ص�����
     static void OnRecvRealPlay(STREAM_HANDLE hRealPlayHandle, int nDataType, char* pBuffer, int nRetLen, void* pUser);
     afx_msg void OnBnClickedBtnLogout();
     afx_msg void OnBnClickedBtnStopPreview();
     BOOL StartPlay(PLAYERTYPE playerType, PBYTE pBuffer, DWORD dwBufferSize, CString strCamName /*= _T("")*/);
     BOOL DealDecodeData(long nPort,char * pBuf,long nSize,void * pFrameInfo);
     //���Ż���dc�ص�����
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
