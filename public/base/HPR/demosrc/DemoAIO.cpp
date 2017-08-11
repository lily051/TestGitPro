/*
 * ====================================================================
 * Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
 *
 *        Filename:  DemoAIO.cpp
 * 
 *     Description:  测试AIO函数
 * 
 *          Author:  tyu, yutao NOSPAM hikvision DOT com
 *
 *            Date:  2009-10-21 15:28:20 中国标准时间
 *
 * Modification History:
 *            <version > <time> <author> <desc>
 * ====================================================================
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "HPR_Config.h"
#include "HPR_Types.h"
#include "HPR_IOCompletionQ.h"
#include "HPR_AlignMem.h"
#include "HPR_Thread.h"
#include "HPR_FileSystem.h"

#include <iostream>
using namespace std;

#define FILES_NUM   100
#define FILE_SIZE   (32 * 1024 * 1024)
#define BUFFER_SIZE (1024 * 1024)

#define FILE_PATH   "/mnt/sdb5"

typedef struct _ioTest
{
    OVERLAPPED  asynchronous_io;
    HPR_HANDLE  hFile;
    HPR_UINT32  dwPos;
    time_t      tBeginTime;
    HPR_UINT32  dwFileNo;
    HPR_UINT32  dwLen;
}IOTEST, *PIOTEST;

// 从1开始
IOTEST g_ioTestInfo[FILES_NUM * 2];

// 完成端口定义
static HPR_HANDLE cq = NULL;
static HPR_INT32 g_Test = 0;

static HPR_VOIDPTR IOTestThread(HPR_VOIDPTR argptr)
{
    int cnt = 0;
    char* dataBuf = (char*)HPR_AlignedMalloc(BUFFER_SIZE, 4096);

    memset(dataBuf, 'A', BUFFER_SIZE);

    //time_t t1 = time(NULL);
    while(g_Test)
    {
        if(cnt == FILES_NUM * 2)
        {
            //fprintf(stderr, "Speed(:%u m/s), Time(:%d)\n", ((HPR_UINT32)(FILES_NUM * 2 * FILE_SIZE)) / (time(NULL) - t1));
            HPR_AlignedFree(dataBuf);
            HPR_Thread_Exit();
            return NULL;
        }

        //int begin_time = time(NULL);
        int i = 0;
        for (i = 0; i < FILES_NUM; i++)
        {
            if(g_ioTestInfo[i].dwLen >= FILE_SIZE)
            {
                continue;
            }

            g_ioTestInfo[i].asynchronous_io.hEvent = NULL;
            g_ioTestInfo[i].asynchronous_io.Internal = (HPR_UINT32)i;
            g_ioTestInfo[i].asynchronous_io.Offset = g_ioTestInfo[i].dwPos;
            g_ioTestInfo[i].asynchronous_io.OffsetHigh = 0;

            int retVal = HPR_AIOWriteFile(g_ioTestInfo[i].hFile, dataBuf, BUFFER_SIZE, NULL, &g_ioTestInfo[i].asynchronous_io);
            if(retVal < 0)
            {
                fprintf(stderr, "HPR_AIOWriteFile return errno: %d\n", errno);
            }

            g_ioTestInfo[i].dwPos += BUFFER_SIZE;
        }

        for (i = FILES_NUM; i < FILES_NUM * 2; i++)
        {
            if(g_ioTestInfo[i].dwLen >= FILE_SIZE)
            {
                continue;
            }

            g_ioTestInfo[i].asynchronous_io.hEvent = NULL;
            g_ioTestInfo[i].asynchronous_io.Internal = (HPR_UINT32)i;
            g_ioTestInfo[i].asynchronous_io.Offset = g_ioTestInfo[i].dwPos;
            g_ioTestInfo[i].asynchronous_io.OffsetHigh = 0;

            int retVal = HPR_AIOWriteFile(g_ioTestInfo[i - FILES_NUM].hFile, dataBuf, BUFFER_SIZE, NULL, &g_ioTestInfo[i].asynchronous_io);
            if(retVal < 0)
            {
                fprintf(stderr, "HPR_AIOWriteFile return errno: %d\n", errno);
                sleep(10);
            }
            
            g_ioTestInfo[i].dwPos += BUFFER_SIZE;
        }

        HPR_UINT32 bytes_transferred = 0;
        HPR_UINT32 completion_key = 0;
        LPOVERLAPPED pIOTest = NULL;
        for (i = 0; i < FILES_NUM * 2; i++)
        {
            HPR_BOOL result;
            //HPR_UINT32 error_no;

            result = HPR_AIOGetQueuedCompletionStatus(cq, &bytes_transferred, &completion_key,  &pIOTest, HPR_INFINITE);

            // Set the return data to the transaction id that completed or failed.
            if (result)
            {
                //fprintf(stderr, "Write File: Number(%d), Bytes(:%d), Len(:%d)\n", 
                //        pIOTest->Internal, bytes_transferred, g_ioTestInfo[pIOTest->Internal].dwLen);
            }

            if (result == HPR_FALSE && pIOTest)
            {
                //fprintf(stderr, "Write File: Number(%d), Bytes(:%d), Len(:%d) errno(:%d)\n", 
                //        pIOTest->Internal, bytes_transferred, g_ioTestInfo[pIOTest->Internal].dwLen, errno);
                continue;
            }

            if (pIOTest)
            {
                g_ioTestInfo[pIOTest->Internal].dwLen += bytes_transferred;
                if (g_ioTestInfo[pIOTest->Internal].dwLen >= FILE_SIZE)
                {
                    cnt++;
                    //g_ioTestInfo[pIOTest->Internal].dwPos = 0;
                    //g_ioTestInfo[pIOTest->Internal].dwLen = 0;

                   // fprintf(stderr, "File Number(%d) Complete, Cost Time(%d s), Len(%d), Complete(%d)\n", g_ioTestInfo[pIOTest->Internal].dwFileNo,
                    //        time(NULL) - g_ioTestInfo[pIOTest->Internal].tBeginTime, g_ioTestInfo[pIOTest->Internal].dwLen, cnt);
					cout << "File Number(" << g_ioTestInfo[pIOTest->Internal].dwFileNo
						<< ") Complete, Cost Time("<< g_ioTestInfo[pIOTest->Internal].tBeginTime
						<< "), Len(" << g_ioTestInfo[pIOTest->Internal].dwLen
						<< "), Complete(" << cnt << ")" << endl;
                }
            }

            //fprintf(stderr, "Actually Write 2M Data for %d files\n", FILES_NUM);
        }
    }

	return 0;
}

int Test_AIO()
{
    // 创建完成端口
    cq = HPR_AIOCreateIoCompletionPort(HPR_INVALID_FILE, NULL, 0, 1);
    if(cq == HPR_INVALID_FILE)
    {
        fprintf(stderr, "Create AIO CompletionPort Failed for errno: %d\n", errno);
        return -1;
    }

    int i = 0;
    char pzFileName[60] = {0};
    HPR_HANDLE fpHandle = NULL;
    for (i = 0; i < FILES_NUM; i++)
    {
        int fd = -1;
        sprintf(pzFileName, "%s/Rec%05d.mp4", FILE_PATH, i);
        
        HPR_HANDLE hFile = HPR_AIOOpenFile(pzFileName, HPR_WRITE | HPR_CREATE | HPR_DIRECT, HPR_UREAD | HPR_UWRITE);
        if(hFile != HPR_INVALID_FILE)
        {
            fd = *(int*)hFile;
            lseek(fd, 0x0, SEEK_SET);
        }

        fpHandle = HPR_AIOCreateIoCompletionPort(hFile, cq, 0, 0);
        if(fpHandle == HPR_INVALID_FILE)
        {
            fprintf(stderr, "Create AIO Attached to the file: %d failed\n", fd);
            return -1;
        }

        g_ioTestInfo[i].dwFileNo= i;
        g_ioTestInfo[i].hFile   = hFile;
        g_ioTestInfo[i].dwPos   = 0;
        g_ioTestInfo[i].tBeginTime = time(NULL);

        g_ioTestInfo[i + FILES_NUM].dwFileNo= i;
        g_ioTestInfo[i + FILES_NUM].hFile   = hFile;
        g_ioTestInfo[i + FILES_NUM].dwPos   = 0;
        g_ioTestInfo[i + FILES_NUM].tBeginTime = time(NULL);
    } 
    
    g_Test = 1;
    HPR_HANDLE retHandle = HPR_Thread_Create(IOTestThread, NULL, 0);
    HPR_Thread_Wait(retHandle);

    return 0;
}

#if 0

int main(int argc, char* argv[])
{
    return Test_AIO();
}

#endif


