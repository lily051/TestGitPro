// PBSkinRed.h : PBSkinRed DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "PBResource.h"		// ������


// CPBSkinRedApp
// �йش���ʵ�ֵ���Ϣ������� PBSkinRed.cpp
//

class CPBSkinRedApp : public CWinApp
{
public:
	CPBSkinRedApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
