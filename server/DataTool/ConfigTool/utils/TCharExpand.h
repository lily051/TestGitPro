#ifndef GUARD_TCHAREXPAND_H
#define GUARD_TCHAREXPAND_H
#pragma once

/**	@file    TCharExpand.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   ������д���ļ���ժҪ��
 *
 *	@author	 xuezhihu
 *	@date	 2012/3/15
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V2.0.0  �����һ�������ӿ�
 *
 *	@warning ������д���ļ���صľ�����Ϣ
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
