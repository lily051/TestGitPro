/**   @file BnNVRStreamFac.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief CBnNVRStreamFac������.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/02/19
 *
 *    @note ���ڶ�̬����CBnNVRStreamFac�Լ���ʼ��
 *    @note 
 *    @note ��ʷ��¼��
 *    @note V1.0.0  
 *
 *    @warning ����©��DECLARE_CLASS_CREATE��IMPLEMENT_CLASS_CREATE��.
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
	// ��ʼ��SDK.
	virtual BOOL InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData);
	// �ͷ�SDK.
	virtual void ReleaseSdk();
	// �����طſ��ƶ���.
	virtual CPlaybackBase* CreatePBBase();
};
