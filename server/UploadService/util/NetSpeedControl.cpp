// implementation for NetSpeedControl.h
#include "stdafx.h"
//#include <crtdbg.h>
#include "NetSpeedControl.h"

void CNetSpeedControl::SpeedControl(DWORD dwDataSize)
{
    DWORD dwSleep = dwDataSize * 1000 / m_dwActulSpeed;
    if (dwSleep != 0)
    {
        Sleep(dwSleep);
    }
}
// 设置速度
void CNetSpeedControl::SetSpeed(DWORD nSpeed)
{
    //ASSERT(fSpeed > 0);
    if (nSpeed <= 0)
    {
        nSpeed = 512; // 默认速度是512K/s
    }
    m_dwActulSpeed = DWORD(nSpeed * 1024 / 8);
    //ASSERT(m_dwActulSpeed != 0);
    if (m_dwActulSpeed == 0)
    {
        m_dwActulSpeed = DWORD(512 * 1024 / 8); // use 512Kb/s as default 
    }
}
int CNetSpeedControl::NetSpeedControl(DWORD dwDataSize, DWORD nSpeed)
{
    DWORD dwAct = DWORD(nSpeed * 1024 / 8);
    DWORD dwSleep = dwDataSize * 1000 / dwAct;
    if (dwSleep != 0)
    {
        //TRACE("****************Sleep %d\n", dwSleep);
        Sleep(dwSleep);
    }
    return 0;
}
