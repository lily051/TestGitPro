#include "StdAfx.h"
#include "StorageMgr.h"
#include "CVR.h"
#include "Cloud.h"
#include "KMS.h"
#include "ObjectCloud.h"
#include <sstream>

SINGLETON_IMPLEMENT(CStorageMgr)///<�����������

CStorageMgr::CStorageMgr(void)
{
    InitializeCriticalSection(&m_csMapLoginInfo);
    for (int i = 0; i < STORAGE_TYPE_MAX; ++i)
    {
        m_pStorageMgr[i] = NULL;
    }
}

CStorageMgr::~CStorageMgr(void)
{
    try
    {
        Fini();
        DeleteCriticalSection(&m_csMapLoginInfo);
    }
    catch (...)
    {
    }
}

/**	@fn	    Init
*	@brief	��ʼ������
*	@param  [in] enumStorageType-�洢������
*	@return	BOOL
*/
BOOL CStorageMgr::Init(ENUM_STORAGE_TYPE enumStorageType)
{
    if (enumStorageType >= STORAGE_TYPE_MAX)
    {
        ULSERVER_LOG_ERROR("Unknown storage type %d",enumStorageType);
        return FALSE;
    }
    BOOL bRes = FALSE;
    switch (enumStorageType)
    {
    case STORAGE_TYPE_CVR:
        {
            if (NULL == m_pStorageMgr[STORAGE_TYPE_CVR])
            {
                m_pStorageMgr[STORAGE_TYPE_CVR] = new(std::nothrow) CCVR();
                if (NULL != m_pStorageMgr[STORAGE_TYPE_CVR])
                {
                    m_pStorageMgr[STORAGE_TYPE_CVR]->Init();
                    ULSERVER_LOG_INFO("init CVR");
                    bRes = TRUE;
                }
                else
                {
                    ULSERVER_LOG_ERROR("new class CVR failed.");
                    bRes = FALSE;
                }
            }
        }
        break;
    case STORAGE_TYPE_CLOUD:
        {
            if (NULL == m_pStorageMgr[STORAGE_TYPE_CLOUD])
            {
                m_pStorageMgr[STORAGE_TYPE_CLOUD] = new(std::nothrow) CCloud();
                if (NULL != m_pStorageMgr[STORAGE_TYPE_CLOUD])
                {
                    m_pStorageMgr[STORAGE_TYPE_CLOUD]->Init();
                    ULSERVER_LOG_INFO("init Cloud");
                    bRes = TRUE;
                }
                else
                {
                    ULSERVER_LOG_ERROR("new class Cloud failed.");
                    bRes = FALSE;
                }
            }
        }
        break;
    case STORAGE_TYPE_KMS:
        {
            if (NULL == m_pStorageMgr[STORAGE_TYPE_KMS])
            {
                m_pStorageMgr[STORAGE_TYPE_KMS] = new(std::nothrow) CKMS();
                if (NULL != m_pStorageMgr[STORAGE_TYPE_KMS])
                {
                    m_pStorageMgr[STORAGE_TYPE_KMS]->Init();
                    ULSERVER_LOG_INFO("init KMS");
                    bRes = TRUE;
                }
                else
                {
                    ULSERVER_LOG_ERROR("new class KMS failed.");
                    bRes = FALSE;
                }
            }
        }
        break;
	case STORAGE_TYPE_OBJECT_CLOUD:
		{
			if (NULL == m_pStorageMgr[STORAGE_TYPE_OBJECT_CLOUD])
			{
				m_pStorageMgr[STORAGE_TYPE_OBJECT_CLOUD] = new(std::nothrow) CObjectCloud();
				if (NULL != m_pStorageMgr[STORAGE_TYPE_OBJECT_CLOUD])
				{
					m_pStorageMgr[STORAGE_TYPE_OBJECT_CLOUD]->Init();
					ULSERVER_LOG_INFO("init local cloud");
					bRes = TRUE;
				}
				else
				{
					ULSERVER_LOG_ERROR("new class OBJECTCLOUD failed.");
					bRes = FALSE;
				}
			}
		}
		break;
    default:
        {
            ULSERVER_LOG_ERROR("Unknown storage type %d",enumStorageType);
            return FALSE;
        }
        break;
    }
    return bRes;
}

/**	@fn	    Fini
*	@brief	����ʼ��������������Դ�ͷ�
*	@param  [in] ��
*	@param  [out] ��
*	@return	BOOL
*/
BOOL CStorageMgr::Fini()
{
    {
        CGuard guard(&m_csMapLoginInfo);
        std::map<std::string,Struct_LoginInfo>::iterator itor = m_mapStorageLoginInfo.begin();
        while (itor != m_mapStorageLoginInfo.end())
        {
            if (STORAGE_LOGIN_FAILED != itor->second.lLoginID)
            {
                DWORD dwError = 0;
                Logout((itor->second.nManType),itor->second.lLoginID);
            }
            itor++;
        }
        m_mapStorageLoginInfo.clear();
    }
    for (int i = 0; i < STORAGE_TYPE_MAX; ++i)
    {
        if (NULL != m_pStorageMgr[i])
        {
            m_pStorageMgr[i]->Cleanup();
            delete m_pStorageMgr[i];
            m_pStorageMgr[i] = NULL;
        }
    }
    return TRUE;
}

/**	@fn	    Login
*	@brief	��¼
*	@param  [in] struStorage -- �洢����Ϣ
*	@param  [out] struStorage -- ��¼���
*	@return	LONG -- ��¼���
*/
LONG CStorageMgr::Login(const Struct_StorageConfig &struStorage)
{
    if (struStorage.storageType >= STORAGE_TYPE_MAX)
    {
        ULSERVER_LOG_ERROR("storage type error.ip:%s, port:%d, type:%d",
            struStorage.strIp.c_str(), struStorage.nPort, struStorage.storageType);      
        return STORAGE_LOGIN_FAILED;
    }

    if (NULL != m_pStorageMgr[struStorage.storageType])
    {
        return m_pStorageMgr[struStorage.storageType]->Login(struStorage);
    }
    else
    {
        ULSERVER_LOG_ERROR("storage type:%d did not init", struStorage.storageType);
        return STORAGE_LOGIN_FAILED;
    }
}

/**	@fn	    Logout
*	@brief	�ǳ�
*	@param  [in] ENUM_STORAGE_TYPE -- �洢������
*	@param  [in] lLoginID -- ��¼���
*	@return	BOOL
*/
BOOL CStorageMgr::Logout(const ENUM_STORAGE_TYPE &storageType, const LONG &lLoginID)
{
    if (storageType >= STORAGE_TYPE_MAX)
    {
        ULSERVER_LOG_ERROR("storage type error.type:%d", storageType);      
        return FALSE;
    }

    if (NULL != m_pStorageMgr[storageType])
    {
        return m_pStorageMgr[storageType]->Logout(lLoginID);
    }
    else
    {
        ULSERVER_LOG_ERROR("storage type:%d did not init", storageType);
        return FALSE;
    }
     
}

/**	@fn	    Upload
*	@brief	�ϴ�¼��
*	@param  [in] lLoginID -- ��¼���
*	@param  [in] struRecord -- ¼����Ϣ
*	@return	BOOL
*/
BOOL CStorageMgr::Upload(const LONG &lLoginID, Struct_RecordInfo &struRecord)
{
    if (struRecord.struStorage.storageType >= STORAGE_TYPE_MAX)
    {
        ULSERVER_LOG_ERROR("storage type error.ip:%s, port:%d, type:%d",
            struRecord.struStorage.strIp.c_str(), struRecord.struStorage.nPort, struRecord.struStorage.storageType);      
        return FALSE;
    }

    if (NULL != m_pStorageMgr[struRecord.struStorage.storageType])
    {
        return m_pStorageMgr[struRecord.struStorage.storageType]->Upload(lLoginID, struRecord);
    }
    else
    {
        ULSERVER_LOG_ERROR("storage type:%d did not init", struRecord.struStorage.storageType);
        return FALSE;
    }
}

/**	@fn	    GetUploadStatus
*	@brief	��ȡ�ϴ�״̬
*	@param  [in] lLoginID -- ��¼���
*	@param  [in] struRecord -- ¼����Ϣ
*	@param  [out] struRecord.recordStatus -- ¼��״̬
*	@return	BOOL
*/
BOOL CStorageMgr::GetUploadStatus(const LONG &lLoginID, Struct_RecordInfo &struRecord)
{
    if (struRecord.struStorage.storageType >= STORAGE_TYPE_MAX)
    {
        ULSERVER_LOG_ERROR("storage type error.type:%d", struRecord.struStorage.storageType);      
        return FALSE;
    }

    if (NULL != m_pStorageMgr[struRecord.struStorage.storageType])
    {
        return m_pStorageMgr[struRecord.struStorage.storageType]->GetUploadStatus(lLoginID, struRecord);
    }
    else
    {
        ULSERVER_LOG_ERROR("storage type:%d did not init", struRecord.struStorage.storageType);
        return FALSE;
    }
}

/**	@fn	    GetUploadRecordInfo
*	@brief	��ȡ�洢�豸�����ϴ���¼����Ϣ
*	@param  [in] lLoginID -- ��¼���
*	@param  [in] struRecord -- �ϴ�¼����Ϣ
*	@param  [out] struRecordParam -- ���ϴ����洢�豸��¼����Ϣ
*	@return	BOOL
*/
BOOL CStorageMgr::GetUploadRecordInfo(const LONG &lLoginID, Struct_RecordInfo &struRecord, Struct_UploadRecordParam &struRecordParam)
{
	if (struRecord.struStorage.storageType >= STORAGE_TYPE_MAX)
	{
		ULSERVER_LOG_ERROR("storage type error.type:%d", struRecord.struStorage.storageType);      
		return FALSE;
	}

	if (NULL != m_pStorageMgr[struRecord.struStorage.storageType])
	{
		return m_pStorageMgr[struRecord.struStorage.storageType]->GetUploadRecordInfo(lLoginID, struRecord, struRecordParam);
	}
	else
	{
		ULSERVER_LOG_ERROR("storage type:%d did not init", struRecord.struStorage.storageType);
		return FALSE;
	}
}

/**	@fn	    LoginManage
*	@brief	��¼����
*	@param  [in] struStorage -- �洢�豸��Ϣ
*	@return	LONG -- ��¼���
*/
LONG CStorageMgr::LoginManage(const Struct_StorageConfig &struStorage)
{
    if (STORAGE_TYPE_KMS == struStorage.storageType)
    {
        ULSERVER_LOG_INFO("KMS don't need login.");
        return 0;
    }
	if (STORAGE_TYPE_OBJECT_CLOUD == struStorage.storageType)
	{
		return  Login(struStorage);
	}
    LONG lLoginID = STORAGE_LOGIN_FAILED;
    std::stringstream strFormat ;
    strFormat << struStorage.strIp << "_" << struStorage.nPort;

    //����3��(�豸�Ѿ���½���д���������������ʹ��)��Ҫ�ȴ���������ʹ����ɣ�Ȼ�������µ�½
    int nTryCount = 2;
    do 
    {
        CGuard guard(&m_csMapLoginInfo);
        std::map<std::string, Struct_LoginInfo>::iterator itor = m_mapStorageLoginInfo.find(strFormat.str());
        if (itor != m_mapStorageLoginInfo.end())
        {
            //�Ѿ���½��û�д����ҵ�½ʱ��С�ڣ�INTERNAL_5_MINUTE��
            if ((-1 != itor->second.lLoginID) && (0 == itor->second.nError))
            {
                if (difftime(time(0),itor->second.loginTime) < INTERNAL_5_MINUTE)
                {
                    itor->second.loginTime = time(0);
                    //itor->second.nCount++;
                    return itor->second.lLoginID;
                }
                else
                {
                    //���ó�ʱ����
                    itor->second.nError++;
                }           
            }

            //û�е�½�����Ѿ���½���д�������û��������ʹ�ã���Ҫ��½
            if ((STORAGE_LOGIN_FAILED == itor->second.lLoginID) || ((itor->second.nError > 0)/* && (itor->second.nCount < 1)*/))
            {
                if (STORAGE_LOGIN_FAILED != itor->second.lLoginID)
                {
                    Logout(struStorage.storageType,itor->second.lLoginID);
                }

                lLoginID = Login(struStorage);
                if (STORAGE_LOGIN_FAILED != lLoginID)
                {
                    itor->second.nManType = struStorage.storageType;
                    itor->second.loginTime = time(0);
                    itor->second.lLoginID = lLoginID;
                    //itor->second.nCount = 1;
                    itor->second.nError = 0;
                }

                return lLoginID;    
            }

            ULSERVER_LOG_INFO("device [%s_%d][loginID:%d error:%d count:%d ]Wait for the relanding",
                struStorage.strIp.c_str(),struStorage.nPort,itor->second.lLoginID,
                itor->second.nError,itor->second.nCount);

            //�������е���������:��Ϊ��������ʹ�ø÷�������ľ������Ҫ�ȴ�������������µ�½�豸
        }
        else
        {
            Struct_LoginInfo struLogin;
            struLogin.nManType = struStorage.storageType;
            struLogin.lLoginID = Login(struStorage);

//             if (STORAGE_LOGIN_FAILED != struLogin.lLoginID)
//             {
//                 struLogin.nCount++;         
//             }

            m_mapStorageLoginInfo[strFormat.str()] = struLogin;

            return struLogin.lLoginID;
        }

        //��Ϊ��������ʹ�ø÷�������ľ������Ҫ�ȴ�������������µ�½�豸
        Sleep(200);
    } while (nTryCount-- >= 0);

    return lLoginID;
}

/**	@fn	    LogoutManage
*	@brief	�ǳ�����
*	@param  [in] struStorage -- �洢�豸��Ϣ
*   @param  [in] bError -- �Ƿ��д�����
*	@return	void
*/
void CStorageMgr::LogoutManage(const Struct_StorageConfig& struStorage, BOOL bError)
{
    if (STORAGE_TYPE_KMS == struStorage.storageType)
    {
        ULSERVER_LOG_INFO("KMS don't need logout.");
        return ;
    }
	if (STORAGE_TYPE_OBJECT_CLOUD == struStorage.storageType)
	{
		ULSERVER_LOG_INFO("object cloud don't need logout.");
		return ;
	}
    std::stringstream strFormat ;
    {
        strFormat << struStorage.strIp << "_" << struStorage.nPort;
    }

    CGuard guard(&m_csMapLoginInfo);
    std::map<std::string,Struct_LoginInfo>::iterator itor = m_mapStorageLoginInfo.find(strFormat.str());
    if (itor != m_mapStorageLoginInfo.end())
    {
        //itor->second.nCount--;
        if (bError)
        {
            itor->second.nError++;
        }

        if ((itor->second.nError > 0)/* && (itor->second.nCount < 1)*/)
        {
            Logout(struStorage.storageType,itor->second.lLoginID);

            itor->second.lLoginID = STORAGE_LOGIN_FAILED;
            //itor->second.nCount = 0;
            itor->second.nError = 0;

            ULSERVER_LOG_INFO("device[%s_%d][loginID:%d error:%d count:%d ]is error,No task use the landing handle��Logout device,error:%d",
                struStorage.strIp.c_str(),struStorage.nPort,
                itor->second.lLoginID,itor->second.nError,
                itor->second.nCount);
        }
    }
    else
    {
        ULSERVER_LOG_ERROR("not find device[%s_%d]",
            struStorage.strIp.c_str(),struStorage.nPort);
    }
}

