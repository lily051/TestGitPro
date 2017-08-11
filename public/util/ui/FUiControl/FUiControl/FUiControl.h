// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 FUICONTROL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// FUICONTROL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef FUICONTROL_EXPORTS
#define FUICONTROL_API EXTERN_C __declspec(dllexport)
#else
#define FUICONTROL_API EXTERN_C __declspec(dllimport)
#endif

// 初始化库
FUICONTROL_API void FUiInit();
// 清理库
FUICONTROL_API void FUiCleanup();
// 需要在 InitInstance 或 构造函数中调用，不能再 OnInitDialog 调用
// 建议设置皮肤文件的绝对路径
// 无密码可以传入 NULL
FUICONTROL_API void FUiSetRes(LPCTSTR lpszRes, LPCTSTR lpszPassword);

FUICONTROL_API Gdiplus::Image* FUiGetRes(LPCTSTR lpszResName);

#include "FUi/RenderMgr.h"
#include "FUi/FMemDCEx.h"
/*控件头文件*/
#include "FUi/FDialog.h"
#include "FUi/FBusyWait.h"
#include "FUi/FButton.h"
#include "FUi/FComboBox.h"
#include "FUi/FEdit.h"
#include "FUi/FListCtrl.h"
#include "FUi/FMenu.h"
#include "FUi/FMessageBox.h"
#include "FUi/FNcButton.h"
#include "FUi/FProgress.h"
#include "FUi/FSlider.h"
#include "FUi/FStatic.h"
#include "FUi/FTabCtrl.h"
#include "FUi/FTreeCtrl.h"
#include "FUi/FTreeCtrlEx.h"
#include "FUi/CalendarDlg.h"
#include "FUi/FDateTimeCtrl.h"
#include "FUi/FComboBoxEx.h"
#include "FUi/FScrollBar.h"
#include "FUi/FScrollWnd.h"
#include "FUi/TreeComboBox.h"

