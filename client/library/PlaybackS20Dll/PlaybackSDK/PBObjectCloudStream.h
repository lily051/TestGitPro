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
#include "hcs_object_api.h"

class CPBObjectCloudStream : public CPlayStream
{
public:
	CPBObjectCloudStream(void);
	~CPBObjectCloudStream(void);

private:
	 //�ƴ洢��ʼ����� 
	HCSClient* m_hHCSClient; 
	// Bucket
	CString m_strBucket;
	// ObjectKey
	CString m_strObjectKey;
	//��ʼʱ��
	time_t m_iStartTime;
	//����ʱ��
	time_t m_iStopTime;
public:
	volatile BOOL m_bStopDataCb;   //ͣ��
	//ȡ���߳̾��
	HANDLE m_hGetObjectCloudStreamThread;

private:
	// ��ʼȡ��.
	BOOL StartData(HCSClient* pClient, const char* pBucket, const char* pObjectKey, const time_t iStartTime, const time_t iStopTime);
	// ֹͣȡ��.
	BOOL StopData();
	//ȡ���ص�����
	static int ObjectCloudStreamDataCb(const char* buff, int bufflen, long long offset, int error, void* p_user);

	static DWORD WINAPI ThreadFuncGetObjectCloudStream(LPVOID pParam);
public:
	// ��ʼ��������SDK.
	static BOOL InitObjectCloudSdk();

	// �ͷŶ�����SDK.
	static void ReleaseObjectCloudSdk();

	// ��½�豸.
	BOOL Login(PB_LOGININFO& stLoginInfo);

	// �ǳ��豸.
	void Logout();

	// �����ļ�.
	BOOL FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount);

	// ��ȡ��Ƶ��ʽ.
	//BOOL GetVideoFormat( int& nVideoFormat);

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

	//ȡ��
	void GetObjectCloudStreamData();

};