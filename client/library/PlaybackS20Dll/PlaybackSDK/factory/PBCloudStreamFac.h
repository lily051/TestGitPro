/**   @file APICloudStream.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief APICloudStream������.
 *
 *    @author   wangzhiyong
 *    @date     2013/12/06
 *
 *    @note ���ڶ�̬����CAPICloudStream�Լ���ʼ��
 *    @note 
 *    @note ��ʷ��¼��
 *    @note V1.0.0  
 *
 *    @warning ����©��DECLARE_CLASS_CREATE��IMPLEMENT_CLASS_CREATE��.
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
	// ��ʼ��SDK.
	virtual BOOL InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData);
	// �ͷ�SDK.
	virtual void ReleaseSdk();
	// �����طſ��ƶ���.
	virtual CPlaybackBase* CreatePBBase();
};
