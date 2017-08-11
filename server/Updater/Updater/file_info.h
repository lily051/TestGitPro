#ifndef _UPDATER_FILE_INFO_H_
#define _UPDATER_FILE_INFO_H_
#endif //_UPDATER_FILE_INFO_H_

class file_info
{
public:
	// ȡ�ó������е�Ŀ¼
	// bEndWithBackSlash���Ƿ��Է�б�߽�β
	static CString GetAppDirectory(BOOL bEndWithBackSlash = TRUE)
	{
		wchar_t acBuffer[MAX_PATH];

		memset((void *)acBuffer, 0, MAX_PATH);
		GetModuleFileName(AfxGetInstanceHandle(), acBuffer, sizeof(acBuffer));

		wchar_t *p = wcsrchr(acBuffer, '\\');
		if (p != NULL)
		{
			if (bEndWithBackSlash)
			{
				*(p + 1) = '\0';
			}
			else
			{
				*p = '\0';
			}
		}
		else
		{
			p = wcsrchr(acBuffer, ':');
			if (p != NULL)
			{
				if (bEndWithBackSlash)
				{
					*(p + 1) = '\\';
					*(p + 2) = '\0';
				}
				else
				{
					*p = '\\';
					*(p + 1) = '\0';
				}
			}
		}

		return CString(acBuffer);
	}

	// ���ļ�ȫ·��ȡ���ļ���
	static CString GetFilename(const CString &sFilePath)
	{
		int iPos = sFilePath.ReverseFind('\\');
		if (iPos == -1)
		{
			return sFilePath;
		}
		else
		{
			return sFilePath.Mid(iPos + 1);
		}
	}

	// ȡ���ļ���չ��������'.'�ַ���
	static CString GetFileExtendName(const CString& sFilename)
	{
		int iPos = sFilename.ReverseFind('.');
		if (iPos == -1)
		{
			return CString(_T(""));
		}
		else
		{
			return sFilename.Mid(iPos + 1);
		}
	}

	// ȡ���ļ���С
	static size_t GetFileSize(const CString& sFilename)
	{
		CFileFind FileFinder;

		if (FileFinder.FindFile(sFilename))
		{
			FileFinder.FindNextFile();
			return (size_t)FileFinder.GetLength();
		}
		else
		{
			return 0;
		}
	}
};
