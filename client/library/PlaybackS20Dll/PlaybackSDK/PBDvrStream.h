/**   @file HikStream.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief �����豸������
 *
 *    @author   zhoupeiqiang
 *    @date     2012/11/13
 *
 *    @note 
 *    @note 
 *    @note ��ʷ��¼��
 *    @note V1.0.0  
 *
 *    @warning 
 */

#pragma once

#include "./PlayStream.h"
#include "HCNetSDK.h"

class CPBDvrStream : public CPlayStream
{
public:
	CPBDvrStream(void);
	~CPBDvrStream(void);

protected:
	// ��¼���.
	LONG m_lLogin; 
	// �طž��.
	LONG m_lPlayHandle;  
	// �ط�����.
	ENUM_PLAY_TYPE m_emPlayType;

protected:
	// ��ʼȡ��.  MOD 20140402 ��չ�Ƿ�ת��ط�
	virtual BOOL StartData(LONG lLogin, NET_DVR_TIME& stNetStartTime, NET_DVR_TIME& stNetStopTime, BYTE byResolution = 0, DWORD dwVideoBitrate = 0, DWORD dwVideoFrameRate = 0);
	// ֹͣȡ��.
	virtual BOOL StopData();
	// �ļ�����ת��.
	virtual int ChangeFileType(int nFileType);


public:
	// ��ʼ������SDK.
	static BOOL InitDvrSdk();

	// �ͷź���SDK.
	static void ReleaseHikSdk();

	// ��½�豸.
	BOOL Login(PB_LOGININFO& stLoginInfo);

	// �ǳ��豸.
	void Logout();

	// �����ļ�.
	BOOL FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount);

    // ���¼������ļ�.
    BOOL FindFileByEvent(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount);

	// ���ܼ���.
	BOOL SmartSearch(SMARTSEARCH_PARAM& stSmartSearchParam, LPB_FILEINFO& lpFileList, int& nFileCount);

	// ��ȡ��Ƶ��ʽ.
	BOOL GetVideoFormat( int& nVideoFormat);

	// ��ʱ��ط�.
	virtual BOOL PlaybackTime(PLAYBACK_TIME_INFO& stPbTimeInfo);

	// ���ļ��ط�.
	virtual BOOL PlaybackFile(PLAYBACK_FILE_INFO& stPbFileInfo);

	// ֹͣ�ط�.
	virtual void StopPlayback();

	// �����ļ�.
	virtual BOOL DownloadFile(DOWNLOAD_FILE_INFO& stDownloadParam);

	// ��ʱ������.
	virtual BOOL DownloadTime(DOWNLOAD_TIME_INFO& stDownloadParam);

	// ֹͣ����.
	void StopDownload();

	// �Ƿ�֧�ְ����Ų�ѯ.
	virtual BOOL IsCardNumAllowed();

    // �Ƿ�֧�ְ��¼���ѯ.
    virtual BOOL IsEventAllowed();
   
	//��ȡѹ������������.
	BOOL GetCompressCfgAbility( char* pOutbuf );
};

extern void CALLBACK pDvrStream(LONG lPlayHandle, DWORD dwDataType, BYTE * pBuffer, DWORD dwBufSize, DWORD dwUser);