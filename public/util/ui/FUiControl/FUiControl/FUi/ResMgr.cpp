#include "stdafx.h"
#include "ResMgr.h"
#include "Guard.h"
#include "fuilib.h"
#include <shlwapi.h>
#include <assert.h>

CResMgr::CResMgr()
: m_uzf(NULL)
{
}


CResMgr::~CResMgr()
{
}

CResMgr *CResMgr::GetInstance()
{
    static CResMgr _Instance;

    return &_Instance;
}

BOOL CResMgr::GetFile(const CString& strName, CString& strPassword, char** pFileBuffer, int& nFileLen)
{
	// 检查zip是否已经打开
	if (NULL == m_uzf)
	{
        TRACE("zip-file is not opened, %d\n");
		return FALSE;
	}

	// 通过文件名定位
	CString strFileName = strName;
	std::string strNameA = fcT2A(strFileName.GetBuffer());
	int nRet = unzLocateFile(m_uzf, strNameA.c_str(), 1);
	if (nRet != UNZ_OK)
	{
		return FALSE;
	}

	// 获取压缩文件的信息
	unz_file_info fileInfo;
	char szFileName[32] = {0};
	nRet = unzGetCurrentFileInfo(m_uzf, &fileInfo, szFileName, sizeof(szFileName), NULL, 0, NULL, 0);
	if (nRet != UNZ_OK)
	{
		return FALSE;
	}

	// 打开该文件
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
	// 压缩前文档大小(文件大小为0，空白的文件大小也为0，需要注意区分， compression_methon都是为0)
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

void CResMgr::SetRes(LPCTSTR lpszRes, LPCTSTR lpszPassword)
{
    if (PathFileExists(lpszRes))
    {
//         m_hZip = OpenZip((void*)lpszRes, 0, ZIP_FILENAME);
//         if (NULL == m_hZip)
//         {
//             TRACE("Open skin File fail");
//             return ;
//         }

        // Zip文件已打开，保证不打开两次
        if (NULL != m_uzf)
        {
            TRACE("zip file %s had been opened \n", lpszRes);
            return;
        }

		m_uzf = unzOpen(fcT2A(lpszRes));
		if (NULL == m_uzf)
		{
			TRACE("Open zip file %s failed\n", lpszRes);
			return ;
		}
		unz_global_info globalInfo;
		unzGetGlobalInfo(m_uzf, &globalInfo);

		m_strPassword = lpszPassword;
    }
}

Image* CResMgr::GetRes(const CString& strResName)
{
    CString strResName_ = strResName;
    strResName_.Replace(_T('/'), _T('\\'));

    CGuard lock(&m_csResMap);
    if (m_mapRes.find(strResName_) != m_mapRes.end())
    {
        return m_mapRes[strResName_];
    }

    CString strResNameInZip = strResName_ + _T(".png");
	char* pBuffer = NULL;
	int nBufferLen = 0;
    // 从zip中查找文件
	//ZIPENTRYW ze;
	//int index = 0;
	//ZRESULT zr = FindZipItem(m_hZip, strResNameInZip, TRUE, &index, &ze);
	//if (zr != ZR_OK)
	//{
	//	return NULL;
	//}
	//nBufferLen = ze.unc_size;

	//pBuffer = new BYTE[nBufferLen];
	//UnzipItem(m_hZip, index, pBuffer, ze.unc_size, ZIP_MEMORY);

	if (FALSE == GetFile(strResNameInZip, m_strPassword, &pBuffer, nBufferLen))
	{
		return FALSE;
	}

    Image *pImg = mi_from_memory(pBuffer, nBufferLen);
    if (NULL == pImg)
    {
        return NULL;
    }

	// CBitmap
	HBITMAP hBitmap = cmi_from_memory(pBuffer, nBufferLen);
	CBitmap *pCBitmap = new CBitmap;
	pCBitmap->Attach(hBitmap);

	free(pBuffer);

    m_mapRes[strResName_] = pImg;
	m_mapResCBmp[pImg] = pCBitmap;

    return pImg;
}

CBitmap* CResMgr::GetResCBitmap(Image* pImage)
{
	CGuard lock(&m_csResMap);
	if (m_mapResCBmp.find(pImage) != m_mapResCBmp.end())
	{
		return m_mapResCBmp[pImage];
	}
	return NULL;
}

void CResMgr::InitResMgr()
{
	InitializeCriticalSection(&m_csResMap);
}

void CResMgr::UninitResMgr()
{
// 	if (m_hZip)
// 	{
// 		CloseZip(m_hZip);
// 		m_hZip = NULL;
// 	}
	if (NULL != m_uzf)
	{
		unzClose(m_uzf);
		m_uzf = NULL;
	}

	m_mapRes.clear();

	// CBitmap
	for (std::map<Image*, CBitmap*>::iterator itr = m_mapResCBmp.begin();
		itr != m_mapResCBmp.end(); ++itr)
	{
		if (itr->second)
		{
			itr->second->DeleteObject();
			delete itr->second;
		}
	}
	m_mapResCBmp.clear();

	DeleteCriticalSection(&m_csResMap);
}

