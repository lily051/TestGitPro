/** @file AutoExchangeModuleState.h
 *  @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *  @brief DLLģ��״̬�Զ��л�
 *
 *	@author Liu Ronghua
 *  @date 2011/11/18
 */
#pragma once

class AutoExchangeModuleState
{
public:
	AutoExchangeModuleState(HINSTANCE hCurInstance)
	{
		AEMS_PrevInstance = AfxGetResourceHandle();
		AfxSetResourceHandle(hCurInstance);
	}
	~AutoExchangeModuleState(void)
	{
		AfxSetResourceHandle(AEMS_PrevInstance);
	}
private:
	HINSTANCE AEMS_PrevInstance;
};
