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
 *    @brief   ��������.
 *    @param   strClassName:����.
 *    @param   
 *    @return  ����ָ��.
 */
CIFactory* CIFactoryProx::CreateObject(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void * pUserData)
{
	// �豸����ת��.
	int nDeviceType = GetDeviceType(emDeviceType, emPlayType, pUserData);	

	// ��������.
	return CIFactory::CreateObject(nDeviceType);
}

/**   @fn      CString CIFactoryProx::GetStrFromDeviceType(ENUM_DEVICE_TYPE emDeviceType)
 *    @brief   Ԥ���ӿڹ����豸����.
 *    @param   emDeviceType:�豸����.
 *    @param   emPlayType:�ط�����.
 *	  @param   pUserData:�û�.
 *    @return  �豸����.
 */
int CIFactoryProx::GetDeviceType(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void * pUserData)
{
	int nDeviceType = -1;

	// DVR,CVR�����豸��������������.
	if (PB_TYPE_DVR == emPlayType)
	{
		nDeviceType = emDeviceType;
	}
	else
	{
		// �ط����͵ı�־��ΪҲ��0��ʼ����Ҫ����DVR.�ط�.
		nDeviceType = emPlayType + PLAYTYPE_FLAG;
	}

	return nDeviceType;
}