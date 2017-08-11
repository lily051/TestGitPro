#pragma once
#include "WaitWnd.h"
#include "TranslucentWnd.h"

class AFX_EXT_CLASS CFBusyWait
{
public:
    CFBusyWait(CWnd* pParent);
    ~CFBusyWait(void);

private:
    CWaitWnd *m_pWaitWnd;
    CTranslucentWnd *m_pTranslucentWnd;

public:
    void ShowBusyWait(LPCTSTR szText);
};
