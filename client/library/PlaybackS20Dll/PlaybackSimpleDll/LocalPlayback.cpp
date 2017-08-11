// LocalPlayback.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PlaybackSimpleDll.h"
#include "LocalPlayback.h"

#define VIDEOWNDLIST_SHOW 1

// CLocalPlayback �Ի���

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


// CLocalPlayback ��Ϣ�������

BOOL CLocalPlayback::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CreateChildWnd();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLocalPlayback::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void CLocalPlayback::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	MoveCtrl();
}

//��ʼ�����ļ�
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
		MsgBoxApi_Show(_T("�ļ������ڣ���ȷ���Ƿ�Ϊ�����ļ����Ѹ���!"), MSGBOX_TYPE_ERROR);
		return 0;
	}
	if (m_pLocalVedioFilePlay != NULL)
	{
		m_pLocalVedioFilePlay->StartPlay(nVideoType, strFilePath);
	}
	return 0;
}

// �����Ӵ���
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

// ��ʼ���ؼ�λ��
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

// ���ػط�.
BOOL CLocalPlayback::LocalPlayback(const int nVideoType, const CString csFilePath)
{
	if (NULL != m_pLocalVedioFile)
	{
		m_pLocalVedioFile->LocalPlayback(nVideoType, csFilePath);
        //����nVideoType��Ϊ���ڵ�����
        m_vidoWndshow = nVideoType;
        if (VIDEOWNDLIST_SHOW == m_vidoWndshow)
        {
            MoveCtrl();
        }
		return TRUE;
	}
	return FALSE;
}

// ֹͣ���ػط�
BOOL CLocalPlayback::StopPlayback()
{
	if (m_pLocalVedioFilePlay != NULL)
	{
		m_pLocalVedioFilePlay->StopPlay();
	}
	return TRUE;
}
