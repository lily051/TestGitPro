#ifndef __INTEGRATED_TEST_CLASS_H__
#define __INTEGRATED_TEST_CLASS_H__


#define ITEST_FAIL "===[FAIL] "
#define ITEST_PASS "===[PASS] "

namespace i_test
{
	class test
	{
	public:
		virtual ~test(){};

	public:
		virtual void Test() = 0;
	};
}


#endif

