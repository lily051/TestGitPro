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
	// ��ȡ������.
	int GetClassType() const;
	// ��ȡ��̬�����������ָ��.
	pfnCreateObject GetConstructor() const;

private:
	// ���־.
	int m_nClassID;
	// �����������ָ��.
	pfnCreateObject const m_pfnCreateObject;
};

// ������,Garbo���ڳ������ʱ�ͷ���Դ.
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

// ��ʵ��,��DVRȡ������,class_id��Ҫ����PLAYTYPE_FLAG,������DVR�豸ֵ�ظ�.
#define IMPLEMENT_CLASS_CREATE(class_id, class_name)\
	CClassInfo class_name::m_ClassInfo(class_id, (pfnCreateObject)class_name::CreateObject);\
	class_name* class_name::m_pObject = NULL;\
	CIFactory* class_name::CreateObject()\
	{ if (NULL == m_pObject) { m_pObject = new class_name; static CGarbo Garbo; } return m_pObject; } \
	int class_name::GetClassType() const\
	{ return m_ClassInfo.GetClassType(); }
	
// SDK��ʼ����������,��ֹ�ڴ�ӳ���������SDK��γ�ʼ��.
#define DELCARE_INITSDK_CONTROL(class_name)\
	static int g_n##class_name##Count = 0;

// SDK��ʼ��.
#define INITIALIZE_PBSDK(class_name)\
	if (g_n##class_name##Count > 0) { g_n##class_name##Count++; return TRUE; }\
	g_n##class_name##Count++;\
	TRACE("@z InitSDK:%s, count:%d", #class_name, g_n##class_name##Count);

// SDK�ͷ�.
#define RELEASE_PBSDK(class_name)\
	g_n##class_name##Count--; if (g_n##class_name##Count > 0) { return; }\
	TRACE("@z ReleaseSDK:%s, count:%d", #class_name, g_n##class_name##Count);


class CIFactory
{
public:
	CIFactory(void);
	virtual ~CIFactory(void);

public:
	// ����Ψһ����,����ڳ������ʱ�Զ��ͷ�.
	static CIFactory* CreateObject(int nClassID);
	// ע����.
	static void RegisterClass(CClassInfo* pClassInfo);
	// ��ȡ��id.
	virtual int GetClassType() const = 0;
	// ��ʼ��SDK.
	virtual BOOL InitSdk(PlaybackSdk::ENUM_DEVICE_TYPE emDeviceType, PlaybackSdk::ENUM_PLAY_TYPE emPlayType, void* const pUserData) = 0;
	// �ͷ�SDK.
	virtual void ReleaseSdk() = 0;
	// �����طſ��ƶ���.
	virtual CPlaybackBase* CreatePBBase() = 0;
};

#endif
