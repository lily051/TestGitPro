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
#include "StreamClient.h"

class CPBKmsStream : public CPlayStream
{
public:
	CPBKmsStream(void);
	~CPBKmsStream(void);

protected:
	// ��¼���.
	LONG m_lLogin; 
	// �طž��.
	LONG m_lPlayHandle;  
	// �ط�����.
	ENUM_PLAY_TYPE m_emPlayType;

	BOOL m_bStreamPause;			 // �Ƿ�����ֹͣ״̬.

protected:
	// ֹͣȡ��.
	virtual BOOL StopData();
	// �ļ�����ת��.
	virtual int ChangeFileType(int nFileType);


public:
	// ��ʼ������SDK.
	static BOOL InitKmsSdk();

	// �ͷź���SDK.
	static void ReleaseHikSdk();

	// ��½�豸.
	BOOL Login(PB_LOGININFO& stLoginInfo);

	// �ǳ��豸.
	void Logout();

	// �����ļ�.
	BOOL FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount);

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

	virtual BOOL PlaybackControl(ENUM_CONTROL_TYPE emControlType, LONG lValueIn, LONG & lValueOut);

};

extern int CALLBACK pKmsStream(int sessionhandle, void* userdata, int datatype, void* pdata, int ilen);
extern int CALLBACK pKmsMsg( int sessionhandle, void* userdata, int opt, void* param1, void* param2, void* param3, void* param4 );