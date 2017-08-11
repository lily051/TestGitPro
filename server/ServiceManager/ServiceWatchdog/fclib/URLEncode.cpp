//lint -library
#include "stdafx.h"
#include "URLEncode.h"

const char *encodeURI(const char *Str)
{
	wchar_t *Bufw = NULL;
	char *Bufc = NULL;
	char RTV[5120];
	long needSize = MultiByteToWideChar(CP_ACP,
		NULL,
		Str,
		-1,
		NULL,
		0);
	if ( 0 == needSize ) goto ERROR_HANDLE;

	Bufw = new wchar_t[needSize];
	if ( NULL == Bufw ) goto ERROR_HANDLE;

	memset(Bufw,0x0,needSize*2);
	MultiByteToWideChar(CP_ACP,
		NULL,
		Str,
		-1,
		Bufw,
		needSize);

	needSize = WideCharToMultiByte(CP_UTF8,
		NULL,
		Bufw,
		-1,
		NULL,
		0,
		NULL,
		NULL);
	if ( 0 == needSize ) goto ERROR_HANDLE;

	Bufc = new char[needSize];
	if ( NULL == Bufc ) goto ERROR_HANDLE;

	memset(Bufc,0x0,needSize);
	WideCharToMultiByte(CP_UTF8,
		NULL,
		Bufw,
		-1,
		Bufc,
		needSize,
		NULL,
		NULL);

	unsigned char *pWork = (unsigned char *)Bufc;
	memset(RTV,0x0,sizeof(RTV));
	if ( strlen(Bufc) > 5120 )
	{ goto ERROR_HANDLE; }
	while( *pWork != 0x0 )
	{
		if ( *pWork != '!' && *pWork != '@' && *pWork != '#' &&
			*pWork != '$' && *pWork != '&' && *pWork != '*' &&
			*pWork != '(' && *pWork != ')' && *pWork != '=' &&
			*pWork != ':' && *pWork != '/' && *pWork != ';' &&
			*pWork != '?' && *pWork != '+' && *pWork != '\'' &&
			*pWork != '.' )
		{
			sprintf(RTV+strlen(RTV),"%%%2X",*pWork);
		}
		else
		{
			sprintf(RTV+strlen(RTV),"%c",*pWork);
		}
		pWork++;
	}

	if ( NULL != Bufw )
	{
		delete [] Bufw;
		Bufw = NULL;
	}
	if ( NULL != Bufc )
	{
		delete [] Bufc;
		Bufc = NULL;
	}
	return RTV;

ERROR_HANDLE:

	if ( NULL != Bufw )
	{
		delete [] Bufw;
		Bufw = NULL;
	}
	if ( NULL != Bufc )
	{
		delete [] Bufc;
		Bufc = NULL;
	}
	return NULL;
}