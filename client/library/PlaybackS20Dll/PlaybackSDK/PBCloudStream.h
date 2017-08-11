/**   @file APICloudStream.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief APICloudStream������
 *
 *    @author   wangzhiyong
 *    @date     2013/12/05
 *
 *    @note 
 *    @note 
 *    @note ��ʷ��¼��
 *    @note V1.0.0  
 *
 *    @warning 
 */

#pragma once

#include "PlayStream.h"
#include "hcs_sdk.h"


class CPBCloudStream : public CPlayStream
{
public:
	CPBCloudStream();
	~CPBCloudStream(void);

private:
	// ��¼���.
	unsigned long long m_nLogin; 
	// ���ž��.
	int m_nPlayHandle;
	// �Ƿ��Ѿ���¼�ɹ�.
	BOOL m_bIsLogin;
	// ¼���ļ��б�.
	PB_FILEINFO m_stFileInfo[MAX_FILE_NUM];
	// ¼���ļ�����.
	int m_nFileNum;
	// ������IP.
	CString m_strDecoderIP;
	// �Ƿ���ͷ����.
	BOOL m_bIsHeadData;
	//ȡ���߳�
	HANDLE m_hCloudPlayThread;
    HANDLE m_hCloudDownloadThread;
	//ͷ����
	char m_szHeadBuf[4*1024];
	//�����ļ�����
	BOOL m_bFinishFindFile;

	HANDLE m_hStopPlayEvent;

	HANDLE m_hStopCheckSearch;

	int m_nDownloadHandle;

public:
    HANDLE m_hStopDownloadEvent;
private:
	// ������Ϣ�ص�����.
	static int UserMessage(int LogicType,int ErrorCode,void* pUser);
	// ��ѯ¼����.
	//static int GetResult(CloudRecordInfo info, int Count, int Flat, void* pUser);

	static void FunDownRecordDataCb(int handle, const char* buff ,int bufflen, int Flat, int error_code, void* pUser);

	// �������¼���߳�.
	static DWORD WINAPI ThreadFuncCheckSearch(LPVOID pParam);

	// ��ʼȡ��.
	BOOL StartData(unsigned long long uLogin, const char* pCamID, const PB_TIME &stStartTime, const PB_TIME &stStopTime);
	// ֹͣȡ��.
	BOOL StopData();

public:
	// ��ʼ��SDK.
	static BOOL InitCloudSdk();

	// �ͷ�SDK.
	static void ReleaseSdk();

	// ��½�豸.
	BOOL Login(PB_LOGININFO& stLoginInfo);

	// �ǳ��豸.
	void Logout();

	// �����ļ�.
	BOOL FindFile(FINDFILE_PARAM& stFindFileParam, LPB_FILEINFO& lpFileList, int& nFileCount);

	// ��ʱ��ط�.
	BOOL PlaybackTime(PLAYBACK_TIME_INFO& stPbTimeInfo);

	// ���ļ��ط�.
	BOOL PlaybackFile(PLAYBACK_FILE_INFO& stPbFileInfo);

	// ֹͣ�ط�.
	void StopPlayback();

	// �����ļ�.
	BOOL DownloadFile(DOWNLOAD_FILE_INFO& stDownloadParam);

	// ��ʱ������.
	BOOL DownloadTime(DOWNLOAD_TIME_INFO& stDownloadParam);

	// ֹͣ����.
	void StopDownload();

	//����
	void CloudPlay();

    void ThreadStopDownLoad();

	//��������Ƿ����
	void StartCheckSearchFinish();

	UINT GetPlayHandle() const { return m_nPlayHandle; }

	BOOL GetFinishFindFile() const { return m_bFinishFindFile; }

	void SetFinishFindFile(BOOL val) { m_bFinishFindFile = val; }

	void StopCheckSearchFinish();

	void SetDownloadPos(int nPos) {m_nDownloadPos = nPos;}
};
