//lint -library
#include "StdAfx.h"
#include "PBCvrStreamFac.h"

// CVR�ط�����������,һ������DVR SDK��������ֵͬPB_DEVICE_HIKVISION,
IMPLEMENT_CLASS_CREATE(PB_TYPE_CVR + PLAYTYPE_FLAG, CPBCvrStreamFac)
DELCARE_INITSDK_CONTROL(CPBCvrStream)

CPBCvrStreamFac::CPBCvrStreamFac(void)
{
}

CPBCvrStreamFac::~CPBCvrStreamFac(void)
{
}

/**   @fn      BOOL InitSdk()
 *    @brief   ��ʼ��PBLayer.
 *    @param   emDeviceType:�豸����.
 *    @param   pUserData:�û��Զ�������.
 *    @return  TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPBCvrStreamFac::InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData)
{
	INITIALIZE_PBSDK(CPBCvrStream)
	return CPBCvrStream::InitCvrSdk();
}

/**   @fn      void ReleaseSdk()
 *    @brief   �ͷ�SDK.
 *    @param   
 *    @param   
 *    @return  
 */
void CPBCvrStreamFac::ReleaseSdk()
{
	RELEASE_PBSDK(CPBCvrStream)
	CPBCvrStream::ReleaseHikSdk();
}

/**   @fn      CPlaybackBase* CBdpStreamFac::CreatePBBase()
 *    @brief   �����طſ��ƶ���.
 *    @param   
 *    @param   
 *    @return  
 */
CPlaybackBase* CPBCvrStreamFac::CreatePBBase()
{
	return new CPBCvrStream;
}