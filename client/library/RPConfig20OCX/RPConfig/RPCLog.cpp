//lint -library
#include "stdafx.h"
#include "RPCLog.h"

void RPC_PRINT(HLOG_LEVEL nLogType, const char* pFile, int nLine, const char* pFunction, const TCHAR *fmt, ...)
{
	va_list vl;
	va_start(vl, fmt);
	TCHAR* pBuff = NULL;
	int nLength = _vsctprintf(fmt, vl) + 1;
	if (nLength <= 0)
	{
		return;
	}
	pBuff = new TCHAR[nLength * sizeof(TCHAR)];
	if (NULL == pBuff)
	{
		return;
	}
	memset(pBuff, 0, sizeof(TCHAR) * nLength);
	_vsntprintf_s(pBuff, nLength, _TRUNCATE, fmt, vl);

	CString strMsg;
	strMsg.Format(_T("@z %s"), pBuff);
	OutputDebugString(strMsg);

	va_end(vl);
	delete []pBuff;
	pBuff = NULL;

#if (defined _UNICODE) || (defined UNICODE)
	CStringA strFor(strMsg);
	hlog_format(nLogType, RPConfig_OBJECT_NAME, "<%s [%d] - %s> <%s>",  pFile, nLine, pFunction, strFor);
#else
	hlog_format(nLogType, RPConfig_OBJECT_NAME, "<%s [%d] - %s> <%s>",  pFile, nLine, pFunction, strMsg);
#endif
}