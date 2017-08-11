/**   @file IFactoryProx
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	CIFactory的代理类.
 *
 *    @author   zhoupeiqiang      
 *    @date     2012/10/27       
 *
 *    @note		解决多个类型设备可能共用一个取流管理对象的问题.
 *    @note 
 *    @note		
 *    @note		V1.0.0
 *
 *    @warning 
 */

#pragma once

#include "IFactory.h"

class CIFactoryProx
{
public:
	CIFactoryProx();
	~CIFactoryProx(void);

public:
	// 创建对象.
	static CIFactory* CreateObject(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void * pUserData);

private:
	// 获取设备类型.
	static int GetDeviceType(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void * pUserData);
};
