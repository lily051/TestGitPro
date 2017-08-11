/**	@file ServiceFactory.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief �������з�����ࡣ�����������ṩ��Щ����ά�������ķ���ȣ�
*	@author		chenbin5
*	@date		2016-3-3
*	@note 
*/
#pragma once
#include "../util/Noncopyable.h"
#include "../util/Singleton.h"
#include "../util/IService.h"
#include "GlobalParam.h"


class CServiceFactory:public Noncopyable, public IService
{
    SINGLETON_DECLARE(CServiceFactory)///<�����������

public:
	/**	@fn	    Init
	*	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
	*/
	int Init();

	/**	@fn	    Fini
	*	@brief	����ʼ��������������Դ�ͷ�
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
	*/
	int Fini();

	/**	@fn	    Start
	*	@brief	��ʼ������������ģ��ĺ��Ĺ��ܣ�һ������Ӧ�ÿ���ĳ���߳̽���ʵ�ʵĹ���
	*          �˺���Ӧ�þ��췵�أ���Ӧ��ռ��̫��ʱ�䣬����С��30s
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
	*/
	int Start();

	/**	@fn	    Stop
	*	@brief	ֹͣ����
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
	*/
	int Stop();

	/**	@fn	    GetVersion
	*	@brief	��ȡ�汾��
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	DWORD�����ذ汾����0x01020304
	*/
	int GetVersion();

	/**	@fn	    GetLastError
	*	@brief	���ش�����
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
	*/
	int GetLastError();
private:
	bool m_bInit;//<�Ƿ��Ѿ���ʼ��
	bool m_bStart;//<�Ƿ��Ѿ�����
private:
    CServiceFactory(void);
    virtual ~CServiceFactory(void);
};
