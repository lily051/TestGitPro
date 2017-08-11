#include "StdAfx.h"
#include "MakeIni.h"
#include <windows.h>
#include "tlhelp32.h"
MakeIni::MakeIni(void)
{
}

MakeIni::~MakeIni(void)
{
}
BOOL MakeIni::GetFileType(const CString &strFileName)
{
	char cFileType[20]={0};
	char cFileName[MAX_PATH] = {0};
	StringChange::CStringToChar(strFileName,cFileName);
	int i = 0;
	while(cFileName[i] != '.')i++;
	for(int k = 0;cFileName[i] != '\0';k++,i++)
	{
		cFileType[k] = cFileName[i];
	}
	if(strcmp(cFileType,".dll") == 0 || strcmp(cFileType,".exe") == 0)
		return TRUE;
	return FALSE;

}
BOOL MakeIni::GetFileVersion(LPCTSTR strFile, CString& strVersion,CString &filename,CString &description)    
{    
	TCHAR szVersionBuffer[8192] = _T("");    
	DWORD dwVerSize;    
	DWORD dwHandle;    

	dwVerSize = GetFileVersionInfoSize(strFile, &dwHandle);    
	if (dwVerSize == 0)    
		return FALSE;    

	if (GetFileVersionInfo(strFile, 0, dwVerSize, szVersionBuffer))    
	{    
		VS_FIXEDFILEINFO * pInfo;    
		unsigned int nInfoLen; 

		if (VerQueryValue(szVersionBuffer, _T("\\"), (void**)&pInfo, &nInfoLen))   
		{   
			//版本号
			strVersion.Format(_T("%d.%d.%d.%d"),    
				HIWORD(pInfo->dwFileVersionMS), LOWORD(pInfo->dwFileVersionMS),    
				HIWORD(pInfo->dwFileVersionLS), LOWORD(pInfo->dwFileVersionLS));    
		} 
		CString language;
		CString szsubBlock;
		struct LANGANDCODEPAGE {
			WORD wLanguage;
			WORD wCodePage;
		}*lpTranslate;

		VerQueryValue(szVersionBuffer, 
			TEXT("\\VarFileInfo\\Translation"),
			(LPVOID *)&lpTranslate,
			&nInfoLen);

		szsubBlock.Format(_T("\\StringFileInfo\\%04x%04x\\OriginalFileName"),
			lpTranslate->wLanguage,lpTranslate->wCodePage);

		if(VerQueryValue(szVersionBuffer, szsubBlock, (LPVOID*)&pInfo, &nInfoLen))
		{
			filename.Format(_T("%s"),pInfo);	
		}      

		szsubBlock.Format(_T("\\StringFileInfo\\%04x%04x\\FileDescription"),
			lpTranslate->wLanguage,lpTranslate->wCodePage);

		if(VerQueryValue(szVersionBuffer, szsubBlock, (LPVOID*)&pInfo, &nInfoLen))
		{
			description.Format(_T("%s"),pInfo);
		}
		return TRUE;   
	}    

	return FALSE;    
}   

void MakeIni::FindFile(CString strPath)
{
	strPath += _T("\\*.*");
	ULONGLONG size;
	CFileStatus fileStatus;
	CString strFile,strVersion,strNumber;
	CString strDescription,strFilename,strTemp;
	CFileFind ff; 
	BOOL res = ff.FindFile(strPath); 
	while (res) 
	{ 
		res = ff.FindNextFile(); 

		if (!ff.IsDirectory() && !ff.IsDots()) 
		{ 
			char cTempName[MAX_PATH]={0};
			StringChange::CStringToChar(ff.GetFileName(),cTempName);
			if(GetFileType(ff.GetFileName()) == FALSE)
				continue;
			MAKEINI_LOG_INFO("文件名%s",cTempName);
			strFile = ff.GetFilePath();
			GetFileVersion(strFile,strVersion,strFilename,strDescription);

			if (CFile::GetStatus(ff.GetFilePath(), fileStatus))
			{
				size = fileStatus.m_size;
			}
			strTemp.Format(_T("%d"),size);
			
			FileInfo  fileinfo = {0};
			fileinfo.strName = ff.GetFileName();//strFilename;
			fileinfo.size = size;
			fileinfo.strVersion = strVersion;
			fileinfo.strPath = strFile;
			m_LocalFilesInfo.push_back(fileinfo);
		} 
		else if (ff.IsDirectory() && !ff.IsDots()) 
		{
			CString strTemp = ff.GetFilePath();
			FindFile(strTemp);	
		}
	} 
	ff.Close(); 
}
void MakeIni::WriteIni()
{
	CString szIniPath = file_info::GetAppDirectory() + TEXT("LocalFiles.ini");  

	CString tmp;
	//tmp.Format(_T("%d"),m_LocalFilesInfo.size());
	//::WritePrivateProfileString(TEXT("Package"), TEXT("Time"), time, szIniPath); 


	/*CString tmp;
	tmp.Format(_T("%d"),m_LocalFilesInfo.size());
	::WritePrivateProfileString(TEXT("COMMON"), TEXT("FileCount"), tmp, szIniPath); 
	for(int j = 0;j < m_LocalFilesInfo.size();j++)
	{
		CString num;
		num.Format(_T("%d"),j+1);
		tmp = TEXT("File") + num;
		::WritePrivateProfileString(TEXT("COMMON"), tmp, m_LocalFilesInfo[j].strName,szIniPath); 
	} 

	for(int i = 0;i < m_LocalFilesInfo.size(); i++)
	{
		CString tmp,strTmp;
		tmp.Format(_T("CommonFile%d"),i+1);

		strTmp = m_LocalFilesInfo[i].strName;
		::WritePrivateProfileString(tmp, TEXT("Name"), strTmp, szIniPath); 

		strTmp = m_LocalFilesInfo[i].strVersion;
		::WritePrivateProfileString(tmp, TEXT("Version"), strTmp, szIniPath); 

		strTmp.Format(_T("%d"),m_LocalFilesInfo[i].size);
		::WritePrivateProfileString(tmp, TEXT("Size"), strTmp, szIniPath); 

		strTmp = m_LocalFilesInfo[i].strPath;
		::WritePrivateProfileString(tmp, TEXT("Path"), strTmp, szIniPath); 
	}*/
}