#pragma once

typedef struct _resfile 
{
	std::string strFileName;
	std::string strFilePath;

	BOOL operator ==(const _resfile& rf)
	{
		if (0 == strFileName.compare(rf.strFileName) && 
			0 == strFilePath.compare(rf.strFilePath))
		{
			return TRUE;
		}
		return FALSE;
	}
}RES_FILE;

const int TYPE_FOLDER = 0;
const int TYPE_IMAGE  = 1;
typedef struct _rcinfo
{
	CString strName;
	int nRcType;		// 0-目录，1-文件
	CString strFullPath;
	std::list<_rcinfo> subRc;

	_rcinfo()
	{
		nRcType = 0;
	}

	void clear()
	{
		strName.Empty();
		nRcType = 0;
		strFullPath.Empty();
		subRc.clear();
	}
}RC_INFO;

const CString DEFAULT_RC_NAME = _T("未保存的资源");