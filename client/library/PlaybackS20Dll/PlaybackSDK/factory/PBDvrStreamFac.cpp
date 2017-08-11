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
 *    @brief   初始化PBLayer.
 *    @param   emDeviceType:设备类型.
 *    @param   pUserData:用户自定义数据.
 *    @return  TRUE:成功,FALSE:失败.
 */
BOOL CPBDvrStreamFac::InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData)
{
	INITIALIZE_PBSDK(CPBDvrStream)
	return CPBDvrStream::InitDvrSdk();
}

/**   @fn      void ReleaseSdk()
 *    @brief   释放SDK.
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
 *    @brief   创建回放控制对象.
 *    @param   
 *    @param   
 *    @return  
 */
CPlaybackBase* CPBDvrStreamFac::CreatePBBase()
{
	return new CPBDvrStream;
}