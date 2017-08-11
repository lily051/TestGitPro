#ifndef __DEMO_MSGQ_CLASS_H__
#define __DEMO_MSGQ_CLASS_H__

#include "DemoClass.h"
#include "HPR_MsgQ.h"

namespace i_test
{
	class i_msgq : public test
	{
	public:
		static void* CALLBACK svc(void*);
		void Test();

	public:
		void putq_before_getq();
		void putq_after_getq();

		void getq_timeout_0();
		void getq_timeout_3s();
		void getq_timeout_infinite();

	private:
		hpr::hpr_msgq m_msgq;
		hpr_handle m_hThreadId;
	};
}

#endif
