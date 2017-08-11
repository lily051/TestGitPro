#ifndef CLOUD_CVI
#define CLOUD_CVI
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CloudAPIInitiative.h"
#include "CloudAPIPublic.h"

#ifdef _WIN32
#ifdef CLOUD_API_EXPORTS
#define CLOUD_API __declspec(dllexport)
#else
#define CLOUD_API __declspec(dllimport)
#endif
#else
#define CLOUD_API
#endif




extern "C"
{

	/** @fn CLOUD_API int API_GetAllDevStatus(unsigned int loginID, CDevStatusCallBack pDevStatusFunc,  void *pUserData)
	*  @brief 获取所有的设备状态
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param pDevStatusFunc[in]       设备状态信息回调函数
	*  @param Count[out]               查询到的设备状态总数量
	*  @param pUserData[in]            用户自定义数据
	*  @@return 成功=0 
	            失败<0:错误码
	*/
	CLOUD_API int API_GetAllDevStatus(unsigned int loginID, CDevStatusCallBack pDevStatusFunc, unsigned int &Count, void *pUserData);


	/** @fn CLOUD_API int API_DevStatus(unsigned int loginID,const char *CameraID, char *DevIP, int &Status,int& Defence,int& ReStatus,int& StreamStatus,int& FrameAnalyze)
	*  @brief 获取单个设备状态
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param CameraID[in]             编码器ID
	*  @param Online[out]              设备在线状态[0:不在线、1:在线]
	*  @param Status[out]              当前录像状态[0:异常、1:正常]
	*  @param Defence[out]             布防状态[0:未布防、1:已经布防]
	*  @param Stream[out]              读取视频流状态[0:异常、1:正常]
	*  @param Analy[out]               帧分析状态[0:异常、1:正常]
	*  @@return 成功=0 
	            失败<0: 错误码
	*/
	CLOUD_API int API_DevStatus(unsigned int loginID, const char *CameraID, unsigned int &Online, unsigned int& Status, unsigned int& Defence, unsigned int& Stream, unsigned int& Analy);


	/** @fn CLOUD_API int API_GetCloudStatus(unsigned int loginID, int *pStatus)
	*  @brief 获取云存储系统状态
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param pStatus[out]             云存储状态[1:在线、0:不在线]
	*  @@return 成功=0 
	            失败<0: 错误码
	*/
	CLOUD_API int API_GetCloudStatus(unsigned int loginID, int &pStatus);


	/** @fn int API_AddRecordPlan(unsigned int loginID, strRecordPlan *pRecordPlan)
	*  @brief 添加录像计划
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param pRecordPlan[in]          录像计划信息 
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_AddRecordPlan(unsigned int loginID, const strRecordPlan *pRecordPlan);


	/** @fn CLOUD_API int API_DelRecordPlan(unsigned int loginID, char *CameraID)
	*  @brief 删除录像计划
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param pRecordPlan[in]          编码器ID
	*  @@return 成功=0
	            失败<0: 错误码
	*/
	CLOUD_API int API_DelRecordPlan(unsigned int loginID, const char *CameraID);


	/** @fn CLOUD_API int API_ModifyRecordPlan(unsigned int loginID, strRecordPlan *pRecordPlan)
	*  @brief 删除录像计划
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param pRecordPlan[in]          录像计划信息
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_ModifyRecordPlan(unsigned int loginID, const strRecordPlan *pRecordPlan);


	/** @fn CLOUD_API int API_GetRecordPlan(unsigned int loginID, char *CameraID, strRecordPlan *pRecordPlan)
	*  @brief 获取指定编码器的录像计划
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param CameraID[in]             编码器ID
	*  @param pRecordPlan[out]         录像计划结信息
	*  @@return 成功=0
	            失败<0: 错误码
	*/
	CLOUD_API int API_GetRecordPlan(unsigned int loginID, const char *CameraID, strRecordPlan *pRecordPlan);


	/** @fn CLOUD_API int API_GetAllRecordPlan(unsigned int loginID, unsigned int *pCount, CRecordInfoCB pRecordInfoCB, void *pUserData)
	*  @brief 查询所有录像计划列表
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param pRecordPlanCB[in]        录像计划信息回调函数
	*  @param Count[in]                查询到的录像计划总数量
	*  @param pUserData[in]            用户自定义数据
	*  @@return 成功=0
	            失败<0: 错误码
	*/
	CLOUD_API int API_GetAllRecordPlan(unsigned int loginID, CRecordPlanCB pRecordPlanCB, unsigned int &Count, void *pUserData);


	/** @fn CLOUD_API int API_GetRecordInfoCB(unsigned int loginID, unsigned int *pCount, CRecordInfoCB pRecordInfoCB, void *pUserData)
	*  @brief 查询所有录像信息列表
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param pRecordInfoCB[in]        录像信息回调函数
	*  @param Count[in]                查询到的录像信息总数量
	*  @param pUserData[in]            用户自定义数据
	*  @@return 成功=0
	            失败<0: 错误码
	*/
	CLOUD_API int API_GetAllRecordInfo(unsigned int loginID, CRecordInfoCB pRecordInfoCB, unsigned int &Count, void *pUserData);


	/** @fn CLOUD_API int API_GetRecordInfo(unsigned int loginID, char *CameraID, strRecordInfo *pRecordInfo)
	*  @brief 查询指定编码器的录像信息
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param CameraID[in]             编码器ID
	*  @param pRecordInfo[out]         录像信息结构
	*  @@return 成功=0
	            失败<0: 错误码
	*/
	CLOUD_API int API_GetRecordInfo(unsigned int loginID, const char *CameraID, strRecordInfo *pRecordInfo);


	/** @fn CLOUD_API int API_AddEventRecord(unsigned int loginID, char *CameraID, unsigned int nRecordType, unsigned int nTime)
	*  @brief 添加事件录像(使用脉冲的方式下发录像信令)
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param CameraID[in]             编码器ID 
	*  @param nRecordType[in]          录像类型[3~254之间]
	*  @param nFlag[in]                录像信令类型[0 – 开始/持续录像；1 – 停止录像]
	*  @param nTime[in]                录像延时时间[在规定时间内没有收到下一个录像脉冲信令，停止事件录像]
	*  @@return 成功=0
	            失败<0: 错误码
	*/
	CLOUD_API int API_AddEventRecord(unsigned int loginID, const char *CameraID, unsigned int nRecordType, unsigned int nFlag, unsigned int nTime);



	/** @fn CLOUD_API int API_PlayViewStart(unsigned int loginID, char *CameraID, unsigned short StreamType, CPlayVeiwDataCB PlayViewDataCB, void *pUserData)
	*  @brief 开启预览
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param CameraID[in]             编码器ID 
	*  @param StreamType[in]           码流类型[0 – 主码流、1 – 子码流]
	*  @param PlayViewDataCB[in]       预览数据回调函数
	*  @param pUserData[in]            用户自定义数据
	*  @@return 成功=>0: 开启预览返回的句柄
	            失败<0: 错误码
	*/
	CLOUD_API int API_PlayViewStart(unsigned int loginID, const char *CameraID, unsigned short StreamType, CPlayVeiwDataCB PlayViewDataCB, void *pUserData);


	/** @fn CLOUD_API int API_PlayViewStop(unsigned int loginID , int PlayViewHandle)
	*  @brief 停止预览
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param PlayViewHandle[in]       成功开启预览返回的句柄  
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_PlayViewStop(unsigned int loginID , int PlayViewHandle);
}

#endif //CLOUD_CVI