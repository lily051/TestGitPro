/**
 * @file      FSlider.h
 * @copyright www.hikvision.com
 *
 * @author    JdDd
 * @date      2016/06/14
 *
 * @version   v0.1
 * @note      ���ݻ����ؼ��淶�޸ĵ�CSliderCtrl
 * @note      
 * @warning
*/

#pragma once


// CFSlider

class AFX_EXT_CLASS CFSlider : public CSliderCtrl
{
	DECLARE_DYNAMIC(CFSlider)

public:
	CFSlider();
	virtual ~CFSlider();

protected:
	DECLARE_MESSAGE_MAP()

private:
    virtual void PreSubclassWindow();

    afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    
protected:
    /**
    * @brief      ��ê��
    * @param[in]  pt ê�����������
    * @param[in]  hDC ��ͼ���
    * @return     NULL
    * @note       
    * @see        
    */ 
    void DrawThumb(CPoint& pt, HDC hDC);
    /**
    * @brief      ����
    * @param[in]  
    * @param[in]  
    * @param[out] 
    * @return     
    * @note       
    * @see        
    */ 
    void DrawChannel(CRect rc, int nPos, HDC hDC);

private:
	/*�ؼ����ڲ�����*/
    CString m_strObjectName;
    /*��ͼ��Դ*/
    Image* m_pImgChannel, *m_pImgThumb;

    COLORREF m_clrBkg;
	/*����Ƿ������ͣ*/
    BOOL m_bHover;

public:
	/**
	* @brief      ���ÿؼ��ڲ�����
	* @param[in]  szObjectName �ؼ����ڲ�����
	* @return     NULL
	* @note       
	* @see        
	*/ 
    void SetObjectName(LPCTSTR szObjectName);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


