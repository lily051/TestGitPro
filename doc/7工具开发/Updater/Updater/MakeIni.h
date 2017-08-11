#ifndef _UPDATER_MAKEINI_H_
#define _UPDATER_MAKEINI_H_
#endif //_UPDATER_MAKEINI_H_

#pragma once
#include "stdafx.h"
#include <vector>
using namespace std;
class MakeIni
{
public:
	MakeIni(void);
	~MakeIni(void);
	void FindFile(CString strPath);
	BOOL GetFileType(const CString &strFileName);
	void WriteIni();
	//输出参数在前面，输入参数在后面，可以修改
	BOOL GetFileVersion(LPCTSTR strFile, CString& strVersion,CString &filename,CString &description);
public:
	vector<FILEINFO> m_LocalFilesInfo;
};
