/**
 * @file      FSlider.h
 * @copyright www.hikvision.com
 *
 * @author    JdDd
 * @date      2016/06/14
 *
 * @version   v0.1
 * @note      根据基础控件规范修改的CSliderCtrl
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
    * @brief      画锚点
    * @param[in]  pt 锚点的中心坐标
    * @param[in]  hDC 绘图句柄
    * @return     NULL
    * @note       
    * @see        
    */ 
    void DrawThumb(CPoint& pt, HDC hDC);
    /**
    * @brief      画槽
    * @param[in]  
    * @param[in]  
    * @param[out] 
    * @return     
    * @note       
    * @see        
    */ 
    void DrawChannel(CRect rc, int nPos, HDC hDC);

private:
	/*控件的内部名称*/
    CString m_strObjectName;
    /*贴图资源*/
    Image* m_pImgChannel, *m_pImgThumb;

    COLORREF m_clrBkg;
	/*标记是否鼠标悬停*/
    BOOL m_bHover;

public:
	/**
	* @brief      设置控件内部名称
	* @param[in]  szObjectName 控件的内部名称
	* @return     NULL
	* @note       
	* @see        
	*/ 
    void SetObjectName(LPCTSTR szObjectName);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


