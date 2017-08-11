/**   @file		FEdit.h
 *    @note		HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief 
 *
 *    @author	jddd
 *    @date		2015/08/13
 *
 *    @note 	
 *    @note 	
 *    @note 	
 *
 *    @warning 	1�������ı�ģʽ�£������Ҫ��ֱ���У�������Ҫ�Ըÿռ�ı�һ�´�С���ܴﵽЧ��
                2��Ҫ����Ϊ�����ı�����Ҫ���� ES_MULTILINE | ES_AUTOHSCROLL ��������
                3������Ϊ�����ǣ���ʱֻ֧��û�й�������ģʽ
 */

#pragma once
#include "FuiBase.h"

// CFEdit

class AFX_EXT_CLASS CFEdit : public CEdit
{
	DECLARE_DYNAMIC(CFEdit)

public:
	CFEdit();
	virtual ~CFEdit();

    BOOL IsFocused() const { return m_bFocus; }
    BOOL IsEmpty() const { return m_bIsEmpty; }

protected:
	DECLARE_MESSAGE_MAP()

    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
    afx_msg void OnNcPaint();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    virtual afx_msg void OnEnChange();
    afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	virtual void PreSubclassWindow();
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
    // �������ֵ�����
    std::map<int, CString> m_mapFont;
    // ������ɫ
    std::map<int, COLORREF> m_mapTextColor;
    // �߿���ɫ
    std::map<int, COLORREF> m_mapBorderColor;
    // ������ʾ���ֵ�����
    CString m_strDefaultText;
    // ������Ƿ���ʾ�������
    BOOL m_bError;
    // ������Ƿ��н���
    BOOL m_bFocus;
    // ������Ƿ�����Ϊ��
    BOOL m_bIsEmpty;
    // �Ƿ�������
    //BOOL m_bInput; 
    // �ؼ��Ƿ�Ϊ������ʽ
    BOOL m_bPassword;
    // �ͻ������λ��ƫ��
    int m_nClientOffsetX;
    int m_nClientOffsetY;
    // �Ƿ�Ϊ����
    BOOL m_bMultiLine;
	// �Ƿ�ΪComboBox�е�Edit��Ͽؼ�
	BOOL m_bIsComboBox;

    //
	CBrush m_brTextBkg;
	CBrush m_brWhiteBkg;

	BOOL m_bCanGetText; // �Ƿ������ȡ�ı�

	// ��ʾ����
	TCHAR m_szTipText[MAX_LEN_TOOLTIP];
private:
    // �ؼ��߿�
    void DrawBorder(CDC* pDC, CRect& rcWnd);    
    // ����������ֱ����
    void VCenterEdit(BOOL bFlag, NCCALCSIZE_PARAMS* lpncsp);
public:
    /**   @brief		������ʾ����
     *    @brief	
     *    @param	
     *    @param	
     *    @return	
     */
    void SetTextFont(int nState, const CString& strFontName);
    /**   @brief		������ʾ������ɫ
     *    @param	
     *    @param	
     *    @return	
     */
    void SetTextColor(int nState, COLORREF color);
    /**   @brief		������ʾ����
     *    @param	
     *    @param	
     *    @return	
     */
    void SetDefaultText(LPCTSTR szDefaultText);
    /**   @brief		���ñ߿���ɫ����̬���н���״̬
     *    @param	
     *    @param	
     *    @return	
     */
    void SetBorderColor(int nState, COLORREF color);
    /**   @brief		�����Ƿ�Ϊ�������״̬
     *    @brief	
     *    @param	
     *    @param	
     *    @return	
     */
    void SetErrorState(BOOL bError = FALSE);

    /**   @brief		����Ϊ������ʽ
     *    @brief	
     *    @param	
     *    @param	
     *    @return	
     */
    void SetPasswordMode(BOOL bSet);

     /**   @brief   ��ȡ�����ı�
     *    @brief	
     *    @param	
     *    @param	
     *    @return	
     */
    void GetPasswordText(CString& strText);

	/*** ComboBoxʹ�õĽӿ� ***/
	/**   @brief		����EditΪComboBox��Ͽؼ�
     *    @brief	
     *    @param	
     *    @param	
     *    @return	
     */
	void SetComboEdit(BOOL bSet){m_bIsComboBox = bSet;}
	void UpdateCtrl(){OnEnChange();}
	CString GetDefaultText() {return m_strDefaultText;}

	// �������ÿؼ�����ʾ����
	void SetTipText(LPCTSTR szTipText);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

};


