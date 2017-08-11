/**   @file DeviceManager.h
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

#pragma once

class CDeviceManager
{
public:
	CDeviceManager(void);
	~CDeviceManager(void);

public:
	// �豸����ת��.
	int GetDeviceType(int nManType, int nDevType);
	// ��ȡ��������.
	int GetPlayType(int nType);
	// ��ȡ�豸����.
	CString GetDeviceName(int nType, CString strName);
	// ���ļ�����ȡ��������.
	int GetPlayTypeFromFileName(const CString& strName);
	// ���ݻط����ͻ�ȡ�ļ���.
	CString GetFileNameFromPBType(int nType, int nPlayType, BOOL bUseVag);
	// �����豸���ͻ�ȡ�ļ���.
	CString GetFileNameFromDevType(int nType);
	// ���ݻط����ͻ�ȡ�ط���������.
	CString GetPBTypeNameFromPBType(int nType);
};
