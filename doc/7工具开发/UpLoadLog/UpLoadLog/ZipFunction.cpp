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
			// ����ߵ�ִֻ��һ��
			if (!strDestPath.IsEmpty())
			{
				// �����ѹ·���ļ��в����� �ȴ���,���� �����κ��޸�
				SHCreateDirectoryEx(NULL, lpszDestPath, NULL);
				UPLOAD_LOG_INFO("����·��");
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
			// �����һ���ļ�Ŀ¼
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
				// �������ļ��� �ݹ����
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
			UPLOAD_LOG_ERROR("ѹ��ʧ��,������%d",zResult);
		}
		g_nCount = 0;

		return zResult;
	}
}