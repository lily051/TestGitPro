#pragma once
/**	@file    PlaybackBase.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   PlaybackSDK����
*
*	@author	 yudan
*	@date	 2012-06-01 16:34
*
*	@note   
*	@note    ��ʷ��¼��
*	@note    V1.0  create at 2012-06-01 16:34 by yudan
*/
#include "PlaybackSdkDef.h"
using namespace PlaybackSdk;

#define OSDTIME_GET_YEAR(time)  (((time)>>26) + 2000)
#define OSDTIME_GET_MONTH(time) (((time)>>22) & 15)
#define OSDTIME_GET_DAY(time)   (((time)>>17) & 31)
#define OSDTIME_GET_HOUR(time)  (((time)>>12) & 31)
#define OSDTIME_GET_MIN(time)   (((time)>>6) & 63)
#define OSDTIME_GET_SEC(time)   (((time)>>0) & 63)

enum _STREAM_TYPE_DATA
{
    PB_STREAM_HEADER = 0,              //ͷ����
    PB_STREAM_DATA,                    //��Ƶ����
    PB_STREAM_END,                     //��������
};

// �طż�����������С,����Ϊ�Ȳ��Ż�������΢��.
#define MAX_SAVEBUF_SIZE 1024 * 1124
// �ౣ��Ļطż������ݴ�С.
#define MAX_PRE_SAVEBUF_SIZE 1024 * 100

class CPlaybackBase
{
public:
	CPlaybackBase();
	virtual ~CPlaybackBase();

protected:
	LONG m_lPlaybackID;         //�طž��
	LONG m_lDownloadID;         //���ؾ��
public:
	LONG m_lChannel;            //ͨ��

protected:
	// �ļ�����ת��.
	virtual int ChangeFileType(int nFileType);
	void PBTimeToCTime(const PB_TIME& stPBTime, CTime& odtTime);

public:
	/** @fn Login
	*   @brief ��¼�豸����ȡ�����������طŻ����ز������ڴ����Ͻ���
	*   @param[in] stLoginInfo: ��¼�豸��Ϣ
	*   @param NULL
	*   @return ��������, -1��ʶʧ��
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

	/** @fn SmartSearch
	*   @brief ���ܼ���
	*   @param[in] stFindFileParam: �����ļ�����
	*   @param[in] lpFileList: �ļ�����
	*   @param[in] nFileCount: �ļ�����
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	virtual BOOL SmartSearch(SMARTSEARCH_PARAM& stSmartSearchParam, LPB_FILEINFO& lpFileList, int& nFileCount);

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
	virtual BOOL PlaybackFile(PLAYBACK_FILE_INFO& stPbFileInfo) = 0;

	/** @fn LocatePlayTime
	*   @brief ��λ����ʱ��
	*   @param[in]  stStartTime: ��λʱ��
	*   @param[in]  stStopTime:ֹͣʱ��.
	*   @param[out] NULL
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	virtual BOOL LocatePlayTime(const PB_TIME& stStartTime, const PB_TIME& stStopTime) = 0;

	/** @fn StopPlayback
	*   @brief ֹͣ�ط�
	*   @param NULL
	*   @param NULL
	*   @return NULL
	*/
	virtual void StopPlayback() = 0;

	/** @fn PlaybackControl
	*   @brief �طſ���
	*   @param[in] emControlType: ��������
	*   @param[in] lValueIn: �������
	*   @param[out] lValueOut: �������
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	virtual BOOL PlaybackControl(ENUM_CONTROL_TYPE emControlType, LONG lValueIn, LONG & lValueOut) = 0;

	/** @fn GetOsdTime
	*   @brief ��ȡosdʱ��
	*   @param[in]  NULL
	*   @param[out] lOsdTime: osdʱ�䣬32time_t��ֵ
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	virtual BOOL GetOsdTime(LONG & lOsdTime) = 0;

	/** @fn GetPlayPos
	*   @brief ��ȡ�طŽ���
	*   @param[in]  NULL
	*   @param[out] lPos: ����
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	virtual BOOL GetPlayPos(LONG & lPos) = 0;

	/** @fn PlaybackCapture
	*   @brief �ط�ץͼ
	*   @param[in] szPicFile: ͼƬ·��
	*   @param[in] emPicType: ͼƬ����
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	virtual BOOL PlaybackCapture(char * szPicFile, ENUM_PIC_TYPE emPicType) = 0;

	/** @fn PB_SaveData
	*   @brief ¼�����
	*   @param[in] szSaveFile: �ļ��洢��ַ
	*   @param NULL
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	virtual BOOL SaveData(char * szSaveFile) = 0;

	/** @fn PB_StopSave
	*   @brief ֹͣ����
	*   @param NULL
	*   @param NULL
	*   @return NULL
	*/
	virtual void StopSave() = 0;

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

	/** @fn PB_GetDownloadPos
	*   @brief ��ȡ���ؽ���
	*   @param[out] nPos: ����
	*   @param NULL
	*   @return TRUE�ɹ� FALSEʧ��
	*/
	virtual BOOL GetDownloadPos(int & nPos) = 0;

	/**  @fn          IsCardNumAllowed
	*    @brief   	  �Ƿ�֧�ְ����Ų�ѯ.
	*    @param[in]   
	*    @param[in]   
	*    @return      TRUE-֧��,FALSE-��֧��.
	*/
	virtual BOOL IsCardNumAllowed();

    /**  @fn          IsEventAllowed
	*    @brief   	  �Ƿ�֧�ְ��¼���ѯ.
	*    @param[in]   
	*    @param[in]   
	*    @return      TRUE-֧��,FALSE-��֧��.
	*/
    virtual BOOL IsEventAllowed();

	/**  @fn          GetStreamData
	*    @brief   	  ��ȡ�ط�������.
	*    @param[in]   
	*    @param[in]   
	*    @return      TRUE-֧��,FALSE-��֧��.
	*/
	virtual BOOL StartGetStreamData(PbStreamDataCB pStreamData, DWORD dwUser);

	/**   @fn          StopGetStreamData
	*    @brief   	   ֹͣ��ȡ�ط�������.
	*    @param[in]    hPlayback:�طž��.
	*    @return     
	*/
	virtual void StopGetStreamData();

	/**   @fn          GetCompressCfgAbility
	*    @brief   	   ��ȡѹ������������.
	*    @param[out]   stCompresscfgAbility:�������ṹ��.
	*    @return     
	*/
	virtual BOOL GetCompressCfgAbility( char* pOutbuf );

	/**   @fn          GetVideoFormat
	*    @brief   	   ��ȡ��Ƶ��ʽ.
	*    @param[out]   nVideoFormat:��Ƶ��ʽ.
	*    @return     
	*/
	virtual BOOL GetVideoFormat(int& nVideoFormat);

	/**   @fn          EnableZoomOut
	*    @brief   	   �����طŽ���Ŵ�.
	*    @param[in]   rcShow: ��ʾ����
	*    @param[in]   nRegionNum: ��ʾ������ţ�Ϊ0ʱ����hWnd��bEnable����
	*    @param[in]   hWnd: Ŀ�괰��
	*    @param[in]   bEnable: �򿪹ر���ʾ����.
	*    @return     
	*/
	virtual BOOL EnableZoomOut(RECT rcShow, DWORD nRegionNum = 0, HWND hWnd = NULL, BOOL bEnable = FALSE);

	/**   @fn          GetPictureSize
	*    @brief   	   ��ȡ����ͼ���С.
	*    @param[out]   lWidth: ��
	*    @param[out]   lHeight: ��.
	*    @return     
	*/
	virtual BOOL GetPictureSize( LONG* lWidth, LONG* lHeight);

    /**   @fn          GetKeyFrameData
	*    @brief   	   ��ȡ�ؼ�֡����.
    *    @param[in]    stFrameTime: �ؼ�֡ʱ��
	*    @param[in]    pFunGetKeyFrame: �ؼ�֡���ݻص�����
	*    @param[in]    dwUser: �û�����.
	*    @return     
	*/
	virtual BOOL GetKeyFrameData(const PB_TIME& stFrameTime, PbKeyFrameDataCB pFunGetKeyFrame, DWORD dwUser );

    /**   @fn          StopGetKeyFrameData
	*    @brief   	   ֹͣ��ȡ�ؼ�֡����.
	*    @return     
	*/
	virtual BOOL StopGetKeyFrameData();
};
