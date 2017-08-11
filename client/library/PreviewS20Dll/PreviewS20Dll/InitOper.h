/**	@file    InitOper.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   ��ʼ������������
 *
 *	@author	 Liu Ronghua
 *	@date	 2013/01/04
 *	@note   
 *	@note    ��ʷ��¼��
 *	@note    V1.0  create at 2013/01/04
 */
#pragma once

class CInitOper
{
private:
    BOOL m_bInit;       ///< �Ƿ�����ɳ�ʼ��

public:
    CInitOper(void);
    ~CInitOper(void);

public:
    /** @fn         Init
     *  @brief      ��ʼ��
     *  @return     TRUE: �ɹ�, FALSE: ʧ��
     */
    BOOL Init();

    /** @fn         UnInit
     *  @brief      ����ʼ��
     *  @return     void
     */
    void UnInit();

    /** @fn         IsInited
     *  @brief      �Ƿ�����ɳ�ʼ��
     *  @return     TRUE: ��, FALSE: ��
     */
    BOOL IsInited();
};
