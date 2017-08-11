#ifndef GUARD_CVR_UDD_H
#define GUARD_CVR_UDD_H

/**	@file    CVR_Udd.h
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
//#include <hpr/include/HPR_Types.h>

typedef struct _cvr_group_item_t 
{
    int rvID; // 录像卷序号
    std::_tstring name; // 录像卷名称
    int activeDevices; // 录像卷对应的活动磁盘数
    int totalDevices; // 录像卷对应的总磁盘数
    unsigned int gSizeHigh; // 录像卷大小的高 32 位(以字节为单位)
    unsigned int gSizeLow; // 录像卷大小的低 32 位(以字节为单位) 
    unsigned int UsedHigh; // 已用录像卷大小的高 32 位(以字节为单位)
    unsigned int UsedLow; // 已用录像卷大小的低 32 位(以字节为单位)
    int wdays; // 数据保留天数 
    int dPolicy; // 录像卷覆盖策略
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

