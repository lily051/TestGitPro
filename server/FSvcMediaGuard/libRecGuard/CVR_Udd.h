#ifndef GUARD_CVR_UDD_H
#define GUARD_CVR_UDD_H

/**	@file    CVR_Udd.h
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
//#include <hpr/include/HPR_Types.h>

typedef struct _cvr_group_item_t 
{
    int rvID; // ¼������
    std::_tstring name; // ¼�������
    int activeDevices; // ¼����Ӧ�Ļ������
    int totalDevices; // ¼����Ӧ���ܴ�����
    unsigned int gSizeHigh; // ¼����С�ĸ� 32 λ(���ֽ�Ϊ��λ)
    unsigned int gSizeLow; // ¼����С�ĵ� 32 λ(���ֽ�Ϊ��λ) 
    unsigned int UsedHigh; // ����¼����С�ĸ� 32 λ(���ֽ�Ϊ��λ)
    unsigned int UsedLow; // ����¼����С�ĵ� 32 λ(���ֽ�Ϊ��λ)
    int wdays; // ���ݱ������� 
    int dPolicy; // ¼����ǲ���
    bool operator<(const _cvr_group_item_t &rightObj) const
    {
        return rvID < rightObj.rvID;
    }
}cvr_group_item_t;

class CCVRGroups
{
public:
    typedef cvr_group_item_t item_type;
    typedef const item_type & const_item_reference;
    typedef std::vector<item_type> container_type;
    typedef container_type::size_type size_type;
    
public:
    CCVRGroups(void)
    {}

    void Add(int rvID, const char *rvName, int activeDevices,
        int totalDevices, unsigned int gSizeHigh, unsigned int gSizeLow,
        unsigned int UsedHigh, unsigned int UsedLow, int wdays,
        int dPolicy)
    {
        m_items.push_back(item_type());
        item_type &item = m_items.back();
        item.rvID = rvID;
        item.name = fcA2T(rvName);
        item.activeDevices = activeDevices;
        item.totalDevices = totalDevices;
        item.gSizeHigh = gSizeHigh;
        item.gSizeLow = gSizeLow;
        item.UsedHigh = UsedHigh;
        item.UsedLow = UsedLow;
        item.wdays = wdays;
        item.dPolicy = dPolicy;
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

#endif // end of GUARD_CVR_UDD_H

