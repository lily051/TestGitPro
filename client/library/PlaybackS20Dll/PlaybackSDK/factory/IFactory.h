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

#ifndef __IFACTORY_H__
#define __IFACTORY_H__

#include "../PlaybackSdkDef.h"
#include "../PlaybackBase.h"
#include <map>
using namespace std;

class CClassInfo;
class CIFactory;
typedef CIFactory* (*pfnCreateObject)(void);
void Register(CClassInfo* pClassInfo);


class CClassInfo
{
public:
	CClassInfo();
	CClassInfo(int nClassID, pfnCreateObject const pfn);
	virtual ~CClassInfo();

public:
	// 获取类名称.
	int GetClassType() const;
	// 获取动态创建类对象函数指针.
	pfnCreateObject GetConstructor() const;

private:
	// 类标志.
	int m_nClassID;
	// 创建类对象函数指针.
	pfnCreateObject const m_pfnCreateObject;
};

// 类声明,Garbo用于程序结束时释放资源.
#define DECLARE_CLASS_CREATE(class_name)\
	protected:\
	static CClassInfo m_ClassInfo;\
	static class_name* m_pObject;\
	public:\
	static CIFactory* CreateObject();\
	virtual int GetClassType() const;\
	private:\
    class CGarbo\
	{\
	public:\
		~CGarbo()\
		{\
			if (class_name::m_pObject != NULL)\
			{\
				delete class_name::m_pObject; \
				class_name::m_pObject = NULL; \
				TRACE("@z Release %s::m_pObject", #class_name);\
			}\
		}\
	};

// 类实现,非DVR取流对象,class_id需要加上PLAYTYPE_FLAG,避免与DVR设备值重复.
#define IMPLEMENT_CLASS_CREATE(class_id, class_name)\
	CClassInfo class_name::m_ClassInfo(class_id, (pfnCreateObject)class_name::CreateObject);\
	class_name* class_name::m_pObject = NULL;\
	CIFactory* class_name::CreateObject()\
	{ if (NULL == m_pObject) { m_pObject = new class_name; static CGarbo Garbo; } return m_pObject; } \
	int class_name::GetClassType() const\
	{ return m_ClassInfo.GetClassType(); }
	
// SDK初始化控制声明,防止内存映射问题造成SDK多次初始化.
#define DELCARE_INITSDK_CONTROL(class_name)\
	static int g_n##class_name##Count = 0;

// SDK初始化.
#define INITIALIZE_PBSDK(class_name)\
	if (g_n##class_name##Count > 0) { g_n##class_name##Count++; return TRUE; }\
	g_n##class_name##Count++;\
	TRACE("@z InitSDK:%s, count:%d", #class_name, g_n##class_name##Count);

// SDK释放.
#define RELEASE_PBSDK(class_name)\
	g_n##class_name##Count--; if (g_n##class_name##Count > 0) { return; }\
	TRACE("@z ReleaseSDK:%s, count:%d", #class_name, g_n##class_name##Count);


class CIFactory
{
public:
	CIFactory(void);
	virtual ~CIFactory(void);

public:
	// 创建唯一对象,其会在程序结束时自动释放.
	static CIFactory* CreateObject(int nClassID);
	// 注册类.
	static void RegisterClass(CClassInfo* pClassInfo);
	// 获取类id.
	virtual int GetClassType() const = 0;
	// 初始化SDK.
	virtual BOOL InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData) = 0;
	// 释放SDK.
	virtual void ReleaseSdk() = 0;
	// 创建回放控制对象.
	virtual CPlaybackBase* CreatePBBase() = 0;
};

#endif
