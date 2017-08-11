#pragma once
/**	@file    CToolTips.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   ��װMyCreateToolTip  base on ljg
*
*	@author	 yudan
*	@date	 2012/09/10
*	@note   
*	@note    ��ʷ��¼��
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
	//����
	BOOL CreateToolTip(int toolID, HWND hDlg, TCHAR* pText, BOOL bNormal = TRUE);
    //����
	BOOL UpdateToolTip(TCHAR* pText);
    //����
    BOOL UpdateToolTip(int nToolID, HWND hDlg, TCHAR* pText);
	// ��Ӧ��Ϣ.
	void RelayEvent(MSG* pMsg);

};