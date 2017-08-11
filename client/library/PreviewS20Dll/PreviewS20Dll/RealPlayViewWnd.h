/*****************************************************
Copyright 2008-2011 Hikvision Digital Technology Co., Ltd.   
FileName:			RealPlayViewWnd.h
Description:  		Ԥ������
Remark:				
Modification History:      
<version > <time>       <author>  <desc>
1.0        2010-4-19     qmx    ����
*****************************************************/
#pragma once
#include "ui/CustomDrawBlackWnd.h"
#include "RealWndToolBar.h"
#include "RealWaveDlg.h"
#include "CaptureTipDlg.h"
#include "util/Markup.h"


// CRealPlayViewWnd

class CRealPlayViewWnd : public CCustomDrawBlackWnd
{
	DECLARE_DYNAMIC(CRealPlayViewWnd)

public:
	CRealPlayViewWnd();
	virtual ~CRealPlayViewWnd();

protected:
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bIsPreviewing;                       //�Ƿ�����Ԥ��
	BOOL m_bIsRecording;						//�Ƿ���¼��
	BOOL m_bIsSounding;						    //��Ƶ�Ƿ���
	BOOL m_bIsOsdShowing;                       //OSD�Ƿ���ʾ
	BOOL m_bIsOsdShow4Row;                      //OSD4�Ƿ�4����ʾ
	int  m_nCamType;                            //��ص�����
	BOOL m_bFullScreen;                         //�Ƿ�ȫ��״̬
	BOOL m_bIsSteaming;                         //�Ƿ�ȡ���ɹ�
	int  m_nCurStreamType;                      //��ǰ��������: 0-��������1-������
	int  m_nOsdInfoShowNum;                     //OSD��Ϣ��ʾ����
	BOOL m_bIsRecordNormal;                     //¼���Ƿ�����
	BOOL m_bIsOpenPSClient;                     //�Ƿ�����¼��ͨ��

	CString m_strShowMsg;						//��ʾ�ڴ����ϵ�ȡ����Ϣ	
	CString m_strCamName;						//��ص�����	
	int m_nDeviceType;							//�豸����
	HANDLE m_hPreviewhandle;					//ȡ�����
	PVOID m_pStreamHead;						//��ʼ���ŵ�ͷ����
	DWORD m_dwHeadSize;							//��ʼ����ͷ���ݴ�С
	HANDLE m_hRecordFile;						//����¼����ļ����
	LONG m_lPort;								//����Port
	PLAYERTYPE m_playerType;					//��������
	CFont m_FontSmall;							//������
	CFont m_FontBig;							//С����
	CFont* m_pTextFont;							//ָ���/С����

	BOOL m_bIsMouseHover;						//���ͣ���ڴ�����

	NET_DVR_SHOWSTRING_V30 m_stShowString;      //OSD��Ϣ����
	PV_EVENT_FUNC m_pvFunc;
	void* m_pCameraDS;

	CCaptureTipDlg *m_pCapDlg;                  //ץͼ��ʾ�Ի���
	CRect m_rcClient;							//����λ�ã�0��0���Ͻǣ�
	CRealWndToolBar *m_pWndToolBar;			    //�����ڹ�����
	BOOL m_bFirstCleanup;                       //�״�����Ԥ��������Ϣ
	PREVIEW_CAMINFO m_stCamInfo;                //��ص���Ϣ
	HWND m_hPlayViewWnd;                        //CPlayViewWnd���ھ��
	CTime m_dtRecordTime;
	CTime m_dtRecordStartCurTime;
	time_t m_ttStart;
	time_t m_ttEnd;
	LONG m_lUserID;								//��½�豸UserID
	NET_DVR_DEVICEINFO_V30 m_stDevInfo;	        //��½�豸��Ϣ
	RECOED_FILE_INFO m_stLastRecordFile;
	int m_nCamID;								//����Ԥ����ص�ID (�ɹ�Ԥ���ļ�ص�)

	UINT m_uiWndIndex;							//����������
	CScope *m_pVideoWav;						// ��Ƶ���οؼ�
	CScope *m_pAudioWav;						// ��Ƶ���οؼ�
	CString m_strOsdInfo;
	DWORD m_dwRecFileSize;                      //¼���ļ�����
	BOOL m_bFirstShow;                          //��ʱ��һ����ʾ
	COleDateTime m_dtRecordStartTime;
	int m_nRecordTimeOut; //�ܹ���ʱʱ�䣨���ۼӣ�
	int m_nRecordTimeOutOld;  //ԭʼ��ʱʱ�䣨�ɲ������룬�����޸ģ�
	int m_nDelayTime;
	BOOL m_bIsShowCaptureInfo;                  //�Ƿ���ʾץͼ��Ϣ
	LONG m_lLastStreamTime;                     //�豸���һ������ʱ��
	time_t m_tmOld;  //��¼�ϴε��¼���ʱ��
    LONG m_lUSBPort;
    BOOL m_bProIsSound;       //ǰһ����Ƶ״̬
public:
	volatile HANDLE m_hPhoneLogin;

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg LRESULT OnToolBarBtnClk(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNetException(WPARAM wParam, LPARAM lParam);

	//��������
	BOOL CreateWnd(const RECT & rect, CWnd * pParent,unsigned int uiID,unsigned int uiIndex);

	//����ȡ������
	BOOL PreparStreamInfo(const PREVIEW_CAMINFO &struPreviewInfo, STREAMPARAMS &stStreamParam);

	//��ʼԤ��
	BOOL StartPreview(const PREVIEW_CAMINFO &struPreviewInfo);

	//ֹͣԤ��
	BOOL StopPreview();

	//ȡ���ص�����
	static void CALLBACK PreviewStreamCallback(HANDLE hStream, HANDLE hUserData, STREAM_DATA_INFO * pStreamDataInfo);
    static void CALLBACK DecCallBack (LONG nPort, unsigned char *pBuf, long nSize, FRAME_INFO *pFrameInfo, void* pUser);

	//����������
	BOOL InputData(PBYTE pBuffer,DWORD dwSize,long lFrameType);

	//���ſ⿪ʼ����
	BOOL StartPlay(PLAYERTYPE playerType, PBYTE pBuffer, DWORD dwBufferSize,CString strCamName = _T(""));

	//���ſ�ֹͣ����
	void StopPlay();

	//�豸����ת���ɽ�������
	PLAYERTYPE GetPlayerType(int nDevType);

	//���Ż���dc�ص�����
	static void CALLBACK DrawFunMsgCB(long nPort,HDC hDC,long lUser);

	//����������ʾ��ʾ��Ϣ
	void DrawMyText(CDC * pDC,CString csDrawText,CRect rcDraw);

	//�����¼��ص�����
	BOOL SetEventFunc(PV_EVENT_FUNC Func);

	//��ȡ¼���ļ���Ϣ
	BOOL GetRecordFile(RECOED_FILE_INFO& recordFile);

	BOOL StopRecordByCaminfo(const PREVIEW_CAMINFO &struPreviewInfo);

	//�Ƿ���¼��
	BOOL IsRecord(){return m_bIsRecording;}

	//�Ƿ���Ԥ��
	BOOL IsPreview(){return m_bIsPreviewing;}

	//�ж���Ƶ�Ƿ���
	BOOL IsSound(){return m_bIsSounding;}

	//�ж�OSD�Ƿ���ʾ
	BOOL IsShowOsd(){return m_bIsOsdShowing;}

	//�ж�OSD�Ƿ���ʾ
	BOOL IsShow4Osd(){return m_bIsOsdShow4Row;}

	//���ô���������
	void SetWndowIndex(int iIndex){m_uiWndIndex = iIndex;}

	//��ȡ����Ԥ���ļ�ص�ID
	int GetCamID(){return m_nCamID;};

	//����Ҫ��ʾ��ȡ����Ϣ
	void SetShowMsg(int nMsgID);
    void SetShowMsg(const CString &strShowMsg);

	//�Ҽ������˵�
	void PopupMenu(BOOL bJustClose = FALSE);

	//ִ�в˵�
	void ExcuteSelectOption(int nCmd);

	//��������ʾ����
	void ShowToolBar(BOOL bShow);
	void HideToolBar();

    //��ȡ��ص�����
    CString GetChanName();

	//Ԥ��ȡ����Ϣ�ص�
	static void CALLBACK PreviewMsgNotifyCallback(HANDLE hStream, HANDLE hUserData, int nMsgID);

    /** @fn         CheckOperateRights
     *  @brief      ���ݲ˵������Ƿ��в���Ȩ��
     *  @param[IN]  nMenuID: ���㲥�Ŵ���
     *  @return     TRUE: �У�FALSE: ��
     */
    BOOL CheckOperateRights(int nMenuID);
    
    /** @fn         SetFullScreenStatus
     *  @brief      ����ȫ��״̬
     *  @param[IN]  bFullScreen: �Ƿ�ȫ��
     *  @return     void
     */
    void SetFullScreenStatus(BOOL bFullScreen = FALSE); 

    // ¼�����ݴ���
    BOOL DealDecodeData(long nPort,char * pBuf,long nSize,void * pFrameInfo);

	// ¼���������ص�����
	void StreamDataProc(long datatype, void *pBuffer, DWORD dwSize);

	// ¼��������Ϣ�ص�����
	void PhoneErrorProc(int nErrortype);

	// ¼�������Ϣ�ص�����
	void VideoErrorProc(int nErrortype);

	void SetFirstWndWave(CScope *pVideoWav, CScope *pAudioWav);

	// ��¼��ͨ��
	BOOL OpenPSCLient(const PREVIEW_CAMINFO &struPreviewInfo);

	// �ر�¼��ͨ��
	void ClosePSCLient();

	// ��¼��ͨ��
	BOOL OpenVSClient(const PREVIEW_CAMINFO &struPreviewInfo);

	// �ر�¼��ͨ��
	BOOL CloseVSClient();

	//��ʼ¼��
	BOOL StartRecord();

	//ֹͣ¼��
	BOOL StopRecord();

	//������Ƶ
	BOOL OpenSound();

	//�ر���Ƶ
	BOOL CloseSound();

	// ���������л�
	void StreamTypeSwitch(const int nStreamType);

	//��ʼԤ��ʱ����osd��״̬
	void SetOsdState();

	// ��ʾOSD
	void ShowOSD();

	// ����OSD
	void HideOSD();

	//����OSD��Ϣ
	BOOL SetOSDInfo(const CString &strOsdInfo);

	 /* 
	 *  @fn         CaptureByMyself
     *  @brief      ץͼ
     *  @return     TRUE: �ɹ���FALSE: ʧ��
     */
    BOOL CapturePic();

	//USB��ʼԤ��
	BOOL USBStartPreview(const PREVIEW_CAMINFO& stUSBPreInfo);

	//USBֹͣԤ��
	BOOL USBStopPreview(LONG lCameraIndex);

	//USB��ʼ¼��
	BOOL USBStartRecord();

	//USBֹͣ¼��
	BOOL USBStopRecord();

	//USB����ͷȡ���ص�����
	static void CALLBACK USBPreviewStreamCallback(OUTPUTDATA_INFO *pDataInfo, unsigned long pUser);

    // ��ȡָ��dvr��ǰʱ���
    BOOL GetDvrTimeStamp(long lUserId, time_t &iDvrTimeStamp, DWORD& dwError);
};