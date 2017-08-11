// DlgConvertPlay.cpp : 实现文件
//

#include "stdafx.h"
//#include "PlaybackDll.h"
#include "DlgConvertPlay.h"
#include "../util/Markup.h"
#define Size_Width  220
#define Size_Heith  165
// CDlgConvertPlay 对话框

IMPLEMENT_DYNAMIC(CDlgConvertPlay, CDialog)

CDlgConvertPlay::CDlgConvertPlay(BOOL bConvert,CONVERTPLAY_PARAM stConvertPlay,
                                 int nXPos, int nYPos, CString strXml,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConvertPlay::IDD, pParent)
	,m_bConvert(bConvert)
	,m_stConvertPlay(stConvertPlay)
	,m_strXml(strXml)
    ,m_nXPos(nXPos)
    ,m_nYPos(nYPos)
{
}

CDlgConvertPlay::~CDlgConvertPlay()
{
}

void CDlgConvertPlay::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_Resolution, m_cmbResolution);
    DDX_Control(pDX, IDC_COMBO_VideoBitrate, m_cmbVedioBitrate);
    DDX_Control(pDX, IDC_COMBO_VideoFrameRate, m_cmbVedioFrameRate);
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Control(pDX, IDCANCEL, m_btnCancel);
    DDX_Control(pDX, IDC_STATIC_TITLE, m_sttTitle);
    DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
    DDX_Control(pDX, IDC_STATIC_res, m_sttRes);
    DDX_Control(pDX, IDC_STATIC_bit, m_sttBit);
    DDX_Control(pDX, IDC_STATIC_FRAME, m_sttFrame);
    DDX_Control(pDX, IDC_STATIC_line1, m_line1);
}


BEGIN_MESSAGE_MAP(CDlgConvertPlay, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgConvertPlay::OnBnClickedOk)
    ON_WM_CTLCOLOR()
    ON_WM_NCHITTEST()
    ON_BN_CLICKED(IDC_BTN_CLOSE, &CDlgConvertPlay::OnBnClickedBtnClose)
END_MESSAGE_MAP()


// CDlgConvertPlay 消息处理程序

BOOL CDlgConvertPlay::OnInitDialog()
{
	CDialog::OnInitDialog();

    m_sttTitle.SetWindowText(_T("转码设置"));
	InitCtrl();
	ParseXml();
	InitData();
    InitPos();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgConvertPlay::InitCtrl()
{
    m_sttTitle.SetTransparent(TRUE);
    m_sttTitle.SetFontBold(TRUE);
    m_sttTitle.SetTextColor(RGB(66,66,66));
    m_sttRes.SetTextColor(RGB(66,66,66));
    m_sttBit.SetTextColor(RGB(66,66,66));
    m_sttFrame.SetTextColor(RGB(66,66,66));

	m_btnOK.Load(RESOURCELOAD_PNG_EX_IMG(IDB_PNG_BTNHOVERBK),RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SELBK));
	m_btnCancel.Load(RESOURCELOAD_PNG_EX_IMG(IDB_PNG_BTNHOVERBK),RESOURCELOAD_PNG_EX_IMG(IDB_PNG_SELBK));
    m_btnClose.LoadbakeImage(RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CLOSE_BK), RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CLOSE_NOR)
        ,RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CLOSE_BK),RESOURCELOAD_PNG_EX_IMG(IDB_PNG_CLOSE_HOVER));
}

void CDlgConvertPlay::InitData()
{
	int nResSel = 0;
	int nFraSel = 0;
	int nBirSel = 0;
	m_cmbResolution.ResetContent();
	std::map<int, STRU_ABILITY>::iterator itor = m_mapAbility.begin();
	for (int i = 0; itor != m_mapAbility.end(); itor++,i++)
	{
		CString strDesc;
		if (itor->second.struRelolution.nValue == 255)
		{
			strDesc = _T("Auto(当前分辨率)");
			if (!m_bConvert)
			{
				nResSel = i;
			}
		}
		else
		{
			strDesc = itor->second.struRelolution.strDesc;
		}
		m_cmbResolution.InsertString(i,strDesc);
		m_cmbResolution.SetItemData(i, itor->second.struRelolution.nValue);
		if (m_bConvert && m_stConvertPlay.byResolution == itor->second.struRelolution.nValue)
		{
			nResSel = i;
		}
	}

	m_cmbResolution.SetCurSel(nResSel);
	int nValue = m_cmbResolution.GetItemData(m_cmbResolution.GetCurSel());
	std::map<int, STRU_ABILITY>::iterator itor_cmb = m_mapAbility.find(nValue);
	if (itor_cmb == m_mapAbility.end())
	{
		return;
	}

	m_cmbVedioFrameRate.ResetContent();
	std::list<STRU_ABILITYDESC>::iterator itorFrame = itor_cmb->second.listAbilityFrame.begin();
	for (int i = 0; itorFrame != itor_cmb->second.listAbilityFrame.end(); itorFrame++,i++)
	{
		m_cmbVedioFrameRate.InsertString(i,itorFrame->strDesc);
		m_cmbVedioFrameRate.SetItemData(i, itorFrame->nValue);
		if (m_bConvert && m_stConvertPlay.dwVideoFrameRate == itorFrame->nValue)
		{
			nFraSel = i;
		}
	}
	m_cmbVedioFrameRate.SetCurSel(nFraSel);

	m_cmbVedioBitrate.ResetContent();
	std::list<STRU_ABILITYDESC>::iterator itorBit = itor_cmb->second.listAbilityBitrate.begin();
	for (int i = 0; itorBit != itor_cmb->second.listAbilityBitrate.end(); itorBit++,i++)
	{
		m_cmbVedioBitrate.InsertString(i,itorBit->strDesc);
		m_cmbVedioBitrate.SetItemData(i, itorBit->nValue);
		if (m_bConvert && m_stConvertPlay.dwVideoBitrate == itorBit->nValue)
		{
			nBirSel = i;
		}
	}
	m_cmbVedioBitrate.SetCurSel(nBirSel);

}

void CDlgConvertPlay::OnBnClickedOk()
{
	m_stConvertPlay.byResolution = (BYTE)m_cmbResolution.GetItemData(m_cmbResolution.GetCurSel());
	m_stConvertPlay.dwVideoBitrate = m_cmbVedioBitrate.GetItemData(m_cmbVedioBitrate.GetCurSel());
	m_stConvertPlay.dwVideoFrameRate = m_cmbVedioFrameRate.GetItemData(m_cmbVedioFrameRate.GetCurSel());

	OnOK();
}

void CDlgConvertPlay::ParseXml()
{
	CMarkup xml;
	if(!xml.SetDoc(m_strXml.GetBuffer(0)))
	{
        PLAYBACKDLL_ERROR(_T("SetDoc error, xml:%s"),m_strXml);
		return;
	}

	if (!xml.FindElem(_T("RecordAbility version=\"2.0\"")))
	{
        PLAYBACKDLL_ERROR(_T("FindElem RecordAbility version=\"2.0\" error"));
		return;
	}
	xml.IntoElem();

	if (!xml.FindElem(_T("PlayBack")))
	{
        PLAYBACKDLL_ERROR(_T("FindElem PlayBack error"));
		return;
	}
	xml.IntoElem();

	if (!xml.FindElem(_T("PlayConvert")))
	{
        PLAYBACKDLL_ERROR(_T("FindElem PlayConvert error"));
		return;
	}
	xml.IntoElem();

	if (!xml.FindElem(_T("VideoResolutionList")))
	{
        PLAYBACKDLL_ERROR(_T("FindElem VideoResolutionList error"));
		return;
	}
	xml.IntoElem();

	while(xml.FindElem(_T("VideoResolutionEntry")))
	{
		xml.IntoElem();

		STRU_ABILITY struAbility;
		if (xml.FindElem(_T("Index")))
		{
			struAbility.struRelolution.nValue = My_atoi(xml.GetData());
		}
		if (xml.FindElem(_T("Name")))
		{
			struAbility.struRelolution.strDesc = xml.GetData();
		}

		if (xml.FindElem(_T("VideoFrameRate")))
		{
			CString strTmp = xml.GetData();	
			ParseString(0, strTmp, struAbility.listAbilityFrame);
		}

		if (xml.FindElem(_T("VideoBitrate")))
		{
			xml.IntoElem();
			if (xml.FindElem(_T("Range")))
			{
				CString strTmp = xml.GetData();
				ParseString(1, strTmp, struAbility.listAbilityBitrate);
			}
			xml.OutOfElem();
		}

		m_mapAbility[struAbility.struRelolution.nValue] = struAbility;
		xml.OutOfElem();
	}

}

void CDlgConvertPlay::ParseString(int nType, CString strSource, std::list<STRU_ABILITYDESC>& listAbility)
{
	CString strTmp(strSource);
	int nPos = 0;
	int nPre = 0;
	nPos = strTmp.Find(',');
	while(nPos != -1)
	{
		STRU_ABILITYDESC struNode;
		struNode.nValue = My_atoi(strTmp.Left(nPos));
		if (nType == 0)
		{
			//帧率
			struNode.strDesc = GetFrameDesc(struNode.nValue);
		}
		else
		{
			//码率
			struNode.strDesc = GetBitrateDesc(struNode.nValue);
		}

		listAbility.push_back(struNode);
		strTmp = strTmp.Right(strTmp.GetLength() - nPos -1);
		nPos = strTmp.Find(',');
	}
}

CString CDlgConvertPlay::GetFrameDesc(int nValue)
{
	switch(nValue)
	{
	case 0:
		return _T("全帧率");
	case 1:
		return _T("1/16");
	case 2:
		return _T("1/8");
	case 3:
		return _T("1/4");
	case 4:
		return _T("1/2");
	case 5:
		return _T("1");
	case 6:
		return _T("2");
	case 7:
		return _T("4");
	case 8:
		return _T("6");
	case 9:
		return _T("8");
	case 10:
		return _T("10");
	case 11:
		return _T("12");
	case 12:
		return _T("16");
	case 13:
		return _T("20");
	case 14:
		return _T("15");
	case 15:
		return _T("18");
	case 16:
		return _T("22");
	case 17:
		return _T("25");
	case 18:
		return _T("30");
	case 19:
		return _T("35");
	case 20:
		return _T("40");
	case 21:
		return _T("45");
	case 22:
		return _T("50");
	case 23:
		return _T("55");
	case 24:
		return _T("60");
	case 25:
		return _T("3");
	case 26:
		return _T("5");
	case 27:
		return _T("7");
	case 28:
		return _T("9");
	case 29:
		return _T("18");
	case 0xfffffffe:
		return _T("Auto(当前帧率)");
	default:
        PLAYBACKDLL_ERROR(_T("nValue:%d"), nValue);
		return _T("");
	}
}

CString CDlgConvertPlay::GetBitrateDesc(int nValue)
{
	switch(nValue)
	{
	case 0:
		return _T("保留");
	case 1:
		return _T("16K");
	case 2:
		return _T("32K");
	case 3:
		return _T("48K");
	case 4:
		return _T("64K");
	case 5:
		return _T("80K");
	case 6:
		return _T("96K");
	case 7:
		return _T("128K");
	case 8:
		return _T("160K");
	case 9:
		return _T("192K");
	case 10:
		return _T("224K");
	case 11:
		return _T("256K");
	case 12:
		return _T("320K");
	case 13:
		return _T("384K");
	case 14:
		return _T("448K");
	case 15:
		return _T("512K");
	case 16:
		return _T("640K");
	case 17:
		return _T("768K");
	case 18:
		return _T("896K");
	case 19:
		return _T("1024K");
	case 20:
		return _T("1280K");
	case 21:
		return _T("1536K");
	case 22:
		return _T("1792K");
	case 23:
		return _T("2048K");
	case 24:
		return _T("3072K");
	case 25:
		return _T("4096K");
	case 26:
		return _T("8196K");
	case 27:
		return _T("16384K");
	case 0xfffffffe:
		return _T("Auto(当前码率)");
	default:
        PLAYBACKDLL_ERROR(_T("nValue:%d"), nValue);
		return _T("");
	}
}


void CDlgConvertPlay::OnCbnSelchangeComboResolution()
{
	int nValue = m_cmbResolution.GetItemData(m_cmbResolution.GetCurSel());
	std::map<int, STRU_ABILITY>::iterator itor = m_mapAbility.find(nValue);
	if (itor == m_mapAbility.end())
	{
		return;
	}

	m_cmbVedioFrameRate.ResetContent();
	std::list<STRU_ABILITYDESC>::iterator itorFrame = itor->second.listAbilityFrame.begin();
	for (int i = 0; itorFrame != itor->second.listAbilityFrame.end(); itorFrame++,i++)
	{
		m_cmbVedioFrameRate.InsertString(i,itorFrame->strDesc);
		m_cmbVedioFrameRate.SetItemData(i, itorFrame->nValue);
	}
	m_cmbVedioFrameRate.SetCurSel(0);

	m_cmbVedioBitrate.ResetContent();
	std::list<STRU_ABILITYDESC>::iterator itorBit = itor->second.listAbilityBitrate.begin();
	for (int i = 0; itorBit != itor->second.listAbilityBitrate.end(); itorBit++,i++)
	{
		m_cmbVedioBitrate.InsertString(i,itorBit->strDesc);
		m_cmbVedioBitrate.SetItemData(i, itorBit->nValue);
	}
	m_cmbVedioBitrate.SetCurSel(0);
}


HBRUSH CDlgConvertPlay::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    if (CTLCOLOR_DLG == nCtlColor || CTLCOLOR_STATIC == nCtlColor)
    {
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetBkColor(RGB(242, 242, 242));
    }

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

LRESULT CDlgConvertPlay::OnNcHitTest(CPoint point)
{
    CRect rect;
    GetClientRect(&rect);
    rect.bottom = 30;
    ClientToScreen(&rect);
    return rect.PtInRect(point)?HTCAPTION:CDialog::OnNcHitTest(point);
}

void CDlgConvertPlay::InitPos()
{
    MoveWindow( m_nXPos - Size_Width , m_nYPos , Size_Width, Size_Heith);
    CRect rcRect;
    GetClientRect(&rcRect);

    //创建圆角
    CRgn rgn;
    rgn.CreateRoundRectRgn(0,0,rcRect.Width()+3,rcRect.Height()+3,5,5);
    SetWindowRgn(rgn,TRUE);

    const int nSpace = 4;
    CRect rcTitle;
    rcTitle.left = rcRect.left + nSpace*3;
    rcTitle.top = rcRect.top + nSpace*2;
    rcTitle.right = rcTitle.left + 100;
    rcTitle.bottom = rcTitle.top + 16;
    m_sttTitle.MoveWindow(rcTitle);

    CRect rcClose;
    rcClose.right = rcRect.right - nSpace*3;
    rcClose.top = rcRect.top + 6;
    rcClose.left = rcClose.right - 16;
    rcClose.bottom = rcClose.top + 16;
    m_btnClose.MoveWindow(rcClose);

    CRect rcLine;
    rcLine.left = rcRect.left + nSpace*2;
    rcLine.right = rcRect.right - nSpace*2;
    rcLine.top = rcTitle.bottom + nSpace;
    rcLine.bottom = rcLine.top + 2;
    m_line1.MoveWindow(rcLine);

    CRect rcText;
    rcText.left = rcLine.left + nSpace;
    rcText.top = rcLine.bottom + nSpace*3;
    rcText.right = rcText.left + 50;
    rcText.bottom = rcText.top + 16;
    m_sttRes.MoveWindow(rcText);

    CRect rcComb;
    rcComb.left = rcText.right;
    rcComb.right = rcRect.right - nSpace*4;
    rcComb.top = rcText.top - 2;
    rcComb.bottom = rcComb.top + 20;
    m_cmbResolution.MoveWindow(rcComb);

    rcText.top = rcText.bottom + nSpace*3;
    rcText.bottom = rcText.top + 16;
    m_sttBit.MoveWindow(rcText);
    rcText.top = rcText.bottom + nSpace*3;
    rcText.bottom = rcText.top + 16;
    m_sttFrame.MoveWindow(rcText);

    rcComb.top = rcComb.bottom + nSpace*2;
    rcComb.bottom = rcComb.top + 20;
    m_cmbVedioBitrate.MoveWindow(rcComb);
    rcComb.top = rcComb.bottom + nSpace*2;
    rcComb.bottom = rcComb.top + 20;
    m_cmbVedioFrameRate.MoveWindow(rcComb);

    CRect rcBtn;
    rcBtn.left = rcRect.left + nSpace*5;
    rcBtn.top = rcText.bottom + nSpace*3;
    rcBtn.right = rcBtn.left + 82;
    rcBtn.bottom = rcBtn.top + 27;
    m_btnOK.MoveWindow(rcBtn);

    rcBtn.right = rcRect.right - nSpace*5;
    rcBtn.left = rcBtn.right - 82;
    m_btnCancel.MoveWindow(rcBtn);
}

void CDlgConvertPlay::OnBnClickedBtnClose()
{
    OnCancel();
}

BOOL CDlgConvertPlay::PreTranslateMessage(MSG* pMsg)
{
    switch(pMsg->message)
    {
    case WM_KEYDOWN:
        {
            //屏蔽Enter
            if (VK_RETURN == pMsg->wParam)
            {
                return TRUE;
            }
            //屏蔽ESC
            if (VK_ESCAPE == pMsg->wParam)
            {
                return TRUE;
            }
            //屏蔽ESC
            if (VK_SPACE == pMsg->wParam)
            {
                return TRUE;
            }
        }
        break;
    case WM_SYSKEYDOWN:
        {
            //屏蔽ALt+F4
            if (VK_F4 == pMsg->wParam)
            {
                return TRUE;
            }
        }
        break;
    default:
        break;
    }

    return CDialog::PreTranslateMessage(pMsg);
}
