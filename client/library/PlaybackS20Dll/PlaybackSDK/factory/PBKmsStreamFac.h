/**   @file CvrPlayFac.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief CKmsPlay工厂类.
 *
 *    @author   caofuchun
 *    @date     2015/08/21
 *
 *    @note 用于动态创建CKmsPlay以及初始化
 *    @note 
 *    @note 历史记录：
 *    @note V1.0.0  
 *
 *    @warning 不能漏了DECLARE_CLASS_CREATE和IMPLEMENT_CLASS_CREATE宏.
 */
//lint -library
#pragma once
#include "IFactory.h"
#include "../PBKmsStream.h"

class CPBKmsStreamFac : public CIFactory
{
	DECLARE_CLASS_CREATE(CPBKmsStreamFac)

public:
	CPBKmsStreamFac(void);
	virtual ~CPBKmsStreamFac(void);

public:
	// 初始化SDK.
	virtual BOOL InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData);
	// 释放SDK.
	virtual void ReleaseSdk();
	// 创建回放控制对象.
	virtual CPlaybackBase* CreatePBBase();
};
