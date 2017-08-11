#include "DemoApp.h"

void test_str_itoa();
void test_str_ltoa();
void test_str_atoi64();
void test_str_atoi32();
void test_str_strtoi64();
void test_str_strtoi32();
void test_str_snprintf();
void test_str_vsnprintf();
void test_str_strcpy();
void test_str_strncpy();
void test_str_strcmp();
void test_str_strncmp();
void test_str_strcasecmp();
void test_str_strncasecmp();
void test_str_strchr();
void test_str_strnchr();
void test_str_strstr();
void test_str_strnstr();
void test_str_strcasestr();

void Test_String()
{
	printf("*********test_str_itoa****************\n");
	test_str_itoa();
	printf("*********test_str_ltoa***************\n");
	test_str_ltoa();
	printf("*********test_str_atoi64***************\n");
	test_str_atoi64();
	printf("*********test_str_atoi32***************\n");
	test_str_atoi32();
	printf("*********test_str_strtoi64***************\n");
	test_str_strtoi64();
	printf("*********test_str_strtoi32***************\n");
	test_str_strtoi32();
	printf("*********test_str_snprintf***************\n");
	test_str_snprintf();
	printf("*********test_str_vsnprintf***************\n");
	test_str_vsnprintf();
	printf("*********test_str_strcpy***************\n");
	test_str_strcpy();
	printf("*********test_str_strncpy***************\n");
	test_str_strncpy();
	printf("*********test_str_strcmp***************\n");
	test_str_strcmp();
	printf("*********test_str_strncmp***************\n");
	test_str_strncmp();
	printf("*********test_str_strcasecmp***************\n");
	test_str_strcasecmp();
	printf("*********test_str_strncasecmp***************\n");
	test_str_strncasecmp();
	printf("*********test_str_strchr***************\n");
	test_str_strchr();
	printf("*********test_str_strnchr***************\n");
	test_str_strnchr();
	printf("*********test_str_strstr***************\n");
	test_str_strstr();
	printf("*********test_str_strnstr***************\n");
	test_str_strnstr();
	printf("*********test_str_strcasestr***************\n");
	test_str_strcasestr();
}

void test_str_itoa()
{
	char szStr[256] = {0};
	int i = 100;
	int j = 100;
	int k = 100;

	printf("i = %s, szStr = %s\n", HPR_Itoa(szStr,i,10), szStr);
	printf("j = %s, szStr = %s\n", HPR_Itoa(szStr,j,16), szStr);
	printf("k = %s, szStr = %s\n", HPR_Itoa(szStr,k,8), szStr);
}

void test_str_ltoa()
{
	char szStr[256] = {0};
	int i = -100;
	int j = 100;
	int k = 100;

	printf("i = %s, szStr = %s\n", HPR_Itoa(szStr,i,10), szStr);
	printf("j = %s, szStr = %s\n", HPR_Itoa(szStr,j,16), szStr);
	printf("k = %s, szStr = %s\n", HPR_Itoa(szStr,k,8), szStr);
}

void test_str_atoi64()
{
	char* szBuf = "1255190400";	
	HPR_UINT64 iRet  = HPR_Atoi64(szBuf);
	//printf("i = %lld\n", iRet);
	cout << "i = " << (hpr_int32)iRet << endl;
}

void test_str_atoi32()
{
	HPR_INT32 i = HPR_Atoi64("-100");
	cout << "i = " << i << endl;
}

void test_str_strtoi64()
{
	char * pi,* pj, *pk;
	int i = HPR_StrToi64("-100jsdk", &pi, 10);
	cout << "i = " << i << endl;
	//printf("i = %lld\n", HPR_StrToi64("-100jsdk", &pi, 10));
	printf("pi = %s\n", pi);
	int j = HPR_StrToi64("0x100kl", &pj, 16);
	cout << "j = " << j << endl;
	//printf("j = %lld\n", HPR_StrToi64("0x100kl", &pj, 16));
	printf("pj = %s\n", pj);
	int k = HPR_StrToi64("0100,d", &pk, 8);
	cout << "k = " << k << endl;
	//printf("k = %lld\n", HPR_StrToi64("0100,d", &pk, 8));
	printf("pk = %s\n", pk);
}

void test_str_strtoi32()
{
	char * pi,* pj, *pk;
	printf("i = %d\n", HPR_StrToi32("-100000jsdk", &pi, 10));
	printf("pi = %s\n", pi);
	printf("j = %d\n", HPR_StrToi32("0x10000000kl", &pj, 16));
	printf("pj = %s\n", pj);
	printf("k = %d\n", HPR_StrToi32("01iiiii00,d", &pk, 8));
	printf("pk = %s\n", pk);
}

void test_str_snprintf()
{
}

void test_str_vsnprintf()
{
}

void test_str_strcpy()
{
	char szDebug[8] = {0};
	printf("string = %s\n", HPR_Strcpy(szDebug, "12345"));
	printf("szDebug = %s\n", szDebug);
}

void test_str_strncpy()
{
	char szDebug[8] = {0};
	printf("string = %s\n", HPR_Strncpy(szDebug, "9876543210", 6));
	printf("szDebug = %s\n", szDebug);
}

void test_str_strcmp()
{
	printf("compare result is %d \n", HPR_Strcmp("12345", "12345"));
	printf("compare result is %d \n", HPR_Strcmp("12345AB", "12345"));
	printf("compare result is %d \n", HPR_Strcmp("12345", "12345AB"));
	printf("compare result is %d \n", HPR_Strcmp("AB12345", "12345"));
	printf("compare result is %d \n", HPR_Strcmp("12345", "AB12345"));
	printf("compare result is %d \n", HPR_Strcmp("ab12345", "AB12345"));
	
	
	printf("compare result is %d \n", strcmp("12345", "12345"));
	printf("compare result is %d \n", strcmp("12345", "AB12345"));
	printf("compare result is %d \n", strcmp("ab12345", "AB12345"));
}

void test_str_strncmp()
{
	printf("compare result is %d \n", HPR_Strncmp("12345", "12345", 5));
	printf("compare result is %d \n", HPR_Strncmp("12345AB", "12345", 5));
	printf("compare result is %d \n", HPR_Strncmp("12345", "12345AB", 5));
	printf("compare result is %d \n", HPR_Strncmp("AB12345", "12345", 5));
	printf("compare result is %d \n", HPR_Strncmp("12345", "AB12345", 5));
	printf("compare result is %d \n", HPR_Strncmp("ab12345", "AB12345", 5));
}

void test_str_strcasecmp()
{
	printf("compare result is %d \n", HPR_Strcasecmp("12345", "12345"));
	printf("compare result is %d \n", HPR_Strcasecmp("12345AB", "12345"));
	printf("compare result is %d \n", HPR_Strcasecmp("12345", "12345AB"));
	printf("compare result is %d \n", HPR_Strcasecmp("AB12345", "12345"));
	printf("compare result is %d \n", HPR_Strcasecmp("12345", "AB12345"));
	printf("compare result is %d \n", HPR_Strcasecmp("ab12345", "AB12345"));
	
	
	printf("compare result is %d \n", strcmp("12345", "12345"));
	printf("compare result is %d \n", strcmp("12345", "AB12345"));
	printf("compare result is %d \n", strcmp("ab12345", "AB12345"));
}

void test_str_strncasecmp()
{
	printf("compare result is %d \n", HPR_Strncasecmp("12345", "12345", 5));
	printf("compare result is %d \n", HPR_Strncasecmp("12345AB", "12345", 5));
	printf("compare result is %d \n", HPR_Strncasecmp("12345", "12345AB", 5));
	printf("compare result is %d \n", HPR_Strncasecmp("AB12345", "12345", 5));
	printf("compare result is %d \n", HPR_Strncasecmp("12345", "AB12345", 5));
	printf("compare result is %d \n", HPR_Strncasecmp("ab12345", "AB12345", 5));
}

void test_str_strchr()
{
	printf("%s, %c\n", HPR_Strchr("dfasjkl29030fdskj.,.",'2'),'2');
}

void test_str_strnchr()
{
	printf("%s, %c\n", HPR_Strnchr("dfasjkl29030fdskj.,.",'2',5),'2');
}

void test_str_strstr()
{
	printf("%s, %s\n", HPR_Strstr("dfasjkl29030fdskj.,.","29"),"29");
}

void test_str_strnstr()
{
	printf("%s, %s\n", HPR_Strnstr("dfasjkl29030fdskj.,.","29",5),"29");
}

void test_str_strcasestr()
{
	printf("%s, %s\n", HPR_Strcasestr("dfasjkl29030fdskj.,.","JKj"),"JK"); }

