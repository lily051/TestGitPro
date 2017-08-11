/**	@file Noncopyable.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief		��ʾ�ؽ�ֹ���ƺ͸�ֵ
*	@author		liangjungao
*	@date		2012-3-26
*	@note		�ο�hik����淶
*/
#pragma once
class Noncopyable
{
protected:
	Noncopyable(){}
	virtual ~Noncopyable(){}
private:
	Noncopyable(const Noncopyable&);
	const Noncopyable& operator=(const Noncopyable&);
};