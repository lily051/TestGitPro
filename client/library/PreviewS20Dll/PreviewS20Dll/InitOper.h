/**	@file    InitOper.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   初始化操作对象类
 *
 *	@author	 Liu Ronghua
 *	@date	 2013/01/04
 *	@note   
 *	@note    历史记录：
 *	@note    V1.0  create at 2013/01/04
 */
#pragma once

class CInitOper
{
private:
    BOOL m_bInit;       ///< 是否已完成初始化

public:
    CInitOper(void);
    ~CInitOper(void);

public:
    /** @fn         Init
     *  @brief      初始化
     *  @return     TRUE: 成功, FALSE: 失败
     */
    BOOL Init();

    /** @fn         UnInit
     *  @brief      反初始化
     *  @return     void
     */
    void UnInit();

    /** @fn         IsInited
     *  @brief      是否已完成初始化
     *  @return     TRUE: 是, FALSE: 否
     */
    BOOL IsInited();
};
