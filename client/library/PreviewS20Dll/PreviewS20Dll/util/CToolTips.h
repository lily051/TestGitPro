#pragma once
/**	@file    CToolTips.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   封装MyCreateToolTip  base on ljg
*
*	@author	 yudan
*	@date	 2012/09/10
*	@note   
*	@note    历史记录：
*	@note    V1.0  create at 2012/09/10 by yudan
*/

class CToolTips
{
public:
	CToolTips();
	~CToolTips();

private:
	HWND m_hWnd;
	int m_nToolID;
	HWND m_hDlg;
	BOOL m_bNormal;

public:
	//创建
	BOOL CreateToolTip(int toolID, HWND hDlg, TCHAR* pText, BOOL bNormal = TRUE);
    //更新
	BOOL UpdateToolTip(TCHAR* pText);
    //更新
    BOOL UpdateToolTip(int nToolID, HWND hDlg, TCHAR* pText);
	// 相应消息.
	void RelayEvent(MSG* pMsg);

};