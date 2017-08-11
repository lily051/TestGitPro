#include "DemoApp.h"

static void test_atomic();

void Test_Atomic()
{
	test_atomic();
}

void test_increment()
{
	HPR_ATOMIC_T iCount = 1;
	HPR_AtomicInc(&iCount);
	printf("test_increment result = %d, right result is 2\n", iCount);
}

void test_decrement()
{
	HPR_ATOMIC_T iCount = 1;
	HPR_AtomicDec(&iCount);
	printf("test_decrement result = %d, right result is 0\n", iCount);
}

void test_add()
{
	HPR_ATOMIC_T iCount = 1;
	HPR_AtomicAdd(&iCount,10);
	printf("test_increment result = %d, right result is 11\n", iCount);
}

void test_cas()
{
	HPR_ATOMIC_T iCount = 1;
	HPR_AtomicCas(&iCount,10,1);
	printf("test_increment result = %d, right result is 10\n", iCount);
	HPR_AtomicCas(&iCount,11,1);
	printf("test_increment result = %d, right result is 10\n", iCount);
}

void test_atomic()
{
	test_increment();
	test_decrement();
	test_add();
	test_cas();
}

