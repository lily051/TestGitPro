/**   @file DeviceManager.cpp
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	�豸���͹���.
 *
 *    @author   zhoupeiqiang
 *    @date     2012/12/11
 *
 *    @note 
 *    @note 
 *    @note ��ʷ��¼��
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
 *    @brief   	   �豸����
 *    @param[in]   nManType:��������.
 *    @param[in]   nDevType:�豸����
 *    @return      ת������豸����.
 */
int CDeviceManager::GetDeviceType(int nManType, int nDevType)
{
	int nType = PB_DEVICE_HIKVISION;
	return nType;
}

/**   @fn          GetPlayType
 *    @brief   	   ��ȡ��������
 *    @param[in]   nType:ԭʼ�豸����.
 *    @param[in]   
 *    @return      ת������豸����.
 */
int CDeviceManager::GetPlayType(int nType)
{
	return PT_HIK;
}

/**   @fn          GetDeviceName
 *    @brief   	   ��ȡ�豸��������.
 *    @param[in]   strName:ԭ����.
 *    @param[in]   nType:�豸����.
 *    @return      ת�������������.
 */
CString CDeviceManager::GetDeviceName(int nType, CString strName)
{
	return _T("����");
}

/**   @fn          GetPlayTypeFromFileName
 *    @brief   	   �����ļ�����ȡ��������
 *    @param[in]   strName:�ļ���.
 *    @param[in]   
 *    @return      ��������.
 */
int CDeviceManager::GetPlayTypeFromFileName(const CString& strName)
{
	return PT_HIK;
}

/**   @fn          GetFileNameFromPBType
 *    @brief   	   ���ݻط����ͻ�ȡ�ļ���.
 *    @param[in]   nType:�ط�����.
 *    @param[in]   
 *    @return      �ļ�����׺.
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
 *    @brief   	   ���ݻط����ͻ�ȡ�ļ���.
 *    @param[in]   nType:�ط�����.
 *    @param[in]   
 *    @return      �ļ�����׺.
 */
CString CDeviceManager::GetFileNameFromDevType(int nType)
{
	return _T("HIK");
}

/**   @fn          GetPBTypeNameFromPBType
 *    @brief   	   ���ݻط����ͻ�ȡ�ط���������.
 *    @param[in]   nType:�ط�����.
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