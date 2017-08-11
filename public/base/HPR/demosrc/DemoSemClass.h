#ifndef __DEMO_SEM_CLASS_H__
#define __DEMO_SEM_CLASS_H__

#include "DemoClass.h"
#include "HPR_Semaphore.h"

namespace i_test
{

	class i_sem : public test
	{
	public:
		static void* CALLBACK svc(void* param);

	public:
		void post_2_before_wait();
		void post_1_before_wait();
		void post_1_after_wait();

		void wait_timeout_0();
		//void wait_timeout_forever();//already tested in post case, not test again.
		void wait_timeout_3();
		void wait_max_int();

	public:
		void Test();

	private:
		hpr::hpr_sem m_sem;
		hpr_handle m_hThreadId;
	};
}

#endif
