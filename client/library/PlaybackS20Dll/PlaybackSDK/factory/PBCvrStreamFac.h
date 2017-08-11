/**   @file BnNVRStreamFac.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief CBnNVRStreamFac工厂类.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/02/19
 *
 *    @note 用于动态创建CBnNVRStreamFac以及初始化
 *    @note 
 *    @note 历史记录：
 *    @note V1.0.0  
 *
 *    @warning 不能漏了DECLARE_CLASS_CREATE和IMPLEMENT_CLASS_CREATE宏.
 */
//lint -library
#pragma once

#include "IFactory.h"
#include "../PBCvrStream.h"

class CPBCvrStreamFac : public CIFactory
{
	DECLARE_CLASS_CREATE(CPBCvrStreamFac)

public:
	CPBCvrStreamFac(void);
	~CPBCvrStreamFac(void);

public:
	// 初始化SDK.
	virtual BOOL InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData);
	// 释放SDK.
	virtual void ReleaseSdk();
	// 创建回放控制对象.
	virtual CPlaybackBase* CreatePBBase();
};
