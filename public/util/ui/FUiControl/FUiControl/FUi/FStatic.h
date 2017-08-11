/**   @file		FStatic.h
 *    @note		HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief    1
 *
 *    @author	Zhangyy
 *    @date		2015/07/27
 *
 *    @note 	2
 *    @note 	3
 *    @note 	4
 *
 *    @warning 	5
 */
#pragma once
#include "FuiBase.h"
/*
 1��֧���Զ�������
 2��֧�ֳ����ؼ����ı���...��ʾ
 3��֧�������������ʱ��ָ����ʽ
 4��֧�����ñ���ɫ
*/

// CFStatic

class AFX_EXT_CLASS CFStatic : public CStatic
{
	DECLARE_DYNAMIC(CFStatic)

public:
	CFStatic();
	virtual ~CFStatic();

protected:
    DECLARE_MESSAGE_MAP()

    afx_msg BOOL OnEraseBkgnd(CDC* pDC);    // 
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);  // �������ָ��
    virtual void PreSubclassWindow();   // �޸Ĵ����Ի���
    virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

    void UpdateFace();
private:
    // �Ƿ����ñ���ɫ
    BOOL m_bTransparent;
    // ����ɫ
    COLORREF m_bgColor;
    // ������ɫ
    COLORREF m_clrText;
    // ����������ʽ
    CString m_strTextFontName;
    // �����ͣ��ָ����ʽ
    HCURSOR m_hCursor;
    // ���ֶ��뷽ʽ
    int m_nTextAlign;

	// ��ʾ����
	TCHAR m_szTipText[MAX_LEN_TOOLTIP];
public:	
    /**
    * @brief �����������ʱ��ָ����ʽ
    * @param [in]hCursor ���õ����ָ��
    * @return void
    * @author JdDd
    * @version v1.0.0.0
    * @date 2015-7-27
    * @since 
    * @bug 
    * @warning 
    */
    void SetHoverCursor(HCURSOR hCursor);
    
    /**
    * @brief	����������ʽ
    * @param	
    * @return	
    * @author	zhangyy
    * @version	v0.1
    * @date		2015/07/27
    * @since	
    * @bug		
    * @warning	
    */
    void SetTextFont(const CString& strFontName);
    
    /**   @brief		���ÿؼ��ı���ɫ
     *    @brief	
     *    @param	
     *    @param	
     *    @return	
     */
    void SetBgcolor(BOOL bTransparent, COLORREF color);
    
    /**   @brief		����������ɫ
     *    @brief	
     *    @param	
     *    @param	
     *    @return	
     */
    void SetTextColor(COLORREF color);
    
    /**   @brief		��������
     *    @brief	
     *    @param	
     *    @param	
     *    @return	
     */
    void SetText(LPCTSTR szText);
   
    /**   @brief		�������ֶ��䷽ʽ
     *    @brief	
     *    @param[in] nAlign ����ϵͳ����õ�DT_LEFT��DT_CENTER�ȵ�
     *    @param	
     *    @return	
     */
    void SetTextAlign(int nAlign);

	// �������ÿؼ�����ʾ����
	void SetTipText(LPCTSTR szTipText);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


