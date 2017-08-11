/**
*   @file   SkinTabCtrl.h
*   @note   HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief 
*  
*   @author mazhentong
*   @date   2012/11/03
*
*   @note
*
*   @warning
*/

#ifndef _WATCH_DOG_SKIN_TAB_CTRL_H_
#define _WATCH_DOG_SKIN_TAB_CTRL_H_

class CSkinTabCtrl : public CTabCtrl
{
    DECLARE_DYNAMIC(CSkinTabCtrl)

public:
    CSkinTabCtrl();
    virtual ~CSkinTabCtrl();

protected:
    DECLARE_MESSAGE_MAP()
public:

    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    void DrawTab(CDC * pDC , const CRect & rcItem, BOOL bSelected = FALSE);

    CFont m_fontSel;
    CBitmap m_bitmap;
protected:
    virtual void PreSubclassWindow();
};


#endif
