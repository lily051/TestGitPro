// IntelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RPConfig.h"
#include "IntelDlg.h"
#include <HCNetSDK.h>
#include <IntelligentCfgDef.h>

// CIntelDlg �Ի���

IMPLEMENT_DYNAMIC(CIntelDlg, CDialog)

CIntelDlg::CIntelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIntelDlg::IDD, pParent)
    , m_bIntelligent(FALSE)
{
    m_brBackGray.CreateSolidBrush(DIALOG_BK_COLOR);
}

CIntelDlg::~CIntelDlg()
{
    m_brBackGray.DeleteObject();
}

void CIntelDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE_DEV, m_treeCtrl);
}


BEGIN_MESSAGE_MAP(CIntelDlg, CDialog)
    ON_WM_DESTROY()
    ON_WM_CTLCOLOR()
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DEV, &CIntelDlg::OnTvnSelchangedTreeDev)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CIntelDlg ��Ϣ�������

BOOL CIntelDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    CBitmap bmpTree[2];
    m_imTree.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 1);
    bmpTree[0].LoadBitmap(IDB_BITMAP_DEVICE);
    m_imTree.Add(&bmpTree[0], RGB(255,255,255));
    bmpTree[1].LoadBitmap(IDB_BITMAP_CAMERA);
    m_imTree.Add(&bmpTree[1], RGB(255,255,255));
    m_treeCtrl.SetImageList(&m_imTree, LVSIL_NORMAL);

    HTREEITEM hP = m_treeCtrl.InsertItem(m_struFunParam.sDevIP,0,0,TVI_ROOT);
    m_treeCtrl.SetItemData(hP,-1);
    for (int i =0;i<m_struFunParam.nChannel;i++)
    {
        CString stName;
        stName.Format(_T("ͨ��%d"),i+1);
        HTREEITEM hParent = m_treeCtrl.InsertItem(stName,1,1,hP);
        m_treeCtrl.SetItemData(hParent,i);
    }
    m_treeCtrl.Expand(hP,TVE_EXPAND);

    //���������ý���
    NET_INTELCFG_WNDPARAM strIntelWnd;
    strIntelWnd.hParWnd = m_hWnd;
    strIntelWnd.iLanguage = LANG_CHI;
    strIntelWnd.bFixedSize = TRUE;
    strIntelWnd.rcWndRect.SetRect(220,11,948,701);

    m_bIntelligent = NET_DVR_CreateWnd(&strIntelWnd);

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CIntelDlg::PreTranslateMessage(MSG* pMsg)
{
    switch (pMsg->message)
    {
    case WM_KEYDOWN:
        {
            //����Enter����
            if (VK_RETURN == pMsg->wParam)
            {
                return TRUE;
            }
            //����Esc��
            if (VK_ESCAPE == pMsg->wParam)
            {
                return TRUE;
            }
        }
        break;
    case WM_SYSKEYDOWN:
        {
            //����Alt + F4
            if (VK_F4  == pMsg->wParam)
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

void CIntelDlg::OnDestroy()
{
    CDialog::OnDestroy();

    NET_DVR_DestroyDlg(true);
}

HBRUSH CIntelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    if(nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG)
    {
        pDC->SetBkColor(DIALOG_BK_COLOR);
        pDC->SetBkMode(TRANSPARENT);
        hbr = m_brBackGray;
    }

    return hbr;
}

void CIntelDlg::OnTvnSelchangedTreeDev(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    DWORD iData = m_treeCtrl.GetItemData(pNMTreeView->itemNew.hItem);

    if (m_bIntelligent)
    {
        USES_CONVERSION;//��Ҫд��ѭ������
        NET_INTELCFG_DEVLOGININFO strIntelDev;
        sprintf_s(strIntelDev.chDevIP,T2A(m_struFunParam.sDevIP));
        sprintf_s(strIntelDev.chUserName,T2A(m_struFunParam.sDevUser));
        sprintf_s(strIntelDev.chPassword,T2A(m_struFunParam.sDevPWD));
        strIntelDev.wDevPort = (WORD)m_struFunParam.nDevPort;
        strIntelDev.iChanNum = iData;
        if (-1 == (int)iData)
        {
            NET_DVR_SetIntelligentCfg(&strIntelDev);
        }
        else
        {
            NET_DVR_SetChanIntelligentCfg(&strIntelDev);
        }
    }
    *pResult = 0;
}

BOOL CIntelDlg::OnEraseBkgnd(CDC* pDC)
{
    return CDialog::OnEraseBkgnd(pDC);
}
