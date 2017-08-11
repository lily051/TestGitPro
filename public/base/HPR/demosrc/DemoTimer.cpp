#include "DemoApp.h"
#include <iostream>
using namespace std;

static HPR_INT64 g_tStartTime;
//static HPR_THREAD_HANDLE g_hThread;
//static void* test_timer_svc(void* pParam);
static void* test_ontimer_svc(HPR_INT32 hEvent, HPR_VOIDPTR pUsrData);

void Test_Timer()
{
	HPR_INT32 hEvent;
	HPR_INT32 ret;
	g_tStartTime = HPR_TimeNow();

	//printf("STDOUT = %d\n", stdout);

	//ret = HPR_SetTimer(test_ontimer_svc, NULL, 3, &hEvent, HPR_FALSE);   // in fact it cost 4ms
	//ret = HPR_SetTimer(test_ontimer_svc, NULL, 15, &hEvent, HPR_FALSE);  // in fact it cost 17ms
	ret = HPR_SetTimer(test_ontimer_svc, NULL, 3000, &hEvent, HPR_FALSE);  // in fact it cost 3001ms
	if (ret)
	{
		printf("Test_Timer HPR_SetTimer error, may bugs!!!, errno = %d, %s\n", errno, strerror(errno));
		return;
	}

	cin.get();

	HPR_KillTimer(hEvent);

}

void* test_ontimer_svc(HPR_INT32 hEvent, HPR_VOIDPTR pUsrData)
{
	HPR_INT64 tNow;
	HPR_INT64 tDiff;
	tNow = HPR_TimeNow();

	tDiff = tNow - g_tStartTime;
	//if (tDiff > 1500)
	//{
	//cout << "tDiff = " << tDiff << "us" << endl;
	//fprintf(stderr, "tDiff = %I64u us\n", tDiff);
	cout << "tDiff = " << (HPR_INT32)tDiff << endl;
	//}

	g_tStartTime = tNow;

	return 0;
}

