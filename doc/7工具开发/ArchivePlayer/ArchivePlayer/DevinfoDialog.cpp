// DevinfoDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ArchivePlayer.h"
#include "DevinfoDialog.h"
#include "GeneralDef.h"

// DevinfoDialog �Ի���

struct ChannelShow
{
    int index;
    std::wstring itemName;

};

static ChannelShow channelList[] = 
{
    {0,_T("���")},
    {1,_T("����")},
    {2,_T("IP��ַ")},
    {3,_T("�˿�")},
    {4,_T("�û���")},
    {4,_T("ͨ����")},
    {5,_T("¼��ƻ�")},
    {5,_T("��һ¼��")},
    {5,_T("�ܶ�¼��")},
    {5,_T("����¼��")},
    {5,_T("����¼��")},
    {5,_T("����¼��")},
    {5,_T("����¼��")},
    {5,_T("����¼��")},

};


struct DevShow
{
    int index;
    std::wstring itemName;

};

 static DevShow DevList[] = 
{
    {0,_T("���")},
    {1,_T("����")},
    {2,_T("IP��ַ")},
    {3,_T("�˿�")},
    {4,_T("�û���")},
    {4,_T("ͨ����")},
    {5,_T("NTPУʱ")},
    {6,_T("��������ַ")},
    {7,_T("NTP�˿�")},
    {8,_T("Уʱ���")},
    {9,_T("�쳣����")},
};

IMPLEMENT_DYNAMIC(DevinfoDialog, CDialog)

DevinfoDialog::DevinfoDialog(CWnd* pParent /*=NULL*/)
	: CDialog(DevinfoDialog::IDD, pParent)
{

}

DevinfoDialog::~DevinfoDialog()
{
    
}



void DevinfoDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //DDX_Control(pDX, IDC_TREE_DEV, m_ctrlTreeDev);
    DDX_Control(pDX, IDC_LIST_DEV, m_listDev);
    DDX_Control(pDX, IDC_LIST_CHANNEL, m_listchannel);
}


void DevinfoDialog::AdjustColumnWidth(CListCtrl& list)
{
    RECT rc;
    CHeaderCtrl* pHeader = list.GetHeaderCtrl();
    if (!pHeader)
        return;

    // �ͻ�����
    list.GetClientRect(&rc);
    int nColCount = pHeader->GetItemCount();
    // ȥ��������ռ�õĿ��
    for (int i = 1; i < nColCount; i++)
    {
        rc.right -= list.GetColumnWidth(i);
    }

    // ȥ��������ռ�õĿ��
    SCROLLBARINFO sbi;
    if (list.GetScrollBarInfo(OBJID_VSCROLL, &sbi))
    {
        rc.right -= sbi.rcScrollBar.right - sbi.rcScrollBar.left;
    }

    // �޸��п�
    list.SetColumnWidth(0, rc.right > rc.left ? rc.right - rc.left : LVSCW_AUTOSIZE_USEHEADER);
}

void DevinfoDialog::initDevList()
{
    m_listDev.DeleteAllItems();
    //m_listDev.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EDITLABELS);

    int len = sizeof(DevList)/sizeof(DevShow);
    CRect drect;
    m_listDev.GetClientRect(&drect); 
    int itemWeight = drect.Width()/len;
    ;
    for(int i =0;i<len;++i)
    {
         DevList[i].index = m_listDev.InsertColumn(i,DevList[i].itemName.c_str(),LVCFMT_LEFT ,itemWeight);
    }

}

void DevinfoDialog::showDevList()
{
    m_listDev.DeleteAllItems();
    if(g_Devlist.empty())
        return;
    CString tmp;
    int cnt = 0;
    int itms = 0;
    for (std::size_t i = 0;i<g_Devlist.size();++i)
    { 
        cnt = 0;
        DevicInfo &pDev = g_Devlist.at(i);
        itms = i;
        m_listDev.InsertItem(itms,_T(""));
        tmp.Format(_T("%d"),i);
        m_listDev.SetItemText(itms, cnt++,tmp.GetBuffer());

        //�豸����
        if (pDev.type == DEVICE_TYPE_NVR)
        {
            tmp = _T("NVR");
        }
        else if(pDev.type == DEVICE_TYPE_DVR)
        {
            tmp = _T("DVR");
        }
        else
        {
            tmp = _T("δ֪�豸");
        }
        m_listDev.SetItemText(itms, cnt++,tmp.GetBuffer());
        coding codeip,codeuser;
        m_listDev.SetItemText(itms, cnt++,codeip.AnsiToUnicode(pDev.ip));

        tmp.Format(_T("%d"),pDev.port);
        m_listDev.SetItemText(itms, cnt++,tmp.GetBuffer());

        m_listDev.SetItemText(itms, cnt++,codeip.AnsiToUnicode(pDev.user));
        
        tmp.Format(_T("%d"),pDev.iIPChanNum);
        m_listDev.SetItemText(itms, cnt++,tmp.GetBuffer());
        //NTPУʱ
        if (pDev.ntpInfo.byEnableNTP == TRUE)
        {
            tmp = _T("����");
        }
        else
        {
            tmp = _T("�ر�");
        }
        m_listDev.SetItemText(itms, cnt++,tmp.GetBuffer());

        coding server;
        m_listDev.SetItemText(itms, cnt++,server.AnsiToUnicode((char *)pDev.ntpInfo.sNTPServer));

        tmp.Format(_T("%d"),pDev.ntpInfo.wNtpPort);
        m_listDev.SetItemText(itms, cnt++,tmp.GetBuffer());

        tmp.Format(_T("%d"),pDev.ntpInfo.wInterval);
        m_listDev.SetItemText(itms, cnt++,tmp.GetBuffer());

        coding codeerr;
        m_listDev.SetItemText(itms, cnt++,codeerr.AnsiToUnicode(pDev.errMsg.c_str()));

    }
}



void DevinfoDialog::showChannelList()
{
    m_listchannel.DeleteAllItems();
    if(g_Devlist.empty())
        return;
    CString tmp,Tmp;
    int cnt = 0;
    int itms = -1;
    for (std::size_t i = 0;i<g_Devlist.size();++i)
    { 
        
        DevicInfo &pDev = g_Devlist.at(i);
      
      
       
        for (std::size_t j = 0;j<pDev.chanList.size();++j)
        {
            CHANNEL_INFO &pChannel = pDev.chanList.at(j);

            if (pDev.iDeviceChanNum == -1)
            {
                continue;
            }    
            if(pChannel.bEnable)  //ͨ����Ч������ͨ����
            {

                cnt = 0;  
                itms++;
                m_listchannel.InsertItem(itms,_T(""));
                tmp.Format(_T("%d"),itms);
                m_listchannel.SetItemText(itms, cnt++,tmp.GetBuffer());
                //�豸����
                if (pDev.type == DEVICE_TYPE_NVR)
                {
                    tmp = _T("NVR");
                }
                else if(pDev.type == DEVICE_TYPE_DVR)
                {
                    tmp = _T("DVR");
                }
                else
                {
                    tmp = _T("δ֪�豸");
                }

                m_listchannel.SetItemText(itms, cnt++,tmp.GetBuffer());
                coding codeip,codeuser;
                m_listchannel.SetItemText(itms, cnt++,codeip.AnsiToUnicode(pDev.ip));

                tmp.Format(_T("%d"),pDev.port);
                m_listchannel.SetItemText(itms, cnt++,tmp.GetBuffer());

                m_listchannel.SetItemText(itms, cnt++,codeip.AnsiToUnicode(pDev.user));

                tmp.Format(_T("%d"),pChannel.iChanIndex);
                m_listchannel.SetItemText(itms, cnt++,tmp.GetBuffer());
                //¼��ƻ�Уʱ
                if (pChannel.recordPlan.dwRecord  == TRUE)
                {
                    tmp = _T("����");
                }
                else
                {
                    tmp = _T("�ر�");
                }          
                m_listchannel.SetItemText(itms, cnt++,tmp.GetBuffer());
                if(pChannel.recordPlan.dwRecord  != TRUE)
                {
                    continue;
                }
                for (int k = 0;k<MAX_DAYS;++k)
                {
                    if (pChannel.recordPlan.struRecAllDay[k].byAllDayRecord == 1)
                    {
                             m_listchannel.SetItemText(itms, cnt++,_T("ȫ��"));
                    }
                    else
                    {
                        Tmp = _T("");
                        for (int h =0;h<MAX_TIMESEGMENT_V30;h++)
                        {
                            tmp.Format(_T(" %02d:%02d~%02d:%02d"),
                            pChannel.recordPlan.struRecordSched[k][h].struRecordTime.byStartHour,
                            pChannel.recordPlan.struRecordSched[k][h].struRecordTime.byStartMin,
                            pChannel.recordPlan.struRecordSched[k][h].struRecordTime.byStopHour,
                            pChannel.recordPlan.struRecordSched[k][h].struRecordTime.byStopMin);
                            Tmp += tmp;

                        }
                         m_listchannel.SetItemText(itms, cnt++,Tmp.GetBuffer());

                    }
                }


            }
        }

    }
}

void DevinfoDialog::initChannalList()
{
    m_listchannel.DeleteAllItems();  

    //m_listDev.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EDITLABELS);

    int len = sizeof(channelList)/sizeof(ChannelShow);
    CRect drect;
    m_listchannel.GetClientRect(&drect); 
    int itemWeight = drect.Width()/len;
    
    for(int i =0;i<len;++i)
    {
        channelList[i].index = m_listchannel.InsertColumn(i,channelList[i].itemName.c_str(),LVCFMT_LEFT ,itemWeight);
    }

    
}


BOOL DevinfoDialog::OnInitDialog()
{
    CDialog::OnInitDialog();
   
    //initDevList();
    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE

}

BEGIN_MESSAGE_MAP(DevinfoDialog, CDialog)
     ON_WM_SIZE()
//   ON_NOTIFY(NM_CLICK, IDC_TREE_DEV, &DevinfoDialog::OnNMClickTreeDev)
//    ON_NOTIFY(NM_DBLCLK, IDC_TREE_DEV, &DevinfoDialog::OnNMDblclkTreeDev)
ON_WM_PAINT()
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DEV, &DevinfoDialog::OnLvnItemchangedListDev)
END_MESSAGE_MAP()


void DevinfoDialog::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    GetClientRect(&m_rect); 



    if(m_listDev.m_hWnd) 
    { 
        m_listDev.SetWindowPos(NULL, m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height()/3, SWP_DRAWFRAME);  
    }    
    if(m_listchannel.m_hWnd) 
    { 
        m_listchannel.SetWindowPos(NULL, m_rect.left, m_rect.top+m_rect.Height()/3+20, m_rect.Width(), m_rect.Height(), SWP_DRAWFRAME);  
    }
  /*  CRect drect;
    m_listDev.GetClientRect(&drect); 
    int itemWeight = drect.Width()/(sizeof(DevList)/sizeof(DevShow));
    m_listDev.SetColumnWidth(0,itemWeight);*/
    // TODO: Add your message handler code here
}


// DevinfoDialog ��Ϣ�������

/*
void DevinfoDialog::setNTPTiming(const int index)
{

    if(index <= 0 || g_Devlist.empty())
    {
        m_ntpdlg.ShowWindow(SW_HIDE);
    }
    else
    {
        m_ntpdlg.ShowWindow(SW_SHOW);

        if(TRUE == g_Devlist.at(index).ntpInfo.byEnableNTP )
        {
            CString tmp;
            tmp.Format(_T("%d"),g_Devlist.at(index).ntpInfo.wInterval);
            m_ntpdlg.m_editNTPinterval.SetWindowText(tmp);

            tmp.Format(_T("%d"),g_Devlist.at(index).ntpInfo.wNtpPort);


            DWORD  dwIP; 
            dwIP  =  inet_addr((char*)g_Devlist.at(index).ntpInfo.sNTPServer);
            unsigned  char  *pIP  =  (unsigned  char*)&dwIP;
            m_ntpdlg.m_editNTPip.SetAddress(*pIP,  *(pIP+1),  *(pIP+2),  *(pIP+3));
            m_ntpdlg.m_editNTPport.SetWindowText(tmp);
            m_ntpdlg.setNTPedit(FALSE);   
            m_ntpdlg.m_checkNTPswitch.SetCheck(TRUE);
        }
        else
        {
            m_ntpdlg.setNTPedit(TRUE);
            m_ntpdlg.m_checkNTPswitch.SetCheck(FALSE);
        }

    }
}

void DevinfoDialog::OnNMDblclkTreeDev(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    HTREEITEM hSelected = m_ctrlTreeDev.GetSelectedItem();
    //δѡ��
    if(NULL == hSelected)
        return;
    DWORD itemData = m_ctrlTreeDev.GetItemData(hSelected);
    HTREEITEM hParent = NULL;
    TRACE("ItemData:%d\n",itemData);

    int itype = itemData/1000;    //
    int iIndex = itemData%1000;
    setNTPTiming(itype-1);
    



    *pResult = 0;
}
*/
void DevinfoDialog::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: �ڴ˴������Ϣ����������
    // ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
    //showDevList();
}

void DevinfoDialog::OnLvnItemchangedListDev(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    if((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1)) /* old state : unchecked */ 
        && (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2)) /* new state : checked */ 
        ) 
    { 
        TRACE("Item %d is checked\n", pNMLV->iItem); 
        if ((std::size_t)pNMLV->iItem<g_Devlist.size())
        {    
            //if (g_Devlist.at(pNMLV->iItem).ntpInfo.byEnableNTP == FALSE)
            {  
                g_Devlist.at(pNMLV->iItem).beOpenNTP = TRUE;
            }
          
        }

    } 
    else if((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2)) /* old state : checked */ 
        && (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1)) /* new state : unchecked */ 
        ) 
    { 
        TRACE("Item %d is unchecked\n", pNMLV->iItem); 
        if ((std::size_t)pNMLV->iItem<g_Devlist.size())
        {   
            g_Devlist.at(pNMLV->iItem).beOpenNTP = FALSE;
        }
    } 
    else 
    { 
        TRACE("Item %d does't change the check-status\n", pNMLV->iItem); 
    } 

    *pResult = 0;
}
