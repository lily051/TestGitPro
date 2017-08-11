#pragma once
#include <map>
#include "FuiBase.h"
#include "zip/unzip.h"
//#include "zip/XUnzip.h"

#define WRITEBUFFERSIZE (16384)

class CResMgr
{
public:
	CResMgr(void);
	~CResMgr(void);

    static CResMgr * GetInstance();

private:
	//CString m_strResPath;

	CRITICAL_SECTION m_csResMap;
	std::map<CString, Image*> m_mapRes;
	std::map<Image*, CBitmap*> m_mapResCBmp;

    static CResMgr *m_pResMgr;

	//HZIP m_hZip;

	CString m_strPassword;
    unzFile m_uzf;
	BOOL GetFile(const CString& strName, CString& strPassword, char** pFileBuffer, int& nFileLen);
public:
	void SetRes(LPCTSTR lpszRes, LPCTSTR lpszPassword = NULL);
	Image* GetRes(const CString& strResName);
	CBitmap* GetResCBitmap(Image* pImage);

	void InitResMgr();
	void UninitResMgr();

};
