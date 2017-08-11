#pragma once

//lint -library

#include <fstream>
#pragma warning(push)
#pragma warning(disable:4996)
#include "BugslayerUtil.h"
#pragma warning(pop)


/**********************************
Copyright 2007--2070 hikvision digital technology co..Ltd.
FileName:UnhandledExceptionFilter.h
Description:Get crash info abaut stack and CPU Register
Author:cao hua dong
Date:2007-12-06
Modification History:
**********************************/

class CUnhandledExceptionFilter
{
public:
	//构造函数
	CUnhandledExceptionFilter()
	{
		TCHAR szPath[MAX_PATH];
		GetModuleFileName(NULL, szPath, MAX_PATH);

		CString csPath(szPath);
		FileName() = csPath.Left(csPath.ReverseFind(_T('.'))) + _T("_exp.log"); 
        
		ProcessID().Format(_T("PID:%08x "), GetCurrentProcessId());

		std::ofstream fout(FileName(), std::ios_base::out | std::ios_base::app);
		fout.seekp(std::ios_base::end);

		CString strTemp = CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S ")) + ProcessID() + _T("Start");
		fout << fcT2A(strTemp) << std::endl;
		fout.close();

		//设置异常处理过滤器
		SetCrashHandlerFilter(_UnhandledExceptionFilter);
	}

	//析构函数
	virtual ~CUnhandledExceptionFilter()
	{
		std::ofstream fout(FileName(), std::ios_base::out | std::ios_base::app);
		fout.seekp(std::ios_base::end);
		
		CString strTemp = CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S ")) + ProcessID() + _T("End");
		fout << fcT2A(strTemp) << std::endl;
		fout.close();
	}

	//获取异常的堆栈信息
	static CString GetCrashInfoString(PEXCEPTION_POINTERS pExPtrs)
	{
		CString csExceptLogs, csTemp;
		
		//获取出现异常的原因
		csExceptLogs = csExceptLogs + GetFaultReason ( pExPtrs ) + _T("\r\n") ;
		
		//获取出现异常时CPU寄存器状态信息
		csExceptLogs = csExceptLogs + GetRegisterString ( pExPtrs ) + _T("\r\n") ;
		

		//枚举调用堆栈
		const TCHAR * szBuff = GetFirstStackTraceString ( GSTSO_PARAMS | GSTSO_MODULE | GSTSO_SYMBOL | GSTSO_SRCLINE, pExPtrs  ) ;
		do
		{
			csExceptLogs = csExceptLogs + szBuff + _T("\n");
			szBuff = GetNextStackTraceString ( GSTSO_PARAMS | GSTSO_MODULE | GSTSO_SYMBOL | GSTSO_SRCLINE , pExPtrs ) ;
		}
		while ( NULL != szBuff ) ;//end while( NULL != szBuff )

		return csExceptLogs;
	}
private:

	//异常处理过滤器
	static LONG __stdcall _UnhandledExceptionFilter(PEXCEPTION_POINTERS pExPtrs)
	{
        // add by xzh  2011:4:6 15:55:22
        // purpose: minidump, if stack_overflow do nothing
        if (EXCEPTION_STACK_OVERFLOW 
            != pExPtrs->ExceptionRecord->ExceptionCode )
        {
            WriteMinidump(pExPtrs);
        }
        // ~minidump

		std::ofstream fout(FileName(), std::ios_base::out | std::ios_base::app);
		fout.seekp(std::ios_base::end);

		CString strTemp = CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S ")) + ProcessID() + (LPCTSTR)GetCrashInfoString(pExPtrs);
		fout << fcT2A(strTemp) << std::endl;
		fout.close();

		return EXCEPTION_EXECUTE_HANDLER;
	}

	//获取程序执行的绝对路径
	static CStringW& FileName()
	{
		static CStringW m_csFile;
		return m_csFile;
	}

    static CString& ProcessID(void)
    {
        static CString strCID;
        return strCID;
    }

	bool m_bRestart;
private:
    static void WriteMinidump(PEXCEPTION_POINTERS pExPtrs)
    {
        try
        {
            CString strFile;
            strFile = FileName();
            strFile = strFile.Left(strFile.ReverseFind('_'));
            SYSTEMTIME sysTime;
            //GetSystemTime(&sysTime);
            GetLocalTime(&sysTime);
            
            strFile.AppendFormat(_T("_%04d-%02d-%02d_%02d-%02d-%02d-%03d.dmp"),
                sysTime.wYear, sysTime.wMonth, sysTime.wDay,
                sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
            HANDLE hFile = CreateFile(strFile, // name of the write
                GENERIC_WRITE,          // open for writing
                0,                      // do not share
                NULL,                   // default security
                CREATE_ALWAYS,          // overwrite existing
                FILE_ATTRIBUTE_NORMAL,  // normal file
                NULL);                  // no attr. template

            if (INVALID_HANDLE_VALUE != hFile)
            {
	            MINIDUMP_EXCEPTION_INFORMATION expInfo;
	            expInfo.ThreadId = ::GetCurrentThreadId();
	            expInfo.ExceptionPointers = pExPtrs;
	            expInfo.ClientPointers = TRUE;
	            MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpNormal, 
                    &expInfo, NULL, NULL);
	            CloseHandle(hFile);
            }
        }
        catch (...)
        {
            std::ofstream fout(FileName(), std::ios_base::out | std::ios_base::app);
            fout.seekp(std::ios_base::end);
			CString strTemp = CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S ")) + ProcessID() + _T("write dump failed");
			fout << fcT2A(strTemp) << std::endl;
            fout.close();
        }
    }
};
