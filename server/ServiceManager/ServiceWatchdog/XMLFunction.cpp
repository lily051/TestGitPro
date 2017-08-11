/**
*   @file   XMLFunction.cpp
*   @note   HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief 
*  
*   @author mazhentong
*   @date   2012/10/24
*
*   @note
*
*   @warning
*/

#include "stdafx.h"

#include "XMLFunction.h"
#include "Markup.h"
#include "Folder.h"
#include <hak.h>

char const INI_CENTER_WEBCFG_FILE_NAME[] = "FSvcWeb_8120\\webapps\\ROOT\\WEB-INF\\classes\\config.properties";
char const INI_COLLECT_WEBCFG_FILE_NAME[] = "FSvcWeb_8120\\webapps\\WebDevice\\WEB-INF\\classes\\config.properties";

/** @fn       BOOL XML_GetServerSysInfo(CString &ServerSysInfo, const SERVER_SYS_INFO& struSSysInfo)
 *  @brief    ��ȡ������ϵͳ��Ϣ��XML����
 *  @param    (CString &) ServerSysInfo [OUT] : ��ȡ����XML����
 *            (const SERVER_SYS_INFO&) struSSysInfo [IN] : ��������ϵͳ��Ϣ
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL XML_GetServerSysInfo(CString &ServerSysInfo, const SERVER_SYS_INFO& struSSysInfo)
{
    ServerSysInfo = _T("");

    CMarkup xmlSysInfo;
    xmlSysInfo.SetDoc(XML_HEAD);

    xmlSysInfo.AddElem(XML_ROOT);
    xmlSysInfo.IntoElem();
    
    xmlSysInfo.AddElem(XML_SERVER);
    // ���������
    xmlSysInfo.AddAttrib(_T("sn"), struSSysInfo.SN);
    xmlSysInfo.AddAttrib(_T("hostname"), struSSysInfo.HostName);
    xmlSysInfo.IntoElem();

    CString csTmp;
    // ����ϵͳ��Ϣ
    xmlSysInfo.AddElem(XML_OS_INFO);
    // version
    xmlSysInfo.AddAttrib(_T("version"), struSSysInfo.OsInfo);
    // type
    csTmp.Format(_T("%dλ"), struSSysInfo.OsType);
    xmlSysInfo.AddAttrib(_T("type"), csTmp);

    // ����ʱ��
    xmlSysInfo.AddElem(XML_START_TIME);
    // system start time
    xmlSysInfo.AddAttrib(_T("systime"), struSSysInfo.startTime);
    // service manager start time
    xmlSysInfo.AddAttrib(_T("sermgtime"), struSSysInfo.csServiceManagerStartTime);

    // �ڴ���Ϣ
    xmlSysInfo.AddElem(XML_MEMORY);
    // capacity (MB)
    xmlSysInfo.AddAttrib(_T("capacity"), struSSysInfo.TotalMem);
    // using
    xmlSysInfo.AddAttrib(_T("using"), struSSysInfo.UsingMem);

    // ��������Ϣ
    xmlSysInfo.AddElem(XML_CPU);
    // type
    xmlSysInfo.AddAttrib(_T("type"), struSSysInfo.CpuInfo);
    // using
    xmlSysInfo.AddAttrib(_T("using"), struSSysInfo.CpuSysWorking);
    
    xmlSysInfo.AddElem(XML_NET_WORK_INFO);
    if (xmlSysInfo.IntoElem())
    {
        // ������Ϣ
        for (size_t nNetIndex = 0; nNetIndex < struSSysInfo.NetWorkInfo.size(); nNetIndex++)
        {
            xmlSysInfo.AddElem(XML_NET_WORK);
            // type
            csTmp.Format(_T("%s"), struSSysInfo.NetWorkInfo[nNetIndex].IpType);
            xmlSysInfo.AddAttrib(_T("type"), csTmp);

            // ip And mask
            for (size_t nIpAddIndex = 0; nIpAddIndex < struSSysInfo.NetWorkInfo[nNetIndex].vecIPAddAndSubNet.size(); nIpAddIndex++)
            {
                xmlSysInfo.AddChildElem(XML_IP_ADD);
                // ip
                xmlSysInfo.AddChildAttrib(_T("ip"), struSSysInfo.NetWorkInfo[nNetIndex].vecIPAddAndSubNet[nIpAddIndex].IpAdd);
                // mask
                xmlSysInfo.AddChildAttrib(_T("mask"), struSSysInfo.NetWorkInfo[nNetIndex].vecIPAddAndSubNet[nIpAddIndex].csMask);
            }
            // gateWay
            if (struSSysInfo.NetWorkInfo[nNetIndex].vecGateway.size() <= 0)
            {
                xmlSysInfo.AddChildElem(XML_GATE_WAY);
                xmlSysInfo.AddChildAttrib(_T("gateway"), _T(""));
            }
            else
            {
                for (size_t nGateIndex = 0; nGateIndex < struSSysInfo.NetWorkInfo[nNetIndex].vecGateway.size(); nGateIndex++)
                {
                    xmlSysInfo.AddChildElem(XML_GATE_WAY);
                    xmlSysInfo.AddChildAttrib(_T("gateway"), struSSysInfo.NetWorkInfo[nNetIndex].vecGateway[nGateIndex]);
                }
            }
        }
        xmlSysInfo.OutOfElem();
    }

    // ������Ϣ
    for (size_t nDiskIndex = 0; nDiskIndex < struSSysInfo.DiskInfo.size(); nDiskIndex++)
    {
        xmlSysInfo.AddElem(XML_HARD_DISK);
        // name
        csTmp.Format(_T("%c"), struSSysInfo.DiskInfo[nDiskIndex].disk);
        xmlSysInfo.AddAttrib(_T("name"), csTmp);
        // capacity
        xmlSysInfo.AddAttrib(_T("capacity"), struSSysInfo.DiskInfo[nDiskIndex].totalNum);
        // using
        xmlSysInfo.AddAttrib(_T("using"), struSSysInfo.DiskInfo[nDiskIndex].UsingNum);
    }

    // ��������Ϣ
    for (size_t nAdapIndex = 0; nAdapIndex < struSSysInfo.AdapterInfo.size(); nAdapIndex++)
    {
        xmlSysInfo.AddElem(XML_ADAPTER);

        // name
        xmlSysInfo.AddAttrib(_T("name"), struSSysInfo.AdapterInfo[nAdapIndex].AdapterName);

        // using
        csTmp.Format("%d%%", struSSysInfo.AdapterInfo[nAdapIndex].WorkUsing);
        xmlSysInfo.AddAttrib(_T("using"), csTmp);

        // speed
        xmlSysInfo.AddAttrib(_T("speed"), struSSysInfo.AdapterInfo[nAdapIndex].LinkSpeed);

        // status
        xmlSysInfo.AddAttrib(_T("status"), struSSysInfo.AdapterInfo[nAdapIndex].AdapterStatus);
    }

    ServerSysInfo = xmlSysInfo.GetDoc();

    return TRUE;
}

/** @fn       BOOL XML_GetServerProcInfo(CString &ServerProcInfo, const std::vector<SERVER_PROCESS_INFO>& struSProcInfo)
 *  @brief    ��ȡ���������Ϣ��XML����
 *  @param    (CString &) ServerProcInfo [] : ��ȡ����XML����
 *            (const std::vector<SERVER_PROCESS_INFO>&) struSProcInfo [IN] : ���������Ϣ
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL XML_GetServerProcInfo(CString &ServerProcInfo, const std::vector<SERVER_PROCESS_INFO>& struSProcInfo)
{
    ServerProcInfo = "";
    CMarkup xmlProcInfo;
    xmlProcInfo.SetDoc(XML_HEAD);

    xmlProcInfo.AddElem(XML_ROOT);
    xmlProcInfo.IntoElem();

    xmlProcInfo.AddElem(XML_PROCESS_INFO);
    xmlProcInfo.IntoElem();

    for (size_t nProcIndex = 0; nProcIndex < struSProcInfo.size(); nProcIndex++)
    {
        // PROCESS
        xmlProcInfo.AddElem(XML_PRCOESS);

        // name
        xmlProcInfo.AddAttrib(_T("name"), struSProcInfo[nProcIndex].ServerName);

        // name
        xmlProcInfo.AddAttrib(_T("moduletype"), struSProcInfo[nProcIndex].ModuleType);

        // image_name
        xmlProcInfo.AddAttrib(_T("image_name"), struSProcInfo[nProcIndex].ProcName);

        // status
        xmlProcInfo.AddAttrib(_T("status"), struSProcInfo[nProcIndex].ServerStatus);

        // thread
        xmlProcInfo.AddAttrib(_T("thread"), struSProcInfo[nProcIndex].ThreadCount);

        // handle
        xmlProcInfo.AddAttrib(_T("handle"), struSProcInfo[nProcIndex].HandleCount);

        // memory (KB)
        xmlProcInfo.AddAttrib(_T("memory"), struSProcInfo[nProcIndex].MemWorkingSize);

        // cpu
        xmlProcInfo.AddAttrib(_T("cpu"), struSProcInfo[nProcIndex].CpuWorking);
        
        // port
        xmlProcInfo.AddAttrib(_T("post"), struSProcInfo[nProcIndex].ListenPort);

        // time
        xmlProcInfo.AddAttrib(_T("time"), struSProcInfo[nProcIndex].startTime);
    }

    ServerProcInfo = xmlProcInfo.GetDoc();
    //
    return TRUE;
}

/** @fn       BOOL XML_GetRecvSocketPort(int &nRecvSocketPort)
 *  @brief    ��ȡ����theService��Ϣ��socket��bind�Ķ˿ں�
 *  @param    (int &) nRecvSocketPort [OUT] : ��ȡ���Ķ˿ں�
 *  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
 */
BOOL XML_GetRecvSocketPort(int &nRecvSocketPort)
{
    nRecvSocketPort = 0;
    CMarkup xmlPortInfo;

    CString fileContent = CFolder::ReadFromUTF8File(CFolder::GetExecutionRoot() + "ServiceWatchDog.dat");
 
    if (strcmp(fileContent, _T("")) == 0)
    {
        return FALSE;
    }

    // ����RecvSocketPortԪ��
    if (xmlPortInfo.SetDoc(fileContent))
    {
        if (xmlPortInfo.FindElem(_T("Config")) && xmlPortInfo.IntoElem())
        {
            if (xmlPortInfo.FindElem(_T("RecvSocketPort")))
            {
                nRecvSocketPort = atoi(xmlPortInfo.GetData());
            }        
        }
    }

    // �Ϸ����ж�
    if (nRecvSocketPort <= 0 || nRecvSocketPort > 65535)
    {
        return FALSE;
    }
    return TRUE;
}

/** @fn       CString XML_GetLogoName(void)
 *  @brief    ����XML���ģ���ȡlogo��
 *  @param    void
 *  @return:  CString��LOGO NAME
 */
CString XML_GetLogoName(void)
{
    CString csLogoName;

    CMarkup xmlPortInfo;

    CString fileContent = CFolder::ReadFromUTF8File(CFolder::GetExecutionRoot() + "ServiceWatchDog.dat");
    if (strcmp(fileContent, _T("")) == 0)
    {
        return csLogoName;
    }

    // ����LOGONAMEԪ��
    if (xmlPortInfo.SetDoc(fileContent))
    {
        if (xmlPortInfo.FindElem(_T("Config")) && xmlPortInfo.IntoElem())
        {
            if (xmlPortInfo.FindElem(_T("LOGONAME")))
            {
                csLogoName = xmlPortInfo.GetData();
            }       
        }
    }

    return csLogoName;
}

/** @fn       int XML_GetSkinType(void)
 *  @brief    ��ȡƤ��������
 *  @param    void
 *  @return:  int��0:bule ; 1:red_black (defult:red_black) others:red_black
 */
int XML_GetSkinType(void)
{
    int nRetValue = 1;

    CMarkup xmlPortInfo;

    CString fileContent = CFolder::ReadFromUTF8File(CFolder::GetExecutionRoot() + "ServiceWatchDog.dat");
    if (strcmp(fileContent, _T("")) == 0)
    {
        return nRetValue;
    }

    // ����SKINTYPEԪ��
    if (xmlPortInfo.SetDoc(fileContent))
    {
        if (xmlPortInfo.FindElem(_T("Config")) && xmlPortInfo.IntoElem())
        {
            if (xmlPortInfo.FindElem(_T("SKINTYPE")))
            {
                nRetValue = atoi(xmlPortInfo.GetData());
            }       
        }
    }

    return nRetValue;
}

//��ȡ�������Ʋ���
CString XML_GetCopyParam()
{
    CString csCopyParam;
    CMarkup xmlPortInfo;
    CString fileContent = CFolder::ReadFromUTF8File(CFolder::GetExecutionRoot() + "ServiceWatchDog.dat");
    if (strcmp(fileContent, _T("")) == 0)
    {
        return csCopyParam;
    }
    // ����LOGONAMEԪ��
    if (xmlPortInfo.SetDoc(fileContent))
    {
        if (xmlPortInfo.FindElem(_T("Config")) && xmlPortInfo.IntoElem())
        {
            if (xmlPortInfo.FindElem(_T("COPY_PARAM")))
            {
                csCopyParam = xmlPortInfo.GetData();
            }       
        }
    }
    return csCopyParam;
}

BOOL XML_GetCollectorDBInfo(db_conn_info_t &collectorDBInfo)
{
    char szAppPath[MAX_PATH] = {0};
    if (HPR_OK != HPR_GetCurExePath(szAppPath, _countof(szAppPath)))
    {
        return FALSE;
    }
    char *pEnd = NULL;
    if ((pEnd = ::strrchr(szAppPath, '\\')) != NULL
        || (pEnd = ::strrchr(szAppPath, '/')) != NULL)
    {
        if (pEnd - szAppPath + 1 < (int)(_countof(szAppPath)))
        {
            *(pEnd + 1) = 0;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return false;
    }
    if (FALSE == ::SetCurrentDirectoryA(szAppPath))
    {
        return false;
    }

    //���������ļ�·��
    std::string strCfgFile = (std::string)szAppPath + (std::string)INI_COLLECT_WEBCFG_FILE_NAME;
    std::string strCenterCfgFile = (std::string)szAppPath + (std::string)INI_CENTER_WEBCFG_FILE_NAME;

    //���ݿ���Ϣ
    std::string strDbInfo = "";
    char DataBuffer[256];

    FILE * file = NULL;
    fopen_s(&file, strCfgFile.c_str(), "rb");
    if(NULL == file)
    {
        fopen_s(&file, strCenterCfgFile.c_str(), "rb");
        if (NULL == file)
        {
            return TRUE;
        }
    }
    while(fgets(DataBuffer, 256, file)!=NULL)
    {
        std::string strCurrInfo = DataBuffer;
        int iPos = strCurrInfo.find("=");
        if(-1 != iPos)
        {
            strCurrInfo = strCurrInfo.substr(0, iPos);
            if (!strcmp("datasource.url", strCurrInfo.c_str()))
            {//url
                std::string strUrl = DataBuffer;
                strDbInfo = strUrl.substr(iPos + 1, strUrl.size());
            }
            else if (!strcmp("datasource.username", strCurrInfo.c_str()))
            {//�û���
                std::string strUserName = DataBuffer;
                strUserName = strUserName.substr(iPos + 1, strUserName.size());
                //ȥ��\r\n
                strUserName.erase(strUserName.find_last_not_of("\r\n") + 1);
                collectorDBInfo.username = fcA2T(strUserName.c_str());
            }
            else if (!strcmp("datasource.password", strCurrInfo.c_str()))
            {//����
                std::string strPassword = DataBuffer;
                strPassword = strPassword.substr(iPos + 1, strPassword.size());
                //ȥ��\r\n
                strPassword.erase(strPassword.find_last_not_of("\r\n") + 1);

                char aes[1024] = { '\0' };
                int aes_len = 1024;
                //��������� �����
                if (Hak9((char *)(strPassword.c_str()), NULL, NULL))
                {
                    char* out = aes;
                    char* outl = (char*)&aes_len;
                    if (Hak7(out, (char *)(strPassword.c_str()), outl))
                    {
                        return false;
                    }
                    collectorDBInfo.password = fcA2T(out);
                }
                else
                {
                    collectorDBInfo.password = fcA2T(strPassword.c_str());
                }
            }
            else
            {
                continue;
            }
        }
    }

    fclose(file);

    if(strDbInfo.size() > 0)
    {
        std::_tstring wstrDbInfo = fcA2T(strDbInfo.c_str());

        int iPos = wstrDbInfo.find_first_of(_T("//"));
        wstrDbInfo = wstrDbInfo.substr(iPos + 2, wstrDbInfo.size());

        //ip
        iPos = wstrDbInfo.find_first_of(_T(":"));
        collectorDBInfo.host = wstrDbInfo.substr(0, iPos);
        wstrDbInfo = wstrDbInfo.substr(iPos + 1, wstrDbInfo.size());
        //ȥ����б��
        iPos = collectorDBInfo.host.find_first_of(_T("\\"));
        if(-1 != iPos)
        {
            collectorDBInfo.host = collectorDBInfo.host.substr(0, iPos);
        }

        //port
        iPos = wstrDbInfo.find_first_of(_T("/"));
        collectorDBInfo.port = wstrDbInfo.substr(0, iPos);

        //Schema
        wstrDbInfo = wstrDbInfo.substr(iPos + 1, wstrDbInfo.size());
        iPos = wstrDbInfo.find_first_of(_T("?"));
        /*collectorDBInfo.dbschema = wstrDbInfo.substr(0, iPos);*/
        collectorDBInfo.dbschema = _T("vpcdbv2");
    }
    return TRUE;

    //CMarkup xmlCollectorDBInfo;

    //CString fileContent = CFolder::ReadFromUTF8File(CFolder::GetExecutionRoot() + "ServiceWatchDog.dat");

    //if (strcmp(fileContent, _T("")) == 0)
    //{
    //    return FALSE;
    //}

    //if (xmlCollectorDBInfo.SetDoc(fileContent))
    //{
    //    if (xmlCollectorDBInfo.FindElem(_T("Config")) && xmlCollectorDBInfo.IntoElem())
    //    {
    //        if (xmlCollectorDBInfo.FindElem(_T("DBHost")))
    //        {
    //            collectorDBInfo.host = xmlCollectorDBInfo.GetData();
    //        }

    //        if (xmlCollectorDBInfo.FindElem(_T("DBPort")))
    //        {
    //            collectorDBInfo.port = xmlCollectorDBInfo.GetData();
    //        }

    //        if (xmlCollectorDBInfo.FindElem(_T("DBUser")))
    //        {
    //            collectorDBInfo.username = xmlCollectorDBInfo.GetData();
    //        }   

    //        if (xmlCollectorDBInfo.FindElem(_T("DBPassword")))
    //        {
    //            collectorDBInfo.password = xmlCollectorDBInfo.GetData();
    //        }   

    //        if (xmlCollectorDBInfo.FindElem(_T("DBSchema")))
    //        {
    //            collectorDBInfo.dbschema = xmlCollectorDBInfo.GetData();
    //        }

    //        return TRUE;
    //    }
    //}

    //return FALSE;
}

BOOL XML_GetCenterProcessorInfo(alarmer_info_t &stAlarmerInfo, center_info_t &stLocalInfo)
{
	CMarkup xmlAlarmerInfo;

	CString fileContent = CFolder::ReadFromUTF8File(CFolder::GetExecutionRoot() + SVC_CFG_FILE);

	if (strcmp(fileContent, _T("")) == 0)
	{
		return FALSE;
	}

	if (xmlAlarmerInfo.SetDoc(fileContent))
	{
		if (xmlAlarmerInfo.FindElem(_T("CentorProcessor")) && xmlAlarmerInfo.IntoElem())
		{
			if (xmlAlarmerInfo.FindElem(_T("AlarmCenter")))
			{
				stAlarmerInfo.ip = xmlAlarmerInfo.GetData();
			}

			// �Ϸ����ж�
			if (strcmp(stAlarmerInfo.ip.c_str(),_T("")) == 0)
			{
				return FALSE;
			}

			if (xmlAlarmerInfo.FindElem(_T("AlarmPort")))
			{
				stAlarmerInfo.port = atoi(xmlAlarmerInfo.GetData());
			}

			// �Ϸ����ж�
			if (stAlarmerInfo.port <= 0 || stAlarmerInfo.port > 65535)
			{
				return FALSE;
			}

			if (xmlAlarmerInfo.FindElem(_T("LocalIP")))
			{
				stLocalInfo.ip = xmlAlarmerInfo.GetData();
			}

			// �Ϸ����ж�
			if (strcmp(stLocalInfo.ip.c_str(),_T("")) == 0)
			{
				return FALSE;
			}

			if (xmlAlarmerInfo.FindElem(_T("LocalPort")))
			{
				stLocalInfo.port = atoi(xmlAlarmerInfo.GetData());
			}

			// �Ϸ����ж�
			if (stLocalInfo.port <= 0 || stLocalInfo.port > 65535)
			{
				return FALSE;
			}

			return TRUE;
		}
	}
	return FALSE;
}

BOOL XML_CheckCfgPara(void)
{
	CString fileContent = CFolder::ReadFromUTF8File(CFolder::GetExecutionRoot() + SVC_CFG_FILE);
	if (strcmp(fileContent, _T("")) == 0)
	{
		return FALSE;
	}

	CMarkup xmlInfo;
	if (!xmlInfo.SetDoc(fileContent))
	{
		return FALSE;
	}

	if (!xmlInfo.FindElem(_T("CentorProcessor")))
	{
		return FALSE;
	}

	xmlInfo.IntoElem();

	CString csTemp;
	if (!xmlInfo.FindElem(_T("AlarmCenter")))
	{
		return FALSE;
	}

	csTemp = xmlInfo.GetData();

	// �Ϸ����ж�
	if (strcmp(csTemp,_T("")) == 0)
	{
		return FALSE;
	}

	if (!xmlInfo.FindElem(_T("AlarmPort")))
	{
		return FALSE;	
	}

	int nTempPort = 0;
	nTempPort = atoi(xmlInfo.GetData());

	// �Ϸ����ж�
	if (nTempPort <= 0 || nTempPort > 65535)
	{
		return FALSE;
	}

	if (!xmlInfo.FindElem(_T("LocalIP")))
	{
		return FALSE;
	}

	csTemp = _T("");
	csTemp = xmlInfo.GetData();

	// �Ϸ����ж�
	if (strcmp(csTemp,_T("")) == 0)
	{
		return FALSE;
	}

	if (!xmlInfo.FindElem(_T("LocalPort")))
	{
		return FALSE;
	}

	nTempPort = 0;
	nTempPort = atoi(xmlInfo.GetData());

	// �Ϸ����ж�
	if (nTempPort <= 0 || nTempPort > 65535)
	{
		return FALSE;
	}

	return TRUE;
}
