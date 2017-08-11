#ifndef NETINFOAPP_H_
#define NETINFOAPP_H_
#pragma once
/**	@file    NetInfoApp.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   ͨ���м����Ϣ����
*
*	@author	 yudan
*	@date	 2012/05/07
*	@note   
*	@note    ��ʷ��¼��
*	@note    V1.0  create at 2012/05/07 by yudan
*/

typedef struct _tagSYNCRSP_CONTEXT  //ͬ���ȴ���Ϣ������
{
	unsigned int nSeq;     //��Ϣ���к�
	int nConnSessionID;    //HPP���ӻỰID
	HANDLE hEvent;         //�ȴ��¼�
	unsigned int nCmdRsp;  //��Ϣ������
	BOOL bGet;             //�Ƿ��ȡ����Ϣ
    string* psMsg;         //������Ϣ����
	_tagSYNCRSP_CONTEXT()
	{
		nSeq = 0;
		nConnSessionID = -1;
		hEvent = NULL;
		nCmdRsp = 0;
		bGet = FALSE;
        psMsg = NULL;
	}
}SYNCRSP_CONTEXT;

typedef struct _tagASYNRSP_CONTEXT   //�첽��Ϣ������
{
	int nSeq;          //��Ϣ���к�
	int nCmdID;        //��Ϣ����
	int nComponentID;  //��ϢԴ���ID
	LONG lWaitTime;    //�ȴ���ʼʱ��
	_tagASYNRSP_CONTEXT()
	{
		nSeq = 0;
		nCmdID = -1;
		nComponentID = 0;
		lWaitTime = (LONG)CTime::GetCurrentTime().GetTime();
	}
}ASYNRSP_CONTEXT;

#endif