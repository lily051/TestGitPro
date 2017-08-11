#ifndef NETINFOAPP_H_
#define NETINFOAPP_H_
#pragma once
/**	@file    NetInfoApp.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   通信中间层信息定义
*
*	@author	 yudan
*	@date	 2012/05/07
*	@note   
*	@note    历史记录：
*	@note    V1.0  create at 2012/05/07 by yudan
*/

typedef struct _tagSYNCRSP_CONTEXT  //同步等待消息上下文
{
	unsigned int nSeq;     //消息序列号
	int nConnSessionID;    //HPP连接会话ID
	HANDLE hEvent;         //等待事件
	unsigned int nCmdRsp;  //消息命令码
	BOOL bGet;             //是否获取到消息
    string* psMsg;         //返回消息数据
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

typedef struct _tagASYNRSP_CONTEXT   //异步消息上下文
{
	int nSeq;          //消息序列号
	int nCmdID;        //消息类型
	int nComponentID;  //消息源组件ID
	LONG lWaitTime;    //等待初始时间
	_tagASYNRSP_CONTEXT()
	{
		nSeq = 0;
		nCmdID = -1;
		nComponentID = 0;
		lWaitTime = (LONG)CTime::GetCurrentTime().GetTime();
	}
}ASYNRSP_CONTEXT;

#endif