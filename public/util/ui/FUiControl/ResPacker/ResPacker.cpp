
// ResPacker.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ResPacker.h"
#include "ResPackerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CResPackerApp

BEGIN_MESSAGE_MAP(CResPackerApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CResPackerApp 构造

CResPackerApp::CResPackerApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CResPackerApp 对象

CResPackerApp theApp;

static ULONG_PTR fui_token = NULL;
// CResPackerApp 初始化

BOOL CResPackerApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	GdiplusStartupInput input;
	GdiplusStartup(&fui_token, &input, NULL);

	CResPackerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

int SuperMkDirs(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
    ASSERT(lpPathName);
    int nResult = -1;
    if (!lpPathName || _tcslen(lpPathName) <= 3)
    {
        OutputDebugStringA("invalid parameter\n");
        return nResult;
    }
    CString strPath = lpPathName;
    int nPos = strPath.Find(_T(":\\"));
    if (nPos == -1)
    {
        OutputDebugStringA("Expect a absolute path\n");
        return nResult;
    }
    if (strPath.Right(1) != _T('\\'))
    {
        strPath += _T("\\");
    }
    ++nPos;
    while ((nPos = strPath.Find(_T('\\'), nPos+1)) != -1)
    {
        TRACE(strPath.Left(nPos) + _T("\n"));
        if (!CreateDirectory(strPath.Left(nPos+1), lpSecurityAttributes))
        {
            if (GetLastError() != ERROR_ALREADY_EXISTS)
            {
                return -1;
            }
            else
            {
                nResult = 1;
            }
        }  
        else
        {
            nResult = 0;
        }
    }

    return nResult;
}

// 生成一段由数字组成的随机字符串
char *GenRandomString(char *pStr, int nLen)
{
    nLen -= 1;
    int *pInt32 = (int*) pStr;

    int i = 0;
    for (; i < nLen / 4; ++i)
    {
        pInt32[i] = (rand() | 0x30303030) & 0x39393939;
    }
    i *= 4;

    for (; i < nLen; ++i)
    {
        pStr[i] = rand() % (0x39 - 0x30) + 0x30;
    }
    pStr[i] = '\0';

    return pStr;
}

//删除一个文件夹下的所有内容  
void myDeleteDirectory(CString strDelPath)
{
    CFileFind finder;
    CString strPath;
    strPath.Format(_T("%s\\*.*"), (LPCTSTR)strDelPath);
    BOOL bWorking = finder.FindFile(strPath);
    while(bWorking)
    {  
        bWorking = finder.FindNextFile();
        if(finder.IsDirectory() && !finder.IsDots())
        {
            //处理文件夹  
            myDeleteDirectory(finder.GetFilePath()); //递归删除文件夹
            RemoveDirectory(finder.GetFilePath());
        }
        else
        {//处理文件
            DeleteFile(finder.GetFilePath());
        }
    }
    RemoveDirectory(strDelPath); 
}

void myCopyDirectory(CString strSrc, CString strDst)
{
    // 创建目标文件夹
    if (PathFileExists(strDst))
    {
        myDeleteDirectory(strDst);
    }
    SuperMkDirs(strDst);

    CFileFind finder;
    CString strPath(strSrc);
    strPath.AppendFormat(_T("\\*.*"), (LPCTSTR)strSrc);
    BOOL bWorking = finder.FindFile(strPath);
    while(bWorking)
    {  
        bWorking = finder.FindNextFile();
        if (!finder.IsDots() && !finder.IsSystem())
        {
            if(finder.IsDirectory())
            {
                //是文件夹 而且 名称不含 . 或 ..  
                myCopyDirectory(finder.GetFilePath(), strDst + _T("\\") + finder.GetFileName()); //递归创建文件夹+"/"+finder.GetFileName()  
            }  
            else
            {
                CString s = finder.GetFilePath();
                //是文件 则直接复制  
                CopyFile(finder.GetFilePath(), strDst + _T("\\") + finder.GetFileName(), FALSE);
            }
        }
    }
}

