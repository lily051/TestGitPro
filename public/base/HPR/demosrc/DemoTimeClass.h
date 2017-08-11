#ifndef __DEMO_TIME_CLASS_H__
#define __DEMO_TIME_CLASS_H__

#include "DemoClass.h"
#include "HPR_Time.h"

namespace i_test
{
	class i_time : public test
	{
	public:
		void Test();

	public:
		void get_data();
		void time2tm();

	private:
		hpr::hpr_time m_time;
	};
}

#endif
