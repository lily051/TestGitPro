#ifndef GUARD_CLOUD_UDD_H
#define GUARD_CLOUD_UDD_H

/**	@file    Cloud_Udd.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   
*
*	@author	 xuezhihu
*	@date	 2013/2/1
*
*	@note    下面的note和warning为可选项目
*	@note    这里填写本文件的详细功能描述和注解
*	@note    历史记录：
*	@note    V0.0.1  创建
*
*	@warning 这里填写本文件相关的警告信息
*/

#include <algorithm>
#include <vector>
#include <fclib/fcTchar.h>

typedef struct _cloud_pool_item_t 
{
	unsigned int PoolID;	               // 录像池ID
	std::_tstring PoolName;	               // 录像池名称
	unsigned int FreeSize;                 // 录像池剩余空间大小[单位: MB]
	unsigned int PoolSize;                 // 录像池大小 [单位: MB]

	bool operator<(const _cloud_pool_item_t &rightObj) const
	{
		return PoolID < rightObj.PoolID;
	}
}cloud_pool_item_t;

class CCloudPools
{
public:
	typedef cloud_pool_item_t item_type;
	typedef const item_type & const_item_reference;
	typedef std::vector<item_type> container_type;
	typedef container_type::size_type size_type;

public:
	CCloudPools(void)
	{}

	void Add(unsigned int PoolID, const char *PoolName, unsigned int FreeSize, 
		unsigned int PoolSize)
	{
		m_items.push_back(item_type());
		item_type &item = m_items.back();
		item.PoolID = PoolID;
		item.PoolName = fcA2T(PoolName);
		item.FreeSize = FreeSize;
		item.PoolSize = PoolSize;
	}
	size_type Size(void) const
	{
		return m_items.size();
	}
	const_item_reference operator[](size_type _Off) const
	{	
		return m_items[_Off];
	}
	void Sort(void)
	{   
		std::sort(m_items.begin(), m_items.end());
	}

private:
	container_type m_items;
};

#endif // end of GUARD_CLOUD_UDD_H

