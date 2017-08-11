// DownloadByTime.cpp : 实现文件
//
//lint  -library
#include "stdafx.h"
#include "../PlaybackSimpleDll.h"
#include "DownloadByTime.h"


// CDownloadByTime 对话框

IMPLEMENT_DYNAMIC(CDownloadByTime, CDialog)

CDownloadByTime::CDownloadByTime(CWnd* pParent /*=NULL*/)
	: CDialog(CDownloadByTime::IDD, pParent)
{
	m_pDownloadDlg = NULL;
	m_nChannel = -1;
	m_nCameraID = -1;
	m_dwRev = 0;
	m_strCamName = _T("");
	m_pLoginInfo = NULL;
	m_nCheckNum = 0;
	m_emPlayType = PB_TYPE_DVR;
}

CDownloadByTime::~CDownloadByTime()
{
	m_pDownloadDlg = NULL;
	m_pLoginInfo = NULL;
}

void CDownloadByTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATASTART1, m_dttDateStart1);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATASTART2, m_dttDateStart2);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATASTART3, m_dttDateStart3);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATASTART4, m_dttDateStart4);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TIMESTART1, m_dttTimeStart1);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TIMESTART2, m_dttTimeStart2);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TIMESTART3, m_dttTimeStart3);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TIMESTART4, m_dttTimeStart4);
	DDX_Control(pDX, IDC_DATETIMEPICKER__DATEEND1, m_dttDateEnd1);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATEEND1, m_dttDateEnd2);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATEEND3, m_dttDateEnd3);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATEEND4, m_dttDateEnd4);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TIMEEND1, m_dttTimeEnd1);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TIMEEND2, m_dttTimeEnd2);
	DDX_Control(pDX, IDC_DATETIMEPICKER__TIMEEND3, m_dttTimeEnd3);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TIMEEND4, m_dttTimeEnd4);
	DDX_Control(pDX, IDC_STATIC_GROUP, m_staCamera);
	DDX_Control(pDX, IDC_CHK_ENABLE_TIME1, m_chkEnable1);
	DDX_Control(pDX, IDC_CHK_ENABLE_TIME2, m_chkEnable2);
	DDX_Control(pDX, IDC_CHK_ENABLE_TIME3, m_chkEnable3);
	DDX_Control(pDX, IDC_CHK_ENABLE_TIME4, m_chkEnable4);
}


BEGIN_MESSAGE_MAP(CDownloadByTime, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHK_ENABLE_TIME1, &CDownloadByTime::OnBnClickedChkEnableTime1)
	ON_BN_CLICKED(IDC_CHK_ENABLE_TIME2, &CDownloadByTime::OnBnClickedChkEnableTime2)
	ON_BN_CLICKED(IDC_CHK_ENABLE_TIME3, &CDownloadByTime::OnBnClickedChkEnableTime3)
	ON_BN_CLICKED(IDC_CHK_ENABLE_TIME4, &CDownloadByTime::OnBnClickedChkEnableTime4)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDownloadByTime 消息处理程序

void CDownloadByTime::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(rcClient, RGB(0xf5, 0xf2, 0xf2));
}

//初始化控件图片
void CDownloadByTime::InitCtrl()
{
	//time
	CTime tmpTm = CTime::GetCurrentTime();
	CTime tInitTime(tmpTm.GetYear(), tmpTm.GetMonth(), tmpTm.GetDay(), 0, 0, 0);
	m_dttTimeStart1.SetTime(&tInitTime);
	m_dttTimeStart2.SetTime(&tInitTime);
	m_dttTimeStart3.SetTime(&tInitTime);
	m_dttTimeStart4.SetTime(&tInitTime);
	m_dttTimeEnd1.SetTime(&tInitTime);
	m_dttTimeEnd2.SetTime(&tInitTime);
	m_dttTimeEnd3.SetTime(&tInitTime);
	m_dttTimeEnd4.SetTime(&tInitTime);

	//group
	m_staCamera.SetWindowText(m_strCameraName);

	// checkbox.
	m_chkEnable1.SetCheck(TRUE);
	EnableControlTime1();
	EnableControlTime2();
	EnableControlTime3();
	EnableControlTime4();

}

//判断时间信息是否有效
BOOL CDownloadByTime::IsTimeEffective(PB_TIME startTime, PB_TIME endTime)
{
	CTime tStart(startTime.dwYear, startTime.dwMonth, startTime.dwDay, 
		startTime.dwHour, startTime.dwMinute, startTime.dwSecond);
	CTime tEnd(endTime.dwYear, endTime.dwMonth, endTime.dwDay, 
		endTime.dwHour, endTime.dwMinute, endTime.dwSecond);

	time_t lStartTime, lEndTime;
	lStartTime = tStart.GetTime();
	lEndTime = tEnd.GetTime();

	if (lStartTime >= lEndTime)
	{
		return FALSE;
	}

	return TRUE;
}

/**   @fn          GetDownloadTime
 *    @brief   	   获取系统时间.
 *    @param[in]   pStartDate:开始日期.
 *    @param[in]   pStartTime:开始时间.
 *	  @param[in]   pStopDate:结束日期.
 *    @param[in]   pStopTime:结束时间.
 *    @return      
 */
void CDownloadByTime::GetDownloadTime(CDateTimeCtrl* pStartDate, CDateTimeCtrl* pStartTime, 
					 CDateTimeCtrl* pStopDate, CDateTimeCtrl* pStopTime, PLAY_PARAM& rPlayParam)
{
	ASSERT(pStartDate != NULL);
	ASSERT(pStartTime != NULL);
	ASSERT(pStopDate != NULL);
	ASSERT(pStopTime != NULL);

	COleDateTime oleTime;

	pStartDate->GetTime(oleTime);
	rPlayParam.startTime.dwYear = oleTime.GetYear();
	rPlayParam.startTime.dwMonth = oleTime.GetMonth();
	rPlayParam.startTime.dwDay = oleTime.GetDay();
	pStartTime->GetTime(oleTime);
	rPlayParam.startTime.dwHour = oleTime.GetHour();
	rPlayParam.startTime.dwMinute = oleTime.GetMinute();
	rPlayParam.startTime.dwSecond = oleTime.GetSecond();
	pStopDate->GetTime(oleTime);
	rPlayParam.stopTime.dwYear = oleTime.GetYear();
	rPlayParam.stopTime.dwMonth = oleTime.GetMonth();
	rPlayParam.stopTime.dwDay = oleTime.GetDay();
	pStopTime->GetTime(oleTime);
	rPlayParam.stopTime.dwHour = oleTime.GetHour();
	rPlayParam.stopTime.dwMinute = oleTime.GetMinute();
	rPlayParam.stopTime.dwSecond = oleTime.GetSecond();
}

/**   @fn          EnableControlTime1
 *    @brief   	   改变时间段1控件界面状态.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CDownloadByTime::EnableControlTime1()
{
	BOOL bCheck = m_chkEnable1.GetCheck();
	m_dttDateStart1.EnableWindow(bCheck);
	m_dttDateEnd1.EnableWindow(bCheck);
	m_dttTimeStart1.EnableWindow(bCheck);
	m_dttTimeEnd1.EnableWindow(bCheck);
}

/**   @fn          EnableControlTime2
 *    @brief   	   改变时间段1控件界面状态.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CDownloadByTime::EnableControlTime2()
{
	BOOL bCheck = m_chkEnable2.GetCheck();
	m_dttDateStart2.EnableWindow(bCheck);
	m_dttDateEnd2.EnableWindow(bCheck);
	m_dttTimeStart2.EnableWindow(bCheck);
	m_dttTimeEnd2.EnableWindow(bCheck);
}

/**   @fn          EnableControlTime3
 *    @brief   	   改变时间段1控件界面状态.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CDownloadByTime::EnableControlTime3()
{
	BOOL bCheck = m_chkEnable3.GetCheck();
	m_dttDateStart3.EnableWindow(bCheck);
	m_dttDateEnd3.EnableWindow(bCheck);
	m_dttTimeStart3.EnableWindow(bCheck);
	m_dttTimeEnd3.EnableWindow(bCheck);
}

/**   @fn          EnableControlTime4
 *    @brief   	   改变时间段1控件界面状态.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void CDownloadByTime::EnableControlTime4()
{
	BOOL bCheck = m_chkEnable4.GetCheck();
	m_dttDateStart4.EnableWindow(bCheck);
	m_dttDateEnd4.EnableWindow(bCheck);
	m_dttTimeStart4.EnableWindow(bCheck);
	m_dttTimeEnd4.EnableWindow(bCheck);
}

void CDownloadByTime::OnBnClickedChkEnableTime1()
{
	// TODO: 在此添加控件通知处理程序代码
	EnableControlTime1();
}

void CDownloadByTime::OnBnClickedChkEnableTime2()
{
	// TODO: 在此添加控件通知处理程序代码
	EnableControlTime2();
}

void CDownloadByTime::OnBnClickedChkEnableTime3()
{
	// TODO: 在此添加控件通知处理程序代码
	EnableControlTime3();
}

void CDownloadByTime::OnBnClickedChkEnableTime4()
{
	// TODO: 在此添加控件通知处理程序代码
	EnableControlTime4();
}


/**   @fn          AddDownload
 *    @brief   	   加入下载.	
 *    @param[in]   
 *    @param[in]   
 *    @return      TRUE:成功,FALSE:失败.
 */
BOOL CDownloadByTime::AddDownload()
{
	// 任务数量
	const int nTaskNum = 4;

	//获取时间信息
	COleDateTime oleTime;
	PLAY_PARAM stPlayParam[nTaskNum];
	for (int i = 0; i < nTaskNum; i++)
	{
		stPlayParam[i].emPlayType = m_emPlayType;
		stPlayParam->emRecordType = RECORD_TYPE_ALL;
	}

	BOOL bTimeEff[nTaskNum];
	BOOL bCheck[nTaskNum];

	//时间段1
	GetDownloadTime(&m_dttDateStart1, &m_dttTimeStart1, &m_dttDateEnd1, &m_dttTimeEnd1, stPlayParam[0]);
	bTimeEff[0] = IsTimeEffective(stPlayParam[0].startTime, stPlayParam[0].stopTime);
	bCheck[0] = m_chkEnable1.GetCheck();

	//时间段2
	GetDownloadTime(&m_dttDateStart2, &m_dttTimeStart2, &m_dttDateEnd2, &m_dttTimeEnd2, stPlayParam[1]);
	bTimeEff[1] = IsTimeEffective(stPlayParam[1].startTime, stPlayParam[1].stopTime);
	bCheck[1] = m_chkEnable2.GetCheck();

	//时间段3
	GetDownloadTime(&m_dttDateStart3, &m_dttTimeStart3, &m_dttDateEnd3, &m_dttTimeEnd3, stPlayParam[2]);
	bTimeEff[2] = IsTimeEffective(stPlayParam[2].startTime, stPlayParam[2].stopTime);
	bCheck[2] = m_chkEnable3.GetCheck();

	//时间段4
	GetDownloadTime(&m_dttDateStart4, &m_dttTimeStart4, &m_dttDateEnd4, &m_dttTimeEnd4, stPlayParam[3]);
	bTimeEff[3] = IsTimeEffective(stPlayParam[3].startTime, stPlayParam[3].stopTime);
	bCheck[3] = m_chkEnable4.GetCheck();

	CString strErrMsg = _T("");
	BOOL bTimeOK = TRUE;
	// 检查时间段是否合法.
	for (int i = 0; i < nTaskNum; i++)
	{
		if (!bTimeEff[i]  &&  bCheck[i])
		{
			if (bTimeOK)
			{
				CString strTemp;
				strTemp.Format(_T("时间段%d"), i + 1);
				strErrMsg += strTemp;
			}
			else
			{
				CString strTemp;
				strTemp.Format(_T("、时间段%d"), i + 1);
				strErrMsg += strTemp;
			}	
			bTimeOK = FALSE;
		}
	}
	strErrMsg += _T("无效!");

	// 时间段不合法.
	if (!bTimeOK)
	{
		BSTR str = strErrMsg.AllocSysString();
		MsgBoxApi_Show(str, MSGBOX_TYPE_WARNING);
		SysFreeString(str);
		return FALSE;
	}

	// 开始下载.
    BOOL bRet = TRUE;
    CString strErrTip;
	for (int i = 0; i < nTaskNum; i++)
	{
		if (bCheck[i]  &&  bTimeEff[i])
		{
            strErrMsg = _T("");
			if(!m_pDownloadDlg->AddTimeDownloadInfo(m_nCameraID, m_dwRev, m_strCamName, m_nChannel, *m_pLoginInfo, stPlayParam[i], strErrMsg))
            {
                strErrTip.AppendFormat(_T("时间段%d：添加失败%s\t\t"),i+1,(LPCTSTR)strErrMsg);
                bRet = FALSE;
            }
		}
	}

    if (!bRet)
    {
        MsgBoxApi_Show(strErrTip.GetBuffer(), MSGBOX_TYPE_WARNING);
    }

	return TRUE;
}

//传入下载窗口指针
void CDownloadByTime::SetDownloadDlgPoint(CDownloadDlg * pDownloadDlg)
{
	m_pDownloadDlg = pDownloadDlg;
}

/**   @fn          SetInitInfo
 *    @brief   	   传入其他信息.
 *    @param[in]   nCameraID:监控点ID.
 *    @param[in]   dwRev:保留字段.
 *    @param[in]   nChannel:通道号.
 *    @param[in]   emPlayType:回放类型.
 *    @return      
 */
void CDownloadByTime::SetInitInfo(int nCameraID, DWORD dwRev, const CString& strName, int nChannel, ENUM_PLAY_TYPE emPlayType, PB_LOGININFO* pLoginInfo)
{
	m_nCameraID = nCameraID;
	m_dwRev = dwRev;
	m_strCamName = strName;
	m_nChannel = nChannel;
	m_emPlayType = emPlayType;
	m_pLoginInfo = pLoginInfo;
}

HBRUSH CDownloadByTime::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(RGB(0xf5, 0xf2, 0xf2));
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(RGB(0xf5, 0xf2, 0xf2));
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

BOOL CDownloadByTime::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

int CDownloadByTime::GetTaskNum()
{
	m_nCheckNum = 0;
	if (m_chkEnable1.GetCheck())
	{
		m_nCheckNum++;
	}

	if (m_chkEnable2.GetCheck())
	{
		m_nCheckNum++;
	}

	if (m_chkEnable3.GetCheck())
	{
		m_nCheckNum++;
	}

	if (m_chkEnable4.GetCheck())
	{
		m_nCheckNum++;
	}

	return m_nCheckNum;
}