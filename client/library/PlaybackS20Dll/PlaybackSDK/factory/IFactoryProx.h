/**   @file IFactoryProx
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	CIFactory�Ĵ�����.
 *
 *    @author   zhoupeiqiang      
 *    @date     2012/10/27       
 *
 *    @note		�����������豸���ܹ���һ��ȡ��������������.
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
	// ��������.
	static CIFactory* CreateObject(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void * pUserData);

private:
	// ��ȡ�豸����.
	static int GetDeviceType(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void * pUserData);
};
