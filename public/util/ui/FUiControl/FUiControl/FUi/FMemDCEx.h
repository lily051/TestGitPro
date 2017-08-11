#ifndef _MEMDC_H
#define _MEMDC_H

//pclint -library
//lint -e1579
namespace FUi
{
    class CMemDCEX : public CDC 
    {
    private:
        CBitmap*	m_bitmap;
        CBitmap*	m_oldBitmap;
        CDC*		m_pDC;
        CRect		m_rcBounds;
    public:
        CMemDCEX(CDC* pDC, const CRect& rcBounds) : CDC()
        {
            CreateCompatibleDC(pDC);
            m_bitmap = new CBitmap;
            m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
            m_oldBitmap = SelectObject(m_bitmap);
            m_pDC = pDC;
            m_rcBounds = rcBounds;
        }
        ~CMemDCEX() 
        {
            try
            {
                m_pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(), 
                    this, /*m_rcBounds.left, m_rcBounds.top*/0,0, SRCCOPY);
                SelectObject(m_oldBitmap);

                m_bitmap->DeleteObject();

                if (m_bitmap != NULL) 
                    delete m_bitmap;
            }
            catch (...)
            {
            }

        }
        CMemDCEX* operator->() 
        {
            return this;
        }
    };
}

//lint +e1579
#endif