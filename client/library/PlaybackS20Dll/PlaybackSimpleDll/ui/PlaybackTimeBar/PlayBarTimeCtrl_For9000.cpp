// PlayBarTimeCtrl_For9000.cpp : implementation file
//
//lint -library
#include "stdafx.h"
#include "PlayBarTimeCtrl_For9000.h"
#include ".\playbartimectrl_for9000.h"


extern HBITMAP CreateGradientBMP(HDC hDC, COLORREF cl1, COLORREF cl2, int nWidth, int nHeight, int nDir, int nNumColors);

// CPlayBarTimeCtrl_For9000

IMPLEMENT_DYNAMIC(CPlayBarTimeCtrl_For9000, CPlayBarTimeCtrl_Base)
CPlayBarTimeCtrl_For9000::CPlayBarTimeCtrl_For9000()
{
	m_bShowMidLine = true;
}

CPlayBarTimeCtrl_For9000::~CPlayBarTimeCtrl_For9000()
{

}


BEGIN_MESSAGE_MAP(CPlayBarTimeCtrl_For9000, CPlayBarTimeCtrl_Base)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CPlayBarTimeCtrl_For9000 message handlers
void CPlayBarTimeCtrl_For9000::Wnd_DrawBK(CDC * pDC)
{
	CRect rect(0,0,0,0);
	CRect rectText(0,0,0,0);
	CString strText = _T("");
	int i = 0;

	CBitmap * pOldBmp = NULL;
	CFont   * pOldFont = NULL;
	CPen    * pOldPen = NULL;
	//CBrush  * pOldBrush = NULL;

	CDC bitmapDC;
	HDC hDC = pDC->GetSafeHdc();
	CBitmap  bmp;
	CBitmap bmpBake;
	bitmapDC.CreateCompatibleDC(pDC);

	GetClientRect(rect);

	CMemDCEX memDC(pDC,rect);

	//����һ������ͼƬ��Ϳ�ϱ���ɫ
	bmpBake.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pOldBmp = bitmapDC.SelectObject(&bmpBake);

	DrawBigImage(&bitmapDC);  // ��ȫ���Ĵ�ͼ

	//��Title�ϵ�ͼ������
	CRect rectTitle(0,0,0,0);
	rectTitle = rect;
	rectTitle.bottom = m_rectList.top;

	if(m_bShowMidLine == true)    // yxn20091017
	{
		// yxn20090915
		bitmapDC.SetBkMode(TRANSPARENT);
		bitmapDC.SetTextColor(RGB(255,255,255));
		pOldFont = bitmapDC.SelectObject(&m_font[1]);
		pOldPen = bitmapDC.SelectObject(&m_Pen[0]);


		if(m_bSynchronize == true)
		{
			// ���ƶ�λ��
			//pOldPen = bitmapDC.SelectObject(&m_Pen[1]);
			//bitmapDC.MoveTo(m_timePosSel.uiTimePos,rectTitle.bottom);
			//bitmapDC.LineTo(m_timePosSel.uiTimePos,rect.bottom);

			// ���Ƶ�ǰ��λʱ��
			//CTime cSelTime(m_timePosSel.ctTime);
			//CRect crSelRect(0,0,0,0);
			//SIZE sz;
			//strText = cSelTime.Format("%Y-%m-%d %H:%M:%S");
			//GetTextExtentPoint32(bitmapDC.m_hDC,strText.GetBuffer(0),strText.GetLength(), &sz);
			//crSelRect.left = m_timePosSel.uiTimePos - sz.cx / 2;
			//crSelRect.right = crSelRect.left + sz.cx;
			//crSelRect.top = rect.top + 2;
			//crSelRect.bottom = crSelRect.top + sz.cy;
			//
			//bitmapDC.SetBkMode(TRANSPARENT);
			//bitmapDC.SetTextColor(RGB(255,255,255));
			//bitmapDC.DrawText(strText.GetBuffer(0),strText.GetLength(),&crSelRect,\
			//	/*DT_CENTER |*/ DT_BOTTOM );
		}
		else
		{
			{
				// ���ƶ�λ��
				//pOldPen = bitmapDC.SelectObject(&m_MinPen);
				//bitmapDC.MoveTo(m_timePosSel.uiTimePos,rectTitle.top);
				//bitmapDC.LineTo(m_timePosSel.uiTimePos,rect.bottom);
				// ���Ƶ�ǰ��λʱ��
				//CTime cSelTime(m_timePosSel.ctTime);
				//CRect crSelRect(0,0,0,0);
				//SIZE sz;
				//strText = cSelTime.Format("%Y-%m-%d %H:%M:%S");
				//GetTextExtentPoint32(bitmapDC.m_hDC,strText.GetBuffer(0),strText.GetLength(), &sz);
				//crSelRect.left = m_timePosSel.uiTimePos - sz.cx / 2;
				//crSelRect.right = crSelRect.left + sz.cx;
				//crSelRect.top = rect.top + 2;
				//crSelRect.bottom = crSelRect.top + sz.cy;

				//bitmapDC.SetBkMode(TRANSPARENT);
				//bitmapDC.SetTextColor(RGB(255,255,255));
				//bitmapDC.DrawText(strText.GetBuffer(0),strText.GetLength(),&crSelRect,\
				//	/*DT_CENTER |*/ DT_BOTTOM );
			}

			// ���ƶ�λ��
			pOldPen = bitmapDC.SelectObject(&m_Pen[1]);
			int iSelElemTop = 0;
			int iSelElemBottom = 0;
			unsigned int iPosSel = 0;
			int nIndex = 0;
			for(int i = m_nTopElem;i<m_elementCollect.GetCount(), nIndex<m_nCycleElem; i++, nIndex++)
			{
				iSelElemTop = nIndex * m_iElemHeight - m_iShowTop + m_rectList.top;
				iSelElemBottom = iSelElemTop + m_iElemHeight;

				// yxn20091203
				iSelElemTop = max(iSelElemTop,m_rectList.top);
				iSelElemBottom = max(iSelElemBottom,m_rectList.top);
				if(true == _GetPosByTime(m_timeSingleElem[i],iPosSel) && iSelElemBottom > iSelElemTop)
				{
					bitmapDC.MoveTo(iPosSel,iSelElemTop);
					bitmapDC.LineTo(iPosSel,iSelElemBottom);
				}
			}
		}

			// ���ƹ�����ڵ�ʱ��
			if(m_timePosHover.uiTimePos >= m_timePosStart.uiTimePos \
				&& m_timePosHover.uiTimePos <= m_timePosEnd.uiTimePos)
			{

				CTime cHoverTime(m_timePosHover.ctTime);
				CRect crHoverRect(0,0,0,0);
				SIZE sz;

				strText = cHoverTime.Format("%Y-%m-%d %H:%M:%S");

				//��ȡ��ʾ���ֵĿ��
				GetTextExtentPoint32(bitmapDC.m_hDC,strText.GetBuffer(0),strText.GetLength(), &sz);

				crHoverRect.left = m_timePosHover.uiTimePos - sz.cx / 2;
				crHoverRect.right = crHoverRect.left + sz.cx;
				crHoverRect.top = rect.top + 2;
				crHoverRect.bottom = crHoverRect.top + sz.cy;

				bitmapDC.SetTextColor(UI_TEXT_COLOR);
				bitmapDC.DrawText(strText.GetBuffer(0),strText.GetLength(),&crHoverRect,\
					/*DT_CENTER |*/ DT_BOTTOM );

			}
	}



	// ��ѡ�е�ͨ��ʱ��� yxn20090917
	DrawSelectSection(&bitmapDC);

	memDC.BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&bitmapDC,0,0,SRCCOPY);

	if(pOldBmp)
	{
		bitmapDC.SelectObject(pOldBmp);
		pOldBmp = NULL;
	}

	if(pOldFont)
	{
		bitmapDC.SelectObject(pOldFont);
		pOldFont = NULL;
	}

	if(pOldPen)
	{
		bitmapDC.SelectObject(pOldPen);
		pOldPen = NULL;
	}

    //�������ģ�pOldBrushһֱ����NULL��������ֵ����������޷�ִ�У�
    //����������Ķ���һ��ע�͵�����Ҫʱ���Ը����������
// 	if(pOldBrush)  
// 	{
// 		bitmapDC.SelectObject(pOldBrush);
// 		pOldBrush = NULL;
// 	}

	if(bmpBake.m_hObject)
	{
		DeleteObject(bmpBake.m_hObject);
	}
}

void CPlayBarTimeCtrl_For9000::_InitDC(void)
{
	m_font[0].CreateFont( 16,6,0,0,FW_BOLD,0,0,0,\
		DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH, _T("Arial") );

	m_font[1].CreateFont( 14,5,0,0,FW_NORMAL,0,0,0,\
		DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T("Arial") );

	m_Pen[0].CreatePen(PS_SOLID, 1, UI_PLAYBAR_TITLESPACE);//title�ָ���
	m_Pen[1].CreatePen(PS_SOLID, 1, UI_PLAYBAR_CURTIME);//��ǰʱ�����ɫ
	m_Pen[2].CreatePen(PS_SOLID, 1, UI_PLAYBAR_TITLESPACE);//�ָ���
	m_Pen[3].CreatePen(PS_DASH, 1, UI_PLAYBAR_SELECT);  //ѡ�п������ yxn20090916
	m_Brush[0].CreateSolidBrush(UI_PLAYBAR_BK);//��������ɫ

	m_MinPen.CreatePen(PS_SOLID, 1, UI_PLAYBAR_MINPEN);
}

// ��ѡ�е�Ƭ��
void CPlayBarTimeCtrl_For9000::DrawSelectSection(CDC *pDC)
{
	CPen *pOldPen = pDC->SelectObject(&m_Pen[3]);

	// yxn20090918
	PPLAYBAR_SELECT_SECTION pListTemp = m_pSelectSections;
	while(pListTemp)
	{
		// ����ID��ȡ��������
		int iElemTop = (pListTemp->iElementID)*m_iElemHeight -  m_iShowTop + m_rectList.top +2;
		int iElemBottom = iElemTop + m_iElemHeight - m_iElemSpanHeight - 4;


		// ����ʱ���ȡ��������
		unsigned int iElemLeft = 0;
		unsigned int iElemRight = 0;
		bool bDrawLeft = _GetPosByTime(pListTemp->timeStart,iElemLeft);
		bool bDrawRight = _GetPosByTime(pListTemp->timeEnd,iElemRight);

		bool bDrawTop = true;
		bool bDrawBottom = true;

		if(iElemTop < m_rectList.top || iElemTop > m_rectList.bottom)
		{
			bDrawTop = false;
		}

		if(iElemBottom > m_rectList.bottom || iElemBottom < m_rectList.top)
		{
			bDrawBottom = false;
		}


		// ��
		if(true == bDrawTop)
		{
			if(bDrawLeft == true && bDrawRight == true)
			{
				// ��
				pDC->MoveTo(iElemLeft,iElemTop);
				pDC->LineTo(iElemRight,iElemTop);
			}
			else if(bDrawLeft == true)
			{
				// ��
				pDC->MoveTo(iElemLeft,iElemTop);
				pDC->LineTo(m_rectList.right,iElemTop);
			}
			else if(bDrawRight == true)
			{
				// ��
				pDC->MoveTo(m_uiListStartPos,iElemTop);
				pDC->LineTo(iElemRight,iElemTop);
			}

		}

		// ��
		if(bDrawBottom == true)
		{
			if(bDrawLeft == true && bDrawRight == true)
			{
				// ��
				pDC->MoveTo(iElemLeft,iElemBottom);
				pDC->LineTo(iElemRight,iElemBottom);
			}
			else if(bDrawLeft == true)
			{
				// ��
				pDC->MoveTo(iElemLeft,iElemBottom);
				pDC->LineTo(m_rectList.right,iElemBottom);
			}
			else if(bDrawRight == true)
			{
				// ��
				pDC->MoveTo(m_uiListStartPos,iElemBottom);
				pDC->LineTo(iElemRight,iElemBottom);
			}
		}

		// ��
		if(bDrawLeft == true)
		{
			if(bDrawTop == true && bDrawBottom == true)
			{
				pDC->MoveTo(iElemLeft,iElemTop);
				pDC->LineTo(iElemLeft,iElemBottom);
			}
			else if(bDrawTop == true)
			{
				pDC->MoveTo(iElemLeft,iElemTop);
				pDC->LineTo(iElemLeft,m_rectList.bottom);
			}
			else if(bDrawBottom == true)
			{
				pDC->MoveTo(iElemLeft,m_rectList.top);
				pDC->LineTo(iElemLeft,iElemBottom);
			}
		}


		// ��
		if(bDrawRight == true)
		{
			if(bDrawTop == true && bDrawBottom == true)
			{
				pDC->MoveTo(iElemRight,iElemTop);
				pDC->LineTo(iElemRight,iElemBottom);
			}
			else if(bDrawTop == true)
			{
				pDC->MoveTo(iElemRight,iElemTop);
				pDC->LineTo(iElemRight,m_rectList.bottom);
			}
			else if(bDrawBottom == true)
			{
				pDC->MoveTo(iElemRight,m_rectList.top);
				pDC->LineTo(iElemRight,iElemBottom);
			}
		}

		pListTemp = pListTemp->pNext;
	}

	if(pOldPen)
	{
		pDC->SelectObject(pOldPen);
		pOldPen = NULL;
	}
}

// ��ȫ����ͼ
void CPlayBarTimeCtrl_For9000::DrawBigImage(CDC *pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pDC);

	pDC->FillSolidRect(rcClient, UI_PLAYBAR_TOP);

	// ����ߵ�ͨ����Ϣ yxn20090916
	if(m_bChgLeft == TRUE)
	{
		CreateLeftBitmap(pDC);
		m_bChgLeft = FALSE;
	}

	dcTemp.SelectObject(&m_bmpLeft);
	pDC->BitBlt(0,m_rectList.top,m_uiListStartPos,rcClient.Height()-m_rectList.top,&dcTemp,0,m_iShowTop,SRCCOPY);

	// �������ʱ���� yxn20090916
	if(m_bChgTop == TRUE)
	{
		CreateTopBitmap(pDC);
		m_bChgTop = FALSE;
	}

	dcTemp.SelectObject(&m_bmpTop);
	int iLeft = (int)(((float)((float)(m_timePosStart.ctTime - m_ctStart)*(float)m_uiCellNormalWid)/(float)m_uiCellSeconds) + m_iMoreCellNum*m_uiCellNormalWid/2 + m_iMoreSpan);
	pDC->BitBlt(0/*m_uiListStartPos*/,0,rcClient.Width()/* - m_uiListStartPos*/,m_rectList.top,&dcTemp,iLeft,0,SRCCOPY);

	// ��ͨ�����ļ��б� yxn20090915
	if(m_bChgBack == TRUE)
	{
		CreateBackBitmapByElem(pDC);
		m_bChgBack = FALSE;
	}

	int iElemStartID = m_iShowTop / m_iElemHeight;
	int iElemStartSpace = m_iShowTop%m_iElemHeight;

	int iElemEndID = (m_iShowTop + rcClient.Height() - m_rectList.top) / m_iElemHeight;
	int iElemEndSpace = (m_iShowTop + rcClient.Height() - m_rectList.top) % m_iElemHeight;

	int iShowStart = m_rectList.top;
	int nIndex = 0;
	for(int i = m_nTopElem/*iElemStartID*/; i <= iElemEndID && nIndex<m_nCycleElem; i++, nIndex++)
	{
// 		CString strMsg;
// 		strMsg.Format("%d  %d  ", iElemEndID, iShowStart);
// 		AfxMessageBox(strMsg);

		HGDIOBJ oldObject = dcTemp.SelectObject(m_bmpBackByElement[i]);
		if(i == m_nTopElem)
		{
			pDC->BitBlt(m_uiListStartPos,iShowStart,rcClient.Width()-m_uiListStartPos,m_iElemHeight - iElemStartSpace,&dcTemp,iLeft,iElemStartSpace,SRCCOPY);
			iShowStart = iShowStart + m_iElemHeight - iElemStartSpace;
		}
		else if(i == iElemEndID)
		{
			pDC->BitBlt(m_uiListStartPos,iShowStart,rcClient.Width()-m_uiListStartPos,iElemEndSpace,&dcTemp,iLeft,0,SRCCOPY);
		}
		else
		{
			pDC->BitBlt(m_uiListStartPos,iShowStart,rcClient.Width()-m_uiListStartPos,m_iElemHeight,&dcTemp,iLeft,0,SRCCOPY);
			iShowStart += m_iElemHeight;
		}
		dcTemp.SelectObject(oldObject);
	}

	// ������߽���
	CPen *pOldPen = pDC->SelectObject(&m_Pen[0]);
	pDC->MoveTo(m_uiListStartPos,m_rectList.top);
	pDC->LineTo(m_uiListStartPos,rcClient.bottom);

	if(pOldPen)
	{
		pDC->SelectObject(pOldPen);
		pOldPen = NULL;
	}
}

void CPlayBarTimeCtrl_For9000::CreateTopBitmap(CDC *pDC)
{
	CDC dcTop;
	dcTop.CreateCompatibleDC(pDC);

	if(m_bmpTop.m_hObject)
	{
		DeleteObject(m_bmpTop.m_hObject);
		m_bmpTop.m_hObject = NULL;
	}

	// ȷ�����δ�С
	CRect rcTop(0,0,m_iBackWidth+m_uiListStartPos,m_rectList.top);  //add m_uiListStartPos by yudan at 2012-8-14 13:59

	// ����ͼƬ
	m_bmpTop.CreateCompatibleBitmap(pDC, rcTop.Width(), rcTop.Height());
	dcTop.SelectObject(&m_bmpTop);

	// ��䱳��ɫ����
	CBrush bkTopBrush;
	bkTopBrush.CreateSolidBrush(UI_PLAYBAR_TOP);
	dcTop.SelectObject(bkTopBrush);
	dcTop.Rectangle(&rcTop);

	// ׼��������
	dcTop.SetBkMode(TRANSPARENT);
	dcTop.SetTextColor(UI_TEXT_COLOR);
	dcTop.SelectObject(&m_font[1]);
	dcTop.SelectObject(&m_Pen[0]);

	// �����һ��ʱ��̶��ߵ�λ�ú�ʱ��
	unsigned int uiStartleft = m_iMoreSpan + m_uiListStartPos;  ////add m_uiListStartPos by yudan at 2012-8-14 13:59
	time_t ctStart = m_ctStart - (m_iMoreCellNum*m_uiCellSeconds)/2;

	// ��ʱ��̶�
	CRect rectText = rcTop;
	rectText.bottom = rcTop.bottom - 5;
	CString strText;
	//add m_uiListStartPos by yudan at 2012-8-14 13:59  
	while(uiStartleft < (unsigned int)m_iBackWidth+m_uiListStartPos)
	{
		// �̶���
		dcTop.SelectObject(&m_Pen[0]);
		dcTop.MoveTo(uiStartleft,rcTop.bottom - 4);
		dcTop.LineTo(uiStartleft,rcTop.bottom);

		// ʱ��
		CTime cTime(ctStart);
		SIZE sz;

		strText = cTime.Format("%H:%M");

		//��ȡ��ʾ���ֵĿ��
		GetTextExtentPoint32(dcTop.m_hDC,strText.GetBuffer(0),strText.GetLength(), &sz);

		rectText.top = rectText.bottom - sz.cy;
		rectText.left = uiStartleft - sz.cx / 2;
		rectText.right = uiStartleft + sz.cx;

		dcTop.DrawText(strText.GetBuffer(0),strText.GetLength(),&rectText,\
			/*DT_CENTER |*/ DT_BOTTOM );

			// �����¸��̶��ߵ�λ��
			uiStartleft += m_uiCellNormalWid;
		ctStart += m_uiCellSeconds;
	}

	dcTop.MoveTo(0, 0);
	dcTop.LineTo(m_iBackWidth+m_uiListStartPos, 0);

	dcTop.MoveTo(0,m_rectList.top-1);
	dcTop.LineTo(m_iBackWidth+m_uiListStartPos,m_rectList.top -1);
}

void CPlayBarTimeCtrl_For9000::CreateLeftBitmap(CDC *pDC)
{
	CDC dcLeft;
	dcLeft.CreateCompatibleDC(pDC);

	if(m_bmpLeft.m_hObject)
	{
		DeleteObject(m_bmpLeft.m_hObject);
		m_bmpLeft.m_hObject = NULL;
	}

	// ȷ�����δ�С
	CRect rcLeft(0,0,m_uiListStartPos,m_iBackHeight);

	// ����ͼƬ
	m_bmpLeft.CreateCompatibleBitmap(pDC, rcLeft.Width(), rcLeft.Height());
	dcLeft.SelectObject(&m_bmpLeft);

	// ������
	dcLeft.SelectObject(&m_Brush[0]);
	dcLeft.SelectObject(&m_Pen[0]);
	dcLeft.Rectangle(&rcLeft);

	// ��������ͨ����Ϣ
	unsigned int uiCount = m_elementCollect.GetCount();
	if(uiCount <= 0)
	{
		return;
	}

	dcLeft.SetBkMode(TRANSPARENT);
	dcLeft.SetTextColor(UI_TEXT_COLOR);
	dcLeft.SelectObject(&m_font[1]);

	// ���λ�������
	PLAYBAR_ELEMENT_FILELIST ElementFileList;
	unsigned int uiElementTop = 0;
	CRect rectElement(0,0,0,0);
	int i = m_nTopElem;
	int nIndex = 0;
	while((unsigned int)i < uiCount && nIndex < m_nCycleElem)
	{
		// ��ȡƬ���б�
		ElementFileList = m_elementCollect.GetAt(i);

		// ����ʱ��������
		rectElement = rcLeft;
		rectElement.top = uiElementTop;
		rectElement.bottom = rectElement.top + m_iElemHeight - m_iElemSpanHeight;

		dcLeft.FillSolidRect(&rectElement, UI_PLAYBAR_LEFT);

		// ���ƴ������ͨ����
		rectElement.right = rcLeft.right;
		rectElement.bottom -= 2;
		dcLeft.DrawText(ElementFileList.ElementInfo.czElementName, wcslen(ElementFileList.ElementInfo.czElementName),&rectElement,\
			DT_CENTER | DT_VCENTER );

		// �����¸�ʱ�����Ķ���
		uiElementTop += m_iElemHeight;

		dcLeft.MoveTo(rectElement.left, rectElement.bottom+2);
		dcLeft.LineTo(rectElement.right, rectElement.bottom+2);

		i++;

		nIndex++;
	}
}
void CPlayBarTimeCtrl_For9000::CreateBackBitmapByElem(CDC *pDC)
{
	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pDC);

	CDC dcImage;
	dcImage.CreateCompatibleDC(pDC);

	// ȷ�����δ�С
	CRect rcBack(0,0,m_iBackWidth,m_iElemHeight);

	// ���λ��Ƹ�ͨ��ͼƬ	
	CRect rectFile(0,0,0,0);
	CRect rectElement(0,0,0,0);
	COLORREF crFileColor = RGB(255,0,0);
	PLAYBAR_ELEMENT_FILELIST struElemFileList;
	PSECTIONINFO_FOR9000 pSectionTemp = NULL;
	PPLAYBAR_LOCK_SECTION pLockSections = NULL;
	PPLAYBAR_REMARK_SECTION pRemarkSections = NULL;
	unsigned int uiStartPos = 0;
	unsigned int uiEndPos = 0;
	unsigned int uiElementTop = 0;
	unsigned int i = m_nTopElem;
	WORD wKey = 0;
	 DWORD dwError = 0;
	int nIndex = 0;
	int nSize = m_elementCollect.GetCount();
	for(i; i < (unsigned int)nSize && nIndex < m_nCycleElem; i++, nIndex++)
	{
		if(m_bmpBackByElement[i].m_hObject)
		{
			//delete m_bmpBackByElement[i];
			//m_bmpBackByElement[i] = NULL;
			DeleteObject(m_bmpBackByElement[i].m_hObject);
			m_bmpBackByElement[i].m_hObject = NULL;
		}

		// ����ͼƬ
		if(FALSE == m_bmpBackByElement[i].CreateCompatibleBitmap(pDC,rcBack.Width(),rcBack.Height()))
		{
			dwError = GetLastError();
		}
		dcTemp.SelectObject(&m_bmpBackByElement[i]);

		// ������
		dcTemp.SelectObject(&m_Brush[0]);
		dcTemp.SelectObject(&m_Pen[0]);
		dcTemp.Rectangle(&rcBack);

		// �������ڵ�ʱ����
		dcTemp.SetBkMode(TRANSPARENT);

		// ��ȡƬ���б�
		struElemFileList = m_elementCollect.GetAt(i);

		// ����ʱ��������
		rectElement = rcBack;
		rectElement.top = 0;
		rectElement.bottom = m_iElemHeight - m_iElemSpanHeight;

		dcTemp.FillSolidRect(&rectElement, UI_PLAYBAR_ELEMENT);;

		// ���λ������ļ�
		rectFile = rectElement;
		rectFile.top += 3;
		rectFile.bottom -= 3;
		pSectionTemp = struElemFileList.pSectionList;
		while(pSectionTemp)
		{
			_GetColorOfFileType(pSectionTemp->iDataType,crFileColor);

			if(_GetRectPosByFileTime(pSectionTemp->tTimeStart,pSectionTemp->tTimeEnd,uiStartPos,uiEndPos))
			{
				rectFile.left = uiStartPos;
				rectFile.right = uiEndPos;
				dcTemp.FillSolidRect(&rectFile,crFileColor);
			}

			pSectionTemp = pSectionTemp->pNext;
		}

		// ��������Ϣ
		dcTemp.SelectObject(&m_Pen[3]);
		wKey = struElemFileList.ElementInfo.iElementID;
		if(m_mapElemLockInfo.Lookup(wKey,(void*&)pLockSections))
		{
			//dcImage.SelectObject(&m_bmpLock);
			while(pLockSections)
			{
				if(_GetRectPosByFileTime(pLockSections->timeStart,pLockSections->timeEnd,uiStartPos,uiEndPos))
				{
					dcTemp.BitBlt(uiEndPos -1,0,15,15,&dcImage,0,0,SRCCOPY);
				}
				pLockSections = pLockSections->pNext;
			}
		}

		// ����ǩ��Ϣ
		if(m_mapElemRemarkInfo.Lookup(wKey,(void*&)pRemarkSections))
		{	
			//dcImage.SelectObject(&m_bmpRemark);
			while(pRemarkSections)
			{
				if(_GetRectPosByFileTime(pRemarkSections->timeStart,pRemarkSections->timeEnd,uiStartPos,uiEndPos))
				{
					dcTemp.BitBlt(uiEndPos -1,0,15,15,&dcImage,0,0,SRCCOPY);
				}
				pRemarkSections = pRemarkSections->pNext;
			}
		}
	}
}

void CPlayBarTimeCtrl_For9000::OnDestroy()
{
	CPlayBarTimeCtrl_Base::OnDestroy();
	for(int i = 0; i<5;i++)
	{
		if(m_Pen[i].m_hObject)
		{
			DeleteObject(m_Pen[i].m_hObject);
		}
	}

	for(int i = 0; i<2;i++)
	{
		if(m_font[i].m_hObject)
		{
			DeleteObject(m_font[i].m_hObject);
		}
	}

	for(int i = 0; i<5;i++)
	{
		if(m_Brush[i].m_hObject)
		{
			DeleteObject(m_Brush[i].m_hObject);
		}
	}

	for(int i = 0;i<m_elementCollect.GetCount();i++)
	{
		if(m_bmpBackByElement[i].m_hObject)
		{
			DeleteObject(m_bmpBackByElement[i].m_hObject);
		}
	}

	if(m_bmpLeft.m_hObject)
	{
		DeleteObject(m_bmpLeft.m_hObject);
	}

	if(m_bmpTop.m_hObject)
	{
		DeleteObject(m_bmpTop.m_hObject);
	}

	if (m_MinPen.m_hObject)
	{
		DeleteObject(m_MinPen.m_hObject);
	}
}
