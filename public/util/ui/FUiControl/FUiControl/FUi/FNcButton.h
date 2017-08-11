/**   @file		FNcButton.h
 *    @note		HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief    �Զ��尴ť�ֻ࣬�����ڱ�������
 *
 *    @author	jddd
 *    @date		2015/08/11
 *
 *    @note 	
 *    @note 	
 *    @note 	
 *
 *    @warning 	
 */

#pragma once

class CFNcButton
{
public:
	CFNcButton(void);
	~CFNcButton(void);

private:
    // ������ָ��
	CWnd *m_pParent;
    // ����Ƿ�Hover
	BOOL m_bHover;
    // ��ť�Ƿ���
	BOOL m_bPressed;
    // ��ť�Ƿ񲻿���
	BOOL m_bDisabled;
    // ��ť����
	CString m_strText;
    // ������ɫ
	COLORREF m_clrText;
    // ���ֶ��䷽ʽ
	int m_nTextAlign;
    // ���ֵ�����
	CString m_strTextFont;

    // ��ťID
	int m_nID;
//     // ��ť��λ��
// 	CRect m_rcRect;
    // �ؼ�������
	CString m_strObjectName;
    // ����ͼƬ
	Image* m_pImgBkg;
    // �����ʾ����
    CString m_strToolTip;

public:

	// �Ƿ�ʼ��Hover��ʽ
	BOOL m_bAlwHover;
	// ��ť��λ��
	CRect m_rcRect;

public:
	void DrawButton(CDC* pDC);
	void SetObjectName(const CString& strObjectName);
	BOOL CreateButton(const CString& strText, CRect rcRect, CWnd* pParent, int nID);
	void EnableButton(BOOL bEnable);
	int GetControlID();
	void SetTextFont(const CString& strFontName);
	void GetRect(CRect& rcRect);
	void MoveWindow(CRect rcRect);

    void LButtonDown();
    void LButtonUp();
    void MouseHover();
    void MouseLeave();

    void SetToolTip(const CString& strToolTip);
    CString GetToolTip();

    void SetFixedWidth(const int nWidth);

	// ����ʼ��Hover��ʽ
	void LockHover(BOOL bLock = TRUE);
};
