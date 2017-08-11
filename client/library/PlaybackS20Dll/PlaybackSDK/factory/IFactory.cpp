/**   @file	 IFactory.h
 *    @note  HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief ������,���������ַ�����̬���������
 *
 *    @author zpq
 *    @date   2012/10/27
 *
 *    @note 
 *    @note 
 *    @note ��ʷ��¼��
 *    @note V1.0.0  
 *
 *    @warning �̳�CIFactory��������Ҫ����DECLARE_CLASS_CREATE��IMPLEMENT_CLASS_CREATE��.
 *      SDK��ʼ����Ҫ����DELCARE_INITSDK_CONTROL  INITIALIZE_PBSDK  RELEASE_PBSDK������.
 */
#include "stdafx.h"
#include "IFactory.h"

// ������ϣ��.
static map<int, CClassInfo*>* g_pClassInfoMap = NULL;
struct _ClassInfoFinish 
{
	_ClassInfoFinish(){}
	~_ClassInfoFinish()
	{
		delete g_pClassInfoMap;
		g_pClassInfoMap = NULL;
	}
} ClassInfoFinish;

/**   @fn	   void Register(CClassInfo* pClassInfo)
 *    @brief   ����Ϣע����.
 *    @param c pClassInfo:����Ϣָ��.
 *    @param n 
 *    @return  
 */
void Register(CClassInfo* pClassInfo)
{
	CIFactory::RegisterClass(pClassInfo);
}

CClassInfo::CClassInfo()
: m_nClassID(0)
, m_pfnCreateObject(NULL)
{

}

CClassInfo::CClassInfo(int nClassID, pfnCreateObject const pfn)
: m_nClassID(nClassID)
, m_pfnCreateObject(pfn)
{
	// ע������Լ��ĸ���.
	Register(this);
}

CClassInfo::~CClassInfo()
{

}

/**   @fn	   const string CClassInfo::GetClassType() const
 *    @brief   ��ȡ��id.
 *    @param	
 *    @param	
 *    @return  ����.
 */
int CClassInfo::GetClassType() const
{
	return m_nClassID;
}

/**   @fn      pfnCreateObject CClassInfo::GetConstructor() const
 *    @brief   ��ȡ��̬�����������ָ��.
 *    @param   
 *    @param   
 *    @return  ����ָ��.
 */
pfnCreateObject CClassInfo::GetConstructor() const
{
	return m_pfnCreateObject;
}


CIFactory::CIFactory(void)
{
}

CIFactory::~CIFactory(void)
{
}

/**   @fn      CDynBase* CDynBase::CreateObject(string strClassName)
 *    @brief   ��̬��������.
 *    @param   nClassID:��ID.
 *    @param   
 *    @return  ����ָ��.
 */
CIFactory* CIFactory::CreateObject(int nClassID)
{
	if (NULL == g_pClassInfoMap)
	{
		PLAYBACKSDK_ERROR("g_pClassInfoMap is NULL");
		return NULL;
	}

	map<int, CClassInfo*>::const_iterator ite = g_pClassInfoMap->find(nClassID);
	if (g_pClassInfoMap->end() == ite)
	{
		PLAYBACKSDK_ERROR("�Ҳ�����ض���:%d", nClassID);
		return NULL;
	}

	PLAYBACKSDK_INFO("���������������:%d", nClassID);

	pfnCreateObject const pfnCreObj = ite->second->GetConstructor();
	return pfnCreObj();
}

/**   @fn      void CDynBase::RegisterClass(CClassInfo* pClassInfo)
 *    @brief   �ṩ�����ע�Ṧ��.
 *    @param   pClassInfo:����Ϣָ��.
 *    @param   
 *    @return  
 */
void CIFactory::RegisterClass(CClassInfo* pClassInfo)
{
	if (NULL == g_pClassInfoMap)
	{
		g_pClassInfoMap = new map<int, CClassInfo*>();
	}
	if (NULL != g_pClassInfoMap)
	{
		map<int, CClassInfo*>::const_iterator ite = g_pClassInfoMap->find(pClassInfo->GetClassType());
		if (g_pClassInfoMap->end() == ite)
		{
			CString strDeb;
			strDeb.Format("@z׼������:%d ���", pClassInfo->GetClassType());
			OutputDebugString(strDeb);
			pair<map<int, CClassInfo*>::iterator, BOOL> ret;
			ret = g_pClassInfoMap->insert(pair<int, CClassInfo*>(pClassInfo->GetClassType(), pClassInfo));
			if (!ret.second)
			{
				PLAYBACKSDK_ERROR("�������ʧ��:%d", pClassInfo->GetClassType());
				return;
			}
		}
	}
}
