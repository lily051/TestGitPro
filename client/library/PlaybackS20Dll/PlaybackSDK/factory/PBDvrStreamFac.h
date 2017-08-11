/**   @file HikStreamFac.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief CHikStream������.
 *
 *    @author   zhoupeiqiang
 *    @date     2012/11/14
 *
 *    @note ���ڶ�̬����CHikStream�Լ���ʼ��
 *    @note 
 *    @note ��ʷ��¼��
 *    @note V1.0.0  
 *
 *    @warning ����©��DECLARE_CLASS_CREATE��IMPLEMENT_CLASS_CREATE��.
 */
//lint -library
#pragma once

#include "IFactory.h"
#include "../PBDvrStream.h"

class CPBDvrStreamFac : public CIFactory
{
	DECLARE_CLASS_CREATE(CPBDvrStreamFac)

public:
	CPBDvrStreamFac(void);
	~CPBDvrStreamFac(void);

public:
	// ��ʼ��SDK.
	virtual BOOL InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData);
	// �ͷ�SDK.
	virtual void ReleaseSdk();
	// �����طſ��ƶ���.
	virtual CPlaybackBase* CreatePBBase();
};
