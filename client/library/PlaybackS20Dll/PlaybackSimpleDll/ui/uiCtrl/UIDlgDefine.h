#pragma once

//////////////////////////////////////////////////////////////////////////
// Color scheme
#define UIDLG_COLOR_BKGND	RGB(0xf5, 0xf2, 0xf2)   // �ͻ���������ɫ
#define UIDLG_COLOR_TITLE	RGB(0, 0, 0)    // ����������ɫ

// ͼƬ�и���ɫ���ử��,����ͼƬʱPNG͸�������ø���ɫ��䵽BMP
#define UIDLG_COLOR_TRANSPARENT	RGB(255, 255, 254)  

//////////////////////////////////////////////////////////////////////////
// Font
#define UIDLG_FONT_BAR_HEIGHT      100
#define UIDLG_FONT_BAR_WEIGHT      FW_MEDIUM
#define UIDLG_FONT_BAR_NAME        _T("����")

//////////////////////////////////////////////////////////////////////////
// ICON & PNG
#define UIDLG_ICON_MAINFRAME    128   // ���Ͻ�ICON��Դ��,��exe�м���
#define UIDLG_PNG               IDB_PNG_UIDIALOG    // PNGͼƬ����Դdll�е���Դ��

// Ĭ�ϰ���PNG��Դ��dll�ļ���,Ҳ������
// UIDialog::SetResFileName(_T("XXResBlack.dll"));
#define UIDLG_RES_FILE          _T("PBSkinRed.dll")    
#define ULDLG_PNG_FILE          L"UIDialog.dat" // PNGͼƬ�ļ���,���ȼ���PNGͼƬ�ļ�

