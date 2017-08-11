// HistoryListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FMSClient.h"
#include "HistoryListDlg.h"
#include "CMSDataOperate.h"


// CHistoryListDlg �Ի���

IMPLEMENT_DYNAMIC(CHistoryListDlg, CFDialog)

CHistoryListDlg::CHistoryListDlg(CWnd* pParent /*=NULL*/)
	: CFDialog(CHistoryListDlg::IDD, pParent)
{

}

CHistoryListDlg::~CHistoryListDlg()
{
}

void CHistoryListDlg::DoDataExchange(CDataExchange* pDX)
{
	CFDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_STARTTIME, m_staticStartTime);
	DDX_Control(pDX, IDC_DATETIME_START, m_dateTimeStart);
	DDX_Control(pDX, IDC_STATIC_ENDTIME, m_staticEndTime);
	DDX_Control(pDX, IDC_DATETIME_END, m_dateTimeEnd);
	DDX_Control(pDX, IDC_STATIC_CUSTOMERNAME, m_staticCustomerName);
	DDX_Control(pDX, IDC_EDIT_CUSTOMERNAME, m_editCustomerName);
	DDX_Control(pDX, IDC_STATIC_BUSINESSNUM, m_staticBusinessNum);
	DDX_Control(pDX, IDC_EDIT_BUSINESSNUM, m_editBusinessNum);
	DDX_Control(pDX, IDC_BTN_QUERY, m_btnQuery);
	DDX_Control(pDX, IDC_BTN_RESET, m_btnReset);
	DDX_Control(pDX, IDC_LIST_HISTORYLIST, m_listHistory);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_STATIC_BLANK1, m_staticBlank1);
}


BEGIN_MESSAGE_MAP(CHistoryListDlg, CFDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CHistoryListDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_QUERY, &CHistoryListDlg::OnBnClickedBtnQuery)
	ON_BN_CLICKED(IDC_BTN_RESET, &CHistoryListDlg::OnBnClickedBtnReset)
END_MESSAGE_MAP()


// CHistoryListDlg ��Ϣ�������

BOOL CHistoryListDlg::OnInitDialog()
{
	CFDialog::OnInitDialog();

	//����������Ƥ��
	LoadSkin();

	MoveWindow(HISTORYLIST_WND_SIZE);
	CenterWindow();

	//��ʼ��
	InitCtrl();

	//�ƶ�
	MoveCtrl();

	//��ȡ��ʼ����
	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CHistoryListDlg::LoadSkin(void)
{
	std::vector<int> vecMargin;
	vecMargin.push_back(MARGIN_LEFT_DLG);
	vecMargin.push_back(MARGIN_TOP_DLG);
	vecMargin.push_back(MARGIN_RIGHT_DLG);
	vecMargin.push_back(MARGIN_BOTTOM_DLG);
	SetMargin(vecMargin);

	SetObjectName(_T("dialog"));
}

void CHistoryListDlg::InitCtrl(void)
{
	//��ʼʱ��
	m_staticStartTime.SetTextFont(YAHEI_18_N);
	m_dateTimeStart.SetStyle(FDTS_CAL);
	m_dateTimeStart.SetCalStyle(CS_DATETIME);
	m_dateTimeStart.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
// 	CTimeSpan span = CTimeSpan(2, 0, 0, 0);  //Ĭ��ǰ3��
// 	CTime timeNow = CTime::GetCurrentTime();
// 	CTime timeSpan = timeNow - span;
// 	CTime timeSet(timeSpan.GetYear(), timeSpan.GetMonth(), timeSpan.GetDay(), 0, 0, 0);
// 	m_dateTimeStart.SetTime(&timeSet);

	

	//����ʱ��
	m_staticEndTime.SetTextFont(YAHEI_18_N);
	m_dateTimeEnd.SetStyle(FDTS_CAL);
	m_dateTimeEnd.SetCalStyle(CS_DATETIME);
	m_dateTimeEnd.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));

	//��ѯ��ť
	m_btnQuery.SetObjectName(_T("btn"));
	m_btnQuery.SetTextAlign(DT_CENTER);
	m_btnQuery.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, YAHEI_18_N);
	m_btnQuery.SetTextColor(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, FUi::White);
	m_btnQuery.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	

	//�ͻ�����
	m_staticCustomerName.SetTextFont(YAHEI_18_N);
	m_editCustomerName.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_editCustomerName.LimitText(32);

	//ҵ�񵥺�
	m_staticBusinessNum.SetTextFont(YAHEI_18_N);
	m_editBusinessNum.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_editBusinessNum.LimitText(32);

	//���ð�ť
	m_btnReset.SetObjectName(_T("cancel"));
	m_btnReset.SetTextAlign(DT_CENTER);
	m_btnReset.SetTextColor(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, FUi::Black);
	m_btnReset.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);

	//��ʷҵ���б�
	//SkinWndScroll(&m_listHistory);
	m_listHistory.SetActrueParent(this);
	m_listHistory.SetHeadHeight(30);
	m_listHistory.SetHeight(25);
	m_listHistory.SetHeadBgColor(RGB(158, 167, 178));
	m_listHistory.SetCheckBox(false);
	//m_listHistory.SetSingleSelect(true);
	m_listHistory.SetToolTipEnable(TRUE);
	m_listHistory.InsertColumn(0, _T("���"), LVCFMT_LEFT, 50);
	m_listHistory.InsertColumn(1, _T("ҵ�񵥺�"), LVCFMT_LEFT, 150);
	m_listHistory.InsertColumn(2, _T("�ͻ�����"), LVCFMT_LEFT, 75);
	m_listHistory.InsertColumn(3, _T("�ͻ�֤����"), LVCFMT_LEFT, 131);
	m_listHistory.InsertColumn(4, _T("��Ʒ����"), LVCFMT_LEFT, 100);
	m_listHistory.InsertColumn(5, _T("������"), LVCFMT_LEFT, 70);
	m_listHistory.InsertColumn(6, _T("����ʱ��"), LVCFMT_LEFT, 140);
	m_listHistory.SetHeadLock(-1, TRUE);	//���ñ�ͷ����

	//�ָ���
	GetDlgItem(IDC_STATIC_BLANK1)->ShowWindow(SW_HIDE);

	//ȷ����ť
	m_btnOK.SetObjectName(_T("btn"));
	m_btnOK.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, YAHEI_18_N);
	m_btnOK.SetTextColor(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, FUi::White);
	m_btnOK.SetTextAlign(DT_CENTER);

	//ȡ����ť
	m_btnCancel.SetObjectName(_T("cancel"));
	m_btnCancel.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, YAHEI_18_N);
	m_btnCancel.SetTextAlign(DT_CENTER);

}

void CHistoryListDlg::InitData()
{
	m_listHistory.DeleteAllItems();

	//��ʼʱ��
	CTimeSpan span = CTimeSpan(2, 0, 0, 0);   //Ĭ��ǰ3��
	CTime timeNow = CTime::GetCurrentTime();
	CTime timeSpan = timeNow - span;
	CTime timeSet(timeSpan.GetYear(), timeSpan.GetMonth(), timeSpan.GetDay(), 0, 0, 0);
	m_dateTimeStart.SetTime(&timeSet);

	//����ʱ��:Ĭ�ϵ���23:59:59
	CTime endTime(timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), 23, 59, 59);
	m_dateTimeEnd.SetTime(&endTime);

	CTime timeStart;
	CString strStartTime;
	m_dateTimeStart.GetTime(timeStart);
	strStartTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), 
		timeStart.GetYear(), timeStart.GetMonth(), timeStart.GetDay(), timeStart.GetHour(), timeStart.GetMinute(), timeStart.GetSecond());

	CTime timeEnd;
	CString strEndTime;
	m_dateTimeEnd.GetTime(timeEnd);
	strEndTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
		timeEnd.GetYear(), timeEnd.GetMonth(), timeEnd.GetDay(), timeEnd.GetHour(), timeEnd.GetMinute(), timeEnd.GetSecond());

	CString strConsumerName;
	m_editCustomerName.GetWindowText(strConsumerName);

	CString strBusinessId;
	m_editBusinessNum.GetWindowText(strBusinessId);

	cms_8120::HisClientInput hisClientInput;
	if (_T("") != strConsumerName)
	{
		hisClientInput.set_name(UTF8_MFCSTRW2A(strConsumerName));
	}
	if (_T("") != strBusinessId)
	{
		hisClientInput.set_nproid(UTF8_MFCSTRW2A(strBusinessId));
	}
	hisClientInput.set_inputuser(theApp.m_nUserId);
	hisClientInput.CheckInitialized();

	//��ѯ��ʷҵ��
	if (!QueryHistoryList(hisClientInput, strStartTime, strEndTime))
	{
		FMSCLIENT_ERROR("QueryHistoryList Failed!");
		return;
	}

	//��ѯ������뵽�б�
	InsertToList();
	
}

void CHistoryListDlg::MoveCtrl(void)
{
	CRect rcClient;
	GetClientRect(rcClient);

	//��ʼʱ��
	CRect rcStaticStart(rcClient);
	rcStaticStart.left = rcStaticStart.left + 20;
	rcStaticStart.top = rcStaticStart.top + 29;
	rcStaticStart.right = rcStaticStart.left + 50;
	rcStaticStart.bottom = rcStaticStart.top + 26;
	m_staticStartTime.MoveWindow(rcStaticStart);

	CRect rcDataTimeStart(rcClient);
	rcDataTimeStart.left = rcStaticStart.right + 10;
	rcDataTimeStart.top = rcClient.top + 26;
	rcDataTimeStart.right = rcDataTimeStart.left + 150;
	rcDataTimeStart.bottom = rcDataTimeStart.top + 26;
	m_dateTimeStart.MoveWindow(rcDataTimeStart);

	//����ʱ��
	CRect rcStaticEnd(rcClient);
	rcStaticEnd.left = rcDataTimeStart.right + 50;
	rcStaticEnd.top = rcStaticStart.top;
	rcStaticEnd.right = rcStaticEnd.left + 50;
	rcStaticEnd.bottom = rcStaticEnd.top + 26;
	m_staticEndTime.MoveWindow(rcStaticEnd);

	CRect rcDataTimeEnd(rcClient);
	rcDataTimeEnd.left = rcStaticEnd.right + 10;
	rcDataTimeEnd.top = rcClient.top + 26;
	rcDataTimeEnd.right = rcDataTimeEnd.left + 150;
	rcDataTimeEnd.bottom = rcDataTimeEnd.top + 26;
	m_dateTimeEnd.MoveWindow(rcDataTimeEnd);

	//��ѯ��ť
	CRect rcBtnQuery(rcClient);
	rcBtnQuery.left = rcDataTimeEnd.right + 50;
	rcBtnQuery.top = rcBtnQuery.top + 24;
	rcBtnQuery.right = rcBtnQuery.left + 90;
	rcBtnQuery.bottom = rcBtnQuery.top + 30;
	m_btnQuery.MoveWindow(rcBtnQuery);

	//�ͻ�����
	CRect rcStaticConsumerName(rcStaticStart);
	rcStaticConsumerName.top = rcStaticConsumerName.bottom + 14;
	rcStaticConsumerName.bottom = rcStaticConsumerName.top + 26;
	m_staticCustomerName.MoveWindow(rcStaticConsumerName);

	CRect rcEditConsumerName(rcDataTimeStart);
	rcEditConsumerName.top = rcEditConsumerName.bottom + 14;
	rcEditConsumerName.bottom = rcEditConsumerName.top + 26;
	m_editCustomerName.MoveWindow(rcEditConsumerName);

	//ҵ�񵥺�
	CRect rcStaticBusinessId(rcStaticEnd);
	rcStaticBusinessId.top = rcStaticBusinessId.bottom + 14;
	rcStaticBusinessId.bottom = rcStaticBusinessId.top + 26;
	m_staticBusinessNum.MoveWindow(rcStaticBusinessId);

	CRect rcEditBusinessId(rcDataTimeEnd);
	rcEditBusinessId.top = rcEditBusinessId.bottom + 14;
	rcEditBusinessId.bottom = rcEditBusinessId.top + 26;
	m_editBusinessNum.MoveWindow(rcEditBusinessId);

	//���ð�ť
	CRect rcReset(rcBtnQuery);
	rcReset.top = rcReset.bottom  + 10;
	rcReset.bottom = rcReset.top + 30;
	m_btnReset.MoveWindow(rcReset);

	//ҵ���¼�б�
	CRect rcList(rcClient);
	rcList.left = rcList.left + 20;
	rcList.top = rcList.top + 106;
	rcList.right = rcList.right - 20;
	rcList.bottom = rcList.bottom - 55;
	m_listHistory.MoveWindow(rcList);

	//�ָ���
	CRect rcStaticBlank1(rcClient);
	rcStaticBlank1.top = rcClient.bottom - 50;
	rcStaticBlank1.bottom = rcStaticBlank1.top + 1;
	m_staticBlank1.MoveWindow(rcStaticBlank1);

	//ȷ����ť
	CRect rcBtnOK(rcClient);
	rcBtnOK.left = rcBtnOK.right - 10 - 90 -10 - 90;
	rcBtnOK.top = rcBtnOK.bottom - 10 - 30;
	rcBtnOK.right = rcBtnOK.right - 10 - 90 - 10;
	rcBtnOK.bottom = rcBtnOK.top + 30;
	m_btnOK.MoveWindow(rcBtnOK);

	//ȡ����ť
	CRect rcBtnCancel(rcBtnOK);
	rcBtnCancel.left = rcBtnOK.right + 10;
	rcBtnCancel.right = rcBtnCancel.left + 90;
	m_btnCancel.MoveWindow(rcBtnCancel);

}
BOOL CHistoryListDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillSolidRect(rcClient, RGB(255, 255, 255));

	return CFDialog::OnEraseBkgnd(pDC);
}

void CHistoryListDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CFDialog::OnPaint()
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));

	CPen linePen;
	linePen.CreatePen(PS_SOLID, 1, RGB(240, 240, 240));
	CPen* pOld = dc.SelectObject(&linePen);

	//�ײ�����
	CRect rc1;
	GetDlgItem(IDC_STATIC_BLANK1)->GetWindowRect(&rc1);
	ScreenToClient(rc1);

	dc.MoveTo(rc1.left, rc1.top + rc1.Height() / 2);
	dc.LineTo(rc1.right, rc1.top + rc1.Height() / 2);
}

void CHistoryListDlg::OnBnClickedOk()
{
	// ��ȡѡ����
	int nIndex = m_listHistory.GetNextItem(-1, LVIS_SELECTED);
	if (-1 == nIndex)
	{
		FMSCLIENT_ERROR("Can't find list item Checked!");
		return;
	}

	cms_8120::HisClientInput hisClientInput = m_arrayHisClientInput.GetAt(nIndex);
	theApp.m_hisClientInput = hisClientInput;

	CDialog::OnOK();
}

void CHistoryListDlg::OnBnClickedBtnQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strConsumerName;
	m_editCustomerName.GetWindowText(strConsumerName);
	CString strBusinessId;
	m_editBusinessNum.GetWindowText(strBusinessId);

	CTime timeStart;
	CString strStartTime;
	m_dateTimeStart.GetTime(timeStart);
	strStartTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), 
		timeStart.GetYear(), timeStart.GetMonth(), timeStart.GetDay(), timeStart.GetHour(), timeStart.GetMinute(), timeStart.GetSecond());

	CTime timeEnd;
	CString strEndTime;
	m_dateTimeEnd.GetTime(timeEnd);
	strEndTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
		timeEnd.GetYear(), timeEnd.GetMonth(), timeEnd.GetDay(), timeEnd.GetHour(), timeEnd.GetMinute(), timeEnd.GetSecond());

	if (timeStart > timeEnd)
	{
		FMessageBox::Show(_T("��ѯ��ʼʱ����ڽ���ʱ��"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);

		//��ʼʱ��
		CTimeSpan span = CTimeSpan(2, 0, 0, 0);   //Ĭ��ǰ3��
		CTime timeNow = CTime::GetCurrentTime();
		CTime timeSpan = timeNow - span;
		CTime timeSet(timeSpan.GetYear(), timeSpan.GetMonth(), timeSpan.GetDay(), 0, 0, 0);
		m_dateTimeStart.SetTime(&timeSet);

		//����ʱ��:Ĭ�ϵ���23:59:59
		CTime endTime(timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), 23, 59, 59);
		m_dateTimeEnd.SetTime(&endTime);
		return;
	}

	CTimeSpan span = timeEnd - timeStart;
	if (span.GetDays() > 31)			//ʱ����>31��
	{
		FMessageBox::Show(_T("��ѯʱ�������ܳ���31��"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);
		return;
	}

	cms_8120::HisClientInput hisClientInput;
	if (_T("") != strConsumerName)
	{
		hisClientInput.set_name(UTF8_MFCSTRW2A(strConsumerName));
	}
	if (_T("") != strBusinessId)
	{
		hisClientInput.set_nproid(UTF8_MFCSTRW2A(strBusinessId));
	}
	hisClientInput.set_inputuser(theApp.m_nUserId);
	hisClientInput.CheckInitialized();

	//��ѯ��ʷҵ��
	if (!QueryHistoryList(hisClientInput, strStartTime, strEndTime))
	{
		FMSCLIENT_ERROR("QueryHistoryList Failed!");
		return;
	}

	//��ѯ������뵽�б�
	InsertToList();

	return;
}

//��ѯ������뵽�б�
void CHistoryListDlg::InsertToList(void)
{
	m_listHistory.DeleteAllItems();
	for (int i = 0; i < m_arrayHisClientInput.GetSize(); i++)
	{
		cms_8120::HisClientInput clientInput = m_arrayHisClientInput.GetAt(i);
		int nIndex = m_listHistory.GetItemCount();
		CString strIndex;
		strIndex.Format(_T("%d"), nIndex + 1);
		m_listHistory.InsertItem(i, strIndex);
		m_listHistory.SetItemText(nIndex, 1, UTF8_A2MFCSTRW(clientInput.nproid()));
		m_listHistory.SetItemText(nIndex, 2, UTF8_A2MFCSTRW(clientInput.name()));
		m_listHistory.SetItemText(nIndex, 3, UTF8_A2MFCSTRW(clientInput.creditcode()));
		m_listHistory.SetItemText(nIndex, 4, UTF8_A2MFCSTRW(clientInput.productname()));
		m_listHistory.SetItemText(nIndex, 5, UTF8_A2MFCSTRW(clientInput.strbuymoney()));
		m_listHistory.SetItemText(nIndex, 6, UTF8_A2MFCSTRW(clientInput.businesstime()));
	}
}

BOOL CHistoryListDlg::QueryHistoryList(cms_8120::HisClientInput& hisClientInput, CString strStartTime, CString strEndTime)
{
	cms_8120::RspCHisClientInput rspHisClientInput;
	if (!CCMSMgr::SnglPtr()->GetHistoryList(rspHisClientInput, hisClientInput, strStartTime, strEndTime) || rspHisClientInput.result() != 0)
	{
		FMSCLIENT_ERROR("Get HisList Failed!");
		return FALSE;
	}
	else
	{			
		m_arrayHisClientInput.SetSize(0);
		for (int i = 0; i < rspHisClientInput.hisclientinput_size(); i++)
		{
			m_arrayHisClientInput.Add(rspHisClientInput.hisclientinput(i));
		}
	}

	return TRUE;
}

void CHistoryListDlg::OnBnClickedBtnReset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_editCustomerName.SetWindowText(_T(""));
	m_editBusinessNum.SetWindowText(_T(""));
	InitData();

	Invalidate();
}

BOOL CHistoryListDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if ((GetFocus() == GetDlgItem(IDC_EDIT_CUSTOMERNAME)) &&  (pMsg->message == WM_CHAR))	//�ͻ����������ַ�Լ��
	{
		if (pMsg->wParam == '\\' || pMsg->wParam == '/' || pMsg->wParam == ':' ||
			pMsg->wParam == '*' || pMsg->wParam == '*' || pMsg->wParam == '&' ||
			pMsg->wParam == '?' || pMsg->wParam == '\"' || pMsg->wParam == '<' ||
			pMsg->wParam == '|' || pMsg->wParam == '\'' || pMsg->wParam == '%' ||
			pMsg->wParam == '%' || pMsg->wParam == '>')
		{
			return TRUE;
		}
	}
	else if ((GetFocus() == GetDlgItem(IDC_EDIT_BUSINESSNUM)) && (pMsg->message == WM_CHAR))  //ֻ���������֡�С�����
	{
		if (pMsg->wParam <= '9' && pMsg->wParam >= '0' || pMsg->wParam == '.' || pMsg->wParam == VK_BACK)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	return CFDialog::PreTranslateMessage(pMsg);
}
