/**	@file IService.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief ÿ����������Ҫʵ�ֵĽӿ�
*	@author		liangjungao
*	@date		2012-3-27
*	@note		ÿ����������ý��乹�캯������������������Ϊprivate
*/

#pragma once

/**	@class IService
*	@brief IService�ӿڶ����ࡣ
*/
class IService
{
public:
	/**	@fn	    Init
	*	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
	*/
	virtual int Init() = 0;


	/**	@fn	    Fini
	*	@brief	����ʼ��������������Դ�ͷ�
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
	*/
	virtual int Fini() = 0;

	/**	@fn	    Start
	*	@brief	��ʼ������������ģ��ĺ��Ĺ��ܣ�һ������Ӧ�ÿ���ĳ���߳̽���ʵ�ʵĹ���
	*          �˺���Ӧ�þ��췵�أ���Ӧ��ռ��̫��ʱ�䣬����С��30s
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
	*/
	virtual int Start() = 0;

	/**	@fn	    Stop
	*	@brief	ֹͣ����
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
	*/
	virtual int Stop() = 0;

	/**	@fn	    GetVersion
	*	@brief	��ȡ�汾��
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	DWORD�����ذ汾����0x01020304
	*/
	virtual int GetVersion() = 0;

	/**	@fn	    GetLastError
	*	@brief	���ش�����
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
	*/
	virtual int GetLastError() = 0;
public:
	IService(void){};
	virtual ~IService(void){};
};