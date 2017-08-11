//lint -library
#include "StdAfx.h"
#include "PBObjectCloudStreamFac.h"

IMPLEMENT_CLASS_CREATE(PB_TYPE_OBJECTCLOUD + PLAYTYPE_FLAG, CPBObjectCloudStreamFac)
DELCARE_INITSDK_CONTROL(CPBObjectCloudStream)

CPBObjectCloudStreamFac::CPBObjectCloudStreamFac(void)
{
}

CPBObjectCloudStreamFac::~CPBObjectCloudStreamFac(void)
{
}

/**   @fn      BOOL InitSdk()
 *    @brief   ��ʼ��PBLayer.
 *    @param   emDeviceType:�豸����.
 *    @param   pUserData:�û��Զ�������.
 *    @return  TRUE:�ɹ�,FALSE:ʧ��.
 */
BOOL CPBObjectCloudStreamFac::InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData)
{
	INITIALIZE_PBSDK(CPBObjectCloudStream);
	return CPBObjectCloudStream::InitObjectCloudSdk();
}

/**   @fn      void ReleaseSdk()
 *    @brief   �ͷ�SDK.
 *    @param   
 *    @param   
 *    @return  
 */
void CPBObjectCloudStreamFac::ReleaseSdk()
{
	RELEASE_PBSDK(CPBObjectCloudStream);
	CPBObjectCloudStream::ReleaseObjectCloudSdk();
}

/**   @fn      CPlaybackBase* CAPICloudStreamFac::CreatePBBase()
 *    @brief   �����طſ��ƶ���.
 *    @param   
 *    @param   
 *    @return  
 */
CPlaybackBase* CPBObjectCloudStreamFac::CreatePBBase()
{
	return new CPBObjectCloudStream;
}