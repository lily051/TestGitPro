#include "StdAfx.h"
#include "zlibHelper.h"
#include "Guard.h"
#include "zlib/CRC32.h"
#include "ResPacker.h"

uLong filetime(const char* lpszFile, tm_zip * /*tmzip*/, uLong *dt)
{
	FILETIME fileTime;
	HANDLE hFind;
	WIN32_FIND_DATAA ff32;
	hFind = FindFirstFileA(lpszFile, &ff32);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		FileTimeToLocalFileTime(&(ff32.ftLastWriteTime), &fileTime);
		FileTimeToDosDateTime(&fileTime, ((LPWORD)dt) + 1, ((LPWORD)dt) + 0);
		FindClose(hFind);
		return 1;
	}
	return 0;
}

long GetFileCRC32(const std::string& strFileA)
{
	// ���ļ�����Ҫ����
	if (FILE_ATTRIBUTE_DIRECTORY == GetFileAttributesA(strFileA.c_str()))
	{
		return 0;
	}

	FILE* pFile = NULL;
	fopen_s(&pFile, strFileA.c_str(), "rb");
	if (NULL == pFile)
	{
		return 0;
	}
	int nBufSize = WRITEBUFFERSIZE;
	void *pBuffer = (void*)malloc(nBufSize);
	if (NULL == pBuffer)
	{
		fclose(pFile);
		return 0;
	}
	int nReadSize = 0;
	long nCrc32 = 0;
	do 
	{
		nReadSize = (int)fread(pBuffer, 1, nBufSize, pFile);
		if (nReadSize > 0)
		{
			nCrc32 = CRC32(nCrc32, (const unsigned char*)pBuffer, nReadSize);
		}
	} while (nReadSize > 0);

	free(pBuffer);

	return nCrc32;
}

zlibHelper::zlibHelper(void)
: m_uzf(NULL)
{
	::InitializeCriticalSection(&m_csLock);
}

zlibHelper::~zlibHelper(void)
{
	::DeleteCriticalSection(&m_csLock);
}

// ����ļ�
BOOL zlibHelper::AddFile(const RES_FILE& resFile)
{
    if (resFile.strFileName.length() > 34)
    {
        return FALSE;
    }
	CGuard lock(&m_csLock);
	for (size_t t = 0; t < m_vecFiles.size(); ++t)
	{
		if (m_vecFiles[t] == resFile)
		{
			return FALSE;
		}
	}
	m_vecFiles.push_back(resFile);

	return TRUE;
}

BOOL zlibHelper::ClearFile()
{
	m_vecFiles.clear();
	return TRUE;
}

// ѹ����zip�ļ�
BOOL zlibHelper::ZipFile(const char* lpszZipFile, const char* lpszPassword)
{
	std::string strZipFile(lpszZipFile);

	// ѹ������
	int nCompressLevel = Z_DEFAULT_COMPRESSION;
	// ����zip�ļ�
	zlib_filefunc_def ffunc;
	fill_win32_filefunc(&ffunc);
	zipFile zf = zipOpen2(strZipFile.c_str(), APPEND_STATUS_CREATE, NULL, &ffunc);
	if (NULL == zf)
	{
		//...
		return FALSE;
	}

	// �ļ���ȡ��Ļ���
	int nBufSize = WRITEBUFFERSIZE;
	void *pBuffer = (void*)malloc(nBufSize);
	if (NULL == pBuffer)
	{
		return FALSE;
	}
	
	for (size_t t = 0; t < m_vecFiles.size(); ++t)
	{
		RES_FILE& resFile = m_vecFiles[t];

		// �ļ���ʱ����Ϣ
		zip_fileinfo zi;
		zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
			zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
		zi.dosDate = 0;
		zi.internal_fa = 0;
		zi.external_fa = 0;
		filetime(resFile.strFilePath.c_str(), &zi.tmz_date, &zi.dosDate);

		FILE* pFile = NULL;
		// zip�д����ļ�
		int nRet = zipOpenNewFileInZip3(zf, resFile.strFileName.c_str(), &zi,
			NULL, 0, NULL, 0, NULL,
			(nCompressLevel != 0) ? Z_DEFLATED : 0,
			nCompressLevel, 0,
			-MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
			lpszPassword, GetFileCRC32(resFile.strFilePath));
		if (nRet != ZIP_OK)
		{
			//...
			continue;
		}

		fopen_s(&pFile, resFile.strFilePath.c_str(), "rb");
		if (NULL == pFile)
		{
			// ...
			continue;
		}
		// ��ȡ�ļ�����
		int nReadSize = 0;
		do 
		{
			nRet = ZIP_OK;
			nReadSize = (int)fread(pBuffer, 1, nBufSize, pFile);
			if (nReadSize < nBufSize)
			{
				if (feof(pFile)==0)
				{
					//printf("error in reading %s\n",filenameinzip);
					nRet = ZIP_ERRNO;
				}
			}
			if (nReadSize > 0)
			{
				nRet = zipWriteInFileInZip(zf, pBuffer, nReadSize);
				if (nRet < 0)
				{
// 					printf("error in writing %s in the zipfile\n",
// 						filenameinzip);
				}
			}
		} while ((nRet == ZIP_OK) && (nReadSize > 0));
		
		// �ر��ļ�
		if (pFile)
		{
			fclose(pFile);
		}
		
		nRet = zipCloseFileInZip(zf);
		if (nRet != ZIP_OK)
		{
			//...
		}
	}
	free(pBuffer);
	zipClose(zf, NULL);

	return TRUE;
}

BOOL zlibHelper::UnzipFile(CString& strZipFile, CString& strExtractFolder, const char* lpszPassword)
{
    std::string strZipFileA = fcT2A(strZipFile.GetBuffer());

	// ���ļ�
	m_uzf = unzOpen(strZipFileA.c_str());
	if (NULL == m_uzf)
	{
		TRACE("Open zip file %s failed\n", strZipFileA.c_str());
		return FALSE;
	}

	unz_global_info globalInfo;
	unzGetGlobalInfo(m_uzf, &globalInfo);

	// �����ļ����н�ѹ��
	int nRet = unzGoToFirstFile(m_uzf);
	if (UNZ_OK != nRet)
	{
		TRACE("Goto First file failed, %d\n", nRet);
		return FALSE;
	}

	// ѭ����ȡ�Ļ���
	void* pReadBuffer = (void*)malloc(WRITEBUFFERSIZE);
	do 
	{
		unz_file_info fileInfo;
		char szFileName[32] = {0};
		nRet = unzGetCurrentFileInfo(m_uzf, &fileInfo, szFileName, sizeof(szFileName), NULL, 0, NULL, 0);
		TRACE("Extract %s\n", szFileName);

		// ��ѹ���������ļ�
		nRet = unzOpenCurrentFilePassword(m_uzf, lpszPassword);
		if (UNZ_OK != nRet)
		{
			TRACE("Open zip-file %s failed, %d\n", szFileName, nRet);
			nRet = unzGoToNextFile(m_uzf);
			continue;
		}

		// ѹ��ǰ�ĵ���С(�ļ���СΪ0���հ׵��ļ���СҲΪ0����Ҫע�����֣� compression_methon����Ϊ0)
		int nFileSize = fileInfo.uncompressed_size;
		if (0 == nFileSize) // �ļ��С����ļ�
		{
			if (szFileName[fileInfo.size_filename - 1] == '/')
			{
				// �����ļ���
				CString strTemp = strExtractFolder + fcA2T(szFileName); 
				SuperMkDirs(strTemp);
				nRet = unzGoToNextFile(m_uzf);
				continue;
			}
		}
		char* pFileBuffer = (char*)malloc(nFileSize);
		if (NULL == pFileBuffer)
		{
			continue; 
		}
		int nCurrentReadSize = 0;
		int nReadSize = 0;
		do 
		{
			nReadSize = unzReadCurrentFile(m_uzf, pReadBuffer, WRITEBUFFERSIZE);
			if (nReadSize < 0)
			{
				TRACE("unzReadCurrentFile Error\n");
				break;
			}
			if (nReadSize > 0)
			{
				memcpy_s(pFileBuffer + nCurrentReadSize, nFileSize - nCurrentReadSize, pReadBuffer, nReadSize);
				nCurrentReadSize += nReadSize;
			}
		} while (nReadSize > 0);

		ASSERT(nCurrentReadSize == nFileSize);
		// ���̴����ļ�
		CString strFile = strExtractFolder + _T("\\") + fcA2T(szFileName);
		if (!SaveFile(strFile, pFileBuffer, nFileSize))
		{
			TRACE("Error\n");
		}
		free(pFileBuffer);

		nRet = unzGoToNextFile(m_uzf);
	} while (nRet == UNZ_OK);

	free(pReadBuffer);

	unzClose(m_uzf);

	return TRUE;
}

BOOL zlibHelper::SaveFile(CString& strFile, char* pFileBuff, const int nFileLen)
{
	if (NULL == pFileBuff)
	{
		return FALSE;
	}
	// ��ȷ���ļ����ڵ�Ŀ¼���ڣ�Ȼ����д���ļ�
	CString strTemp(strFile);
	strTemp.Replace(_T('/'), _T('\\'));
	CString strFilePath = strFile.Left(strFile.ReverseFind(_T('\\')));
	SuperMkDirs(strFilePath);

	FILE* pFile = NULL;
	fopen_s(&pFile, fcT2A(strFile.GetBuffer()), "wb");
	if (NULL == pFile)
	{
		TRACE("1231321\n");
		return FALSE;
	}

	fwrite(pFileBuff, 1, nFileLen, pFile);
	fclose(pFile);

	return TRUE;
}

BOOL zlibHelper::OpenRc(const CString& strRcPath)
{
	CloseRc();

	m_strZipFile = strRcPath;
	m_uzf = unzOpen(fcT2A(m_strZipFile.GetBuffer()));
	if (NULL == m_uzf)
	{
		TRACE("Open zip file %s failed\n", (LPCTSTR)m_strZipFile);
		return FALSE;
	}

	unz_global_info globalInfo;
	unzGetGlobalInfo(m_uzf, &globalInfo);

	return TRUE;
}

BOOL zlibHelper::CloseRc()
{
	if (m_uzf != NULL)
	{
		unzClose(m_uzf);
		m_uzf = NULL;
		m_strZipFile.Empty();
	}
	return TRUE;
}

BOOL zlibHelper::GetRcInfoList(RC_INFO& rcInfoList)
{
	int nPos1 = m_strZipFile.ReverseFind(_T('\\')) + 1;
	CString strRootName = m_strZipFile.Right(m_strZipFile.GetLength() - nPos1);
	rcInfoList.strName = strRootName;

	if (NULL == m_uzf)
	{
		return FALSE;
	}
	// �ṹ��
	int nRet = unzGoToFirstFile(m_uzf);
	if (UNZ_OK != nRet)
	{
		TRACE("Goto First file failed, %d\n", nRet);
		return FALSE;
	}

	do 
	{
		unz_file_info fileInfo;
		char szFileName[32] = {0};
		nRet = unzGetCurrentFileInfo(m_uzf, &fileInfo, szFileName, sizeof(szFileName), NULL, 0, NULL, 0);

		CString strFileName = fcA2T(szFileName);
		TRACE(L"%s\n", (LPCTSTR)strFileName);
		AddToRcInfoList(rcInfoList, strFileName);

		nRet = unzGoToNextFile(m_uzf);
	} while (nRet == UNZ_OK);

	return TRUE;
}

BOOL zlibHelper::AddToRcInfoList(RC_INFO& rcInfoList, const CString& strName)
{
	int nPos1 = strName.Find(_T('/'));
	if (nPos1 == -1)
	{
		// �ļ�
		RC_INFO rcInfo;
		rcInfo.strName = strName;
		rcInfo.nRcType = TYPE_IMAGE;

		rcInfoList.subRc.push_back(rcInfo);
	}
	else
	{
		// Ŀ¼ ���� Ŀ¼/.../�ļ�
		CString strSub1 = strName.Left(nPos1);
		CString strSub2 = strName.Right(strName.GetLength() - nPos1 - 1);

		if (strSub2.IsEmpty())	// Ŀ¼
		{
			// �ж��Ƿ���ڣ�
			RC_INFO rcInfo;
			rcInfo.strName = strSub1;
			rcInfo.nRcType = TYPE_FOLDER;

			rcInfoList.subRc.push_back(rcInfo);
		}
		else
		{
			// Ŀ¼/.../�ļ�
			// �� strSub1 Ȼ����� strSub2
			std::list<RC_INFO>::iterator itr = rcInfoList.subRc.begin();
			for (; itr != rcInfoList.subRc.end(); ++itr)
			{
				if (itr->strName == strSub1)
				{
					break;
				}
			}
			if (itr == rcInfoList.subRc.end())
			{
				// add one?
			}
			
			RC_INFO& subRcInfo = *itr;
			
			AddToRcInfoList(subRcInfo, strSub2);
		}
	}

	return TRUE;
}

BOOL zlibHelper::GetFile(const CString& strName, CString& strPassword, char** pFileBuffer, int& nFileLen)
{
	// ���zip�Ƿ��Ѿ���
	if (NULL == m_uzf)
	{
		return FALSE;
	}
	// ͨ���ļ�����λ
	CString strFileName = strName;
	std::string strNameA = fcT2A(strFileName.GetBuffer());
	int nRet = unzLocateFile(m_uzf, strNameA.c_str(), 1);
	if (nRet != UNZ_OK)
	{
		return FALSE;
	}

	// ��ȡѹ���ļ�����Ϣ
	unz_file_info fileInfo;
	char szFileName[32] = {0};
	nRet = unzGetCurrentFileInfo(m_uzf, &fileInfo, szFileName, sizeof(szFileName), NULL, 0, NULL, 0);
	if (nRet != UNZ_OK)
	{
		return FALSE;
	}

	// �򿪸��ļ�
	std::string strPasswordA = fcT2A(strPassword.GetBuffer());
	if (strPasswordA.empty())
	{
		nRet = unzOpenCurrentFilePassword(m_uzf, 0);
	}
	else
	{
		nRet = unzOpenCurrentFilePassword(m_uzf, strPasswordA.c_str());
	}
	if (UNZ_OK != nRet)
	{
		TRACE("Open zip-file %s failed, %d\n", szFileName, nRet);
		return FALSE;
	}

	void* pReadBuffer = (void*)malloc(WRITEBUFFERSIZE);
	// ѹ��ǰ�ĵ���С(�ļ���СΪ0���հ׵��ļ���СҲΪ0����Ҫע�����֣� compression_methon����Ϊ0)
	nFileLen = fileInfo.uncompressed_size;
	*pFileBuffer = (char*)malloc(nFileLen);
	int nCurrentReadSize = 0;
	int nReadSize = 0;
	do 
	{
		nReadSize = unzReadCurrentFile(m_uzf, pReadBuffer, WRITEBUFFERSIZE);
		if (nReadSize < 0)
		{
			TRACE("unzReadCurrentFile Error\n");
			break;
		}
		if (nReadSize > 0)
		{
			memcpy_s(*pFileBuffer + nCurrentReadSize, nFileLen - nCurrentReadSize, pReadBuffer, nReadSize);
			nCurrentReadSize += nReadSize;
		}
	} while (nReadSize > 0);
	
	free(pReadBuffer);

	if (nCurrentReadSize != nFileLen)
	{
		return FALSE;
	}

	return TRUE;
}

