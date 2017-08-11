#include "StdAfx.h"
#include "Config.h"




std::vector<Iteminfo> Itemlist;
Config::Config(std::wstring &pathname)
{
    m_pathName = pathname;
    //m_pathName = _T("..\\config\\config.ini");
}

Config::~Config(void)
{
    
}

//获取配置文件
int  Config::GetConfig(mysqlInfo& sqlcfg)
{
	// 得到exe执行路径.  

	//_tcsrchr() 反向搜索获得最后一个'\\'的位置，并返回该位置的指针  
	const WCHAR *pFind = wcsrchr(m_pathName.c_str(), '\\');  
	if (pFind == NULL)  
	{  

		AfxMessageBox(L"找不到配置文件!");
		return FALSE;  
	}  
 
    
	CString szIniPath = m_pathName.c_str();  

	//下面执行读取 ----------------------------------  
	if (!::PathFileExists(szIniPath))  
	{  
		AfxMessageBox(L"找不到配置文件!");
		return FALSE;  
	}  

	TCHAR port[MAX_PATH];
	TCHAR ip[MAX_PATH];
	TCHAR dbname[MAX_PATH];
	TCHAR username[MAX_PATH];
	TCHAR password[MAX_PATH];

	::GetPrivateProfileStringW(TEXT("Mysql"), TEXT("IP"), NULL, ip, MAX_PATH, szIniPath);  
	::GetPrivateProfileStringW(TEXT("Mysql"), TEXT("Port"), NULL,port,MAX_PATH, szIniPath); 
	::GetPrivateProfileStringW(TEXT("Mysql"), TEXT("Dbname"), NULL, dbname, MAX_PATH, szIniPath);  
	::GetPrivateProfileStringW(TEXT("Mysql"), TEXT("Username"), NULL, username, MAX_PATH, szIniPath);  
	::GetPrivateProfileStringW(TEXT("Mysql"), TEXT("Password"), NULL, password, MAX_PATH, szIniPath); 

    m_sql.Dbname = dbname;
    m_sql.Port =  port;
    m_sql.Ip = ip;
    m_sql.Username = username;
    m_sql.Password = password;
    
    sqlcfg = m_sql;
    GetConfig();
    return TRUE;
}

//获取配置文件


int  Config::GetConfig()
{
  
    const WCHAR *pFind = wcsrchr(m_pathName.c_str(), '\\');  
    if (pFind == NULL)  
    {  

        AfxMessageBox(L"找不到配置文件!");
        return FALSE;  
    }  


    CString szIniPath = m_pathName.c_str();  

    //下面执行读取 ----------------------------------  
    if (!::PathFileExists(szIniPath))  
    {  
        AfxMessageBox(L"找不到配置文件!");
        return FALSE;  
    }  
    Itemlist.clear();
    TCHAR DBitemName[MAX_PATH];
    TCHAR ListName[MAX_PATH];
    CString item;
    int cnt = 0;
    Iteminfo tmpinfo;
    while(TRUE)
    {
        cnt++;
        item.Format(_T("Item%d"),cnt);
        DWORD ret = ::GetPrivateProfileStringW(item.GetBuffer(), TEXT("DBitemName"), NULL, DBitemName, sizeof(DBitemName), szIniPath); 
        if (ret == 0)
        {
            break;
        }
        tmpinfo.Dbname = DBitemName;
        DWORD ret2 = ::GetPrivateProfileStringW(item.GetBuffer(), TEXT("ListName"), NULL, ListName, sizeof(ListName), szIniPath); 
        if (ret2 == 0)
        {
            break;
        }
        tmpinfo.listname = ListName;
        Itemlist.push_back(tmpinfo);

    }

    return TRUE;
}


int  Config::initItem()
{
  
    Itemlist.clear();
    Iteminfo tmpinfo;
    tmpinfo.Dbname = _T("nProId");
    tmpinfo.listname = _T("业务单号");
    tmpinfo.type = TABLE_CLIENTINPUT;
    Itemlist.push_back(tmpinfo);

    /*tmpinfo.Dbname = _T("status");
    tmpinfo.listname = _T("业务单状态");
    tmpinfo.type = TABLE_CLIENTINPUT;
    Itemlist.push_back(tmpinfo);*/

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("inputRegionName");
    tmpinfo.listname = _T("所属网点");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("productName");
    tmpinfo.listname = _T("产品名称");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("productTypeName");
    tmpinfo.listname = _T("产品类型名称");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("proName");
    tmpinfo.listname = _T("业务员姓名");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("inputTime");
    tmpinfo.listname = _T("录入时间");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_PROSAVERECORD;
    tmpinfo.Dbname = _T("fileName");
    tmpinfo.listname = _T("录像名称");
    Itemlist.push_back(tmpinfo);

 /*   tmpinfo.Dbname = _T("");
    tmpinfo.listname = _T("录像类型");
    Itemlist.push_back(tmpinfo);*/

    tmpinfo.type = TABLE_PROSAVERECORD;
    tmpinfo.Dbname = _T("upload");
    tmpinfo.listname = _T("录像状态");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_PROSAVERECORD;
    tmpinfo.Dbname = _T("startTime");
    tmpinfo.listname = _T("开始时间");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("endTime");
    tmpinfo.listname = _T("结束时间");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_PROSAVERECORD;
    tmpinfo.Dbname = _T("");
    tmpinfo.listname = _T("存储IP");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_PROSAVERECORD;
    tmpinfo.Dbname = _T("streamNo");
    tmpinfo.listname = _T("编码器信息");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = -1;
    tmpinfo.Dbname = _T("");
    tmpinfo.listname = _T("设备录像状况");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = -1;
    tmpinfo.Dbname = _T("");
    tmpinfo.listname = _T("中心录像状况");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = -1;
    tmpinfo.Dbname = _T("");
    tmpinfo.listname = _T("异常描述");
    Itemlist.push_back(tmpinfo);
    return TRUE;
}