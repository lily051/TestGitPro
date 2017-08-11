#ifndef GUARD_FCUNCOPYABLE_H
#define GUARD_FCUNCOPYABLE_H

/**	@file    fcUnCopyable.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   
 *
 *	@author	 xuezhihu
 *	@date	 2013/4/10
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V0.0.1  创建
 *
 *	@warning 这里填写本文件相关的警告信息
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
