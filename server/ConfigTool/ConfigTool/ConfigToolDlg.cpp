
// ConfigToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ConfigTool.h"
#include "ConfigToolDlg.h"
#include "Tinyxml/tinyxml.h"
#include "Tinyxml/tinyxmlutils.h"
#include <vector>
#pragma comment(lib,"Iphlpapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CONFIG_FILE			"ServiceConfig.xml"
#define TOP_DIRECTORY_CONFIG_FILE   "../ServiceConfig.xml"
#define NODE_ROOT			"ROOT"
#define NODE_LOCAL_PARAM	"LOCAL_PARAM"
#define NODE_DATA_CENTER	"DATA_CENTER"

/*******************************************************************************
*    Function:      AppFun_GetLocalIPAddr 
*    Description:   ��ȡ����IP
*    Input:         
*    Output:        CString & csIPAddr���õ�IP
*    Return:	    �ɹ�����TRUE,����FALSE 
*******************************************************************************/
BOOL AppFun_GetLocalIPAddr(CString & csIPAddr)
{
	WORD   wVersionRequested;   
	WSADATA   wsaData;   
	char   name[255];   
	PHOSTENT   hostinfo;   
	wVersionRequested = MAKEWORD(2,0);   

    TRACE("@ljg-----A");
	if(WSAStartup(wVersionRequested,&wsaData) == 0)   
	{   
		if(gethostname(name,sizeof(name)) == 0)   
		{   
			if((hostinfo = gethostbyname(name)) != NULL)   
			{   
				csIPAddr = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
			}   
		}   
		WSACleanup();
	}   
    TRACE("@ljg-----B");
	if (!csIPAddr.IsEmpty())
	{
		return TRUE;
	}
	return FALSE;
}

void AppFun_GetAllLocalIPAddr(std::vector<CString> & vecIP)   
{       
    PMIB_IPADDRTABLE pIPAddrTable;
    DWORD dwSize = 0;
    pIPAddrTable = (MIB_IPADDRTABLE*) GlobalAlloc(GPTR, sizeof(MIB_IPADDRTABLE));
    if ( pIPAddrTable ) 
    {
        if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) 
        {
            GlobalFree( pIPAddrTable );
            pIPAddrTable = (MIB_IPADDRTABLE *) GlobalAlloc (GPTR, dwSize);
        }
    }
    else
    {
        OutputDebugString(_T("@ljg-Memory allocation failed."));
    }

    if ( pIPAddrTable ) 
    {
        // Make a second call to GetIpAddrTable to get the
        // actual data we want
        if ( GetIpAddrTable( pIPAddrTable, &dwSize, 0 ) == NO_ERROR ) 
        { 
            for (unsigned int i=0;i<dwSize/sizeof(MIB_IPADDRTABLE);++i)
            {
                if (pIPAddrTable->table[i].dwAddr != 0)
                {
                    in_addr in;
                    in.S_un.S_addr = pIPAddrTable->table[i].dwAddr;
                    CString tempIP;
                    tempIP = ::inet_ntoa(in);
                    vecIP.push_back(tempIP);
                }
            }
        }
        else 
        {
            OutputDebugString(_T("@ljg-Call to GetIpAddrTable failed."));
        }
    }

    if (pIPAddrTable)
    {
        GlobalFree( pIPAddrTable );
    }
}


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CConfigToolDlg �Ի���




CConfigToolDlg::CConfigToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigToolDlg::IDD, pParent)
    , m_sOldDCPort(_T("8849"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConfigToolDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_DATA_PORT, m_editDCPort);
    DDX_Control(pDX, IDC_IP_DC, m_dcIPCtrl);
    DDX_Control(pDX, IDC_LOCAL_IP, m_localIPCtrl);
    DDX_Control(pDX, IDC_HISTORY_IP, m_historyIP);
}

BEGIN_MESSAGE_MAP(CConfigToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_SAVE, &CConfigToolDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CConfigToolDlg::OnBnClickedBtnExit)
END_MESSAGE_MAP()


// CConfigToolDlg ��Ϣ�������

BOOL CConfigToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    m_editDCPort.SetLimitText(6);
    AppFun_GetAllLocalIPAddr(m_vecLocalIP);
	ReadConfig();

    m_dcIPCtrl.SetWindowText(m_sOldDCIP);
    m_editDCPort.SetWindowText(m_sOldDCPort);
    m_localIPCtrl.SetWindowText(m_sOldLocalIP);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CConfigToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CConfigToolDlg::OnPaint()
{
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CConfigToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// return 1 or 0
int is_valid_ip(const char *ip_str) 
{
    char temp[31];
    int a,b,c,d; 
    if(sscanf(ip_str, "%d.%d.%d.%d ",&a,&b,&c,&d)==4 
        &&   a>=0   &&   a<=255 
        &&   b>=0   &&   b<=255 
        &&   c>=0   &&   c<=255   
        &&   d>=0   &&   d<=255)    
    {    
        sprintf(temp, "%d.%d.%d.%d",a,b,c,d);    //�Ѹ�ʽ��������д���ַ���temp     
        if(strcmp(temp,ip_str)==0)     
        {    
            return 1;     
        }     
        else    
        {    
            return 0;
        }    
    }    
    else     
    {    
        return 0; 
    }    
}

//�������Ϸ���
BOOL CConfigToolDlg::CheckParamInvalid()
{
    //�������ĺϷ���
    DWORD dwDCAddress;
    int nRet = m_dcIPCtrl.GetAddress(dwDCAddress);
    if (4 != nRet) 
    {
		MessageBox(_T("���ķ���ip��ַ���Ϸ�!"),_T("����"),MB_ICONWARNING|MB_OK);
        m_dcIPCtrl.SetFocus();
        return FALSE;
    }
    if (0 == dwDCAddress)
    {
		MessageBox(_T("���ķ���ip��ַ���Ϸ�!"),_T("����"),MB_ICONWARNING|MB_OK);
        m_dcIPCtrl.SetFocus();
        return FALSE;
    }

    DWORD dwLocalAddress;
    nRet = m_localIPCtrl.GetAddress(dwLocalAddress);
    if (4 != nRet) 
    {
		MessageBox(_T("����ip��ַ���Ϸ�!"),_T("����"),MB_ICONWARNING|MB_OK);
        m_localIPCtrl.SetFocus();
        return FALSE;
    }
    if (0 == dwLocalAddress)
    {
		MessageBox(_T("����ip��ַ���Ϸ�!"),_T("����"),MB_ICONWARNING|MB_OK);
        m_localIPCtrl.SetFocus();
        return FALSE;
    }

    CString csDCIP,csDCPort,csLocalIP;
    m_dcIPCtrl.GetWindowText(csDCIP);
    csDCIP.Trim();
    m_editDCPort.GetWindowText(csDCPort);
    csDCPort.Trim();
    m_localIPCtrl.GetWindowText(csLocalIP);
    csLocalIP.Trim();

    if (!is_valid_ip(CStringA(csDCIP)))
    {
        MessageBox(_T("���ķ���ip��ַ���Ϸ�!"),_T("����"),MB_ICONWARNING|MB_OK);
        m_dcIPCtrl.SetFocus();
        return FALSE;
    }

    if (csDCPort.IsEmpty())
    {
		MessageBox(_T("���ķ���ip��ַ����Ϊ��!"),_T("����"),MB_ICONWARNING|MB_OK);
        m_editDCPort.SetFocus();
        m_editDCPort.SetSel(0, -1);
        return FALSE;
    }

    int nDCPort = _tstoi(csDCPort);
    if (nDCPort<=1024 || nDCPort >=65535)
    {
		MessageBox(_T("���ķ���˿ڷ�ΧΪ 1024~65535!"),_T("����"),MB_ICONWARNING|MB_OK);
        m_editDCPort.SetFocus();
        m_editDCPort.SetSel(0, -1);
        return FALSE;
    }

    if (!is_valid_ip(CStringA(csLocalIP)))
    {
        MessageBox(_T("����ip��ַ���Ϸ�!"),_T("����"),MB_ICONWARNING|MB_OK);
        m_localIPCtrl.SetFocus();
        return FALSE;
    }

    return TRUE;
}
void CConfigToolDlg::OnBnClickedBtnSave()
{
    //�������Ϸ���
    if (!CheckParamInvalid())
    {
        return;
    }

    //////////////////////////////////////
	SaveConfig();
    m_dcIPCtrl.GetWindowText(m_sOldDCIP);
    m_editDCPort.GetWindowText(m_sOldDCPort);
    m_localIPCtrl.GetWindowText(m_sOldLocalIP);

	MessageBox(_T("����ɹ�!"),_T("��ʾ"),MB_ICONINFORMATION|MB_OK);
}

void CConfigToolDlg::OnBnClickedBtnExit()
{
	CConfigToolDlg::OnCancel();
}

//��ȡ�����ļ�
void CConfigToolDlg::ReadConfig()
{
	TiXmlDocument xmlDoc;
	if (!xmlDoc.LoadFile(CONFIG_FILE))
	{
        if (!xmlDoc.LoadFile(TOP_DIRECTORY_CONFIG_FILE))
        {
            return;
        }
	}

	TiXmlHandle  docHandle(&xmlDoc);

	//���ز���
	do 
	{
		TiXmlElement * pLocalNode = docHandle.FirstChild(NODE_ROOT).FirstChild(NODE_LOCAL_PARAM).ToElement();
		if (pLocalNode)
        {
            std::_tstring sIP = CSC::UTF82Tstring(pLocalNode->Attribute("ip"));
            m_sOldLocalIP = sIP.c_str();
		}
	} while (0);

	//��������
	do 
	{
		TiXmlElement * pDataCenterNode = docHandle.FirstChild(NODE_ROOT).FirstChild(NODE_DATA_CENTER).ToElement();
		if (pDataCenterNode)
		{
            m_sOldDCIP = CSC::UTF82Tstring(pDataCenterNode->Attribute("ip")).c_str();
			m_sOldDCPort = CSC::UTF82Tstring(pDataCenterNode->Attribute("port")).c_str();
        }
	} while (0);
}

/*lint -e429*/ 
//���������ļ�
void CConfigToolDlg::SaveConfig()
{
    CString csDCIP,csDCPort,csLocalIP;
    m_dcIPCtrl.GetWindowText(csDCIP);
    m_editDCPort.GetWindowText(csDCPort);
    m_localIPCtrl.GetWindowText(csLocalIP);

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

// 		pLocalNode->SetAttribute("loginname", "admin");
// 		pLocalNode->SetAttribute("loginpwd", "888");
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

/*lint +e429*/ 

BOOL CConfigToolDlg::PreTranslateMessage(MSG* pMsg)
{
    switch(pMsg->message)
    {
    case WM_KEYDOWN:
        {
            if (VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam)
            {
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
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void CConfigToolDlg::OnCancel()
{
    CString sCMSIP,sCMSPort,sLocalIP;
    m_dcIPCtrl.GetWindowText(sCMSIP);
    m_editDCPort.GetWindowText(sCMSPort);
    m_localIPCtrl.GetWindowText(sLocalIP);

    if (m_sOldDCIP != sCMSIP
        || m_sOldDCPort != sCMSPort
        || m_sOldLocalIP != sLocalIP)
    {
		if (IDYES == MessageBox(_T("�Ƿ񱣴�?"),_T("��ʾ"),MB_YESNO|MB_ICONQUESTION))
        {
            //�������Ϸ���
            if (!CheckParamInvalid())
            {
                return;
            }

            SaveConfig();
        }
    }

    CDialog::OnCancel();
}

void CConfigToolDlg::IPHistoryDropdown(void)
{
    int ipCount = m_vecLocalIP.size();
    if (ipCount <= 0)
    {
        return;
    }

    CMenu IPListMenu;
    IPListMenu.CreatePopupMenu();	// create menu

    for (int i = 0; i < ipCount; ++i)
    {
        if (!m_vecLocalIP[i].IsEmpty())
        {
            //���õ�IDֵ��TPM_RETURNCMD�л᷵�أ�������Ҫ����Ϊ0����Ϊδѡ��ʱ���ص�Ҳ��0
            IPListMenu.AppendMenu(MF_STRING | MF_ENABLED, i+1, m_vecLocalIP[i]);
        }
    }

    POINT curPos;
    GetCursorPos(&curPos);
    SetForegroundWindow();

    // show the menu
    int nSel = IPListMenu.TrackPopupMenu(
        TPM_LEFTBUTTON | TPM_RIGHTALIGN | TPM_NONOTIFY | TPM_RETURNCMD, 
        curPos.x, curPos.y+10, this);

    if (nSel>0)
    {
        // fill the IP address control with the selection in the menu
        m_localIPCtrl.SetWindowText(m_vecLocalIP[nSel-1]);
        ::PostMessage(m_hWnd, 0, 0, 0);
    }


    // destroy pop up menu
    IPListMenu.DestroyMenu();
}
