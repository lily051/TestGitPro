/**   @file APICloudStream.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief APICloudStream工厂类.
 *
 *    @author   wangzhiyong
 *    @date     2013/12/06
 *
 *    @note 用于动态创建CAPICloudStream以及初始化
 *    @note 
 *    @note 历史记录：
 *    @note V1.0.0  
 *
 *    @warning 不能漏了DECLARE_CLASS_CREATE和IMPLEMENT_CLASS_CREATE宏.
 */

#pragma once
//lint -library
#include "IFactory.h"
#include "../PBCloudStream.h"

class CPBCloudStreamFac : public CIFactory
{
	DECLARE_CLASS_CREATE(CPBCloudStreamFac)

public:
	CPBCloudStreamFac(void);
	~CPBCloudStreamFac(void);

public:
	// 初始化SDK.
	virtual BOOL InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData);
	// 释放SDK.
	virtual void ReleaseSdk();
	// 创建回放控制对象.
	virtual CPlaybackBase* CreatePBBase();
};
