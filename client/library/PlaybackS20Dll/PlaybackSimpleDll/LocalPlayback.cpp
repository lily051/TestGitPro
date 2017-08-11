// LocalPlayback.cpp : 实现文件
//

#include "stdafx.h"
#include "PlaybackSimpleDll.h"
#include "LocalPlayback.h"

#define VIDEOWNDLIST_SHOW 1

// CLocalPlayback 对话框

IMPLEMENT_DYNAMIC(CLocalPlayback, CDialog)

CLocalPlayback::CLocalPlayback(CWnd* pParent /*=NULL*/)
	: CDialog(CLocalPlayback::IDD, pParent)
{
	m_pLocalVedioFile = NULL;
	m_pLocalVedioFilePlay = NULL;
    m_vidoWndshow = 0;
}

CLocalPlayback::~CLocalPlayback()
{
	m_pLocalVedioFile = NULL;
	m_pLocalVedioFilePlay = NULL;
}

void CLocalPlayback::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLocalPlayback, CDialog)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_MESSAGE(MSG_LOCALVEDIO_PLAY, &CLocalPlayback::OnMsgLocalVideoPlay)
END_MESSAGE_MAP()


// CLocalPlayback 消息处理程序

BOOL CLocalPlayback::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CreateChildWnd();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLocalPlayback::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_pLocalVedioFile != NULL)
	{
		m_pLocalVedioFile->DestroyWindow();
		delete m_pLocalVedioFile;
		m_pLocalVedioFile = NULL;
	}
	if (m_pLocalVedioFilePlay != NULL)
	{
		m_pLocalVedioFilePlay->DestroyWindow();
		delete m_pLocalVedioFilePlay;
		m_pLocalVedioFilePlay = NULL;
	}
}

void CLocalPlayback::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}

void CLocalPlayback::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	MoveCtrl();
}

//开始播放文件
LRESULT CLocalPlayback::OnMsgLocalVideoPlay(WPARAM wParam, LPARAM lParam)
{
	if (m_pLocalVedioFile == NULL)
	{
		return 0;
	}
	int nVideoType = 0;
	CString strFilePath = _T("");
	m_pLocalVedioFile->GetFileInfo(nVideoType, strFilePath);
	DWORD dwDirattr = 0;
	PLAYBACKDLL_INFO(_T("OnMsgLocalVideoPlay:%s"), strFilePath);
	dwDirattr = GetFileAttributes(strFilePath);
	if (dwDirattr == 0xFFFFFFFF)
	{
		MsgBoxApi_Show(_T("文件不存在，请确认是否为本地文件或已覆盖!"), MSGBOX_TYPE_ERROR);
		return 0;
	}
	if (m_pLocalVedioFilePlay != NULL)
	{
		m_pLocalVedioFilePlay->StartPlay(nVideoType, strFilePath);
	}
	return 0;
}

// 创建子窗口
void CLocalPlayback::CreateChildWnd(void)
{
	if (NULL == m_pLocalVedioFile)
	{
		m_pLocalVedioFile = new CLocalVideoFile;
		m_pLocalVedioFile->Create(CLocalVideoFile::IDD, this);
		m_pLocalVedioFile->ShowWindow(SW_SHOW);
	}
	if (NULL == m_pLocalVedioFilePlay)
	{
		m_pLocalVedioFilePlay = new CLocalVideoFilePlay;
		m_pLocalVedioFilePlay->Create(CLocalVideoFilePlay::IDD, this);
		m_pLocalVedioFilePlay->ShowWindow(SW_SHOW);
	}

}

// 初始化控件位置
void CLocalPlayback::MoveCtrl(void)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rcLocalVedioFile(rcClient);
    if (VIDEOWNDLIST_SHOW == m_vidoWndshow)
    {
        rcLocalVedioFile.right = 204;
    }
    else
    {
        rcLocalVedioFile.right = 0;
    }
	
	if (m_pLocalVedioFile != NULL)
	{
		m_pLocalVedioFile->MoveWindow(rcLocalVedioFile);
	}

	CRect rcLocalVedioFilePlay(rcClient);
	rcLocalVedioFilePlay.left = rcLocalVedioFile.right;
	if (m_pLocalVedioFilePlay != NULL)
	{
		m_pLocalVedioFilePlay->MoveWindow(rcLocalVedioFilePlay);
	}
}

// 本地回放.
BOOL CLocalPlayback::LocalPlayback(const int nVideoType, const CString csFilePath)
{
	if (NULL != m_pLocalVedioFile)
	{
		m_pLocalVedioFile->LocalPlayback(nVideoType, csFilePath);
        //复用nVideoType作为窗口的区分
        m_vidoWndshow = nVideoType;
        if (VIDEOWNDLIST_SHOW == m_vidoWndshow)
        {
            MoveCtrl();
        }
		return TRUE;
	}
	return FALSE;
}

// 停止本地回放
BOOL CLocalPlayback::StopPlayback()
{
	if (m_pLocalVedioFilePlay != NULL)
	{
		m_pLocalVedioFilePlay->StopPlay();
	}
	return TRUE;
}
