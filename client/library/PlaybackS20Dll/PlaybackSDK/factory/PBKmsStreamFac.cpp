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
 *    @brief   初始化PBLayer.
 *    @param   emDeviceType:设备类型.
 *    @param   pUserData:用户自定义数据.
 *    @return  TRUE:成功,FALSE:失败.
 */
BOOL CPBKmsStreamFac::InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData)
{
	INITIALIZE_PBSDK(CPBKmsStream)
	return CPBKmsStream::InitKmsSdk();
}

/**   @fn      void ReleaseSdk()
 *    @brief   释放SDK.
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
 *    @brief   创建回放控制对象.
 *    @param   
 *    @param   
 *    @return  
 */
CPlaybackBase* CPBKmsStreamFac::CreatePBBase()
{
	return new CPBKmsStream;
}