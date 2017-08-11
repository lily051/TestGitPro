/**	@file Singleton.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief �궨��һ����ʵ�ֵ�����Ҫ����һЩ������
*	@author		liangjungao
*	@date		2012-3-26
*	@note		ÿ����������ý��乹�캯������������������Ϊprivate
*/
#pragma once
#include <new>

#define SINGLETON_DECLARE(theclass)		\
public:									\
	static theclass * GetSingletonPtr();\
	static void FreeSingleton();		\
private:								\
	static theclass * m_s##theclass;	\

#define SINGLETON_IMPLEMENT(theclass)		\
theclass * theclass::m_s##theclass = NULL;	\
theclass * theclass::GetSingletonPtr()		\
{											\
	if (NULL == m_s##theclass)				\
	{										\
	    m_s##theclass = new(std::nothrow) theclass;	\
	}										\
	return m_s##theclass;					\
}											\
void theclass::FreeSingleton()				\
{											\
	if (NULL != m_s##theclass)				\
	{										\
		delete m_s##theclass;				\
        m_s##theclass = NULL;              \
	}										\
}											\

//////////////////////////////////////////////////////////////////////////