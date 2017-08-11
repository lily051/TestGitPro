#include "StdAfx.h"
#include "ZipFunction.h"
#include <io.h>
int g_nCount = 0;
namespace ZipUtils
{ 
	//g_nCount = 0;
	ZRESULT DirToZip(LPCTSTR lpszSrcPath, LPCTSTR lpszZipName, HZIP& hz, LPCTSTR lpszDestPath)
	{
		static CString strFileName;
		g_nCount++;
		DWORD zResult = ZR_OK;
		TCHAR buffer[MAX_PATH] = {0};

		CString strDestPath = lpszDestPath;

		if (g_nCount == 1)
		{
			// 这里边的只执行一次
			if (!strDestPath.IsEmpty())
			{
				// 如果解压路径文件夹不存在 先创建,存在 不做任何修改
				SHCreateDirectoryEx(NULL, lpszDestPath, NULL);
				UPLOAD_LOG_INFO("创建路径");
			}
			else
			{
				GetCurrentDirectory(MAX_PATH, (LPTSTR)&buffer);
				strDestPath = buffer;
				SHCreateDirectoryEx(NULL, strDestPath, NULL);
			}
			hz = CreateZip((CString)strDestPath+_T("\\")+(CString)lpszZipName, 0);
		}

		HANDLE file;
		WIN32_FIND_DATA fileData;
		file = FindFirstFile((CString)lpszSrcPath+_T("\\*.*"), &fileData);
		FindNextFile(file, &fileData);
		while (FindNextFile(file, &fileData))
		{
			// 如果是一个文件目录
			if(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (strFileName.IsEmpty())
				{
					ZipAddFolder(hz, fileData.cFileName);
				}
				else
				{
					ZipAddFolder(hz, strFileName+_T("\\")+fileData.cFileName);
				}

				strFileName = fileData.cFileName;
				// 存在子文件夹 递归调用
				DirToZip((CString)lpszSrcPath+_T("\\")+ fileData.cFileName, lpszZipName, hz, lpszDestPath);
				strFileName.Empty();
			}
			else
			{
				CString strTempPath;
				strTempPath.Format(_T("%s\\%s"), (CString)lpszSrcPath, fileData.cFileName);
				if (strFileName.IsEmpty())
				{
					ZipAdd(hz, fileData.cFileName, strTempPath);
				}
				else
				{
					ZipAdd(hz, strFileName+_T("\\")+fileData.cFileName, strTempPath);
				}

				if (zResult != ZR_OK)
				{
					return zResult;
				}
			}
		}

		return zResult;
	}

	ZRESULT CompressDirToZip(LPCTSTR lpszSrcPath, LPCTSTR lpszZipName, LPCTSTR lpszDestPath)
	{
		HZIP hz;
		DWORD zResult = ZR_OK;
		zResult = DirToZip(lpszSrcPath, lpszZipName,hz, lpszDestPath);
		if(zResult == ZR_OK)
		{
			CloseZip(hz);
		}
		else
		{
			UPLOAD_LOG_ERROR("压缩失败,错误码%d",zResult);
		}
		g_nCount = 0;

		return zResult;
	}
}