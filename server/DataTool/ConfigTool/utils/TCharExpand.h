#ifndef GUARD_TCHAREXPAND_H
#define GUARD_TCHAREXPAND_H
#pragma once

/**	@file    TCharExpand.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   这里填写本文件的摘要。
 *
 *	@author	 xuezhihu
 *	@date	 2012/3/15
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V2.0.0  添加了一个导出接口
 *
 *	@warning 这里填写本文件相关的警告信息
 */
#if (defined _UNICODE) || (defined UNICODE)
#include <iostream>
#include <string>
#else
#include <iostream>
#include <string>
#endif


#if (defined _UNICODE) || (defined UNICODE)
#define _tcin    wcin
#define _tstring wstring
#else
#define _tcin    cin
#define _tstring string
#endif



#endif // end of GUARD_TCHAREXPAND_H
