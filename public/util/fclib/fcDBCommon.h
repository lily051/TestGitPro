#ifndef GUARD_FCDBCOMMON_H
#define GUARD_FCDBCOMMON_H

/**	@file    fcDBCommon.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   ���ݿ�ͨ������
 *
 *	@author	 xuezhihu
 *	@date	 2013/4/15
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V0.0.1  ����
 *
 *	@warning ������д���ļ���صľ�����Ϣ
 */

#include "./fcTchar.h"


typedef struct _db_conn_info_t {
    std::_tstring host;
    std::_tstring port;
    std::_tstring username;
    std::_tstring password;
    std::_tstring dbschema;
}db_conn_info_t;

#endif // end of GUARD_FCDBCOMMON_H
