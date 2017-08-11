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
 *    @brief   初始化PBLayer.
 *    @param   emDeviceType:设备类型.
 *    @param   pUserData:用户自定义数据.
 *    @return  TRUE:成功,FALSE:失败.
 */
BOOL CPBCloudStreamFac::InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData)
{
	INITIALIZE_PBSDK(CPBCloudStream);
	return CPBCloudStream::InitCloudSdk();
}

/**   @fn      void ReleaseSdk()
 *    @brief   释放SDK.
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
 *    @brief   创建回放控制对象.
 *    @param   
 *    @param   
 *    @return  
 */
CPlaybackBase* CPBCloudStreamFac::CreatePBBase()
{
	return new CPBCloudStream;
}