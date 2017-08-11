//lint -library
#include "StdAfx.h"
#include "PBKmsStreamFac.h"
IMPLEMENT_CLASS_CREATE(PB_TYPE_KMS + PLAYTYPE_FLAG, CPBKmsStreamFac)
DELCARE_INITSDK_CONTROL(CPBKmsStream)
CPBKmsStreamFac::CPBKmsStreamFac(void)
{
}

CPBKmsStreamFac::~CPBKmsStreamFac(void)
{
}
/**   @fn      BOOL InitSdk()
 *    @brief   ��ʼ��PBLayer.
 *    @param   emDeviceType:�豸����.
 *    @param   pUserData:�û��Զ�������.
 *    @return  TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPBKmsStreamFac::InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData)
{
	INITIALIZE_PBSDK(CPBKmsStream)
	return CPBKmsStream::InitKmsSdk();
}

/**   @fn      void ReleaseSdk()
 *    @brief   �ͷ�SDK.
 *    @param   
 *    @param   
 *    @return  
 */
void CPBKmsStreamFac::ReleaseSdk()
{
	RELEASE_PBSDK(CPBKmsStream)
	CPBKmsStream::ReleaseHikSdk();
}

/**   @fn      CPlaybackBase* CKmsPlayFac::CreatePBBase()
 *    @brief   �����طſ��ƶ���.
 *    @param   
 *    @param   
 *    @return  
 */
CPlaybackBase* CPBKmsStreamFac::CreatePBBase()
{
	return new CPBKmsStream;
}