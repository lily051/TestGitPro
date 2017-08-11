#pragma once

//////////////////////////////////////////////////////////////////////////
// Color scheme
#define UIDLG_COLOR_BKGND	RGB(0xf5, 0xf2, 0xf2)   // 客户区背景颜色
#define UIDLG_COLOR_TITLE	RGB(0, 0, 0)    // 标题字体颜色

// 图片中该颜色不会画出,加载图片时PNG透明部分用该颜色填充到BMP
#define UIDLG_COLOR_TRANSPARENT	RGB(255, 255, 254)  

//////////////////////////////////////////////////////////////////////////
// Font
#define UIDLG_FONT_BAR_HEIGHT      100
#define UIDLG_FONT_BAR_WEIGHT      FW_MEDIUM
#define UIDLG_FONT_BAR_NAME        _T("宋体")

//////////////////////////////////////////////////////////////////////////
// ICON & PNG
#define UIDLG_ICON_MAINFRAME    128   // 左上角ICON资源号,从exe中加载
#define UIDLG_PNG               IDB_PNG_UIDIALOG    // PNG图片在资源dll中的资源号

// 默认包含PNG资源的dll文件名,也可设置
// UIDialog::SetResFileName(_T("XXResBlack.dll"));
#define UIDLG_RES_FILE          _T("PBSkinRed.dll")    
#define ULDLG_PNG_FILE          L"UIDialog.dat" // PNG图片文件名,优先加载PNG图片文件

