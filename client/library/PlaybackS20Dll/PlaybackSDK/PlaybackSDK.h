// PlaybackSDK.h : PlaybackSDK DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPlaybackSDKApp
// �йش���ʵ�ֵ���Ϣ������� PlaybackSDK.cpp
//

class CPlaybackSDKApp : public CWinApp
{
public:
	CPlaybackSDKApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};
