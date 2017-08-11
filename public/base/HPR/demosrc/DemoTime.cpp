#include "DemoApp.h"
#include <time.h>

void Test_Time()
{
#if 0
	HPR_TIME_EXP_T newtime = {0};
	HPR_TIME_T tTime = 0x4b06729c;
	HPR_UINT32 nTime = tTime;
	//将本地时间转化为年月日格式
	printf("nTime = %x\n", nTime);
	//nTime = HPR_Ntohl(nTime);
	printf("nTime = %x\n", nTime);
	//HPR_TIME_T tNow = HPR_TimeFromAnsiTime((time_t)nTime);
	//HPR_ExpTimeFromTimeGMT(tNow, &newtime);

	printf("nTime = %x\n", nTime);


	HPR_TIME_T tNow = HPR_TimeNow();
	fprintf(stderr, "%ul\n", tNow);
	HPR_TIME_EXP_T timePara = {0};
	//将本地时间转化为年月日格式
	HPR_ExpTimeFromTimeLocal(tNow, &timePara);

	fprintf(stderr, "%04d:%02d:%02d %02d:%02d:%02d\n", timePara.tm_year+1900, \
		timePara.tm_mon, timePara.tm_mday, timePara.tm_hour, timePara.tm_min, timePara.tm_sec);
	//HPR_INT32 len = (HPR_INT32)((sizeof(logbuf) - 1) - strlen(logbuf));
	//if (HPR_StrFmtTime(logbuf+strlen(logbuf), &len, DEBUG_PRINTMSG_MAXLEN
	//	, "%Y-%M-%D %H:%M:%S ", &timePara) != HPR_OK)
	//{
	//	//HPR_OutputDebug("the time info of writelog error\n");
	//	return HPR_ERROR;
	//}
#endif

	HPR_OutputDebug("now time is %lu\n", HPR_GetTimeTick64());

	HPR_Sleep(3000);

	HPR_OutputDebug("after sleep 3s, now time is %lu\n", HPR_GetTimeTick64());


#if 0
	HPR_TIME_EXP_T struExpTime;
	memset(&struExpTime, 0, sizeof(struExpTime));
	HPR_TIME_T time1 =  HPR_TimeNow();
	HPR_TIME_T time2 = HPR_AnsiTimeFromTime(time1);
	HPR_TIME_T time3 = time1/1000000;
	HPR_TIME_T currTime = time(NULL);
	//fprintf(stderr, "%I64u - %u - %u - %u\n", time1, time2, time3, currTime);
	cout << (HPR_INT32)time1 << "-" << (HPR_INT32)time2 << "-" << (HPR_INT32)time3 << "-" << (HPR_INT32)currTime << endl;
	
	HPR_ExpTimeFromTimeLocal(time1, &struExpTime);
	HPR_ExpTimeFromTimeGMT(time1, &struExpTime);
#endif

	return;
}
