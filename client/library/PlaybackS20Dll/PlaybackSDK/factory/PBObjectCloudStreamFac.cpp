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
 *    @brief   初始化PBLayer.
 *    @param   emDeviceType:设备类型.
 *    @param   pUserData:用户自定义数据.
 *    @return  TRUE:成功,FALSE:失败.
 */
BOOL CPBObjectCloudStreamFac::InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData)
{
	INITIALIZE_PBSDK(CPBObjectCloudStream);
	return CPBObjectCloudStream::InitObjectCloudSdk();
}

/**   @fn      void ReleaseSdk()
 *    @brief   释放SDK.
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
 *    @brief   创建回放控制对象.
 *    @param   
 *    @param   
 *    @return  
 */
CPlaybackBase* CPBObjectCloudStreamFac::CreatePBBase()
{
	return new CPBObjectCloudStream;
}