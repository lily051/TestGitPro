/*****************************************************
Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.   
FileName: FileLayer_define.h
Description: 结构和宏定义  	 
Author: guanguochen    
Date: 2009.06.10
*****************************************************/
#pragma once
#include "HPR_Config.h"
#include "HPR_Types.h"

#define MAX_URI_LEN 512

//打开类型
#define	OPENMODE_WRITE			0x1		//只写模式
#define	OPENMODE_READ			0x2		//只读模式

//媒体数据类型
#define	MEDIADATA_HEAD			0x1		//数据流头
#define	MEDIADATA_STREAM		0x2		//媒体数据流

//媒体数据单元类型
#define	MEDIAUNIT_I_FRAME		0x3		//I帧数据
#define MEDIAUNIT_FRAME			0x4		//视音频帧数据

//元数据单元类型
#define	METADATA_POS			0x400	//POS机信息
#define	METADATA_ATM			0x500	//ATM信息
#define	METADATA_IVS			0x600	//智能分析信息
#define	METADATA_ALARM			0x700	//报警信息
#define	METADATA_MOTION			0x800	//移动侦测信息

//定位类型
#define SEEK_ASB_TIME		0x1		//绝对时间
#define SEEK_RELATE_TIME	0x2		//相对时间

//时间单位
#define SEEK_SECOND			0x1		//秒
#define SEEK_MILLISECOND	0x2		//毫秒

//IO模型
#define	IOMODE_SYNC			0x1		//同步IO
#define	IOMODE_ASYNC		0x2		//异步IO

// 系统封装格式	
#define SYSTEM_NULL			0x0				//没有系统层，纯音频流或视频流	
#define SYSTEM_HIK          0x1				//海康文件层
#define SYSTEM_MPEG2_PS     0x2				//PS封装
#define SYSTEM_MPEG2_TS     0x3				//TS封装
#define SYSTEM_RTP          0x4				//rtp封装
#define SYSTEM_3GPP         0x5				//3GPP封装


/*********************MIME注册参数***add by hdj*********************************/
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
	HPR_UINT32 filesize;			//文件大小
	HPR_UINT64 rellastts;			//最后一个相对时间戳
	ABSTIME abslastts;				//最后一个绝对时间戳
	HPR_UINT32 frameamount;			//帧数
	HPR_UINT32 framerate;			//帧率
	HPR_UINT16 systemformat;		//封装格式
	HPR_UINT16 video_format;        //视频编码类型
    HPR_UINT16 audio_format;        //音频编码类型
} MEDIAFILE_INFO, *PMEDIAFILE_INFO;

typedef struct MediaUnit_Info
{
	HPR_UINT32 unitsize;			//单元大小
	HPR_UINT32 frameamount;			//帧数
	HPR_UINT32 framerate;			//帧率

	HPR_UINT32 FilePos;             //I帧位置
	HPR_UINT32 framenum;            //I帧序号
} MEDIAUNIT_INFO, *PMEDIAUNIT_INFO;

typedef struct MetaUnit_Info
{
	HPR_UINT32 unitsize;			//单元大小
} METAUNIT_INFO, *PMETAUNIT_INFO;

typedef struct FileInfo
{
	HPR_UINT32 filesize;			//文件大小
	HPR_UINT32 totaltime;           //播放总时间
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