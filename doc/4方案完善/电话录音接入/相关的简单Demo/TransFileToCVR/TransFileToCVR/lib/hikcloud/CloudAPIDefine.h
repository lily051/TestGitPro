/** @file   CloudApiDefine.h
*  @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*  @brief  API Interface H File
*  @author weiwenjuan@hikvision.com.cn
*  @date   2013-01-17
*
*  @change log
*
*/
//lint -library
#ifndef __CLOUD_API_DEFINE_H__
#define __CLOUD_API_DEFINE_H__

#include "CloudAPIPublic.h"

//#define CAMERAID_LEN         72    // 定义编码器字符长度
//#define IPLength             20    // 定义IP地址信息长度
//#define DeviceIDLength       48    // 定义设备ID长度


//typedef unsigned char       BYTE;
//typedef unsigned long       DWORD;

extern "C"
{
	/**  @enum  Record_Data_Type [CloudApiDefine.h]
	*  @brief 录像数据类型
	*
	*/
	enum RecordDataType
	{
		Record_Normal = 1,                         // 普通录像
		Record_Patch = 2,                          // 补录
	};


	/**  @enum Record_Attached_Type [CloudApiDefine.h]
	*  @brief 录像附属信息类型
	*
	*/
	enum RecordAttachedType
	{
		RecordNULL = 0 ,                          // 无附加操作
		RecordBack = 1,		                      // 备份
		RecordRedundancy = 3,                     // 冗余
	};


	/**  @enum FrameType[CloudApiDefine.h]
	*  @brief 录像帧类型
	*
	*/
	enum FrameType
	{
		FrameN = 1,                               // 普通帧
		FrameI = 3,                               // 关键帧
	};

	/** @struct tagCloudAttachedInfo [CloudApiDefine.h]
	* @brief 附属信息数据结构定义
	*/
	typedef struct  tagCloudAttachedInfo
	{
		tagCloudAttachedInfo()
		{
			memset(Camera,0,sizeof(Camera) );
			BeginTime=0;
			EndTime=0;
			Type=0;
			Lock=0;
			Size=0;
		}

		tagCloudAttachedInfo(tagCloudAttachedInfo&tag)
		{
			memcpy(Camera,tag.Camera,sizeof(tag.Camera));
			BeginTime=tag.BeginTime;
			EndTime=tag.EndTime;
			Type=tag.Type;
			Lock=tag.Lock;
			Size=tag.Size;
		}

		tagCloudAttachedInfo& operator=(tagCloudAttachedInfo& tag)
		{
			memcpy(Camera,tag.Camera,sizeof(tag.Camera));
			BeginTime=tag.BeginTime;
			EndTime=tag.EndTime;
			Type=tag.Type;
			Lock=tag.Lock;
			Size=tag.Size;
			return *this;
		}

		char  Camera[CAMERAID_LEN];	           // 编码器ID
		unsigned int  BeginTime ;	           // 附属信息开始时间
		unsigned int  EndTime;	               // 附属信息结束时间
		int   Type;	                           // 附属类型
		int	  Lock;	                           // 附属信息锁定状态 [0: 全部类型(锁定或未锁定)、1: 未锁定、2: 锁定]
		int	  Size;                            // 附属大小[单位: 字节]
	}CloudAttachedInfo;

	/** 
	*  @brief 附属信息查询结果回调函数定义
	*  @param info[out] 附属信息 
	*  @param Count[out] 查询附属信息结果总数量
	*  @param Flat[out]  剩余附属信息总数量 [等于0: 表示附属信息全部返回、 小于0: 为错误码、大于0: 表示剩余的附属信息总数]
	*  @param pUser[out] 用户自定义数据 
	*  @@return 等于0:数据已处理、小于0:数据未处理
	*/
	typedef int (*CAttachedSearchDataCb)(CloudAttachedInfo info, int Count, int Flat, void* pUser);

	/** 
	*  @brief 录像下载数据回调函数定义
	*  @param buff[out] 录像数据
	*  @param bufflen[out] 录像数据长度 
	*  @param Flat[out]  下载完成标识 [小于等于0: 下载结束、 小于0: 为错误码]
	*  @param pUser[out] 用户自定义数据 
	*  @@return 等于0:数据已处理、小于0:数据未处理
	
	typedef int (*CDownloadDataCb)(const char* buff, int bufflen, int Flat, void* pUser);
*/

	/** 
	*  @brief 图片下载回调函数定义
	*  @param buff[out] 图片数据
	*  @param bufflen[out] 图片数据长度
	*  @param pUser[out] 用户自定义数据 
	*  @@return 等于0:数据已处理、小于0:数据未处理
	
	typedef int (*CPictureDownLoadCB)(const char* buff, int bufflen, void* pUser);
*/

	/** 
	*  @brief 批量图片下载回调函数定义
	*  @param buff[out] 图片数据
	*  @param bufflen[out] 图片数据长度
	*  @param Type[out] 图片类型 [1:JPG、2:BMP、3:PNG] 
	*  @param Count[out] 查询到的图片总数量
	*  @param Flat[out]  剩余图片总数量(可用于判断停止下载) [等于0: 表示下载完成、 小于0: 为错误码、大于0: 表示需要下载图片的总数]
	*  @param pUser[out] 用户自定义数据 
	*  @@return 等于0:数据已处理、小于0:数据未处理
	*/
	typedef int (*CPictureDownLoadCBEx)(const char* buff, int bufflen, int Type, int Count, int Flat, void* pUser);


    /** 
	*  @brief 事件回调函数定义.包含网络错误、程序异常和消息上报
	*  @param LogicType[out] API的模块序号 [0:验证、1:录像(包含标注)、2:图片、3:附属信息、4:录像池管理]
	*  @param ErrorCode[out] 错误码
	*  @param pUser[out] 用户自定义数据 
	*  @@return 等于0:数据已处理、小于0:数据未处理
	
	typedef int (*CUserMessage)(int LogicType, int ErrorCode, void* pUser);
*/
}//extern "C"

#endif  //__CLOUD_API_DEFINE_H__
