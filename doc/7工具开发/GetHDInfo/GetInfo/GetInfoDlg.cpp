
// GetInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GetInfo.h"
#include "GetInfoDlg.h"
#include "HCNetSDK.h"
#include "DecodeCardSdk.h"
#include "DataType.h"
#include "plaympeg4.h"
#include <string>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CGetInfoDlg �Ի���




CGetInfoDlg::CGetInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetInfoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGetInfoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_GETINFO, &CGetInfoDlg::OnBnClickedBtnGetinfo)
END_MESSAGE_MAP()


// CGetInfoDlg ��Ϣ�������

BOOL CGetInfoDlg::OnInitDialog()
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

	SetDlgItemText(IDC_EDIT1,L"127.0.0.1");
	SetDlgItemText(IDC_EDIT2,L"8000");
	SetDlgItemText(IDC_EDIT3,L"admin");
	SetDlgItemText(IDC_EDIT4,L"hik12345+");

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGetInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGetInfoDlg::OnPaint()
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
HCURSOR CGetInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGetInfoDlg::OnBnClickedBtnGetinfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//---------------------------------------
	// ��ʼ��
	NET_DVR_Init();
	//��������ʱ��������ʱ��
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	//��¼�����������豸��ַ����¼�û��������
	NET_DVR_USER_LOGIN_INFO struLoginInfo;// = {0};
	memset(&struLoginInfo,0,sizeof(NET_DVR_USER_LOGIN_INFO));

	CString csip,csport,csuser,cspsd;

	GetDlgItemText(IDC_EDIT1,csip);
	GetDlgItemText(IDC_EDIT2,csport);
	GetDlgItemText(IDC_EDIT3,csuser);
	GetDlgItemText(IDC_EDIT4,cspsd);
   
    int port = _ttoi(csport);
	//strcpy_s(struLoginInfo.sDeviceAddress, "10.16.37.105"); //�豸IP��ַ

    
	USES_CONVERSION;

	struLoginInfo.bUseAsynLogin = 0; //ͬ����¼��ʽ
	strcpy_s(struLoginInfo.sDeviceAddress, T2A(csip)); //�豸IP��ַ
	struLoginInfo.wPort = port; //�豸����˿�
	strcpy_s(struLoginInfo.sUserName, T2A(csuser)); //�豸��¼�û���
	strcpy_s(struLoginInfo.sPassword, T2A(cspsd)); //�豸��¼����

	//�豸��Ϣ, �������
	NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};
	//ע���豸
	LONG lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
	
	if (lUserID == -1)
	{
		AfxMessageBox(L"��¼ʧ��");
		return ;
	}

	CString strInfo;
	NET_DVR_HDCFG struHDCfg;
	memset(&struHDCfg,0,sizeof(NET_DVR_HDCFG));
	DWORD BytesReturned = 0;
	if(!NET_DVR_GetDVRConfig(lUserID
		,NET_DVR_GET_HDCFG
		,0xFFFFFFFF
		,&struHDCfg,sizeof(NET_DVR_HDCFG)
		,&BytesReturned))
	{
		LONG dwErrno = NET_DVR_GetLastError();
		std::string szLastErr = NET_DVR_GetErrorMsg(&dwErrno);

        CString str;
	    //String תCString
		int len = MultiByteToWideChar(CP_ACP, NULL, szLastErr.c_str(), szLastErr.size(), NULL, 0); 
		wchar_t* szUnicode = new wchar_t[len+1]; 
		MultiByteToWideChar(CP_ACP, NULL, szLastErr.c_str(), szLastErr.size(), szUnicode, len); 
		szUnicode[len] = 0; 
		std::wstring rs = szUnicode; 
        str = szUnicode;
		strInfo.Format(_T("��ȡ������Ϣʧ�ܣ�����ԭ��(%s)!"),str);
		MessageBox(strInfo, _T("����"), MB_ICONSTOP);
		NET_DVR_Logout_V30(lUserID);
		return;
	}
	else
	{
		CString count,dwHDNo,dwCapacity,dwFreeSpace,dwHdStatus,dwStorageType,byRecycling;
		CString byHDAttr,byHDType,byDiskDriver,dwHdGroup,dwPictureCapacity,dwFreePictureSpace;
		CString strHDInfo;
        
		strHDInfo.Format(_T("Ӳ������ %d ���� \r\n"),struHDCfg.dwHDCount);
		for (int i = 0; i < struHDCfg.dwHDCount; i++)
		{
			dwHDNo.Format(_T("Ӳ�̺�%d; "),struHDCfg.struHDInfo[i].dwHDNo);
			dwCapacity.Format(_T("������%dM; "),struHDCfg.struHDInfo[i].dwCapacity);
			dwFreeSpace.Format(_T("ʣ������%dM; "),struHDCfg.struHDInfo[i].dwFreeSpace);
			dwHdStatus.Format(_T("Ӳ��״̬%d; \r\n"),struHDCfg.struHDInfo[i].dwHdStatus);
			strHDInfo+=dwHDNo+dwCapacity+dwFreeSpace+dwHdStatus;
		}

		
		//byHDAttr.Format(_T("0-��ͨ, 1-���� 2-ֻ�� 3-�浵��CVRר�ã�4-���ɶ�д  %d; "),struHDCfg.struHDInfo[0].byHDAttr);;       /*0-��ͨ, 1-����; 2-ֻ�� 3-�浵��CVRר�ã���4-���ɶ�д*/
		//byHDType.Format(_T("0-����Ӳ��,1-ESATAӲ��,2-NASӲ��,3-iSCSIӲ�� 4-Array�������,5-SD��  %d; "),struHDCfg.struHDInfo[0].byHDType);;		  /*0-����Ӳ��,1-ESATAӲ��,2-NASӲ��,3-iSCSIӲ�� 4-Array�������,5-SD��*/
		//byDiskDriver.Format(_T("ֵ ������ASCII�ַ� %d; "),struHDCfg.struHDInfo[0].byDiskDriver);;   // ֵ ������ASCII�ַ� 
		////byRes1.Format(_T("�����ĸ�����%d"),struHDCfg.struHDInfo[0].byRes1);;
		//dwHdGroup.Format(_T("�����ĸ�����  %d; "),struHDCfg.struHDInfo[0].dwHdGroup);;      /*�����ĸ����� 1-MAX_HD_GROUP*/
		//byRecycling.Format(_T("�Ƿ�ѭ������ 0��ѭ������1ѭ������  %d; "),struHDCfg.struHDInfo[0].byRecycling);;   // �Ƿ�ѭ������ 0����ѭ�����ã�1��ѭ������
		////BYTE  byRes2[3];
		//dwStorageType.Format(_T("��λ��ʾ 0-��֧�� ��0-֧��  %d; "),struHDCfg.struHDInfo[0].dwStorageType);    //��λ��ʾ 0-��֧�� ��0-֧��

		////dwHdGroup.Format(_T("�����ĸ�����%d"),struHDCfg.struHDInfo[0].dwHdGroup);

		//dwPictureCapacity.Format(_T("Ӳ��ͼƬ���� %d; "),struHDCfg.struHDInfo[0].dwPictureCapacity);
		//dwFreePictureSpace.Format(_T("ʣ��ͼƬ�ռ� %d; "),struHDCfg.struHDInfo[0].dwFreePictureSpace);

		MessageBox(strHDInfo, _T("Ӳ��״̬����"), MB_ICONINFORMATION);
		NET_DVR_Logout_V30(lUserID);
		return;
		//MessageBox(L"OK", _T("����"), MB_ICONSTOP);
	}
	if (struHDCfg.dwHDCount == 0)
	{
		strInfo.Format(_T("��¼��ʧ�ܣ����豸û��Ӳ�̴洢¼��!"));
		MessageBox(strInfo, _T("����"), MB_ICONSTOP);
		//return void;
	}
	NET_DVR_Logout_V30(lUserID);
	
}
