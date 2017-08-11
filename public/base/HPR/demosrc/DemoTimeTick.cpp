#include "DemoApp.h"
#include "DemoClass.h"
#include "HPR_Types.h"
#include <iostream>

static void judge_convert(hpr_int64 result, hpr_int64 need, int param, const char* desc)
{
	if (result != need)
	{
		cout << ITEST_FAIL << " " << desc << " " << param << endl;
	}
}

void Test_TimeTick()
{
	hpr::hpr_time_tick cTimeTick;
	
	hpr_int64 t1;
	hpr_int64 t2;
	hpr_int64 t3;

	//////////////////////////////////////////////////////////////////////////
	t1 = cTimeTick.get_time();
	if (t1 == 0)
	{
		cout << ITEST_FAIL << "get_time()" << endl;
		return;
	}
	else
	{
		cout << ITEST_PASS << "get_time()" << endl;
	}

	//////////////////////////////////////////////////////////////////////////
	HPR_Sleep(500);
	t2 = cTimeTick.now();
	t3 = cTimeTick.update_time();
	//printf("%ll\n", t2);
	//printf("%ll\n", t3);
	//cout << (hpr_int32)(t2 = cTimeTick.now()) << endl;
	//cout << (hpr_int32)(t3 = cTimeTick.update_time()) << endl;
	if (t2 < (t1 + 400))
	{
		cout << ITEST_FAIL << "now()" << endl;
		cout <<  (hpr_int32)t1 << endl;
		cout << (hpr_int32)t2 << endl;
		return;
	}	
	else
	{
		cout << ITEST_PASS << "now()" << endl;
	}

	if (t3 < t2)
	{
		cout << ITEST_FAIL << "update_time()" << endl;
		return;
	}
	cout << ITEST_PASS << "update_time()" << endl;

	//////////////////////////////////////////////////////////////////////////
	hpr_int64 t4 = t3;
	hpr::hpr_time_tick timetick2(t4);
	if (t4 != t3)
	{
		std::cout << ITEST_FAIL << "hpr_time_tick(hpr_int64 t)" << endl;
		return;
	}
	std::cout << ITEST_PASS << "hpr_time_tick(hpr_int64 t)" << endl;

	//////////////////////////////////////////////////////////////////////////
	hpr::hpr_time_tick timetick3(timetick2);
	if (timetick3.get_time() != t3)
	{
		std::cout << ITEST_FAIL << "hpr_time_tick(const hpr_time_tick&)" << endl;
		return;
	}
	std::cout << ITEST_PASS << "hpr_time_tick(const hpr_time_tick&)" << endl;

	//////////////////////////////////////////////////////////////////////////
	timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 1000;
	hpr_int64 mt = 0;
	mt = hpr::hpr_time_tick::timeval2ms(&tv);
	//std::cout << (hpr_int32)mt << endl;
	//printf("%ll", mt);
	if (mt != ((tv.tv_sec*HPR_MS_PER_SEC) + (tv.tv_usec/HPR_US_PER_MS)))
	{
		cout << ITEST_FAIL << "timeval2ms" << endl;
		return;
		//cout << "timeval2ms---" << endl;
		//cout << "mt = " << (hpr_int32)mt << endl;
		////printf("%ld", mt);
		//cout << "tv.tv_sec = " << tv.tv_sec << endl;
		//cout << "tv.tv_usec = " << tv.tv_usec << endl;
		//std::cout << "!!! timeval2ms error" << endl;
	}
	cout << ITEST_PASS << "timeval2ms" << endl;

	//////////////////////////////////////////////////////////////////////////
	hpr_int64 ut = 0;
	ut = hpr::hpr_time_tick::timeval2us(&tv);
	//printf("%ld", ut);
	//std::cout << (hpr_int32)ut << endl;
	if (ut != ((tv.tv_sec*HPR_US_PER_SEC) + tv.tv_usec))
	{
		cout << ITEST_FAIL << "timeval2us" << endl;
		//std::cout << "!!! timeval2us error" << endl;
		return;
	}
	cout << ITEST_PASS << "timeval2us" << endl;

	//////////////////////////////////////////////////////////////////////////
	timeval tv2 = {0, 0};
	hpr::hpr_time_tick::ms2timeval(mt, &tv2);
	if (tv.tv_sec != tv2.tv_sec || tv2.tv_usec != 1000)
	{
		cout << ITEST_FAIL << "ms2timeval" << endl;
		//cout << "ms2timeval----" << endl;
		//std::cout << (hpr_int32)mt << endl;
		////printf("%ld", mt);
		//std::cout << tv.tv_sec << endl;
		//std::cout << tv.tv_usec << endl;
		//std::cout << tv2.tv_sec << endl;
		//std::cout << tv2.tv_usec << endl;

		//std::cout << "!!! ms2timeval error" << endl;
	}
	cout << ITEST_PASS << "ms2timeval" << endl;

	hpr::hpr_time_tick::us2timeval(ut, &tv2);
	if ((tv.tv_sec != tv2.tv_sec) || (tv.tv_usec != tv2.tv_usec))
	{
		cout << ITEST_FAIL << "us2timeval" << endl;
		//std::cout << "!!! us2timeval error" << endl;
	}
	cout << ITEST_PASS << "us2timeval" << endl;


	hpr_int64 us;
	hpr_int64 ms;
	//hpr_int64 s;
	//////////////////////////////////////////////////////////////////////////
	//us2ms
	us = 1001;
	ms = hpr::hpr_time_tick::us2ms(us);
	judge_convert(ms, 1, us, "us2ms");

	us = 1500;
	ms = hpr::hpr_time_tick::us2ms(us);
	judge_convert(ms, 2, us, "us2ms");

	us = 1499;
	ms = hpr::hpr_time_tick::us2ms(us);
	judge_convert(ms, 1, us, "us2ms");

	us = 1999;
	ms = hpr::hpr_time_tick::us2ms(us);
	judge_convert(ms, 2, us, "us2ms");

	us = 0;
	ms = hpr::hpr_time_tick::us2ms(us);
	judge_convert(ms, 0, us, "us2ms");

	us = 999;
	ms = hpr::hpr_time_tick::us2ms(us);
	judge_convert(ms, 1, us, "us2ms");

	us = 1;
	ms = hpr::hpr_time_tick::us2ms(us);
	judge_convert(ms, 0, us, "us2ms");

	cout << ITEST_PASS << "us2ms" << endl;
	
	//////////////////////////////////////////////////////////////////////////
	 //us2s
	//us = 1000001;
	//s = hpr::hpr_time_tick::us2s(us);
	//judge_convert(s, 1, us, "us2s");

	//us = 1000500;
	//s = hpr::hpr_time_tick::us2s(us);
	//judge_convert(s, 2, us, "us2s");

	//us = 1000499;
	//s = hpr::hpr_time_tick::us2s(us);
	//judge_convert(s, 1, us, "us2s");

	//us = 1999;
	//s = hpr::hpr_time_tick::us2s(us);
	//judge_convert(s, 2, us, "us2s");

	//us = 0;
	//s = hpr::hpr_time_tick::us2s(us);
	//judge_convert(s, 0, us, "us2s");

	//us = 999;
	//s = hpr::hpr_time_tick::us2s(us);
	//judge_convert(s, 1, us, "us2s");

	//us = 1;
	//s = hpr::hpr_time_tick::us2s(us);
	//judge_convert(s, 0, us, "us2s");
	//cout << ITEST_PASS << "us2s" << endl;

	//////////////////////////////////////////////////////////////////////////
	//ms2s
	//cout << ITEST_PASS << "ms2s" << endl;


}
