#ifndef GUARD_FCDBCOMMON_H
#define GUARD_FCDBCOMMON_H

/**	@file    fcDBCommon.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   数据库通用内容
 *
 *	@author	 xuezhihu
 *	@date	 2013/4/15
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V0.0.1  创建
 *
 *	@warning 这里填写本文件相关的警告信息
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
