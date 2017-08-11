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

// ����������ϵͳ��Ϣ��XML����
BOOL XML_GetServerSysInfo(CString &ServerSysInfo, const SERVER_SYS_INFO& struSSysInfo);

// �������������Ϣ��XML����
BOOL XML_GetServerProcInfo(CString &ServerProcInfo, const std::vector<SERVER_PROCESS_INFO>& struSProcInfo);

// �������ģ���ȡ����theService��Ϣ��socket��bind�Ķ˿ں�
BOOL XML_GetRecvSocketPort(int &nRecvSocketPort);

// �������ģ���ȡLOGO NAME
CString XML_GetLogoName(void);

// �������ģ���ȡƤ������
int XML_GetSkinType(void);

//��ȡ�������Ʋ���
CString XML_GetCopyParam();

BOOL XML_GetCollectorDBInfo(db_conn_info_t &collectorDBInfo);

BOOL XML_GetCenterProcessorInfo(alarmer_info_t &stAlarmerInfo, center_info_t &stLocalInfo);

BOOL XML_CheckCfgPara(void);

#endif