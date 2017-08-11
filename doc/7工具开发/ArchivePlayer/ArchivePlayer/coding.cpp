#include "StdAfx.h"
#include "coding.h"

coding::coding(void)
{
	pResult = NULL;
    pwText  = NULL;
}

coding::~coding(void)
{
	if(NULL != pResult)
	{
		delete[] pResult;
		pResult = NULL;
	}

    if(NULL != pwText)
    {
        delete[] pwText;
        pwText = NULL;
    }
}
