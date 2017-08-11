#ifndef GUARD_FCUNCOPYABLE_H
#define GUARD_FCUNCOPYABLE_H

/**	@file    fcUnCopyable.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   
 *
 *	@author	 xuezhihu
 *	@date	 2013/4/10
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V0.0.1  ����
 *
 *	@warning ������д���ļ���صľ�����Ϣ
 */

class CUnCopyable
{
protected:
    CUnCopyable(void) {}
    virtual ~CUnCopyable(void) {}
private:
    CUnCopyable(const CUnCopyable &);
    CUnCopyable &operator=(const CUnCopyable &);
};

#endif // end of GUARD_FCUNCOPYABLE_H
