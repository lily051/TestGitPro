//lint -library
#include "stdafx.h"
#include "TipHeader.h"
#include "TipDlg.h"
#include <vector>
#include <list>

static bool g_bInit = false;
static int g_nMaxDlgNum = 1;
static std::vector<CTipDlg*> g_vcpTipDlg;
static std::list<MSG_DETAIL> g_lsMsgDetail;
static CRITICAL_SECTION m_csMsg;
static CRITICAL_SECTION m_csDlgPtr;

static HANDLE g_hHandleMsg = NULL, g_hExitEvent = NULL;
unsigned _stdcall WT_HandleMsg(void *pParam);

CTipDlg* GetOneFreeDlg(int &nIndex);

bool InitTipDlg(const int nMaxDlgNum /* = 2*/)
{
	if (g_bInit)
	{
		return true;
	}
    if (nMaxDlgNum <= 0)
    {
        return false;
    }

    g_nMaxDlgNum = nMaxDlgNum;
    for (int i = 0; i < nMaxDlgNum; i++)
    {
        CTipDlg *pDlg = new CTipDlg;
        pDlg->Create(IDD_DLG_TIP, CWnd::FromHandle(GetDesktopWindow()));
        pDlg->ShowWindow(SW_HIDE);

        g_vcpTipDlg.push_back(pDlg);
    }

	InitializeCriticalSection(&m_csMsg);
	InitializeCriticalSection(&m_csDlgPtr);

    if (NULL == g_hExitEvent)
    {
        g_hExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (NULL == g_hExitEvent)
        {
            DestoryTipDlg();
            return false;
        }
    }
    if (NULL == g_hHandleMsg)
    {
        g_hHandleMsg = (HANDLE)_beginthreadex(NULL, 0, &WT_HandleMsg, NULL, 0, NULL);
        if (NULL == g_hHandleMsg)
        {
            DestoryTipDlg();
            return false;
        }
    }
	g_bInit = true;

    return true;
}

bool ShowTipMsg(MSG_DETAIL &msgDetail)
{
	if (!g_bInit)
	{
		return false;
	}
    EnterCriticalSection(&m_csMsg);
    g_lsMsgDetail.push_back(msgDetail);
    LeaveCriticalSection(&m_csMsg);
	
	return true;
}

bool DestoryTipDlg()
{
	g_bInit = false;

	if (g_hHandleMsg)
	{
		SetEvent(g_hExitEvent);
		WaitForSingleObject(g_hHandleMsg, INFINITE);
		CloseHandle(g_hHandleMsg);
		g_hHandleMsg = NULL;
	}
	if (g_hExitEvent)
	{
		CloseHandle(g_hExitEvent);
		g_hExitEvent = NULL;
	}

    for (size_t i = 0; i < g_vcpTipDlg.size(); i++)
    {
        CTipDlg *pDlg = g_vcpTipDlg[i];
		pDlg->DestoryTips();
        pDlg->DestroyWindow();
        delete pDlg;
        pDlg = NULL;
    }
    g_vcpTipDlg.clear();
    g_lsMsgDetail.clear();

	DeleteCriticalSection(&m_csDlgPtr);
    DeleteCriticalSection(&m_csMsg);

    return true;
}

CTipDlg *GetOneFreeDlg(int &nIndex)
{
	for (int i = 0; i < g_nMaxDlgNum; i++)
	{
		if (g_vcpTipDlg[i] != NULL)
		{
			nIndex = i;
			return g_vcpTipDlg[i];
		}
	}
	nIndex = -1;
	return NULL;
}

unsigned _stdcall WT_HandleMsg(void *pParam)
{
    while (true)
    {
        TRACE("msg size %d\n", g_lsMsgDetail.size());
        
        if (WAIT_OBJECT_0 == WaitForSingleObject(g_hExitEvent, 500))
        {
            return 0;
        }

		int nIndex = -1;
		CTipDlg *pFreeDlg = GetOneFreeDlg(nIndex);
		if (NULL == pFreeDlg)
		{
			continue;
		}
		if (-1 == nIndex)
		{
			continue;
		}

		if (0 == g_lsMsgDetail.size())
		{
			continue;
		}
		EnterCriticalSection(&m_csMsg);
		MSG_DETAIL msgDetail = g_lsMsgDetail.front();
		g_lsMsgDetail.pop_front();
		LeaveCriticalSection(&m_csMsg);

		pFreeDlg->ShowTips(msgDetail.strCaption, msgDetail.strMsg, nIndex, msgDetail.tdp);
    }
    return 0;
}
