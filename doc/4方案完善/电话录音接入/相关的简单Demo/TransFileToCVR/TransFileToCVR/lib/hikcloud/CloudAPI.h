/** @file   Cloud_API.h
*  @note	HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*  @brief	API Interface H File
*  @author weiwenjuan@hikvision.com.cn
*  @date	2013-01-17
*
*  @change log
*
*/
//lint -library
#ifndef __CLOUD_API_H__
#define __CLOUD_API_H__

#include "CloudAPIDefine.h"

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

	/*****************************************************************************
	*                                                                            *
	*                             视频录像部分                                   *
	*                                                                            *
	******************************************************************************/


	/** @fn CLOUD_API int API_RecordStart(unsigned int loginID,const char* CameraID,
	*                  int RecordType,RecordDataType DataType,RecordAttachedType AttachedType,
	*					int FrameSupport,unsigned int PoolID,const char* HeadBuff,int HeadSize)
	*  @brief 开始录像
	*  @param loginID[in]        成功登陆云存储返回的句柄
	*  @param CameraID[in]       编码器ID
	*  @param RecordType[in]     录像类型
	*  @param Data_Type[in]      录像数据类型
	*  @param Attached_Type[in]  录像附加类型
	*  @param FrameSupport[in]   是否自定义I帧 [0: 不自定义I帧  1: 自定义I帧]
	*  @param PoolID[in]         创建录像池成功返回的ID
	*  @param HeadBuff[in]       视频数据头buf(例如：海康的文件头)
	*  @param HeadSize[in]       视频数据头大小
	*  @@return 成功>=0 :录像句柄
	            失败<0 :错误码
	*/
	CLOUD_API int API_RecordStart(unsigned int loginID, const char* CameraID, int RecordType,
	                              RecordDataType DataType, RecordAttachedType AttachedType, int FrameSupport,
								  unsigned int PoolID, const char* HeadBuff, int HeadSize);


	/** @fn CLOUD_API int API_RecordData(unsigned int loginID, int RecordHandle,FrameType Type ,unsigned int BeginTime,
	*                                    unsigned int EndTime ,const char* Buffer,unsigned int Bufflen)
	*                                       
	*  @brief 录像数据
	*  @param loginID[in]       成功登陆云存储返回的句柄
	*  @param RecordHandle[in]  成功开启录像返回的句柄
	*  @param Type[in]          帧类型
	*  @param BeginTime[in]     录像开始时间
	*  @param EndTime[in]       录像结束时间
	*  @param Buffer[in]        数据
	*  @param Bufflen[in]       数据长度
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_RecordData(unsigned int loginID, int RecordHandle, FrameType Type,
		                         unsigned int BeginTime, unsigned int EndTime, const char* Buffer, unsigned int Bufflen);


	/** @fn CLOUD_API int API_SetRecordType(unsigned int loginID,int RecordHandle,int NewRecordType)
	*  @brief 修改录像类型
	*  @param loginID[in]       成功登陆云存储返回的句柄
	*  @param RecordHandle[in]  成功开启录像返回的句柄
	*  @param NewRecordType[in] 新录像类型
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_SetRecordType(unsigned int loginID, int RecordHandle, int NewRecordType);


#ifdef RECORD_REDUNDANCY
#endif
    /** @fn CLOUD_API int API_redundancy(unsigned int loginID, int handle, int poolID, unsigned int Type)
    *  @brief 录像冗余
    *  @param loginID[in]       成功登陆云存储返回的句柄
    *  @param handle[in]        开启录像的句柄
    *  @param poolID[in]        冗余的目的录像池ID
    *  @param Type[in]          冗余任务类型：1：设定任务；2：取消任务
    *  @@return 成功=0
                失败<0 :错误码
    */
    CLOUD_API int API_redundancy(unsigned int loginID, int handle, unsigned int poolID, unsigned int Type);

	/** @fn CLOUD_API int API_RecordStop(unsigned int loginID,int RecordHandle)
	*  @brief 录像停止
	*  @param loginID[in]       成功登陆云存储返回的句柄
	*  @param RecordHandle[in]  成功开启录像返回的句柄
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_RecordStop(unsigned int loginID, int RecordHandle);

	/***************************************************************************************
	*                                                                                      *
	*                                         图片部分                                     *
	*                                                                                      *
	****************************************************************************************/


	/** @fn CLOUD_API int API_PictureStoreStart(unsigned int loginID,const char* CameraID , unsigned int PoolID)
	*  @brief 开启存储图片
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param CameraID[in]             编码器ID [最大64字符]
	*  @param PoolID[in]               成功创建录像池返回的ID
	*  @@return 成功>=0:图片存储句柄
	            失败<0 :错误码
	*/
	CLOUD_API int API_PictureStoreStart(unsigned int loginID, const char* CameraID, unsigned int PoolID);


	/** @fn CLOUD_API int API_PictureStoreData(unsigned int loginID,int PictureStoreHandle,unsigned int TimePos,unsigned short PictureType,const char* buff ,unsigned int bufflen,char* RetURL)
	*  @brief 写入图片数据
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param PictureStoreHandle[in]   成功开启图片存储返回的句柄
	*  @param TimePos[in]              图片时间戳[存储图片的时间或前端编码器的时间]
	*  @param PictureType[in]          图片类型 [1:JPG、2:BMP、3:PNG] 
	*  @param buff[in]                 图片数据信息
	*  @param bufflen[in]              图片数据长度
	*  @param RetURL[out]              返回保存图片的URL
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_PictureStoreData(unsigned int loginID, int PictureStoreHandle, unsigned int TimePos,
		                               unsigned short PictureType, const char* buff, unsigned int bufflen, char* RetURL);


	/** @fn CLOUD_API int API_PictrueStoreStop(unsigned int loginID,int PictureStoreHandle)
	*  @brief 停止存储图片
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param PictureStoreHandle[in]   成功开启图片存储返回的句柄
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_PictureStoreStop(unsigned int loginID, int PictureStoreHandle);


	/** @fn CLOUD_API int API_PictureStoreDataEx(unsigned int loginID,int PictureStoreHandle,unsigned int TimePos,unsigned short PictureType,const char* buff ,unsigned int bufflen,char* RetURL)
	*  @brief 写入图片数据(为新业务定制)
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param PictureStoreHandle[in]   成功开启图片存储返回的句柄
	*  @param TimePos[in]              图片时间戳[存储图片的时间或前端编码器的时间]
	*  @param PictureType[in]          图片类型 [1:JPG、2:BMP、3:PNG] 
	*  @param buff[in]                 存储的图片数据
	*  @param bufflen[in]              存储数据长度
	*  @param Radom[int]               随机码[当前时间段内存储的图片序号，由调用程序生成的4位随机数，1000 ～  9999]
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_PictureStoreDataEx(unsigned int loginID, int PictureStoreHandle, unsigned int TimePos,
		                                 unsigned short PictureType,  const char* buff, unsigned int bufflen, unsigned int Radom);


	/** @fn CLOUD_API int API_PictrueStoreStop(unsigned int loginID,int PictureStoreHandle)
	*  @brief 停止图片存储(为新业务定制)
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param PictureStoreHandle[in]   成功开启图片存储返回的句柄
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_PictureStoreStopEx(unsigned int loginID,int PictureStoreHandle);


	/** @fn CLOUD_API int API_PictureDeleteByURL(unsigned int loginID,const char* URL)
	*  @brief 删除当前RUL的图片
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param URL[in]                  保存图片的URL 
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_PictureDeleteByURL(unsigned int loginID,const char* URL);


	/** @fn CLOUD_API int API_PictureDeleteByTime(unsigned int loginID,const char* CameraID,unsigned int BeginTime ,unsigned int EndTime)
	*  @brief 删除当前时间段的图片
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param CameraID[in]             编码器ID [最大64字符]
	*  @param BeginTime[in]            开始时间
	*  @param EndTime[in]              结束时间 
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_PictureDeleteByTime(unsigned int loginID, const char* CameraID, unsigned int BeginTime , unsigned int EndTime);


	/** @fn CLOUD_API int API_PictureLockByURL(unsigned int loginID,const char* URL,unsigned int LockTime)
	*  @brief 锁定当前URL的图片
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param URL[in]                  保存图片的URL
	*  @param LockTime[in]             锁定时间[0: 表示解锁、大于0: 表示锁定的时长(从当前时间到需要解锁时间点的时长) 单位:秒]
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_PictureLockByURL(unsigned int loginID, const char* URL, unsigned int LockTime);


	/** @fn CLOUD_API int API_PictureLockByTime(unsigned int loginID,const char* CameraID
	*                                       ,unsigned int BeginTime,unsigned int EndTime,unsigned int LockTime)
	*  @brief 锁定当前时间段内的图片
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param BeginTime[in]            开始时间
	*  @param EndTime[in]              结束时间
	*  @param LockTime[in]             锁定时间[0: 表示不锁定、大于0: 表示锁定的时长(从当前时间到需要解锁时间点的时长) 单位:秒]
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_PictureLockByTime(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		                                unsigned int EndTime, unsigned int LockTime);


	/** @fn CLOUD_API int API_PictureDownloadByURL(unsigned int loginID,const char* URL,unsigned int &Count,CPictureDownLoadCB BuffCB,void* pUser,float zoom )
	*  @brief 按URL下载图片
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param URL[in]                  保存图片的URL 
	*  @param ZoomType[in]             缩放类型[0:不缩放、1:按尺寸缩放比例、2:按大小缩放比例]
	*  @param ZoomValue[in]            缩放比例值[取值1-99 按百分比例计算]
	*  @param pUserData[in]            可拓展参数 保留
	*  @param pBuf[in]                 图片缓存内存
	*  @param BufLen[out]              图片大小 单位字节
	*  @param PictureType[out]         图片类型 [1:JPG、2:BMP、3:PNG] 
	*  @@return 成功=0
	失败<0 :错误码
	*/
	CLOUD_API int API_PictureDownloadByURL(unsigned int loginID,const char* URL,int ZoomType,int ZoomValue,void* pUserData ,char* pBuf,int& BufLen,int&PictureType );


	/** @fn CLOUD_API int API_PictureDownloadByTime(unsigned int loginID,
	*                      const char* CameraID,const char* PictureType,unsigned int BeginTime,
	*                     unsigned int EndTime,unsigned int &Count,CPictureDownLoadCB BuffCB,void* pUser,float zoom)
	*  @brief 按时间下载图片
	*  @param loginID[in]              成功登陆云存储返回的句柄
	*  @param CameraID[in]             编码器ID 最大64字符
	*  @param PictureType[in]          需要下载的图片类型(可以使用组合类型)
	*  @param BeginTime[in]            开始时间
	*  @param EndTime[in]              结束时间
	*  @param Count[out]               查询到的图片总数量
	*  @param BuffCB[in]               数据回调函数
	*  @param pUser[in]                用户自定义数据
	*  @param ZoomType[in]             缩放类型[0:不缩放、1:按尺寸缩放比例、2:按大小缩放比例]
	*  @param zoom[in]                 缩放比例值 [取值1-99 按百分比例计算]
	*  @@return 成功>=0:下载句柄
	            失败<0 :错误码
	*/
	CLOUD_API int API_PictureDownloadByTime(unsigned int loginID, const char* CameraID, const char* PictureType,
		                                    unsigned int BeginTime, unsigned int EndTime, unsigned int &Count,
											CPictureDownLoadCBEx BuffCB, void* pUser, int ZoomType, int ZoomValue);


	/** @fn CLOUD_API int API_PictureDownloadByTimeStop(unsigned int loginID,int PictureDownloadHandler)
	*  @brief 停止按时间下载图片
	*  @param loginID[in]                成功登陆云存储返回的句柄
	*  @param PictureDownloadHandler[in] 成功开启图片下载返回的句柄
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_PictureDownloadByTimeStop(unsigned int loginID, int PictureDownloadHandler);



	/**************************************************************************
	*                                                                         *
	*                                附属信息部分                             *
	*                                                                         *
	***************************************************************************/

	/** @fn CLOUD_API int API_AttachedInfoStart(unsigned int loginID,const char* CameraID,
	*                                             unsigned int PoolID,const char* HeadBuff,int HeadSize)
	*  @brief 开始保存附属信息
	*  @param loginID[in]                成功登陆云存储返回的句柄
	*  @param CameraID[in]               编码器ID 最大64字符
	*  @param PoolID[in]                 录像池ID
	*  @param HeadBuff[in]               附属信息数据头
	*  @param HeadSize[in]               数据头大小
	*  @@return 成功>0 :附属信息存储句柄
	            失败<0 :错误码
	*/
	CLOUD_API int API_AttachedInfoStart(unsigned int loginID, const char* CameraID, unsigned int PoolID,
		                                const char* HeadBuff, int HeadSize);


	/** @fn CLOUD_API int API_AttachedInfoData(unsigned int loginID,int AttachedInfoHandle
	*                             ,unsigned int BeginTime,unsigned int EndTime,const char* Buff,int Bufflen)
	*  @brief 附属信息数据存储
	*  @param loginID[in]                成功登陆云存储返回的句柄
	*  @param AttachedInfoHandle[in]     成功开启写入附属信息返回的句柄
	*  @param BeginTime[in]              开始时间
	*  @param EndTime[in]                结束时间
	*  @param Buff[in]                   附属信息数据
	*  @param Bufflen[in]                附属信息数据长度
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_AttachedInfoData(unsigned int loginID, int AttachedInfoHandle, unsigned int BeginTime,
		                               unsigned int EndTime, const char* Buff, int Bufflen);


	/** @fn CLOUD_API int API_AttachedInfoStop(unsigned int loginID,int AttachedInfoHandle)
	*  @brief 附属信息停止存储
	*  @param loginID[in]                成功登陆云存储返回的句柄
	*  @param AttachedInfoHandle[in]     成功开启写入附属信息返回的句柄
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_AttachedInfoStop(unsigned int loginID,int AttachedInfoHandle);


	/** @fn CLOUD_API int API_AttachedInfoLock(unsigned int loginID,
	*                     const char* CameraID,unsigned int BeginTime,unsigned int EndTime,unsigned int LockTime)
	*  @brief 附属信息锁定
	*  @param loginID[in]                成功登陆云存储返回的句柄
	*  @param CameraID[in]               编码器ID 最大64字符
	*  @param BeginTime[in]              开始时间
	*  @param EndTime[in]                结束时间
	*  @param LockTime[in]               锁定时长[0: 表示解锁、大于0: 表示锁定的时长(从当前时间到需要解锁时间点的时长) 单位:秒]
	*  @@return 成功=0
	失败<0 :错误码
	*/
	CLOUD_API int API_AttachedInfoLock(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		                               unsigned int EndTime, unsigned int LockTime);

	/** @fn CLOUD_API int API_AttachedSearch(unsigned int loginID,const char* CameraID,unsigned int BeginTime,unsigned int EndTime,unsigned int &Count,CAttachedSearchDataCb AttachedSearchDataCb,void* pUser)
	*  @brief 附属信息检索
	*  @param loginID[in]                成功登陆云存储返回的句柄
	*  @param CameraID[in]               编码器ID 最大64字符
	*  @param BeginTime[in]              开始时间
	*  @param EndTime[in]                结束时间
	*  @param Count[out]                 查询到的附属信息段总数量
	*  @param AttachedSearchDataCb[in]   回调函数
	*  @param pUser[in]                  用户自定义数据
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_AttachedSearch(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		unsigned int EndTime, unsigned int &Count, CAttachedSearchDataCb AttachedSearchDataCb, void* pUser);


	/** @fn CLOUD_API int API_AttachedDownloadStart(unsigned int loginID,
	*                      const char* CameraID,unsigned int BeginTime,unsigned int EndTime,int Downloadoffset)
	*  @brief 附属信息开始下载
	*  @param loginID[in]                成功登陆云存储返回的句柄
	*  @param CameraID[in]               编码器ID 最大64字符
	*  @param BeginTime[in]              开始时间
	*  @param EndTime[in]                结束时间
	*  @param Downloadoffset[in]         已经下载的字节数
	*  @@return 成功>0 :附属信息下载句柄
	            失败<0 :错误码
	*/
	CLOUD_API int API_AttachedDownloadStart(unsigned int loginID, const char* CameraID,
		                                    unsigned int BeginTime, unsigned int EndTime, int Downloadoffset);


	/** @fn CLOUD_API int API_AttachedDownloadStop(unsigned int loginID,int AttachedDownloadHandle)
	*  @brief 停止下载 附属信息(只能停止当前用户的下载)
	*  @param loginID[in]                成功登陆云存储返回的句柄
	*  @param AttachedDownloadHandle[in] 成功开启下载附属信息返回的句柄
	*  @@return 成功=0
	失败<0 :错误码
	*/
	CLOUD_API int API_AttachedDownloadStop(unsigned int loginID,int AttachedDownloadHandle);


	/** @fn CLOUD_API int API_AttachedDel(unsigned int loginID,const char* CameraID,unsigned int BeginTime,unsigned int EndTime)
	*  @brief 删除附属信息
	*  @param loginID[in]                成功登陆云存储返回的句柄
	*  @param CameraID[in]               成功开启下载附属信息返回的句柄
	*  @param BeginTime[in]              开始时间
	*  @param EndTime[in]                结束时间
	*  @@return 成功=0
	            失败<0 :错误码
	*/
	CLOUD_API int API_AttachedDel(unsigned int loginID,const char* CameraID, unsigned int BeginTime, unsigned int EndTime);


};//extern "C"

#endif //__CLOUD_API_H__
