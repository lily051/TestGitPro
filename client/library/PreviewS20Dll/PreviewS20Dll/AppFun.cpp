#include "stdafx.h"
#include "AppFun.h"
#include <TlHelp32.h>  //获取进程时需要的头文件

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
Description:	创建目录
Input:			char * csFilePath 需要创建的目录
Output:      	
Return:			TRUE表示创建成功，否则失败
2010年4月13日10:04:17 Qimx
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
					strError.Format(_T("创建目录 %s 失败!"),csFilePath);
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
Description:	创建文件
Input:			char * csFileName 需要创建文件的绝对路径
Output:      	
Return:			失败返回NULL，否则返回文件句柄
2010年4月13日10:04:17 Qimx
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
Description:	释放偶obj资源
Input:			HGDIOBJ& hObject 句柄
Output:      	
Return:			无
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
Description:	查看路径是否合法(% ％　')
Input:          CString csString,被检查的字符串
				int nMaxLen,路径最大长度
Output:      	
Return:			返回枚举类型 CHECKPATH_SUCESS成功 否则失败
qmx 2009年10月26日8:55:25 添加
*************************************************/
ENUM_CHECKPATH_LEAGL AppFun_CheckStrLegality(CString csString, int nMaxLen,int nMinLen)
{
	//csString.Trim();

	if (0 == wcscmp(csString,_T("")))
	{
		return CHECKPATH_EMPTY;
	}
	
	if (csString.Find(_T("%"))>=0 || csString.Find(_T("'"))>=0 || csString.Find(_T("％"))>=0)
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
Description:	根据进程的名称获取进程的ID
Input:			pName进程名称
Output:      	
Return:			-1获取失败，否则返回进程ID
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
Description:	获得分割模式对应的窗口个数
Input:			int nLayoutMode,分割模式
Output:      	
Return:			无
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
*    Description:   显示保存对话框
*    Input:         sFileName：文件名称；sTitle：标题；sFilter：过滤文件
*    Output:         OutputFileName：输出文件名称
*    Return:	    成功返回TRUE,否则FALSE 
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
			if(!DeleteFile(OutputFileName))//删除已存在文件
			{  
				CString szTemp;
				szTemp = "删除文件失败！";
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
*    Description:   获取本机IP
*    Input:         
*    Output:        CString & csIPAddr：得到IP
*    Return:	    成功返回TRUE,否则FALSE 
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
	BROWSEINFO bi;								  //必须传入的参数,下面就是这个结构的参数的初始化
	CString strDisplayName;						  //用来得到,你选择的活页夹路径,相当于提供一个缓冲区
	bi.hwndOwner = hWnd;				  //得到父窗口Handle值
	bi.pidlRoot = pidlRoot;						  //这个变量就是我们在上面得到的.
	bi.pszDisplayName = strDisplayName.GetBuffer(MAX_PATH + 1);		 //得到缓冲区指针
	szTemp = _T("选择文件夹");
	bi.lpszTitle = szTemp.GetBuffer(szTemp.GetLength() + 1);//设置标题
	szTemp.ReleaseBuffer();
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;			 //设置标志
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;				
	LPITEMIDLIST lpIDList = SHBrowseForFolder(&bi);					//打开对话框
	strDisplayName.ReleaseBuffer();									//和上面的GetBuffer()相对应
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