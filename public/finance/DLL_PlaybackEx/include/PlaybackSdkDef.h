#pragma once
/**	@file    PlaybackSdkDef.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   PlaybackSDK���ݶ���
*
*	@author	 yudan
*	@date	 2012-06-01 18:59
*
*	@note   
*	@note    ��ʷ��¼��
*	@note    V1.0  create at 2012-06-01 18:59 by yudan
*/
//lint -library
namespace PlaybackSdk
{
/*************DEFINE*********************************************/
#define MAX_RECORD_COUNT 4000           //�ļ���������

/*************ENUM***********************************************/
	// �ط����ͱ�־λ,,��500��ʼ,��������NVR��PROX��BDP�ȷ�DVR�ط�ȡ����DVRȡ������̬����ȡ������ʱ��,���豸���ͺŲ��ܴ���500.
	#define PLAYTYPE_FLAG 500

	typedef enum _emDeviceType
	{
		PB_DEVICE_HIKVISION          = 0    //��������
	}ENUM_DEVICE_TYPE;

	typedef enum _emPbPlayType
	{
		PB_TYPE_DVR = 0,
		PB_TYPE_CVR,
		PB_TYPE_CLOUD,
        PB_TYPE_KMS                       
	}ENUM_PLAY_TYPE;

	//¼������
	typedef enum _emEnumRecordType
	{
		RECORD_TYPE_ALL = 0xFF,            //ȫ��
		RECORD_TYPE_TIMING = 0,            //��ʱ¼�� 
		RECORD_TYPE_MOVE	= 1,           //�ƶ����
		RECORD_TYPE_ALARM_TRIGER = 2,      //��������
		RECORD_TYPE_ALARM_OR_MOVE = 3,     //����|�ƶ����	
		RECORD_TYPE_ALARM_AND_MOVE = 4,    //����&�ƶ����
		RECORD_TYPE_CMD_TRIGER	= 5,       //�����
		RECORD_TYPE_MANUAL_RECORD = 6,     //�ֶ�¼��

		RECORD_TYPE_BACKUP = 7,			   // ����¼��,�������ط�����ʹ��.
		RECORD_TYPE_EVENT = 8,			   // ��������¼��,�������ط�����ʹ��.

		//�������¼������
		RECORD_TYPE_SMART_RECORD = 9,				//�������
		RECORD_TYPE_VCA_TRAVERSE_PLANE = 10,               //Խ�����
		RECORD_TYPE_FIELD_DETECTION = 11,                  //����������� 
		RECORD_TYPE_AUDIO_INPUT_ALARM	= 12,                //��Ƶ�����쳣
		RECORD_TYPE_SOUND_INTENSITY_ALARM = 13,            //��ǿͻ�����
		RECORD_TYPE_FACE_DETECTION = 14,                   //�������
		RECORD_TYPE_VIRTUAL_FOCUS_ALARM = 15,              //�齹���
		RECORD_TYPE_SCENE_CHANGE_ALARM	= 16,            //����������
	}ENUM_RECORD_TYPE;

    //�����������
    typedef enum _emEnumEventType
    {
		EVENT_TYPE_ALL	= 0xffff,						//ȫ��
        EVENT_TYPE_VCA_TRAVERSE_PLANE = 1,               //Խ�����
        EVENT_TYPE_FIELD_DETECTION = 2,                  //����������� 
        EVENT_TYPE_AUDIO_INPUT_ALARM	= 3,                //��Ƶ�����쳣
        EVENT_TYPE_SOUND_INTENSITY_ALARM = 4,            //��ǿͻ�����
        EVENT_TYPE_FACE_DETECTION = 5,                   //�������
        EVENT_TYPE_VIRTUAL_FOCUS_ALARM = 6,              //�齹���
        EVENT_TYPE_SCENE_CHANGE_ALARM	= 7,            //����������
    }ENUM_EVENT_TYPE;

	//ͼƬ����
	typedef enum _emPicType
	{
		PIC_TYPE_JPG = 1,                  //JPG
		PIC_TYPE_BMP = 2,                  //BMP
	}ENUM_PIC_TYPE;

	//�طſ����붨��
	typedef enum _emControlType
	{
		PB_CONTROL_TYPE_NOREMAL = 0,             //�����ٶ�
		PB_CONTROL_TYPE_FAST2	= 1,             //2����(���)	
		PB_CONTROL_TYPE_FAST4	= 2,             //4����(���)	
		PB_CONTROL_TYPE_FAST8	= 3,             //8����(���)	
		PB_CONTROL_TYPE_FAST16	= 4,             //16����(���)
		PB_CONTROL_TYPE_SLOW2   = -1,            //1/2��(����)
		PB_CONTROL_TYPE_SLOW4   = -2,            //1/4��(����)
		PB_CONTROL_TYPE_SLOW8   = -3,            //1/8��(����)
		PB_CONTROL_TYPE_SLOW16  = -4,            //1/16��(����)

		PB_CONTROL_TYPE_PAUSE	= 11,            //��ͣ�ط�
		PB_CONTROL_TYPE_RESTART = 12,            //�ָ��ط�
		PB_CONTROL_TYPE_OPEN_AUDIO	= 13,        //������
		PB_CONTROL_TYPE_CLOSE_AUDIO	= 14,        //�ر�����
		PB_CONTROL_TYPE_SET_VOLUME	= 15,        //�����������������0~100
		PB_CONTROL_TYPE_SINGLE_FRAME = 16,		 // ��֡.
		PB_CONTROL_TYPE_SINGLE_FRAME_RESTORE = 17,// ֹͣ��֡
		PB_CONTROL_TYPE_PASSIVE_DEC_RESETBUFFER = 18,  //��������
	}ENUM_CONTROL_TYPE;
	
/*************FUNCTION*********************************************/
	typedef int (CALLBACK* PbStreamDataCB)(int nDataType, BYTE* pBuffer, DWORD dwBufSize, DWORD dwUser);

	/**************************************************************************
	*   Description:    ��ͼ�ص�����
	*   Param:          long nPort ���Ŷ˿�
	*					HDC hDC �����豸������
	*					LONG nUser �û�����
	**************************************************************************/
	typedef void (CALLBACK *VideoDrawFunction)(LONG nPort, HDC hDC, LONG nUser);

    /**************************************************************************
	*   Description:    �ؼ�֡���ݻ�ȡ�ص�����
	*   Param:          nMsgType ��Ϣ���ͣ�0--�ɹ�������ʧ��
	*					pBuffer ͼƬ����
	*					dwBufSize ���ݴ�С
                        tmTimeStamp ͼƬʱ���
                        dwUser �û�����
	**************************************************************************/
    typedef int (CALLBACK* PbKeyFrameDataCB)(int nMsgType, BYTE* pBuffer, DWORD dwBufSize, time_t tmTimeStamp, DWORD dwUser);

/*************STRUCT*********************************************/

	//ʱ��ṹ��
	typedef struct _stPbTime
	{
		DWORD	dwYear;			           //��
		DWORD	dwMonth;		           //��
		DWORD	dwDay;			           //��
		DWORD	dwHour;			           //ʱ
		DWORD	dwMinute;		           //��
		DWORD	dwSecond;		           //��
 		_stPbTime()
 		{
 			dwYear = 2012;
 			dwMonth = 1;
 			dwDay = 1;
 			dwHour = 0;
 			dwMinute = 0;
 			dwSecond = 0;
 		}
		BOOL operator == (_stPbTime & st)  //�����ȷ���TRUE
		{
			if (dwYear != st.dwYear || dwMonth != st.dwMonth || dwDay != st.dwDay ||
					  dwHour != st.dwHour || dwMinute != st.dwMinute || dwSecond != st.dwSecond)
			{
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
		BOOL operator != (_stPbTime & st)  //�������ȷ���TRUE
		{
			if (dwYear != st.dwYear || dwMonth != st.dwMonth || dwDay != st.dwDay ||
				dwHour != st.dwHour || dwMinute != st.dwMinute || dwSecond != st.dwSecond)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		BOOL operator < (_stPbTime& st)	
		{
			CTime ctNow((int)dwYear, (int)dwMonth, (int)dwDay, (int)dwHour, (int)dwMinute, (int)dwSecond);
			CTime ctCom((int)st.dwYear, (int)st.dwMonth, (int)st.dwDay, (int)st.dwHour, (int)st.dwMinute, (int)st.dwSecond);
			if (ctNow < ctCom)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		BOOL operator > (_stPbTime& st)	
		{
			CTime ctNow((int)dwYear, (int)dwMonth, (int)dwDay, (int)dwHour, (int)dwMinute, (int)dwSecond);
			CTime ctCom((int)st.dwYear, (int)st.dwMonth, (int)st.dwDay, (int)st.dwHour, (int)st.dwMinute, (int)st.dwSecond);
			if (ctNow > ctCom)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		static BOOL PBTIME2Ctime(const _stPbTime &stTime, CTime &tm)
		{
			if (stTime.dwYear <= 0 || stTime.dwMonth <= 0 || stTime.dwDay <= 0)
			{
				return FALSE;
			}
			tm = CTime(stTime.dwYear, stTime.dwMonth, stTime.dwDay, stTime.dwHour, stTime.dwMinute, stTime.dwSecond);
			return TRUE;
		}
		static BOOL Ctime2PBTIME(const CTime &tm, _stPbTime &stTime)
		{
			if (tm.GetYear() < 1970 || tm.GetMonth() <= 0 || tm.GetDay() <= 0 )
			{
				return FALSE;
			}

			stTime.dwYear = tm.GetYear();
			stTime.dwMonth = tm.GetMonth();
			stTime.dwDay = tm.GetDay();
			stTime.dwHour = tm.GetHour();
			stTime.dwMinute = tm.GetMinute();
			stTime.dwSecond = tm.GetSecond();
			return TRUE;
		}

	}PB_TIME,*LPPB_TIME;

	

	
	//dvr/cvr/�ƴ洢��¼��Ϣ
	typedef struct _stPbDvrLoginInfo
	{
		char szIP[16];                     //ip
		int nPort;                         //�˿�
		int nDataPort;                     //���ݶ˿�
		char szUserName[32];               //�û���
		char szPassword[32];               //����
		char szIndex[32];                   //�ƴ洢Ψһ��ʾ
		char szRev[160];                    //Ԥ��
 		_stPbDvrLoginInfo()
 		{
 			memset(szIP, 0, sizeof(szIP));
 			nPort = 8000;
			nDataPort = 3720;
 			memset(szUserName, 0, sizeof(szUserName));
 			memset(szPassword, 0, sizeof(szPassword));
			memset(szIndex, 0, sizeof(szIndex));
 			memset(szRev, 0, sizeof(szRev));
 		}
	}PB_DVRLOGININFO;

	// ��¼������Ϣ
	typedef struct _stLoginTypeInfo
	{
		PB_DVRLOGININFO stDvrLoginInfo;
	}LoginTypeInfo;

	//��¼��Ϣ
	typedef struct _stPbLoginInfo
	{
		ENUM_PLAY_TYPE emPlayType;         //�ط�����
		ENUM_DEVICE_TYPE emDevType;       //�豸����
		LoginTypeInfo unLoginType;
		_stPbLoginInfo()
		{
			emDevType = PB_DEVICE_HIKVISION;
			emPlayType = PB_TYPE_DVR;
		}
	}PB_LOGININFO;

	//�ļ����Ҳ�����Ϣ
	typedef struct _stFindFileParam
	{
		LONG lChannel;                      //ͨ��
		char szStreamID[32];                //��ID,ֻ��CVR��Ч
		PB_TIME stStartTime;                //������ʼʱ��
		PB_TIME stStopTime;                 //���ҽ���ʱ��
		ENUM_RECORD_TYPE emRecordType;      //�����ļ�����
		DWORD dwUseCardNo;					// �Ƿ񰴿��Ų�ѯ.
		BYTE sCardNumber[32];				// ����.
        BOOL bByEvent;                      // �Ƿ��¼���ѯ
        ENUM_EVENT_TYPE emEventType;        //�����¼�����
		_stFindFileParam()
		{
			lChannel = 0;
			szStreamID[0] = '\0';
			emRecordType = RECORD_TYPE_ALL;
			dwUseCardNo = 0;
			memset(sCardNumber, 0, sizeof(sCardNumber));
            bByEvent = FALSE;
            emEventType = EVENT_TYPE_VCA_TRAVERSE_PLANE;
		}
	}FINDFILE_PARAM;

	typedef struct _stConvertPlayParam
	{
		BYTE byResolution;		//�ֱ���
		DWORD dwVideoBitrate;   //��Ƶ����
		DWORD dwVideoFrameRate; //֡��

		_stConvertPlayParam()
		{
			byResolution = 0;
			dwVideoBitrate = 0;
			dwVideoFrameRate = 0;	
		}
	}CONVERTPLAY_PARAM;

	///<������ṹ
	typedef struct tagPB_POINT
	{	
		float fX;								///<X������, 0.001~1
		float fY;								///<Y������, 0.001~1
	}PB_POINT, *LPPB_POINT;

	///<��������ṹ
	typedef struct tagPB_RECT
	{	
		float fLEFT;								///<X������, 0.001~1
		float fTOP;								///<Y������, 0.001~1
		float fWidth;
		float fHeight;
	}PB_RECT, *LPPB_RECT;

	//���ܼ���������Ϣ
	typedef struct _stSmartSearchParam
	{
		LONG lChannel;				//ͨ��
		PB_TIME stStartTime;        //������ʼʱ��
		PB_TIME stStopTime;         //���ҽ���ʱ��
		int	nRecordType;			//�������ͣ�0-�ƶ���⣬1-Խ����⣬2-��������
		DWORD dwCurArea;			//��ǰ����
		DWORD dwSensitivity;		//������.
		DWORD dwCrossDirection;		//��Խ����--------Խ�����
		DWORD dwPreTime;			//������ǰʱ��
		DWORD dwDelayTime;			//������ʱʱ��
		DWORD dwIntrusionRate;		//ռ�ȣ�����������δ����Ŀ��ߴ�Ŀ��ռ��������ı��أ���һ��Ϊ1~100---�������� 
		DWORD dwIntrusionDuration;	//��Ϊ�¼�����ʱ����ֵ: 1~100�룬����5�룬�ж���Ч������ʱ��
										//(�����ATM�豸��ʱ��Ϊ1~1200��)---�������� 
		DWORD dwPointNum;   //����������Ч����
		PB_POINT struPos[10];  //����α߽�㣬���ֵΪ10 
		BYTE byMotionScope[64][96];  //�ƶ��������

		_stSmartSearchParam()
		{
			nRecordType = 0;
			lChannel = 0;
			dwCurArea = 0;
			dwSensitivity = 1;
			dwCrossDirection = 0;
			dwPreTime = 0;
			dwDelayTime = 0;
			dwIntrusionRate = 0;
			dwIntrusionDuration = 0;
			dwPointNum = 0;
		}
	}SMARTSEARCH_PARAM;

	//ʱ��ط���Ϣ
	typedef struct _stPlaybackTimeInfo
	{
		LONG lChannel;                      //ͨ��
		char szStreamID[32];                //��ID,ֻ��CVR��Ч
		PB_TIME stStartTime;                //��ʼ�ط�ʱ��
		PB_TIME stStopTime;                 //�����ط�ʱ��
		HWND hWnd;                          //���Ŵ��ھ��
		ENUM_RECORD_TYPE emRecordType;      //�����ļ�����
        BOOL bByEvent;                      // �Ƿ��¼���ѯ
        ENUM_EVENT_TYPE emEventType;        //�����¼�����
		//ת��ط�-------------------------------
		BOOL bConvertPlay;      //�Ƿ�ת��ط�
		CONVERTPLAY_PARAM stConvertParam;  //ת��طŲ��� 
		VideoDrawFunction pDrawFunc;		//�طŻ�ͼ����
		LONG nUser;                         //�û�����
        char szPlayUrl[255];                //�ط�url
		LONG lBwControl;                     //�������
		_stPlaybackTimeInfo()
		{
			lChannel = 0;
			szStreamID[0] = '\0';
			hWnd = NULL;
			emRecordType = RECORD_TYPE_ALL;
			bConvertPlay = FALSE;
			pDrawFunc = NULL;
			nUser = 0;
            bByEvent = FALSE;
            emEventType = EVENT_TYPE_VCA_TRAVERSE_PLANE;
            szPlayUrl[0] = '\0';
			lBwControl = 0;
		}
	}PLAYBACK_TIME_INFO;

	//�ļ���Ϣ
	typedef struct _stFileInfo
	{
		char szFileName[128];               //�ļ���
		DWORD dwFileSize;                   //�ļ���С
		int nFileType;                      //�ļ�����
		PB_TIME stStartTime;                //��ʼʱ��
		PB_TIME stEndTime;                  //����ʱ��
		ENUM_PLAY_TYPE emPlayType;          //�ļ��洢Դ
		ENUM_DEVICE_TYPE emDevType;         //�豸����
        char szPlayUrl[255];                //�ط�url
		char szStreamID[32];                //��ID
		char szRev[32];                     //Ԥ��
 		_stFileInfo()
 		{
 			szFileName[0] = '\0';
 			dwFileSize = 0;
 			nFileType = 0;
            szPlayUrl[0] = '\0';
			szStreamID[0] = '\0';
 			szRev[0] = '\0';
 		}
	}PB_FILEINFO, *LPB_FILEINFO;

	//�ļ��طŲ���
	typedef struct _stPlaybackFileInfo
	{
		LONG lChannel;                      //ͨ����
		char szStreamID[32];                //��ID,ֻ��CVR��Ч
		PB_FILEINFO stFileInfo;             //�ļ���Ϣ
		HWND hWnd;							//���Ŵ���
		VideoDrawFunction pDrawFunc;		//�طŻ�ͼ����
		LONG nUser;                         //�û�����
		LONG lBwControl;                     //�������

		//ת��ط�-------------------------------
		BOOL bConvertPlay;      //�Ƿ�ת��ط�
		CONVERTPLAY_PARAM stConvertParam;  //ת��طŲ��� 
		_stPlaybackFileInfo()
		{
			lChannel = 0;
			szStreamID[0] = '\0';
			hWnd = NULL;
			bConvertPlay = FALSE;
			pDrawFunc = NULL;
			nUser = 0;
			lBwControl = 0;
		}
	}PLAYBACK_FILE_INFO;

	//�ļ����ز���
	typedef struct _stDownloadFileInfo
	{
		LONG lChannel;                      //ͨ����
		char szStreamID[32];                //��ID,ֻ��CVR��Ч
		PB_FILEINFO stFileInfo;             //�ļ���Ϣ
		char szDownloadFile[255];           //���ص�ַ
 		_stDownloadFileInfo()
 		{
 			lChannel = 0;
			szStreamID[0] = '\0'; 
 			memset(szDownloadFile, 0, sizeof(szDownloadFile));
 		}
	}DOWNLOAD_FILE_INFO;

	//ʱ�����ز���
	typedef struct _stDownloadTimeInfo
	{
		LONG lChannel;                      //ͨ����
		char szStreamID[32];                //�ļ���
		char szFileName[128];               //�ļ���
		char szFilePath[512];               //�ļ�·��
		DWORD dwFileSize;                   //�ļ���С
		PB_TIME stStartTime;                //��ʼʱ��
		PB_TIME stStopTime;                 //����ʱ��
		char szDownloadFile[255];           //���ص�ַ
		char szPlayUrl[255];                //�ط�url
		int nBwControl;                     //�������
 		_stDownloadTimeInfo()
 		{
 			lChannel = 0;
			szStreamID[0] = '\0';
			dwFileSize = 0;
 			memset(szDownloadFile, 0, sizeof(szDownloadFile));
			memset(szFileName, 0, sizeof(szFileName));
			memset(szFilePath, 0, sizeof(szFilePath));
			szPlayUrl[0] = '\0';
			nBwControl = 0;
 		}
	}DOWNLOAD_TIME_INFO;

	//ץͼ����
	typedef struct _stCaptureParam 
	{
		char sSavePic[255];                 //����ͼƬ��·��������
		ENUM_PIC_TYPE emPicType;            //ͼƬ����
		_stCaptureParam()
		{
			memset(sSavePic, 0, sizeof(sSavePic));
			emPicType = PIC_TYPE_JPG;
		}
	}CAPTURE_PARAM, *LPCAPTURE_PARAM;

	//�طű������ݲ���
	typedef struct _stSaveDataParam
	{
		char sSaveName[128];                //�������ݵ�·��������
		_stSaveDataParam()
		{
			memset(sSaveName, 0, sizeof(sSaveName));
		}
	}SAVE_DATA_PARAM, *LPSAVE_DATA_PARAM;

	//�豸�������ṹ����----------------------------------------------------
	#define XML_ABILITY_OUT_LEN	3*1024*1024

	//>>>>>>>>>>>>>>>>>>>>>>>>>>�ƴ洢�ط����>>>>>>>>>>>>>>>>>>>>>>>>>>
	// ����ļ�����.
	#define MAX_FILE_NUM 3000
	static size_t const SEG_HEAD_LEN = 1024;
	typedef struct _seg_head
	{
		INT32 cbSize;                                    // �˽ṹ��С
		INT32 meidaType;                                 // ¼�� or ¼��
		INT32 mediaFormat;                               // hikmp4 or dahuamp4 or wav or mp3
		INT32 segno;                                     // ��ԭʼ���ݵĵ�n��Ƭ��
		INT32 begintime;                                 // ԭʼ���ݵĿ�ʼʱ�䣬������ݿ�����ԭʼ���ݵ�һ��Ƭ��
		INT32 endtime;                                   // ԭʼ���ݵĽ���ʱ��
		UINT32 headSize;                                 // ý��ͷ��С
		BYTE headData[SEG_HEAD_LEN - 7 * sizeof(INT32)]; // ý��ͷ����
	}seg_head_t;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<�ƴ洢�ط����<<<<<<<<<<<<<<<<<<<<<<<<<<
}
