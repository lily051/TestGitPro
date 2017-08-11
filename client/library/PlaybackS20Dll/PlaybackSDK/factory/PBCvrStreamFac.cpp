//lint -library
#include "StdAfx.h"
#include "PBCvrStreamFac.h"

// CVR回放类型有两种,一种是走DVR SDK，其类型值同PB_DEVICE_HIKVISION,
IMPLEMENT_CLASS_CREATE(PB_TYPE_CVR + PLAYTYPE_FLAG, CPBCvrStreamFac)
DELCARE_INITSDK_CONTROL(CPBCvrStream)

CPBCvrStreamFac::CPBCvrStreamFac(void)
{
}

CPBCvrStreamFac::~CPBCvrStreamFac(void)
{
}

/**   @fn      BOOL InitSdk()
 *    @brief   初始化PBLayer.
 *    @param   emDeviceType:设备类型.
 *    @param   pUserData:用户自定义数据.
 *    @return  TRUE:成功,FALSE:失败.
 */
BOOL CPBCvrStreamFac::InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData)
{
	INITIALIZE_PBSDK(CPBCvrStream)
	return CPBCvrStream::InitCvrSdk();
}

/**   @fn      void ReleaseSdk()
 *    @brief   释放SDK.
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
 *    @brief   创建回放控制对象.
 *    @param   
 *    @param   
 *    @return  
 */
CPlaybackBase* CPBCvrStreamFac::CreatePBBase()
{
	return new CPBCvrStream;
}