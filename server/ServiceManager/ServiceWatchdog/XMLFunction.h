/**
*   @file   XMLFunction.h
*   @note   HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief 
*  
*   @author mazhentong
*   @date   2012/10/24
*
*   @note
*
*   @warning
*/

#ifndef _XML_FUNCTION_H_
#define _XML_FUNCTION_H_

#include "GenDef.h"
#include "fclib/fcDBCommon.h"

// 构建服务器系统信息的XML报文
BOOL XML_GetServerSysInfo(CString &ServerSysInfo, const SERVER_SYS_INFO& struSSysInfo);

// 构建服务进程信息的XML报文
BOOL XML_GetServerProcInfo(CString &ServerProcInfo, const std::vector<SERVER_PROCESS_INFO>& struSProcInfo);

// 解析报文，获取接收theService消息的socket的bind的端口号
BOOL XML_GetRecvSocketPort(int &nRecvSocketPort);

// 解析报文，获取LOGO NAME
CString XML_GetLogoName(void);

// 解析报文，获取皮肤类型
int XML_GetSkinType(void);

//获取参数复制参数
CString XML_GetCopyParam();

BOOL XML_GetCollectorDBInfo(db_conn_info_t &collectorDBInfo);

BOOL XML_GetCenterProcessorInfo(alarmer_info_t &stAlarmerInfo, center_info_t &stLocalInfo);

BOOL XML_CheckCfgPara(void);

#endif