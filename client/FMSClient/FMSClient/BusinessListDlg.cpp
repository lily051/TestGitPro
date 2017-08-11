// BusinessListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FMSClient.h"
#include "BusinessListDlg.h"
#include "CMSDataOperate.h"
#include "type/cmd.pb.h"
#include ".\Excel\Excel.h"

using namespace HTMLParse;

#define ICON_TOTAL 4    //图标个数


// CBusinessListDlg 对话框

IMPLEMENT_DYNAMIC(CBusinessListDlg, CFDialog)

CBusinessListDlg::CBusinessListDlg(CWnd* pParent /*=NULL*/)
	: CFDialog(CBusinessListDlg::IDD, pParent)
	,m_emListType(EM_LIST_TOTAL)
	,m_pPlaybackDlg(NULL)
{

}

CBusinessListDlg::~CBusinessListDlg()
{
	if (m_pPlaybackDlg)
	{
		delete m_pPlaybackDlg;
		m_pPlaybackDlg = NULL;
	}
}

void CBusinessListDlg::DoDataExchange(CDataExchange* pDX)
{
	CFDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_TOTAL, m_btnTotal);
	DDX_Control(pDX, IDC_BTN_EXCEPTION, m_btnException);
	DDX_Control(pDX, IDC_BTN_UPLOAD, m_btnUpload);
	DDX_Control(pDX, IDC_BTN_REFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_BTN_EXPORT, m_btnExport);
	DDX_Control(pDX, IDC_LIST_TOTAL, m_listTotal);
	DDX_Control(pDX, IDC_LIST_EXCEPTION, m_listException);
	DDX_Control(pDX, IDC_DATETIME_START, m_dateTimeStart);
	DDX_Control(pDX, IDC_DATETIME_END, m_dateTimeEnd);
	DDX_Control(pDX, IDC_BTN_QUERY, m_btnQuery);
	DDX_Control(pDX, IDC_STATIC_STARTTIME, m_staticStartTime);
	DDX_Control(pDX, IDC_STATIC_ENDTIME, m_staticEndTime);
	DDX_Control(pDX, IDC_STATIC_CONSUMERNAME, m_staticConsumerName);
	DDX_Control(pDX, IDC_EDIT_CONSUMERNAME, m_editConsumerName);
	DDX_Control(pDX, IDC_STATIC_PRODUCTNAME, m_staticProductName);
	DDX_Control(pDX, IDC_EDIT_PRODUCTNAME, m_editProductName);
	DDX_Control(pDX, IDC_BTN_RESET, m_btnReset);
}


BEGIN_MESSAGE_MAP(CBusinessListDlg, CFDialog)
	ON_BN_CLICKED(IDC_BTN_TOTAL, &CBusinessListDlg::OnBnClickedBtnTotal)
	ON_BN_CLICKED(IDC_BTN_EXCEPTION, &CBusinessListDlg::OnBnClickedBtnException)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_QUERY, &CBusinessListDlg::OnBnClickedBtnQuery)
	ON_BN_CLICKED(IDC_BTN_RESET, &CBusinessListDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_EXPORT, &CBusinessListDlg::OnBnClickedBtnExport)
	ON_BN_CLICKED(IDC_BTN_UPLOAD, &CBusinessListDlg::OnBnClickedBtnUpload)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TOTAL, &CBusinessListDlg::OnNMClickListTotal)
END_MESSAGE_MAP()


// CBusinessListDlg 消息处理程序

void CBusinessListDlg::InitCtrl(void)
{
	std::vector<int> vecMargin(4, 0);
// 	vecMargin.push_back(0);
// 	vecMargin.push_back(0);
// 	vecMargin.push_back(0);
// 	vecMargin.push_back(0);
	SetMargin(vecMargin);

	MoveWindow(BUSINESS_WND_SIZE);

	m_btnTotal.SetTextAlign(DT_CENTER);
	m_btnException.SetTextAlign(DT_CENTER);

	m_btnTotal.SetTextColor(FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Normal, RGB(63, 133, 228));
	m_btnTotal.SetTextFont(FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Normal, YAHEI_18_B);
	m_btnException.SetTextColor(FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Normal, RGB(63, 133, 228));
	m_btnException.SetTextFont(FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Normal, YAHEI_18_B);
	
	m_btnUpload.SetIcon(_T("uplaod1"), ICON_TOTAL);
	m_btnUpload.SetTextAlign(DT_CENTER);
	m_btnUpload.SetTipText(_T("上传"));
	m_btnUpload.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_btnUpload.SetTextColor(FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, RGB(63, 133, 228));

	m_btnRefresh.SetIcon(_T("refresh"), ICON_TOTAL);
	m_btnRefresh.SetTextAlign(DT_CENTER);
	m_btnRefresh.SetTipText(_T("刷新"));
	m_btnRefresh.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_btnRefresh.SetTextColor(FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, RGB(63, 133, 228));
	m_btnRefresh.ShowWindow(SW_HIDE);  //删除了刷新按钮的显示

	m_btnExport.SetIcon(_T("export1"), ICON_TOTAL);
	m_btnExport.SetTextAlign(DT_CENTER);
	m_btnExport.SetTipText(_T("导出"));
	m_btnExport.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_btnExport.SetTextColor(FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, RGB(63, 133, 228));

//	m_staticGroup.SetBgcolor(FALSE, RGB(125, 125, 125));


	//开始时间
	m_staticStartTime.SetTextFont(YAHEI_18_N);
	m_dateTimeStart.SetStyle(FDTS_CAL);
	m_dateTimeStart.SetCalStyle(CS_DATETIME);
	m_dateTimeStart.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
// 	CTimeSpan span = CTimeSpan(2, 0, 0, 0);   //默认前3天
// 	CTime timeNow = CTime::GetCurrentTime();
// 	CTime timeSpan = timeNow - span;
// 	CTime timeSet(timeSpan.GetYear(), timeSpan.GetMonth(), timeSpan.GetDay(), 0, 0, 0);
// 	m_dateTimeStart.SetTime(&timeSet);

	//结束时间
	m_staticEndTime.SetTextFont(YAHEI_18_N);
	m_dateTimeEnd.SetStyle(FDTS_CAL);
	m_dateTimeEnd.SetCalStyle(CS_DATETIME);
	m_dateTimeEnd.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));

	//查询按钮
	m_btnQuery.SetObjectName(_T("btn"));
	m_btnQuery.SetTextAlign(DT_CENTER);
	m_btnQuery.SetTextColor(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, FUi::White);
	m_btnQuery.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);

	//客户姓名
	m_staticConsumerName.SetTextFont(YAHEI_18_N);
	m_editConsumerName.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_editConsumerName.LimitText(32);
	//产品名称
	m_staticProductName.SetTextFont(YAHEI_18_N);
	m_editProductName.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_editProductName.LimitText(32);
	//重置按钮
	m_btnReset.SetObjectName(_T("cancel"));
	m_btnReset.SetTextAlign(DT_CENTER);
	m_btnReset.SetTextColor(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, FUi::Black);
	m_btnReset.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);

	m_listTotal.ShowWindow(SW_SHOW);
	m_listException.ShowWindow(SW_HIDE);
	//全部列表
	m_listTotal.SetToolTipEnable(true);
	m_listTotal.SetHeadHeight(30);
	m_listTotal.SetHeight(28);
	m_listTotal.SetCheckBox(FALSE);
	//m_listTotal.SetCursor(LoadCursor(NULL, IDC_HAND));

	//SkinWndScroll(&m_listTotal);	 // 使用Fui的滚动条
	m_listTotal.SetHeadBgColor(RGB(158, 167, 178));
	m_listTotal.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40);
	m_listTotal.InsertColumn(1, _T("业务单号"), LVCFMT_LEFT, 150);
	m_listTotal.InsertColumn(2, _T("客户姓名"), LVCFMT_LEFT, 70);
	m_listTotal.InsertColumn(3, _T("产品名称"), LVCFMT_LEFT, 70);
	m_listTotal.InsertColumn(4, _T("购买金额"), LVCFMT_LEFT, 70);
	m_listTotal.InsertColumn(5, _T("录像数"), LVCFMT_LEFT, 55);
	m_listTotal.InsertColumn(6, _T("办理时间"), LVCFMT_LEFT, 140);
	m_listTotal.InsertColumn(7, _T("业务状态"), LVCFMT_LEFT, 70);
	m_listTotal.InsertColumn(8, _T("录像状态"), LVCFMT_LEFT, 70);
	m_listTotal.InsertColumn(9, _T("操作"), LVCFMT_LEFT, 120);
	m_listTotal.SetButton(9, 4, _T("detail"), _T("play"), _T("uplaod"), _T("print_list"));
	m_listTotal.SetButtonToolTip(4, _T("详情"), _T("播放"), _T("上传"), _T("打印"));
	m_listTotal.SetHeadLock(-1, TRUE);	//设置表头锁定

	//异常列表
	m_listException.SetToolTipEnable(true);
	m_listException.SetHeadHeight(30);
	m_listException.SetHeight(28);
	m_listException.SetCheckBox(FALSE);
	//SkinWndScroll(&m_listException);	// 使用Fui的滚动条
	m_listException.SetHeadBgColor(RGB(158, 167, 178));
	m_listException.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40);
	m_listException.InsertColumn(1, _T("录像名称"), LVCFMT_LEFT, 100);
	m_listException.InsertColumn(2, _T("业务单号"), LVCFMT_LEFT, 145);
	m_listException.InsertColumn(3, _T("客户姓名"), LVCFMT_LEFT, 70);
	m_listException.InsertColumn(4, _T("产品名称"), LVCFMT_LEFT, 70);
	m_listException.InsertColumn(5, _T("购买金额"), LVCFMT_LEFT, 70);
	m_listException.InsertColumn(6, _T("办理时间"), LVCFMT_LEFT, 130);
	m_listException.InsertColumn(7, _T("合规状态"), LVCFMT_LEFT, 70);
	m_listException.InsertColumn(8, _T("上传状态"), LVCFMT_LEFT, 70);
	m_listException.InsertColumn(9, _T("操作"), LVCFMT_LEFT, 90);
	m_listException.SetButton(9, 3, _T("play"), _T("uplaod"), _T("delete"));
	m_listException.SetButtonToolTip(3, _T("播放"), _T("重新上传"), _T("删除"));
	m_listException.SetHeadLock(-1, TRUE);	//设置表头锁定

	ShowSelectList();
}

void CBusinessListDlg::MoveCtrl(void)
{
	CRect rcClient;
	GetClientRect(rcClient);

	//全部&异常
	CRect rcBtnTotal(rcClient);
	rcBtnTotal.left = rcBtnTotal.left + 20;
	rcBtnTotal.top = rcBtnTotal.top + 10;
	rcBtnTotal.right = rcBtnTotal.left + 100;
	rcBtnTotal.bottom = rcBtnTotal.top + 26;
	m_btnTotal.MoveWindow(rcBtnTotal);

	CRect rcBtnException(rcClient);
	rcBtnException.left = rcBtnTotal.right + 10;
	rcBtnException.top = rcBtnTotal.top;
	rcBtnException.right = rcBtnException.left + 100;
	rcBtnException.bottom = rcBtnException.top + 26;
	m_btnException.MoveWindow(rcBtnException);

	//上传&刷新&导出
	CRect rcUpload(rcClient);
	rcUpload.left = rcUpload.left + 20;
	rcUpload.top = rcBtnException.bottom + 5 + 10;
	rcUpload.right = rcUpload.left + 80;
	rcUpload.bottom = rcUpload.top + 30;
	m_btnUpload.MoveWindow(rcUpload);

	CRect rcRefresh(rcUpload);
	rcRefresh.left = rcRefresh.right;
	rcRefresh.right = rcRefresh.left + 80;
	m_btnRefresh.MoveWindow(rcRefresh);	//删除了刷新按钮的显示

	CRect rcExport(rcRefresh);
// 	rcExport.left = rcExport.right;
// 	rcExport.right = rcExport.left + 80;
	m_btnExport.MoveWindow(rcExport);

	//开始时间
	CRect rcStaticStartTime(rcClient);
	rcStaticStartTime.left = rcStaticStartTime.left + 40;
	rcStaticStartTime.top = 103;
	rcStaticStartTime.right = rcStaticStartTime.left + 60;
	rcStaticStartTime.bottom = rcStaticStartTime.top + 26;
	m_staticStartTime.MoveWindow(rcStaticStartTime);

	CRect rcStartTime(rcStaticStartTime);
	rcStartTime.left = rcStartTime.right + 5;
	rcStartTime.top = 100;
	rcStartTime.right = rcStartTime.left + 170;
	rcStartTime.bottom = rcStartTime.top + 26;
	m_dateTimeStart.MoveWindow(rcStartTime);

	//结束时间
	CRect rcStaticEndTime(rcStartTime);
	rcStaticEndTime.left = rcStaticEndTime.right + 40;
	rcStaticEndTime.top = 103;
	rcStaticEndTime.right = rcStaticEndTime.left + 60;
	rcStaticEndTime.bottom = rcStaticEndTime.top + 26;
	m_staticEndTime.MoveWindow(rcStaticEndTime);

	CRect rcEndTime(rcStaticEndTime);
	rcEndTime.left = rcStaticEndTime.right + 5;
	rcEndTime.top = 100;
	rcEndTime.right = rcEndTime.left + 170;
	rcEndTime.bottom = rcEndTime.top + 26;
	m_dateTimeEnd.MoveWindow(rcEndTime);

	//查询按钮
	CRect rcBtnQuery(rcEndTime);
	rcBtnQuery.left = rcBtnQuery.right + 40;
	rcBtnQuery.right = rcBtnQuery.left + 90;
	m_btnQuery.MoveWindow(rcBtnQuery);

	//客户姓名
	CRect rcStaticConsumerName(rcStaticStartTime);
	rcStaticConsumerName.top = rcStaticConsumerName.bottom + 15;
	rcStaticConsumerName.bottom = rcStaticConsumerName.top + 26;
	m_staticConsumerName.MoveWindow(rcStaticConsumerName);

	CRect rcEditConsumerName(rcStaticConsumerName);
	rcEditConsumerName.left = rcEditConsumerName.right + 5;
	rcEditConsumerName.top = rcEndTime.bottom + 14;
	rcEditConsumerName.right = rcEditConsumerName.left + 170;
	rcEditConsumerName.bottom = rcEditConsumerName.top + 26;
	m_editConsumerName.MoveWindow(rcEditConsumerName);

	//产品名称
	CRect rcStaticProductName(rcEditConsumerName);
	rcStaticProductName.left = rcStaticProductName.right + 40;
	rcStaticProductName.top = rcStaticConsumerName.top;
	rcStaticProductName.right = rcStaticProductName.left + 60;
	rcStaticProductName.bottom = rcStaticConsumerName.bottom;
	m_staticProductName.MoveWindow(rcStaticProductName);

	CRect rcEditProductName(rcStaticProductName);
	rcEditProductName.left = rcEditProductName.right + 5;
	rcEditProductName.top = rcEditConsumerName.top;
	rcEditProductName.right = rcEditProductName.left + 170;
	rcEditProductName.bottom = rcEditConsumerName.bottom;
	m_editProductName.MoveWindow(rcEditProductName);

	//重置
	CRect rcBtnReset(rcEditProductName);
	rcBtnReset.left = rcBtnReset.right + 40;
	rcBtnReset.right = rcBtnReset.left + 90;
	m_btnReset.MoveWindow(rcBtnReset);

	//列表
	CRect rcList(rcClient);
	rcList.left = rcList.left + 20;
	rcList.top = rcEditProductName.bottom + 15;
	rcList.right = rcList.right - 20;
	rcList.bottom = rcList.bottom - 28;
	m_listTotal.MoveWindow(rcList);
	m_listTotal.ShowWindow(SW_SHOW);
	m_listException.MoveWindow(rcList);
	m_listException.ShowWindow(SW_HIDE);

}



void CBusinessListDlg::InitData(void)
{

	//开始时间
	CTimeSpan span = CTimeSpan(2, 0, 0, 0);   //默认前3天
	CTime timeNow = CTime::GetCurrentTime();
	CTime timeSpan = timeNow - span;
	CTime timeSet(timeSpan.GetYear(), timeSpan.GetMonth(), timeSpan.GetDay(), 0, 0, 0);
	m_dateTimeStart.SetTime(&timeSet);

	//结束时间:默认当天23:59:59
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
	m_editConsumerName.GetWindowText(strConsumerName);

	CString strProductName;
	m_editProductName.GetWindowText(strProductName);


	m_listTotal.DeleteAllItems();
	m_listBusinessInfo.RemoveAll();
	QueryBusinessList(strStartTime, strEndTime, strConsumerName, strProductName);


	m_listException.DeleteAllItems();
	m_listErrorRecInfo.RemoveAll();
	QueryErrorRecordsList(strStartTime, strEndTime, strConsumerName, strProductName);
	
}

void CBusinessListDlg::ShowSelectList()
{
	CClientDC dc(this);
	DrawSelectLine(dc);

	CRect rcClient;
	GetClientRect(rcClient);

	switch(m_emListType)
	{
	case EM_LIST_TOTAL:
		{
			m_btnTotal.SetTextColor(1, RGB(63, 133, 228));
			m_btnTotal.SetTextFont(FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Normal, YAHEI_18_B);
			m_btnException.SetTextColor(1, RGB(102, 102, 102));
			m_btnException.SetTextFont(FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Normal, YAHEI_18_B);
			m_listTotal.ShowWindow(SW_SHOW);
			m_listException.ShowWindow(SW_HIDE);
		}
		break;
	case EM_LIST_EXCEPTION:
		{
			m_btnTotal.SetTextColor(1, RGB(102, 102, 102));
			m_btnTotal.SetTextFont(FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Normal, YAHEI_18_B);
			m_listTotal.ShowWindow(SW_HIDE);
			m_btnException.SetTextColor(1, RGB(63, 133, 228));
			m_btnException.SetTextFont(FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Normal, YAHEI_18_B);
			m_listException.ShowWindow(SW_SHOW);
		}
		break;
	default:
		break;
	}
}

void CBusinessListDlg::DrawSelectLine(CDC& dc)
{
	CRect rcClient;
	GetClientRect(rcClient);

	//画底线
	CPen linePen;
	linePen.CreatePen(PS_SOLID, 1, RGB(204, 204, 204));
	CPen * pOld = dc.SelectObject(&linePen);
	dc.MoveTo(rcClient.left + 20, rcClient.top + 40);
	dc.LineTo(rcClient.right - 20, rcClient.top + 40);

	dc.SelectObject(pOld);
	linePen.DeleteObject();

	//画亮线
	CPen linePenSel;
	linePenSel.CreatePen(PS_SOLID, 2, RGB(63, 133, 228));
	pOld = dc.SelectObject(&linePenSel);

	int nSection = 0;
	switch(m_emListType)
	{
	case EM_LIST_TOTAL:
		{
			nSection = EM_LIST_TOTAL;
		}
		break;
	case EM_LIST_EXCEPTION:
		{
			nSection = EM_LIST_EXCEPTION;
		}
		break;
	default:
		break;
	}
	dc.MoveTo(rcClient.left + 20 + 3 * nSection + 100*(nSection - 1), rcClient.top + 40);
	dc.LineTo(rcClient.left + 20 + 3 * nSection + 100*nSection, rcClient.top + 40);

	dc.SelectObject(pOld);
	linePenSel.DeleteObject();
}

BOOL CBusinessListDlg::OnInitDialog()
{
	CFDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrl();
	MoveCtrl();
	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CBusinessListDlg::OnBnClickedBtnTotal()
{
	// TODO: 在此添加控件通知处理程序代码
	m_emListType = EM_LIST_TOTAL;
	ShowSelectList();
	//刷新残影
	CRect rcClient;
	GetClientRect(rcClient);
	CRect rcRect(rcClient.left + 20, rcClient.top + 39, rcClient.right - 20, rcClient.top + 41);
	InvalidateRect(rcRect);

	UpdateData(FALSE);

	m_listTotal .DeleteAllItems();
	m_listBusinessInfo.RemoveAll();

	QueryBusiness(FALSE);
}

void CBusinessListDlg::OnBnClickedBtnException()
{
	// TODO: 在此添加控件通知处理程序代码
	m_emListType = EM_LIST_EXCEPTION;
	ShowSelectList();
	//刷新残影
	CRect rcClient;
	GetClientRect(rcClient);
	CRect rcRect(rcClient.left + 20, rcClient.top + 39, rcClient.right - 20, rcClient.top + 41);
	InvalidateRect(rcRect);

	UpdateData(FALSE);

	m_listException .DeleteAllItems();
	m_listErrorRecInfo.RemoveAll();

	QueryBusiness(FALSE);
}

void CBusinessListDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//窗口背景色
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));

	//查询过滤条件区域背景色
	CRect rcGroup(rect);
	rcGroup.left += 20;
	rcGroup.top += 85;
	rcGroup.right = rcGroup.right - 20;
	rcGroup.bottom = rcGroup.top + 100;
	dc.FillSolidRect(rcGroup, RGB(245, 245, 245));

	ShowSelectList();
}

//查询业务记录
void CBusinessListDlg::OnBnClickedBtnQuery()
{
	//查询
	QueryBusiness();

	return;
}

//查询业务记录（业务单）列表
BOOL CBusinessListDlg::QueryBusinessList(CString strStartTime, CString strEndTime, CString strConsumerName, CString strProductName)
{
	cms_8120::RspCsClientRecordsInfo rspBusinessInfo;
	if (!CCMSMgr::SnglPtr()->GetBusinessList(rspBusinessInfo, strStartTime, strEndTime, strConsumerName, strProductName, m_emListType) || rspBusinessInfo.result() != 0)
	{
		CString strInfo = UTF8_A2MFCSTRW(rspBusinessInfo.result_info());
		FMSCLIENT_ERROR("Get List Failed:%s", fcT2A(strInfo));
		return FALSE;
	}
	else
	{
		cms_8120::RecordsInfo recordInfo;
		for (int i = rspBusinessInfo.recordsinfo_size() - 1; i >= 0; i--)   //业务记录总数(时间最近的排在上面)
		{
			recordInfo = rspBusinessInfo.recordsinfo(i);
			InsertToTotalList(recordInfo);
			m_listBusinessInfo.AddTail(recordInfo);
		}
	}

	return TRUE;
}

BOOL CBusinessListDlg::InsertToTotalList(cms_8120::RecordsInfo& recordInfo)
{
	// 图标列表
	int nImage0 = m_listTotal.SetImageList(_T("line-up"));
	int nImage1 = m_listTotal.SetImageList(_T("success (2)"));
	int nImage2 = m_listTotal.SetImageList(_T("nofinish"));
	int nIndex = m_listTotal.GetItemCount();
	CString strIndex;
	strIndex.Format(_T("%d"), nIndex + 1);
	int nCount = recordInfo.videonum();
	CString strCount;
	strCount.Format(_T("%d"), nCount);

	m_listTotal.InsertItem(nIndex, strIndex);
	m_listTotal.SetItemText(nIndex, 1, UTF8_A2MFCSTRW(recordInfo.nproid()));
	m_listTotal.SetItemText(nIndex, 2, UTF8_A2MFCSTRW(recordInfo.custname()));
	m_listTotal.SetItemText(nIndex, 3, UTF8_A2MFCSTRW(recordInfo.productname()));
	m_listTotal.SetItemText(nIndex, 4, UTF8_A2MFCSTRW(recordInfo.strbuymoney()));
	m_listTotal.SetItemText(nIndex, 5, strCount);
	m_listTotal.SetItemText(nIndex, 6, UTF8_A2MFCSTRW(recordInfo.businesstime()));
	
// 	if (recordInfo.lockclient() == 0)
// 	{
// 		m_listTotal.SetItem(nIndex, 7, UTF8_A2MFCSTRW(recordInfo.strlockclient()), nImage0);
// 	}
// 	else if (recordInfo.lockclient() == 1)
// 	{
// 		m_listTotal.SetItem(nIndex, 7, UTF8_A2MFCSTRW(recordInfo.strlockclient()), nImage1);
// 	}
// 	else if (recordInfo.lockclient() == 2)
// 	{
// 		m_listTotal.SetItem(nIndex, 7, UTF8_A2MFCSTRW(recordInfo.strlockclient()), nImage2);
// 	}
	m_listTotal.SetItemText(nIndex, 7, UTF8_A2MFCSTRW(recordInfo.strlockclient()));
	m_listTotal.SetItemText(nIndex, 8, UTF8_A2MFCSTRW(recordInfo.strrecordstatus()));
	
	return TRUE;
}

//查询异常录像列表
BOOL CBusinessListDlg::QueryErrorRecordsList(CString strStartTime, CString strEndTime, CString strConsumerName, CString strProductName)
{
	cms_8120::RspErrRecordsInfo rspErrorRecInfo;
	if (!CCMSMgr::SnglPtr()->GetErrorRecList(rspErrorRecInfo, strStartTime, strEndTime, strConsumerName, strProductName, m_emListType) || rspErrorRecInfo.result() != 0)
	{
		CString strInfo = UTF8_A2MFCSTRW(rspErrorRecInfo.result_info());
		FMSCLIENT_ERROR("Get List Failed:%s", fcT2A(strInfo));
		return FALSE;
	}
	else
	{
		cms_8120::ErrRecordsInfo errorRecInfo;
		for(int i = 0; i < rspErrorRecInfo.errrecordsinfo_size(); i++)
		{
			errorRecInfo = rspErrorRecInfo.errrecordsinfo(i);
			InsertToErrorList(errorRecInfo);
			m_listErrorRecInfo.AddTail(errorRecInfo);
		}
	}

	return TRUE;
}

BOOL CBusinessListDlg::InsertToErrorList(cms_8120::ErrRecordsInfo& errorRecInfo)
{
	int nIndex = m_listException.GetItemCount();
	CString strIndex;
	strIndex.Format(_T("%d"), nIndex + 1);

	m_listException.InsertItem(nIndex, strIndex);
	m_listException.SetItemText(nIndex, 1, UTF8_A2MFCSTRW(errorRecInfo.filename()));
	m_listException.SetItemText(nIndex, 2, UTF8_A2MFCSTRW(errorRecInfo.nproid()));
	m_listException.SetItemText(nIndex, 3, UTF8_A2MFCSTRW(errorRecInfo.custname()));
	m_listException.SetItemText(nIndex, 4, UTF8_A2MFCSTRW(errorRecInfo.productname()));
	m_listException.SetItemText(nIndex, 5, UTF8_A2MFCSTRW(errorRecInfo.strbuymoney()));
	m_listException.SetItemText(nIndex, 6, UTF8_A2MFCSTRW(errorRecInfo.businesstime()));
	m_listException.SetItemText(nIndex, 7, UTF8_A2MFCSTRW(errorRecInfo.strrecordcheck()));
	m_listException.SetItemText(nIndex, 8, UTF8_A2MFCSTRW(errorRecInfo.strrecordstatus()));

	return TRUE;
}

BOOL CBusinessListDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// 多按钮点击消息
	if (MSG_MULTIBTN_CLICK == wParam)  
	{
		//按钮信息的结构体
		STRU_MULTIBTN_INFO * multiBtnInfo = (STRU_MULTIBTN_INFO*)lParam;
		//CString strFileName = theApp.m_strAppCapPath + _T("\\") + m_listRecords.GetItemText(multiBtnInfo->nItem, 1);
		int nItem = multiBtnInfo->nItem;
		if (-1 == nItem)
		{
			FMSCLIENT_ERROR("未选中任何项!\n");
			//return FALSE;
		}

		if (EM_LIST_TOTAL == m_emListType)  //全部（业务单）
		{
			//详情/编辑按钮
			if (0 == multiBtnInfo->nNumber)
			{
				CBusinessInfoDlg *pBusinessInfoDlg;
				try
				{
					pBusinessInfoDlg = new CBusinessInfoDlg;
				}
				catch (...)
				{
					return FALSE;
				}
				if (NULL != pBusinessInfoDlg)
				{
					pBusinessInfoDlg->SetDialogData(m_listBusinessInfo.GetAt(m_listBusinessInfo.FindIndex(nItem)));
					if (IDOK == pBusinessInfoDlg->DoModal())
					{
						//查询、刷新
						OnBnClickedBtnQuery();
					}
				}
				if(NULL != pBusinessInfoDlg)
				{
					delete pBusinessInfoDlg;
					pBusinessInfoDlg = NULL;
				}
				
			}
			else if (1 == multiBtnInfo->nNumber)  //回放
			{
				PlaybackBusiness(nItem);
			}
			else if (2 == multiBtnInfo->nNumber)  //重新上传
			{
				ReUploadBusiness(nItem);
				//处理完之后刷新
				UpdateData(FALSE);
			}
			else if (3 == multiBtnInfo->nNumber) //打印
			{
				PrintBusiness(nItem);
			}
		}
		else if (EM_LIST_EXCEPTION == m_emListType)  //异常录像列表
		{
			if (0 == multiBtnInfo->nNumber)		//回放按钮
			{
				PlaybackErrorRecord(nItem);
			}
			else if (1 == multiBtnInfo->nNumber)  //重新上传
			{
				ReUploadErrorRecord(nItem);
				//处理完之后刷新
				UpdateData(FALSE);
			}
			else if (2 == multiBtnInfo->nNumber)  //删除
			{
				DeleteErrorRecord(nItem);
			}
		}

		//释放内存，并返回TRUE
		delete multiBtnInfo;
		return TRUE;
	}

	return CFDialog::OnCommand(wParam, lParam);
}

void CBusinessListDlg::OnBnClickedBtnReset()
{
	CFBusyWait busywait(this);
	busywait.ShowBusyWait(_T("正在重置..."));

	m_editConsumerName.SetWindowText(_T(""));
	m_editProductName.SetWindowText(_T(""));
	InitData();

	Invalidate();
}

BOOL CBusinessListDlg::PlaybackErrorRecord(int nItem)
{
	cms_8120::ErrRecordsInfo errRecordInfo = m_listErrorRecInfo.GetAt(m_listErrorRecInfo.FindIndex(nItem));
	if (NULL == m_pPlaybackDlg)
	{
		try
		{
			m_pPlaybackDlg = new CPlaybackCtrlDlg(this);
		}
		catch (...)
		{
			return FALSE;
		}
		m_pPlaybackDlg->Create(IDD_DLG_PLAYBACK_CTRL, this);
	}
	m_pPlaybackDlg->ShowWindow(SW_SHOW);
	m_pPlaybackDlg->StartPlayBack(0, UTF8_A2MFCSTRW(errRecordInfo.strurl()));

	return TRUE;
}

BOOL CBusinessListDlg::DeleteErrorRecord(int nItem)
{
	if (IDYES != FMessageBox::Show(_T("确定删除该条录像记录吗？"), _T("删除"), MB_YESNO | MB_ICONQUESTION))
	{
		return TRUE;
	}
	cms_8120::ErrRecordsInfo errorRec = m_listErrorRecInfo.GetAt(m_listErrorRecInfo.FindIndex(nItem));
	CUIntArray recIdArray;
	recIdArray.Add(errorRec.id());
	if (errorRec.recordstatus() != 9)
	{
		FMessageBox::Show(_T("非上传失败录像无法删除！"), _T("提示"), IDOK);
		return TRUE;
	}

	cms_8120::RspCsClientRecord rspRecord;
	if (!CCMSMgr::SnglPtr()->DeleteErrorRecord(rspRecord, recIdArray) || rspRecord.result() != 0)
	{
		CString strInfo = UTF8_A2MFCSTRW(rspRecord.result_info());
		FMSCLIENT_ERROR("Delete Error Record Failed:%d, %s", errorRec.id(), fcT2A(strInfo));
		FMessageBox::Show(_T("删除失败!"), _T("提示"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	else
	{
		m_listException.DeleteItem(nItem);
		FMSCLIENT_INFO("Delete Error Record Succeed:%d", errorRec.id());
		//FMessageBox::Show(_T("删除成功！"), _T("提示"), MB_OK);
		return TRUE;
	}

	return TRUE;
}

//异常录像重新上传
BOOL CBusinessListDlg::ReUploadErrorRecord(int nItem, BOOL bAll)
{
	cms_8120::ErrRecordsInfo errorRec = m_listErrorRecInfo.GetAt(m_listErrorRecInfo.FindIndex(nItem));
	if (!bAll && errorRec.recordstatus() != 9)
	{
		FMessageBox::Show(_T("只有上传失败的录像才能手动上传。"), _T("提示"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	
	CUIntArray recIdArray;
	recIdArray.Add(errorRec.id());
	cms_8120::RspCsClientRecord rspRecord;
	if (!CCMSMgr::SnglPtr()->ReUploadRecord(rspRecord, recIdArray) || rspRecord.result() != 0)
	{
		CString strInfo = UTF8_A2MFCSTRW(rspRecord.result_info());
		CString strText;
		strText.Format(_T("[%d]%s"), nItem+1, strInfo);
		FMessageBox::Show(strText, _T("提示"), MB_OK | MB_ICONINFORMATION);
		FMSCLIENT_ERROR("ReUpload Error Record Failed:%s", fcT2A(strText));
		return FALSE;
	}
	else
	{
		m_listException.SetItemText(nItem, 8, _T("未上传"));  //界面修改上传状态
		//FMessageBox::Show(_T("修改成功！"), _T("提示"), MB_OK);
		FMSCLIENT_INFO("ReUploadErrorRecord Succeed,RecordId:%d", errorRec.id());
		return TRUE;
	}

	return TRUE;
}

//数据导出
void CBusinessListDlg::OnBnClickedBtnExport()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime timeStart;
	m_dateTimeStart.GetTime(timeStart);
	CTime timeEnd;
	m_dateTimeEnd.GetTime(timeEnd);
	CString strTime;
	strTime.Format(_T("(%04d%02d%02d%02d%02d%02d-%04d%02d%02d%02d%02d%02d)"), timeStart.GetYear(), timeStart.GetMonth(), timeStart.GetDay(), timeStart.GetHour(), timeStart.GetMinute(), timeStart.GetSecond(),
		timeEnd.GetYear(), timeEnd.GetMonth(), timeEnd.GetDay(), timeEnd.GetHour(), timeEnd.GetMinute(), timeEnd.GetSecond());

	CExcel excel(EXCEL_OT_MS_EXCEL, EXCEL_VERSION_2003);
	_tstring strFilePath;
	if (1 == m_emListType)  //全部业务记录
	{
		CString strName;
		strName.Format(_T("业务记录"));
		if (!excel.ShowSaveFile(strName, _T("导出业务记录"), strFilePath))
		{
			return;
		}

		if (!excel.NewExcelApp(FALSE))
		{
			FMessageBox::Show(_T("导出业务记录失败"), _T("错误"), MB_OK | MB_ICONERROR);
			return;
		}

		CFBusyWait busywait(this);
		busywait.ShowBusyWait(_T("正在导出..."));

		AddBusinessListSheet(excel);

	}
	else		//异常录像记录
	{
		CString strName;
		strName.Format(_T("异常录像记录"));
		if (!excel.ShowSaveFile(strName, _T("导出异常录像记录"), strFilePath))
		{
			return;
		}

		if (!excel.NewExcelApp(FALSE))
		{
			FMessageBox::Show(_T("导出异常录像记录失败"), _T("错误"), MB_OK | MB_ICONERROR);
			return;
		}

		CFBusyWait busywait(this);
		busywait.ShowBusyWait(_T("正在导出..."));

		AddExceptionListSheet(excel);
	}

	//保存EXCEL文件
	excel.SaveFile();
	
	FMessageBox::Show(_T("导出成功！"), _T("提示"), MB_OK);
}

//导出业务记录
void CBusinessListDlg::AddBusinessListSheet(CExcel& excel)
{
	excel.AddWorkSheet(_T("业务记录"), 1);
	
	//最大行数
	LONG lMaxRowCount = m_listTotal.GetItemCount();
	//最大列数
	LONG lMaxColumnCount = 8;
	//单元格高度
	DOUBLE dRowHeight = 15.5;
	//设置操作excel的范围
	excel.SetRange(lMaxRowCount+3,lMaxColumnCount,dRowHeight);
	//将excel操作范围的单元格边框加粗
	excel.SetBorder();

	//设置标题
	Struct_Title struTitle;
	struTitle.lStartRow = 1;//起始行号
	struTitle.lStartColumn = 1;//起始列号
	struTitle.lEndRow = 2;//结束行号
	struTitle.lEndColumn = lMaxColumnCount;//结束列号
	struTitle.strName = _T("业务记录");//标题名称
	struTitle.bBold = TRUE;//字体加粗
	struTitle.lFontSize = 20;//字体大小
	struTitle.lHAlign = 1;//文本居中
	excel.SetTitle(struTitle);

	//设置列标题
	std::vector<Struct_Header> vecHeader;
	Struct_Header struHeader;
	struHeader.lRow = 3;  //起始行号
	struHeader.lColumn = 1; //起始列号
	struHeader.strName = _T("业务单号");  //列名称
	struHeader.lWidth = 30;  //列宽度
	vecHeader.push_back(struHeader);

	struHeader.lColumn++;
	struHeader.strName = _T("客户姓名");
	struHeader.lWidth = 20;
	vecHeader.push_back(struHeader);

	struHeader.lColumn++;
	struHeader.strName = _T("产品名称");
	struHeader.lWidth = 20;
	vecHeader.push_back(struHeader);

	struHeader.lColumn++;
	struHeader.strName = _T("购买金额");
	struHeader.lWidth = 10;
	vecHeader.push_back(struHeader);

	struHeader.lColumn++;
	struHeader.strName = _T("录像数");
	struHeader.lWidth = 10;
	vecHeader.push_back(struHeader);

	struHeader.lColumn++;
	struHeader.strName = _T("办理时间");
	struHeader.lWidth = 50;
	vecHeader.push_back(struHeader);

	struHeader.lColumn++;
	struHeader.strName = _T("业务单状态");
	struHeader.lWidth = 15;
	vecHeader.push_back(struHeader);

	struHeader.lColumn++;
	struHeader.strName = _T("录像状态");
	struHeader.lWidth = 15;
	vecHeader.push_back(struHeader);

	excel.AddHeaders(vecHeader);

	excel.BeginWrite();
	_tstring strTemp;
	CString strData;
	for (int i = 0; i < lMaxRowCount; i++)
	{
		//只导出勾选的项
// 		if (!m_listTotal.IsItemChecked(i))
// 		{
// 			continue;
// 		}
		excel.StartWriteRow(i);
		for (int j = 0; j < lMaxColumnCount; j++)
		{
			strData = (_T("'") + m_listTotal.GetItemText(i, j+1));
			excel.WriteCell(i+4, j+1, strData.GetBuffer());
		}
		excel.EndWriteRow();
	}
	excel.EndWrite();

}

//导出异常录像记录
void CBusinessListDlg::AddExceptionListSheet(CExcel& excel)
{
	excel.AddWorkSheet(_T("异常录像记录"), 1);

	//最大行数
	LONG lMaxRowCount = m_listException.GetItemCount();
	//最大列数
	LONG lMaxColumnCount = 8;
	//单元格高度
	DOUBLE dRowHeight = 15.5;
	//设置操作excel的范围
	excel.SetRange(lMaxRowCount+3,lMaxColumnCount,dRowHeight);
	//将excel操作范围的单元格边框加粗
	excel.SetBorder();

	//设置标题
	Struct_Title struTitle;
	struTitle.lStartRow = 1;//起始行号
	struTitle.lStartColumn = 1;//起始列号
	struTitle.lEndRow = 2;//结束行号
	struTitle.lEndColumn = lMaxColumnCount;//结束列号
	struTitle.strName = _T("异常录像记录");//标题名称
	struTitle.bBold = TRUE;//字体加粗
	struTitle.lFontSize = 20;//字体大小
	struTitle.lHAlign = 1;//文本居中
	excel.SetTitle(struTitle);

	//设置列标题
	std::vector<Struct_Header> vecHeader;
	Struct_Header struHeader;
	struHeader.lRow = 3;  //起始行号
	struHeader.lColumn = 1; //起始列号
	struHeader.strName = _T("录像名称");  //列名称
	struHeader.lWidth = 30;  //列宽度
	vecHeader.push_back(struHeader);

	struHeader.lColumn++;
	struHeader.strName = _T("业务单号");
	struHeader.lWidth = 30;
	vecHeader.push_back(struHeader);

	struHeader.lColumn++;
	struHeader.strName = _T("客户姓名");
	struHeader.lWidth = 20;
	vecHeader.push_back(struHeader);

	struHeader.lColumn++;
	struHeader.strName = _T("产品名称");
	struHeader.lWidth = 10;
	vecHeader.push_back(struHeader);

	struHeader.lColumn++;
	struHeader.strName = _T("购买金额");
	struHeader.lWidth = 10;
	vecHeader.push_back(struHeader);

	struHeader.lColumn++;
	struHeader.strName = _T("办理时间");
	struHeader.lWidth = 50;
	vecHeader.push_back(struHeader);

	struHeader.lColumn++;
	struHeader.strName = _T("合规状态");
	struHeader.lWidth = 15;
	vecHeader.push_back(struHeader);

	struHeader.lColumn++;
	struHeader.strName = _T("上传状态");
	struHeader.lWidth = 15;
	vecHeader.push_back(struHeader);

	excel.AddHeaders(vecHeader);

	excel.BeginWrite();
	_tstring strTemp;
	CString strData;
	for (int i = 0; i < lMaxRowCount; i++)
	{
		//只导出勾选的项
// 		if (!m_listException.IsItemChecked(i))
// 		{
// 			continue;
// 		}
		excel.StartWriteRow(i);
		for (int j = 0; j < lMaxColumnCount; j++)
		{
			strData = (_T("'") + m_listException.GetItemText(i, j+1));
			excel.WriteCell(i+4, j+1, strData.GetBuffer());
		}
		excel.EndWriteRow();
	}
	excel.EndWrite();
}

//回放
BOOL CBusinessListDlg::PlaybackBusiness(int nItem)
{
	CString strFileNames = _T("");
	cms_8120::RecordsInfo recordInfo = m_listBusinessInfo.GetAt(m_listBusinessInfo.FindIndex(nItem));
	for (int i = recordInfo.recordsdetailinfo_size() - 1; i >= 0; i--)
	{
		RecordsDetailInfo recordDetail = recordInfo.recordsdetailinfo(i);
		strFileNames += UTF8_A2MFCSTRW(recordDetail.strurl());
		strFileNames += _T("|");  //构造文件路径，多个文件之间用分号隔开
	}

	//CPlaybackCtrlDlg playbackDlg;
// 	m_playbackDlg.Create(IDD_DLG_PLAYBACK_CTRL, this);
// 	m_playbackDlg.ShowWindow(SW_SHOW);
// 	m_playbackDlg.StartPlayBack(1, strFileNames);

	if (NULL == m_pPlaybackDlg)
	{
		try
		{
			m_pPlaybackDlg = new CPlaybackCtrlDlg(this);
		}
		catch (...)
		{
			return FALSE;
		}
		m_pPlaybackDlg->Create(IDD_DLG_PLAYBACK_CTRL, this);
	}
	m_pPlaybackDlg->ShowWindow(SW_SHOW);
	m_pPlaybackDlg->StartPlayBack(1, strFileNames);
	return TRUE;
}

//业务单录像重新上传
BOOL CBusinessListDlg::ReUploadBusiness(int nItem, BOOL bAll)
{
	CUIntArray recIdArray;
	cms_8120::RecordsInfo recordInfo = m_listBusinessInfo.GetAt(m_listBusinessInfo.FindIndex(nItem));
	for (int i = 0; i < recordInfo.recordsdetailinfo_size(); i++)
	{
		cms_8120::RecordsDetailInfo recordDetail = recordInfo.recordsdetailinfo(i);
		if (recordDetail.recordstatus() == 9) //9:上传失败
		{
			recIdArray.Add(recordDetail.videoid());
		}
	}

	if (recIdArray.GetSize() == 0)
	{
		if (!bAll)
		{
			FMessageBox::Show(_T("只有上传失败的录像才能手动上传。"), _T("提示"), MB_OK | MB_ICONINFORMATION);
		}
		return TRUE;
	}

	cms_8120::RspCsClientRecord rspRecord;
	if (!CCMSMgr::SnglPtr()->ReUploadRecord(rspRecord, recIdArray) || rspRecord.result() != 0)
	{
		CString strInfo = UTF8_A2MFCSTRW(rspRecord.result_info());
		CString strText;
		strText.Format(_T("[%d]%s"), nItem+1, strInfo);
		FMessageBox::Show(strText, _T("提示"), MB_OK | MB_ICONERROR);
		FMSCLIENT_ERROR("ReUpload Error Record Failed:%s", fcT2A(strText));
		return FALSE;
	}
	else
	{
		FMSCLIENT_INFO("ReUploadBusiness Succeed:RecordIndo Id:%d", recordInfo.id());
		m_listTotal.SetItemText(nItem, 8, _T("未上传"));  //界面修改上传状态
		return TRUE;
	}

	return TRUE;
}

//打印业务单
void CBusinessListDlg::PrintBusiness(int nItem)
{
	FMSCLIENT_INFO("Into PrintBusiness");
	cms_8120::RecordsInfo recordInfo = m_listBusinessInfo.GetAt(m_listBusinessInfo.FindIndex(nItem));

	CString strUrl = theApp.m_strAppPath + _T("\\printBusiness.html");
	HtmlParser htmlParser;
	MemBuffer memOutHtml;
	MemBuffer memHtml;

	int len = WideCharToMultiByte(CP_ACP, 0, strUrl, -1, NULL, 0, NULL, NULL);  
	char * ptxtTemp = new char[len + 1];  
	WideCharToMultiByte(CP_ACP, 0, strUrl, -1, ptxtTemp, len, NULL, NULL);  
	
	char * fileName = "printBusiness.html";
	if (memHtml.loadFromFile(ptxtTemp))
	{
		MemBuffer outFileName;
		outFileName.appendText(fileName);
		outFileName.appendText(".notes.txt", -1, true);
		FILE *out = fopen((const char*)outFileName.getData(), "wb+");
		if (out == NULL)
		{
			FMSCLIENT_ERROR("Can't open output file:%s", outFileName.getData());
		}
		htmlParser.parseHtml((const char*)memHtml.getData(), true);
		htmlParser.dumpHtmlNodes(out);		//输出解析文件
		fclose(out);

		CString strTemp;
		//客户姓名
		strTemp = UTF8_A2MFCSTRW(recordInfo.custname());
		if (_T("") == strTemp)
		{
			return;
		}
		TCHAR * pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_CONSUMER_NAME, pTemp);

		//证件类型
		strTemp = UTF8_A2MFCSTRW(recordInfo.creditname());
		if (_T("") == strTemp)
		{
			return;
		}
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_ID_TYPE, pTemp);

		//证件号码
			strTemp = UTF8_A2MFCSTRW(recordInfo.creditcode());
		if (_T("") == strTemp)
		{
			return;
		}
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_ID_NUMBER, pTemp);

		//业务单号
		strTemp = UTF8_A2MFCSTRW(recordInfo.nproid());
		if (_T("") == strTemp)
		{
			return;
		}
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_BUSINESS_ID, pTemp);

		//办理时间
		strTemp = UTF8_A2MFCSTRW(recordInfo.businesstime());
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_BUSINESS_TIME, pTemp);

		//发行机构
		strTemp = UTF8_A2MFCSTRW(recordInfo.strgrade());
		if (_T("") == strTemp)
		{
			return;
		}
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_ISSUER, pTemp);

		//产品类型
		strTemp = UTF8_A2MFCSTRW(recordInfo.producttypename());
		if (_T("") == strTemp)
		{
			return;
		}
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_PRODUCT_TYPE, pTemp);

		//产品名称
		strTemp = UTF8_A2MFCSTRW(recordInfo.productname());
		if (_T("") == strTemp)
		{
			return;
		}
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_PRODUCT_NAME, pTemp);

		//业务员姓名
		strTemp = theApp.m_strSalesmanName;
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_SALESMAN_NAME, pTemp);

		//业务员编号
		strTemp = theApp.m_strSalesmanCode;
		pTemp = strTemp.GetBuffer();
		htmlParser.SetHtmlNodeText(INDEX_SALESMAN_ID, pTemp);

		CString strPicStart = UTF8_A2MFCSTRW(recordInfo.picturestrart());
		if (strPicStart.IsEmpty())
		{
			FMSCLIENT_ERROR("pictureStart is empty.");
			pTemp = TEXT("nopic");
		}
		else
		{
			strPicStart.Replace(_T("\\"), _T("//"));
			pTemp = strPicStart.GetBuffer();
		}
		htmlParser.SetAttributeValue(INDEX_PIC1, pTemp);

		CString strPicEnd = UTF8_A2MFCSTRW(recordInfo.pictureend());
		if (strPicEnd.IsEmpty())
		{
			FMSCLIENT_ERROR("pictureEnd is empty.");
			pTemp = TEXT("nopic");
		}
		else
		{
			strPicEnd.Replace(_T("\\"), _T("//"));
			pTemp = strPicEnd.GetBuffer();
		}
		htmlParser.SetAttributeValue(INDEX_PIC2, pTemp);

		memOutHtml.empty();
		htmlParser.outputHtml(memOutHtml);
		outFileName.empty();
		outFileName.appendText(fileName);
		memOutHtml.saveToFile((const char*)outFileName.getData());
	}
	else
	{
		FMSCLIENT_ERROR("Can't open print file:%s!%ld", fileName, GetLastError());
		return;
	}

	FMSCLIENT_INFO("Before PrintBusiness ShellExecute");
	//打开ie浏览器
	HINSTANCE hinst = ShellExecute(NULL, _T("open"), _T("iexplore.exe"), strUrl, NULL, SW_SHOWMAXIMIZED);
	FMSCLIENT_INFO("After PrintBusiness ShellExecute");

	delete[] ptxtTemp;  
}

//上传按钮
void CBusinessListDlg::OnBnClickedBtnUpload()
{
	CFBusyWait busywait(this);
	busywait.ShowBusyWait(_T("正在操作..."));

	if (EM_LIST_TOTAL == m_emListType)  //全部（业务单）
	{

		for (int i = 0; i < m_listTotal.GetItemCount(); i++)
		{
			if (!ReUploadBusiness(i, TRUE))
			{
				//FMessageBox::Show(_T("操作失败！"), _T("错误"), MB_OK | MB_ICONERROR);
				continue;
			}
		}
	}
	else if (EM_LIST_EXCEPTION == m_emListType)  //异常录像列表
	{
		for (int i = 0; i < m_listException.GetItemCount(); i++)
		{
			cms_8120::ErrRecordsInfo errorRec = m_listErrorRecInfo.GetAt(m_listErrorRecInfo.FindIndex(i));
			if (errorRec.recordstatus() != 9)  //非上传失败不上传
			{
				continue;
			}

			if (!ReUploadErrorRecord(i, TRUE))
			{
				//FMessageBox::Show(_T("操作失败！"), _T("错误"), MB_OK | MB_ICONERROR);
				continue;
			}
		}
	}

	Sleep(1000);
	//处理完之后重新查询
	OnBnClickedBtnQuery();
	UpdateData(FALSE);

	return;
}


//业务记录：全部列表项点击事件
void CBusinessListDlg::OnNMClickListTotal(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
// 		CString strtemp;
// 		strtemp.Format(_T("单击的是第%d行第%d列"),
// 			pNMListView->iItem, pNMListView->iSubItem);
// 		AfxMessageBox(strtemp);
	}

// 	int nItem = pNMListView->iItem;
// 	int nSubItem = pNMListView->iSubItem;
// 	//点击业务单号列，显示详情
// 	if (nItem != -1 && nSubItem == 1)  
// 	{
// 		CBusinessInfoDlg *pBusinessInfoDlg;
// 		try
// 		{
// 			pBusinessInfoDlg = new CBusinessInfoDlg;
// 		}
// 		catch (...)
// 		{
// 			return;
// 		}
// 		if (pBusinessInfoDlg)
// 		{
// 			pBusinessInfoDlg->SetDialogData(m_listBusinessInfo.GetAt(m_listBusinessInfo.FindIndex(nItem)));
// 			if (IDOK == pBusinessInfoDlg->DoModal())
// 			{
// 				//查询、刷新
// 				OnBnClickedBtnQuery();
// 			}
// 		}
// 		if(NULL != pBusinessInfoDlg)
// 		{
// 			delete pBusinessInfoDlg;
// 			pBusinessInfoDlg = NULL;
// 		}
// 	}


	*pResult = 0;
}

BOOL CBusinessListDlg::PreTranslateMessage(MSG* pMsg)
{
	// 屏蔽ESC和Enter键
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))  
	{
			return TRUE;
	}
	else if ((GetFocus() == GetDlgItem(IDC_EDIT_CONSUMERNAME)) &&  (pMsg->message == WM_CHAR))	//客户姓名特殊字符约束
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
	else if ((GetFocus() == GetDlgItem(IDC_EDIT_PRODUCTNAME)) &&  (pMsg->message == WM_CHAR))	//产品名称特殊字符约束
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


	return CFDialog::PreTranslateMessage(pMsg);
}


//查询接口
void CBusinessListDlg::QueryBusiness(BOOL bShowError)
{
	// TODO: 在此添加控件通知处理程序代码
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
		if(bShowError)
		{
			FMessageBox::Show(_T("查询开始时间大于结束时间"), _T("提示"), MB_OK | MB_ICONINFORMATION);
			return;
		}
		else
		{
			InitData();
			return;
		}
	}

	CTimeSpan span = timeEnd - timeStart;
	if (span.GetDays() > 31)			//时间跨度>31天
	{
		if(bShowError)
		{
			FMessageBox::Show(_T("查询时间间隔不能超过31天"), _T("提示"), MB_OK | MB_ICONINFORMATION);
		}
		return;
	}

	CFBusyWait busywait(this);
	busywait.ShowBusyWait(_T("正在查询..."));

	CString strConsumerName;
	m_editConsumerName.GetWindowText(strConsumerName);

	CString strProductName;
	m_editProductName.GetWindowText(strProductName);

	if (1 == m_emListType)		//业务记录(业务单)
	{
		m_listTotal .DeleteAllItems();
		m_listBusinessInfo.RemoveAll();
		QueryBusinessList(strStartTime, strEndTime, strConsumerName, strProductName);
	}
	else if (2 == m_emListType)  //异常录像
	{
		m_listException.DeleteAllItems();
		m_listErrorRecInfo.RemoveAll();
		QueryErrorRecordsList(strStartTime, strEndTime, strConsumerName, strProductName);
	}
}