/**   @file	 IFactory.h
 *    @note  HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief 反射类,根据类名字符串动态创建类对象。
 *
 *    @author zpq
 *    @date   2012/10/27
 *
 *    @note 
 *    @note 
 *    @note 历史记录：
 *    @note V1.0.0  
 *
 *    @warning 继承CIFactory的子类需要调用DECLARE_CLASS_CREATE和IMPLEMENT_CLASS_CREATE宏.
 *      SDK初始化需要调用DELCARE_INITSDK_CONTROL  INITIALIZE_PBSDK  RELEASE_PBSDK三个宏.
 */
#include "stdafx.h"
#include "IFactory.h"

// 类管理哈希表.
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
 *    @brief   类信息注册者.
 *    @param c pClassInfo:类信息指针.
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
	// 注册包含自己的父类.
	Register(this);
}

CClassInfo::~CClassInfo()
{

}

/**   @fn	   const string CClassInfo::GetClassType() const
 *    @brief   获取类id.
 *    @param	
 *    @param	
 *    @return  类名.
 */
int CClassInfo::GetClassType() const
{
	return m_nClassID;
}

/**   @fn      pfnCreateObject CClassInfo::GetConstructor() const
 *    @brief   获取动态创建类对象函数指针.
 *    @param   
 *    @param   
 *    @return  函数指针.
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
 *    @brief   动态创建对象.
 *    @param   nClassID:类ID.
 *    @param   
 *    @return  对象指针.
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
		PLAYBACKSDK_ERROR("找不到相关对象:%d", nClassID);
		return NULL;
	}

	PLAYBACKSDK_INFO("创建操作管理对象:%d", nClassID);

	pfnCreateObject const pfnCreObj = ite->second->GetConstructor();
	return pfnCreObj();
}

/**   @fn      void CDynBase::RegisterClass(CClassInfo* pClassInfo)
 *    @brief   提供类具体注册功能.
 *    @param   pClassInfo:类信息指针.
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
			strDeb.Format("@z准备对象:%d 完毕", pClassInfo->GetClassType());
			OutputDebugString(strDeb);
			pair<map<int, CClassInfo*>::iterator, BOOL> ret;
			ret = g_pClassInfoMap->insert(pair<int, CClassInfo*>(pClassInfo->GetClassType(), pClassInfo));
			if (!ret.second)
			{
				PLAYBACKSDK_ERROR("插入对象失败:%d", pClassInfo->GetClassType());
				return;
			}
		}
	}
}
