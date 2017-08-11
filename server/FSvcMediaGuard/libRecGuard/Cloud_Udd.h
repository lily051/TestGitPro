#ifndef GUARD_CLOUD_UDD_H
#define GUARD_CLOUD_UDD_H

/**	@file    Cloud_Udd.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   
*
*	@author	 xuezhihu
*	@date	 2013/2/1
*
*	@note    �����note��warningΪ��ѡ��Ŀ
*	@note    ������д���ļ�����ϸ����������ע��
*	@note    ��ʷ��¼��
*	@note    V0.0.1  ����
*
*	@warning ������д���ļ���صľ�����Ϣ
*/

#include <algorithm>
#include <vector>
#include <fclib/fcTchar.h>

typedef struct _cloud_pool_item_t 
{
	unsigned int PoolID;	               // ¼���ID
	std::_tstring PoolName;	               // ¼�������
	unsigned int FreeSize;                 // ¼���ʣ��ռ��С[��λ: MB]
	unsigned int PoolSize;                 // ¼��ش�С [��λ: MB]

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

