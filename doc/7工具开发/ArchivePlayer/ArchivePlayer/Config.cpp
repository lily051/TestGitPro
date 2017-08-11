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

//��ȡ�����ļ�
int  Config::GetConfig(mysqlInfo& sqlcfg)
{
	// �õ�exeִ��·��.  

	//_tcsrchr() ��������������һ��'\\'��λ�ã������ظ�λ�õ�ָ��  
	const WCHAR *pFind = wcsrchr(m_pathName.c_str(), '\\');  
	if (pFind == NULL)  
	{  

		AfxMessageBox(L"�Ҳ��������ļ�!");
		return FALSE;  
	}  
 
    
	CString szIniPath = m_pathName.c_str();  

	//����ִ�ж�ȡ ----------------------------------  
	if (!::PathFileExists(szIniPath))  
	{  
		AfxMessageBox(L"�Ҳ��������ļ�!");
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

//��ȡ�����ļ�


int  Config::GetConfig()
{
  
    const WCHAR *pFind = wcsrchr(m_pathName.c_str(), '\\');  
    if (pFind == NULL)  
    {  

        AfxMessageBox(L"�Ҳ��������ļ�!");
        return FALSE;  
    }  


    CString szIniPath = m_pathName.c_str();  

    //����ִ�ж�ȡ ----------------------------------  
    if (!::PathFileExists(szIniPath))  
    {  
        AfxMessageBox(L"�Ҳ��������ļ�!");
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
    tmpinfo.listname = _T("ҵ�񵥺�");
    tmpinfo.type = TABLE_CLIENTINPUT;
    Itemlist.push_back(tmpinfo);

    /*tmpinfo.Dbname = _T("status");
    tmpinfo.listname = _T("ҵ��״̬");
    tmpinfo.type = TABLE_CLIENTINPUT;
    Itemlist.push_back(tmpinfo);*/

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("inputRegionName");
    tmpinfo.listname = _T("��������");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("productName");
    tmpinfo.listname = _T("��Ʒ����");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("productTypeName");
    tmpinfo.listname = _T("��Ʒ��������");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("proName");
    tmpinfo.listname = _T("ҵ��Ա����");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("inputTime");
    tmpinfo.listname = _T("¼��ʱ��");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_PROSAVERECORD;
    tmpinfo.Dbname = _T("fileName");
    tmpinfo.listname = _T("¼������");
    Itemlist.push_back(tmpinfo);

 /*   tmpinfo.Dbname = _T("");
    tmpinfo.listname = _T("¼������");
    Itemlist.push_back(tmpinfo);*/

    tmpinfo.type = TABLE_PROSAVERECORD;
    tmpinfo.Dbname = _T("upload");
    tmpinfo.listname = _T("¼��״̬");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_PROSAVERECORD;
    tmpinfo.Dbname = _T("startTime");
    tmpinfo.listname = _T("��ʼʱ��");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_CLIENTINPUT;
    tmpinfo.Dbname = _T("endTime");
    tmpinfo.listname = _T("����ʱ��");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_PROSAVERECORD;
    tmpinfo.Dbname = _T("");
    tmpinfo.listname = _T("�洢IP");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = TABLE_PROSAVERECORD;
    tmpinfo.Dbname = _T("streamNo");
    tmpinfo.listname = _T("��������Ϣ");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = -1;
    tmpinfo.Dbname = _T("");
    tmpinfo.listname = _T("�豸¼��״��");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = -1;
    tmpinfo.Dbname = _T("");
    tmpinfo.listname = _T("����¼��״��");
    Itemlist.push_back(tmpinfo);

    tmpinfo.type = -1;
    tmpinfo.Dbname = _T("");
    tmpinfo.listname = _T("�쳣����");
    Itemlist.push_back(tmpinfo);
    return TRUE;
}