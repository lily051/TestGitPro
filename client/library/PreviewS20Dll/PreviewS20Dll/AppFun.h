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
Description:	�鿴·���Ƿ�Ϸ�(% ����')
Input:          CString csString,�������ַ���
				int nMaxLen,·����󳤶�
Output:      	
Return:			TRUE��ʾ�Ϸ���FALSE��ʾ�Ƿ�
2010��4��13��10:04:17 Qimx
*************************************************/
ENUM_CHECKPATH_LEAGL AppFun_CheckStrLegality(CString csString, int nMaxLen = 256, int nMinLen = 0);

/*************************************************
Function:    	AppFun_CreateDirectory
Description:	����Ŀ¼
Input:			char * csFilePath ��Ҫ������Ŀ¼
Output:      	
Return:			TRUE��ʾ�����ɹ�������ʧ��
2010��4��13��10:04:17 Qimx
*************************************************/
BOOL AppFun_CreateDirectory(TCHAR * csFilePath);

/*************************************************
Function:    	AppFun_CreateFile
Description:	�����ļ�
Input:			char * csFileName ��Ҫ�����ļ��ľ���·��
Output:      	
Return:			ʧ�ܷ���NULL�����򷵻��ļ����
2010��4��13��10:04:17 Qimx
*************************************************/
HANDLE AppFun_CreateFile(TCHAR *csFileName);

/*************************************************
Function:    	AppFun_GetProcessIDByName
Description:	���ݽ��̵����ƻ�ȡ���̵�ID
Input:			pName��������
Output:      	
Return:			-1��ȡʧ�ܣ����򷵻ؽ���ID
*************************************************/
DWORD AppFun_GetProcessIDByName(LPCTSTR pName);

/*************************************************
Function:    	AppFun_FreeImageResource
Description:	�ͷ�obj��Դ
Input:			HGDIOBJ& hObject ���
Output:      	
Return:			��
*************************************************/
void AppFun_FreeImageResource(HGDIOBJ& hObject);

/*************************************************
Function:    	AppFun_GetLayoutWndNum
Description:	��÷ָ�ģʽ��Ӧ�Ĵ��ڸ���
Input:			int nLayoutMode,�ָ�ģʽ
Output:      	
Return:			��
*************************************************/
int AppFun_GetLayoutWndNum(int nLayoutMode);

/********************************************************************
Function��	 ShowSaveFileDlg
Description����ʾ����Ի���
Input��		 sFileName���ļ����ƣ�sTitle�����⣻sFilter�������ļ�
Output��     OutputFileName������ļ�����
Return��	 ���ȷ�����ߵ��ȡ��
*********************************************************************/
BOOL AppFun_ShowSaveFileDlg(const CString sFileName,const CString sTitle,const CString sFilter,CString& OutputFileName);

/*******************************************************************************
*    Function:      AppFun_GetLocalIPAddr 
*    Description:   ��ȡ����IP
*    Input:         
*    Output:        CString & csIPAddr���õ�IP
*    Return:	    �ɹ�����TRUE,����FALSE 
*******************************************************************************/
BOOL AppFun_GetLocalIPAddr(CString & csIPAddr);

//��ȡ·��
CString AppFun_GetDirectoryPath(HWND hWnd, CString strPath=NULL);

//��ȡ�������е�·��
CString AppFun_GetAppPath();

#define SAFE_DELETE(p) {if(p) { delete (p); (p)=0;} }
#endif
