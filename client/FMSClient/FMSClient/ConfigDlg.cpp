// ConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FMSClient.h"
#include "ConfigDlg.h"
#include "CMSDataOperate.h"
#include "utils/Markup.h"

#define PV_XML_PARAM	_T("Param")
#define PV_XML_CAMERA	_T("Camera")
#define PV_XML_CAMINDEX	_T("CamIndex")
#define PV_XML_CAMNAME	_T("CamName")

//最大支持USB设备数
#define MAX_USB_DEVICE 5


// CConfigDlg 对话框

IMPLEMENT_DYNAMIC(CConfigDlg, CFDialog)

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CFDialog(CConfigDlg::IDD, pParent)
	, m_strUploadStart(_T(""))
	, m_strUploadEnd(_T(""))
	, m_strMaxRecordTime(_T(""))
	,m_nCamIndex(0)
	,m_strCamName(_T(""))
{

}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CFDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_BASIC_PARM, m_btnBasicParm);
	DDX_Control(pDX, IDC_STATIC_BASIC_PARM, m_staticBasicParm);
	DDX_Control(pDX, IDC_STATIC_DEFAULT_FOLDER, m_staticDefaultFolder);
	DDX_Control(pDX, IDC_EDIT_DEFAULT_FOLDER, m_editDefaultFolder);
	DDX_Control(pDX, IDC_BTN_CHANGE_DIR, m_btnChangeDir);
	DDX_Control(pDX, IDC_BTN_OPEN_FOLDER, m_btnOpenFolder);
	DDX_Control(pDX, IDC_BTN_SALESMAN_INFO, m_btnSalesmanInfo);
	DDX_Control(pDX, IDC_STATIC_SALESMAN_INFO, m_staticSalesmanInfo);
	DDX_Control(pDX, IDC_STATIC_DEFAULT_IPC, m_staticDefaultIPC);
	DDX_Control(pDX, IDC_COMBO_DEFAULT_IPC, m_comboDefaultIPC);
	DDX_Control(pDX, IDC_STATIC_RECORDTIME, m_staticRecTime);
	DDX_Control(pDX, IDC_EDIT_RECORDTIME, m_editRecTime);
	DDX_Control(pDX, IDC_STATIC_AUDIOSET, m_staticAudioSet);
	DDX_Control(pDX, IDC_CHECK_OPEN, m_checkOpenAduio);
	DDX_Control(pDX, IDC_CHECK_CLOSE, m_checkCloseAudio);
	DDX_Control(pDX, IDC_BTN_UPLOAD_INFO, m_btnUploadParm);
	DDX_Control(pDX, IDC_STATIC_UPLOAD_INFO, m_staticUploadParm);
	DDX_Control(pDX, IDC_STATIC_UPLOADTIME, m_staticUploadTime);
	DDX_Control(pDX, IDC_EDIT_UPLOADTIMESTART, m_editUploadTimeStart);
	DDX_Control(pDX, IDC_STATIC_AND, m_staticAnd);
	DDX_Control(pDX, IDC_EDIT_UPLOADTIMEEND, m_editUploadTimeEnd);
	DDX_Text(pDX, IDC_EDIT_UPLOADTIMESTART, m_strUploadStart);
	DDX_Text(pDX, IDC_EDIT_UPLOADTIMEEND, m_strUploadEnd);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, ID_RESET, m_btnCancel);
	DDX_Text(pDX, IDC_EDIT_RECORDTIME, m_strMaxRecordTime);
	DDX_Control(pDX, IDC_BTN_SYSTEM_INFO, m_btnSystemInfo);
	DDX_Control(pDX, IDC_STATIC_SYSTEM_INFO, m_staticSystemInfo);
	DDX_Control(pDX, IDC_STATIC_STARTUPSET, m_staticStartupSet);
	DDX_Control(pDX, IDC_CHECK_STARTUP, m_checkStartup);
	DDX_Control(pDX, IDC_STATIC_CLOSE, m_staticClose);
	DDX_Control(pDX, IDC_CHECK_EXIT, m_checkExit);
	DDX_Control(pDX, IDC_CHECK_TRAY, m_checkTray);
	DDX_Control(pDX, IDC_STATIC_MINUTE, m_staticMinute);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CFDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_OPEN, &CConfigDlg::OnBnClickedCheckOpen)
	ON_BN_CLICKED(IDC_CHECK_CLOSE, &CConfigDlg::OnBnClickedCheckClose)
	ON_BN_CLICKED(IDC_BTN_CHANGE_DIR, &CConfigDlg::OnBnClickedBtnChangeDir)
	ON_BN_CLICKED(IDC_BTN_OPEN_FOLDER, &CConfigDlg::OnBnClickedBtnOpenFolder)
	ON_BN_CLICKED(IDOK, &CConfigDlg::OnBnClickedOk)
	ON_CBN_DROPDOWN(IDC_COMBO_DEFAULT_IPC, &CConfigDlg::OnCbnDropdownComboDefaultIpc)
	ON_BN_CLICKED(IDC_CHECK_EXIT, &CConfigDlg::OnBnClickedCheckExit)
	ON_BN_CLICKED(IDC_CHECK_TRAY, &CConfigDlg::OnBnClickedCheckTray)
	ON_BN_CLICKED(ID_RESET, &CConfigDlg::OnBnClickedReset)
END_MESSAGE_MAP()


// CConfigDlg 消息处理程序

BOOL CConfigDlg::OnInitDialog()
{
	CFDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	MoveWindow(BUSINESS_WND_SIZE);

	InitCtrl();

	MoveCtrl();

	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CConfigDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
}

void CConfigDlg::InitCtrl(void)
{
	std::vector<int> vecMargin;
	vecMargin.push_back(0);
	vecMargin.push_back(0);
	vecMargin.push_back(0);
	vecMargin.push_back(0);
	SetMargin(vecMargin);

	//基本参数
	m_btnBasicParm.SetIcon(_T("navi_bkg"), 1);
	m_staticBasicParm.SetTextFont(YAHEI_18_B);
	m_staticDefaultFolder.SetTextFont(YAHEI_18_N);
	m_staticDefaultFolder.SetTextAlign(DT_RIGHT);
	m_editDefaultFolder.SetReadOnly(TRUE);
	m_editDefaultFolder.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_btnChangeDir.SetObjectName(_T("reset"));
	m_btnChangeDir.SetTextColor(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, FUi::Blue);
	m_btnChangeDir.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_btnOpenFolder.SetObjectName(_T("btn"));
	m_btnOpenFolder.SetTextColor(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, FUi::White);
	m_btnOpenFolder.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);

	//业务员信息
	m_btnSalesmanInfo.SetIcon(_T("navi_bkg"), 1);
	m_staticSalesmanInfo.SetTextFont(YAHEI_18_B);
	m_staticDefaultIPC.SetTextFont(YAHEI_18_N);
	m_staticDefaultIPC.SetTextAlign(DT_RIGHT);
	m_comboDefaultIPC.SetTextFont(1, YAHEI_18_N);
	m_comboDefaultIPC.SetObjectName(_T("combo1"));
	m_comboDefaultIPC.SetReadOnly(TRUE);
	m_staticRecTime.SetTextFont(YAHEI_18_N);
	m_staticRecTime.SetTextAlign(DT_RIGHT);
	m_editRecTime.SetReadOnly(TRUE);
	m_editRecTime.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_staticAudioSet.SetTextFont(YAHEI_18_N);
	m_staticAudioSet.SetTextAlign(DT_RIGHT);
	m_checkOpenAduio.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_checkOpenAduio.SetObjectName(_T("radio"));
	m_checkOpenAduio.SetCheckBox();
	m_checkCloseAudio.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_checkCloseAudio.SetObjectName(_T("radio"));
	m_checkCloseAudio.SetCheckBox();

	//上传参数
	m_btnUploadParm.SetIcon(_T("navi_bkg"), 1);
	m_staticUploadParm.SetTextFont(YAHEI_18_B);
	m_staticUploadTime.SetTextFont(YAHEI_18_N);
	m_staticUploadTime.SetTextAlign(DT_RIGHT);
	m_staticUploadTime.SetTextAlign(DT_RIGHT);
	m_editUploadTimeStart.SetReadOnly(TRUE);
	m_editUploadTimeStart.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_editUploadTimeEnd.SetReadOnly(TRUE);
	m_editUploadTimeEnd.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_staticMinute.SetTextFont(YAHEI_18_N);

	//系统参数
	m_btnSystemInfo.SetIcon(_T("navi_bkg"), 1);
	m_staticSystemInfo.SetTextFont(YAHEI_18_B);
	m_staticStartupSet.SetTextFont(YAHEI_18_N);
	m_staticStartupSet.SetTextAlign(DT_RIGHT);
	m_checkStartup.SetObjectName(_T("cbtn"));
	m_checkStartup.SetCheckBox();
	m_staticClose.SetTextFont(YAHEI_18_N);
	m_staticClose.SetTextAlign(DT_RIGHT);
	m_checkExit.SetObjectName(_T("radio"));
	m_checkExit.SetCheckBox();
	m_checkExit.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_checkTray.SetObjectName(_T("radio"));
	m_checkTray.SetCheckBox();
	m_checkTray.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);

	//保存&重置按钮
	m_btnOK.SetObjectName(_T("btn"));
	m_btnOK.SetTextAlign(DT_CENTER);
	m_btnOK.SetTextColor(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, FUi::White);
	m_btnOK.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);
	m_btnCancel.SetObjectName(_T("cancel"));
	m_btnCancel.SetTextAlign(DT_CENTER);
	m_btnCancel.SetTextColor(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed, FUi::Black);
	m_btnCancel.SetTextFont(FUi::ctrl_state::Normal | FUi::ctrl_state::Hovered | FUi::ctrl_state::Pressed | FUi::ctrl_state::Disabled, YAHEI_18_N);

	m_staticUploadTime.SetTextAlign(DT_CENTER);

	return;
}

void CConfigDlg::MoveCtrl(void)
{
	CRect rcClient;
	GetClientRect(rcClient);

	//基本参数
	CRect rcBtnBasicParm(rcClient);
	rcBtnBasicParm.left = rcBtnBasicParm.left + 40;
	rcBtnBasicParm.top = rcBtnBasicParm.top + 33;
	rcBtnBasicParm.right = rcBtnBasicParm.left + 12;
	rcBtnBasicParm.bottom = rcBtnBasicParm.top + 10;
	m_btnBasicParm.MoveWindow(rcBtnBasicParm);

	CRect rcStaticBasicParm(rcClient);
	rcStaticBasicParm.left = rcBtnBasicParm.right + 6;
	rcStaticBasicParm.top = rcClient.top + 30;
	rcStaticBasicParm.right = rcStaticBasicParm.left + 80;
	rcStaticBasicParm.bottom = rcStaticBasicParm.top + 26;
	m_staticBasicParm.MoveWindow(rcStaticBasicParm);

	//默认文件夹
	CRect rcStaticDefaultFolder(rcClient);
	rcStaticDefaultFolder.left = rcClient.left + 70;
	rcStaticDefaultFolder.top = rcStaticBasicParm.bottom + 8;
	rcStaticDefaultFolder.right = rcStaticDefaultFolder.left + 80;
	rcStaticDefaultFolder.bottom = rcStaticDefaultFolder.top + 20;
	m_staticDefaultFolder.MoveWindow(rcStaticDefaultFolder);

	CRect rcEditDefaultFolder(rcClient);
	rcEditDefaultFolder.left = rcStaticDefaultFolder.right + 10;
	rcEditDefaultFolder.top = rcStaticBasicParm.bottom + 5;
	rcEditDefaultFolder.right = rcEditDefaultFolder.left + 340;
	rcEditDefaultFolder.bottom = rcEditDefaultFolder.top + 26;
	m_editDefaultFolder.MoveWindow(rcEditDefaultFolder);

	CRect rcBtnChangeDir(rcEditDefaultFolder);
	rcBtnChangeDir.left = rcBtnChangeDir.right + 16;
	rcBtnChangeDir.right = rcBtnChangeDir.left + 70;
	m_btnChangeDir.MoveWindow(rcBtnChangeDir);

	CRect rcBtnOpenFolder(rcBtnChangeDir);
	rcBtnOpenFolder.left = rcBtnOpenFolder.right + 10;
	rcBtnOpenFolder.right = rcBtnOpenFolder.left + 80;
	m_btnOpenFolder.MoveWindow(rcBtnOpenFolder);

	//业务员信息
	CRect rcBtnSalesmanInfo(rcClient);
	rcBtnSalesmanInfo.left = rcClient.left + 40;
	rcBtnSalesmanInfo.top = rcClient.top + 118;
	rcBtnSalesmanInfo.right = rcBtnSalesmanInfo.left + 12;
	rcBtnSalesmanInfo.bottom = rcBtnSalesmanInfo.top + 10;
	m_btnSalesmanInfo.MoveWindow(rcBtnSalesmanInfo);

	CRect rcStaticSalesmanInfo(rcClient);
	rcStaticSalesmanInfo.left = rcBtnSalesmanInfo.right + 6;
	rcStaticSalesmanInfo.top = rcClient.top + 115;
	rcStaticSalesmanInfo.right = rcStaticSalesmanInfo.left + 80;
	rcStaticSalesmanInfo.bottom = rcStaticSalesmanInfo.top + 26;
	m_staticSalesmanInfo.MoveWindow(rcStaticSalesmanInfo);

	//默认相机
	CRect rcStaticDefaultIPC(rcClient);
	rcStaticDefaultIPC.left = rcClient.left + 50;
	rcStaticDefaultIPC.top = rcStaticSalesmanInfo.bottom + 8;
	rcStaticDefaultIPC.right = rcStaticDefaultIPC.left + 100;
	rcStaticDefaultIPC.bottom = rcStaticDefaultIPC.top + 26;
	m_staticDefaultIPC.MoveWindow(rcStaticDefaultIPC);

	CRect rcComboIPC(rcStaticDefaultIPC);
	rcComboIPC.left = rcComboIPC.right + 10;
	rcComboIPC.top = rcStaticSalesmanInfo.bottom + 5;
	rcComboIPC.right = rcComboIPC.left + 340;
	rcComboIPC.bottom = rcComboIPC.top + 26;
	m_comboDefaultIPC.MoveWindow(rcComboIPC);
	m_comboDefaultIPC.SetItemHeight(-1, 22);  //需要先MoveWindow,再SetItemHeight

	//录像提示时间
	CRect rcStaticRecTime(rcStaticDefaultIPC);
	rcStaticRecTime.top = rcStaticRecTime.bottom + 14;
	rcStaticRecTime.right = rcStaticRecTime.left + 100;
	rcStaticRecTime.bottom = rcStaticRecTime.top + 26;
	m_staticRecTime.MoveWindow(rcStaticRecTime);

	CRect rcEditRecTime(rcStaticRecTime);
	rcEditRecTime.left = rcEditRecTime.right + 10;
	rcEditRecTime.top = rcComboIPC.bottom + 14;
	rcEditRecTime.right = rcEditRecTime.left + 340;
	rcEditRecTime.bottom = rcEditRecTime.top + 26;
	m_editRecTime.MoveWindow(rcEditRecTime);

	CRect rcStaticMinute(rcEditRecTime);
	rcStaticMinute.left = rcStaticMinute.right + 5;
	rcStaticMinute.top += 5;
	rcStaticMinute.right = rcStaticMinute.left + 30;
	m_staticMinute.MoveWindow(rcStaticMinute);

	//声音
	CRect rcStaticAudio(rcStaticRecTime);
	rcStaticAudio.top = rcStaticAudio.bottom + 14;
	rcStaticAudio.bottom = rcStaticAudio.top + 26;
	m_staticAudioSet.MoveWindow(rcStaticAudio);

	CRect rcCheckAudioOpen(rcStaticAudio);
	rcCheckAudioOpen.left = rcStaticAudio.right + 10;
	rcCheckAudioOpen.top = rcEditRecTime.bottom + 14;
	rcCheckAudioOpen.right = rcCheckAudioOpen.left + 100;
	rcCheckAudioOpen.bottom = rcCheckAudioOpen.top + 26;
	m_checkOpenAduio.MoveWindow(rcCheckAudioOpen);

	CRect rcCheckAudioClose(rcCheckAudioOpen);
	rcCheckAudioClose.left = rcCheckAudioClose.right + 10;
	rcCheckAudioClose.right = rcCheckAudioClose.left + 90;
	m_checkCloseAudio.MoveWindow(rcCheckAudioClose);


	//上传参数
	CRect rcBtnUploadParm(rcClient);
	rcBtnUploadParm.left = rcClient.left + 40;
	rcBtnUploadParm.top = rcClient.top + 282;
	rcBtnUploadParm.right = rcBtnUploadParm.left + 12;
	rcBtnUploadParm.bottom = rcBtnUploadParm.top + 10;
	m_btnUploadParm.MoveWindow(rcBtnUploadParm);

	CRect rcStaticUploadParm(rcClient);
	rcStaticUploadParm.left = rcBtnSalesmanInfo.right + 6;
	rcStaticUploadParm.top = rcClient.top + 280;
	rcStaticUploadParm.right = rcStaticUploadParm.left + 80;
	rcStaticUploadParm.bottom = rcStaticUploadParm.top + 26;
	m_staticUploadParm.MoveWindow(rcStaticUploadParm);

	//录像上传时间
	CRect rcStaticUploadTime(rcClient);
	rcStaticUploadTime.left = rcStaticUploadTime.left + 64;
	rcStaticUploadTime.top = rcStaticUploadParm.bottom + 10;
	rcStaticUploadTime.right = rcStaticUploadTime.left + 86;
	rcStaticUploadTime.bottom = rcStaticUploadTime.top + 26;
	m_staticUploadTime.MoveWindow(rcStaticUploadTime);

	CRect rcEditUploadStart(rcStaticUploadTime);
	rcEditUploadStart.left = rcEditUploadStart.right + 10;
	rcEditUploadStart.top = rcStaticUploadParm.bottom + 5;
	rcEditUploadStart.right = rcEditUploadStart.left + 140;
	rcEditUploadStart.bottom = rcEditUploadStart.top + 26;
	m_editUploadTimeStart.MoveWindow(rcEditUploadStart);

	CRect rcStaticAnd(rcEditUploadStart);
	rcStaticAnd.left = rcStaticAnd.right + 10;
	rcStaticAnd.right = rcStaticAnd.left + 10;
	m_staticAnd.MoveWindow(rcStaticAnd);

	CRect rcEditUploadEnd(rcStaticAnd);
	rcEditUploadEnd.left = rcEditUploadEnd.right + 10;
	rcEditUploadEnd.right = rcEditUploadEnd.left + 140;
	m_editUploadTimeEnd.MoveWindow(rcEditUploadEnd);

	//系统参数
	CRect rcBtnSysParm(rcClient);
	rcBtnSysParm.left = rcClient.left + 40;
	rcBtnSysParm.top = rcClient.top + 367;
	rcBtnSysParm.right = rcBtnSysParm.left + 12;
	rcBtnSysParm.bottom = rcBtnSysParm.top + 10;
	m_btnSystemInfo.MoveWindow(rcBtnSysParm);

	CRect rcStaticSysParm(rcClient);
	rcStaticSysParm.left = rcBtnSysParm.right + 6;
	rcStaticSysParm.top = rcClient.top + 364;
	rcStaticSysParm.right = rcStaticSysParm.left + 80;
	rcStaticSysParm.bottom = rcStaticSysParm.top + 26;
	m_staticSystemInfo.MoveWindow(rcStaticSysParm);

	//启动设置
	CRect rcStaticStart(rcStaticUploadTime);
	rcStaticStart.top = rcStaticSysParm.bottom + 8;
	rcStaticStart.bottom = rcStaticStart.top + 26;
	m_staticStartupSet.MoveWindow(rcStaticStart);

	CRect rcCheckStart(rcStaticStart);
	rcCheckStart.left = rcCheckStart.right + 10;
	rcCheckStart.top = rcCheckStart.top - 3;
	rcCheckStart.right = rcCheckStart.left +90;
	rcCheckStart.bottom = rcCheckStart.top + 26;
	m_checkStartup.MoveWindow(rcCheckStart);

	//关闭按钮
	CRect rcStaticClose(rcStaticStart);
	rcStaticClose.top = rcStaticClose.bottom + 5;
	rcStaticClose.bottom = rcStaticClose.top + 26;
	m_staticClose.MoveWindow(rcStaticClose);

	CRect rcCheckExit(rcStaticClose);
	rcCheckExit.left = rcCheckExit.right + 10;
	rcCheckExit.top = rcCheckExit.top - 3;
	rcCheckExit.right = rcCheckExit.left + 90;
	rcCheckExit.bottom = rcCheckExit.top + 26;
	m_checkExit.MoveWindow(rcCheckExit);

	CRect rcCheckTray(rcCheckExit);
	rcCheckTray.left = rcCheckTray.right + 10;
	rcCheckTray.right = rcCheckTray.left + 150;
	m_checkTray.MoveWindow(rcCheckTray);

	//保存&取消按钮
	CRect rcBtnOK(rcClient);
	rcBtnOK.left = rcBtnOK.left + 58;	//和大标题对齐
	rcBtnOK.top = rcBtnOK.top + 480;
	rcBtnOK.right = rcBtnOK.left + 90;
	rcBtnOK.bottom = rcBtnOK.top + 30;
	m_btnOK.MoveWindow(rcBtnOK);

	CRect rcCancel(rcBtnOK);
	rcCancel.left = rcCancel.right + 10;
	rcCancel.right = rcCancel.left + 90;
	m_btnCancel.MoveWindow(rcCancel);

}

void CConfigDlg::InitData(void)
{
	GetSystemConfig();
	GetLocalConfig();
	UpdateData(FALSE);
}

BOOL CConfigDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)  
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:  //屏蔽Enter
			return TRUE;
		case VK_ESCAPE:   //屏蔽ESC键
			return TRUE;
		}
	}

	return CFDialog::PreTranslateMessage(pMsg);
}

BOOL CConfigDlg::GetSystemConfig(void)
{
	cms_8120::RspSysconfig rspSystemCfg;
	if (!CCMSMgr::SnglPtr()->GetSystemConfig(rspSystemCfg) || rspSystemCfg.result() != 0)
	{
		FMSCLIENT_ERROR("Get System Config Failed!");
		return FALSE;
	}
	else
	{
		cms_8120::SysconfigInfo systemCfg;
		for (int i = 0; i < rspSystemCfg.sysconfiginfo_size(); i++)
		{
			systemCfg = rspSystemCfg.sysconfiginfo(i);
			switch(systemCfg.nkey())
			{
			case MAX_RECORD_TIME:
				m_strMaxRecordTime = UTF8_A2MFCSTRW(systemCfg.strvalue());
				theApp.m_nMaxRecordTime = My_atoi(m_strMaxRecordTime);
				break;
			case UPLOAD_TIME_START:
				m_strUploadStart = UTF8_A2MFCSTRW(systemCfg.strvalue());
				break;
			case UPLOAD_TIME_END:
				m_strUploadEnd = UTF8_A2MFCSTRW(systemCfg.strvalue());
				break;
			default:
				break;
			}
		}
	}

	return TRUE;
}

void CConfigDlg::GetLocalConfig()
{
	//配置文件路径
	CString strConfigPath = theApp.m_strAppPath;
	strConfigPath.Append(_T("\\"));
	strConfigPath.Append(CLIENT_CONFIG_FILENAME);

	TCHAR szCapPath[MAX_PATH] = {0};
	GetPrivateProfileString(_T("LocalConfig"), _T("CapPath"), theApp.m_strAppCapPath, szCapPath, MAX_PATH, strConfigPath);
	m_editDefaultFolder.SetWindowText(szCapPath);

	m_nCamIndex = GetPrivateProfileInt(_T("LocalConfig"), _T("CamIndex"), 0, strConfigPath);
	CString strUSBCamInfo= _T("");
	EnumAllUSB();
	int nKey;
	POSITION pos = m_USBMap.GetStartPosition();
	while(pos)
	{
		m_USBMap.GetNextAssoc(pos, nKey, strUSBCamInfo);
		if (nKey == m_nCamIndex)
		{
			m_strCamName = strUSBCamInfo;
			//m_comboDefaultIPC.SetText(m_strCamName);
			break;
		}
	}
	m_comboDefaultIPC.SetCurSel(m_nCamIndex);

	int nPreviewAudio = GetPrivateProfileInt(_T("LocalConfig"), _T("PreviewAudio"), 0, strConfigPath);
	if (nPreviewAudio == 1)
	{
		m_checkOpenAduio.SetCheck(1);
		m_checkCloseAudio.SetCheck(0);
	}
	else if (nPreviewAudio == 0)
	{
		m_checkOpenAduio.SetCheck(0);
		m_checkCloseAudio.SetCheck(1);
	}
	
	int nStartUp = GetPrivateProfileInt(_T("LocalConfig"), _T("BootStart"), 0, strConfigPath);
	if (nStartUp == 0)
	{
		m_checkStartup.SetCheck(0);
	}
	else if (nStartUp == 1)
	{
		m_checkStartup.SetCheck(1);
	}

	int nClose = GetPrivateProfileInt(_T("LocalConfig"), _T("Close"), 1, strConfigPath);
	if (nClose == 1)
	{
		m_checkExit.SetCheck(1);
		m_checkTray.SetCheck(0);
	}
	else if (nClose == 2)
	{
		m_checkExit.SetCheck(0);
		m_checkTray.SetCheck(1);
	}

	return;
}

void CConfigDlg::OnBnClickedCheckOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	m_checkOpenAduio.SetCheck(1);
	m_checkCloseAudio.SetCheck(0);
}

void CConfigDlg::OnBnClickedCheckClose()
{
	// TODO: 在此添加控件通知处理程序代码
	m_checkOpenAduio.SetCheck(0);
	m_checkCloseAudio.SetCheck(1);
}

//修改保存路径
void CConfigDlg::OnBnClickedBtnChangeDir()
{
	TCHAR szPath[MAX_PATH] = {0};

	BROWSEINFO bi;
	memset(&bi, 0, sizeof(BROWSEINFO));
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = _T("选择文件保存路径");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (idl && SHGetPathFromIDList(idl, szPath))
	{
		CString strPath = szPath;
		strPath += _T("\\");
		m_editDefaultFolder.SetWindowText(strPath);
	}

	return;
}

//打开文件夹
void CConfigDlg::OnBnClickedBtnOpenFolder()
{
	UpdateData(FALSE);

	CString strFolder = _T("");
	m_editDefaultFolder.GetWindowText(strFolder);
	strFolder.Replace(_T("\\\\"), _T("\\"));

	//打开资源管理器
	HINSTANCE hInstance = ShellExecute(NULL, _T("open"), strFolder, NULL, NULL, SW_SHOW);
	BOOL bRet = FALSE;
	switch((int)hInstance)
	{
	case 0:
		MessageBox(_T("内存不足"), _T("Error"));
		break;
	case ERROR_FILE_NOT_FOUND:
		MessageBox(_T("文件名或路径名错误"), _T("Error"));
		break;
	case ERROR_PATH_NOT_FOUND:
		MessageBox(_T("文件名或路径名错误"), _T("Error"));
		break;
	case ERROR_BAD_FORMAT:
		MessageBox(_T("ERROR_BAD_FORMAT"), _T("Error"));
		break;
	case SE_ERR_ASSOCINCOMPLETE:
		MessageBox(_T("SE_ERR_ASSOCINCOMPLETE"), _T("Error"));
		break;
	case SE_ERR_ACCESSDENIED:
		MessageBox(_T("SE_ERR_ACCESSDENIED"), _T("Error"));
		break;
	case SE_ERR_DDEBUSY:
		MessageBox(_T("SE_ERR_DDEBUSY"), _T("Error"));
		break;
	case SE_ERR_DDEFAIL:
		MessageBox(_T("SE_ERR_DDEFAIL"), _T("Error"));
		break;
	case SE_ERR_DDETIMEOUT:
		MessageBox(_T("SE_ERR_DDETIMEOUT"), _T("Error"));
		break;
	case SE_ERR_DLLNOTFOUND:
		MessageBox(_T("SE_ERR_DLLNOTFOUND"), _T("Error"));
		break;
	case SE_ERR_NOASSOC:
		MessageBox(_T("SE_ERR_NOASSOC"), _T("Error"));
		break;
	case SE_ERR_OOM:
		MessageBox(_T("SE_ERR_OOM"), _T("Error"));
		break;
	case SE_ERR_SHARE:
		MessageBox(_T("SE_ERR_SHARE"), _T("Error"));
		break;
	default:
		bRet = TRUE;
		break;
	}

	return;
}

void CConfigDlg::OnBnClickedOk()
{
	// 保存
	CString strIniFile = theApp.GetAppPath() + _T("\\") + CLIENT_CONFIG_FILENAME;

	//默认文件夹
	CString strCapPath;
	m_editDefaultFolder.GetWindowText(strCapPath);
	BOOL bRet = WritePrivateProfileString(_T("LocalConfig"), _T("CapPath"), strCapPath, strIniFile);
	if (!bRet)
	{
		DWORD dwError = GetLastError();
		FMSCLIENT_ERROR("Save LocalConfig CapPath failed[%u]", dwError);
		FMessageBox::Show(_T("保存失败！"), _T("保存配置"), MB_OK | MB_ICONERROR);
		return;
	}

	//默认摄像头索引
	int nIndex = m_comboDefaultIPC.GetCurSel();
	if (nIndex < 0)
	{
		nIndex = 0;
	}
	CString strIndex = My_itot(nIndex);
	bRet = WritePrivateProfileString(_T("LocalConfig"), _T("CamIndex"), strIndex, strIniFile);
	if (!bRet)
	{
		DWORD dwError = GetLastError();
		FMSCLIENT_ERROR("Save LocalConfig CamIndex failed[%u]", dwError);
		FMessageBox::Show(_T("保存失败！"), _T("保存配置"), MB_OK | MB_ICONERROR);
		return;
	}
	FMSCLIENT_INFO("Save LocalConfig CamIndex[%d]", nIndex);

	//预览声音
	int nOpenAudio = 0;
	if (m_checkOpenAduio.GetCheck())
	{
		nOpenAudio = 1;
	}
	else if (m_checkCloseAudio.GetCheck())
	{
		nOpenAudio = 0;
	}
	CString strPreviewAudio = My_itot(nOpenAudio);
	bRet = WritePrivateProfileString(_T("LocalConfig"), _T("PreviewAudio"), strPreviewAudio, strIniFile);
	if (!bRet)
	{
		DWORD dwError = GetLastError();
		FMSCLIENT_ERROR("Save LocalConfig PreviewAudio failed[%u]", dwError);
		FMessageBox::Show(_T("保存失败！"), _T("保存配置"), MB_OK | MB_ICONERROR);
		return;
	}

	int nStart = 0;
	if (m_checkStartup.GetCheck())  //开机启动
	{
		nStart = 1;
		SetAutoRun(true);
	}
	else	//取消开机启动
	{
		nStart = 0;
		SetAutoRun(false);
	}
	CString strStart = My_itot(nStart);
	bRet = WritePrivateProfileString(_T("LocalConfig"), _T("BootStart"), strStart, strIniFile);
	if (!bRet)
	{
		DWORD dwError = GetLastError();
		FMSCLIENT_ERROR("Save LocalConfig BootStart failed[%u]", dwError);
		FMessageBox::Show(_T("保存失败！"), _T("保存配置"), MB_OK | MB_ICONERROR);
		return;
	}

	//关闭按钮:1-关闭程序；2-最小化到系统托盘
	int nClose = -1;
	if (m_checkExit.GetCheck())
	{
		nClose = 1;
	}
	else if (m_checkTray.GetCheck())
	{
		nClose = 2;
	}
	CString strClose = My_itot(nClose);
	bRet = WritePrivateProfileString(_T("LocalConfig"), _T("Close"), strClose, strIniFile);
	if (!bRet)
	{
		DWORD dwError = GetLastError();
		FMSCLIENT_ERROR("Save LocalConfig Close failed[%u]", dwError);
		FMessageBox::Show(_T("保存失败！"), _T("保存配置"), MB_OK | MB_ICONERROR);
		return;
	}

	FMessageBox::Show(_T("保存成功（重启后生效）。"), _T("保存配置"), MB_OK);

	return;
}

//设置开机是否启动
BOOL CConfigDlg::SetAutoRun(bool bRun)
{
	// 写入注册表，开机启动
	WCHAR szAppName[MAX_PATH];
	GetModuleFileName(theApp.m_hInstance, szAppName, MAX_PATH);
	CString strAppPath(szAppName);
	LPCTSTR lpszKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	CRegKey regeditKey;
	if (bRun)  //开机启动
	{
		if (ERROR_SUCCESS == regeditKey.Open(HKEY_LOCAL_MACHINE, lpszKey))
		{
			regeditKey.SetStringValue(_T("FMSClient"), strAppPath);
			regeditKey.Close();
		}
	}
	else	//取消开机启动
	{
		if (ERROR_SUCCESS == regeditKey.Open(HKEY_LOCAL_MACHINE, lpszKey))    
		{  
			regeditKey.DeleteValue(_T("FMSClient"));   
			regeditKey.Close();
		}  
	}

	return TRUE;
}

void CConfigDlg::OnCbnDropdownComboDefaultIpc()
{
	//清空 
	m_comboDefaultIPC.ResetContent();

	CString strUSBCamInfo= _T("");
	EnumAllUSB();
	int nKey;
	POSITION pos = m_USBMap.GetStartPosition();
	while(pos)
	{
		m_USBMap.GetNextAssoc(pos, nKey, strUSBCamInfo);
		if (nKey == m_nCamIndex)
		{
			m_strCamName = strUSBCamInfo;
		}
		//m_comboDefaultIPC.AddString(strUSBCamInfo);
	}	
	m_comboDefaultIPC.SetCurSel(0);

	return;
}

BOOL CConfigDlg::EnumAllUSB(void)
{
	m_USBMap.RemoveAll();
	m_comboDefaultIPC.ResetContent();

	CString strUSBCamInfo= _T("");
	if (!PV_GetAllUSBCameraInfo(strUSBCamInfo))
	{
		FMSCLIENT_ERROR("Client PV_GetAllUSBCameraInfo Failed!");
		return FALSE;
	}

	CMarkup xml;
	if (!xml.SetDoc(strUSBCamInfo))
	{
		return FALSE;
	}

	if (!xml.FindElem(PV_XML_PARAM))
	{
		return FALSE;
	}

	xml.IntoElem();

	for (int i = 0; i < MAX_USB_DEVICE; i++)
	{
		if (!xml.FindElem(PV_XML_CAMERA))
		{
			break;;
		}
		xml.IntoElem();
		int nIndex = 0;
		CString strCamName = _T("");
		if (xml.FindElem(PV_XML_CAMINDEX))
		{
			nIndex = _wtoi(xml.GetData());
		}
		if (xml.FindElem(PV_XML_CAMNAME))
		{
			strCamName = xml.GetData();
			m_comboDefaultIPC.AddString(strCamName);
		}
		m_USBMap.SetAt(nIndex, strCamName);
		xml.OutOfElem();
	}

	return TRUE;
}

//选择退出程序
void CConfigDlg::OnBnClickedCheckExit()
{
	m_checkExit.SetCheck(TRUE);
	m_checkTray.SetCheck(FALSE);
}

//选择最小化到系统托盘
void CConfigDlg::OnBnClickedCheckTray()
{
	m_checkExit.SetCheck(FALSE);
	m_checkTray.SetCheck(TRUE);
}

void CConfigDlg::OnBnClickedReset()
{
	// TODO: 在此添加控件通知处理程序代码
// 	m_editDefaultFolder.SetWindowText(theApp.m_strDefaultCapPath);
// 	EnumAllUSB();
// 	int nKey;
// 	CString strUSBCamInfo = _T("");
// 	POSITION pos = m_USBMap.GetStartPosition();
// 	while(pos)
// 	{
// 		m_USBMap.GetNextAssoc(pos, nKey, strUSBCamInfo);
// 		if (nKey == m_nCamIndex)
// 		{
// 			m_strCamName = strUSBCamInfo;
// 		}
// 		m_comboDefaultIPC.AddString(strUSBCamInfo);
// 	}	
// 	m_comboDefaultIPC.SetCurSel(0);
// 
// 	m_checkOpenAduio.SetCheck(0);
// 	m_checkCloseAudio.SetCheck(1);
// 
// 	m_checkStartup.SetCheck(0);
// 	m_checkExit.SetCheck(1);
// 	m_checkTray.SetCheck(0);

	InitData();

	return;
}

int CConfigDlg::GetUSBCamCount(void)
{
	EnumAllUSB();
	return m_USBMap.GetSize();
}
