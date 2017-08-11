/**************************************************************************
*   Copyright 2007-2010 Hikvision Digital Technology Co., Ltd.
*   FileName:       PlayerHie.h
*   Description:    ���ſ�API
*   Author:         created by Ѧ�ǻ� and  continued by ������  and continued by ����
*   Date:           24/3/2010
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         24/3/2010       ������         created
**************************************************************************/
#ifndef GUARD_GENERALPLAYERAPI_H
#define GUARD_GENERALPLAYERAPI_H

#ifdef GENERALPLAYERAPI_EXPORTS
#define GENERALPLAYER_API  __declspec(dllexport)
#else
#define GENERALPLAYER_API __declspec(dllimport)
#endif

//���岥������
typedef enum _PlayerType{
	PT_HIK = 0,          //�����豸
	PT_DAHUA,			//���豸
	PT_BSR,				//��ɫ�Ǽ��豸
	PT_HIE,				//�����豸���Ƿ��豸
	PT_TM,				//ͼ���豸
	PT_DALI,			//�����豸
	PT_QT,				//�����豸
	PT_DL400,			//����400�豸
	PT_SS,				//�����豸
	PT_INF,				//Ӣ�����豸
	PT_BSRLimit,		//��ɫ�Ǽ�limit
	PT_HENGTONG,		//��ͨ�豸
	PT_SHENGXUN,		//��Ѹ�豸
	PT_ZHINUO,			//��ŵ�豸
	PT_LULINWANG,		//¼�����豸
	PT_TIANDIIPC,		//���ΰҵ�豸
	PT_WAV,				//��׼wav��ʽ����չ��Ƶ
	PT_TIANYUE,			//��Ծ�豸
	PT_HUANGHE,			//�ƺ�IPC
	PT_GB28181,			//���ұ�׼GB28181����
	PT_WSQ,				//ΰ�����豸
	PT_HUAANTAI,		//����̩�豸
	PT_VAG,				//VAG�����豸����
	PT_HB,				//�����豸
}PLAYERTYPE;

//��������������
typedef enum _StreamMode {
	SM_REALTIME = 0,
	SM_FILE
}STREAMMODE;

//ͼ���ʽ
typedef enum _resolution{
	R_CIF = 0,
	R_2CIF,
    R_4CIF,
	R_D1,
    R_HD1
}RESOLUTION;

//��Ƶ��ʽ
typedef enum _videoFormat{
	VF_PAL = 0,
	VF_NTSC
}GP_VIDEOFORMAT;

//����ͼƬ����
typedef enum _PictureFormat{
	PF_BMP = 0,
	PF_JPEG,
}PICTUREFORMAT;

//�����ٶ�
typedef enum _PlaySpeed{
	PS_NORMAL = 0, //�����ٶ�
	PS_Fast2 = 1,	// 2����(���)					
	PS_Fast4 = 2,	// 4����(���)					
	PS_Fast8 = 3,	// 8����(���)					
	PS_Fast16 = 4,	// 16����(���)			
	PS_SLOW2 = -1,	// 1/2��(����)					
	PS_SLOW4 = -2,	// 1/4��(����)					
	PS_SLOW8 = -3,	// 1/8��(����)					
	PS_SLOW16 = -4,	// 1/16��(����)
}PLAYSPEED;

//��������ص�ͼ����������Ϣ
typedef struct{
	long nWidth;
	long nHeight;
	long nStamp;
	long nType;
	long nFrameRate;
	DWORD dwFrameNum;
}HIK_FRAME_INFO;

//VAG����ص�ͼ��֡��Ϣ
typedef struct tag_VAG_FrameInfo											// ͼ��֡��Ϣ
{
	int iWidth;															// ͼ���
	int iHeight;														// ͼ���
	int iTimeStamp;														// ʱ���
	int iFrameType;														// ͼ��yuv����

	tag_VAG_FrameInfo()
	{
		iWidth = 0;
		iHeight = 0;
		iTimeStamp = 0;
		iFrameType = 0;
	}
}VAG_VPLAY_FRAME_INFO, *LVAG_FRAME_INFO;

enum VAG_SNAP_MODE
{
	VAG_SNAP_ONCELICK = 0,											// ����ץ��
	VAG_SNAP_CONTINUOUS_BYTIME =1,									// ��ʱ������ץ��
	VAG_SNAP_CONTINUOUS_BYFRAME = 2,									// ��֡����ץ��
	VAG_SNAP_ONE_BLOCK = 3											// ����ģʽץͼ,����
};

// VAGץͼģʽ
typedef struct tag_VAG_SnapModeStruct										
{
	VAG_SNAP_MODE enumSnapMode;										// ץͼģʽ
	int iSnapTimes;														// ����ץ�Ĵ���,��ץͼģʽΪVAG_SNAP_CONTINUOUS_BYTIME
	// ��VAG_SNAP_CONTINUOUS_BYFRAMEʱ,��Ҫ���ø�ֵ
	int iSpan;															// ץ�ļ��,��λ:����

	tag_VAG_SnapModeStruct()
	{
		enumSnapMode = VAG_SNAP_ONCELICK;
		iSnapTimes = 0;
		iSpan = 0;
	}
}VAG_SNAP_MODE_PARA, *LVAG_SNAP_MODE_PARA;

// VAGͼƬ��ʽ
enum VAG_SNAP_PICTYPE													
{
	VAG_PICTYPE_JPG = 0,
	VAG_PICTYPE_BMP = 1
};

//VAGͼƬ��Ϣ
typedef struct tag_VAG_SnapPicStruct										
{
	VAG_SNAP_PICTYPE enumPicType;										// ͼƬ��ʽ
	int iPicQuality;													// 0-100,0���,100 ���,Ĭ��80 

	tag_VAG_SnapPicStruct()
	{
		enumPicType = VAG_PICTYPE_JPG;
		iPicQuality = 80;
	}
}VAG_SNAP_PIC_PARA,* LVAG_SNAP_PIC_PARA;

//VAGץͼ����
typedef struct tag_VAG_SnapStruct
{
	unsigned int nFlag;													// ������� //HIWORD(dwFlag) id LOWORD(dwFlag) type
	int	iSnapId;
	int iSnapType;
	VAG_SNAP_PIC_PARA stPicType;										// ͼƬ��ʽ
	VAG_SNAP_MODE_PARA stSnapMode;										// ץͼģʽ
	char szSavePath[260];												// ����Ŀ¼,Ĭ�ϴ��ϵͳ��ʱĿ¼::GetTempPath
	char szKeyName[260];												// ͼƬ����Ӧ�������Ĺؼ���

	tag_VAG_SnapStruct()
	{
		nFlag = 0;
		szSavePath[0] = '\0';
		szKeyName[0] = '\0';
	}
}VAG_SNAP_PARA, *LVAG_SNAP_PARA;

typedef struct tag_VAG_MediaHead
{
	unsigned char nMediaMagic[4];										// HVAG
	unsigned short nMediaVersion;										// �汾��:ָ����Ϣ�ṹ�汾��,ĿǰΪ0x0101,��1.01�汾,01:���汾��;01:�Ӱ汾��.

	unsigned short nRev1;
	unsigned short nSystemFormat;										// ϵͳ��װ��
	unsigned short nVideoFormat;										// ��Ƶ��������
	unsigned char chRev2[12];
	unsigned char nDecodeId[10];										// ������id
	unsigned char nPrivateSystemFormat;									// �Ƿ���˽��ͷ�Լ�˽��ͷ����,���ֶ���ת��װ�⿪����Ա����
	unsigned char cnRev3[3];
	unsigned short nStreamHeadLen;										// ��������ͷ
}VAG_MEDIA_HEAD, *LPVAG_MEDIA_HEAD;

//ý��ͷ�������
typedef struct tag_VAG_MediaHeadAnalInfo									// ý��ͷ�������
{
	VAG_MEDIA_HEAD stSysMediaHead;									// ý��ϵͳͷ��Ϣ(һ��Ϊ40�ֽ�)
	int iSysMediaHeadLen;												// ý��ϵͳͷ��Ϣ����
	char* pPriMediaHead;												// ý��˽��ͷ��Ϣ(�������Զ����ý��ͷ)
	int iPriMediaHeadLen;												// ý��˽��ͷ��Ϣ����
}VAG_MEDIA_HEAD_ANAL_INFO, *LPVAG_MEDIA_HEAD_ANAL_INFO;

//VAG��Ƶ��������
enum VAG_STREAM_DATA_TYPE
{
	GPAPI_VAG_SYSHEAD = 1,													// ϵͳͷ����
	GPAPI_VAG_STREAMDATA,													// ��Ƶ������(����������������Ƶ�ֿ�����Ƶ������)
	GPAPI_VAG_AUDIOSTREAMDATA,												// ��Ƶ������
	GPAPI_VAG_STD_VIDEODATA,												// ��׼��Ƶ������
	GPAPI_VAG_STD_AUDIODATA													// ��׼��Ƶ������
};

//����֡����˵��
#define DECODE_NORMAIL 0		//��������
#define DECODE_KEY_FRAME 1		//ֻ��ؼ�֡
#define DECODE_NONE 2			//������Ƶ֡

//Ĭ�ϻ�������С
const DWORD DEFAULT_PLAY_BUFFER = 1024 * 1024; // Ĭ�ϲ��Ż����СΪһ��

//Ĭ�ϴ��֡��
const DWORD DEFAULT_DISPLAY_BUFFER = 15; // Ĭ�ϴ��15֡�Ļ��棬25֡/s �������Ϊ0.6�������

//�����豸��ʼ����Ϣ�ص�����
typedef void (CALLBACK *GPAPI_NotifyCallBack)(UINT Msg, UINT wParam, UINT lPara, void* cbpara);

/**************************************************************************
*   Function:		VIDEODRAWFUNC
*   Description:    ��ͼ�ص�����
*   Param:          long nPort ���Ŷ˿�
*					HDC hDC �����豸������
*					LONG nUser �û�����
**************************************************************************/
typedef void (CALLBACK *VIDEODRAWFUNC)(LONG nPort, HDC hDC, LONG nUser);

/**************************************************************************
*  Function:  GPAPI_HIKSetDecCallBack
*  brief:     ������GB28181�豸����ص�����
*  parameter: long nPort ���Ŷ˿�
*  parameter: GPAPI_HIKSetDecCallBack �û��������ص�
*  parameter: char * pDest Ŀ������
*  parameter: long nDestSize Ŀ�����ݴ�С
*  parameter: long nUser   �û��Զ�������
**************************************************************************/
typedef void (CALLBACK *GPAPI_HIKSetDecCallBack)(long nPort, char* pBuf, long nSize, HIK_FRAME_INFO* pFrameInfo,
												 long nUser, long nReserved2);

/**************************************************************************
 *  Function:  VAG_fDecodeDataCallback
 *	@brief	<VAG�豸�������ݻص�����>
 *	@param iPlayHandle ������
 *	@param pDataBuf ����ָ��
 *	@param iDataSize ���ݳ���
 *	@param pstFrameInfo ͼ��֡��Ϣ
 *	@param pUserData �û��Զ�������
 */
typedef void (CALLBACK* VAG_fDecodeDataCallback)(int iPlayHandle, const char* pDataBuf, int iDataSize, LVAG_FRAME_INFO pstFrameInfo, void* pUserData);

/**	@fn	int VAG_fSnapCallback
 *	@brief	<VAG�豸ץͼ�ص�����>
 *	@param iPlayHandle ������
 *	@param pSavePath ͼƬ·��
 *	@param iErrorcode ������
 *	@param stSnapPara ץͼ����
 *	@param pUserData �û��Զ�������
 */
typedef void (CALLBACK* VAG_fSnapCallback)(int iPlayHandle, const char* pSavePath, int iErrorcode, LVAG_SNAP_PARA pstSnapPara, void* pUserData);

/**	@fn	int VAG_fStreamDataCallBack
 *	@brief	<VAG�豸ԭʼý�����ݻص�>
 *	@param iPlayHandle ������
  *	@param iDataType �������� 1��ͷ���ݣ�2����������
 *	@param iDataType ����
 *	@param pData ���ݳ���
 *	@param pUser �û��Զ�������
 */
typedef void(CALLBACK* VAG_fGPAPIStreamDataCallBack)(int iPlayHandle, int iDataType, void* pData, int iDataSize, void* pUser);

/**************************************************************************
*   Function:		GPAPI_Startup
*   Description:    ��ʼ���豸���͵Ĳ��ſ⣬�ʼ����
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*                   HWND owner, ���������¼�����豸������ʹ�ã�
								�����豸��owner ���ھ�����������ղ��ſ��ڽ��벥�ŵĹ����в�����һЩ��Ϣ��������Ҫ����ΪNULL
								�����豸��owner ��������ʱ��Ч������ΪNULL
							    ¼�����豸:���ſ⶯̬���أ�ownerΪ1��ʾ��¼�����ڲ�����ʼ���������
							    �����豸��ֵ��ΪNULL
*                   GPAPI_NotifyCallBack Notify,�����豸��ʼ��ʱ���õ���Ϣ�ص��������豸����ΪNULL
*                   void* cbpara,�����豸��ʼ��ʱ���õ��Զ������ݣ������豸����ΪNULL
*   Output:         (null)
*   Return:			BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_Startup(PLAYERTYPE playerType, HWND owner, GPAPI_NotifyCallBack Notify, void* cbpara);

/**************************************************************************
*   Function:		GPAPI_Cleanup
*   Description:    ����ʼ���豸���Ͳ��ſ⣬����ʱ����
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*   Output:         (null)
*   Return:			BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_Cleanup(PLAYERTYPE playerType);

/**************************************************************************
*   Function:		GPAPI_GetPort
*   Description:    ��ȡδʹ�õĲ��Ŷ˿ں�
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*	Input:			HWND hwnd,��ʾ������
							  ��ֻ֧���豸��ͼ������ͨ����Ѷ
							  �����豸��ΪNULL
*   Output:         LONG& nPort,���ã�����δʹ�õĶ˿ں�
*   Return:			BOOL, TRUE:��ȡ�ɹ�,FALSE:��ȡʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_GetPort(PLAYERTYPE playerType, LONG& nPort,HWND hwnd = NULL);

/**************************************************************************
*   Function:		GPAPI_FreePort
*   Description:    �ͷ���ʹ�õĲ��Ŷ˿ں�
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*                   LONG nPort,nPort ���Ŷ˿ں� 
*   Output:         (null)
*   Return:			BOOL, TRUE:��ȡ�ɹ�,FALSE:��ȡʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_FreePort(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_StartPlay
*   Description:    ��ʼ���ţ��˽ӿ������˳�����SetStreamOpenMode
*                   OpenStream����Play�ӿڣ�����GPAPI_GetPort���������
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*                   LONG nPort, �����ö˿�
*                   HWND hWnd�� ���ţ���ʾͼ��ģ����ھ��
*                   PBYTE pFileHeadBuf�� �ļ�ͷ������ָ��
*                   DWORD nHeadSize�� �ļ�ͷ�����С			
*					char *strOsdChannelName ��Ƶ������ʾ��������������ʾ��չ��Ϣ����ֻ֧���豸����ɫ�Ǽʡ������豸��
					VIDEODRAWFUNC fnVideoDraw ��ͼ�ص���������ֻ֧���豸���������󻪡���ɫ�Ǽʡ����ڡ�ͼ��������������400�����VAG������
					LONG nUser ��ͼ�ص��û��Զ�������
*                   DWORD nBufPoolSize�� ���Ż����С��Ĭ��1�ף�DEFAULT_PLAY_BUFFER��
*                   STREAMMODE nMode�����ŷ�ʽ��Ĭ��ʵʱ��ģʽ��SM_REALTIME��
*                   int frameRate,���Ż�������󻺳�֡����Ĭ��25֡,���������豸������Ϊ6֡��ȡֵ6��50
*                   VIDEOFORMAT videoFormat, ��Ƶ��ʽ������ר�ã�Ĭ��PAL��VF_PAL��
*                   RESOLUTION resolution�� �����ֱ��ʣ�����ר�ã�Ĭ��CIF��R_CIF��
*                   
*   Output:         (null)
*   Return:			long���������󻪷����Ƿ񲥷ųɹ����ɹ�����0��ʧ�ܷ���-1
*                   �������ز��ž��
**************************************************************************/
GENERALPLAYER_API long GPAPI_StartPlay(PLAYERTYPE playerType, LONG& nPort, HWND hWnd, 
					  PBYTE pFileHeadBuf, DWORD nHeadSize, char * strOsdChannelName = "�����Ƽ�", VIDEODRAWFUNC fnVideoDraw = NULL, LONG nUser = 0, DWORD nBufPoolSize = DEFAULT_PLAY_BUFFER, 
					  STREAMMODE nMode = SM_REALTIME, int frameRate = 25, GP_VIDEOFORMAT videoFormat = VF_PAL,
					  RESOLUTION resolution = R_CIF);

/**************************************************************************
*   Function:		GPAPI_StopPlay
*   Description:    ֹͣ���ţ��˽ӿ������˳�����CloseStream��Stop�ӿ�
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*                   LONG nPort, ���Ŷ˿�
*   Output:         (null)
*   Return:			BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_StopPlay(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_InputData
*   Description:    �򲥷ſ��������ݣ������ݣ�����GPAPI_StartPlay�����
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*                   LONG nPort�������ö˿ڣ������ǲ��ž��
*                   PBYTE pBuf�������ݻ����ַ
*                   DWORD nSize�� �����ݴ�С
*   Output:         (null)
*   Return:			BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_InputData(PLAYERTYPE playerType, LONG nPort, PBYTE pBuf, DWORD nSize);

/**************************************************************************
*   Function:		GPAPI_ResetSourceBuffer
*   Description:    ���������ģʽ��Դ������ʣ�����ݣ��ݲ�֧�����ǡ���ɫ�Ǽ�Limit,��ͨ�����Ӣ���ء���Ծ�豸������̩
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*                   LONG nPort, ���Ŷ˿�
*   Output:         (null)
*   Return:			BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_ResetSourceBuffer(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_Pause
*   Description:    ��ͣ
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*                   LONG nPort, �����ö˿�
*                   BOOL bPause, TRUE����ͣ��FALSE�����
*   Output:         (null)
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_Pause(PLAYERTYPE playerType, LONG nPort,BOOL bPause);

/**************************************************************************
*   Function:		GPAPI_SetPlaySpeed
*   Description:    ���ò����ٶ�
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*                   LONG nPort, �����ö˿ڻ򲥷ž��
*                   PLAYSPEED playSpeed�������ٶ� 
*                   HWND hWnd, ������ʾ����
*   Output:         (null)
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetPlaySpeed(PLAYERTYPE playerType, LONG nPort,PLAYSPEED playSpeed, HWND hWnd);

/**************************************************************************
*   Function:		GPAPI_SetPlayReverseSpeed
*   Description:    ���õ��Ų����ٶ�
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪡�VAG�ȱ�ʶ
*                   LONG nPort, �����ö˿ڻ򲥷ž��
*                   PLAYSPEED playSpeed�������ٶ� 
*                   HWND hWnd, ������ʾ����
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
*	Remark:			Ŀǰֻ֧�ֺ����豸
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetPlayReverseSpeed(PLAYERTYPE playerType, LONG nPort,PLAYSPEED playSpeed, HWND hWnd);

/**************************************************************************
*   Function:		GPAPI_Capture
*   Description:    �Զ�ץͼ��BMP��JPEG��
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ���ݲ�֧�ֺ�ͨ
*                   LONG nPort, �����ö˿ڻ򲥷ž��
*                   LPCTSTR strFileFullPath��ץͼ����ľ���·��
*                   PICTUREFORMAT picFormat��ץͼ��ʽ��Ĭ��BMP
*   Output:         (null)
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_Capture(PLAYERTYPE playerType, LONG nPort, const char* strFileFullPath, 
					PICTUREFORMAT picFormat = PF_BMP);

/**************************************************************************
*   Function:		GPAPI_CaptureByUser
*   Description:    �û��ֶ�����ץͼ ��������ͼ���С�����֧��ץͼ��СΪ25M
*   Input:          PLAYERTYPE playerType,���������ͣ���ֻ֧�ֺ��������豸�����ڣ�ֻ֧��BMP��ʽ����ͼ����ֻ֧��BMP��ʽ����
					����400�豸��ֻ֧��BMP��ʽ)���ƺ��豸��ֻ֧��BMP��ʽ)������̩�豸��ֻ֧��BMP��ʽ)
*                   LONG nPort, �����ö˿ڻ򲥷ž��
*					PBYTE pBitmap, �������ͼ���ַ������С��ͼ���С
*					          �Ƽ���С:sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+w*h*4,
*					          ����w��h�ֱ�Ϊͼ����.
*					DWORD bmpSize,�����ʵ�ʵ�ͼ���С
*                   PICTUREFORMAT picFormat��ץͼ��ʽ��Ĭ��BMP
*   Output:         (null)
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_CaptureByUser(PLAYERTYPE playerType, LONG nPort, PBYTE pBitmap, DWORD &bmpSize, 
									 PICTUREFORMAT picFormat = PF_BMP);

/**************************************************************************
*   Function:		GPAPI_CaptureByUserEx
*   Description:    �ֶ�ץͼ��չ ������ͼ���С
*   Input:          PLAYERTYPE playerType,���������ͣ���ֻ֧�ֺ������󻪡�GB28181��VAG�豸
*                   LONG nPort, �����ö˿ڻ򲥷ž��
*					PBYTE pBitmap, �������ͼ���ַ������С��ͼ���С
*					          �Ƽ���С:sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+w*h*4,
*					          ����w��h�ֱ�Ϊͼ����.
*					DWORD dBufSize,���뻺������С
*                   PICTUREFORMAT picFormat��ץͼ��ʽ��Ĭ��BMP
*   Output:         DWORD & bmpSize ��õ�ʵ��ͼ���С
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_CaptureByUserEx(PLAYERTYPE playerType, LONG nPort, 
											 PBYTE pBitmap,DWORD dBufSize,DWORD &bmpSize,PICTUREFORMAT picFormat = PF_BMP);

/**************************************************************************
*   Function:		GPAPI_GetPictureSize
*   Description:    ��ȡԭʼͼ���С����ֻ֧�ֺ������󻪡�����̩�豸
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*                   LONG nPort, �����ö˿ڻ򲥷ž��
*   Output:         pWidth  ԭʼͼ��Ŀ��
*					pHeight ԭʼͼ��ĸ߶�
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_GetPictureSize(PLAYERTYPE playerType,LONG nPort, LONG * pWidth, LONG * pHeight);

// ��ͼ�ص�����(��������ӿڣ���Ҫע��ֱ����StartPlay�н��о���)
//GENERALPLAYER_API BOOL GPAPI_RigisterDrawFun(PLAYERTYPE playerType,LONG nPort,void (CALLBACK* DrawFun)(long nPort,HDC hDc,LONG nUser),LONG nUser);

/**************************************************************************
*   Function:		GPAPI_SetDisplayBuf
*   Description:    ���ò��Ż���,��ֻ֧�ֺ������󻪡����ǡ����ڡ������豸
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*                   LONG nPort, �����ö˿�
*                   DWORD nNum,������֡����ȡֵ��Χ1-50
*   Output:         (null)
*   Return:			BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetDisplayBuf(PLAYERTYPE playerType, LONG nPort,DWORD nNum);

/**************************************************************************
*   Function:		GPAPI_ThrowBFrameNum
*   Description:    ���ò����� B ֡֡��;������ B ֡,���Լ�СCPU������,
*                   ���������û�� B ֡����ô�������ֵ�����������á�
*                   ���ڿ��ٲ��ţ���֧�ֶ�·�� CPU������̫�ߵ�����¿��Կ���ʹ�á�
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*                   LONG nPort, �����ö˿�
*                   DWORD nNum, ������ B ֡��֡��(0-2)
*   Output:         (null)
*   Return:			BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_ThrowBFrameNum(PLAYERTYPE playerType, LONG nPort, DWORD nNum);

/**************************************************************************
*   Function:		GPAPI_SetDisplayCallBack
*   Description:    ����ץͼ�ص�
*   Input:          PLAYERTYPE playerType ��������,�纣�����󻪵ȱ�ʶ
*					LONG nPort   ���Ŷ˿�
*					DisplayCBFun ץͼ�ص��������粻��Ҫ����ΪNULL
*   Output:         (null)
*   Return:			BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
*   Remark:         ��ֻ֧�ֺ������󻪡������豸
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetDisplayCallBack(PLAYERTYPE playerType, LONG nPort, void (CALLBACK *DisplayCBFun)(long nPort,char * pBuf,long nSize,long nWidth,long nHeight,long nStamp,long nType,long Receaved));

/**************************************************************************
*   Function:		GPAPI_GetLastError
*   Description:    ��ȡ���������룬���ʹ��GPAPI_GetLastErrorString�ӿڻ�ȡ���ұ�ʶ
*   Input:          PLAYERTYPE playerType ��������,�纣�����󻪵ȱ�ʶ
*					LONG nPort   ���Ŷ˿�
*   Output:         (null)
*   Return:			DWORD ������
*   Remark:         ��ֻ֧�ֺ������󻪡���ɫ�Ǽʡ����ڡ����������ΰҵ���ƺӡ�ΰ���塢����̩��VAG�����豸
**************************************************************************/
GENERALPLAYER_API DWORD GPAPI_GetLastError(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_GetLastErrorString
*   Description:    ��ȡ���г��ұ�ʶ�Ĵ�����
*   Input:          PLAYERTYPE playerType ��������,�纣�����󻪵ȱ�ʶ
*					LONG nPort   ���Ŷ˿�
*   Output:         (null)
*   Return:			const char* ���г��ұ�ʶ�Ĵ�����Ϣ
*   Remark:         ��ֻ֧�ֺ������󻪡���ɫ�Ǽʡ����ڡ����������ΰҵ���ƺӡ�ΰ���塢����̩��VAG�����豸
**************************************************************************/
GENERALPLAYER_API const char* GPAPI_GetLastErrorString(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*  Function:  GPAPI_SetDecCallBack
*  brief:     ���ò��ſ����ص�
*  parameter: long nPort ���Ŷ˿�
*  parameter: SetDecCallBack �û��������ص�
*  parameter: char * pDest Ŀ������
*  parameter: long nDestSize Ŀ�����ݴ�С
*  parameter: long nUser   �û��Զ�������
*  returns:   BOOL �ɹ�����TRUE,ʧ�ܷ���FALSE
*  remark:    ��ֻ֧�ֺ����豸
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetDecCallBack(PLAYERTYPE playerType,long nPort,void* DecDataCallBack,char* pDest, long nDestSize, long nUser);

// ��������������
/**************************************************************************
*   Function:		GPAPI_PlaySound
*   Description:    ��������ͬһʱ��ֻ����һ·������
*                   ��������Ѿ��������򿪣����Զ��ر�ԭ���Ѿ��򿪵�������
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*                   LONG nPort, �����ö˿ڻ򲥷ž��
*   Output:         (null)
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL  GPAPI_PlaySound(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_StopSound
*   Description:    �ر�����
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*   Output:         (null)
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL  GPAPI_StopSound(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_PlaySoundShare
*   Description:    �򿪹���������ͬһʱ�̿��Դ򿪶�·����,��ֻ֧�ֺ��������豸
*                   �Թ���ʽ��������,���ű�·��������ر�����·��������
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ��
*                   LONG nPort, �����ö˿ڻ򲥷ž��
*   Output:         (null)
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL  GPAPI_PlaySoundShare(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_StopSoundShare
*   Description:    �رչ�������,��ֻ֧�ֺ��������豸
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*   Output:         (null)
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL  GPAPI_StopSoundShare(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_SetVolume
*   Description:    ��������,�����ڲ���֮ǰ���ã��ݲ�֧�����ǡ���ɫ�Ǽ�Limit����ͨ��Ӣ���ء�����̩�豸
*                   �����õ�ֵ�����棬����Ϊ��������ʱ�ĳ�ʼ������
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ��
*                   LONG nPort, �����ö˿ڻ򲥷ž��
*                   WORD nVolume, ������ֵ����Χ 0-100; 
*   Output:         (null)
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL  GPAPI_SetVolume(PLAYERTYPE playerType, LONG nPort, WORD nVolume);

/**************************************************************************
*   Function:		GPAPI_GetVolume
*   Description:    ��õ�ǰ���õ��������ݲ�֧�����ǡ���ɫ�Ǽ�Limit,��ͨ��Ӣ�����豸
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*                   LONG nPort, �����ö˿ڻ򲥷ž��
*   Output:         (null)
*   Return:			WORD,����ֵ
**************************************************************************/
GENERALPLAYER_API WORD  GPAPI_GetVolume(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_SetDisplayRegion
*   Description:    ���û�������ʾ����,�������ֲ��Ŵ���ʾ����ֻ֧�ֺ������󻪡�GB28181����ŵ�豸
*   Input:          PLAYERTYPE playerType,����������
*                   LONG nPort, �����ö˿ڻ򲥷ž��
*					DWORD nRegionNum����ʾ������ţ�0~3��
									���nRegionNum Ϊ0 �� ��ʾ����Ҫ��ʾ����
									(PlayM4_Play �����õĴ���) �������ã� ������
									hDestWnd ��bEnable �����á�
*					RECT * pSrcRect��������Ҫ��ʾ��ԭʼͼ���ϵ������磺���ԭʼͼ
									����352*288����ôpSrcRect �����õķ�Χֻ���ڣ�0��
									0��352��288��֮�С����pSrcRect=NULL,����ʾ����
									ͼ��
*					HWND hDestWnd��������ʾ���ڡ����������Ĵ����Ѿ����ù�����
									��������ô�ò��������ԡ�
*					BOOL bEnable, �򿪣����ã���ر���ʾ����
*   Return:			�ɹ�����TURE��ʧ�ܷ���FALSE
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetDisplayRegion(PLAYERTYPE playerType,LONG nPort,DWORD nRegionNum, RECT *pSrcRect,
											  HWND hDestWnd, BOOL bEnable);

// �ļ���������
/**************************************************************************
*   Function:		GPAPI_FileStart
*   Description:    �򿪲����ļ�,�ļ���С���ܳ���4G��С��4K
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*                   LONG nPort, �����ö˿ڻ򲥷ž��
*                   LPCTSTR strFileName,�ļ���
*                   HWND hWnd, ���Ŵ���
					VIDEODRAWFUNC fnVideoDraw,��ͼ�ص��������ݲ�֧���豸����ͨ��Ӣ�����豸
					LONG nUser����ͼ�ص��û��Զ�������
*   Output:         (null)
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_FileStart(PLAYERTYPE playerType, LONG &nPort, LPSTR strFileName, HWND hWnd, VIDEODRAWFUNC fnVideoDraw = NULL, LONG nUser = 0);

/**************************************************************************
*   Function:		GPAPI_FileStop
*   Description:    �رղ����ļ�
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*                   LONG nPort, �����ö˿ڻ򲥷ž��
*   Output:         (null)
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_FileStop(PLAYERTYPE playerType, LONG nPort); 

/**************************************************************************
*   Function:		GPAPI_SetPlayPos
*   Description:    �����ļ�����ָ������λ�ã��ٷֱȣ�,�ݲ�֧�����ǡ���ͨ��Ӣ�����豸
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*                   LONG nPort, �����ö˿ڻ򲥷ž��
*                   float fRelativePos, ����λ�ã���Χ 0-100%
*   Output:         (null)
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetPlayPos(PLAYERTYPE playerType, LONG nPort,float fRelativePos); 

/**************************************************************************
*   Function:		GPAPI_GetPlayPos
*   Description:    ����ļ�����ָ������λ�ã��ݲ�֧�����ǡ���ͨ��Ӣ�����豸
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
*                   LONG nPort, �����ö˿ڻ򲥷ž��
*   Output:         (null)
*   Return:			float, ����λ�ã���Χ 0-100%
**************************************************************************/
GENERALPLAYER_API float GPAPI_GetPlayPos(PLAYERTYPE playerType, LONG nPort); 

/**************************************************************************
*   Function:		GPAPI_GetFileTime
*   Description:    ��ȡ�ļ��ܵ�ʱ�䳤�ȣ���λ�룬�ݲ�֧�����ǡ���ͨ��Ӣ�����豸
*   Input:			PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ      
					LONG nPort, �����ö˿ڻ򲥷ž��
*   Return:			DWORD �ļ��ܵ�ʱ�䳤��,��λ��
**************************************************************************/
GENERALPLAYER_API DWORD GPAPI_GetFileTime(PLAYERTYPE playerType, LONG nPort); 

/**************************************************************************
*   Function:		GPAPI_GetPlayedTime
*   Description:    ��ȡ�ļ���ǰ���ŵ�ʱ�䣬��λ�룬�ݲ�֧�����ǡ���ͨ��Ӣ�����豸
*   Input:			PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ       
					LONG nPort, �����ö˿ڻ򲥷ž��
*   Return:			DWORD �ļ���ǰ���ŵ�ʱ�䣬��λ��
**************************************************************************/
GENERALPLAYER_API DWORD GPAPI_GetPlayedTime(PLAYERTYPE playerType, LONG nPort); 


/**************************************************************************
*   Function:		GPAPI_GetCurrentFrameRate
*   Description:    ��ȡ��ǰ֡�ʣ���ֻ֧�ֺ������󻪡�GB28181��VAG�豸
*   Input:			PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ       
					LONG nPort, �����ö˿ڻ򲥷ž��
	Remark:			��������е�֡��С��1���򷵻�0
*   Return:			DWORD ��ǰ֡��
**************************************************************************/
GENERALPLAYER_API DWORD GPAPI_GetCurrentFrameRate(PLAYERTYPE playerType, LONG nPort); 

/**************************************************************************
*   Function:		GPAPI_GetPlayedFrames
*   Description:    ��ȡ��ǰ�ѽ������Ƶ֡������ֻ�������󻪡�GB28181��VAG�豸
*   Input:			PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ       
					LONG nPort, �����ö˿ڻ򲥷ž��
*   Return:			DWORD �ѽ������Ƶ֡��
**************************************************************************/
GENERALPLAYER_API DWORD GPAPI_GetPlayedFrames(PLAYERTYPE playerType, LONG nPort); 


/**************************************************************************
*   Function:		GPAPI_GetSpecialData
*   Description:    ��ȡ��ǰ��ʾ֡��ȫ��ʱ��,��ֻ֧�ֺ������󻪣�������ͼ����ʾ����ܵ��ô˽ӿ�
*   Input:          PLAYERTYPE playerType,���������ͣ��纣�����󻪵ȱ�ʶ
					LONG nPort, �����ö˿ڻ򲥷ž��
*   Return:			DWORD ʧ�ܷ���-1,�ɹ�����һ��ȫ��ʱ���ѹ��ֵ����ȷ����
**************************************************************************/
GENERALPLAYER_API DWORD GPAPI_GetSpecialData(PLAYERTYPE playerType, LONG nPort); 

/**************************************************************************
*   Function:		GPAPI_SetFileEndCallback
*   Description:    �����ļ����Ž����ص��������ýӿ���OpenStream��OpenFile ֮ǰ
*                   ���ò���Ч�� 
*   Input:          PLAYERTYPE playerType ��������
*					LONG nPort, �����ö˿ڻ򲥷ž��
*					FileEndCallback���ص����� 
*                   void *pUser, �ص���������
*   Return:			BOOL,�ɹ�����TRUE,ʧ�ܷ���FALSE
*	Remark:			��ֻ֧�ֺ���
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetFileEndCallback(PLAYERTYPE playerType, LONG nPort,void(CALLBACK*FileEndCallback)(long nPort, void *pUser),void *pUser);

//Display type
#define DISPLAY_NORMAL  1   
#define DISPLAY_QUARTER 2
/**************************************************************************
*   Function:		GPAPI_SetDisplayType
*   Description:    ������ʾģʽ
*   Input:          LONG nPort, �����ö˿ڻ򲥷ž��LONG nType
					LONG nType������ģʽ��
					DISPLAY_NORMAL �����ֱ����������Կ���ʾ��
					DISPLAY_QUARTER 1/4 �ֱ����������Կ���ʾ��
					DISPLAY_YC_SCALE YC ����
					DISPLAY_NOTEARING ����ʾ˺��Ч��
*   Return:			�ɹ�����TURE��ʧ�ܷ���FALSE
*	Remark:			ֻ֧�ֺ����豸
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetDisplayType(PLAYERTYPE playerType, LONG nPort, LONG nType);

/**************************************************************************
*   Function:		GPAPI_GetSourceBufferRemain
*   Description:    ��ȡ���Ż�����ʣ�����ݴ�С
*   Input:          LONG nPort, �����ö˿ڻ򲥷ž��
*   Return:			DWORD:���ݴ�С����λ[BYTE]
*	Remark:			��ʱֻ֧�ֺ������󻪡�GB28181��VAG��ΰ�����豸
**************************************************************************/
GENERALPLAYER_API DWORD GPAPI_GetSourceBufferRemain(PLAYERTYPE playerType, LONG lPort);

/**************************************************************************
*   Function:		GPAPI_GetBufferValue
*   Description:    ��ȡָ���������Ĵ�С
*   Input:          LONG nPort, �����ö˿ڻ򲥷ž��,nBufType,����������.
*   Return:			DWORD:���ݴ�С
*	Remark:			��ʱֻ֧�ֺ�������
**************************************************************************/
GENERALPLAYER_API DWORD GPAPI_GetBufferValue(PLAYERTYPE playerType, LONG nPort,DWORD nBufType);

/**************************************************************************
*   Function:		GPAPI_PlayOneByOne
*   Description:    ��֡ǰ��
*   Input:          LONG nPort, �����ö˿ڻ򲥷ž��
*   Return:			BOOL,TRUE��ʾ���óɹ�
*	Remark:			��ʱֻ֧�ֺ������󻪡�GB28181�豸��VAG������̩��ΰ����
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_PlayOneByOne(PLAYERTYPE playerType, LONG nPort);

/*************************************************************************** 
*  Function:		GPAPI_PlayOneByOneRestore
*  Description:     ��֡���Żָ�
*  Input:           LONG nPort, �����ö˿ڻ򲥷ž��,HWND hWnd,���Ŵ���.
*  Return:			BOOL,TRUE��ʾ���óɹ�
*  Remark:			��ʱֻ֧�ֺ������󻪡�GB28181��VAG��ΰ�����豸
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_PlayOneByOneRestore(PLAYERTYPE playerType, LONG nPort, HWND hWnd);

/**************************************************************************
*   Function:		GPAPI_PlayOneByOneBack
*   Description:    ��֡���ˡ�ÿ����һ�ε���һ֡��ֻ֧���ļ����ţ��˺����������ļ�����
*                   ����֮����ܵ��� 
*   Input:          PLAYERTYPE playerType,���������ͣ�LONG nPort, �����ö˿ڻ򲥷ž��
*  Return:			BOOL,TRUE��ʾ���óɹ�
*  Remark:			��ʱֻ֧�ֺ������󻪡�GB28181��VAG��ΰ�����豸
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_PlayOneByOneBack(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_ReversePlay
*   Description:    �طŵ���
*   Input:          PLAYERTYPE playerType,���������ͣ�LONG nPort, �����ö˿ڻ򲥷ž��.
*   Return:			BOOL,TRUE��ʾ���óɹ�
*	Remark:			��ʱֻ֧�ֺ����衢GB28181�豸���������ſ�6.3�汾��ʼ֧�ָýӿڣ�
                    �ýӿڱ�����PLAYM4_Play֮�����
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_ReversePlay(PLAYERTYPE playerType, LONG nPort);

/**************************************************************************
*   Function:		GPAPI_SetDecodeFrameType
*   Description:    ������Ƶ֡��������
*   Input:          PLAYERTYPE playerType,���������ͣ�LONG nPort, �����ö˿ڻ򲥷ž��
					DWORD nFrameType ����֡����˵��
					#define DECODE_NORMAIL 0		//��������
					#define DECODE_KEY_FRAME 1		//ֻ��ؼ�֡
					#define DECODE_NONE 2			//������Ƶ֡
*   Return:			BOOL,TRUE��ʾ���óɹ�
*	Remark:			��ʱֻ֧�ֺ�����VAG�����豸
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_SetDecodeFrameType(PLAYERTYPE playerType, LONG nPort,DWORD nFrameType);

/**************************************************************************
*   Function:		GPAPI_RenderPrivateData
*   Description:    ��ʾ˽�����ݣ�������Ϣ��ʾ���أ���
*   Input:          PLAYERTYPE playerType,���������ͣ�LONG nPort, �����ö˿ڻ򲥷ž��
					int nIntelType �ڲ�����
					enum _PLAYM4_PRIDATA_RENDER{
					PLAYM4_RENDER_ANA_INTEL_DATA    = 0x00000001, //���ܷ���
					PLAYM4_RENDER_MD                = 0x00000002, //�ƶ����
					PLAYM4_RENDER_ADD_POS           = 0x00000004  //POS��Ϣ�����
					}PLAYM4_PRIDATA_RENDER
					BOOL bTrue	�Ƿ���ʾ
*   Return:			�ɹ�����TRUE;����ʱ����FALSE
*	Remark:			��ʱֻ֧�ֺ���������
					�˽ӿ������ڴ���������Ϣ��������
					����������Ϣ����������ô˽ӿ�û��Ч����
					POS������Ϣ��Ĭ�Ϲرյģ�����������Ĭ�Ͽ����ġ�
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_RenderPrivateData(PLAYERTYPE playerType,LONG nPort,int nIntelType, BOOL bTrue);



// ͨ�����ͽӿڣ����в������͵��������ݱ���ʱ���������������
/**************************************************************************/
/**	@fn	GPAPI_SetTransFormatCallback
*	@brief	<����ԭʼ������װת����>
*	@param   <playerType ��������>
*	@param	<nPort (IN) �����ö˿ڻ򲥷ž������nPortֵ�赥���������ܺ����ڲ��ŵ�nPort����>
*	@param	<pStreamDataFun (IN)��װת�������ݻص�����>
*	@param	<pcomplexHead (char*)����ͷ����>
*	@param	<complexHeadLen (IN)����ͷ����>
*	@param	<pUserData (IN)�û�����>
*	@remark  ���ò���   GPAPI_SetTransFormatCallback
									GPAPI_AnalyzeMediaHead	  
									GPAPI_InputTransformData
*	@return	�ɹ�����TRUE;����ʱ����FALSE.
*/
GENERALPLAYER_API BOOL GPAPI_SetTransFormatCallback(PLAYERTYPE playerType, LONG nPort, VAG_fGPAPIStreamDataCallBack pStreamDataFun, const char* pcomplexHead, int complexHeadLen, void* pUserData);

/**	@fn	    GPAPI_AnalyzeMediaHead
*	@brief	<����pStreamDataFun�лص�����ͷ>
*	@param   <playerType ��������>
*	@param	<pMediaHead (IN)ý��ͷ����ָ��>
*	@param	<iMediaHeadLen (IN)ý��ͷ���ݳ���>
*	@param	<pstMediaAnalyzeInfo (OUT)���صķ������> 
					<pMediaHead Ӧ����GPAPI_SetTransFormatCallback�ص�������ͷ���ݣ�
					LPVAG_MEDIA_HEAD_ANAL_INFO �е�
					char* pPriMediaHead �豸ԭʼͷ�����ļ��ʼ���м�Żص���������������
					stSysMediaHead �����ļ���ĩβ>
*	@return	�ɹ����� TRUE;����ʱ����FALSE.
*/
GENERALPLAYER_API BOOL GPAPI_AnalyzeMediaHead(PLAYERTYPE playerType, const char* pMediaHead, int iMediaHeadLen, LPVAG_MEDIA_HEAD_ANAL_INFO pstMediaAnalyzeInfo);

/**	@fn	GPAPI_InputTransformData
*	@brief	<������Ƶ������>
*	@param   <playerType ��������>
*	@param	<nPort (IN) �����ö˿ڻ򲥷ž����Ӧ��GPAPI_SetTransFormatCallback�����nPortֵһ��>
*	@param	<emDataType (IN)�������Ƶ����������>
*	@param	<pDataBuf (IN)�������Ƶ������ָ��>
*	@param	<nBufSize (IN)�������Ƶ�����ݳ���>
*	@remark	<��GPAPI_SetTransFormatCallback�󣬵��øú���>
*	@return	�ɹ�����TRUE;����ʱ����FALSE.
*/
GENERALPLAYER_API BOOL GPAPI_InputTransformData(PLAYERTYPE playerType, LONG nPort, VAG_STREAM_DATA_TYPE emDataType, char* pDataBuf, UINT64 nBufSize, void* pUserData);
//end of ͨ�����ͽӿڣ����в������͵��������ݱ���ʱ���������������
/**************************************************************************/




//���½ӿ�ֻ���VAG�����豸(����������ά������ʹ��GPAPIͨ�ýӿ�)
/**************************************************************************
**************************************************************************/

/**************************************************************************
*   Function:		GPAPI_VAG_Capture
*   Description:    VAGץͼ���������VAG����ʹ�ýӿڣ�GPAPI_CaptureByUserEx��
*   Input:          LONG nPort, �����ö˿ڻ򲥷ž��
*                   LPVPLAY_SNAP_PARA pstSnapPara, �����ö˿ڻ򲥷ž��
*                   pSnapShotFun��ץͼ�ص�����
*                   pUserData���û��Զ�������
*   Output:         (null)
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_VAG_Capture(LONG nPort, LVAG_SNAP_PARA pstSnapPara, VAG_fSnapCallback pSnapShotFun, void* pUserData);

/**************************************************************************
*   Function:		GPAPI_VAG_SaveFile
*   Description:    VAG�Զ�����¼���ļ����ýӿ���ͨ����ʼ���ź����
*   Input:          LONG nPort, �����ö˿ڻ򲥷ž��
*                   pFileName ����¼���ļ�·��
*   Output:         (null)
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_VAG_SaveFile( LONG nPort,const char* pFileName );

/**************************************************************************
*   Function:		GPAPI_VAG_CloseSaveFile
*   Description:    VAGֹͣ�Զ�����¼���ļ�
*   Input:          LONG nPort, �����ö˿ڻ򲥷ž��
*   Output:         (null)
*   Return:			BOOL, TRUE:�ɹ�,FALSE:ʧ�� 
**************************************************************************/
GENERALPLAYER_API BOOL GPAPI_VAG_CloseSaveFile( LONG nPort);

//���½ӿ�ֻ���VAG������ʽ�����豸ԭʼ������¼���ļ�
/**************************************************************************
**************************************************************************/
/**	@fn	    GPAPI_VAG_AnalyzeMediaHead
*	@brief	<����VAG����ͷ>
*	@param	<pMediaHead (IN)ý��ͷ����ָ��>
*	@param	<iMediaHeadLen (IN)ý��ͷ���ݳ���>
*	@param	<pstMediaAnalyzeInfo (OUT)���صķ������,
*			char* pPriMediaHead �豸ԭʼͷ�����ļ��ʼ��stSysMediaHead �����ļ���ĩβ>
*	@return	�ɹ����� TRUE;����ʱ����FALSE.
*/
GENERALPLAYER_API BOOL GPAPI_VAG_AnalyzeMediaHead(const char* pMediaHead, int iMediaHeadLen, LPVAG_MEDIA_HEAD_ANAL_INFO pstMediaAnalyzeInfo);

/**	@fn	GPAPI_VAG_SetTransFormatCallback
*	@brief	<VAGע��������װת���ص�����>
*	@param	<nPort (IN) �����ö˿ڻ򲥷ž�����ö˿������ⴴ��>
*	@param	<pStreamDataFun (IN)��װת�������ݻص�����>
*	@param	<pUserData (IN)�û�����>
*	@return	�ɹ�����TRUE;����ʱ����FALSE.
*/
GENERALPLAYER_API BOOL GPAPI_VAG_SetTransFormatCallback(LONG nPort, VAG_fGPAPIStreamDataCallBack pStreamDataFun, void* pUserData);

/**	@fn	GPAPI_VAG_InputTransformData
*	@brief	<����VAG��Ƶ������>
*	@param	<nPort (IN) �����ö˿ڻ򲥷ž��>
*	@param	<emDataType (IN)�������Ƶ����������>
*	@param	<pDataBuf (IN)�������Ƶ������ָ��>
*	@param	<nBufSize (IN)�������Ƶ�����ݳ���>
*	@remark	<���øú����������ݺ���GPAPI_VAG_SetTransFormatCallback���õĻص������лص����豸��ԭʼ����>
*	@return	�ɹ�����TRUE;����ʱ����FALSE.
*/
GENERALPLAYER_API BOOL GPAPI_VAG_InputTransformData(LONG nPort, VAG_STREAM_DATA_TYPE emDataType, char* pDataBuf, UINT64 nBufSize);

#endif // GUARD_GENERALPLAYERAPI_H