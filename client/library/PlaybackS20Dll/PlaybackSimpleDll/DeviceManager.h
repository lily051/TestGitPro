/**   @file DeviceManager.h
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

#pragma once

class CDeviceManager
{
public:
	CDeviceManager(void);
	~CDeviceManager(void);

public:
	// 设备类型转换.
	int GetDeviceType(int nManType, int nDevType);
	// 获取播放类型.
	int GetPlayType(int nType);
	// 获取设备名称.
	CString GetDeviceName(int nType, CString strName);
	// 从文件名获取播放类型.
	int GetPlayTypeFromFileName(const CString& strName);
	// 根据回放类型获取文件名.
	CString GetFileNameFromPBType(int nType, int nPlayType, BOOL bUseVag);
	// 根据设备类型获取文件名.
	CString GetFileNameFromDevType(int nType);
	// 根据回放类型获取回放类型名称.
	CString GetPBTypeNameFromPBType(int nType);
};
