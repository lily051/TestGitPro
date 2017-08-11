//lint -library
#include "StdAfx.h"
#include "PBCloudStreamFac.h"

IMPLEMENT_CLASS_CREATE(PB_TYPE_CLOUD + PLAYTYPE_FLAG, CPBCloudStreamFac)
DELCARE_INITSDK_CONTROL(CPBCloudStream)

CPBCloudStreamFac::CPBCloudStreamFac(void)
{
}

CPBCloudStreamFac::~CPBCloudStreamFac(void)
{
}

/**   @fn      BOOL InitSdk()
 *    @brief   ��ʼ��PBLayer.
 *    @param   emDeviceType:�豸����.
 *    @param   pUserData:�û��Զ�������.
 *    @return  TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPBCloudStreamFac::InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData)
{
	INITIALIZE_PBSDK(CPBCloudStream);
	return CPBCloudStream::InitCloudSdk();
}

/**   @fn      void ReleaseSdk()
 *    @brief   �ͷ�SDK.
 *    @param   
 *    @param   
 *    @return  
 */
void CPBCloudStreamFac::ReleaseSdk()
{
	RELEASE_PBSDK(CPBCloudStream);
	CPBCloudStream::ReleaseSdk();
}

/**   @fn      CPlaybackBase* CAPICloudStreamFac::CreatePBBase()
 *    @brief   �����طſ��ƶ���.
 *    @param   
 *    @param   
 *    @return  
 */
CPlaybackBase* CPBCloudStreamFac::CreatePBBase()
{
	return new CPBCloudStream;
}