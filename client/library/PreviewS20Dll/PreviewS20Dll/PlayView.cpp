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
Description:	创建窗口
Input:			REALPLAYWND_TYPE emType,窗口种类
				rect 窗口的位置
				pParent 父窗口
				unsigned int uiID 窗口的id
				uiStartWndIndex  起始序号
				uiMaxWndNum      播放窗口数
Output:      	
Return:			是否创建成功
*************************************************/
BOOL CPlayView::CreateWnd(const RECT & rect, CWnd * pParent, 
						  unsigned int uiID,unsigned int uiStartWndIndex,unsigned int uiMaxWndNum)
{
	if(uiMaxWndNum < 0ul || uiMaxWndNum > PREVIEW_WND_MAXNUM)
	{
		return FALSE;
	}

	//创建窗口
	CWnd* pWnd = this;
	if(pWnd->Create(NULL, _T("PlayView"), WS_VISIBLE|WS_CHILD|WS_TABSTOP,rect, pParent, uiID))
	{
		//保存窗口信息
		m_nStartWndIndex = uiStartWndIndex;
		m_nMaxWndIndex = uiMaxWndNum;

		//创建子窗口 
		CreateChildWnd();
		
		return TRUE;
	}
		
	return FALSE;
}

/*************************************************
Function:    	CreateChildWnd
Description:    创建子窗口
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
			//创建窗口
			m_pRealPlayWnd[i]->CreateWnd(CRect(0,0,20,20),this,
				ID_WND_PLAYVIEW_CHILD,i + m_nStartWndIndex);
		}

		//注册窗口指针
		PlayView_WndArrangeOper_AssignPlayWnd(m_pRealPlayWnd[i], i);
	}

	//各个预览窗口排列位置
	_UIFun_Update_OutputPose();

	//第一个窗口设置焦点
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
Description:    销毁子窗口
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
Description:	设置焦点窗口
Input:			nWndIndex 焦点窗口索引
Output:      	
Return:			是否成功
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
Description:	设置焦点窗口最大化
Input:			
Output:      	
Return:			是否成功
*************************************************/
BOOL CPlayView::SetChildWndEnlarge()
{
	_PlayView_WndArrangeOper_SetEnlargeWnd();

	//重画边框
	DrawOutputBorder();

	return TRUE;
}

/*************************************************
Function:    	DrawOutputBorder
Description:	画窗口边框，焦点窗口边框是红色
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
Description:	初始化预览窗口位置
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
Description:	设置画面分割模式
Input:			wMode 分割模式	
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
Description:	获取鼠标所在窗口的索引号
Input:			
Output:      	
Return:			鼠标所在窗口的索引号
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
Description:	焦点窗口窗口中预览
Input:			struCamInfo,监控点信息
Output:      	
Return:			TRUE表示播放成功
*************************************************/
BOOL CPlayView::StartPreview(const PREVIEW_CAMINFO &struPreviewInfo)
{
	//获取焦点窗口索引号
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
Description:	在鼠标停留的窗口预览
Input:			struCamInfo,监控点信息
Output:      	
Return:			TRUE表示播放成功
*************************************************/
BOOL CPlayView::StartPreviewMouseHover(const PREVIEW_CAMINFO &struPreviewInfo)
{
	//获取鼠标停留窗口索引号
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
Description:	指定窗口预览
Input:			UINT uiWndIndex,窗口ID	
				Struct_CamreaInfo stCameraInfo,监控点信息
				int nPresetIndex,需要转到预置点的索引
                BOOL bWriteLog,是否写预览日志
Output:      	
Return:			TRUE表示开始成功
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
Description:	停止指定窗口预览
Input:			UINT uiWndIndex,窗口ID		
Output:      	
Return:			TRUE表示停止成功
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
Description:	停止预览
Input:				
Output:      	
Return:			TRUE表示停止成功
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

	//选中第一个窗口
	SetChildWndZoom(0);
}

/*************************************************
Function:    	GetCamIDFromFocusWnd
Description:	获取焦点realplayviewwnd窗口中监控点ID
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
Description:	获取焦点窗口指针
Input:			
Output:      	
Return:			焦点窗口指针
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

	//画预览窗口边框
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
 *  @brief      检查指定窗口是否正在预览
 *  @param[IN]  uWndIndex: 窗口索引
 *  @return     TRUE: 正在预览，FALSE: 空闲
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
 *  @brief      带宽控制服务器强制关闭指定监控点的预览
 *  @param[IN]  nCamID: 指定监控点
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
                strTips.Format(_T("带宽控制服务强制关闭预览[%s]"), m_pRealPlayWnd[i]->GetChanName());
                m_pRealPlayWnd[i]->StopPreview();
                m_pRealPlayWnd[i]->SetShowMsg(strTips);
                PV_TRACE("带宽控制服务器强制关闭预览[CamID=%d]啦！", nCamID);
                break;
            }
        }
    }
}

///** @fn         BwReRequestPreview
// *  @brief      客户端重连上带宽控制服务器后重新请求正在预览的监控点
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
 *  @brief      检查指定监控点是否有在预览
 *  @param[IN]  nCamID: 指定监控点ID
 *  @return     TRUE: 有，FALSE: 无
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
 *  @brief      设置全屏状态
 *  @param[IN]  bFullScreen: 是否全屏
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
