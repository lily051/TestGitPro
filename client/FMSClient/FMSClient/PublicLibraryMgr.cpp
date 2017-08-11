#include "StdAfx.h"
#include "PublicLibraryMgr.h"
#include "MultiLang/tinyxml/tinyxml.h"
#include "FMSClient.h"

CPublicLibraryMgr::CPublicLibraryMgr(CString strPath)
{
    LoadDll(strPath);
}

CPublicLibraryMgr::~CPublicLibraryMgr(void)
{
    try
    {
        FreeDll();
    }
    catch (...)
    {
    }

}

void CPublicLibraryMgr::LoadConfig(CString& strPath)
{
    //<preload>
    //    <file>hpr.dll</file>
    //    <file>hpp.dll</file>
    //    <file>GeneralPlayerAPI.dll</file>
    //</preload>

    TiXmlDocument doc;
    if (!doc.LoadFile(fcT2A((LPCTSTR)strPath)))
    {
       // FMSCLIENT_ERROR("LoadFile failed : %s", fcT2A(strPath));
        return ;
    }

    TiXmlElement* pRoot = doc.RootElement();
    if (NULL == pRoot)
    {
       // FMSCLIENT_ERROR("Find RootElement failed");
        return ;
    }

	int nCount = 0;
    TiXmlElement* pFileNode = pRoot->FirstChildElement("file");
    while (pFileNode)
    {
		LIBRARY_INFO libinfo;
        libinfo.strFile = fcU2T(pFileNode->FirstChild()->Value());
        
		m_mapLibrary[nCount++] = libinfo;

        pFileNode = pFileNode->NextSiblingElement();
    }
}

void CPublicLibraryMgr::LoadDll(CString& strPath)
{
    if (strPath.IsEmpty())
    {
        //CLIENT_INFO("PublicLibrary path is Empty");
        return ;
    }

    LoadConfig(strPath);

    std::map<int, LIBRARY_INFO>::iterator itor = m_mapLibrary.begin();
    while (itor != m_mapLibrary.end())
    {
		CString strFile = theApp.GetAppPath() + itor->second.strFile;
        itor->second.hInstance = LoadLibrary(strFile);
        if(NULL == itor->second.hInstance)
        {
            //CLIENT_INFO("LoadLibrary failed %d: %s", GetLastError(), fcT2A(strFile));
		}
	//	CLIENT_INFO("LoadLibrary OK %s", fcT2A(strFile));

        itor++;
    }
}

void CPublicLibraryMgr::FreeDll()
{
    std::map<int, LIBRARY_INFO>::iterator itor = m_mapLibrary.begin();
    while (itor != m_mapLibrary.end())
    {
        if (NULL != itor->second.hInstance)
        {
            FreeLibrary(itor->second.hInstance);
        }

        //CLIENT_INFO("FreeLibrary : %s", fcT2A(itor->second.strFile));
        itor++;
    }
	m_mapLibrary.clear();
}
