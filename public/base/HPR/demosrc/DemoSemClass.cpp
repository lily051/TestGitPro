#include "DemoSemClass.h"
#include "HPR_Utils.h"
#include "HPR_Thread.h"
#include <iostream>
using namespace std;

#ifndef INT_MAX
#define INT_MAX (2147483647)
#endif

void i_test::i_sem::Test()
{
	cout << "+++Test hpr::hpr_sem" << endl;

	post_2_before_wait();

	post_1_before_wait();

	post_1_after_wait();

	wait_timeout_0();
	wait_timeout_3();
	wait_max_int();
}

void* CALLBACK i_test::i_sem::svc(void* param)
{
	if (!param)
	{
		return 0;
	}

	i_test::i_sem* sem = (i_test::i_sem*)(param);

	HPR_Sleep(1000);

	sem->m_sem.post_1();

	return 0;
}

void i_test::i_sem::post_2_before_wait()
{
	if (-1 == m_sem.post(2))
	{
		cout << ITEST_FAIL << "post_2_before_wait post 2 sem" << endl;
		return;
	}

	if (-1 == m_sem.wait(-1))
	{
		cout << ITEST_FAIL << "post_2_before_wait wait 1st sem" << endl;
		return;
	}

	if (-1 == m_sem.wait(-1))
	{
		cout << ITEST_FAIL << "post_2_before_wait wait 2nd sem" << endl;
		return;
	}
	else
	{
		cout << ITEST_PASS << "post_2_before_wait" << endl;
		return;
	}
}

void i_test::i_sem::post_1_before_wait()
{
	if (-1 == m_sem.post_1())
	{
		cout << ITEST_FAIL << "post_1_before_wait post 1 sem" << endl;
		return;
	}

	if (-1 == m_sem.wait(-1))
	{
		cout << ITEST_FAIL << "post_1_before_wait wait 1st sem" << endl;
		return;
	}
	else
	{
		cout << ITEST_PASS << "post_1_before_wait" << endl;
		return;
	}
}

void i_test::i_sem::post_1_after_wait()
{
	m_hThreadId = HPR_Thread_Create(svc, this, 0);

	if (-1 == m_sem.wait(-1))
	{
		cout << ITEST_FAIL << "post_1_after_wait main wait" << endl;
	}
	else
	{
		cout << ITEST_PASS << "post_1_after_wait" << endl;
	}

	HPR_Thread_Wait(m_hThreadId);

	return;
}

void i_test::i_sem::wait_timeout_0()
{
	if (0 != m_sem.post(1))
	{
		cout << ITEST_FAIL << "wait_timeout_0 post fail" << endl;
	}

	if (0 == m_sem.wait(0))
	{
		cout << ITEST_PASS << "wait_timeout_0" << endl;
	}
	else
	{
		cout << ITEST_FAIL << "wait_timeout_0 wait fail" << endl;
	}
}

void i_test::i_sem::wait_timeout_3()
{
	m_hThreadId = HPR_Thread_Create(svc, this, 0);

	if (-1 == m_sem.wait(3000))
	{
		cout << ITEST_FAIL << "wait_timeout_3 main wait" << endl;
	}
	else
	{
		cout << ITEST_PASS << "wait_timeout_3" << endl;
	}

	HPR_Thread_Wait(m_hThreadId);

	return;
}

void i_test::i_sem::wait_max_int()
{
	if (0 != m_sem.post(1))
	{
		cout << ITEST_FAIL << "wait_max_int post fail" << endl;
	}

	if (0 == m_sem.wait(INT_MAX))
	{
		cout << ITEST_PASS << "wait_max_int" << endl;
	}
	else
	{
		cout << ITEST_FAIL << "wait_max_int wait fail" << endl;
	}
}
