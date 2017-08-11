#ifndef __DEMO_STRING_CLASS_H__
#define __DEMO_STRING_CLASS_H__

#include "HPR_String.h"
#include "DemoClass.h"

namespace i_test
{
	class i_string : public test
	{
	public:
		void Test();

	public:
		void t_itoa();
		void t_split();

	private:
		hpr::hpr_string m_string;
	};
}

#endif

