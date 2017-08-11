/**	@file Singleton.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief 宏定义一个类实现单例需要做的一些工作。
*	@author		liangjungao
*	@date		2012-3-26
*	@note		每个单例类最好将其构造函数和虚析构函数设置为private
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