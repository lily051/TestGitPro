/**   @file CvrPlayFac.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief CKmsPlay������.
 *
 *    @author   caofuchun
 *    @date     2015/08/21
 *
 *    @note ���ڶ�̬����CKmsPlay�Լ���ʼ��
 *    @note 
 *    @note ��ʷ��¼��
 *    @note V1.0.0  
 *
 *    @warning ����©��DECLARE_CLASS_CREATE��IMPLEMENT_CLASS_CREATE��.
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
	// ��ʼ��SDK.
	virtual BOOL InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData);
	// �ͷ�SDK.
	virtual void ReleaseSdk();
	// �����طſ��ƶ���.
	virtual CPlaybackBase* CreatePBBase();
};
