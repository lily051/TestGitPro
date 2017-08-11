#pragma once

#include "zlib/zlib.h"
#include "zlib/zip.h"
#include "zlib/unzip.h"
#include "zlib/iowin32.h"

#define WRITEBUFFERSIZE (16384)
#define MAXFILENAME (256)

#include "DataDef.h"

class zlibHelper
{
public:
	zlibHelper(void);
	~zlibHelper(void);
	//////////////////////////////////////////////////////////////////////////
	// ����ļ�
	BOOL AddFile(const RES_FILE& resFile);
	BOOL ClearFile();
	// ѹ����zip�ļ�
	BOOL ZipFile(const char* lpZipFile, const char* lpPassword = NULL);

	//////////////////////////////////////////////////////////////////////////

	BOOL UnzipFile(CString& strZipFile, CString& strExtractFolder, const char* lpszPassword);

	BOOL SaveFile(CString& strFile, char* pFileBuff, const int nFileLen);

	BOOL OpenRc(const CString& strRcPath);
	BOOL CloseRc();
	BOOL GetRcInfoList(RC_INFO& rcInfoList);
	BOOL AddToRcInfoList(RC_INFO& rcInfoList, const CString& strName);
	BOOL GetFile(const CString& strName, CString& strPassword, char** pFileBuff, int& nFileLen);
private:
	// ѹ���ļ�
	CRITICAL_SECTION m_csLock;
	std::vector<RES_FILE> m_vecFiles;

	// ��ѹ���ļ�
	unzFile m_uzf;
	CString m_strZipFile;
};
