#pragma once

// PlaybackOCX3.0.h : PlaybackOCX3.0.DLL ����ͷ�ļ�

#if !defined( __AFXCTL_H__ )
#error "�ڰ������ļ�֮ǰ������afxctl.h��"
#endif

#include "resource.h"       // ������


// CPlaybackOCX30App : �й�ʵ�ֵ���Ϣ������� PlaybackOCX3.0.cpp��

class CPlaybackOCX30App : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

