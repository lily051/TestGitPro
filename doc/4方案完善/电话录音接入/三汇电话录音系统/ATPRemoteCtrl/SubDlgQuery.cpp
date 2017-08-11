// SubDlgQuery.cpp : implementation file
//

#include "stdafx.h"
#include "ATPRemoteCtrl.h"
#include "SubDlgQuery.h"
#include "ATPRemoteCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubDlgQuery dialog


CSubDlgQuery::CSubDlgQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CSubDlgQuery::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubDlgQuery)
	m_szMsg = _T("");
	m_tStart = 0;
	m_tEnd = 0;
	m_nCalcMode = 0;
	m_nCalcMonth = -1;
	m_szCalcYear = _T("");
	m_nEndCh = 0;
	m_nStartCh = 0;
	m_nStartIndex = 0;
	m_tEndDate = 0;
	m_tEndTime = 0;
	m_tStartDate = 0;
	m_tStartTime = 0;
	m_nMediaType = 0;
	//}}AFX_DATA_INIT
	m_tEndDate = m_tEnd = CTime::GetCurrentTime();
	CTimeSpan ts(30,0,0,0);
	m_tStartDate = m_tStart = m_tEnd-ts;
	m_nCalcMonth = m_tEnd.GetMonth()-1;
	m_tStartTime += ts;
	m_tEndTime += ts;
}


void CSubDlgQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubDlgQuery)
	DDX_Text(pDX, IDC_EDIT_MSG, m_szMsg);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_tStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_tEnd);
	DDX_Radio(pDX, IDC_RADIO_CALCDATA_MODE, m_nCalcMode);
	DDX_CBIndex(pDX, IDC_COMBO_CALC_MON, m_nCalcMonth);
	DDX_CBString(pDX, IDC_COMBO_CALC_YEAR, m_szCalcYear);
	DDX_Text(pDX, IDC_EDIT_END_CH, m_nEndCh);
	DDX_Text(pDX, IDC_EDIT_START_CH, m_nStartCh);
	DDX_Text(pDX, IDC_EDIT_START_INDEX, m_nStartIndex);
	DDX_DateTimeCtrl(pDX, IDC_END_DATE, m_tEndDate);
	DDX_DateTimeCtrl(pDX, IDC_END_TIME, m_tEndTime);
	DDX_DateTimeCtrl(pDX, IDC_START_DATE, m_tStartDate);
	DDX_DateTimeCtrl(pDX, IDC_START_TIME, m_tStartTime);
	DDX_CBIndex(pDX, IDC_COMBO_MEDIA_TYPE, m_nMediaType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSubDlgQuery, CDialog)
	//{{AFX_MSG_MAP(CSubDlgQuery)
	ON_BN_CLICKED(IDC_BUTTON_ExecuteSQL, OnBUTTONExecuteSQL)
	ON_BN_CLICKED(IDC_BUTTON_CalcData, OnBUTTONCalcData)
	ON_BN_CLICKED(IDC_RADIO_CALCDATA_MODE, OnRadioCalcdataMode)
	ON_BN_CLICKED(IDC_RADIO_CALCDATA_MODE2, OnRadioCalcdataMode2)
	ON_BN_CLICKED(IDC_BUTTON_QueryRecord, OnBUTTONQueryRecord)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubDlgQuery message handlers

void CSubDlgQuery::OnTargetChanged(int nTarget)
{
	if (nTarget==0) // dll
	{
		GetDlgItem(IDC_EDIT_MSG)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ExecuteSQL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_QueryRecord)->EnableWindow(FALSE);
		GetDlgItem(IDC_START_DATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_START_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_END_DATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_END_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_START_CH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_END_CH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_START_INDEX)->EnableWindow(FALSE);
	}
	else //soap
	{
		GetDlgItem(IDC_EDIT_MSG)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ExecuteSQL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_QueryRecord)->EnableWindow(TRUE);
		GetDlgItem(IDC_START_DATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_START_TIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_END_DATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_END_TIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_START_CH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_END_CH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_START_INDEX)->EnableWindow(TRUE);
	}
	OnRadioCalcdataMode();

}

void CSubDlgQuery::OnBUTTONExecuteSQL() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		MessageBox("SyAtp.dll has not this function yet!");
	}
	else
	{
		ns1__ExecuteSQLResponse p1;
		ret = TopDlg(GetParent())->m_pSynapiProxy->ExecuteSQL(m_szMsg.GetBuffer(0), p1);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			tmpMsg.Format("result=%d, sqlinfo=%s", p1.ExecuteSQLReturn->result, p1.ExecuteSQLReturn->sqlinfo);
			TopDlg(GetParent())->AppendResult(tmpMsg);
		}
	}
	
}

void CSubDlgQuery::OnBUTTONCalcData() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		CalculateInput ci;
		ci.cSelectType=m_nCalcMode;
		if (m_nCalcMode)
		{
			ci.stMonthwise.iYear = atoi(m_szCalcYear);
			ci.stMonthwise.ucMonth = m_nCalcMonth+1;
		}
		else
		{
			ci.stStartDate.iYear = m_tStart.GetYear();
			ci.stStartDate.ucMonth = m_tStart.GetMonth();
			ci.stStartDate.ucDay = m_tStart.GetDay();
			ci.stEndDate.iYear = m_tEnd.GetYear();
			ci.stEndDate.ucMonth = m_tEnd.GetMonth();
			ci.stEndDate.ucDay = m_tEnd.GetDay();
		}
		if (m_nMediaType<0)
		{
			MessageBox("Select Media Type");
			return;
		}
		ci.mediaType=E_DVD_R+m_nMediaType;
		ret = CalculateData(g_szDefaultServer, &ci);
		if (ret)
		{
			tmpMsg.Format("result=%d",ret);
			MessageBox(tmpMsg);
		}
	}
	else
	{
		ns1__CalculateInput ci;
		ci.SelectType=m_nCalcMode;
		if (m_nCalcMode)
		{
			ci.monthwise_USCOREyear = atoi(m_szCalcYear);
			ci.monthwise_USCOREmonth = m_nCalcMonth+1;
			ci.monthwise_USCOREday = 1;
		}
		else
		{
			ci.start_USCOREyear = m_tStart.GetYear();
			ci.start_USCOREmonth = m_tStart.GetMonth();
			ci.start_USCOREday = m_tStart.GetDay();
			ci.end_USCOREyear = m_tEnd.GetYear();
			ci.end_USCOREmonth = m_tEnd.GetMonth();
			ci.end_USCOREday = m_tEnd.GetDay();
		}
		if (m_nMediaType<0)
		{
			MessageBox("Select Media Type");
			return;
		}
		ci.mediaType=E_DVD_R+m_nMediaType;
		ns1__CalculateDataResponse p1;
		ret = TopDlg(GetParent())->m_pSynapiProxy->CalculateData(&ci, p1);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			tmpMsg.Format("TotalCalls=%d,TotalSize=%d(MB)/%d(kB), Enough space in DVD=%d",
				p1.CalculateDataResponse->totalcalls, p1.CalculateDataResponse->totalsizeM,
				p1.CalculateDataResponse->totalsizeKB, p1.CalculateDataResponse->sizeAvailable);
			TopDlg(GetParent())->AppendResult(tmpMsg);
		}
	}
	
}

void CSubDlgQuery::OnRadioCalcdataMode() 
{
	{
		if (!UpdateData(TRUE)) return;
		if (m_nCalcMode==0)
		{
			GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(TRUE);
			GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow(TRUE);
			GetDlgItem(IDC_COMBO_CALC_YEAR)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_CALC_MON)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(FALSE);
			GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_CALC_YEAR)->EnableWindow(TRUE);
			GetDlgItem(IDC_COMBO_CALC_MON)->EnableWindow(TRUE);
		}
	}
}

void CSubDlgQuery::OnRadioCalcdataMode2() 
{
	OnRadioCalcdataMode();
}

BOOL CSubDlgQuery::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CComboBox * pCb = (CComboBox*)GetDlgItem(IDC_COMBO_CALC_YEAR);
	CString tmp;
	tmp.Format("%d", m_tStart.GetYear()-1);
	pCb->InsertString(0,tmp);
	tmp.Format("%d", m_tStart.GetYear());
	pCb->InsertString(1,tmp);
	pCb->SetCurSel(1);
	OnTargetChanged(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSubDlgQuery::OnBUTTONQueryRecord() 
{
	int ret;
	if (!UpdateData(TRUE)) return;
	if (TopDlg(GetParent())->m_nTarget==0) // dll
	{
		MessageBox("SyAtp.dll has not this function yet!");
	}
	else
	{
		ns1__QueryRecordResponse p1;
		ret = TopDlg(GetParent())->m_pSynapiProxy->QueryRecord(
			m_tStartDate.Format("%Y-%m-%d").GetBuffer(0), m_tStartTime.Format("%H:%M:%S").GetBuffer(0),
			m_tEndDate.Format("%Y-%m-%d").GetBuffer(0), m_tEndTime.Format("%H:%M:%S").GetBuffer(0),
			m_nStartCh, m_nEndCh, m_nStartIndex, p1);
		if (ret!=SOAP_OK)
		{
			tmpMsg.Format("soap failed, error code=%d", ret);
			MessageBox(tmpMsg);
		}
		else
		{
			tmpMsg.Format("result=%d, sqlinfo=%s", p1.QueryRecordReturn->result, p1.QueryRecordReturn->sqlinfo);
			TopDlg(GetParent())->AppendResult(tmpMsg);
		}
	}
	
}
