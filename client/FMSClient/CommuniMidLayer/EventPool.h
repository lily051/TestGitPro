#pragma once
/**	@file    CEventPool.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   事件池
*
*	@author	 yudan
*	@date	 2011-9-15 10:05
*
*	@note   
*	@note    历史记录：
*	@note    V1.0  创建文件 at 2011-9-15 10:05 by yudan
*
*	@warning 
*/
#include<list>

//事件信息结构体
typedef struct _tagEventInfo
{
	HANDLE hEvent;                           //事件句柄
	LONG lSetTime;                            //事件以激活时间
	BOOL bStatus;                             //事件状态
	_tagEventInfo()
	{
		hEvent = NULL;
		lSetTime = 0;
		bStatus = FALSE;
	}
}EVENT_INFO, *LPEVENT_INFO;

class CEventPool
{
public:
	CEventPool();
	CEventPool(int nEventNum);
	~CEventPool();

private:
	int m_nEventNum;  //事件池中事件数量
public:
	int m_nEventTime;  //每个事件允许最大激活事件
	std::list<LPEVENT_INFO> m_listFreeEvent;  //指向空闲事件的指针链表
	std::list<LPEVENT_INFO> m_listUsedEvent;  //指向使用中事件的指针链表
	CRITICAL_SECTION m_lckEvent;  //事件操作锁
	HANDLE m_hThread;  //线程句柄
	BOOL m_bWorking;  //线程工作标识

public:
	/*设置事件池中事件数量*/
	void SetEventNum(int nEventNum);
	/*设置每个事件允许最大激活事件*/
	void SetEventTime(int nEventTime);
	/*初始化所有事件对象*/
	BOOL InitAllEvent();
	/*释放所有事件对象*/
	void ReleaseAllEvent();
	/*获取激活单个空闲事件对象*/
	HANDLE GetFreeEvent();
	/*释放单个事件*/
	BOOL SetSingleEvent(HANDLE hEventHandle);

	/*线程函数，检查使用中的事件时间是否超过允许的最大时间*/
	static void OnEventTimeCheck(LPVOID lpvoid);
	
};
