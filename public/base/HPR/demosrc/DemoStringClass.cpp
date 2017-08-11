#include "DemoStringClass.h"
#include <iostream>
using namespace std;

void i_test::i_string::Test()
{
	cout << "+++Test hpr::i_string" << endl;

	t_itoa();
	//t_split();
}

void i_test::i_string::t_itoa()
{
	int i = 10;
	char result[5];
	char* p = NULL;

	p = hpr::hpr_string::itoa(result, i, 10);
	if ((p == result) && hpr::hpr_string::strncasecmp(result, "10", 2) == 0)
	{
		cout << ITEST_PASS << "i_test::i_string::t_itoa d" << endl;
		//cout << "result d is " << result << endl;
	}
	else
	{
		cout << ITEST_FAIL << "i_test::i_string::t_itoa d" << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	p = hpr::hpr_string::itoa(result, i, 16);
	if ((p == result) && hpr::hpr_string::strncasecmp(result, "A", 1) == 0)
	{
		cout << ITEST_PASS << "i_test::i_string::t_itoa x" << endl;
		//cout << "result x is " << result << endl;
	}
	else
	{
		cout << ITEST_FAIL << "i_test::i_string::t_itoa x" << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	p = hpr::hpr_string::itoa(result, i, 8);
	if ((p == result) && hpr::hpr_string::strncasecmp(result, "12", 2) == 0)
	{
		cout << ITEST_PASS << "i_test::i_string::t_itoa o" << endl;
		//cout << "result o is " << result << endl;
	}
	else
	{
		cout << ITEST_FAIL << "i_test::i_string::t_itoa o" << endl;
	}

	//i = ;
}
//
//void i_test::i_string::t_split()
//{
//	const char* str = "123:456:789:abc:def";
//	char array[10][10];
//	hpr_int32 count = 10;
//	char c = ':';
//	hpr_int32 ret = 0;
//
//	ret = hpr::hpr_string::split(str, c, array, &count);
//
//	if (ret == 0)
//	{
//		if (count == 5)
//		{
//			if ((hpr::hpr_string::strcmp(array[0], "123") == 0) &&
//				(hpr::hpr_string::strcmp(array[1], "456") == 0) &&
//				(hpr::hpr_string::strcmp(array[2], "789") == 0) &&
//				(hpr::hpr_string::strcmp(array[3], "abc") == 0) &&
//				(hpr::hpr_string::strcmp(array[4], "def") == 0)
//				)
//			{
//				cout << ITEST_PASS << "hpr::hpr_string::split" << endl;
//				return;
//			}
//		}
//	}
//
//	cout << ITEST_FAIL << "hpr::hpr_string::split" << endl;
//}


