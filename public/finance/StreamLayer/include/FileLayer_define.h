/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: FileLayer_define.h
Description: �ṹ�ͺ궨��  	 
Author: guanguochen    
Date: 2009.06.10
*****************************************************/
#pragma once
#include "HPR_Config.h"
#include "HPR_Types.h"

#define MAX_URI_LEN 512

//������
#define	OPENMODE_WRITE			0x1		//ֻдģʽ
#define	OPENMODE_READ			0x2		//ֻ��ģʽ

//ý����������
#define	MEDIADATA_HEAD			0x1		//������ͷ
#define	MEDIADATA_STREAM		0x2		//ý��������

//ý�����ݵ�Ԫ����
#define	MEDIAUNIT_I_FRAME		0x3		//I֡����
#define MEDIAUNIT_FRAME			0x4		//����Ƶ֡����

//Ԫ���ݵ�Ԫ����
#define	METADATA_POS			0x400	//POS����Ϣ
#define	METADATA_ATM			0x500	//ATM��Ϣ
#define	METADATA_IVS			0x600	//���ܷ�����Ϣ
#define	METADATA_ALARM			0x700	//������Ϣ
#define	METADATA_MOTION			0x800	//�ƶ������Ϣ

//��λ����
#define SEEK_ASB_TIME		0x1		//����ʱ��
#define SEEK_RELATE_TIME	0x2		//���ʱ��

//ʱ�䵥λ
#define SEEK_SECOND			0x1		//��
#define SEEK_MILLISECOND	0x2		//����

//IOģ��
#define	IOMODE_SYNC			0x1		//ͬ��IO
#define	IOMODE_ASYNC		0x2		//�첽IO

// ϵͳ��װ��ʽ	
#define SYSTEM_NULL			0x0				//û��ϵͳ�㣬����Ƶ������Ƶ��	
#define SYSTEM_HIK          0x1				//�����ļ���
#define SYSTEM_MPEG2_PS     0x2				//PS��װ
#define SYSTEM_MPEG2_TS     0x3				//TS��װ
#define SYSTEM_RTP          0x4				//rtp��װ
#define SYSTEM_3GPP         0x5				//3GPP��װ


/*********************MIMEע�����***add by hdj*********************************/
//MIME Registration parameter for MPEG4
typedef struct _MPEG4_MIME_SETS_
{

}MPEG4_MIME_SETS;

//MIME Registration parameter for AVC
typedef struct _AVC_MIME_SETS_
{

}AVC_MIME_SETS;
/*******************************************************************************/

typedef struct abstime
{
	HPR_UINT32 year;
	HPR_UINT32 month;
	HPR_UINT32 day;
	HPR_UINT32 hour;
	HPR_UINT32 minute;
	HPR_UINT32 second;
} ABSTIME, *PABSTIME;

typedef struct MediaFile_Info
{
	HPR_UINT32 filesize;			//�ļ���С
	HPR_UINT64 rellastts;			//���һ�����ʱ���
	ABSTIME abslastts;				//���һ������ʱ���
	HPR_UINT32 frameamount;			//֡��
	HPR_UINT32 framerate;			//֡��
	HPR_UINT16 systemformat;		//��װ��ʽ
	HPR_UINT16 video_format;        //��Ƶ��������
    HPR_UINT16 audio_format;        //��Ƶ��������
} MEDIAFILE_INFO, *PMEDIAFILE_INFO;

typedef struct MediaUnit_Info
{
	HPR_UINT32 unitsize;			//��Ԫ��С
	HPR_UINT32 frameamount;			//֡��
	HPR_UINT32 framerate;			//֡��

	HPR_UINT32 FilePos;             //I֡λ��
	HPR_UINT32 framenum;            //I֡���
} MEDIAUNIT_INFO, *PMEDIAUNIT_INFO;

typedef struct MetaUnit_Info
{
	HPR_UINT32 unitsize;			//��Ԫ��С
} METAUNIT_INFO, *PMETAUNIT_INFO;

typedef struct FileInfo
{
	HPR_UINT32 filesize;			//�ļ���С
	HPR_UINT32 totaltime;           //������ʱ��
} FILEINFO, *PFILEINFO;

typedef HPR_VOID (*FileAsyncIOCB)(HPR_ULONG ErrorCode, HPR_ULONG NumberOfBytes, HPR_VOIDPTR SelfDefineData);

#if defined(OS_WINDOWS)
	#if defined(FILELAYER_EXPORTS)
		#define FILELAYER_DECLARE extern "C" __declspec(dllexport)
	#else
		#define FILELAYER_DECLARE extern "C" __declspec(dllimport)
	#endif
#elif defined(OS_POSIX)
	#define FILELAYER_DECLARE extern "C"
#else
	#error OS Not Implement Yet.
#endif