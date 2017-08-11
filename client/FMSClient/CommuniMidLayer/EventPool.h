#pragma once
/**	@file    CEventPool.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   �¼���
*
*	@author	 yudan
*	@date	 2011-9-15 10:05
*
*	@note   
*	@note    ��ʷ��¼��
*	@note    V1.0  �����ļ� at 2011-9-15 10:05 by yudan
*
*	@warning 
*/
#include<list>

//�¼���Ϣ�ṹ��
typedef struct _tagEventInfo
{
	HANDLE hEvent;                           //�¼����
	LONG lSetTime;                            //�¼��Լ���ʱ��
	BOOL bStatus;                             //�¼�״̬
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
	int m_nEventNum;  //�¼������¼�����
public:
	int m_nEventTime;  //ÿ���¼�������󼤻��¼�
	std::list<LPEVENT_INFO> m_listFreeEvent;  //ָ������¼���ָ������
	std::list<LPEVENT_INFO> m_listUsedEvent;  //ָ��ʹ�����¼���ָ������
	CRITICAL_SECTION m_lckEvent;  //�¼�������
	HANDLE m_hThread;  //�߳̾��
	BOOL m_bWorking;  //�̹߳�����ʶ

public:
	/*�����¼������¼�����*/
	void SetEventNum(int nEventNum);
	/*����ÿ���¼�������󼤻��¼�*/
	void SetEventTime(int nEventTime);
	/*��ʼ�������¼�����*/
	BOOL InitAllEvent();
	/*�ͷ������¼�����*/
	void ReleaseAllEvent();
	/*��ȡ����������¼�����*/
	HANDLE GetFreeEvent();
	/*�ͷŵ����¼�*/
	BOOL SetSingleEvent(HANDLE hEventHandle);

	/*�̺߳��������ʹ���е��¼�ʱ���Ƿ񳬹���������ʱ��*/
	static void OnEventTimeCheck(LPVOID lpvoid);
	
};
