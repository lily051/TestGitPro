// BusinessDialg.cpp : 实现文件
//

#include "stdafx.h"
#include "ArchivePlayer.h"
#include "BusinessDialg.h"


// BusinessDialg 对话框

IMPLEMENT_DYNAMIC(BusinessDialg, CDialog)

BusinessDialg::BusinessDialg(CWnd* pParent /*=NULL*/)
	: CDialog(BusinessDialg::IDD, pParent)
{
    m_curItem = -1;
}

BusinessDialg::~BusinessDialg()
{
}

void BusinessDialg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_listdata);
}

BOOL BusinessDialg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_listdata.SetExtendedStyle(LVS_EX_GRIDLINES /*| LVS_EX_CHECKBOXES*/|LVS_EX_FULLROWSELECT|LVS_EDITLABELS);
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE

}

BEGIN_MESSAGE_MAP(BusinessDialg, CDialog)
    ON_WM_SIZE()
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &BusinessDialg::OnNMCustomdrawList1)
END_MESSAGE_MAP()

void BusinessDialg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
  
    GetClientRect(&m_rect); 

    if(m_listdata.m_hWnd) 
    { 
        m_listdata.SetWindowPos(NULL, m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), SWP_DRAWFRAME);  
    }
    // TODO: Add your message handler code here
}

 int BusinessDialg::getWeight()
 {
     return m_rect.Width();
 }

void BusinessDialg::setItemColour(int item)
{

    m_curItem = item;
}

 void BusinessDialg::OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult)
 {
     NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );  

     // Take the default processing unless we set this to something else below.  
     *pResult = CDRF_DODEFAULT;  


     // First thing - check the draw stage. If it's the control's prepaint  
     // stage, then tell Windows we want messages for every item.  
     if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )  
     {  
         *pResult = CDRF_NOTIFYITEMDRAW;  
     }  
     else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )  
     {  
         // This is the notification message for an item.  We'll request  
         // notifications before each subitem's prepaint stage.  


         *pResult = CDRF_NOTIFYSUBITEMDRAW;  
     }  
     else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )  
     {  


         COLORREF clrNewTextColor, clrNewBkColor;  

         int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );  

       // m_listdata.GetHeaderCtrl()->GetItemCount()-1;
        CString strTemp = m_listdata.GetItemText(nItem,ITEM_RECORD_REMOTE);  
        
        if( strTemp.CollateNoCase(_T("异常"))==0){  
             clrNewTextColor = RGB(0,0,0);       //text  
             clrNewBkColor = RGB(255,0,0);           //红色  
         }else{  
             clrNewTextColor = RGB(0,0,0);       //text  
             clrNewBkColor = RGB(255,255,255);           //白色  
         }  


         pLVCD->clrText = clrNewTextColor;  
         pLVCD->clrTextBk = clrNewBkColor;  




         // Tell Windows to paint the control itself.  
         *pResult = CDRF_DODEFAULT; 

     }  
 }
