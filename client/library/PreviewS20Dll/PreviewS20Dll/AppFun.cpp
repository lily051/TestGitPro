#include "stdafx.h"
#include "AppFun.h"
#include <TlHelp32.h>  //��ȡ����ʱ��Ҫ��ͷ�ļ�

#include "CustomLayoutData.h"

BOOL AppFun_GetDeskRect(LPRECT pRect)
{
	HWND hWnd;

	hWnd = ::GetDesktopWindow();

	if(hWnd)
	{
		::GetClientRect(hWnd, pRect);
		return TRUE;
	}

	return FALSE;
}

/*************************************************
Function:    	AppFun_CreateDirectory
Description:	����Ŀ¼
Input:			char * csFilePath ��Ҫ������Ŀ¼
Output:      	
Return:			TRUE��ʾ�����ɹ�������ʧ��
2010��4��13��10:04:17 Qimx
*************************************************/
BOOL AppFun_CreateDirectory(TCHAR * csFilePath)
{
	CString strText = _T("");
	strText.Format(csFilePath);

	if(strText.Right(1) != "\\")
	{
		strText += "\\";
	}

	DWORD dwRet = 0;
	int iStart = 0;
	int iret = 0;
	CString strPath = _T("");

	while(TRUE)
	{
		iret = strText.Find(_T("\\"),iStart);
		if(iret >= 0)
		{
			iStart = iret + 1;
			strPath = strText.Left(iStart);
			dwRet  = GetFileAttributes(strPath.GetBuffer(0));

			if(dwRet==0xFFFFFFFF)
			{
				if(!CreateDirectory(strPath,NULL))
				{
					CString strError = _T("");
					strError.Format(_T("����Ŀ¼ %s ʧ��!"),csFilePath);
					return FALSE;
				}
			}
		}
		else
		{
			return TRUE;
		}
	}
}

/*************************************************
Function:    	AppFun_CreateFile
Description:	�����ļ�
Input:			char * csFileName ��Ҫ�����ļ��ľ���·��
Output:      	
Return:			ʧ�ܷ���NULL�����򷵻��ļ����
2010��4��13��10:04:17 Qimx
*************************************************/
HANDLE AppFun_CreateFile(TCHAR *csFileName)
{
	HANDLE hFileHandle = NULL;
	CString strError = _T("");
	DWORD dwDirattr = 0;
	CFileFind tempFind;

	dwDirattr = GetFileAttributes(csFileName);

	if(dwDirattr == 0xFFFFFFFF)
	{
		hFileHandle = CreateFile(csFileName,
			GENERIC_WRITE,                
			FILE_SHARE_READ,             
			NULL,                        
			OPEN_ALWAYS,               
			FILE_ATTRIBUTE_NORMAL,       
			NULL	
			);

		if(INVALID_HANDLE_VALUE == hFileHandle)
		{
			return NULL;
		}
	}

	return hFileHandle;
}

/*************************************************
Function:    	AppFun_FreeImageResource
Description:	�ͷ�żobj��Դ
Input:			HGDIOBJ& hObject ���
Output:      	
Return:			��
*************************************************/
void AppFun_FreeImageResource(HGDIOBJ& hObject)
{
	if( hObject )
	{
		if( DeleteObject(hObject) )
		{
			hObject = NULL;
		}
	}
}

/*************************************************
Function:    	CheckStrLegality
Description:	�鿴·���Ƿ�Ϸ�(% ����')
Input:          CString csString,�������ַ���
				int nMaxLen,·����󳤶�
Output:      	
Return:			����ö������ CHECKPATH_SUCESS�ɹ� ����ʧ��
qmx 2009��10��26��8:55:25 ���
*************************************************/
ENUM_CHECKPATH_LEAGL AppFun_CheckStrLegality(CString csString, int nMaxLen,int nMinLen)
{
	//csString.Trim();

	if (0 == wcscmp(csString,_T("")))
	{
		return CHECKPATH_EMPTY;
	}
	
	if (csString.Find(_T("%"))>=0 || csString.Find(_T("'"))>=0 || csString.Find(_T("��"))>=0)
	{
		return CHECKPATH_ILLEAGLCHAR;
	}

	if (0 == wcscmp(csString, _T(" ")))
	{
		return CHECKPATH_EMPTY;
	}

	if (csString.GetLength() > nMaxLen)
	{
		return CHECKPATH_OVERLENGTH;
	}
	if (csString.GetLength() < nMinLen )
	{
		return CHECKPATH_OVERSHORT;
	}

	return CHECKPATH_SUCCESS;
}

/*************************************************
Function:    	AppFun_GetProcessIDByName
Description:	���ݽ��̵����ƻ�ȡ���̵�ID
Input:			pName��������
Output:      	
Return:			-1��ȡʧ�ܣ����򷵻ؽ���ID
*************************************************/
//lint -e570
DWORD AppFun_GetProcessIDByName(LPCTSTR pName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	DWORD dwPID;
	if (INVALID_HANDLE_VALUE == hSnapshot) 
	{
		return -1;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL fOk;
	for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe)) 
	{
		if (!_tcscmp(pe.szExeFile, pName)) 
		{
			CloseHandle(hSnapshot);
			dwPID = pe.th32ProcessID;
			return dwPID;
		}
	}
	return -1;
}
//lint +e570

/*************************************************
Function:    	AppFun_GetLayoutWndNum
Description:	��÷ָ�ģʽ��Ӧ�Ĵ��ڸ���
Input:			int nLayoutMode,�ָ�ģʽ
Output:      	
Return:			��
*************************************************/
int AppFun_GetLayoutWndNum(int nLayoutMode)
{
	int nWndNum = 1;
	switch (nLayoutMode)
	{
		case PREVIEW_ARRANGE_TYPE_1_1_A:
			nWndNum = 1;
			break;
		case PREVIEW_ARRANGE_TYPE_2_2_A:
			nWndNum = 4;
			break;
		case PREVIEW_ARRANGE_TYPE_3_3_B:
			nWndNum = 6;
			break;
		case PREVIEW_ARRANGE_TYPE_4_4_D:
			nWndNum = 8;
			break;
		case PREVIEW_ARRANGE_TYPE_3_3_A:
			nWndNum = 9;
			break;
		case PREVIEW_ARRANGE_TYPE_4_4_C:
			nWndNum = 10;
			break;
		case PREVIEW_ARRANGE_TYPE_4_4_B:
			nWndNum = 13;
			break;
		case PREVIEW_ARRANGE_TYPE_5_5_D:
			nWndNum = 14;
			break;
		case PREVIEW_ARRANGE_TYPE_4_4_A:
			nWndNum = 16;
			break;
		case PREVIEW_ARRANGE_TYPE_5_5_C:
			nWndNum = 17;
			break;
		case PREVIEW_ARRANGE_TYPE_5_5_B:
			nWndNum = 22;
			break;
		case PREVIEW_ARRANGE_TYPE_5_5_A:
			nWndNum = 25;
			break;
		default:
            nWndNum = -1;
			break;
	}
	return nWndNum;
}

/*******************************************************************************
*    Function:      AppFun_ShowSaveFileDlg 
*    Description:   ��ʾ����Ի���
*    Input:         sFileName���ļ����ƣ�sTitle�����⣻sFilter�������ļ�
*    Output:         OutputFileName������ļ�����
*    Return:	    �ɹ�����TRUE,����FALSE 
*******************************************************************************/
BOOL AppFun_ShowSaveFileDlg(const CString sFileName,const CString sTitle,const CString sFilter,CString& OutputFileName)
{
	CFileDialog dlgFile(FALSE,NULL,sFileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,sFilter);

	dlgFile.m_ofn.lpstrTitle  = sTitle;
	if (dlgFile.DoModal() == IDOK)
	{
		OutputFileName = dlgFile.GetPathName();

		CFileFind finder;   
		if(finder.FindFile(OutputFileName))   
		{   
			if(!DeleteFile(OutputFileName))//ɾ���Ѵ����ļ�
			{  
				CString szTemp;
				szTemp = "ɾ���ļ�ʧ�ܣ�";
				AfxMessageBox(szTemp,NULL,MB_OK|MB_ICONEXCLAMATION);
				return FALSE;
			}
		}

		return TRUE;
	}
	return FALSE;
}

/*******************************************************************************
*    Function:      AppFun_GetLocalIPAddr 
*    Description:   ��ȡ����IP
*    Input:         
*    Output:        CString & csIPAddr���õ�IP
*    Return:	    �ɹ�����TRUE,����FALSE 
*******************************************************************************/
BOOL AppFun_GetLocalIPAddr(CString & csIPAddr)
{
	WORD   wVersionRequested;   
	WSADATA   wsaData;   
	char   name[255];   
	PHOSTENT   hostinfo;   
	wVersionRequested = MAKEWORD(2,0);   

	if(WSAStartup(wVersionRequested,&wsaData) == 0)   
	{   
		if(gethostname(name,sizeof(name)) == 0)   
		{   
			if((hostinfo = gethostbyname(name)) != NULL)   
			{   
				csIPAddr = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
			}   
		}   
		WSACleanup();
	}     
	if (!csIPAddr.IsEmpty())
	{
		return TRUE;
	}
	return FALSE;
}


CString AppFun_GetDirectoryPath(HWND hWnd, CString strPath)
{
	CString szTemp;
	LPITEMIDLIST pidlRoot = NULL;
	SHGetSpecialFolderLocation(hWnd,CSIDL_DRIVES,&pidlRoot);
	BROWSEINFO bi;								  //���봫��Ĳ���,�����������ṹ�Ĳ����ĳ�ʼ��
	CString strDisplayName;						  //�����õ�,��ѡ��Ļ�ҳ��·��,�൱���ṩһ��������
	bi.hwndOwner = hWnd;				  //�õ�������Handleֵ
	bi.pidlRoot = pidlRoot;						  //���������������������õ���.
	bi.pszDisplayName = strDisplayName.GetBuffer(MAX_PATH + 1);		 //�õ�������ָ��
	szTemp = _T("ѡ���ļ���");
	bi.lpszTitle = szTemp.GetBuffer(szTemp.GetLength() + 1);//���ñ���
	szTemp.ReleaseBuffer();
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;			 //���ñ�־
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;				
	LPITEMIDLIST lpIDList = SHBrowseForFolder(&bi);					//�򿪶Ի���
	strDisplayName.ReleaseBuffer();									//�������GetBuffer()���Ӧ
	TCHAR pPath[MAX_PATH];
	CString Str;
	if(lpIDList)
	{
		SHGetPathFromIDList(lpIDList,pPath);
		Str = pPath;
	}
	return Str;
}

CString AppFun_GetAppPath()
{
    CString strPathName;
    TCHAR szFileName[MAX_PATH];

    if (0 != GetModuleFileName(AfxGetInstanceHandle(), szFileName, MAX_PATH - 1))
    {
        szFileName[MAX_PATH - 1] = '\0';
    }

    TCHAR * p = _tcsrchr(szFileName, '\\') + 1;

    if (p)
    {
        *p = '\0';

        strPathName = szFileName;
    }
    return strPathName;
}