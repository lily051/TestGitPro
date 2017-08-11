#ifndef __DEMO_MUTEX_CLASS_H__
#define __DEMO_MUTEX_CLASS_H__


#include "DemoApp.h"
#include "DemoClass.h"

/**
 * test case
 * lock : lock is not locked before. 
 * lock : lock is locked before.
 * lock : recursive lock.
 * trylock : lock is locked before
 * trylock : lock is not locked before.
 * unlock : lock is locked before.
 * unlock : lock is not locked before.
 */

#include "HPR_Mutex.h"

namespace i_test
{
	class i_mutex : public i_test::test
	{
	public:
		static void* CALLBACK svc_lock_lock(void*);
		static void* CALLBACK svc_trylock_lock(void*);

	public:

	public:
		void Test();
		//void stat(int total, int passed, int failed);

	private:
		int lock_no_locked_before();
		int lock_locked_before();
		int lock_recursive();

		int trylock_locked_before();
		int trylocl_no_locked_before();
		int trylock_recursive();

	private:
		hpr::hpr_mutex m_mutex;
		hpr_handle m_hThread;

	private:
	};
}
#endif
