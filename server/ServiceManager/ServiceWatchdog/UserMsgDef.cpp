/*****************************************************
*  Copyright 2009-2010 Hikvision Digital Technology Co., Ltd.
*
*  FileName:
*      UserMsgDef.cpp
*
*  Description:
*      User Message Definition .
*
*  Modification History:
*    <version>    <time>        <author>      <desc>
*    1.0          2009-11-23    Hu Tonghai    Create
*****************************************************/

#include "stdafx.h"

//
// Summary:
//     响应任务栏重建消息。
//
UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));
