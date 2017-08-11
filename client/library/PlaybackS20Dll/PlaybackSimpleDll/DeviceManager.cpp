/**   @file DeviceManager.cpp
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	设备类型管理.
 *
 *    @author   zhoupeiqiang
 *    @date     2012/12/11
 *
 *    @note 
 *    @note 
 *    @note 历史记录：
 *    @note V1.0.0  
 *
 *    @warning 
 */


#include "StdAfx.h"
#include "DeviceManager.h"

CDeviceManager::CDeviceManager(void)
{
}

CDeviceManager::~CDeviceManager(void)
{
}

/**   @fn          GetDeviceType
 *    @brief   	   设备类型
 *    @param[in]   nManType:厂商类型.
 *    @param[in]   nDevType:设备类型
 *    @return      转换后的设备类型.
 */
int CDeviceManager::GetDeviceType(int nManType, int nDevType)
{
	int nType = PB_DEVICE_HIKVISION;
	return nType;
}

/**   @fn          GetPlayType
 *    @brief   	   获取播放类型
 *    @param[in]   nType:原始设备类型.
 *    @param[in]   
 *    @return      转换后的设备类型.
 */
int CDeviceManager::GetPlayType(int nType)
{
	return PT_HIK;
}

/**   @fn          GetDeviceName
 *    @brief   	   获取设备类型名称.
 *    @param[in]   strName:原名称.
 *    @param[in]   nType:设备类型.
 *    @return      转换后的类型名称.
 */
CString CDeviceManager::GetDeviceName(int nType, CString strName)
{
	return _T("海康");
}

/**   @fn          GetPlayTypeFromFileName
 *    @brief   	   根据文件名获取播放类型
 *    @param[in]   strName:文件名.
 *    @param[in]   
 *    @return      播放类型.
 */
int CDeviceManager::GetPlayTypeFromFileName(const CString& strName)
{
	return PT_HIK;
}

/**   @fn          GetFileNameFromPBType
 *    @brief   	   根据回放类型获取文件名.
 *    @param[in]   nType:回放类型.
 *    @param[in]   
 *    @return      文件名后缀.
 */
CString CDeviceManager::GetFileNameFromPBType(int nType, int nPlayType, BOOL bUseVag)
{
	switch (nType)
	{
	case PB_DEVICE_HIKVISION:
		return _T("HIK");
	default:
		return _T("");
	}

}

/**   @fn          GetFileNameFromPBType
 *    @brief   	   根据回放类型获取文件名.
 *    @param[in]   nType:回放类型.
 *    @param[in]   
 *    @return      文件名后缀.
 */
CString CDeviceManager::GetFileNameFromDevType(int nType)
{
	return _T("HIK");
}

/**   @fn          GetPBTypeNameFromPBType
 *    @brief   	   根据回放类型获取回放类型名称.
 *    @param[in]   nType:回放类型.
 *    @param[in]   
 *    @return      
 */
CString CDeviceManager::GetPBTypeNameFromPBType(int nType)
{
    switch (nType)
    {
    case PB_TYPE_DVR:
        return _T("dvr");

    case PB_TYPE_CVR:
        return _T("cvr");

    case PB_TYPE_CLOUD:
        return _T("cloud");

    case PB_TYPE_KMS:
        return _T("kms");

	case PB_TYPE_OBJECTCLOUD:
		return _T("objectcloud");

    default:
        return _T("dvr");
    }
}