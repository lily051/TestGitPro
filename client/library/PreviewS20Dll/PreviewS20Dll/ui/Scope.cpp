// Scope1.cpp : implementation file
//
//lint -library
#include "stdafx.h"
#include "Scope.h"
#include "../util/Guard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

unsigned __stdcall DealAudioFrameFun(void *pParam)
{
    OutputDebugString(_T("yhj---DealAudioFrameFun---in!\n"));
    CScope* pScope = (CScope*)pParam;
    if (pScope != NULL)
    {
        pScope->DealAudioFrame();
    }
    OutputDebugString(_T("yhj---DealAudioFrameFun---out!\n"));
    return 0;
}

CScope::CScope()
: m_hThread(NULL)
, m_bShowWave(FALSE)
, m_nZoom(1)
{
    for(int i=0; i<MAXVAULENUM; i++)
    {
        m_Vaule[i] = MAXVAULE/2;
    }
    InitializeCriticalSection(&m_csListAudio);
}

CScope::~CScope()
{
    try
    {
        m_bShowWave = FALSE;
        if (m_hThread != NULL)
        {
            WaitForSingleObject(m_hThread,2000);
            CloseHandle(m_hThread);
            m_hThread = NULL;
        }
        //清空buffer
        list<AUDIOFRAME>::iterator iterFrame = m_listAudioFrame.begin();
        while (iterFrame != m_listAudioFrame.end())
        {
            delete []iterFrame->pBuf;
            iterFrame->pBuf = NULL;
            iterFrame ++;
        }
        m_listAudioFrame.clear();
        DeleteCriticalSection(&m_csListAudio);
    }
    catch(...)
    {

    }
}
BEGIN_MESSAGE_MAP(CScope, CStatic)
	//{{AFX_MSG_MAP(CScope)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CScope::OnEraseBkgnd(CDC* pDC) 
{
	GetClientRect(&m_rectClient);
    CBrush brushBack(BACKCOLOR);
	pDC->FillRect(&m_rectClient,&brushBack);
	return TRUE;
}

void CScope::OnPaint() 
{
	CPaintDC dc(this);
    GetClientRect(&m_rectClient);
    DrawVaule(&dc);
}
void CScope::PreSubclassWindow() 
{
	CStatic::PreSubclassWindow();
}

void CScope::DrawVaule( CDC *pDC )
{
    CPen pen(PS_SOLID,2,WAVECOLOR); 
    CPen* pOldPen = pDC->SelectObject (&pen);
    POINT oldPoint,newPoint;
    oldPoint.x = m_rectClient.left;
    oldPoint.y = m_rectClient.top + m_rectClient.Height()/2;
    for(int i=0; i<MAXVAULENUM; i++)
    {
        newPoint.x = m_rectClient.left + i*m_rectClient.Width()/MAXVAULENUM + 1;
        newPoint.y = m_rectClient.top  + m_Vaule[i]*m_rectClient.Height()/MAXVAULE;
        pDC->MoveTo(oldPoint);
        pDC->LineTo(newPoint);
        oldPoint = newPoint;
    }
    pDC->SelectObject(&pOldPen);
}

void CScope::DealAudioFrame()
{
    while (m_bShowWave)
    {
        Sleep(100);
        CGuard  guard(&m_csListAudio);
        int nIndxLast = MAXVAULENUM-1;
        list<AUDIOFRAME>::iterator iterFrame = m_listAudioFrame.begin();
        list<AUDIOFRAME>::iterator iterNextFrame = m_listAudioFrame.begin();
        if (iterNextFrame != m_listAudioFrame.end())
        {
            iterNextFrame ++;
            while (iterNextFrame != m_listAudioFrame.end())
            {
                if (iterFrame->pBuf == NULL)
                {
                    iterFrame ++;
                    iterNextFrame ++;
                    continue;
                }
                long timeSpan = iterFrame->nStamp-iterNextFrame->nStamp;
                int nVauleNum = iterFrame->nFrameRate*timeSpan/1000;
                int nByte = iterFrame->nType/8;
                //去除错误数据
                if(nVauleNum*nByte>iterFrame->nSize)
                {
                    iterFrame ++;
                    iterNextFrame ++;
                    continue;
                }
                for (int i=nVauleNum-1;i>=0;i--)
                {
                    int num = MAXVAULE/2;
                    switch(nByte)
                    {
                    case 1:
                        {
                            char nVaule = *(iterFrame->pBuf+i);
                            num = nVaule*MAXVAULE*m_nZoom/0xff+MAXVAULE/2;
                        }
                        break;
                    case 2:
                        {
                            short nVaule = *(short*)(iterFrame->pBuf+(i*nByte));
                            num = nVaule*MAXVAULE*m_nZoom/0xffff+MAXVAULE/2;
                        }
                        break;
                    default:
                        break;
                    }   
                    if (nIndxLast<0)
                    {
                        break;
                    }
                    if (num > 99)
                    {
                        num = 99;
                    }
                    else if (num < 1)
                    {
                        num = 1;
                    }
                    m_Vaule[nIndxLast] = num;
                    nIndxLast--;
                }
                if (nIndxLast<0)
                {
                    break;
                }
                iterFrame ++;
                iterNextFrame ++;
            }
        }
    }
}

BOOL CScope::OpenScope()
{
//     unsigned nThreadId;
//     m_hThread = (HANDLE)_beginthreadex(NULL,0,DealAudioFrameFun,this,0,&nThreadId);
//     if (m_hThread == NULL)
//     {
//         m_bShowWave = FALSE;
//     }
//     m_bShowWave = TRUE;
//     return m_bShowWave;
	return FALSE;
}

BOOL CScope::CloseScope()
{
    m_bShowWave = FALSE;
    WaitForSingleObject(m_hThread,2000);
    CloseHandle(m_hThread);
    m_hThread = NULL;
    //清空buffer
    list<AUDIOFRAME>::iterator iterFrame = m_listAudioFrame.begin();
    while (iterFrame != m_listAudioFrame.end())
    {
        delete []iterFrame->pBuf;
        iterFrame->pBuf =NULL;
        iterFrame ++;
    }
    m_listAudioFrame.clear();
    //重绘水平线
    for(int i=0; i<MAXVAULENUM; i++)
    {
        m_Vaule[i] = MAXVAULE/2;
    }
    Invalidate();
    return TRUE;
}

void CScope::AddAudioFrame( AUDIOFRAME audioFrame )
{
    CGuard  guard(&m_csListAudio);
    m_listAudioFrame.push_front(audioFrame);
    if(m_listAudioFrame.size()>MAXFRAMENUM)
    {
        AUDIOFRAME backFrame = m_listAudioFrame.back();
        delete []backFrame.pBuf;
        m_listAudioFrame.pop_back();
    }
    Invalidate();
}

BOOL CScope::SetZoomNumber( int nZoom )
{
    if(nZoom<=10 && nZoom>0)
    {
        m_nZoom = nZoom;
        return TRUE;
    }
    return FALSE;
}
