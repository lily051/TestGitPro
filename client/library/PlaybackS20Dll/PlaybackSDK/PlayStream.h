#pragma once
/**	@file    PlayStream.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   �����
*
*	@author	 yudan
*	@date	 2012-06-02 13:51
*
*	@note   
*	@note    ��ʷ��¼��
*	@note    V1.0  create at 2012-06-02 13:51 by yudan
*/
#include "./PlaybackBase.h"
#include "GeneralPlayerAPI.h"


#define PLAYBACK_DETECT_TIME 10
#define DOWNLOAD_DETECT_TIME 10

class CPlayStream : public CPlaybackBase
{
public:
	CPlayStream();
	virtual ~CPlayStream();

protected:
	PLAYERTYPE m_emPlayer;           //����������
	HWND m_hPlayHwnd;                //���Ŵ��ھ��
	BYTE m_byteHeadBuf[1024];        //ͷ�ļ����ݱ���
	BYTE m_byteSaveBuf[MAX_SAVEBUF_SIZE];	// �طż���������,ѭ��д��������.
	BOOL m_bIsReWrite;				 // �������Ƿ����ˣ�����ͷ��ʼд����.
	DWORD m_dwCurBufPointer;				// ������������ǰָ��.
	int m_nHeadLen;                  //ͷ���ݴ�С
	HANDLE m_hSaveFile;              //�ļ����
	BOOL m_bSaveFile;				 // �Ƿ�Ҫ�����ļ�
	HANDLE m_hDownloadFile;          //�����ļ����
	DWORD m_dwTotleSize;             //�ļ��ܴ�С
	DWORD m_dwCurDownSize;           //��ǰ���صĴ�С
	PLAYBACK_TIME_INFO m_stPlaybackTimeInfo;  //��ǰ�طŵ�ʱ�����
	CRITICAL_SECTION m_csSaveBufLock;// �طż���������.
	BOOL m_bOpenSound;				 // ����Ƶ.
	PbStreamDataCB m_pfnStreamCB;	 // ȡ���ص�����
	DWORD m_dwStreamUserData;		 // ȡ���ص��û��Զ�������.
	BOOL m_bStreamCB;				 // �Ƿ�Ҫ��ʼȡ���ص�.
	BOOL m_bGetHead;				 // �Ƿ��ȡͷ����.
	VIDEODRAWFUNC m_pDrawFunc;		 // ��Ƶ��ͼ����
	LONG m_nDrawUserData;			// �û�����

public:
    BOOL m_bPlaying;                 //�Ƿ������
	LONG m_lPort;                    //����˿�
	BOOL m_bDownload;                //�Ƿ�����
	BOOL m_bPause;                   //�Ƿ���ͣ�ط���
	LONG m_lPlaybackLastTime;        //�ط����һ������ʱ��
	LONG m_lDownloadLastTime;        //�������һ������ʱ��
	int m_nPlaybackPos;              //�طŽ���
	int m_nDownloadPos;              //���ؽ���
	BOOL m_bPlaybackThread;          //�طż���߳�
	HANDLE m_hPlaybackThread;        //�طż���߳̾��
	BOOL m_bDownloadThread;          //���ؼ���߳�
	HANDLE m_hDownloadThread;        //���ؼ���߳̾��

	BOOL m_bReversePlay;			// �Ƿ��ǵ���.
	BOOL m_bStopPlay;		
	BOOL m_bBlock;					// �Ƿ��������ݻص�.
	CString m_strSaveFilePath;      //�����ļ�·��

    CRITICAL_SECTION m_csStreamCB;  //ȡ���ص�������
	ENUM_CONTROL_TYPE m_emPlaySpeed; //�ط��ٶ�
	DWORD m_dwBuffSize;             //���ſ⻺������С

protected:
	// �豸���ͺͲ�������ת��.
	PLAYERTYPE GetPlayTypeFromDeviceType(int nType);

public:
	// ����ͷ���ݻ���.
	void SetHeadData(PBYTE pBuffer, DWORD dwBufSize);
	// ��ȡͷ����.
	void GetHeadData(PbStreamDataCB pfnStreamData, DWORD dwUserData);
	// ���û���������.
	void SetBufferData(PBYTE pBuffer, DWORD dwBufSize);
	// ��ȡ����������.
	void GetBufferData(PbStreamDataCB pfnStreamData, DWORD dwUserData);
	// ��ȡȡ���ص�����.
	PbStreamDataCB GetStreamDataCB();
	// ��ȡȡ���ص��û��Զ�������.
	DWORD GetStreamUserData();
	// ���ϲ�����.
	void PushStreamData(int nDataType, BYTE * pBuffer, DWORD dwBufSize);
	// ��ȡ�ط�����.
	PLAYERTYPE GetPlayType();
	// ��ȡ�����ļ����.
	HANDLE GetSaveFileHandle();

public:
	/** @fn pFunPlayback
	*   @brief ȡ���ص�����
	*   @param[in] hPlayData: �طž��
	*   @param[in] nDataType: ������
	*   @param[in] pBuffer:   ������
	*   @param[in] dwBufferSize: ���ݴ�С
	*   @return NULL
	*/
	BOOL pFunPlayback(HANDLE hPlayData, int nDataType, BYTE * pBuffer, DWORD dwBufferSize);

	/** @fn pFunDownload
	*   @brief ȡ���ص�����
	*   @param[in] hPlayData: �طž��
	*   @param[in] nDataType: ������
	*   @param[in] pBuffer:   ������
	*   @param[in] dwBufferSize: ���ݴ�С
	*   @return NULL
	*/
	void pFunDownload(HANDLE hPlayData, int nDataType, BYTE * pBuffer, int dwBufferSize);

	/** @fn PlaybackStreamDetect
	*   @brief �ط�������߳�
	*   @param[in] lpvoid: �̲߳���
	*   @param NULL
	*   @return NULL
	*/
	static void PlaybackStreamDetect(LPVOID lpvoid);

	/** @fn DownloadStreamDetect
	*   @brief ����������߳�
	*   @param[in] lpvoid: �̲߳���
	*   @param NULL
	*   @return NULL
	*/
	static void DownloadStreamDetect(LPVOID lpvoid);

	/** @fn StartStreamThread
	*   @brief �����߳�
	*   @param[in] bPlayback: TRUE�ط��߳� FALSE�����߳�
	*   @param NULL
	*   @return BOOL�ɹ� FALSEʧ��
	*/
	BOOL StartStreamThread(BOOL bPlayback);
	BOOL StartCheckPlayStream(BOOL bPlayback);

	/** @fn StopStreamThread
	*   @brief ֹͣ������߳�
	*   @param[in] bPlayback: TRUE�ط��߳� FALSE�����߳�
	*   @param NULL
	*   @return NULL
	*/
	void StopStreamThread(BOOL bPlayback);
	void StopCheckPlayStream(BOOL bPlayback);

	/** @fn Login
	*   @brief ��¼�豸����ȡ�����������طŻ����ز������ڴ����Ͻ���
	*   @param[in] stLoginInfo: ��¼�豸��Ϣ
	*   @param NULL
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	virtual BOOL Login(PB_LOGININFO& stLoginInfo) = 0;

	/** @fn Logout
	*   @brief �ǳ��豸,���پ��
	*   @param NULL
	*   @param NULL
	*   @return NULL
	*/
	virtual void Logout() = 0;

	/** @fn FindFile
	*   @brief �����ļ�
	*   @param[in] stFindFileParam: �����ļ�����
	*   @param[in] lpFileList: �ļ�����
	*   @param[in] nFileCount: �ļ�����
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	virtual BOOL FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount) = 0;

	/** @fn PlaybackTime
	*   @brief ʱ��ط�
	*   @param[in] stPbTimeInfo: ʱ��طŲ���
	*   @param NULL
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	virtual BOOL PlaybackTime(PLAYBACK_TIME_INFO& stPbTimeInfo) = 0;

	/** @fn PlaybackFile
	*   @brief �ļ��ط�
	*   @param[in] stPbFileInfo: �ļ��طŲ���
	*   @param NULL
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	virtual BOOL PlaybackFile(PLAYBACK_FILE_INFO& stPbFileInfo)= 0;

	/** @fn StopPlayback
	*   @brief ֹͣ�ط�
	*   @param NULL
	*   @param NULL
	*   @return NULL
	*/
	virtual void StopPlayback();

	/** @fn PB_DownloadFile
	*   @brief �ļ�����
	*   @param[in] stDownloadParam: ���ز���
	*   @param NULL
	*   @return ���ؾ����NULL��ʾ����ʧ��
	*/
	virtual BOOL DownloadFile(DOWNLOAD_FILE_INFO& stDownloadParam) = 0;

	/** @fn PB_DownloadTime
	*   @brief ʱ������
	*   @param[in] stDownloadParam: ������Ϣ
	*   @param NULL
	*   @return ���ؾ����NULL��ʾ����ʧ��
	*/
	virtual BOOL DownloadTime(DOWNLOAD_TIME_INFO& stDownloadParam) = 0;

	/** @fn PB_StopDownload
	*   @brief ֹͣ����
	*   @param[in] hDownload: ���ؾ��
	*   @param NULL
	*   @return NULL
	*/
	virtual void StopDownload() = 0;

	/** @fn LocatePlayTime
	*   @brief ��λ����ʱ��
	*   @param[in]  stStartTime: ��λʱ��
	*   @param[in]  stStopTime:ֹͣʱ��.
	*   @param[out] NULL
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	BOOL LocatePlayTime(const PB_TIME& stStartTime, const PB_TIME& stStopTime);

	/** @fn StopPlay
	*   @brief ֹͣ����
	*   @param NULL
	*   @param NULL
	*   @return NULL
	*/
	void StopPlay();

	/** @fn PlaybackControl
	*   @brief �طſ���
	*   @param[in] emControlType: ��������
	*   @param[in] lValueIn: �������
	*   @param[out] lValueOut: �������
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	virtual BOOL PlaybackControl(ENUM_CONTROL_TYPE emControlType, LONG lValueIn, LONG & lValueOut);

	/** @fn GetOsdTime
	*   @brief ��ȡosdʱ��
	*   @param[in]  NULL
	*   @param[out] lOsdTime: osdʱ�䣬32time_t��ֵ
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	virtual BOOL GetOsdTime(LONG & lOsdTime);

	/** @fn GetPlayPos
	*   @brief ��ȡ�طŽ���
	*   @param[in]  NULL
	*   @param[out] lPos: ����
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	virtual BOOL GetPlayPos(LONG & lPos);

	/** @fn PlaybackCapture
	*   @brief �ط�ץͼ
	*   @param[in] szPicFile: ͼƬ·��
	*   @param[in] emPicType: ͼƬ����
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	BOOL PlaybackCapture(char * szPicFile, ENUM_PIC_TYPE emPicType);

	/** @fn PB_SaveData
	*   @brief ¼�����
	*   @param[in] szSaveFile: �ļ��洢��ַ
	*   @param NULL
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	BOOL SaveData(char * szSaveFile);

	/** @fn PB_StopSave
	*   @brief ֹͣ����
	*   @param NULL
	*   @param NULL
	*   @return NULL
	*/
	void StopSave();

	/** @fn PB_GetDownloadPos
	*   @brief ��ȡ���ؽ���
	*   @param[out] nPos: ����
	*   @param NULL
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	virtual	BOOL GetDownloadPos(int & nPos);

public:
	// ���ط���.
	static DWORD WINAPI ThreadFuncCheckPlayStream(LPVOID pParam);

	// ��ȡ���������ֹͣ��־.
	virtual DWORD GetSpecialStopFlag();

	// ��ȡȡ���ط�����.
	virtual BOOL StartGetStreamData(PbStreamDataCB pStreamData, DWORD dwUser);

	// ֹͣ��ȡȡ���ط�����.
	virtual void StopGetStreamData();

	//��ȡ����ͼ��Ĵ�С
	virtual BOOL GetPictureSize( LONG* lWidth, LONG* lHeight);
};

extern int CALLBACK OnSaveData(int nDataType, BYTE* pBuffer, DWORD dwBufSize, DWORD dwUser);