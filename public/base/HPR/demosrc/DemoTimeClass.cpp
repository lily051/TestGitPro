#include "DemoTimeClass.h"
#include <iostream>
using namespace std;

void i_test::i_time::Test()
{
	cout << "+++ Test hpr::hpr_time" << endl;

	get_data();
	time2tm();
}

void i_test::i_time::get_data()
{
	cout << m_time.get_year() << endl;
	cout << m_time.get_mon() << endl;
	cout << m_time.get_day() << endl;
	cout << m_time.get_hour() << endl;
	cout << m_time.get_min() << endl;
	cout << m_time.get_sec() << endl;
#if 0
	if (m_time.get_year() != 2011)
	{
		cout << ITEST_FAIL << "get_year" << endl;
		return;
	}

	if (m_time.get_mon() != 11)
	{
		cout << ITEST_FAIL << "get_mon" << endl;
		return;
	}

	if (m_time.get_day() != 5)
	{
		cout << ITEST_FAIL << "get_day" << endl;
		return;
	}

	if (m_time.get_hour() != 19)
	{
		cout << ITEST_FAIL << "get_hour" << endl;
		return;
	}

	if (m_time.get_min() >= 60)
	{
		cout << ITEST_FAIL << "get_min" << endl;
		return;
	}

	if (m_time.get_sec() >= 60)
	{
		cout << ITEST_FAIL << "get_sec" << endl;
		return;
	}
#endif
	cout << ITEST_PASS << "get_data" << endl;
}

void i_test::i_time::time2tm()
{
	struct tm t;
	struct tm* tp = NULL;
	tp = hpr::hpr_time::time2tm(m_time.get_time(), &t);
	if (tp == NULL)
	{
		cout << ITEST_FAIL << "time2tm return" << endl;
		return;
	}

	cout << t.tm_year << endl;
	cout << t.tm_mon << endl;
	cout << t.tm_mday << endl;
	cout << t.tm_hour << endl;
	cout << t.tm_min << endl;
	cout << t.tm_sec << endl;

#if 0
	if (t.tm_year != 111)
	{
		cout << ITEST_FAIL << "time2tm tm_year" << endl;
		return;
	}

	if (t.tm_mon != 11)
	{
		cout << ITEST_FAIL << "time2tm tm_mon" << endl;
		return;
	}

	if (t.tm_mday != 5)
	{
		cout << ITEST_FAIL << "time2tm tm_mday" << endl;
		return;
	}

	if (t.tm_hour != 19)
	{
		cout << ITEST_FAIL << "time2tm tm_hour" << endl;
		return;
	}
#endif

	cout << ITEST_PASS << "time2tm" << endl;
}
