#include "DemoApp.h"
#include "DemoClass.h"
#include "DemoMutexClass.h"


void* CALLBACK i_test::i_mutex::svc_lock_lock(void* param)
{
	if (param == NULL)
	{
		return 0;
	}

	i_test::i_mutex* mutex = (i_test::i_mutex*)(param);

	mutex->m_mutex.lock();

	mutex->m_mutex.unlock();

	return 0;
}

void* CALLBACK i_test::i_mutex::svc_trylock_lock(void* param)
{
	if (param == NULL)
	{
		return 0;
	}

	i_test::i_mutex* mutex = (i_test::i_mutex*)(param);

	if (-1 == mutex->m_mutex.trylock())
	{
		cout << ITEST_PASS << "trylock_locked_before" << endl;
 	}
	else
	{
		cout << ITEST_FAIL << "trylock_locked_before" << endl;
	}

	return 0;
}

void i_test::i_mutex::Test()
{
	cout << "+++Test hpr::hpr_mutex" << endl;

	lock_no_locked_before();
	lock_recursive();

	trylock_locked_before();
	trylocl_no_locked_before();
	trylock_recursive();

	//open it when needed.
	//lock_locked_before();
}

//void i_test::i_mutex::stat(int total, int passed, int failed)
//{
//	(void)(total);
//	(void)(passed);
//	(void)(failed);
//
//	return ;
//}

int i_test::i_mutex::lock_no_locked_before()
{
	if (0 == m_mutex.lock())
	{
		cout << ITEST_PASS << "lock_no_locked_before" << endl;
	}
	else
	{
		cout << ITEST_FAIL << "lock_no_locked_before" << endl;
	}

	m_mutex.unlock();

	return 0;
}

int i_test::i_mutex::lock_locked_before()
{
	m_mutex.lock();

	m_hThread = HPR_Thread_Create(svc_lock_lock, this, 0);

	HPR_Thread_Wait(m_hThread);

	cout << ITEST_FAIL << "lock_locked_before" << endl;

	return 0;
}

int i_test::i_mutex::lock_recursive()
{
	if (-1 == m_mutex.lock())
	{
		cout << ITEST_FAIL << "lock_recursive 1 lock" << endl;
	}

	if (-1 == m_mutex.lock())
	{
		cout << ITEST_FAIL << "lock_recursive 2 lock" << endl;
	}
	else
	{
		cout << ITEST_PASS << "lock_recursive" << endl;
	}

	m_mutex.unlock();
	m_mutex.unlock();

	return 0;
}

int i_test::i_mutex::trylock_locked_before()
{
	m_mutex.lock();

	m_hThread = HPR_Thread_Create(svc_trylock_lock, this, 0);

	HPR_Thread_Wait(m_hThread);

	return 0;
}

int i_test::i_mutex::trylocl_no_locked_before()
{

	if (0 == m_mutex.trylock())
	{
		cout << ITEST_PASS << "trylocl_no_locked_before" << endl;
	}
	else
	{
		cout << ITEST_FAIL << "trylocl_no_locked_before" << endl;
	}

	return 0;
}

int i_test::i_mutex::trylock_recursive()
{
	if (-1 == m_mutex.trylock())
	{
		cout << ITEST_FAIL << "trylock_recursive 1 trylock" << endl;
	}

	if (-1 == m_mutex.trylock())
	{
		cout << ITEST_FAIL << "trylock_recursive 2 trylock" << endl;
	}
	else
	{
		cout << ITEST_PASS << "trylock_recursive" << endl;
	}

	m_mutex.unlock();
	m_mutex.unlock();

	return 0;
}

