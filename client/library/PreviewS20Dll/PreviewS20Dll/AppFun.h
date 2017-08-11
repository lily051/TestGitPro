#if !defined APPFUN
#define APPFUN

enum ENUM_CHECKPATH_LEAGL
{
	CHECKPATH_SUCCESS = 0,
	CHECKPATH_EMPTY,
	CHECKPATH_ILLEAGLCHAR,
	CHECKPATH_OVERLENGTH,
	CHECKPATH_OVERSHORT,
};

/*************************************************
Function:    	AppFun_CheckStrLegality
Description:	查看路径是否合法(% ％　')
Input:          CString csString,被检查的字符串
				int nMaxLen,路径最大长度
Output:      	
Return:			TRUE表示合法，FALSE表示非法
2010年4月13日10:04:17 Qimx
*************************************************/
ENUM_CHECKPATH_LEAGL AppFun_CheckStrLegality(CString csString, int nMaxLen = 256, int nMinLen = 0);

/*************************************************
Function:    	AppFun_CreateDirectory
Description:	创建目录
Input:			char * csFilePath 需要创建的目录
Output:      	
Return:			TRUE表示创建成功，否则失败
2010年4月13日10:04:17 Qimx
*************************************************/
BOOL AppFun_CreateDirectory(TCHAR * csFilePath);

/*************************************************
Function:    	AppFun_CreateFile
Description:	创建文件
Input:			char * csFileName 需要创建文件的绝对路径
Output:      	
Return:			失败返回NULL，否则返回文件句柄
2010年4月13日10:04:17 Qimx
*************************************************/
HANDLE AppFun_CreateFile(TCHAR *csFileName);

/*************************************************
Function:    	AppFun_GetProcessIDByName
Description:	根据进程的名称获取进程的ID
Input:			pName进程名称
Output:      	
Return:			-1获取失败，否则返回进程ID
*************************************************/
DWORD AppFun_GetProcessIDByName(LPCTSTR pName);

/*************************************************
Function:    	AppFun_FreeImageResource
Description:	释放obj资源
Input:			HGDIOBJ& hObject 句柄
Output:      	
Return:			无
*************************************************/
void AppFun_FreeImageResource(HGDIOBJ& hObject);

/*************************************************
Function:    	AppFun_GetLayoutWndNum
Description:	获得分割模式对应的窗口个数
Input:			int nLayoutMode,分割模式
Output:      	
Return:			无
*************************************************/
int AppFun_GetLayoutWndNum(int nLayoutMode);

/********************************************************************
Function：	 ShowSaveFileDlg
Description：显示保存对话框
Input：		 sFileName：文件名称；sTitle：标题；sFilter：过滤文件
Output：     OutputFileName：输出文件名称
Return：	 点击确定或者点击取消
*********************************************************************/
BOOL AppFun_ShowSaveFileDlg(const CString sFileName,const CString sTitle,const CString sFilter,CString& OutputFileName);

/*******************************************************************************
*    Function:      AppFun_GetLocalIPAddr 
*    Description:   获取本机IP
*    Input:         
*    Output:        CString & csIPAddr：得到IP
*    Return:	    成功返回TRUE,否则FALSE 
*******************************************************************************/
BOOL AppFun_GetLocalIPAddr(CString & csIPAddr);

//获取路径
CString AppFun_GetDirectoryPath(HWND hWnd, CString strPath=NULL);

//获取程序运行的路径
CString AppFun_GetAppPath();

#define SAFE_DELETE(p) {if(p) { delete (p); (p)=0;} }
#endif
