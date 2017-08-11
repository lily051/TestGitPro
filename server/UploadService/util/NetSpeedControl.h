/**	@file    NetSpeedControl.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   这里填写本文件的摘要。
 *
 *	@author	 Zhihu Xue
 *	@date	 2010/9/15
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V2.0.0  添加了一个导出接口
 *
 *	@warning 这里填写本文件相关的警告信息
 */

#ifndef GUARD_NetSpeedControl_H
#define GUARD_NetSpeedControl_H

// 控制速度类
class CNetSpeedControl
{
public:
    // 传入网络带宽，单位Mb
    CNetSpeedControl(DWORD dSpeed = 0.5)
        : m_dwActulSpeed(0)
    {
        SetSpeed(dSpeed);
    }
    // 数据大小，单位Byte
    void SpeedControl(DWORD dwDataSize);
    // 设置速度
    void SetSpeed(DWORD dSpeed);
    static int NetSpeedControl(DWORD dwDataSize, DWORD dSpeed);
private:
    DWORD m_dwActulSpeed;
};


#endif // end of GUARD_NetSpeedControl_H
