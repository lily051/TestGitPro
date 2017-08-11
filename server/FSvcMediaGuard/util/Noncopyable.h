/**	@file Noncopyable.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief		显示地禁止复制和赋值
*	@author		liangjungao
*	@date		2012-3-26
*	@note		参考hik编码规范
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