/**	@file    NetSpeedControl.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   ������д���ļ���ժҪ��
 *
 *	@author	 Zhihu Xue
 *	@date	 2010/9/15
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V2.0.0  �����һ�������ӿ�
 *
 *	@warning ������д���ļ���صľ�����Ϣ
 */

#ifndef GUARD_NetSpeedControl_H
#define GUARD_NetSpeedControl_H

// �����ٶ���
class CNetSpeedControl
{
public:
    // �������������λMb
    CNetSpeedControl(DWORD dSpeed = 0.5)
        : m_dwActulSpeed(0)
    {
        SetSpeed(dSpeed);
    }
    // ���ݴ�С����λByte
    void SpeedControl(DWORD dwDataSize);
    // �����ٶ�
    void SetSpeed(DWORD dSpeed);
    static int NetSpeedControl(DWORD dwDataSize, DWORD dSpeed);
private:
    DWORD m_dwActulSpeed;
};


#endif // end of GUARD_NetSpeedControl_H
