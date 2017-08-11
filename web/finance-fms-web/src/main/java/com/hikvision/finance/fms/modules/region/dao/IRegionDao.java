/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月25日 上午9:54:45
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.region.dao;

import java.util.List;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.region.dto.RegionInfo;
import com.hikvision.finance.fms.modules.region.qo.RegionQo;


/**
 * <p></p>
 * @author jinxindong 2016年2月25日 上午9:54:45
 * @version V1.0 
 */
public interface IRegionDao extends IBaseDao<Region, RegionQo> {
	
	/**
	 * 根据ids删除组织
	 * @author jinxindong 2016年2月26日 下午2:07:01
	 * @param ids
	 * @return
	 */
	public boolean deleteRegByIds(List<Integer> ids);
	
	/**
	 * 获取组织分页数据
	 * @author jinxindong 2016年3月1日 上午9:51:31
	 * @param limit
	 * @param start
	 * @param keywords
	 * @return
	 */
	Page getDataPage(Integer limit, Integer start, RegionInfo regionInfo);
	
	/**
	 * 根据角色获取权限根节点
	 * @author jinxindong 2016年3月7日 上午9:16:50
	 * @param role
	 * @return
	 */
	public Region getRootOrg(Users user);
	
	/**
	 * 获取strpath匹配的节点
	 * @author jinxindong 2016年3月17日 下午6:13:22
	 * @param region
	 * @return
	 */
	public List<Region> getLikeStrPath(Region region);
	
	/**
	 * 获取匹配strpath的节点id
	 * @author jinchenwu 2016年4月13日 下午3:28:31
	 * @param region
	 * @return
	 */
    public List<Integer> getLikeStrPathIds(Region region);
    
    /**
     * 根据父节点获取 子孙节点的组织
     * @author jinxindong 2016年4月14日 下午6:42:12
     * @param region
     * @return
     */
	public List<Region> getChildStrPath(Region region);
	
	/**
	 * 获取ids
	 * @author jinxindong 2016年7月12日 上午10:23:48
	 * @param region
	 * @return
	 */
	public List<Integer> getIdsLikeStrPath(Region region);
	
	/**
	 * 导出组织表格
	 * @author jinxindong 2016年4月19日 下午4:48:44
	 * @param regionInfo
	 * @return
	 */
	public List<RegionInfo> exportRegionExcel(RegionInfo regionInfo);
	
	/**
	 * 获取数据库中网点的类型
	 * @author jinxindong 2016年4月22日 上午11:16:23
	 * @return
	 */
	public List<String> getNetName();
	
	/**
	 * 获取指定组织的当前层级
	 * @author jinxindong 2016年5月4日 下午3:15:41
	 * @param regionId
	 * @return
	 */
	public int getRegLevel(int regionId);
}
