// Cloud_Object.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CCloud_ObjectApp:
// 有关此类的实现，请参阅 Cloud_Object.cpp
//

class CCloud_ObjectApp : public CWinApp
{
public:
	CCloud_ObjectApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CCloud_ObjectApp theApp;