/** @file   CloudAPIInitiative.h
*  @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*  @brief  API Interface H File
*  @author weiwenjuan@hikvision.com.cn
*  @date   2013-01-17
*
*  @change log
*
*/

#ifndef __CLOUD_API_INITIATIVE_H__
#define __CLOUD_API_INITIATIVE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_CAMERAID_LEN		64
#define MAX_DEV_DATA_LEN		64
#define MAX_SDK_DAYS			7
#define MAX_SDK_TIMESEGMENT		8
#define MAX_IP_LEN				64

typedef unsigned char       BYTE;
typedef unsigned long       DWORD;

extern "C"
{

	// 录像信息数据结构定义
	typedef struct tagstrRecordInfo
	{
		char CamerID[MAX_CAMERAID_LEN] ;   // 编码器ID
		unsigned int StartTime;			   // 录像最早时间
		unsigned int CoverTime;			   // 覆盖后的录像最早时间
		unsigned int LastTime;			   // 录像结束时间 
		long long ToatalSizeunsigned;	   // 录像文件的总大小
	}strRecordInfo;


	// 设备状态数据结构定义
	typedef struct tagDevStatus
	{
		char CamerID[MAX_CAMERAID_LEN];	   // 编码器ID
		unsigned int Online;	           // 编码器在线状态 [0:不在线、1:在线]
		unsigned int Status;	           // 录像状态 [0:异常、1:正常]
		unsigned int Defence;	           // 布防状态 [0:没有布防、1:布防]
		unsigned int Stream;	           // 取流状态 [0:异常、1:正常]
		unsigned int Analy;		           // 帧分析状态 [0:异常、1:正常]
	}DevStatus;



	//时间段(子结构)
	typedef struct _sdk_schedtime
	{
		//开始时间
		unsigned char byStartHour;
		unsigned char byStartMin;

		//结束时间
		unsigned char byStopHour;
		unsigned char byStopMin;
	}SDK_SCHEDTIME, *LPSDK_SCHEDTIME;

	//录像计划
	typedef struct _sdk_recordday
	{
		unsigned short wAllDayRecord;                    // 是否全天录像[0:否; 1:是]
		unsigned char byRecordType;                      // [0:定时录像; 1:移动侦测录像]
		char    reservedData;                            // 保留值
	}SDK_RECORDDAY, *LPSDK_RECORDDAY;

	typedef struct _sdk_recordsched
	{
		SDK_SCHEDTIME struRecordTime;                    // 录像时间
		unsigned char byRecordType;                      // 录象类型[0:定时录像; 1:移动侦测录像]
		char reservedData[3];//
	}SDK_RECORDSCHED, *LPSDK_RECORDSCHED;

	typedef struct _sdk_recordplan{
		unsigned int dwSize;                            // 结构体大小
		unsigned int dwRecord;                          // 是否录像[0:否; 1:是]
		unsigned int nStreamType;						//码流类型【0-主码流；1-子码流】
		SDK_RECORDDAY struRecAllDay[MAX_SDK_DAYS];
		SDK_RECORDSCHED struRecordSched[MAX_SDK_DAYS][MAX_SDK_TIMESEGMENT];
		unsigned int dwRecordTime;                      // (保留)录象延时长度0-5秒，1-20秒，2-30秒，3-1分钟，4-2分钟，5-5分钟，6-10分钟
		unsigned int dwPreRecordTime;                   // 预录[0:不预录; 1:开始预录]
		unsigned int dwRecorderDuration;	            // (保留)录像保存的最长时间
		unsigned char byRedundancyRec;	                // (保留)是否冗余录像,重要数据双备份：/1
		unsigned char byAudioRec;	                    // (保留)录像时复合流编码时是否记录音频数据：国外有此法规
		unsigned char byReserve[10];                    // (保留)
	}SDK_RECORD, *LPSDK_RECORD;


	// 录像信息数据结构定义
	typedef struct strRecordPlan
	{
		char szDevIP[MAX_IP_LEN];                  // 前端设备IP
		char szCameraID[MAX_CAMERAID_LEN];         // 编码器ID
		char szCameraName[MAX_CAMERAID_LEN];	   // 编码器名称
		char szUserName[MAX_DEV_DATA_LEN];         // 登录前端设备的用户名
		char szPassword[MAX_DEV_DATA_LEN];         // 登录前端设备的密码
		unsigned int nDevPort;                     // 前端设备端口号
		unsigned int nDevType;                     // 设备类型[0:海康、1:大华]
		unsigned int nChanlNum;                    // 设备通道号
		unsigned short nTransType;                 // 流媒体从前端设备取流的协议类型[0:RTSP、1:RTP/TCP、2:RTP/UDP】
		char szStreamIP[MAX_IP_LEN];               // 流媒体服务器IP
		unsigned int nStreamPort;                  // 流媒体端口号
		unsigned int nRecordPoolID;                // 录像池ID
		SDK_RECORD pRecordTask;                    // 录像计划
		unsigned int nAlloc;					   // 录像计划下发状态[0:未下发、1:已下发]
	}SDK_RECORDPLAN, *LPSDK_RECORDPLAN;




	// 获取设备状态信息回调函数
	// DevStatus *DevStatusInfo    设备状态信息结构
	// unsigned int Count          查询到的设备状态信息总数量
	// int Flat                    剩余设备状态信息数量[等于0: 表示信息全部返回、 小于0: 为错误码、大于0: 表示剩余的信息总数]
	//void *pUserData              用户自定义信息
	
	//  备注：当Flat标示符为0，Count标示符不为0时，表示获取当前云存储系统的设备状态信息结束
	//        当Flat标示符为0，Count标示符也为0时，表示回调结束
	typedef void (*CDevStatusCallBack)(DevStatus *DevStatusInfo, unsigned int Count, int Flat, void *pUserData);


	// 获取录像信息回调函数
	// strRecordInfo *RecordInfo   录像信息结构
	// unsigned int Count          查询到的录像信息总数量
	// int Flat                    剩余录像信息数量[小于等于0: 表示信息全部返回、 小于0: 为错误码、大于0: 表示剩余的信息总数]
	// void *pUserData             用户自定义信息    
	
	//  备注：当Flat标示符为0，Count标示符不为0时，表示获取当前云存储系统的录像信息结束
	//        当Flat标示符为0，Count标示符也为0时，表示回调结束           
	typedef void (*CRecordInfoCB)(strRecordInfo *RecordInfo, unsigned int Count, int Flat, void *pUserData) ;


	// 获取预览视频数据回调函数
	// int PlayViewHandle          成功开启预览返回的句柄
	// int nDataType               获取的数据类型[1:文件头、2:视频数据、3:音频数据]
	// void *pDataBuf              获取的视频数据(当pDataBuf为NULL且nBufLen为0时表示数据回调结束)
	// unsigned int nBufLen        获取的视频数据长度
	// void *pUserData             用户自定义信息
	typedef void (*CPlayVeiwDataCB)(int PlayViewHandle, int nDataType, void *pDataBuf, unsigned int nBufLen, void *pUserData);


	// 获取录像计划信息回调函数
	// strRecordPlan *RecordInfo   录像信息数据结构
	// unsigned int Count          查询到的录像计划信息总数量
	// int Flat                    剩余录像计划信息数量[小于等于0: 表示信息全部返回、 小于0: 为错误码、大于0: 表示剩余的信息总数]
	// void*pUserData              用户自定义信息
	
	//  备注：当Flat标示符为0，Count标示符不为0时，表示获取当前云存储系统的录像计划信息结束
	//        当Flat标示符为0，Count标示符也为0时，表示回调结束
	typedef void (*CRecordPlanCB)(strRecordPlan *RecordInfo, unsigned int Count, int Flat, void*pUserData);

}//extern "C"

#endif  //__CLOUD_API_INITIATIVE_H__
