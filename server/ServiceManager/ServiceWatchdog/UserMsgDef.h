#pragma once
//
// Copyright (c) 2009-2010 Hikvision Digital Technology Co., Ltd.
//
// Summary:
//     User Message Definition .
//
// Modification History:
//   2009-11-23:
//     V1.0 / 胡通海 / 创建
//

//
// Summary:
//     ServiceController Windows Service 状态变更消息。
//
#define WM_SERVICE_STATUS_CHANGED           (WM_USER + 100)
//
// Summary:
//     ServiceController 异常消息。
//
#define WM_SERVICE_CONTROL_COMMAND_ERROR    (WM_USER + 101)

//
// Summary:
//     托盘消息。
//
#define WM_ICON_NOTIFY                      (WM_USER + 102)

// 其他程序发来的退出程序的消息, ID 不可以改变！
#define WM_USER_CLOSE                       (WM_USER + 2025)
//
// Summary:
//     响应任务栏重建消息。
//
extern UINT WM_TASKBARCREATED;

