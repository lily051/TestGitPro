// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� FUICONTROL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// FUICONTROL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef FUICONTROL_EXPORTS
#define FUICONTROL_API EXTERN_C __declspec(dllexport)
#else
#define FUICONTROL_API EXTERN_C __declspec(dllimport)
#endif

// ��ʼ����
FUICONTROL_API void FUiInit();
// �����
FUICONTROL_API void FUiCleanup();
// ��Ҫ�� InitInstance �� ���캯���е��ã������� OnInitDialog ����
// ��������Ƥ���ļ��ľ���·��
// ��������Դ��� NULL
FUICONTROL_API void FUiSetRes(LPCTSTR lpszRes, LPCTSTR lpszPassword);

FUICONTROL_API Gdiplus::Image* FUiGetRes(LPCTSTR lpszResName);

#include "FUi/RenderMgr.h"
#include "FUi/FMemDCEx.h"
/*�ؼ�ͷ�ļ�*/
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

