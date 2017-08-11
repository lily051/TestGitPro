/**   @file BwControl.h
 *    @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	���������.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/03/05
 *
 *    @note 
 *    @note 
 *    @note ��ʷ��¼��
 *    @note V1.0.0  
 *
 *    @warning 
 */

#pragma once

// ������������Ʒ�������ʱ������10��.
#define BWSERVER_RECONNECT_TIMESPAN 10 * 1000

// ����ע��ص�����.
typedef void (CALLBACK * BwRegistMsgCB)(BOOL bRet, LPVOID pParam);

// ����ע���߳���Ϣ.
class CBwControl;
typedef struct _RegisThreadData
{
	CBwControl* pControl;		// ��ǰָ��.
	BwRegistMsgCB pfnMsgCB;		// ������ƻص�����.
	LPVOID pUserData;			// �û��Զ�������.

	_RegisThreadData()
	{
		pControl = NULL;
		pfnMsgCB = NULL;
		pUserData = NULL;
	}
}RegisThreadData, *LPRegisThreadData;


class CBwControl
{
public:
	CBwControl(void);
	~CBwControl(void);

private:
	// ������Ʒ�����ע���߳̾��.
	HANDLE m_hRegThread;
	// ������Ʒ�����ע���߳�ֹͣ�ź���.
	HANDLE m_hStopReg;
	// ����ע���̲߳���.
	RegisThreadData m_stData;

public:
	// ע�������Ʒ�����.
	BOOL RegistBwServer(BwRegistMsgCB pfnBwFunc, LPVOID pUserData);
	// ע�������Ʒ������̺߳���.
	static DWORD WINAPI ThreadFuncRegBwServer(LPVOID pParam);
	// ֹͣע�������Ʒ�����.
	void StopRegistBwServer();
	// ����������.
	BOOL ReqBwControl(int nCameraID, BOOL bReq);
	// �����Ƿ�ע�����.
	void SetRegisted();
};
