#include "StdAfx.h"
#include "IFactoryProx.h"
#include "../PlaybackSdkDef.h"
using namespace PlaybackSdk;

CIFactoryProx::CIFactoryProx()
{

}

CIFactoryProx::~CIFactoryProx(void)
{

}

/**   @fn      CIFactory* CIFactoryProx::CreateObject(string strClassName)
 *    @brief   创建对象.
 *    @param   strClassName:类名.
 *    @param   
 *    @return  对象指针.
 */
CIFactory* CIFactoryProx::CreateObject(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void * pUserData)
{
	// 设备类型转换.
	int nDeviceType = GetDeviceType(emDeviceType, emPlayType, pUserData);	

	// 创建对象.
	return CIFactory::CreateObject(nDeviceType);
}

/**   @fn      CString CIFactoryProx::GetStrFromDeviceType(ENUM_DEVICE_TYPE emDeviceType)
 *    @brief   预留接口管理设备类型.
 *    @param   emDeviceType:设备类型.
 *    @param   emPlayType:回放类型.
 *	  @param   pUserData:用户.
 *    @return  设备类型.
 */
int CIFactoryProx::GetDeviceType(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void * pUserData)
{
	int nDeviceType = -1;

	// DVR,CVR根据设备类型来创建对象.
	if (PB_TYPE_DVR == emPlayType)
	{
		nDeviceType = emDeviceType;
	}
	else
	{
		// 回放类型的标志因为也从0开始，需要区分DVR.回放.
		nDeviceType = emPlayType + PLAYTYPE_FLAG;
	}

	return nDeviceType;
}