/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月25日 上午10:24:06
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.tree.service;

import com.hikvision.finance.fwork.util.tree.TreeNode;





/**
 * <p></p>
 * @author jinxindong 2016年2月25日 上午10:24:06
 * @version V1.0 
 */
public interface ITreeService {
	
	/**
	 * 获取左侧的组织树
	 * @author jinxindong 2016年2月25日 上午10:27:18
	 * @return
	 */
	public TreeNode getRegionTree();
	
	/**
	 * 获取完整组织的树（根节点是控制单元）
	 * @author jinxindong 2016年3月5日 下午1:05:55
	 * @return
	 */
	public	TreeNode getAllRegionTree();
	
	/**
	 * 获取选中组织的树
	 * @author jinxindong 2016年3月5日 下午1:12:40
	 * @param userId
	 * @return
	 */
	public	TreeNode getRegionTreeByUserId(Integer userId);
	
	/**
	 * 获取全部的功能权限树
	 * @author jinxindong 2016年3月5日 下午1:51:11
	 * @return
	 */
	public TreeNode getALLPowerTree();
	
	/**
	 * 获取理财经理的功能权限树
	 * @author jinxindong 2016年4月28日 下午4:33:20
	 * @return
	 */
	public TreeNode getCollectPowerTree();
	
	/**
	 * 获取角色选中的功能权限树
	 * @author jinxindong 2016年3月5日 下午1:51:20
	 * @param roleId
	 * @return
	 */
	public TreeNode getPowerTreeByRoleId(Integer roleId);
	
	/**
	 * 获取理财经理角色选中的功能权限树
	 * @author jinxindong 2016年4月28日 下午4:34:05
	 * @param roleId
	 * @return
	 */
	public TreeNode getCollectTreeByRoleId(Integer roleId);
	
	/**
	 *  异步加载监控点树
	 * @author jinxindong 2016年3月10日 下午2:30:10
	 * @param strId
	 * @return
	 */
	public TreeNode getAllChannelTree(String strId);
	
	/**
	 * 异步加载已勾选的监控点树
	 * @author jinxindong 2016年3月10日 下午2:30:16
	 * @param strId
	 * @param userId
	 * @return
	 */
	public TreeNode getChannelTreeByUserId(String strId,Integer userId);
	
	/**
	 * 异步加载初始化一级树
	 * @author jinxindong 2016年3月10日 下午12:36:07
	 * @return
	 */
	public TreeNode getInitChannelTree();
	
	/**
	 * 获取产品类型树
	 * @author jinxindong 2016年3月21日 下午2:39:13
	 * @return
	 */
	public TreeNode getProductTypeTree();
	
	/**
	 * 异步加载产品树 
	 * @author jinxindong 2016年3月23日 下午7:23:56
	 * @param strId
	 * @return
	 */
	public TreeNode getProductTree(String strId);

	/**
	 * 异步加载已选产品的树
	 * @author jinxindong 2016年3月23日 下午7:24:20
	 * @param strId
	 * @param certId
	 * @return
	 */
	public TreeNode getProductTreeByCertId(String strId,Integer certId);
	
	/**
	 * 加载当前用户的设备树
	 * @author jinxindong 2016年4月6日 下午4:07:02
	 * @return
	 */
	public TreeNode getDeviceTree();
	
	/**
	 * 同步加载有权限的产品树，用于理财经理选择产品
	 * @author jinchenwu 2016年4月21日 下午7:16:31
	 * @return
	 */
	public TreeNode getPermitProductTree();
	
}
