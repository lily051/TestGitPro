#pragma once
#include "SplashDlg.h"

class CSplash
{
public:
	CSplash(CSplashDlg * pDlg);
	~CSplash(void);

private:
	CSplashDlg * m_pSplashDlg;
};
