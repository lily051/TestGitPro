//lint -library
#include "StdAfx.h"
#include "PBDvrStreamFac.h"

IMPLEMENT_CLASS_CREATE(PB_TYPE_DVR, CPBDvrStreamFac)
DELCARE_INITSDK_CONTROL(CPBDvrStream)

CPBDvrStreamFac::CPBDvrStreamFac(void)
{
}

CPBDvrStreamFac::~CPBDvrStreamFac(void)
{
}

/**   @fn      BOOL InitSdk()
 *    @brief   ��ʼ��PBLayer.
 *    @param   emDeviceType:�豸����.
 *    @param   pUserData:�û��Զ�������.
 *    @return  TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPBDvrStreamFac::InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData)
{
	INITIALIZE_PBSDK(CPBDvrStream)
	return CPBDvrStream::InitDvrSdk();
}

/**   @fn      void ReleaseSdk()
 *    @brief   �ͷ�SDK.
 *    @param   
 *    @param   
 *    @return  
 */
void CPBDvrStreamFac::ReleaseSdk()
{
	RELEASE_PBSDK(CPBDvrStream)
	CPBDvrStream::ReleaseHikSdk();
}

/**   @fn      CPlaybackBase* CBdpStreamFac::CreatePBBase()
 *    @brief   �����طſ��ƶ���.
 *    @param   
 *    @param   
 *    @return  
 */
CPlaybackBase* CPBDvrStreamFac::CreatePBBase()
{
	return new CPBDvrStream;
}