#pragma once

// PreviewS20OCX.h : PreviewS20OCX.DLL ����ͷ�ļ�

#if !defined( __AFXCTL_H__ )
#error "�ڰ������ļ�֮ǰ������afxctl.h��"
#endif

#include "resource.h"       // ������


// CPreviewS20OCXApp : �й�ʵ�ֵ���Ϣ������� PreviewS20OCX.cpp��

class CPreviewS20OCXApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

