/********************************************************************
    Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.	
	created:	2009/10/09   21:07 
	filename:	playview.cpp
	author:		GuoLinlin
    Description:  
	Modification History:      
	<version>   <time>        <author>        <desc>	
	   1.0      2009-10-9    GuoLinlin       Created	   
	   1.0      2009-10-9    GuoLinlin       Modify	
	   3.0      2012-4-18    nobody			 Modify	
*********************************************************************/
//lint -library
#include "stdafx.h"
#include "PlayView.h"
#include "ui\memdcex.h"
#include "PreviewS20Dll.h"


IMPLEMENT_DYNAMIC(CPlayView, CWnd)

CPlayView::CPlayView()
: m_nStartWndIndex(0)
, m_nFocusWndIndex(-1)
, m_nMaxWndIndex(0)
, m_bFullScreen(FALSE)
{
	for(int i = 0; i < PREVIEW_WND_MAXNUM; i++)
	{
		m_pRealPlayWnd[i] = NULL;
	}
}

CPlayView::~CPlayView()
{

}


BEGIN_MESSAGE_MAP(CPlayView, CWnd)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/*************************************************
Function:    	CreateWnd
Description:	��������
Input:			REALPLAYWND_TYPE emType,��������
				rect ���ڵ�λ��
				pParent ������
				unsigned int uiID ���ڵ�id
				uiStartWndIndex  ��ʼ���
				uiMaxWndNum      ���Ŵ�����
Output:      	
Return:			�Ƿ񴴽��ɹ�
*************************************************/
BOOL CPlayView::CreateWnd(const RECT & rect, CWnd * pParent, 
						  unsigned int uiID,unsigned int uiStartWndIndex,unsigned int uiMaxWndNum)
{
	if(uiMaxWndNum < 0ul || uiMaxWndNum > PREVIEW_WND_MAXNUM)
	{
		return FALSE;
	}

	//��������
	CWnd* pWnd = this;
	if(pWnd->Create(NULL, _T("PlayView"), WS_VISIBLE|WS_CHILD|WS_TABSTOP,rect, pParent, uiID))
	{
		//���洰����Ϣ
		m_nStartWndIndex = uiStartWndIndex;
		m_nMaxWndIndex = uiMaxWndNum;

		//�����Ӵ��� 
		CreateChildWnd();
		
		return TRUE;
	}
		
	return FALSE;
}

/*************************************************
Function:    	CreateChildWnd
Description:    �����Ӵ���
Input:		
Output:      	
Return:			
*************************************************/
void CPlayView::CreateChildWnd(void)
{
	for(int i = 0; i < m_nMaxWndIndex; i++)
	{
		if (NULL == m_pRealPlayWnd[i])
		{
			m_pRealPlayWnd[i] = new CRealPlayViewWnd;
		}
		
		if(NULL != m_pRealPlayWnd[i])
		{
			//��������
			m_pRealPlayWnd[i]->CreateWnd(CRect(0,0,20,20),this,
				ID_WND_PLAYVIEW_CHILD,i + m_nStartWndIndex);
		}

		//ע�ᴰ��ָ��
		PlayView_WndArrangeOper_AssignPlayWnd(m_pRealPlayWnd[i], i);
	}

	//����Ԥ����������λ��
	_UIFun_Update_OutputPose();

	//��һ���������ý���
	if(NULL != m_pRealPlayWnd[0])
	{
		SetChildWndZoom(0);
	}
}

void CPlayView::OnDestroy()
{
	DestroyChildWnd();
}

/*************************************************
Function:    	DestroyChildWnd
Description:    �����Ӵ���
Input:		
Output:      	
Return:			
*************************************************/
void CPlayView::DestroyChildWnd(void)
{
	for(int i = 0; i < PREVIEW_WND_MAXNUM; i++)
	{
		if(m_pRealPlayWnd[i] != NULL)
		{
			m_pRealPlayWnd[i]->DestroyWindow();
			delete m_pRealPlayWnd[i];
			m_pRealPlayWnd[i] = NULL;
		}
	}
}

/*************************************************
Function:    	SetChildWndZoom
Description:	���ý��㴰��
Input:			nWndIndex ���㴰������
Output:      	
Return:			�Ƿ�ɹ�
*************************************************/
void CPlayView::SetChildWndZoom(int nWndIndex)
{
	UINT uiWndIndwx = (UINT)nWndIndex - m_nStartWndIndex;

	if(uiWndIndwx < 0u || uiWndIndwx > m_nMaxWndIndex)
	{
		return;
	}

	if(m_nFocusWndIndex == uiWndIndwx)
	{
		return;
	}

	if(m_pRealPlayWnd[uiWndIndwx])
	{
		PlayView_WndArrangeOper_SetZoom(m_pRealPlayWnd[uiWndIndwx]);
		m_pRealPlayWnd[uiWndIndwx]->SetFocus();
		DrawOutputBorder();
		m_nFocusWndIndex = uiWndIndwx;
	}
}

/*************************************************
Function:    	SetChildWndEnlarge
Description:	���ý��㴰�����
Input:			
Output:      	
Return:			�Ƿ�ɹ�
*************************************************/
BOOL CPlayView::SetChildWndEnlarge()
{
	_PlayView_WndArrangeOper_SetEnlargeWnd();

	//�ػ��߿�
	DrawOutputBorder();

	return TRUE;
}

/*************************************************
Function:    	DrawOutputBorder
Description:	�����ڱ߿򣬽��㴰�ڱ߿��Ǻ�ɫ
Input:			
Output:      	
Return:			
*************************************************/
void CPlayView::DrawOutputBorder(void)
{
	CDC * pDC= GetDC();

	if(pDC)
	{
		_PlayView_WndArrangeOper_DrawOutputBorder(pDC);

		ReleaseDC(pDC);
		pDC = NULL;
	}
}

void CPlayView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	InitPose();
}

/*************************************************
Function:    	InitPose
Description:	��ʼ��Ԥ������λ��
Input:			
Output:      	
Return:			
*************************************************/
void CPlayView::InitPose(void)
{
	CRect rectClient;
	GetClientRect(&rectClient);
	
	_PlayView_WndArrangeOper_SetSize(rectClient);
}


/*************************************************
Function:    	SetLayoutMode
Description:	���û���ָ�ģʽ
Input:			wMode �ָ�ģʽ	
Output:      	
Return:			
*************************************************/
BOOL CPlayView::SetLayoutMode(WORD wMode)
{
	if(_PlayView_WndArrangeOper_SetWndArrMode(wMode) == TRUE)
	{
		DrawOutputBorder();
		return TRUE;
	}

	return FALSE;
}

/*************************************************
Function:    	GetHoverWndIndex
Description:	��ȡ������ڴ��ڵ�������
Input:			
Output:      	
Return:			������ڴ��ڵ�������
*************************************************/
int CPlayView::GetHoverWndIndex(void)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	int i = 0;
	CRect rectWnd(0,0,0,0);

	for(UINT i = 0; i < m_nMaxWndIndex;i++)
	{
		if(m_pRealPlayWnd[i] && m_pRealPlayWnd[i]->m_hWnd && m_pRealPlayWnd[i]->IsWindowVisible())
		{
			m_pRealPlayWnd[i]->GetWindowRect(&rectWnd);
			ScreenToClient(&rectWnd);

			if(rectWnd.PtInRect(pt))
			{
				return i;
			}
		}
	}

	return -1;
}

/*************************************************
Function:    	StartPreview
Description:	���㴰�ڴ�����Ԥ��
Input:			struCamInfo,��ص���Ϣ
Output:      	
Return:			TRUE��ʾ���ųɹ�
*************************************************/
BOOL CPlayView::StartPreview(const PREVIEW_CAMINFO &struPreviewInfo)
{
	//��ȡ���㴰��������
	UINT uiSelectedIndex = PlayView_WndArrangeOper_GetZoom();
	uiSelectedIndex -= m_nStartWndIndex;

	if (uiSelectedIndex < 0u || uiSelectedIndex > m_nMaxWndIndex)
	{
		return FALSE;
	}

    BOOL bRet = FALSE;
    if (m_pRealPlayWnd[uiSelectedIndex] != NULL)
    {
        bRet = m_pRealPlayWnd[uiSelectedIndex]->StartPreview(struPreviewInfo);
    }

    if (!bRet)
    {
        return FALSE;
    }
	return TRUE;
}

/*************************************************
Function:    	StartPreviewMouseHover
Description:	�����ͣ���Ĵ���Ԥ��
Input:			struCamInfo,��ص���Ϣ
Output:      	
Return:			TRUE��ʾ���ųɹ�
*************************************************/
BOOL CPlayView::StartPreviewMouseHover(const PREVIEW_CAMINFO &struPreviewInfo)
{
	//��ȡ���ͣ������������
	UINT uiHoverWndIndex = GetHoverWndIndex();
	uiHoverWndIndex -= m_nStartWndIndex;

	if (uiHoverWndIndex < 0u || uiHoverWndIndex > m_nMaxWndIndex)
	{
		return FALSE;
	}

    BOOL bRet = FALSE;
    if (m_pRealPlayWnd[uiHoverWndIndex] != NULL)
    {
        bRet = m_pRealPlayWnd[uiHoverWndIndex]->StartPreview(struPreviewInfo);
    }

    return bRet;
}

/*************************************************
Function:    	StartPreviewByWndIndex
Description:	ָ������Ԥ��
Input:			UINT uiWndIndex,����ID	
				Struct_CamreaInfo stCameraInfo,��ص���Ϣ
				int nPresetIndex,��Ҫת��Ԥ�õ������
                BOOL bWriteLog,�Ƿ�дԤ����־
Output:      	
Return:			TRUE��ʾ��ʼ�ɹ�
*************************************************/
BOOL CPlayView::StartPreviewByWndIndex(UINT uiWndIndex, const PREVIEW_CAMINFO &struPreviewInfo, int nPresetIndex, BOOL bWriteLog)
{
	uiWndIndex -= m_nStartWndIndex;
	if (uiWndIndex < 0u || uiWndIndex > m_nMaxWndIndex)
	{
		return FALSE;
	}

    BOOL bRet = FALSE;
	if (m_pRealPlayWnd[uiWndIndex] != NULL)
	{
		bRet = m_pRealPlayWnd[uiWndIndex]->StartPreview(struPreviewInfo);
	}
	
	return bRet;
}

/*************************************************
Function:    	StopPreviewByWndIndex
Description:	ָֹͣ������Ԥ��
Input:			UINT uiWndIndex,����ID		
Output:      	
Return:			TRUE��ʾֹͣ�ɹ�
*************************************************/
BOOL CPlayView::StopPreviewByWndIndex(UINT uiWndIndex,int nPresetIndex)
{
	BOOL bRet = FALSE;

	uiWndIndex -= m_nStartWndIndex;
	if (uiWndIndex < 0u || uiWndIndex > m_nMaxWndIndex)
	{
		return FALSE;
	}

	if (m_pRealPlayWnd[uiWndIndex] != NULL)
	{
		bRet = m_pRealPlayWnd[uiWndIndex]->StopPreview();
	}

	return bRet;
}

/*************************************************
Function:    	StopPreview
Description:	ֹͣԤ��
Input:				
Output:      	
Return:			TRUE��ʾֹͣ�ɹ�
*************************************************/
void CPlayView::StopPreview(void)
{
	for(int i = 0; i < PREVIEW_WND_MAXNUM; i++)
	{
		if(m_pRealPlayWnd[i] != NULL)
		{
			m_pRealPlayWnd[i]->StopPreview();
		}
	}

	//ѡ�е�һ������
	SetChildWndZoom(0);
}

/*************************************************
Function:    	GetCamIDFromFocusWnd
Description:	��ȡ����realplayviewwnd�����м�ص�ID
Input:			
Output:      	
Return:			
*************************************************/
int CPlayView::GetCamIDFromFocusWnd()
{
    if (m_nFocusWndIndex < 0 || m_nFocusWndIndex > m_nMaxWndIndex)
    {
        return -1;
    }

	return m_pRealPlayWnd[m_nFocusWndIndex]->GetCamID();
}

/*************************************************
Function:    	GetFocusWndPointer
Description:	��ȡ���㴰��ָ��
Input:			
Output:      	
Return:			���㴰��ָ��
*************************************************/
CWnd* CPlayView::GetFocusWndPointer()
{
    if (m_nFocusWndIndex < 0 || m_nFocusWndIndex > m_nMaxWndIndex)
    {
        return NULL;
    }

	return (CWnd *)m_pRealPlayWnd[m_nFocusWndIndex];
}

BOOL CPlayView::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	pDC->FillSolidRect(rcClient, theApp.GetUIColor(UI_COLOR_TYPE_PV));

	//��Ԥ�����ڱ߿�
	_PlayView_WndArrangeOper_DrawOutputBorder(pDC);

	return TRUE;
}

void CPlayView::ShowToolBar(int nWndIndex)
{
	for(int i = 0; i < PREVIEW_WND_MAXNUM; i++)
	{
		if(m_pRealPlayWnd[i] != NULL)
		{
			if (i == nWndIndex)
			{
				m_pRealPlayWnd[i]->ShowToolBar(TRUE);
			}
			else
			{
				m_pRealPlayWnd[i]->ShowToolBar(FALSE);
			}
		}
	}
}

/** @fn         IsWndPreview
 *  @brief      ���ָ�������Ƿ�����Ԥ��
 *  @param[IN]  uWndIndex: ��������
 *  @return     TRUE: ����Ԥ����FALSE: ����
 */
BOOL CPlayView::IsWndPreview(UINT uWndIndex)
{
    if(uWndIndex >= 0u && uWndIndex < m_nMaxWndIndex)
    {
        if(m_pRealPlayWnd[uWndIndex])
        {
            return m_pRealPlayWnd[uWndIndex]->IsPreview();
        }
    }
    return FALSE;
}

/** @fn         BwEnforceStopPreview
 *  @brief      ������Ʒ�����ǿ�ƹر�ָ����ص��Ԥ��
 *  @param[IN]  nCamID: ָ����ص�
 *  @return     void
 */
void CPlayView::BwEnforceStopPreview(int nCamID)
{
    for(int i = 0; i < m_nMaxWndIndex; i++)
    {
        if(m_pRealPlayWnd[i] != NULL)
        {
            if (m_pRealPlayWnd[i]->GetCamID() == nCamID && m_pRealPlayWnd[i]->IsPreview())
            {
                CString strTips;
                strTips.Format(_T("������Ʒ���ǿ�ƹر�Ԥ��[%s]"), m_pRealPlayWnd[i]->GetChanName());
                m_pRealPlayWnd[i]->StopPreview();
                m_pRealPlayWnd[i]->SetShowMsg(strTips);
                PV_TRACE("������Ʒ�����ǿ�ƹر�Ԥ��[CamID=%d]����", nCamID);
                break;
            }
        }
    }
}

///** @fn         BwReRequestPreview
// *  @brief      �ͻ��������ϴ�����Ʒ�������������������Ԥ���ļ�ص�
// *  @return     void
// */
//void CPlayView::BwReRequestPreview()
//{
//    for (int i = 0; i < m_nMaxWndIndex; i++)
//    {
//        if (m_pRealPlayWnd[i] != NULL)
//        {
//            if (m_pRealPlayWnd[i]->IsPreview())
//            {
//                m_pRealPlayWnd[i]->BwReRequestPreview();
//            }
//        }
//    }
//}

/** @fn         CheckCameraIsPlaying
 *  @brief      ���ָ����ص��Ƿ�����Ԥ��
 *  @param[IN]  nCamID: ָ����ص�ID
 *  @return     TRUE: �У�FALSE: ��
 */
BOOL CPlayView::CheckCameraIsPlaying(int nCamID)
{
    for (int i = 0; i < m_nMaxWndIndex; i++)
    {
        if (m_pRealPlayWnd[i] != NULL)
        {
            if (m_pRealPlayWnd[i]->GetCamID() == nCamID && m_pRealPlayWnd[i]->IsPreview())
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

/** @fn         SetFullScreenStatus
 *  @brief      ����ȫ��״̬
 *  @param[IN]  bFullScreen: �Ƿ�ȫ��
 *  @return     void
 */
void CPlayView::SetFullScreenStatus(BOOL bFullScreen/* = FALSE*/)
{
    m_bFullScreen = bFullScreen;
    
    for (int i = 0; i < m_nMaxWndIndex; i++)
    {
        if (m_pRealPlayWnd[i] != NULL)
        {
            m_pRealPlayWnd[i]->SetFullScreenStatus(bFullScreen);
        }
    }
}

BOOL CPlayView::StartRecord()
{
	if (m_nFocusWndIndex < 0 || m_nFocusWndIndex > m_nMaxWndIndex)
	{
		return -1;
	}

	return m_pRealPlayWnd[m_nFocusWndIndex]->StartRecord();
}

BOOL CPlayView::StopRecord()
{
	if (m_nFocusWndIndex < 0 || m_nFocusWndIndex > m_nMaxWndIndex)
	{
		return -1;
	}

	PV_INFO("CPlayView::StopRecord");
	return m_pRealPlayWnd[m_nFocusWndIndex]->StopRecord();
}

BOOL CPlayView::OpenSound()
{
    if (m_nFocusWndIndex < 0 || m_nFocusWndIndex > m_nMaxWndIndex)
    {
        return -1;
    }

    return m_pRealPlayWnd[m_nFocusWndIndex]->OpenSound();
}

BOOL CPlayView::CloseSound()
{
    if (m_nFocusWndIndex < 0 || m_nFocusWndIndex > m_nMaxWndIndex)
    {
        return -1;
    }

    return m_pRealPlayWnd[m_nFocusWndIndex]->CloseSound();
}

BOOL CPlayView::GetRecordFile(RECOED_FILE_INFO& recordFile)
{
	if (m_nFocusWndIndex < 0 || m_nFocusWndIndex > m_nMaxWndIndex)
	{
		return -1;
	}

	return m_pRealPlayWnd[m_nFocusWndIndex]->GetRecordFile(recordFile);
}

BOOL CPlayView::SetEventFunc(PV_EVENT_FUNC Func)
{
	if (m_nFocusWndIndex < 0 || m_nFocusWndIndex > m_nMaxWndIndex)
	{
		return -1;
	}

	return m_pRealPlayWnd[m_nFocusWndIndex]->SetEventFunc(Func);
}

void CPlayView::SetFirstWndWave(CScope *pVideoWav, CScope *pAudioWav)
{
	m_pRealPlayWnd[0]->SetFirstWndWave(pVideoWav, pAudioWav);
}

BOOL CPlayView::SetShowString( const CString &strBusinessInfo)
{
	if (m_nFocusWndIndex < 0 || m_nFocusWndIndex > m_nMaxWndIndex)
	{
		return -1;
	}

	return m_pRealPlayWnd[m_nFocusWndIndex]->SetOSDInfo(strBusinessInfo);
}

void CPlayView::OnToolBarExecute(int nBtnType)
{
	m_pRealPlayWnd[m_nFocusWndIndex]->ExcuteSelectOption(nBtnType);
}
