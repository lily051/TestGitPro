#include "DemoApp.h"

HPR_HANDLE g_hFile = NULL;

void g_OpenFile();
void g_CloseFile();
void g_ReadFile();
void g_WriteFile();
void g_FileStat();
int EnumFiles();

void Test_FileSystem()
{
	//g_OpenFile();
	//g_WriteFile();
	//g_ReadFile();
	//g_CloseFile();
	g_FileStat();

	//EnumFiles();
}

void g_OpenFile()
{
	g_hFile = NULL;
	g_hFile = HPR_OpenFile("./hpr_filesystem.test", HPR_READ|HPR_WRITE|HPR_CREATE, 0);
	if (g_hFile == HPR_INVALID_FILE)
	{
		fprintf(stderr, "DemoFileSystem.cpp, test g_OpenFile Error\n");
	}
}

void g_CloseFile()
{
	if (g_hFile != HPR_INVALID_FILE)
	{
		HPR_CloseFile(g_hFile);
		g_hFile = HPR_INVALID_FILE;
	}
}

void g_ReadFile()
{

}

void g_WriteFile()
{

}

char* TimeStr(HPR_TIME_T Time, char *pStrTime)
{

    HPR_TIME_EXP_T TimeExp;
	//HPR_ExpTimeFromTime(Time, 0, &TimeExp);
	HPR_ExpTimeFromTimeLocal(Time, &TimeExp);
	sprintf(pStrTime, "%04d-%02d-%02d %02d:%02d:%02d", TimeExp.tm_year+1900, TimeExp.tm_mon+1, TimeExp.tm_mday, TimeExp.tm_hour, TimeExp.tm_min, TimeExp.tm_sec);
    return pStrTime;

	/*struct tm t;
	time_t tt = Time;
	t = *localtime (&tt);
	sprintf(pStrTime, "%04d-%02d-%02d %02d:%02d:%02d", t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	return pStrTime;*/
}

void PrintFileType(HPR_FILETYPE_T iType)
{
	if (iType & HPR_TYPE_CHAR)
	{
		fprintf(stderr, "[Type]:HPR_TYPE_CHAR\n");
	}
	if (iType & HPR_TYPE_DISK)
	{
		fprintf(stderr, "[Type]:HPR_TYPE_DISK\n");
	}
	if (iType & HPR_TYPE_PIPE)
	{
		fprintf(stderr, "[Type]:HPR_TYPE_PIPE\n");
	}
	if (iType & HPR_TYPE_UNKNOWN)
	{
		fprintf(stderr, "[Type]:HPR_TYPE_UNKNOWN\n");
	}
	if (iType & HPR_TYPE_SOCK)
	{
		fprintf(stderr, "[Type]:HPR_TYPE_SOCK\n");
	}
	if (iType & HPR_TYPE_FIFO)
	{
		fprintf(stderr, "[Type]:HPR_TYPE_FIFO\n");
	}
	if (iType & HPR_TYPE_LINK)
	{
		fprintf(stderr, "[Type]:HPR_TYPE_LINK\n");
	}
	if (iType & HPR_TYPE_BLK)
	{
		fprintf(stderr, "[Type]:HPR_TYPE_BLK\n");
	}
	if (iType & HPR_TYPE_DIR)
	{
		fprintf(stderr, "[Type]:HPR_TYPE_DIR\n");
	}
	if (iType & HPR_TYPE_REG)
	{
		fprintf(stderr, "[Type]:HPR_TYPE_REG\n");
	}
}

void PrintFileAccess(unsigned int iAccess)
{
#if defined(_WIN32)
	if (iAccess & HPR_ATTR_READONLY)
	{
		fprintf(stderr, "[Access]:HPR_ATTR_READONLY\n");
	}
	if (iAccess & HPR_ATTR_HIDDEN)
	{
		fprintf(stderr, "[Access]:HPR_ATTR_HIDDEN\n");
	}
	if (iAccess & HPR_ATTR_WRITE)
	{
		fprintf(stderr, "[Access]:HPR_ATTR_WRITE\n");
	}
	if (iAccess & HPR_ATTR_EXECUTABLE)
	{
		fprintf(stderr, "[Access]:HPR_ATTR_EXECUTABLE\n");
	}
	if (iAccess & HPR_ATTR_SYSTEM)
	{
		fprintf(stderr, "[Access]:HPR_ATTR_SYSTEM\n");
	}
	if (iAccess & HPR_DELONCLOSE)
	{
		fprintf(stderr, "[Access]:HPR_DELONCLOSE\n");
	}
#else
    if (iAccess & HPR_UREAD)
	{
		fprintf(stderr, "[Access]:HPR_UREAD\n");
	}
	if (iAccess & HPR_UWRITE)
	{
		fprintf(stderr, "[Access]:HPR_UWRITE\n");
	}
	if (iAccess & HPR_UEXECUTE)
	{
		fprintf(stderr, "[Access]:HPR_UEXECUTE\n");
	}
	if (iAccess & HPR_GREAD)
	{
		fprintf(stderr, "[Access]:HPR_GREAD\n");
	}
	if (iAccess & HPR_GWRITE)
	{
		fprintf(stderr, "[Access]:HPR_GWRITE\n");
	}
	if (iAccess & HPR_GEXECUTE)
	{
		fprintf(stderr, "[Access]:HPR_GEXECUTE\n");
	}
	if (iAccess & HPR_WREAD)
	{
		fprintf(stderr, "[Access]:HPR_WREAD\n");
	}
	if (iAccess & HPR_WWRITE)
	{
		fprintf(stderr, "[Access]:HPR_WWRITE\n");
	}
	if (iAccess & HPR_WEXECUTE)
	{
		fprintf(stderr, "[Access]:HPR_WEXECUTE\n");
	}
#endif
}

int EnumFiles()
{
	char sDir[256] = {0};
#if defined(_WIN32)
	strcpy(sDir, "E:/");
#elif defined(__linux__)
    strcpy(sDir, "/etc/");
#endif
	HPR_HANDLE hFind = HPR_OpenDir(sDir);
	if (HPR_INVALID_HANDLE == hFind)
	{
		fprintf(stderr, "Open dir failed! err:%d\n", errno);
		return -1;
	}
	
	HPR_FILEFIND_INFO FileInfo;
	memset(&FileInfo, 0, sizeof(FileInfo));
	int iCount=0;
	while(HPR_FindFileInDir(hFind, &FileInfo) != HPR_ERROR)
	{
		iCount++;
		
		fprintf(stderr, "\n[%d]\n", iCount);
		fprintf(stderr, "[name]:%s\n", FileInfo.sFileName);
		PrintFileType(FileInfo.FileInfo.nFileType);
		//fprintf(stderr, "[Size]:%d\n", FileInfo.FileInfo.nSize);
		cout << "[Size]:" << (HPR_INT32)FileInfo.FileInfo.nSize << endl;
		PrintFileAccess(FileInfo.FileInfo.nProtection);
		
		char strCreateTime[256] = {0};
		char strAccessTime[256] = {0};
		char strWriteTime[256] = {0};
#if defined(_WIN32)
		
		fprintf(stderr, "[Create time]:%s\n[Access time]:%s\n[Modification time]:%s\n", TimeStr(FileInfo.FileInfo.iCreateTime, strCreateTime), \
			TimeStr(FileInfo.FileInfo.iAccessTime, strAccessTime), TimeStr(FileInfo.FileInfo.iWriteTime, strWriteTime));
#elif defined(__linux__)
		fprintf(stderr, "[Change time]:%s\n[Access time]:%s\n[Modification time]:%s\n", TimeStr(FileInfo.FileInfo.iCreateTime, strCreateTime), \
			TimeStr(FileInfo.FileInfo.iAccessTime, strAccessTime), TimeStr(FileInfo.FileInfo.iWriteTime, strWriteTime));
#endif
		
		//check exist
		if (sDir[strlen(sDir)-1] != '/' && sDir[strlen(sDir)-1] != '\\')
		{
			sDir[strlen(sDir)] = '/';
		}
		char sFilePath[256] = {0};
		sprintf(sFilePath, "%s%s", sDir, FileInfo.sFileName);
		if (HPR_AccessFile(sFilePath, HPR_F_OK) == HPR_OK)
		{
			fprintf(stderr, "[Exist]:%s exist!\n", sFilePath);
		}
		else
		{
			fprintf(stderr, "[Exist]%s not exist!\n", sFilePath);
		}
	}
	
	fprintf(stderr, "\n");
    HPR_CloseDir(hFind);
	
	return 0;
}


void g_FileStat()
{
	int nCount     = 0;
	HPR_FINFO_T stFileAttr = {0}; 

	HPR_HANDLE hFile = NULL;

	while(1)
	{
		hFile = HPR_OpenFile("C:\\filestat.txt", HPR_READ, 0);

		HPR_FileStat(hFile, &stFileAttr); ///< 注释此行代码，不会出现内存增长，或者全部采用fopen fstat fclose 也不会产生内存增长。

		//Sleep(10);

		HPR_CloseFile(hFile);

		/* 调整至100ms，内存泄露会很快出现大概一百次泄露4K，每次4K递增，若设置为3000ms，运行一晚上(大约小时)出现464K增长*/
		Sleep(100); 

		nCount++;

		printf(" cur :%d \n", nCount);
	}

	return;
}
