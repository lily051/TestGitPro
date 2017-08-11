/**   @file	    Server.cpp
 *    @note     HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief    �ͻ��˵�¼����
 *
 *    @author   ZhangYunjun
 *    @date     2015/07/13
 *
 *    @note     Description
 *    @note     ��ʷ��¼��
 *    @note     V1.0.0:details
 *
 *    @warning 
 */

#include "stdafx.h"
#include "Login.h"
#include "LoginDlg.h"
#include "MultiLang/tinyxml/tinyxml.h"
#include "MultiLang/fcTchar.h"
#include "Markup.h"
#include "StringConverter.h"
#include <set>
#include "../FMSClient/hlogconfig.h"


// ��¼ƽ̨�Զ�����Ϣ
#define WM_LOGINCENTER  (WM_USER + 8849)
#define WM_PRELOGIN     (WM_USER + 8850)
#define WM_AFTLOGIN     (WM_USER + 8851)

#define WM_LOGIN_FP_RESULT (WM_USER + 1000) // ��¼ָ�ƻ����
#define WM_FINGER_LOGIN    (WM_USER + 1001) // ָ�Ƶ�¼

// ��ȫɾ����Դ
#define SAFE_DEL_IMG(p) if (p) { delete (p); (p) = NULL;}
// ��¼��ʷ�����ļ�
#define LOGIN_FILE  _T("Config\\Login.xml")
// 
#define TIMER_LOGIN_PROGRESS 1234
#define TIMER_FINGER_REFLASH 1235   // ָ��ͼ��ˢ��

//�ϴ��ͻ���������Ϣ
#define CONFIG_FILE			"./UploadClient/ServiceConfig.xml"
#define TOP_DIRECTORY_CONFIG_FILE   "../ServiceConfig.xml"
#define NODE_ROOT			"ROOT"
#define NODE_LOCAL_PARAM	"LOCAL_PARAM"
#define NODE_DATA_CENTER	"DATA_CENTER"

// Ĭ�ϴ��ڴ�С
#define  DEFAULT_WND_SIZE   CRect(0, 0, 692, 346)

unsigned _stdcall WT_LoginCenter(void* p)
{
    CLoginDlg* pLoginDlg = (CLoginDlg*) p;
    if (NULL == pLoginDlg)
    {
        //FMSCLIENT_ERROR("Login thread param error");
        return 2;
    }

    pLoginDlg->LoginCenter();

    return 0;
}

void FingerLoginCallBack(int nResult, void* pUser)
{
    CLoginDlg* pDlg = (CLoginDlg*)pUser;

    if (pDlg)
    {
        PostMessage(pDlg->GetSafeHwnd(), WM_LOGIN_FP_RESULT, nResult, NULL);
    }
}

void FingerAlarmCallBack(char* strCardNo, void* pUser)
{
    CLoginDlg* pDlg = (CLoginDlg*)pUser;

    if (pDlg)
    {
        pDlg->FingerLogin(strCardNo);
    }
}

// CLoginDlg �Ի���

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(_LoginEvent pfunLoginEvent, void* pUser, 
                     int nUseAutoLogin, int nSkin, int nLanguage, 
                     CWnd* pParent)
	: CDialog(CLoginDlg::IDD, pParent)
    , m_bShowServer(FALSE)
    , m_bRemmberPwd(FALSE)
    , m_bAutoLogin(FALSE)
    , m_hLoginThread(NULL)
    , m_pImgBkg(NULL)
    , m_nSkin(nSkin)
    , m_nLanguage(nLanguage)
    , m_nUseAutoLogin(nUseAutoLogin)
    , m_pLoginEvent(pfunLoginEvent)
    , m_pUser(pUser)
	, m_bPassWordReset(FALSE)
    , m_nLoginType(LOGIN_SHOW_TYPE_ACCOUNT_ONLY)
    , m_pImgFinger(NULL)
    , m_nFingerImgId(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);

    GetAppPath();

	Hak1((char*)103, "DATA", NULL);

    LANG_INIT("CentralClient", m_nLanguage);
}

CLoginDlg::~CLoginDlg()
{
    try
    {
		Hak2(NULL, NULL, NULL);
        SAFE_DELETE_FONT(m_ftTitle);
        SAFE_DELETE_FONT(m_ftCopyright);
    }
    catch (...)
    {

    }
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_MIN, m_btnMin);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_BTN_LOGIN, m_btnLogin);
	DDX_Control(pDX, IDC_STATIC_WELCOME, m_stWelcome);
	DDX_Control(pDX, IDC_STATIC_COPYRIGHT, m_stCopyright);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_edPassword);
	DDX_Control(pDX, IDC_COMBO_USERNAME, m_cbUsername);
	DDX_Control(pDX, IDC_CHECK_REMEMBER, m_ckbRemember);
	DDX_Control(pDX, IDC_CHECK_AUTOLOGIN, m_ckbAutologin);
	DDX_Control(pDX, IDC_EDIT_SERVER_IP, m_edServerIP);
	DDX_Control(pDX, IDC_EDIT_SERVER_PORT, m_edServerPort);
	DDX_Control(pDX, IDC_BTN_ERROR, m_btnErrorTip);
	DDX_Control(pDX, IDC_PROGRESS_LOGIN, m_pgcLogin);
	DDX_Control(pDX, IDC_COMBO_LOCALIP, m_cbLocalIP);
	DDX_Control(pDX, IDC_BTN_CONFIG, m_btnConfig);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_USERLOGIN, m_btnUserLogin);
	DDX_Control(pDX, IDC_BUTTON_FPLOGIN, m_btnFpLogin);
	DDX_Control(pDX, IDC_STATIC_FINGER, m_stFinger);
	DDX_Control(pDX, IDC_BUTTON_LOCAL, m_btnLocalLogin);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
    ON_WM_ERASEBKGND()
    ON_WM_NCHITTEST()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BTN_CONFIG, &CLoginDlg::OnBnClickedBtnConfig)
    ON_BN_CLICKED(IDC_BTN_MIN, &CLoginDlg::OnBnClickedBtnMin)
    ON_BN_CLICKED(IDC_BTN_CLOSE, &CLoginDlg::OnBnClickedBtnClose)
    ON_BN_CLICKED(IDC_CHECK_REMEMBER, &CLoginDlg::OnBnClickedCheckRemember)
    ON_BN_CLICKED(IDC_CHECK_AUTOLOGIN, &CLoginDlg::OnBnClickedCheckAutologin)
    ON_BN_CLICKED(IDC_BTN_LOGIN, &CLoginDlg::OnBnClickedBtnLogin)
    ON_CBN_SELCHANGE(IDC_COMBO_USERNAME, &CLoginDlg::OnCbnSelchangeComboUsername)
    ON_MESSAGE(WM_LOGINCENTER, &CLoginDlg::OnMsgLoginCenter)
    ON_CBN_SELCHANGE(IDC_COMBO_LOCALIP, &CLoginDlg::OnCbnSelchangeComboLocalip)
    ON_MESSAGE(WM_PRELOGIN, &CLoginDlg::OnMsgPreLogin)
    ON_MESSAGE(WM_AFTLOGIN, &CLoginDlg::OnMsgAftLogin)
	ON_CBN_EDITUPDATE(IDC_COMBO_USERNAME, &CLoginDlg::OnCbnEditupdateComboUsername)
	ON_EN_SETFOCUS(IDC_EDIT_PASSWORD, &CLoginDlg::OnEnSetfocusEditPassword)
    ON_BN_CLICKED(IDC_BUTTON_SAVE, &CLoginDlg::OnBnClickedButtonSave)
    ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CLoginDlg::OnBnClickedButtonCancel)
    ON_BN_CLICKED(IDC_BUTTON_USERLOGIN, &CLoginDlg::OnBnClickedButtonUserlogin)
    ON_BN_CLICKED(IDC_BUTTON_FPLOGIN, &CLoginDlg::OnBnClickedButtonFplogin)
    ON_MESSAGE(WM_LOGIN_FP_RESULT, &CLoginDlg::OnMsgFpLoginResult)
    ON_MESSAGE(WM_FINGER_LOGIN, &CLoginDlg::OnMsgFingerLogin)
    ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_LOCAL, &CLoginDlg::OnBnClickedButtonLocal)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CLoginDlg ��Ϣ�������

BOOL CLoginDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // ���ر�������
    LoadConfig();

    // ������Դ
    LoadSkin();

    // ��ʼ�������ϵĿؼ�
    InitCtrl();

    // ��������
    InitText();

    // �ؼ�����
    MoveCtrl();

    // Ĭ�����뽹���������û�����
    m_cbUsername.SetFocus();

    // �������Զ���¼
    if (m_nUseAutoLogin && m_bAutoLogin)
    {
        PostMessage(WM_LOGINCENTER, 0, 0);
    }

    m_fingerMgr.Init();

    SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    return FALSE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

//
BOOL CLoginDlg::OnEraseBkgnd(CDC* pDC)
{
    CRect rcClient;
    GetClientRect(rcClient);
    FUi::CMemDCEX memDC(pDC, rcClient);

    RectF rcf((Gdiplus::REAL)rcClient.left, (Gdiplus::REAL)rcClient.top, 
        (Gdiplus::REAL)rcClient.Width(), (Gdiplus::REAL)rcClient.Height());
    Graphics graphics(memDC->GetSafeHdc());
    graphics.DrawImage(m_pImgBkg, rcf);

    CFont *pOld = memDC->SelectObject(RenderEngine->GetFont(YAHEI_26_SB)); // �и�25
    pDC->SelectObject(RenderEngine->GetFont(YAHEI_26_SB));

    CRect rcCal(25, 262, 375, 310);
    int nHeight = pDC->DrawText(m_strTitle, rcCal, DT_CENTER | DT_WORDBREAK | DT_CALCRECT);
    if (nHeight > 25)
    {
        // ��Ҫ������ʾ
        // ����һ���������17�������ַ�
        CRect rcTextTop(25, 262, 375, 288);
        CString strTopHalf = m_strTitle.Left(17);
        RenderEngine->DrawTextX(&memDC, strTopHalf, rcTextTop, FUi::White, DT_TOP | DT_CENTER);

        CRect rcTextBottom(25, 288, 375, 310);
        CString strBottomHalf = m_strTitle.Right(m_strTitle.GetLength() - 17);
        RenderEngine->DrawTextX(&memDC, strBottomHalf, rcTextBottom, FUi::White, DT_TOP | DT_CENTER | DT_END_ELLIPSIS);
    }
    else
    {
        CRect rcText(25, 262, 375, 290);
        RenderEngine->DrawTextX(&memDC, m_strTitle, rcText, FUi::White, DT_TOP | DT_CENTER);
    }
    memDC->SelectObject(pOld);
    pDC->SelectObject(pOld);

    return TRUE;
}

// ��Ӧ�������Ϣ��֧���ƶ�����
LRESULT CLoginDlg::OnNcHitTest(CPoint point)
{
    UINT uHitTest = CDialog::OnNcHitTest(point);

    if (HTCLIENT== uHitTest)
    {
        uHitTest = HTCAPTION;
    }

    return uHitTest;
}

// ���ر�������
void CLoginDlg::LoadConfig()
{
    CString strLoginFile = m_strAppPath+ LOGIN_FILE;
    std::string strLoginFileA = fcT2A(strLoginFile);

    TiXmlDocument doc;
    if (!doc.LoadFile(strLoginFileA))
    {
        ShowLogin(FALSE);
        //FMSCLIENT_ERROR("No Login File");
        return ;
    }
    TiXmlElement* pRoot = doc.RootElement();
    if (NULL == pRoot)
    {
        ShowLogin(FALSE);
        //FMSCLIENT_ERROR("Load Login file fail: no root element");
        return ;
    }

    // �ϴ�ʹ�õ��û�
    TiXmlElement* pLastNode = pRoot->FirstChildElement("last_user");
    if (pLastNode)
    {
        m_strLastUser = fcU2T(Decrypt(pLastNode->FirstChild()->Value()).c_str());   
    }

    // �Ƿ��ס����
    TiXmlElement* pRememberNode = pRoot->FirstChildElement("remember");
    if (pRememberNode)
    {
        m_bRemmberPwd = atoi(pRememberNode->FirstChild()->Value());
    }

    // �Ƿ��Զ���¼
    TiXmlElement* pAutologin = pRoot->FirstChildElement("auto_login");
    if (pAutologin)
    {
        m_bAutoLogin = atoi(pAutologin->FirstChild()->Value());
    }

    BOOL bConfig = TRUE;
    LOGIN_USER_INFO loginUserInfo;
    TiXmlElement* pServerConfig = pRoot->FirstChildElement("server_config");
    if (pServerConfig)
    {
        TiXmlElement* pChildNode = pServerConfig->FirstChildElement("server_ip");
        if (NULL != pChildNode->FirstChild())
        {
            loginUserInfo.strServerIP = fcU2T(pChildNode->FirstChild()->Value());
        }
        else
        {
            bConfig = FALSE;
        }

        pChildNode = pServerConfig->FirstChildElement("server_port");
        if (NULL != pChildNode->FirstChild())
        {
            loginUserInfo.nServerPort = atoi(pChildNode->FirstChild()->Value());
        }
        else
        {
            bConfig = FALSE;
        }

        pChildNode = pServerConfig->FirstChildElement("local_ip");
        if (NULL != pChildNode->FirstChild())
        {
            loginUserInfo.strLocalIP = fcU2T(pChildNode->FirstChild()->Value());
        }
        else
        {
            bConfig = FALSE;
        }
    }
    else
    {
        bConfig = FALSE;
    }

    // ��ȡָ����Ϣ
    GetFingerPrint(m_stuFpInfo);

    // δ���÷�����Ϣ���Զ�������������ҳ
    if (bConfig)
    {//������ָ�Ƶ�¼��¼
//         if (m_stuFpInfo.nUse)
//         {
//             ShowLogin(TRUE, LOGIN_SHOW_TYPE_ACCOUNT);
//         }
//         else
        {
            ShowLogin(TRUE, LOGIN_SHOW_TYPE_ACCOUNT_ONLY);
        }
    }
    else
    {
        ShowLogin(FALSE);
    }

    // �������ʷ��¼
    TiXmlElement* pUserNode = pRoot->FirstChildElement("user");
    while (pUserNode)
    {
        loginUserInfo.nIdx = atoi(pUserNode->Attribute("idx"));
        loginUserInfo.strUsername = fcU2T(Decrypt(pUserNode->Attribute("name")).c_str());

        loginUserInfo.strPassword = fcU2T(Decrypt(pUserNode->Attribute("password")).c_str());

        // ����
        m_mapLoginUser[loginUserInfo.strUsername] = loginUserInfo;

        // ��һ���ڵ�
        pUserNode = pUserNode->NextSiblingElement();
    }
}

// ��ʼ���ؼ�
void CLoginDlg::InitCtrl()
{
    // �������볤������
    m_cbUsername.LimitText(50);
    m_edPassword.LimitText(64);

    m_btnErrorTip.ShowWindow(SW_HIDE);
    m_btnErrorTip.SetTextAlign(DT_LEFT);

    // checkbox
    m_ckbRemember.SetCheck(m_bRemmberPwd);
    m_ckbAutologin.SetCheck(m_bAutoLogin);
    m_ckbRemember.SetFixedWidth(100);
    m_ckbRemember.SetTextMargin(5);
    m_ckbAutologin.SetFixedWidth(100);
    m_ckbAutologin.SetTextMargin(5);

    m_pgcLogin.SetRange(0, 300);
    m_pgcLogin.SetPos(0);
    m_pgcLogin.ShowWindow(SW_HIDE);

    // static
    m_stCopyright.SetTextFont(SONG_12_N);
    m_stCopyright.SetTextAlign(DT_VCENTER | DT_SINGLELINE | DT_CENTER | DT_END_ELLIPSIS);
    if (1 == m_nSkin)
    {
        m_stCopyright.SetTextColor(RGB(147, 197, 246));
    }
    else
    {
        m_stCopyright.SetTextColor(RGB(255, 255, 255));
    }

    m_stWelcome.SetTextFont(SONG_16_B);
    m_stWelcome.SetTextColor(FUi::Gray);

    m_stFinger.SetTextFont(YAHEI_20_B);
    m_stFinger.SetWindowText(LOGIN_TIP_FINGER);
    m_stFinger.SetTextAlign(DT_TOP | DT_WORDBREAK | DT_CENTER);
    m_stFinger.SetTextColor(FUi::LightGray);

    int nState = FUi::Normal | FUi::Hovered | FUi::Pressed | FUi::Disabled;

    // login btn
    m_btnLogin.SetTextFont(nState, SONG_16_B);
    m_btnLogin.SetTextColor(nState, FUi::White);
    m_btnLogin.SetTextAlign(DT_CENTER);

    m_btnSave.SetTextAlign(DT_CENTER);
    m_btnSave.SetWindowText(BTN_SAVE);
    m_btnSave.SetTextFont(nState, YAHEI_20_N);
    m_btnSave.SetTextColor(nState, FUi::White);

    m_btnCancel.SetTextAlign(DT_CENTER);
    m_btnCancel.SetWindowText(BTN_CANCEL);
    m_btnCancel.SetTextFont(nState, YAHEI_20_N);
    m_btnCancel.SetTextColor(nState, FUi::Black);


	m_btnLocalLogin.SetWindowText(_T("�������ߵ�¼"));
	m_btnLocalLogin.SetTextFont(FUi::Normal | FUi::Hovered | FUi::Pressed, YAHEI_18_N);
	m_btnLocalLogin.SetTextColor(FUi::Normal, RGB(0x3F, 0x85, 0xE4));
	m_btnLocalLogin.SetTextColor(FUi::Hovered | FUi::Pressed, RGB(0x2D, 0x5F, 0xA4));
	m_btnLocalLogin.SetTextAlign(DT_RIGHT);

    m_btnUserLogin.SetTextAlign(DT_CENTER);
    m_btnUserLogin.SetWindowText(LOGIN_BTN_TYPE_USER);
    m_btnUserLogin.SetTextFont(nState, YAHEI_20_N);
    m_btnUserLogin.SetTextColor(nState, FUi::Gray);
    m_btnUserLogin.SetTextColor(FUi::Hovered | FUi::Pressed, FUi::Black);

    m_btnFpLogin.SetTextAlign(DT_CENTER);
    m_btnFpLogin.SetWindowText(LOGIN_BTN_TYPE_FP);
    m_btnFpLogin.SetTextFont(nState, YAHEI_20_N);
    m_btnFpLogin.SetTextColor(nState, FUi::Gray);
    m_btnFpLogin.SetTextColor(FUi::Hovered | FUi::Pressed, FUi::Black);

    // input
	m_edPassword.SetPasswordMode(TRUE);
    m_edPassword.SetTextFont(nState, ARIAL_15_N);
    m_edPassword.SetTextColor(FUi::Normal, FUi::Black);
    m_edPassword.SetTextColor(FUi::Focus, RGB(184, 184, 184));

    m_cbUsername.SetTextFont(nState, ARIAL_15_N);
    m_cbUsername.SetTextColor(FUi::Normal, FUi::Black);
    m_cbUsername.SetTextColor(FUi::Focus, RGB(184, 184, 184));

    m_edServerIP.SetTextFont(nState, ARIAL_15_N);
    m_edServerIP.SetTextColor(FUi::Normal, FUi::Black);
    m_edServerIP.SetTextColor(FUi::Focus, RGB(184, 184, 184));

    m_edServerPort.SetTextFont(nState, ARIAL_15_N);
    m_edServerPort.SetTextColor(FUi::Normal, FUi::Black);
    m_edServerPort.SetTextColor(FUi::Focus, RGB(184, 184, 184));

    m_cbLocalIP.SetTextFont(nState, ARIAL_15_N);
    m_cbLocalIP.SetTextColor(FUi::Normal, FUi::Black);
    m_cbLocalIP.SetTextColor(FUi::Focus, RGB(184, 184, 184));
    m_cbLocalIP.SetReadOnly(TRUE);

    // error tip btn
    m_btnErrorTip.SetTextFont(nState, SONG_12_N);
    m_btnErrorTip.SetTextColor(nState, RGB(207, 104, 104));
    m_btnErrorTip.SetTextMargin(5);
}

// ���ݶ��������ý�������
void CLoginDlg::InitText(void)
{
    LoadText();

    SetWindowText(LOGIN_TITLE);
    m_stCopyright.SetWindowText(m_strCopyRight);

    // ������������
    m_cbUsername.SetDefaultText(LOGIN_USER);
    m_edPassword.SetDefaultText(LOGIN_PWD);
    m_ckbRemember.SetWindowText(NEW_LOGIN_REMEMBER_PASSWORD);
    m_ckbAutologin.SetWindowText(NEW_LOGIN_AUTOLOGIN);
    m_btnLogin.SetWindowText(LOGIN_BTN_LOGIN);
    m_edServerIP.SetDefaultText(NEW_LOGIN_SERVER_IP);
    m_edServerPort.SetDefaultText(NEW_LOGIN_SERVER_PORT);
    m_cbLocalIP.SetDefaultText(NEW_LOGIN_LOCAL_IP);
    m_edServerPort.SetWindowText(_T("8849"));

    // ��¼��ʷ
    for (MAP_LOGIN_USER::const_iterator citr = m_mapLoginUser.begin();
        citr != m_mapLoginUser.end(); ++citr)
    {
        m_cbUsername.AddString(citr->first);
    }
    if (m_cbUsername.GetCount() > 0)
    {
        m_cbUsername.AddString(NEW_LOGIN_CLEAR_HISTORY);
    }

    // ����IP��ַ
    std::vector<CString> vecLocalIP;
    GetLocalIP(vecLocalIP);
    for (size_t i = 0; i < vecLocalIP.size(); ++i)
    {
		if (vecLocalIP[i] != _T("127.0.0.1"))
		{
			m_cbLocalIP.AddString(vecLocalIP[i]);
		}
    }
    // ��ʾ��һ�εĵ�¼��¼
    ShowLoginInfo(m_strLastUser);
}

// �ƶ��ؼ�λ��
void CLoginDlg::MoveCtrl(void)
{
    CRect rcText(20, 324, 380, 344);
    m_stCopyright.MoveWindow(rcText);

    int nX = 438, nY = 50;
    m_stWelcome.SetWindowPos(NULL, nX, nY, 0, 0, SWP_NOSIZE |SWP_NOZORDER);

    m_btnUserLogin.SetWindowPos(NULL, nX, nY, 110, 30, SWP_NOZORDER);

    nX += 110;
    m_btnFpLogin.SetWindowPos(NULL, nX, nY, 110, 30, SWP_NOZORDER);
    nX -= 110;

    nY = 85;
    m_btnErrorTip.SetWindowPos(NULL, nX-2, nY, 0, 0, SWP_NOSIZE |SWP_NOZORDER);

    int nImgFingerH = 0;
    if (m_pImgFinger)
    {
        nImgFingerH = m_pImgFinger->GetHeight();
        int nPadding = (220 - nImgFingerH) / 2;
        m_rcFingerImg = CRect(nX + nPadding, nY + 30,
            nX + nPadding + nImgFingerH, nY + 30 + nImgFingerH);
    }

    m_stFinger.SetWindowPos(NULL, nX, nY + 30 + nImgFingerH + 30, 220, 30, SWP_NOZORDER);

    nY = 108;
    int nW = 220, nH = 34;
    m_cbUsername.MoveWindow(nX, nY, nW, nH);
    m_cbUsername.SetItemHeight(-1, 28); //< ʵ�ʿ������� +6 ��Ч��

    nY += 56;
    m_edPassword.MoveWindow(nX, nY, nW, nH);

    nY = 272;
    m_btnLogin.SetWindowPos(NULL, nX, nY, 0, 0, SWP_NOSIZE |SWP_NOZORDER);

	CRect rcLocalLogin;
	rcLocalLogin.left = 550;
	rcLocalLogin.top = 313;
	rcLocalLogin.right = 667;
	rcLocalLogin.bottom = rcLocalLogin.top + 30;
	m_btnLocalLogin.MoveWindow(rcLocalLogin);
// 	nY = 320;
// 	m_btnLocalLogin.SetWindowPos(NULL, nX, nY, 0, 0, SWP_NOSIZE |SWP_NOZORDER);

    nY = 215;
    m_ckbRemember.SetWindowPos(NULL, nX, nY, 0, 0, SWP_NOSIZE |SWP_NOZORDER);

    nX += 140;
    m_ckbAutologin.SetWindowPos(NULL, nX, nY, 0, 0, SWP_NOSIZE |SWP_NOZORDER);

    // btn
    CRect rcBtn;
    m_btnClose.GetClientRect(rcBtn);
    CRect rcClient;
    GetClientRect(rcClient);

    nX = rcClient.Width() - rcBtn.Width() - 10;
    nY = 10;
    m_btnClose.SetWindowPos(NULL, nX, nY, 0, 0, SWP_NOSIZE |SWP_NOZORDER);
    
    nX -= (rcBtn.Width() + 10);
    m_btnMin.SetWindowPos(NULL, nX, nY, 0, 0, SWP_NOSIZE |SWP_NOZORDER);

    nX -= (rcBtn.Width() + 10);
    m_btnConfig.SetWindowPos(NULL, nX, nY, 0, 0, SWP_NOSIZE |SWP_NOZORDER);

    nX = 438, nY = 108;
    // server ip
    m_edServerIP.MoveWindow(nX, nY, nW, nH);

    // server port
    nY += 48;
    m_edServerPort.MoveWindow(nX, nY, nW, nH);

    // local ip
    nY += 48;
    m_cbLocalIP.MoveWindow(nX, nY, nW, nH);
    m_cbLocalIP.SetItemHeight(-1, 28); //< ʵ�ʿ������� +6 ��Ч��

    nY = 272;
    m_btnSave.SetWindowPos(NULL, nX, nY, 0, 0, SWP_NOSIZE |SWP_NOZORDER);

    nX += 130;
    m_btnCancel.SetWindowPos(NULL, nX, nY, 0, 0, SWP_NOSIZE |SWP_NOZORDER);

    nX = 402;
    nY = 342;
    m_pgcLogin.SetWindowPos(NULL, nX, nY, 0, 0, SWP_NOSIZE |SWP_NOZORDER);
}

// ����Ƥ��
void CLoginDlg::LoadSkin(void)
{
    // Dialog
    SAFE_DEL_IMG(m_pImgBkg);
    m_pImgBkg = FUiGetRes(_T("login\\bkg"));
    MoveWindow(DEFAULT_WND_SIZE);

    // login button
    m_btnLogin.SetObjectName(_T("login\\login"));

    // btn close
    m_btnClose.SetObjectName(_T("login\\close"));

    // btn min
    m_btnMin.SetObjectName(_T("login\\min"));

    // ��������
    m_btnConfig.SetObjectName(_T("login\\config"));

    m_btnSave.SetObjectName(_T("login\\save"));

    m_btnCancel.SetObjectName(_T("login\\cancel"));

    m_btnUserLogin.SetObjectName(_T("login\\login_type"));

    m_btnFpLogin.SetObjectName(_T("login\\login_type"));

    // btn error
    m_btnErrorTip.SetObjectName(_T("login\\error_bkg"));
    m_btnErrorTip.SetIcon(_T("login\\error"));

    // btn remember
    m_ckbRemember.SetObjectName(_T("login\\ckb"));
    m_ckbRemember.SetCheckBox();

    // btn autologin
    m_ckbAutologin.SetObjectName(_T("login\\ckb"));
    m_ckbAutologin.SetCheckBox();

    // loading
    m_pgcLogin.SetObjectName(_T("login\\loading"));

    // combobox
    m_cbUsername.SetObjectName(_T("login\\combo"));
    m_cbUsername.SetItemHeight(-1, 34);

    // combobox
    m_cbLocalIP.SetObjectName(_T("login\\combo"));
    m_cbLocalIP.SetItemHeight(-1, 34);

    m_pImgFinger = FUiGetRes(_T("finger"));
}

// ��ʾ��¼���棬bShow:TRUE��¼���棬FALSE�������
// nType:0-�û�����¼��1-˫��¼(�û���)��2-˫��¼(ָ��)
void CLoginDlg::ShowLogin(BOOL bShow, int nType)
{
    m_btnErrorTip.ShowWindow(SW_HIDE);
    KillTimer(TIMER_FINGER_REFLASH);

    if (!bShow || nType != m_nLoginType)
    {
        FlashFingerImg(FALSE);
    }

    if (!bShow)
    {
        m_stWelcome.SetText(NEW_LOGIN_SERVER);
        m_stWelcome.ShowWindow(SW_SHOW);
        m_cbUsername.ShowWindow(SW_HIDE);
        m_edPassword.ShowWindow(SW_HIDE);
        m_ckbRemember.ShowWindow(SW_HIDE);
        m_ckbAutologin.ShowWindow(SW_HIDE);
        m_btnLogin.ShowWindow(SW_HIDE);
        m_btnFpLogin.ShowWindow(SW_HIDE);
        m_btnUserLogin.ShowWindow(SW_HIDE);
        m_stFinger.ShowWindow(SW_HIDE);
		m_btnLocalLogin.ShowWindow(SW_HIDE);

        m_edServerIP.ShowWindow(SW_SHOW);
        m_edServerPort.ShowWindow(SW_SHOW);
        m_cbLocalIP.ShowWindow(SW_SHOW);
        m_btnSave.ShowWindow(SW_SHOW);
        m_btnCancel.ShowWindow(SW_SHOW);
    }
    else
    {
        switch(nType)
        {
        case 0:
            {
                m_btnFpLogin.ShowWindow(SW_HIDE);
                m_btnUserLogin.ShowWindow(SW_HIDE);

                m_stFinger.ShowWindow(SW_HIDE);

                m_stWelcome.SetText(NEW_LOGIN_WELCOME);
                m_stWelcome.ShowWindow(SW_SHOW);
                m_cbUsername.ShowWindow(SW_SHOW);
                m_edPassword.ShowWindow(SW_SHOW);
                m_ckbRemember.ShowWindow(SW_SHOW);
                m_ckbAutologin.ShowWindow(SW_SHOW);
                m_btnLogin.ShowWindow(SW_SHOW);
				m_btnLocalLogin.ShowWindow(SW_SHOW);
            }
            break;
        case 1:
            {
                m_btnFpLogin.LockHover(FALSE);
                m_btnUserLogin.LockHover(TRUE);
                m_btnFpLogin.ShowWindow(SW_SHOW);
                m_btnUserLogin.ShowWindow(SW_SHOW);

                m_stFinger.ShowWindow(SW_HIDE);

                m_stWelcome.ShowWindow(SW_HIDE);
                m_cbUsername.ShowWindow(SW_SHOW);
                m_edPassword.ShowWindow(SW_SHOW);
                m_ckbRemember.ShowWindow(SW_SHOW);
                m_ckbAutologin.ShowWindow(SW_SHOW);
                m_btnLogin.ShowWindow(SW_SHOW);
				m_btnLocalLogin.ShowWindow(SW_SHOW);
            }
            break;
        case 2:
            {
                m_btnFpLogin.LockHover(TRUE);
                m_btnUserLogin.LockHover(FALSE);
                m_btnFpLogin.ShowWindow(SW_SHOW);
                m_btnUserLogin.ShowWindow(SW_SHOW);

                m_stFinger.ShowWindow(SW_SHOW);

                m_stWelcome.ShowWindow(SW_HIDE);
                m_cbUsername.ShowWindow(SW_HIDE);
                m_edPassword.ShowWindow(SW_HIDE);
                m_ckbRemember.ShowWindow(SW_HIDE);
                m_ckbAutologin.ShowWindow(SW_HIDE);
                m_btnLogin.ShowWindow(SW_HIDE);
				m_btnLocalLogin.ShowWindow(SW_HIDE);

                SetTimer(TIMER_FINGER_REFLASH, 150, NULL);
            }
            break;
        default:
            break;
        }

        m_edServerIP.ShowWindow(SW_HIDE);
        m_edServerPort.ShowWindow(SW_HIDE);
        m_cbLocalIP.ShowWindow(SW_HIDE);
        m_btnSave.ShowWindow(SW_HIDE);
        m_btnCancel.ShowWindow(SW_HIDE);

        m_nLoginType = nType;
    }

    m_bShowServer = !bShow;
}

// �л���������Ϣ�����Ƿ���ʾ
void CLoginDlg::OnBnClickedBtnConfig()
{
    if (m_bShowServer)
    {
        ShowLogin(TRUE, m_nLoginType);
    }
    else
    {
        LOGIN_USER_INFO serverInfo;
        if (LoadServerConfig(serverInfo))
        {
            OutputDebugString(_T("LoadServerConfig failed"));
        }

        m_edServerIP.SetWindowText(serverInfo.strServerIP);
        m_edServerPort.SetWindowText(My_itot(serverInfo.nServerPort));
        m_cbLocalIP.SetWindowText(serverInfo.strLocalIP);

        ShowLogin(FALSE);
    }
}
// ��С��
void CLoginDlg::OnBnClickedBtnMin()
{
    ShowWindow(SW_MINIMIZE);
}

// �ر�
void CLoginDlg::OnBnClickedBtnClose()
{
    OnCancel();
}

// ��ס����
void CLoginDlg::OnBnClickedCheckRemember()
{
    m_bRemmberPwd = m_ckbRemember.GetCheck();

    // ����ס���벻���Զ���¼
    if (!m_bRemmberPwd)
    {
        m_bAutoLogin = FALSE;
        m_ckbAutologin.SetCheck(FALSE);
    }
}

// �Զ���¼
void CLoginDlg::OnBnClickedCheckAutologin()
{
    m_bAutoLogin = m_ckbAutologin.GetCheck();

    // �Զ���¼�����ס����
    if (m_bAutoLogin)
    {
        m_bRemmberPwd = TRUE;
        m_ckbRemember.SetCheck(TRUE);
    }
}

//< ��¼ƽ̨
void CLoginDlg::OnBnClickedBtnLogin()
{
	UpdateData(TRUE);
    // ��ȡ������Ϣ
    m_cbUsername.GetWindowText(m_loginUserInfo.strUsername);
   // m_edPassword.GetWindowText(m_loginUserInfo.strPassword);
	m_edPassword.GetPasswordText(m_loginUserInfo.strPassword);
    m_edServerIP.GetWindowText(m_loginUserInfo.strServerIP);
    m_loginUserInfo.nServerPort = GetDlgItemInt(IDC_EDIT_SERVER_PORT);
    m_cbLocalIP.GetWindowText(m_loginUserInfo.strLocalIP);

    // У�������Ƿ�Ϸ�
    if (FALSE == CheckInputs(m_loginUserInfo))
    {
        return ;
    }

    // �����û�����Ŀո񣬷�ֹ��תWeb����
    int nLen = m_loginUserInfo.strUsername.GetLength();
    while (nLen - 1 == m_loginUserInfo.strUsername.ReverseFind(_T(' ')))
    {
        // �û���Ϊ�գ������д���
        if (0 == nLen)
        {
            break;
        }

        m_loginUserInfo.strUsername.Delete(nLen - 1);
        nLen = m_loginUserInfo.strUsername.GetLength();
    }

    if (m_hLoginThread)
    {
        CloseHandle(m_hLoginThread);
    }
    m_hLoginThread = (HANDLE)_beginthreadex(NULL, 0, WT_LoginCenter, this, 0, NULL);
    if (NULL == m_hLoginThread)
    {
        //FMSCLIENT_ERROR("Create Login Thread fail, error:%d", GetLastError());
        return ;
    }
}

// У�������Ƿ�Ϸ�
BOOL CLoginDlg::CheckInputs(LOGIN_USER_INFO& loginUserInfo)
{
    //����û���
    ENUM_CHECKPATH_LEAGL enumCheckUserName = StringLegality(loginUserInfo.strUsername, MAX_USERNAME_LEN);
    BOOL bSuc = TRUE;
    switch (enumCheckUserName)
    {
    case CHECKPATH_EMPTY:
        {
            ShowErrorMsg(LOGIN_ERROR1);
            bSuc = FALSE;
        }
        break;
    case CHECKPATH_ILLEAGLCHAR:
        {
            ShowErrorMsg(LOGIN_ERROR2);
            bSuc = FALSE;
        }
        break;
    case CHECKPATH_OVERLENGTH:
        {
            ShowErrorMsg(LOGIN_ERROR3);
            bSuc = FALSE;
        }
        break;
    case CHECKPATH_SUCCESS:
        break;
    default:
        break;
    }

    if (!bSuc)
    {
        m_cbUsername.SetFocus();
        m_cbUsername.SetState(TRUE);
        return FALSE;
    }
    else
    {
        m_cbUsername.SetFocus();
        m_cbUsername.SetState(FALSE);
    }

    //�������
    ENUM_CHECKPATH_LEAGL enumCheckPassword = StringLegality(loginUserInfo.strPassword, MAX_PASSWORD_LEN);
    switch (enumCheckPassword)
    {
    case CHECKPATH_EMPTY:
        {
            ShowErrorMsg(LOGIN_ERROR4);
            bSuc = FALSE;
        }
        break;
    case CHECKPATH_OVERLENGTH:
        {
            ShowErrorMsg(LOGIN_ERROR5);
            bSuc = FALSE;
        }
        break;
    case CHECKPATH_SUCCESS:
        break;
    default:
        break;
    }

    if (!bSuc)
    {
        m_edPassword.SetFocus();
        m_edPassword.SetErrorState(TRUE);
        return FALSE;
    }

    //������IP
    std::string strServerIP = fcT2A(loginUserInfo.strServerIP.GetBuffer());
    if (!ip_valid(strServerIP.c_str()))
    {
        ShowErrorMsg(LOGIN_ERROR6);
        if (!m_bShowServer)
        {
            OnBnClickedBtnConfig();
        }
        m_edServerIP.SetFocus();
        m_edServerIP.SetErrorState(TRUE);
        return FALSE;
    }

    //������˿�
    if (loginUserInfo.nServerPort < 1000 || loginUserInfo.nServerPort > 65535)
    {
        ShowErrorMsg(LOGIN_ERROR7);
        if (!m_bShowServer)
        {
            OnBnClickedBtnConfig();
        }
        m_edServerPort.SetFocus();
        m_edServerPort.SetErrorState(TRUE);
        return FALSE;
    }

    //��鱾��IP
    std::string strLocalIP = fcT2A(loginUserInfo.strLocalIP.GetBuffer());
    if (!ip_valid(strLocalIP.c_str()))
    {
        ShowErrorMsg(LOGIN_ERROR6);
        if (!m_bShowServer)
        {
            OnBnClickedBtnConfig();
        }
        m_cbLocalIP.SetFocus();
        m_cbLocalIP.SetState(TRUE);
        return FALSE;
    }

    return TRUE;
}

//��¼��������
BOOL CLoginDlg::LoginToServer(LOGIN_USER_INFO& stLoginInfo)
{
    if (NULL == m_pLoginEvent)
    {
        return FALSE;
    }

    LIB_LOGIN_USER_INFO loginUserInfo;
    loginUserInfo.nLoginType = LOGIN_TYPE_ACCOUNT;
    LoginUserInfoTrans(loginUserInfo, stLoginInfo);

    BOOL nRet = m_pLoginEvent(loginUserInfo, m_pUser);
    if (!nRet)
    {
        ShowErrorMsg(loginUserInfo.szErrorMsg);
    }

    return nRet;
}

void CLoginDlg::SaveLoginInfo()
{
//lint -e423
    TiXmlDocument *pDoc = new TiXmlDocument;
    TiXmlDeclaration *pDeclaration = new TiXmlDeclaration("1.0","UTF-8","");
    pDoc->LinkEndChild(pDeclaration);

    // ���ڵ�
    TiXmlElement *pRootNode = new TiXmlElement("root");
    pDoc->LinkEndChild(pRootNode);
    // ����¼���û�
    TiXmlElement *pLastNode = new TiXmlElement("last_user");
    pRootNode->LinkEndChild(pLastNode);
    
    TiXmlText *pNodeValue = new TiXmlText(Encrypt(fcT2U(m_strLastUser.GetBuffer())));
    pLastNode->LinkEndChild(pNodeValue);

    // �Ƿ��ס����
    TiXmlElement *pRememberNode = new TiXmlElement("remember");
    pRootNode->LinkEndChild(pRememberNode);

    pNodeValue = new TiXmlText(My_itoa(m_bRemmberPwd?1:0));
    pRememberNode->LinkEndChild(pNodeValue);

    // �Ƿ��Զ���¼
    TiXmlElement *pAutologinNode = new TiXmlElement("auto_login");
    pRootNode->LinkEndChild(pAutologinNode);

    pNodeValue = new TiXmlText(My_itoa(m_bAutoLogin?1:0));
    pAutologinNode->LinkEndChild(pNodeValue);

    // ������Ϣ
    TiXmlElement *pServerConfig= new TiXmlElement("server_config");
    pRootNode->LinkEndChild(pServerConfig);
    {
        // ������IP
        TiXmlElement *pServerNode = new TiXmlElement("server_ip");
        pServerConfig->LinkEndChild(pServerNode);

        CString strTmp;
        m_edServerIP.GetWindowText(strTmp);
        pNodeValue = new TiXmlText(fcT2U(strTmp));
        pServerNode->LinkEndChild(pNodeValue);

        // ����˿�
        TiXmlElement *pServerPortNode = new TiXmlElement("server_port");
        pServerConfig->LinkEndChild(pServerPortNode);

        pNodeValue = new TiXmlText(My_itoa(GetDlgItemInt(IDC_EDIT_SERVER_PORT)));
        pServerPortNode->LinkEndChild(pNodeValue);

        // ����IP
        TiXmlElement *pLocalIPNode = new TiXmlElement("local_ip");
        pServerConfig->LinkEndChild(pLocalIPNode);

        m_cbLocalIP.GetWindowText(strTmp);
        pNodeValue = new TiXmlText(fcT2U(strTmp));
        pLocalIPNode->LinkEndChild(pNodeValue);
    }

    // ָ�Ƶ�¼
    TiXmlElement *pFingerPrint= new TiXmlElement("fingerprint_login");
    pRootNode->LinkEndChild(pFingerPrint);

    STU_FP_INFO stuFpInfo;
    if (GetFingerPrint(stuFpInfo))
    {
        // ԭ������ָ���豸��Ϣ
        pFingerPrint->SetAttribute("use", stuFpInfo.nUse);

        // IP
        TiXmlElement *pIPNode = new TiXmlElement("ip");
        pFingerPrint->LinkEndChild(pIPNode);

        pNodeValue = new TiXmlText(stuFpInfo.strIp);
        pIPNode->LinkEndChild(pNodeValue);

        // �˿�
        TiXmlElement *pPortNode = new TiXmlElement("port");
        pFingerPrint->LinkEndChild(pPortNode);

        pNodeValue = new TiXmlText(stuFpInfo.strPort);
        pPortNode->LinkEndChild(pNodeValue);

        // �û���
        TiXmlElement *pUserNode = new TiXmlElement("user");
        pFingerPrint->LinkEndChild(pUserNode);

        pNodeValue = new TiXmlText(Encrypt(stuFpInfo.strUser.c_str()));
        pUserNode->LinkEndChild(pNodeValue);

        // ����
        TiXmlElement *pPwdNode = new TiXmlElement("password");
        pFingerPrint->LinkEndChild(pPwdNode);

        pNodeValue = new TiXmlText(Encrypt(stuFpInfo.strPwd.c_str()));
        pPwdNode->LinkEndChild(pNodeValue);
    }
    else
    {
        pFingerPrint->SetAttribute("use", 0);
    }

    // ����û�
    std::set<CString> stUser;    // ��Ҫ������û����û�����ͨ������ɾ����һЩ���ñ���ġ�
    for (int i = 0; i < m_cbUsername.GetCount(); ++i)
    {
        CString strUser;
        m_cbUsername.GetLBText(i, strUser);

        stUser.insert(strUser.GetBuffer());
    }
//lint -e423
//lint -e429
    int nIdx = 0;
    for (MAP_LOGIN_USER::iterator itr = m_mapLoginUser.begin();
        itr != m_mapLoginUser.end(); ++itr)
    {
        if (stUser.find(itr->first) != stUser.end())       // ��Ҫ���浽����
        {
            TiXmlElement *pUserNode = new TiXmlElement("user");
            pRootNode->LinkEndChild(pUserNode);

            pUserNode->SetAttribute("idx", nIdx++);
			pUserNode->SetAttribute("name", Encrypt(fcT2U(itr->second.strUsername.GetBuffer())));

            // ��ǰ��¼�û�����ѡ��ס����ʱ�ż�ס����
            if (0 == itr->second.strUsername.Compare(m_loginUserInfo.strUsername))
            {
                if (m_bRemmberPwd)
                {
                    pUserNode->SetAttribute("password", Encrypt(fcT2U(itr->second.strPassword.GetBuffer())));
                }
                else
                {
                    pUserNode->SetAttribute("password", fcT2U(_T("")));
                }
            }
            else
            {
                pUserNode->SetAttribute("password", Encrypt(fcT2U(itr->second.strPassword.GetBuffer())));
            }
        }
    }

    TiXmlPrinter printer;
    printer.SetIndent( "    " );
    pDoc->Accept( &printer );
    std::string xmltext = printer.CStr();

    // ���浽�ļ�
    CString strLoginFile = m_strAppPath + LOGIN_FILE;
    if (!pDoc->SaveFile(fcT2A(strLoginFile.GetBuffer())))
    {
        OutputDebugString(_T("Login Save Xml fail\n"));
    }
    delete pDoc;
}

// ���̰��»س���Ӧ��¼��Ϣ
BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
    switch (pMsg->message)
    {
    case WM_KEYDOWN:
        {
            if (VK_RETURN == pMsg->wParam)
            {
                // �������ý���ʱ
                if (m_bShowServer)
                {
                    OnBnClickedButtonSave();
                    return TRUE;
                }

                if ((LOGIN_SHOW_TYPE_ACCOUNT == m_nLoginType ||
                    LOGIN_SHOW_TYPE_ACCOUNT_ONLY == m_nLoginType))
                {
                    OnBnClickedBtnLogin();
                }
                return TRUE;
            }
        }
        break;
    case WM_SYSKEYDOWN:
        {
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

// ѡ�������û�ʱ���л������ؼ��ϵ���ʾ
void CLoginDlg::OnCbnSelchangeComboUsername()
{
	int nIndex = m_cbUsername.GetCurSel();
	if (nIndex < 0)
	{
		return;
	}
    CString strSelUser;
    m_cbUsername.GetLBText(m_cbUsername.GetCurSel(), strSelUser);
    if (strSelUser == NEW_LOGIN_CLEAR_HISTORY)
    {
        if (!ClearLoginHistory())
        {
            ShowLoginInfo(m_strLastUser);
        }
    }

    MAP_LOGIN_USER::const_iterator citr = m_mapLoginUser.find(strSelUser);
    if (citr != m_mapLoginUser.end())
    {
        if (m_bRemmberPwd)
        {
            m_edPassword.SetWindowText(citr->second.strPassword);
        }
        m_cbUsername.SetWindowText(citr->second.strUsername);
    }

	m_bPassWordReset = FALSE;
}

void CLoginDlg::OnCbnSelchangeComboLocalip()
{
	int nIndex = m_cbLocalIP.GetCurSel();
	if (nIndex < 0)
	{
		return;
	}
    CString strLocalIP;
    m_cbLocalIP.GetLBText(m_cbLocalIP.GetCurSel(), strLocalIP);

    m_cbLocalIP.SetWindowText(strLocalIP);
}


LRESULT CLoginDlg::OnMsgLoginCenter(WPARAM, LPARAM)
{
    OnBnClickedBtnLogin();
    return 0;
}

void CLoginDlg::ShowErrorMsg(const CString& strMsg)
{
    m_btnErrorTip.SetWindowText(strMsg);
    m_btnErrorTip.SetTipText(strMsg);
    m_btnErrorTip.ShowWindow(SW_SHOW);
}

void CLoginDlg::DisableCtrl(BOOL bDisable)
{/*
    m_cbUsername.EnableWindow(bDisable);
    m_edPassword.EnableWindow(bDisable);
    m_edServerIP.EnableWindow(bDisable);
    m_edServerPort.EnableWindow(bDisable);
    m_edLocalIP.EnableWindow(bDisable);*/

    m_ckbRemember.EnableWindow(bDisable);
    m_ckbAutologin.EnableWindow(bDisable);
    m_btnLogin.EnableWindow(bDisable);
	m_btnLocalLogin.EnableWindow(bDisable);

    m_btnFpLogin.EnableWindow(bDisable);
    m_btnUserLogin.EnableWindow(bDisable);
    m_btnConfig.EnableWindow(bDisable);
}

void CLoginDlg::LoginCenter()
{
    SendMessage(WM_PRELOGIN, 0, 0);
    // ��¼����
    BOOL nRet = LoginToServer(m_loginUserInfo);

	m_bPassWordReset = FALSE;

    PostMessage(WM_AFTLOGIN, nRet, 0);
}

void CLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (TIMER_LOGIN_PROGRESS == nIDEvent)
    {
        if (!m_pgcLogin.IsWindowVisible())
        {
            m_pgcLogin.ShowWindow(SW_SHOW);
        }

        // set progress
        m_pgcLogin.StepIt();
    }
    else if(TIMER_FINGER_REFLASH == nIDEvent)
    {
        FlashFingerImg(TRUE);
    }

    CDialog::OnTimer(nIDEvent);
}

void CLoginDlg::OnCancel()
{
    if (!m_btnLogin.IsWindowEnabled())
    {
        //MsgBoxApi_Show(APP_ERROR_LOGINING, MSGBOX_TYPE_WARNING);
        return ;
    }
    if (m_hLoginThread)
    {
        WaitForSingleObject(m_hLoginThread, INFINITE);
        CloseHandle(m_hLoginThread);
    }

    // �ж��Ƿ��Զ���¼
    if (GetAutoLogin())
    {
        ::WriteProfileString(_T("reboot"), _T("switch"), _T("1"));
    }
    else
    {
        ::WriteProfileString(_T("reboot"), _T("switch"), _T("0"));
    }

    CDialog::OnCancel();
}

BOOL CLoginDlg::ClearLoginHistory()
{
	if (IDYES != FMessageBox::Show(NEW_LOGIN_CLEAR_CONFIRM, MSGBOX_TIPS, MB_YESNO | MB_ICONWARNING))
    {
        return FALSE;
    }
    // ���汾���ļ�
    m_mapLoginUser.clear();
    SaveLoginInfo();
    // ����ؼ�����
    m_cbUsername.ClearText();
    m_edPassword.SetWindowText(_T(""));
	//�����¼��ʷ���û��������룩�����������������Ϣ
//     m_edServerIP.SetWindowText(_T(""));
//     m_edServerPort.SetWindowText(_T(""));
//     m_cbLocalIP.ClearText();

    return TRUE;
}

void CLoginDlg::ShowLoginInfo(const CString& /*strUsername*/)
{
    MAP_LOGIN_USER::const_iterator citr = m_mapLoginUser.find(m_strLastUser);
    if (citr != m_mapLoginUser.end())
    {
        if (citr->second.nIdx < m_cbUsername.GetCount() - 1)
        {
            m_cbUsername.SetCurSel(citr->second.nIdx);
            OnCbnSelchangeComboUsername();
        }
        else
        {
            // ������ֵ����Combox�ؼ����û�����˵�������ļ�����
            // ��յ�¼��Ϣ
            m_mapLoginUser.clear();
            m_cbUsername.ClearText();
            SaveLoginInfo();
        }
    }

    LOGIN_USER_INFO serverInfo;
    LoadServerConfig(serverInfo);

    m_edServerIP.SetWindowText(serverInfo.strServerIP);
    m_edServerPort.SetWindowText(My_itot(serverInfo.nServerPort));
    m_cbLocalIP.SetWindowText(serverInfo.strLocalIP);
}

void CLoginDlg::LoadText()
{
    TCHAR szPath[MAX_PATH] = {0};
    PathCombine(szPath, (LPCTSTR)m_strAppPath, _T("FMSClient.ini"));
    TCHAR szTemp[MAX_PATH] = {0};
    GetPrivateProfileString(_T("FMSClient"), _T("CustomTitle"), _T("��Ʒ��չ���ϵͳ"), szTemp, 120, szPath);
    m_strTitle = szTemp;
    m_strSubTitle = szTemp;
    GetPrivateProfileString(_T("FMSClient"), _T("Copyright"), _T(""), szTemp, 120, szPath);
    m_strCopyRight = szTemp;
}

void CLoginDlg::LoginUserInfoTrans(LIB_LOGIN_USER_INFO& loginUserInfo, LOGIN_USER_INFO& stLoginUserInfo)
{
    _tcscpy_s(loginUserInfo.szUserName, MAX_USERNAME_LEN, (LPCTSTR)stLoginUserInfo.strUsername);
    _tcscpy_s(loginUserInfo.szPassword, MAX_PASSWORD_LEN, (LPCTSTR)stLoginUserInfo.strPassword);
    _tcscpy_s(loginUserInfo.szServerIP, MAX_IP_LEN, (LPCTSTR)stLoginUserInfo.strServerIP);
    _tcscpy_s(loginUserInfo.szLocalIP, MAX_IP_LEN, (LPCTSTR)stLoginUserInfo.strLocalIP);
    loginUserInfo.nServerPort = stLoginUserInfo.nServerPort;
}

std::string CLoginDlg::Encrypt(const char *szString)
{
	char szTemp0[MAX_PASSWORD_LEN + 1] = {0};						// ԭʼ���뻺����
	strncpy_s(szTemp0, MAX_PASSWORD_LEN  + 1, szString, _TRUNCATE);
	char szAesTemp[1024] = {0};						// �������뻺����
	int nAecLen = 1024;

	Hak5(szAesTemp, szTemp0, (char*)&nAecLen);

	return std::string(szAesTemp);
}

std::string CLoginDlg::Decrypt(const char *szString)
{
	char szTemp0[MAX_PASSWORD_LEN + 1] = {0};						// ԭʼ���뻺����
	char szAesTemp[1024] = {0};						// �������뻺����
	strcpy_s(szAesTemp, MAX_PASSWORD_LEN  + 1, szString);
	int nAecLen = 1024;

	Hak7(szTemp0, szAesTemp, (char*)&nAecLen);

	return std::string(szTemp0);
}

void CLoginDlg::GetAppPath()
{
    TCHAR szFileName[MAX_PATH] = {0};
    if (0 == GetModuleFileName(NULL, szFileName, MAX_PATH - 1))
    {
        DebugMsg(_T("GetModuleFileName fail, error:%d\n"), GetLastError());
        return ;
    }
    m_strAppPath = szFileName;

    szFileName[m_strAppPath.ReverseFind(_T('\\')) + 1] = _T('\0');
    m_strAppPath = szFileName;
}

LRESULT CLoginDlg::OnMsgPreLogin(WPARAM, LPARAM)
{
    // �����˳�ʱ��������Timer�Ƿ���ڣ���Ϊ����Ҫ�˳���
    // ���Timerֻ����¼������ʾ��������������
    m_pgcLogin.SetPos(0);
    SetTimer(TIMER_LOGIN_PROGRESS, 100, NULL);
    DisableCtrl(FALSE);

    return 0;
}

LRESULT CLoginDlg::OnMsgAftLogin(WPARAM wParam, LPARAM)
{
    DisableCtrl(TRUE);
    KillTimer(TIMER_LOGIN_PROGRESS);
    m_pgcLogin.ShowWindow(SW_HIDE);

    BOOL bLoginRet = (BOOL)wParam;
    if (!bLoginRet)
    {
        return 2;
    }

    m_strLastUser = m_loginUserInfo.strUsername;
    m_cbUsername.AddString(m_strLastUser);  // ����ؼ��У�����ʱ���й���
    m_mapLoginUser[m_strLastUser] = m_loginUserInfo;

    // ָ�Ƶ�¼��������Ϣ
    if (LOGIN_SHOW_TYPE_FINGER != m_nLoginType)
    {
        // ��¼��Ϣ����
        SaveLoginInfo();
    }

    OnOK();

    return 0;
}

void CLoginDlg::OnCbnEditupdateComboUsername()
{
	if (m_bPassWordReset != TRUE)
	{
		m_edPassword.SetWindowText(_T(""));
		m_bPassWordReset = TRUE;
	}
}

void CLoginDlg::OnEnSetfocusEditPassword()
{
	m_edPassword.SetWindowText(_T(""));
	m_bPassWordReset = TRUE;
}

BOOL CLoginDlg::GetFingerPrint(STU_FP_INFO& stuFpInfo)
{
    CString strPath = m_strAppPath + LOGIN_FILE;
    std::string strPathA = fcT2A(strPath.GetBuffer());

    TiXmlDocument doc;
    if (!doc.LoadFile(strPathA))
    {
        return FALSE;
    }

    TiXmlElement *pRootNode = doc.FirstChildElement();	// root
    if (!pRootNode)
    {
        return FALSE;
    }

    TiXmlElement* pFingerPrint = pRootNode->FirstChildElement("fingerprint_login");
    if (pFingerPrint)
    {
        stuFpInfo.nUse = atoi(pFingerPrint->Attribute("use"));

        TiXmlElement* pChildNode = pFingerPrint->FirstChildElement("ip");
        if (pChildNode && NULL != pChildNode->FirstChild())
        {
            stuFpInfo.strIp = pChildNode->GetText();
        }

        pChildNode = pFingerPrint->FirstChildElement("port");
        if (pChildNode && NULL != pChildNode->FirstChild())
        {
            stuFpInfo.strPort = pChildNode->GetText();
        }

        pChildNode = pFingerPrint->FirstChildElement("user");
        if (pChildNode && NULL != pChildNode->FirstChild())
        {
            stuFpInfo.strUser = Decrypt(pChildNode->GetText());
        }

        pChildNode = pFingerPrint->FirstChildElement("password");
        if (pChildNode && NULL != pChildNode->FirstChild())
        {
            stuFpInfo.strPwd = Decrypt(pChildNode->GetText());
        }
    }

    return TRUE;
}


void CLoginDlg::OnBnClickedButtonSave()
{
    LOGIN_USER_INFO loginUserInfo;
    m_edServerIP.GetWindowText(loginUserInfo.strServerIP);
    loginUserInfo.nServerPort = GetDlgItemInt(IDC_EDIT_SERVER_PORT);
    m_cbLocalIP.GetWindowText(loginUserInfo.strLocalIP);

    if (!ip_valid(fcT2A(loginUserInfo.strServerIP)))
    {
        ShowErrorMsg(LOGIN_ERROR6);
        m_edServerIP.SetFocus();
        m_edServerIP.SetErrorState(TRUE);
        return ;
    }
    else
    {
        m_edServerIP.SetErrorState(FALSE);
    }

    if (loginUserInfo.nServerPort < 1000 || loginUserInfo.nServerPort > 65535)
    {
        ShowErrorMsg(LOGIN_ERROR7);
        m_edServerPort.SetFocus();
        m_edServerPort.SetErrorState(TRUE);
        return ;
    }
    else
    {
        m_edServerPort.SetErrorState(FALSE);
    }

    if (!ip_valid(fcT2A(loginUserInfo.strLocalIP)))
    {
        ShowErrorMsg(LOGIN_ERROR6);
        m_cbLocalIP.SetFocus();
        m_cbLocalIP.SetState(TRUE);
        return ;
    }
    else
    {
        m_cbLocalIP.SetState(FALSE);
    }

    SaveLoginInfo();

	//��������ϴ��ͻ���
	StartUploadClient();

    m_btnErrorTip.ShowWindow(SW_HIDE);
    ShowLogin(TRUE, m_nLoginType);
}

void CLoginDlg::OnBnClickedButtonCancel()
{
    LOGIN_USER_INFO loginUserInfo;
    m_edServerIP.GetWindowText(loginUserInfo.strServerIP);
    loginUserInfo.nServerPort = GetDlgItemInt(IDC_EDIT_SERVER_PORT);
    m_cbLocalIP.GetWindowText(loginUserInfo.strLocalIP);

    if (!ServerConfigCheck(loginUserInfo))
    {
        if (IDYES == FMessageBox::Show(SERVER_INFO_CHANGE, MSGBOX_TIPS, MB_YESNO | MB_ICONWARNING))
        {
            ShowLogin(TRUE, m_nLoginType);
        }
    }
    else
    {
        ShowLogin(TRUE, m_nLoginType);
    }
}

BOOL CLoginDlg::ServerConfigCheck(const LOGIN_USER_INFO& logininfo)
{
    LOGIN_USER_INFO logininfoExist;
    LoadServerConfig(logininfoExist);

    if (0 != logininfoExist.strServerIP.Compare(logininfo.strServerIP))
    {
        return FALSE;
    }

    if (logininfoExist.nServerPort != logininfo.nServerPort)
    {
        return FALSE;
    }

    if (0 != logininfoExist.strLocalIP.Compare(logininfo.strLocalIP))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CLoginDlg::LoadServerConfig(LOGIN_USER_INFO& logininfo)
{
    CString strLoginFile = m_strAppPath+ LOGIN_FILE;
    std::string strLoginFileA = fcT2A(strLoginFile);
    TiXmlDocument doc;
    if (!doc.LoadFile(strLoginFileA))
    {
        return FALSE;
    }
    TiXmlElement* pRoot = doc.RootElement();
    if (NULL == pRoot)
    {
        return FALSE;
    }

    TiXmlElement* pServerConfig = pRoot->FirstChildElement("server_config");
    if (pServerConfig)
    {
        TiXmlElement* pChildNode = pServerConfig->FirstChildElement("server_ip");
        if (NULL != pChildNode->FirstChild())
        {
            logininfo.strServerIP = fcU2T(pChildNode->FirstChild()->Value());
        }

        pChildNode = pServerConfig->FirstChildElement("server_port");
        if (NULL != pChildNode->FirstChild())
        {
            logininfo.nServerPort = atoi(pChildNode->FirstChild()->Value());
        }

        pChildNode = pServerConfig->FirstChildElement("local_ip");
        if (NULL != pChildNode->FirstChild())
        {
            logininfo.strLocalIP = fcU2T(pChildNode->FirstChild()->Value());
        }
    }

    return TRUE;
}

void CLoginDlg::OnBnClickedButtonUserlogin()
{
    ShowLogin(TRUE, LOGIN_SHOW_TYPE_ACCOUNT);
}

void CLoginDlg::OnBnClickedButtonFplogin()
{
    ShowLogin(TRUE, LOGIN_SHOW_TYPE_FINGER);

    if (!m_fingerMgr.LoginFinger(m_stuFpInfo, FingerLoginCallBack, this))
    {
        return;
    }

    //if (-1 == m_lFingerAlarm && -1 != m_lFingerLogin)
    //{
    //    SetFingerAlarm(TRUE);
    //}
}

LRESULT CLoginDlg::OnMsgFpLoginResult(WPARAM wParam, LPARAM)
{
    if (LOGIN_SHOW_TYPE_FINGER != m_nLoginType  || m_bShowServer)
    {
        return FALSE;
    }

    switch ((int)wParam)
    {
    case 0: // ��¼�ɹ�
        {
            CString strError;
            if (!m_fingerMgr.StartAlarm(strError, FingerAlarmCallBack, this))
            {
                ShowErrorMsg(strError);
            }
        }
        break;
    case 1: // ��¼ʧ��
        {
            ShowErrorMsg(LOGIN_ERROR_FP_LOGIN_FAILED);
        }
        break;
    case 2: // ��¼�豸��֧��
        {
            ShowErrorMsg(LOGIN_ERROR_FP_DEVICE_ERROR);
        }
        break;
    default:
        break;
    }

    return TRUE;
}

void CLoginDlg::FingerLogin(char* strCardNo)
{
    if (NULL == strCardNo)
    {
        return;
    }

    if (LOGIN_SHOW_TYPE_FINGER == m_nLoginType &&
        0 == strlen(m_strCardNo) && 0 != strlen(strCardNo))
    {
        strcpy_s(m_strCardNo, ACS_CARD_NO_LEN, strCardNo);
        PostMessage(WM_FINGER_LOGIN, NULL, NULL);
    }
    else
    {
        memset(m_strCardNo, 0, ACS_CARD_NO_LEN);
    }
}

LRESULT CLoginDlg::OnMsgFingerLogin(WPARAM, LPARAM)
{
    SendMessage(WM_PRELOGIN, 0, 0);

    LOGIN_USER_INFO loginUserInfo;
    m_edServerIP.GetWindowText(loginUserInfo.strServerIP);
    loginUserInfo.nServerPort = GetDlgItemInt(IDC_EDIT_SERVER_PORT);
    m_cbLocalIP.GetWindowText(loginUserInfo.strLocalIP);

    LIB_LOGIN_USER_INFO libLoginUserInfo;
    libLoginUserInfo.nLoginType = LOGIN_TYPE_FINGER;
    LoginUserInfoTrans(libLoginUserInfo, loginUserInfo);
    _tcscpy_s(libLoginUserInfo.szCardNo, ACS_CARD_NO_LEN, fcA2T(m_strCardNo));

    BOOL nRet = m_pLoginEvent(libLoginUserInfo, m_pUser);
    if (!nRet)
    {
        ShowErrorMsg(libLoginUserInfo.szErrorMsg);
    }

    PostMessage(WM_AFTLOGIN, nRet, 0);
    return TRUE;
}

void CLoginDlg::OnDestroy()
{
    CDialog::OnDestroy();

    m_fingerMgr.UnInit();
}

void CLoginDlg::FlashFingerImg(BOOL bDrawImg)
{
    if (!m_pImgFinger)
    {
        return;
    }

    CDC* pDC = GetDC();

    --m_nFingerImgId;
    if (m_nFingerImgId < 0)
    {
        m_nFingerImgId = 11;
    }

    RectF rcfDst((Gdiplus::REAL)m_rcFingerImg.left, (Gdiplus::REAL)m_rcFingerImg.top,
        (Gdiplus::REAL)m_rcFingerImg.Width(), (Gdiplus::REAL)m_rcFingerImg.Height());

    if (!bDrawImg)
    {
        // ���ָ��ͼƬ
		if (0 == m_nSkin)
		{
			RenderEngine->DrawColor(pDC, m_rcFingerImg, RGB(248, 248, 248));
		}
		else
		{
			RenderEngine->DrawColor(pDC, m_rcFingerImg, RGB(250, 252, 254));
		}
    }
    else
    {
        // ˢ����ָ��ͼƬ
        RenderEngine->DrawImageX(pDC, m_pImgFinger, rcfDst, m_nFingerImgId * (Gdiplus::REAL)m_rcFingerImg.Width(), 0,
            (Gdiplus::REAL)m_rcFingerImg.Width(), (Gdiplus::REAL)m_rcFingerImg.Height());
    }

	ReleaseDC(pDC);
}

BOOL CLoginDlg::GetAutoLogin()
{
    CString strPath = m_strAppPath + LOGIN_FILE;
    std::string strPathA = fcT2A(strPath.GetBuffer());

    TiXmlDocument doc;
    if (!doc.LoadFile(strPathA))
    {
        return FALSE;
    }

    TiXmlElement *pRootNode = doc.FirstChildElement();	// root
    if (!pRootNode)
    {
        return FALSE;
    }

    TiXmlElement* pAutoLogin = pRootNode->FirstChildElement("auto_login");
    BOOL bAutoLogin = (BOOL)atoi(pAutoLogin->GetText());

    return bAutoLogin;
}

void CLoginDlg::StartUploadClient()
{
	SaveUploadClientConfig();

	CString rootPath;
	rootPath.Preallocate(128);
	DWORD nLength;
	if (0 != (nLength = GetModuleFileName(NULL, rootPath.GetBuffer(0), 128 - 1)))
	{
		rootPath.ReleaseBuffer(nLength);
		int idx = rootPath.ReverseFind('\\');
		rootPath = rootPath.Left(idx + 1);
	}

	CString runDir;
	// ����
	//runDir.Format(_T("%srestart.vbs"), rootPath);
    runDir.Format(_T("%sUploadClient\\restart.bat"), rootPath);
	FMSCLIENT_INFO("UploadClient Path:%s", fcT2A(runDir));
	RunCmd(runDir, runDir);
}

void CLoginDlg::SaveUploadClientConfig()
{
	CString csDCIP,csDCPort,csLocalIP;
	m_edServerIP.GetWindowText(csDCIP);
	m_edServerPort.GetWindowText(csDCPort);
	m_cbLocalIP.GetWindowText(csLocalIP);

	//��������
	TiXmlDocument *pDoc = new TiXmlDocument;
	TiXmlDeclaration *pDeclaration = new TiXmlDeclaration("1.0","UTF-8","");  
	pDoc->LinkEndChild(pDeclaration);

	TiXmlElement *pRootNode = new TiXmlElement(NODE_ROOT);  
	pDoc->LinkEndChild(pRootNode);  

	//���ز���
	do 
	{
		TiXmlElement *pLocalNode = new TiXmlElement(NODE_LOCAL_PARAM);  
		pRootNode->LinkEndChild(pLocalNode);  

		std::_tstring sLocalIP(csLocalIP.GetBuffer());
		csLocalIP.ReleaseBuffer();
		pLocalNode->SetAttribute("ip", CSC::Tstring2UTF8(sLocalIP));

		pLocalNode->SetAttribute("loginname", "admin");
		pLocalNode->SetAttribute("loginpwd", "888");
	} while (0);

	//��������
	do 
	{
		TiXmlElement *pDataCenterNode = new TiXmlElement(NODE_DATA_CENTER);  
		pRootNode->LinkEndChild(pDataCenterNode);  

		std::_tstring sDataCenterIP(csDCIP.GetBuffer());
		csDCIP.ReleaseBuffer();
		pDataCenterNode->SetAttribute("ip", CSC::Tstring2UTF8(sDataCenterIP));

		std::_tstring sDataCenterPort(csDCPort.GetBuffer());
		csDCPort.ReleaseBuffer();
		pDataCenterNode->SetAttribute("port", CSC::Tstring2UTF8(sDataCenterPort));
	} while (0);

	pDoc->SaveFile(CONFIG_FILE);
	delete pDoc;
}

/** @fn       void CConfigToolDlg::RunCmd(const CString& cmd, const CString& DefaultDir)
 *  @brief    ��������
 *  @param    (CString) cmd [IN] : ShellExecute������
 *            (CString) DefaultDir [IN] : Ĭ�ϵ�����Ŀ¼
 *  @return:  void
 */
BOOL CLoginDlg::RunCmd(const CString& cmd, const CString& DefaultDir)
{
    OutputDebugString(_T("---------------------> RunCmd start.\n"));

    // ��־
	CString strDebug = _T("");
	strDebug.Format(_T("cmd[%s], DefaultDir[%s]."), (LPCTSTR)cmd, (LPCTSTR)DefaultDir);
	OutputDebugString(strDebug);

    // ִ������
	//UINT hInstance = WinExec("Wscript.exe restart.vbs ",SW_SHOW);
  
	HINSTANCE hInstance = ShellExecute(NULL, _T("open"), cmd, cmd, _T(""), SW_HIDE);


    BOOL bRet = FALSE;
    switch((int)hInstance)
    {
    case 0:
        MessageBox(_T("�ڴ治��"), _T("Error"));
        break;
    case ERROR_FILE_NOT_FOUND:
        MessageBox(_T("�ļ�����·��������"), _T("Error"));
        break;
    case ERROR_PATH_NOT_FOUND:
        MessageBox(_T("�ļ�����·��������"), _T("Error"));
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

    OutputDebugString(_T("---------------------> RunCmd end.\n"));
    return bRet;
}

//���ص�¼
void CLoginDlg::OnBnClickedButtonLocal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString rootPath;
	rootPath.Preallocate(128);
	DWORD nLength;
	if (0 != (nLength = GetModuleFileName(NULL, rootPath.GetBuffer(0), 128 - 1)))
	{
		rootPath.ReleaseBuffer(nLength);
		int idx = rootPath.ReverseFind('\\');
		rootPath = rootPath.Left(idx + 1);
	}

	CString runDir;
	// �������߿ͻ���
	runDir.Format(_T("%sRecTool.exe"), rootPath);
	FMSCLIENT_INFO("RecTool.exe Path:%s", fcT2A(runDir));

	// ִ������
	HINSTANCE hInstance = ShellExecute(NULL, _T("open"), runDir, runDir, NULL, SW_RESTORE);

	//�رձ�����
	CLoginDlg::OnCancel();
}

void CLoginDlg::OnPaint()
{
   // CPaintDC dc(this); // device context for painting
    // TODO: �ڴ˴������Ϣ����������
    // ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
    if (IsIconic())
    {
        CPaintDC dc(this); // ���ڻ��Ƶ��豸������

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // ����ͼ��
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

HCURSOR CLoginDlg::OnQueryDragIcon()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    return static_cast<HCURSOR>(m_hIcon);

}
