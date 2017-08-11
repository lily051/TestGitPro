/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月25日 上午9:34:08
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.tree.action;

import com.hikvision.finance.fms.common.tree.service.ITreeService;
import com.hikvision.finance.fms.modules.product.dto.CrtificateInfo;
import com.hikvision.finance.fms.modules.role.dto.RoleInfo;
import com.hikvision.finance.fms.modules.user.dto.UserInfo;
import com.hikvision.finance.fwork.util.tree.TreeNode;
import com.hikvision.finance.fwork.www.action.BaseAction;


/**
 * <p>全部tree的请求汇总，提高可复用性和统一权限控制</p>
 * @author jinxindong 2016年2月25日 上午9:34:08
 * @version V1.0 
 */
public class TreeAction extends BaseAction<TreeNode>{
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 2397787472759779032L;
	private ITreeService treeService;
	private RoleInfo roleInfo;
	private UserInfo usersInfo;
	private String strId ;
	private CrtificateInfo crtificateInfo;
	/**
	 * 获取左侧的组织树
	 * 通过seeion中的根节点获取，并组建树
	 * @author jinxindong 2016年2月25日 上午10:18:26
	 * @return
	 */
	public String getRegionTree() {
		treeNode = treeService.getRegionTree();
		return TREE;
	}
	/**
	 * 获取完整组织的树（根节点是控制单元）
	 * @author jinxindong 2016年3月5日 下午1:08:02
	 * @return
	 */
	public String getAllRegionTree() {
		treeNode = treeService.getAllRegionTree();
		return TREE;
	}
	
	/**
	 * 获取选中组织的树
	 * @author jinxindong 2016年3月5日 下午1:08:28
	 * @return
	 */
	public String getRegionTreeByUserId() {
		treeNode = treeService.getRegionTreeByUserId(usersInfo.getId());
		return TREE;
	}
	
	/**
	 * 加载完整的功能权限树
	 * @author jinxindong 2016年2月29日 下午3:31:43
	 * @return
	 */
	public String getALLPowerTree() {
		treeNode = treeService.getALLPowerTree();
		return TREE;
	}
	
	/**
	 * 加载角色的功能权限树
	 * @author jinxindong 2016年2月29日 下午3:31:37
	 * @return
	 */
	public String getPowerTreeByRoleId() {
		treeNode = treeService.getPowerTreeByRoleId(roleInfo.getId());
		return TREE;
	}
	
	/**
	 * 加载完整的理财经理功能权限树
	 * @author jinxindong 2016年4月28日 下午4:32:33
	 * @return
	 */
	public String getCollectPowerTree() {
		treeNode = treeService.getCollectPowerTree();
		return TREE;
	}
	
	/**
	 * 加载理财经理角色的功能权限树
	 * @author jinxindong 2016年4月28日 下午4:32:38
	 * @return
	 */
	public String getCollectTreeByRoleId() {
		treeNode = treeService.getCollectTreeByRoleId(roleInfo.getId());
		return TREE;
	}
	/**
	 * 异步加载初始化一级树
	 * @author jinxindong 2016年3月10日 下午12:35:07
	 * @return
	 */
	public String getInitChannelTree(){
		if(strId==null){
			treeNode = treeService.getInitChannelTree();
		}else{
			treeNode = treeService.getAllChannelTree(strId);
		}
		list = treeNode.getChildren();
		return "treeList";
	}
	
	/**
	 * 权限管理模块加载理财经理监控点树
	 * @author jinxindong 2016年3月9日 下午4:10:46
	 * @return
	 */
	public String getChannelTreeByUserId(){
		treeNode = treeService.getChannelTreeByUserId(strId,usersInfo.getId());
		list = treeNode.getChildren();
		return "treeList";
	}
	
	/**
	 * 加载完整的产品类型树
	 * @author jinxindong 2016年3月28日 下午2:30:32
	 * @return
	 */
	public String getProductTypeTree() {
		treeNode = treeService.getProductTypeTree();
		return TREE;
	}
	
	/**
	 * 异步加载产品树
	 * @author jinxindong 2016年3月28日 下午2:30:37
	 * @return
	 */
	public String getProductTree(){
		treeNode = treeService.getProductTree(strId);
		list = treeNode.getChildren();
		return "treeList";
	}
	
	/**
	 * 异步加载已经勾选的产品树
	 * @author jinxindong 2016年3月28日 下午2:30:47
	 * @return
	 */
	public String getProductTreeByCertId(){
		treeNode = treeService.getProductTreeByCertId(strId,crtificateInfo.getId());
		list = treeNode.getChildren();
		return "treeList";
	}
	
	/**
	 * 加载设备树
	 * @author jinxindong 2016年4月6日 下午4:06:06
	 * @return
	 */
	public String getDeviceTree() {
		treeNode = treeService.getDeviceTree();
		return TREE;
	}

	/**
	 * 同步加载有权限的产品树，用于理财经理选择产品
	 * @author jinchenwu 2016年4月21日 下午6:24:26
	 * @return
	 */
	public String getPermitProductTree() {
		treeNode = treeService.getPermitProductTree();
		return TREE;
	}
	
	//======================getter/setter==========================

	
    public ITreeService getTreeService() {
    	return treeService;
    }

	
    public void setTreeService(ITreeService treeService) {
    	this.treeService = treeService;
    }

	
    public RoleInfo getRoleInfo() {
    	return roleInfo;
    }

	
    public void setRoleInfo(RoleInfo roleInfo) {
    	this.roleInfo = roleInfo;
    }
	
    public UserInfo getUsersInfo() {
    	return usersInfo;
    }
	
    public void setUsersInfo(UserInfo usersInfo) {
    	this.usersInfo = usersInfo;
    }
	
    public String getStrId() {
    	return strId;
    }
	
    public void setStrId(String strId) {
    	this.strId = strId;
    }
	
    public CrtificateInfo getCrtificateInfo() {
    	return crtificateInfo;
    }
	
    public void setCrtificateInfo(CrtificateInfo crtificateInfo) {
    	this.crtificateInfo = crtificateInfo;
    }
	
	


	
}
