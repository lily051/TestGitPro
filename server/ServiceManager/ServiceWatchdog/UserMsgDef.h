#pragma once
//
// Copyright (c) 2009-2010 Hikvision Digital Technology Co., Ltd.
//
// Summary:
//     User Message Definition .
//
// Modification History:
//   2009-11-23:
//     V1.0 / ��ͨ�� / ����
//

//
// Summary:
//     ServiceController Windows Service ״̬�����Ϣ��
//
#define WM_SERVICE_STATUS_CHANGED           (WM_USER + 100)
//
// Summary:
//     ServiceController �쳣��Ϣ��
//
#define WM_SERVICE_CONTROL_COMMAND_ERROR    (WM_USER + 101)

//
// Summary:
//     ������Ϣ��
//
#define WM_ICON_NOTIFY                      (WM_USER + 102)

// �������������˳��������Ϣ, ID �����Ըı䣡
#define WM_USER_CLOSE                       (WM_USER + 2025)
//
// Summary:
//     ��Ӧ�������ؽ���Ϣ��
//
extern UINT WM_TASKBARCREATED;

