#include "DemoMsgQClass.h"
#include "HPR_Thread.h"
#include "HPR_Utils.h"
#include <iostream>
using namespace std;

void* i_test::i_msgq::svc(void* param)
{
	if (param == NULL)
	{
		return 0;
	}

	i_test::i_msgq* mq = (i_test::i_msgq*)(param);

	HPR_MSG_BLOCK_T* msg = new HPR_MSG_BLOCK_T;
	if (msg != NULL)
	{
		msg->iMsgType = 1;
		msg->iMsgLen = 1;
		msg->pMsg[0] = '1';
	}

	HPR_Sleep(2000);

	mq->m_msgq.putq(msg);

	return 0;
}

void i_test::i_msgq::Test()
{
	cout << "+++Test hpr::hpr_msgq" << endl;

	putq_before_getq();
	putq_after_getq();

	getq_timeout_0();
	getq_timeout_3s();
	getq_timeout_infinite();
}

void i_test::i_msgq::putq_before_getq()
{
	HPR_MSG_BLOCK_T* msg = new HPR_MSG_BLOCK_T;
	if (msg != NULL)
	{
		msg->iMsgType = 1;
		msg->iMsgLen = 1;
		msg->pMsg[0] = '1';
	}

	if (-1 == m_msgq.putq(msg))
	{
		cout << ITEST_FAIL << "putq_before_getq  putq" << endl;
	}

	HPR_MSG_BLOCK_T* m = NULL;
	if (-1 == m_msgq.getq(&m, 0))
	{
		cout << ITEST_FAIL << "putq_before_getq  getq" << endl;
		return;
	}

	if (m->iMsgLen == 1 && m->iMsgType == 1 && m->pMsg[0] == '1')
	{
		cout << ITEST_PASS << "putq_before_getq" << endl;
	}
	else
	{
		cout << ITEST_FAIL << "putq_before_getq  content" << endl;
	}
}

void i_test::i_msgq::putq_after_getq()
{
	m_hThreadId = HPR_Thread_Create(svc, this, 0);

	HPR_MSG_BLOCK_T* msg = NULL;
	if (-1 == m_msgq.getq(&msg, -1))
	{
		cout << ITEST_FAIL << "putq_after_getq getq" << endl;
		return;
	}

	if (msg->iMsgLen == 1 && msg->iMsgType == 1 && msg->pMsg[0] == '1')
	{
		cout << ITEST_PASS << "putq_after_getq" << endl;	
	}
	else
	{
		cout << ITEST_FAIL << "putq_after_getq getq content" << endl;
		return;
	}

	delete msg;

	HPR_Thread_Wait(m_hThreadId);
}

void i_test::i_msgq::getq_timeout_0()
{
	m_hThreadId = HPR_Thread_Create(svc, this, 0);

	HPR_MSG_BLOCK_T* msg = NULL;
	if (-1 == m_msgq.getq(&msg, 0))
	{
		cout << ITEST_PASS << "getq_timeout_0 getq" << endl;
		return;
	}

	HPR_Thread_Wait(m_hThreadId);
}

void i_test::i_msgq::getq_timeout_3s()
{
	m_hThreadId = HPR_Thread_Create(svc, this, 0);

	HPR_MSG_BLOCK_T* msg = NULL;
	if (-1 == m_msgq.getq(&msg, 3000))
	{
		cout << ITEST_FAIL << "getq_timeout_3s getq" << endl;
		return;
	}

	if (msg->iMsgLen == 1 && msg->iMsgType == 1 && msg->pMsg[0] == '1')
	{
		cout << ITEST_PASS << "getq_timeout_3s" << endl;	
	}
	else
	{
		cout << ITEST_FAIL << "getq_timeout_3s getq content" << endl;
		return;
	}

	delete msg;

	HPR_Thread_Wait(m_hThreadId);
}

void i_test::i_msgq::getq_timeout_infinite()
{
	m_hThreadId = HPR_Thread_Create(svc, this, 0);

	HPR_MSG_BLOCK_T* msg = NULL;
	if (-1 == m_msgq.getq(&msg, -1))
	{
		cout << ITEST_FAIL << "getq_timeout_infinite getq" << endl;
		return;
	}

	if (msg->iMsgLen == 1 && msg->iMsgType == 1 && msg->pMsg[0] == '1')
	{
		cout << ITEST_PASS << "getq_timeout_infinite" << endl;	
	}
	else
	{
		cout << ITEST_FAIL << "getq_timeout_infinite getq content" << endl;
		return;
	}

	delete msg;

	HPR_Thread_Wait(m_hThreadId);
}
