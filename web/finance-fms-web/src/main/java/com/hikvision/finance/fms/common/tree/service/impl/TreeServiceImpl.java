/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月25日 上午10:24:40
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.tree.service.impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.google.common.collect.Maps;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.fms.common.model.MenuBean;
import com.hikvision.finance.fms.common.model.UserSession;
import com.hikvision.finance.fms.common.tree.service.ITreeService;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.SessionAttrbutes;
import com.hikvision.finance.fms.common.util.Constants.TreeConstants;
import com.hikvision.finance.fms.common.util.SessionUtil;
import com.hikvision.finance.fms.model.Device;
import com.hikvision.finance.fms.model.Devicechannel;
import com.hikvision.finance.fms.model.Privilege;
import com.hikvision.finance.fms.model.Product;
import com.hikvision.finance.fms.model.Producttype;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.modules.device.dao.IDeviceChannelDao;
import com.hikvision.finance.fms.modules.device.dao.IDeviceDao;
import com.hikvision.finance.fms.modules.product.dao.ICertProductMappingDao;
import com.hikvision.finance.fms.modules.product.dao.IProductDao;
import com.hikvision.finance.fms.modules.product.dao.IProductTypeDao;
import com.hikvision.finance.fms.modules.product.service.IProductService;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fms.modules.role.dao.IPrivilegeDao;
import com.hikvision.finance.fms.modules.role.dao.IRolePrivilegeMappingDao;
import com.hikvision.finance.fms.modules.user.dao.IUsercrtificatemappingDao;
import com.hikvision.finance.fms.modules.user.dao.IUserdevicemappingDao;
import com.hikvision.finance.fms.modules.user.dao.IUserregionmappingDao;
import com.hikvision.finance.fms.modules.user.dao.IUserrolemappingDao;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.tree.TreeNode;
import com.hikvision.finance.fwork.www.session.HikSession;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年2月25日 上午10:24:40
 * @version V1.0
 */
public class TreeServiceImpl implements ITreeService {
	private IProductService productService;
	private IRegionDao regionDao;
	private IRolePrivilegeMappingDao rolePrivilegeMappingDao;
	private IUserregionmappingDao userregionmappingDao;
	private IUserrolemappingDao userrolemappingDao;
	private IUserdevicemappingDao userdevicemappingDao;
	private IDeviceDao deviceDao;
	private IDeviceChannelDao deviceChannelDao;
	private IProductTypeDao productTypeDao;
	private IProductDao productDao;
	private IUsercrtificatemappingDao usercrtificatemappingDao;
	private ICertProductMappingDao certProductMappingDao;
	private IPrivilegeDao  privilegeDao;
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.common.tree.service.ITreeService#getOrgTree()
	 */
	@Override
	public TreeNode getRegionTree() {
		TreeNode tree = new TreeNode();
		//1、通过session查询系统角色类型是管理员还是自定义角色
		UserSession userSession = SessionUtil.getUserSession();
		//如果是管理员
		if(userSession.isAdmin()){
			// 创建树并生成根节点
			tree.setId(TreeConstants.PREFIX_REG + Constants.ROOT_REG_PARENT);
			//tree.setLabel(regionDao.get(Constants.ROOT_REG_ID).getStrName());
		}else{
			//如果是自定义角色，session获取该用户的根节点
			tree.setId(TreeConstants.PREFIX_REG + regionDao.get(userSession.getRootOrgId()).getNparentId());		
			//tree.setLabel(regionDao.get(userSession.getRootOrgId()).getStrName());
		}		
		
		//2,查询所有组织（可以改成通过根节点去取组织树）
		List<Region> regs = regionDao.getAll();
		
		// 3,获取组织Id与其直接子组织的Map集合
		Map<Integer, List<Region>> regsqoMapChildren = getRegMapChildren(regs);
		
		// 4, 递归构建树
		buildRegChildNode(tree, regsqoMapChildren,null,userSession.getRootOrgId());
		
		//返回被修饰过的tree（TreeNode和他的所有子节点）
		if (tree.getChildren() != null) {
			tree = tree.getChildren().get(0);
		}
		return tree;
	}
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.common.tree.service.ITreeService#getAllRegionTree()
     */
    @Override
    public TreeNode getAllRegionTree() {
		TreeNode tree = new TreeNode();
		//1、如果是管理员
		// 创建树并生成根节点
		tree.setId(TreeConstants.PREFIX_REG + Constants.ROOT_REG_PARENT);		
		//2,查询所有组织（可以改成通过根节点去取组织树）
		List<Region> regs = regionDao.getAll();
		
		// 3,获取组织Id与其直接子组织的Map集合
		Map<Integer, List<Region>> regsqoMapChildren = getRegMapChildren(regs);
		
		// 4, 递归构建树
		buildRegChildNode(tree, regsqoMapChildren,null,null);
		
		//返回被修饰过的tree（TreeNode和他的所有子节点）
		if (tree.getChildren() != null) {
			tree = tree.getChildren().get(0);
		}
		return tree;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.common.tree.service.ITreeService#getRegionTreeByUserId()
     */
    @Override
    public TreeNode getRegionTreeByUserId(Integer userId) {
    	List<String> checkedIds = new ArrayList<String>();
    	List<Integer> checkedId = userregionmappingDao.getUserRegionCode(userId);//获取被选中的region的id
    	for(Integer cid:checkedId){
    		checkedIds.add(TreeConstants.PREFIX_REG+cid);
    	}
    	
		TreeNode tree = new TreeNode();
		//1、如果是管理员
		// 创建树并生成根节点
		tree.setId(TreeConstants.PREFIX_REG + Constants.ROOT_REG_PARENT);		
		//2,查询所有组织（可以改成通过根节点去取组织树）
		List<Region> regs = regionDao.getAll();
		
		// 3,获取组织Id与其直接子组织的Map集合
		Map<Integer, List<Region>> regsqoMapChildren = getRegMapChildren(regs);
		
		// 4, 递归构建树
		buildRegChildNode(tree, regsqoMapChildren,checkedIds,null);
		
		//返回被修饰过的tree（TreeNode和他的所有子节点）
		if (tree.getChildren() != null) {
			tree = tree.getChildren().get(0);
		}
		return tree;
    }
    
	/** ---------------------------- 组织树的私有方法 start-------------------------------- **/
	/**
	 * 组装Map<父节点id1，[子节点1，子节点2]；父节点id2，[子节点1，子节点2]>
	 * 父节点id为key，value是所有子节点的list组合
	 * @author jinxindong 2016年2月25日 上午10:55:45
	 * @param orgs
	 * @return
	 */
	private Map<Integer, List<Region>> getRegMapChildren(List<Region> regs) {
		Map<Integer, List<Region>> regMapChildren = Maps.newHashMap();
		for (Region reg : regs) {
			Integer parentId = reg.getNparentId();
			List<Region> list = regMapChildren.get(parentId);
			if (list == null) {
				list = new ArrayList<Region>();
				regMapChildren.put(parentId, list);
			}
			list.add(reg);
		}
		return regMapChildren;
	}
	
	/**
	 * 构建组织树子节点
	 * @author jinxindong 2016年2月25日 上午10:59:55
	 * @param parent
	 * @param orgMapChildren
	 */
	private void buildRegChildNode(TreeNode parent, Map<Integer, List<Region>> regMapChildren,List<String> checkedIds,Integer rootOrgId) {
		Integer parentId = Integer.parseInt(parent.getId().split("_")[1]);//自己设置的id是一个reg_0的形式
		List<Region> children = regMapChildren.get(parentId);//通过初始id，去map中寻找子节点的list集合
		List<String> strPathList = new ArrayList<String>();
		if(checkedIds!=null && checkedIds.size()>0 ){//编辑展开操作
			for(String checkedId:checkedIds){
				Integer regionId = Integer.parseInt(checkedId.split("_")[1]);
				Region region = regionDao.get(regionId);
				strPathList.add(region.getStrPath());
			}
		}
		
		if (children != null && children.size() > 0) {
			for (Region child : children) {
				if(rootOrgId!=null){
					if(!child.getId().equals(rootOrgId)){
						continue;
					}
				}
				TreeNode childNode = buildTreeNode(child, checkedIds);//将节点实体转化为TreeNode类型（设置前段需要的属性）
				childNode.setParentNode(parent);//给子节点设置父节点实体
				childNode.setParentId(parent.getId());//给字节点设置父节点id
				if (checkedIds != null) {// 编辑打开操作
					for(String strPath:strPathList){
						if(strPath.contains(child.getStrPath())){//如果包含，说明要打开
							childNode.setOpen(true);
						}						
					}					
				}
				// 递归创建子节点
				buildRegChildNode(childNode, regMapChildren,checkedIds,null);
				parent.addChildren(childNode);
			}
		}
	}
	
	/**
	 * 创建组织类型的树节点
	 * 	private boolean chkDisabled = false; // 选择禁用
		private boolean isHidden;            // 节点是否隐藏
		private boolean open = false;        // 是否展开节点
	 * @author jinxindong 2016年2月25日 上午11:01:25
	 * @param child
	 * @param checkedId
	 * @return
	 */
	private TreeNode buildTreeNode(Region child, List<String> checkedId) {
		TreeNode node = new TreeNode();
		String strId = String.valueOf(child.getId());
		strId = TreeConstants.PREFIX_REG + strId;
		if (child.getId()==Constants.ROOT_REG_ID) {
			node.setCls("home");//设置图标
		} else {
			node.setCls("reg");
		}
		node.setId(strId);// ID
		node.setLabel(child.getStrName());//节点名字显示
		node.putExtra("strCode", child.getStrCode());//区域编码
		node.putExtra("nsortId", child.getNsortId());//额外的属性列表
		node.putExtra("hasCheck", false);//额外的属性列表
		node.putExtra("strAreaCode", child.getStrInterAreaCode());//额外的属性列表
		node.setChecked(checkedId != null && checkedId.contains(node.getId()));// 判断是否勾选,设置勾选属性
		if(checkedId != null && checkedId.contains(node.getId())){
			node.setOpen(true);
		}
		return node;
	}
	
	/** ---------------------------- 组织树的私有方法end -------------------------------- **/
	
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.common.tree.service.ITreeService#getALLPowerTree()
     */
    @Override
    public TreeNode getALLPowerTree() {
	    return getALLPowerTree(null);
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.common.tree.service.ITreeService#getPowerTreeByRoleId(java.lang.Integer)
     */
    @Override
    public TreeNode getPowerTreeByRoleId(Integer roleId) {
    	List<String> checkedIds = rolePrivilegeMappingDao.getRolePowerCode(roleId);
		return getALLPowerTree(checkedIds);
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.common.tree.service.ITreeService#getCollectPowerTree()
     */
    @Override
    public TreeNode getCollectPowerTree() {
	    return getCollectPowerTree(null);
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.common.tree.service.ITreeService#getCollectTreeByRoleId(java.lang.Integer)
     */
    @Override
    public TreeNode getCollectTreeByRoleId(Integer roleId) {
    	List<String> checkedIds = rolePrivilegeMappingDao.getRolePowerCode(roleId);
    	return getCollectPowerTree(checkedIds);
    }
    /** ---------------------------- 功能权限树的私有方法start -------------------------------- **/
    /**
     * 加载功能-模块-子模块树，并附加节点在加载出后直接勾上 
     * @author jinxindong 2016年3月5日 下午1:58:16
     * @param checkedIds
     * @return
     */
    private TreeNode getALLPowerTree(List<String> checkedIds){
    	TreeNode root = new TreeNode();
		root.setLabel("功能权限");
		root.setCls("power");
		root.setOpen(true);
		HikSession session = SessionUtil.getHikSession();
		@SuppressWarnings("unchecked")
		List<MenuBean> menus = (List<MenuBean>)session.getAttribute(SessionAttrbutes.MENUS);
		buildMenuTree(root, menus, checkedIds);
		return root;
    }
    
	/**
	 * 构建权限菜单树
	 * @author jinxindong 2016年3月5日 下午1:59:54
	 * @param parent
	 * @param menus
	 * @param checkedIds
	 */
	private void buildMenuTree(TreeNode parent, List<MenuBean> menus, List<String> checkedIds) {
		if (menus == null || menus.size() < 1) {
			return;
		}
		for (MenuBean menu : menus) {
			TreeNode node = new TreeNode();
			node.setId(menu.getMenuId());
			node.setLabel(menu.getName());
			// node.setCls("fun");
			buildMenuTree(node, menu.getChildren(), checkedIds);
			node.setChecked(checkedIds != null && checkedIds.contains(node.getId()));
			parent.addChildren(node);
		}
	}
	
	/**
	 * 加载功能-模块-子模块树，并附加节点在加载出后直接勾上 
	 * @author jinxindong 2016年4月28日 下午5:06:02
	 * @param checkedIds
	 * @return
	 */
    private TreeNode getCollectPowerTree(List<String> checkedIds){
    	TreeNode root = new TreeNode();
		root.setLabel("功能权限");
		root.setCls("power");
		root.setOpen(true);
		List<Privilege> powers  = privilegeDao.findBy(new String[] {"nenabled", "nsysNo"}, new Object[] {Constants.CONS_1,
		        Constants.CONS_1});
		List<MenuBean> menus = buildMenus(powers, "5");
		buildMenuTree(root, menus, checkedIds);
		return root;
    }
    
    /**
     * 构建menuBean
     * @author jinxindong 2016年4月28日 下午5:28:10
     * @param powers
     * @param parentCode
     * @return
     */
	private List<MenuBean> buildMenus(List<Privilege> powers, String parentCode) {
		List<MenuBean> subMenus = new ArrayList<MenuBean>();
		if (powers != null && powers.size() > 0) {
			for (Privilege power : powers) {
				if (power.getStrSuper().equals(parentCode)) {
					MenuBean menu = new MenuBean();
					menu.setMenuId(power.getStrCode());
					menu.setParentId(parentCode);
					menu.setName(power.getStrName());
					menu.setUrl(power.getStrHref());
					menu.setNbtype(power.getNbtype());
					menu.setChildren(buildMenus(powers, power.getStrCode()));
					subMenus.add(menu);
				}
			}
		}
		return subMenus;
	}
    /** ---------------------------- 功能权限树的私有方法 end-------------------------------- **/
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.common.tree.service.ITreeService#getInitChannelTree()
     */
    @Override
    public TreeNode getInitChannelTree() {
    	TreeNode tree = new TreeNode();
		//1、通过session查询系统角色类型是管理员还是自定义角色
		UserSession userSession = SessionUtil.getUserSession();
		//如果是管理员
		if(userSession.isAdmin()){
			// 创建树并生成根节点
			tree.setId(TreeConstants.PREFIX_REG + Constants.ROOT_REG_PARENT);
		}else{
			//如果是自定义角色，session获取该用户的根节点
			//tree.setId(TreeConstants.PREFIX_REG + userSession.getRootOrgId());
			tree.setId(TreeConstants.PREFIX_REG + regionDao.get(userSession.getRootOrgId()).getNparentId());
		}
		//查询所有组织（可以改成通过根节点去取组织树）
		List<Region> regs = regionDao.getAll();		
		
    	// 获取全部信道
    	List<Devicechannel>  deviceChannelList = deviceChannelDao.getAll();
		if (deviceChannelList == null || deviceChannelList.size() < 1) {
			throw new ExpectedException("", "没有可用的通道");			
		}
		
		// 获取组织Id与其直接子组织的Map集合
		Map<Integer, List<Region>> regMapChildren = getRegMapChildren(regs);  
		
		//获取区域id和其直接子信道的Map集合
		Map<Integer, List<Devicechannel>> deviceChannelMapChildren = getChannelMapChildren(deviceChannelList);
		Map<Integer,Region> regionMap = getRegionMap(regs);
		Map<Integer,Devicechannel> devChannelMap = getDevChannelMap(deviceChannelList);
		buildDevChannelChildNode(tree, regMapChildren, deviceChannelMapChildren, null,regionMap,devChannelMap);
		
		return tree;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.common.tree.service.ITreeService#getAllChannelTree()
     */
    @Override
    public TreeNode getAllChannelTree(String strId) {
    	TreeNode tree = new TreeNode();
		tree.setId(strId);

		//查询所有组织（可以改成通过根节点去取组织树）
		List<Region> regs = regionDao.getAll();		
		
    	// 获取全部信道
    	List<Devicechannel>  deviceChannelList = deviceChannelDao.getAll();
		if (deviceChannelList == null || deviceChannelList.size() < 1) {
			throw new ExpectedException("", "没有可用的通道");			
		}
		
		// 获取组织Id与其直接子组织的Map集合
		Map<Integer, List<Region>> regMapChildren = getRegMapChildren(regs);  
		
		//获取设备id和其直接子信道的Map集合
		Map<Integer, List<Devicechannel>> deviceChannelMapChildren = getChannelMapChildren(deviceChannelList);
		Map<Integer,Region> regionMap = getRegionMap(regs);
		Map<Integer,Devicechannel> devChannelMap = getDevChannelMap(deviceChannelList);
		buildDevChannelChildNode(tree, regMapChildren, deviceChannelMapChildren, null,regionMap,devChannelMap);
		
		return tree;
    }
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.common.tree.service.ITreeService#getChannelTreeByUserId(java.lang.Integer)
     */
    @Override
	public TreeNode getChannelTreeByUserId(String strId, Integer userId) {
		
    	// 获取被选中的信道
		List<String> checkedIds = new ArrayList<String>();
		List<Integer> checkedId = userdevicemappingDao.getChannelByUserId(userId);
		for (Integer cid : checkedId) {
			checkedIds.add(TreeConstants.PREFIX_CHANNEL + cid);
		}
		
		if (strId == null) {// 节点Id为null，是控制单元
			UserSession userSession = SessionUtil.getUserSession();
			// 如果是管理员
			if (userSession.isAdmin()) {
				// 创建树并生成根节点
				strId = TreeConstants.PREFIX_REG + Constants.ROOT_REG_PARENT;
			} else {
				// 如果是自定义角色，session获取该用户的根节点
				//strId = TreeConstants.PREFIX_REG + userSession.getRootOrgId();
				strId = TreeConstants.PREFIX_REG + regionDao.get(userSession.getRootOrgId()).getNparentId();
				
			}
			
		}
		
		// 树节点
		TreeNode tree = new TreeNode();
		tree.setId(strId);
		
		// 查询所有组织（可以改成通过根节点去取组织树）
		List<Region> regs = regionDao.getAll();
		
		// 获取全部信道
		List<Devicechannel> deviceChannelList = deviceChannelDao.getAll();
		if (deviceChannelList == null || deviceChannelList.size() < 1) {
			throw new ExpectedException("", "没有可用的通道");			
		}
		
		// 获取组织Id与其直接子组织的Map集合
		Map<Integer, List<Region>> regMapChildren = getRegMapChildren(regs);
		
		// 获取设备id和其直接子信道的Map集合
		Map<Integer, List<Devicechannel>> deviceChannelMapChildren = getChannelMapChildren(deviceChannelList);
		
		// 获取regionid和region的Map集合
		Map<Integer,Region> regionMap = getRegionMap(regs);
		
		// 获取channelId和channel的Map集合
		Map<Integer,Devicechannel> devChannelMap = getDevChannelMap(deviceChannelList);
		
		buildDevChannelChildNode(tree, regMapChildren, deviceChannelMapChildren, checkedIds,regionMap,devChannelMap);
		
		return tree;
	}
    
    
    Map<Integer,Region> getRegionMap(List<Region> regs) {
		Map<Integer, Region> regionMap = Maps.newHashMap();
		for(Region region :regs){
			regionMap.put(region.getId(), region);
		}
		return regionMap;  	
    }
    
    Map<Integer,Devicechannel> getDevChannelMap(List<Devicechannel> deviceChannelList) {
    	Map<Integer, Devicechannel> devChannelMap = Maps.newHashMap();
    	for(Devicechannel devicechannel :deviceChannelList){
    		devChannelMap.put(devicechannel.getId(), devicechannel);
    	}
    	return devChannelMap;  	
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.common.tree.service.ITreeService#getDeviceTree()
     */
    @Override
	public TreeNode getDeviceTree() {
		TreeNode tree = new TreeNode();
		// 1、通过session查询系统角色类型是管理员还是自定义角色
		UserSession userSession = SessionUtil.getUserSession();
		// 如果是管理员
		if (userSession.isAdmin()) {
			// 创建树并生成根节点
			tree.setId(TreeConstants.PREFIX_REG + Constants.ROOT_REG_PARENT);
		} else {
			// 如果是自定义角色，session获取该用户的根节点
			//tree.setId(TreeConstants.PREFIX_REG + userSession.getRootOrgId());
			tree.setId(TreeConstants.PREFIX_REG + regionDao.get(userSession.getRootOrgId()).getNparentId());
			
		}
		// 2,查询所有组织（可以改成通过根节点去取组织树）
		List<Region> regs = regionDao.getAll();
		
		//3,获取组织Id与其直接子组织的Map集合
		Map<Integer, List<Region>> regMapChildren = getRegMapChildren(regs);
		
		//4、获取全部存储设备
		List<Device> deviceList = deviceDao.getAllStoDevice();
		if (deviceList == null || deviceList.size() < 1) {
			throw new ExpectedException("", "尚未添加储存设备");			
		}
		
		//5、获取区域id和其直接子设备的Map集合
		Map<Integer, List<Device>> deviceMapChildren = getDevMapChildren(deviceList);
		 
		//6、递归构建树
		buildDeviceChildNode(tree, regMapChildren, deviceMapChildren, null);
		 
		//返回被修饰过的tree（TreeNode和他的所有子节点）
		if (tree.getChildren() != null) {
			tree = tree.getChildren().get(0);
		}
		return tree;
	}
    /** ---------------------------- 监控点树的私有方法 start-------------------------------- **/
    
    /**
     * 信道封装
     * 封装成<区域id1,<信道1,信道1,信道2,信道3>;区域id2,<信道1,信道2,信道3>>
     * 组装Map<父节点id1，[子节点1，子节点2]；父节点id2，[子节点1，子节点2]>
     * @author jinxindong 2016年3月9日 上午11:05:34
     * @param channelList
     * @return
     */
    private Map<Integer, List<Devicechannel>> getChannelMapChildren(List<Devicechannel> channelList) {
		Map<Integer, List<Devicechannel>> channelMapChildren = Maps.newHashMap();
		for (Devicechannel channel : channelList) {
			Integer parentId = channel.getNregionId();
			List<Devicechannel> list = channelMapChildren.get(parentId);
			if (list == null) {
				list = new ArrayList<Devicechannel>();
				channelMapChildren.put(parentId, list);
			}
			list.add(channel);
		}
		return channelMapChildren;  	
    }
	/**
	 * 将信道转化为节点
	 * @author jinxindong 2016年3月9日 下午3:05:32
	 * @param child
	 * @param checkedId
	 * @return
	 */
	private TreeNode buildDevChannelTreeNode(Devicechannel child, List<String> checkedId) {
		TreeNode node = new TreeNode();
		String strId = String.valueOf(child.getId());
		strId = TreeConstants.PREFIX_CHANNEL + strId;
		String channelId = TreeConstants.PREFIX_CHANNEL + child.getId();
		node.setCls("devChannel");
		node.setId(strId);// ID
		node.setLabel(child.getNchannelName());//节点名字显示
		//node.setOpen(true);// 是否展开节点
		node.putExtra("devId", child.getNdeviceId());
		node.putExtra("devChannel", child.getId());//设备通道id
		node.setChecked(checkedId != null && checkedId.contains(channelId));// 判断是否勾选,设置勾选属性
		return node;
	}
	
    /**
     * 构建信道树
     * @author jinxindong 2016年3月9日 下午2:41:08
     * @param regMapChildren
     * @param deviceMapChildren
     * @param getChannelMapChildren
     * @param checkedIds
     */
	private void buildDevChannelChildNode(TreeNode parent, Map<Integer, List<Region>> regMapChildren,Map<Integer, List<Devicechannel>> deviceChannelMapChildren,
	        List<String> checkedIds,Map<Integer,Region> regionMap,Map<Integer,Devicechannel> devChannelMap ) {
		List<String> strPathList = new ArrayList<String>(); //id--strpath的map
		if(checkedIds!=null && checkedIds.size()>0 ){//编辑半选操作
			for(String checkedId:checkedIds){
				Integer nchannelId = Integer.parseInt(checkedId.split("_")[1]);//获取每个信道 的信道号
				Devicechannel devicechannel = devChannelMap.get(nchannelId);
				Integer regionId = devicechannel.getNregionId();//获取信道的上一级区域的id
				Region region = regionMap.get(regionId);
				strPathList.add(region.getStrPath());
			}
		}
		
		Integer parentId = Integer.parseInt(parent.getId().split("_")[1]);//自己设置的id是一个reg_0的形
		List<Region> children = regMapChildren.get(parentId);//通过初始id，去map中寻找子节点的list集合
		if (children != null && children.size() > 0) {
			for (Region child : children) {
				TreeNode childNode = buildTreeNode(child, null);//将节点实体转化为TreeNode类型（设置前段需要的属性）
				childNode.setParentNode(parent);//给子节点设置父节点实体
				childNode.setParentId(parent.getId());//给字节点设置父节点id
				if (checkedIds != null) {// 编辑半选操作
					for(String strPath:strPathList){
						if(strPath.contains(child.getStrPath())){//如果包含，说明要半选
							childNode.setHalfCheck(true);
						}						
					}
					
				}
				// 没有子节点的区域且没有子节点的信道需要被隐藏
				if(regMapChildren.get(Integer.parseInt(childNode.getId().split("_")[1]))==null &&
						deviceChannelMapChildren.get(Integer.parseInt(childNode.getId().split("_")[1]))==null){
					childNode.setIsHidden(true);
				}else if(!regionSonHasChannel(child, deviceChannelMapChildren)){//如果没有区域节点的子孙节点没有信道也要被隐藏
					childNode.setIsHidden(true);
				}else{					
					childNode.setIsParent(true);//反之需要添加可展开
				}
				parent.addChildren(childNode);
			}
		}
		// 2，构建设备子节点(信道节点)
		List<Devicechannel> devicechannels = deviceChannelMapChildren.get(parentId);
		if (devicechannels != null && devicechannels.size() > 0) {
			for (Devicechannel devicechannel : devicechannels) {
				TreeNode devicechannelNode = buildDevChannelTreeNode(devicechannel, checkedIds);
				devicechannelNode.setParentNode(parent);
				devicechannelNode.setParentId(parent.getId());
				parent.addChildren(devicechannelNode);
			}
		}
	}
	
	/**
	 * 判断区域节点的子孙节点没有信道
	 * @author jinxindong 2016年3月19日 上午11:02:56
	 * @param child
	 * @param deviceChannelMapChildren
	 * @return
	 */
	private Boolean regionSonHasChannel(Region child, Map<Integer, List<Devicechannel>> deviceChannelMapChildren) {
		Boolean hasChannel = false;
		// 通过strpath找到子孙节点
		List<Region> regions = regionDao.getLikeStrPath(child);
		// 遍历子孙节点是否在deviceChannelMapChildren中含有信道
		for (Region region : regions) {
			if (null != deviceChannelMapChildren.get(region.getId())) {
				hasChannel = true;
			}
		}
		return hasChannel;
	}
    /** ---------------------------- 监控点树的私有方法 end-------------------------------- **/
    
    /** ---------------------------- 设备树的私有方法 start-------------------------------- **/
    /**
     * 设备封装
     * 封装成<区域id1,<设备1,设备1,设备2,设备3>;区域id2,<设备1,设备2,设备3>>
     * 组装Map<父节点id1，[子节点1，子节点2]；父节点id2，[子节点1，子节点2]>
     * @author jinxindong 2016年3月9日 上午10:58:55
     * @param deviceList
     * @return
     */
    private Map<Integer, List<Device>> getDevMapChildren(List<Device> deviceList) {
		Map<Integer, List<Device>> devMapChildren = Maps.newHashMap();
		for (Device dev : deviceList) {
			Integer parentId = dev.getNregionId();
			List<Device> list = devMapChildren.get(parentId);
			if (list == null) {
				list = new ArrayList<Device>();
				devMapChildren.put(parentId, list);
			}
			list.add(dev);
		}
		return devMapChildren;  	
    }
    
	/**
	 * 将设备转化为节点
	 * @author jinxindong 2016年3月9日 下午3:05:32
	 * @param child
	 * @param checkedId
	 * @return
	 */
	private TreeNode buildDevTreeNode(Device child, List<String> checkedId) {
		TreeNode node = new TreeNode();
		String strId = String.valueOf(child.getId());
		strId = TreeConstants.PREFIX_DEV + strId;
		node.setCls("dev");
		node.setId(strId);// ID
		node.setLabel(child.getStrName());//节点名字显示
		node.setChecked(checkedId != null && checkedId.contains(node.getId()));// 判断是否勾选,设置勾选属性
		return node;
	}
	
    /**
     * 构建设备树
     * @author jinxindong 2016年3月9日 下午3:05:06
     * @param parent
     * @param regMapChildren
     * @param deviceMapChildren
     * @param checkedIds
     */
	private void buildDeviceChildNode(TreeNode parent, Map<Integer, List<Region>> regMapChildren,
	        Map<Integer, List<Device>> deviceMapChildren, List<String> checkedIds) {
		Integer parentId = Integer.parseInt(parent.getId().split("_")[1]);
		List<Region> children = regMapChildren.get(parentId);
		if (children != null && children.size() > 0) {
			for (Region child : children) {
				TreeNode childNode = buildTreeNode(child, checkedIds);
				childNode.setParentNode(parent);
				childNode.setParentId(parent.getId());
				// 递归创建子节点
				buildDeviceChildNode(childNode, regMapChildren, deviceMapChildren, checkedIds);
				// 没有子节点的区域不添加到父节点（前台不用展示）
				if (child.getNparentId() == Constants.ROOT_REG_PARENT
				        || (childNode.getChildren() != null && childNode.getChildren().size() > 0)) {
					parent.addChildren(childNode);
				}
			}
		}
		
		// 2，构建设备子节点
		List<Device> devices = deviceMapChildren.get(parentId);
		if (devices != null && devices.size() > 0) {
			for (Device device : devices) {
				TreeNode deviceNode = buildDevTreeNode(device, checkedIds);
				deviceNode.setParentNode(parent);
				deviceNode.setParentId(parent.getId());
				parent.addChildren(deviceNode);
			}
		}
	}
    
    /** ---------------------------- 设备树的私有方法 end-------------------------------- **/
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.common.tree.service.ITreeService#getProductTree()
     */
    @Override
    public TreeNode getProductTypeTree() {
		TreeNode tree = new TreeNode();

		// 创建树并生成根节点
		tree.setId(TreeConstants.PREFIX_PRODUCTTYPE + Constants.ROOT_PRO_PARENT);
	
		//2,查询所有产品类型
		List<Producttype> pros = productTypeDao.getAll();
		if (pros == null || pros.size() < 1) {
			throw new ExpectedException("", "尚未添加产品");			
		}
		
		// 3,获取组织Id与其直接子类型的Map集合
		Map<Integer, List<Producttype>> prosMapChildren = getProMapChildren(pros);
		
		// 4, 递归构建树
		buildProTypeChildNode(tree, prosMapChildren);
		
		//返回被修饰过的tree（TreeNode和他的所有子节点）
		if (tree.getChildren() != null) {
			tree = tree.getChildren().get(0);
		}
		return tree;
    }
    
    /** ---------------------------- 理财经理产品树 start-------------------------------- **/
    
    /*
     * (non-Javadoc)
     * @see com.hikvision.finance.fms.common.tree.service.ITreeService#getAllProductTree()
     */
	public TreeNode getPermitProductTree() {// TODO
		TreeNode tree = new TreeNode();
		List<Product> permitProductList = productService.getPermitProductList();
		Map<Integer, List<Object>> permitProductMap = new HashMap<Integer, List<Object>>();// 要显示的产品Map	√
		for (Product permitProduct : permitProductList) {
			if(!permitProductMap.containsKey(permitProduct.getPid())){
				List<Object> tempProductList = new ArrayList<Object>();
				tempProductList.add(permitProduct);
				permitProductMap.put(permitProduct.getPid(), tempProductList);
			}else{
				permitProductMap.get(permitProduct.getPid()).add(permitProduct);
			}
		}

		List<Producttype> allProductTypeList = productTypeDao.getAll();// 所有产品类型List
		List<Producttype> tempProducttypeList = new ArrayList<Producttype>();// 有产品的产品类型List
		for (Producttype producttype : allProductTypeList) {
			if(permitProductMap.containsKey(producttype.getId())){
				tempProducttypeList.add(producttype);
			}
		}
		
		for (Producttype tempproducttype : tempProducttypeList) {
			for (Producttype allproducttype : allProductTypeList) {
				if(tempproducttype.getStrPath().contains(allproducttype.getStrPath())){// 路径包含
					if(!permitProductMap.containsKey(allproducttype.getPid())){
						List<Object> tempProducttype = new ArrayList<Object>();
						tempProducttype.add(allproducttype);
						permitProductMap.put(allproducttype.getPid(), tempProducttype);	
					}else{
						List<Object> tempProducttype = permitProductMap.get(allproducttype.getPid());
						if(!tempProducttype.contains(allproducttype)){
							permitProductMap.get(allproducttype.getPid()).add(allproducttype);
						}
					}
				}
			}
		}
		tree.setId(TreeConstants.PREFIX_PRODUCT + Constants.ROOT_PRO_PARENT);
		if(permitProductMap.size()==0){
			throw new ExpectedException("", "没有授权的理财产品");
		}
		buildAllProductTree(tree, permitProductMap);
		if (tree.getChildren() != null) {
			tree = tree.getChildren().get(0);
		}
		return tree;
	}
	
    /**
     * 构建产品类型&产品树
     * @author jinchenwu 2016年5月4日 下午4:44:37
     * @param parent
     * @param permitProductMap
     */
	private void buildAllProductTree(TreeNode parent, Map<Integer, List<Object>> permitProductMap) {
		Integer parentId = Integer.parseInt(parent.getId().split("_")[1]);
		List<Object> children = permitProductMap.get(parentId);//通过父节点id，去map中寻找子节点的list集合
		if (children != null && children.size() > 0) {
			for (Object child : children) {
				TreeNode childNode = new TreeNode();
				if(child instanceof Producttype){
					childNode = buildProTypeTreeNode((Producttype)child);//将节点实体转化为TreeNode类型（设置前段需要的属性）
					childNode.setParentNode(parent);//给子节点设置父节点实体
					childNode.setParentId(parent.getId());//给字节点设置父节点id
					// 递归创建子节点
					buildAllProductTree(childNode, permitProductMap);
					parent.addChildren(childNode);
				}else{
					Product product = (Product)child;
					childNode.setId(TreeConstants.PREFIX_PRODUCT + product.getId());
					childNode.setLabel(product.getStrName());
					childNode.putExtra("strCode", product.getStrCode());
					childNode.putExtra("saveType", product.getSaveType());
					childNode.putExtra("saveYears", product.getSaveYears());
					childNode.putExtra("guaranteeType", product.getGuaranteeType());
					childNode.putExtra("guaranteeYears", product.getGuaranteeYears());
					childNode.putExtra("strEndDate", product.getStrEndDate());
					childNode.putExtra("strGrade", product.getStrGrade());
					childNode.putExtra("strRisklevel", product.getStrRisklevel());
					childNode.setParentNode(parent);//给子节点设置父节点实体
					childNode.setParentId(parent.getId());//给字节点设置父节点id
					parent.addChildren(childNode);
				}
			}
		}
	}
	
    /** ---------------------------- 产品类型树的私有方法 start-------------------------------- **/
    /**
     * 产品类型封装
     * 封装成<产品类型id1,<产品类型1,产品类型1,产品类型2,产品类型3>;产品类型id2,<产品类型1,产品类型2,产品类型3>>
     * 组装Map<父节点id1，[子节点1，子节点2]；父节点id2，[子节点1，子节点2]>
     * @author jinxindong 2016年3月21日 下午2:52:48
     * @param productList
     * @return
     */
    private Map<Integer, List<Producttype>> getProMapChildren(List<Producttype> productTypeList) {
		Map<Integer, List<Producttype>> productTypeMapChildren = Maps.newHashMap();
		for (Producttype productType : productTypeList) {
			Integer parentId = productType.getPid();
			List<Producttype> list = productTypeMapChildren.get(parentId);
			if (list == null) {
				list = new ArrayList<Producttype>();
				productTypeMapChildren.put(parentId, list);
			}
			list.add(productType);
		}
		return productTypeMapChildren;  	
    }
    
    /**
     * 构建产品类型树
     * @author jinxindong 2016年3月21日 下午3:07:46
     * @param parent
     * @param proMapChildren
     */
	private void buildProTypeChildNode(TreeNode parent, Map<Integer, List<Producttype>> proMapChildren) {
		Integer parentId = Integer.parseInt(parent.getId().split("_")[1]);//自己设置的id是一个reg_0的形式
		List<Producttype> children = proMapChildren.get(parentId);//通过初始id，去map中寻找子节点的list集合
		if (children != null && children.size() > 0) {
			for (Producttype child : children) {
				TreeNode childNode = buildProTypeTreeNode(child);//将节点实体转化为TreeNode类型（设置前段需要的属性）
				childNode.setParentNode(parent);//给子节点设置父节点实体
				childNode.setParentId(parent.getId());//给字节点设置父节点id
				// 递归创建子节点
				buildProTypeChildNode(childNode, proMapChildren);
				parent.addChildren(childNode);
			}
		}
	}
    
	/**
	 * 将产品类型对象转化为树节点对象
	 * @author jinxindong 2016年3月21日 下午3:17:13
	 * @param child
	 * @return
	 */
	private TreeNode buildProTypeTreeNode(Producttype child) {
		TreeNode node = new TreeNode();
		String strId = String.valueOf(child.getId());
		strId = TreeConstants.PREFIX_PRODUCTTYPE + strId;
		if (child.getId()==Constants.ROOT_PRO_ID) {
			node.setCls("product_home");//设置图标
		} else {
			node.setCls("productType");
		}
		node.setId(strId);// ID
		node.setLabel(child.getStrName());//节点名字显示
		//node.setOpen(true);
		node.putExtra("nsortId", child.getNsortId());//额外的属性列表
		return node;
	}
	
    /** ---------------------------- 产品类型树的私有方法 end-------------------------------- **/
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.common.tree.service.ITreeService#getProductTree(java.lang.String)
     */
    @Override
    public TreeNode getProductTree(String strId) {
    	TreeNode tree = new TreeNode();
    	if(StringUtils.isBlank(strId)){
    		tree.setId(TreeConstants.PREFIX_PRODUCT + Constants.ROOT_PRO_PARENT);
    	}else{
    		tree.setId(strId);
    	}

		//查询所有产品类型
		List<Producttype> producttypes = productTypeDao.getAll();		
		
    	// 获取全部产品
    	List<Product>  products = productDao.getAll();
		if (products == null || products.size() < 1) {
			throw new ExpectedException("", "尚未添加产品");			
		}
		
		// 获取产品类型Id与其直接子类型的Map集合
		Map<Integer, List<Producttype>> proTypeMapChildren = getProMapChildren(producttypes);  
		
		//获取设备id和其直接子信道的Map集合
		Map<Integer, List<Product>> productMapChildren = getProductMapChildren(products);
		Map<Integer, Product> productMap = getProductMap(products);
		Map<Integer, Producttype> productTypeMap = getProductTypeMap(producttypes);
		buildProductChildNode(tree, proTypeMapChildren, productMapChildren, null,productMap,productTypeMap);
		
		return tree;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.common.tree.service.ITreeService#getProductTreeByCertId(java.lang.String, java.lang.Integer)
     */
    @Override
	public TreeNode getProductTreeByCertId(String strId, Integer certId) {
		List<String> checkedIds = new ArrayList<String>();
		// 根据用户id查找先查找证书ids，根据证书id找到产品ids
		//List<Integer> certIds = usercrtificatemappingDao.getCertIdsByUserId(userId);
		List<Integer> checkedId = certProductMappingDao.getProductIdsbyCertId(certId);// 获取被选中的产品的id
		for (Integer cid : checkedId) {
			checkedIds.add(TreeConstants.PREFIX_PRODUCT + cid);
		}
		TreeNode tree = new TreeNode();
		if (strId == null) {
			strId = TreeConstants.PREFIX_PRODUCT + Constants.ROOT_PRO_PARENT;
		}
		tree.setId(strId);
		// 查询所有产品类型
		List<Producttype> producttypes = productTypeDao.getAll();
		
		// 获取全部产品
		List<Product> products = productDao.getAll();
		if (products == null || products.size() < 1) {
			throw new ExpectedException("", "尚未添加产品");			
		}
		
		// 获取产品类型Id与其直接子类型的Map集合
		Map<Integer, List<Producttype>> proTypeMapChildren = getProMapChildren(producttypes);
		
		// 获取设备id和其直接子信道的Map集合
		Map<Integer, List<Product>> productMapChildren = getProductMapChildren(products);
		Map<Integer, Product> productMap = getProductMap(products);
		Map<Integer, Producttype> productTypeMap = getProductTypeMap(producttypes);
		buildProductChildNode(tree, proTypeMapChildren, productMapChildren, checkedIds,productMap,productTypeMap);
		
		return tree;
	}
    /** ---------------------------- 产品树的私有方法 start-------------------------------- **/
    
    /** 
     * 产品封装
     * 封装成<产品类型id1,<产品1,产品2,产品3,产品4>;产品类型id2,<产品1,产品2,产品3,产品4>>
     * 组装Map<父节点id1，[子节点1，子节点2]；父节点id2，[子节点1，子节点2]>
     * @author jinxindong 2016年3月28日 上午9:18:58
     * @param productList
     * @return
     */
     
	Map<Integer, List<Product>> getProductMapChildren(List<Product> productList) {		
		Map<Integer, List<Product>> productMapChildren = Maps.newHashMap();
		for (Product product : productList) {
			Integer parentId = product.getPid();
			List<Product> list = productMapChildren.get(parentId);
			if (list == null) {
				list = new ArrayList<Product>();
				productMapChildren.put(parentId, list);
			}
			list.add(product);
		}
		return productMapChildren;
	}
	
	/**
	 * 获取product的map
	 * @author jinxindong 2016年7月13日 下午2:17:08
	 * @param productList
	 * @return
	 */
	Map<Integer, Product> getProductMap(List<Product> productList) {		
		Map<Integer, Product> productMap = Maps.newHashMap();
		for (Product product : productList) {
			productMap.put(product.getId(), product);
		}
		return productMap;
	}
	/**
	 * 获取productType的map
	 * @author jinxindong 2016年7月13日 下午2:20:34
	 * @param productTypeList
	 * @return
	 */
	Map<Integer, Producttype> getProductTypeMap(List<Producttype> productTypeList) {		
		Map<Integer, Producttype> productTypeMap = Maps.newHashMap();
		for (Producttype producttype : productTypeList) {
			productTypeMap.put(producttype.getId(), producttype);
		}
		return productTypeMap;
	}
	
	/**
	 * 将产品转化为节点
	 * @author jinxindong 2016年3月9日 下午3:05:32
	 * @param child
	 * @param checkedId
	 * @return
	 */
	private TreeNode buildProductTreeNode(Product child, List<String> checkedId) {
		TreeNode node = new TreeNode();
		String strId = String.valueOf(child.getId());
		strId = TreeConstants.PREFIX_PRODUCT + strId;
		node.setCls("product");
		node.setId(strId);// ID
		node.setLabel(child.getStrName());// 节点名字显示
		//node.setOpen(true);// 是否展开节点
		node.setChecked(checkedId != null && checkedId.contains(strId));// 判断是否勾选,设置勾选属性
		return node;
	}
	
    /**
     * 构建产品树
     * @author jinxindong 2016年3月28日 上午9:37:28
     * @param parent
     * @param proTypeMapChildren
     * @param productMapChildren
     * @param checkedIds
     */
	private void buildProductChildNode(TreeNode parent, Map<Integer, List<Producttype>> proTypeMapChildren,
	        Map<Integer, List<Product>> productMapChildren, List<String> checkedIds,Map<Integer,Product> productMap,Map<Integer,Producttype> productTypeMap) {
		List<String> strPathList = new ArrayList<String>(); // id--strpath的map
		if (checkedIds != null) {// 编辑半选操作
			for (String checkedId : checkedIds) {
				Integer productId = Integer.parseInt(checkedId.split("_")[1]);// 获取每个产品的id
				Product product = productMap.get(productId);
				Integer proTypeId = product.getPid();// 获取产品的上一级的id（直接类型的id）
				Producttype producttype = productTypeMap.get(proTypeId);
				strPathList.add(producttype.getStrPath());
			}
		}
		
		Integer parentId = Integer.parseInt(parent.getId().split("_")[1]);// 自己设置的id是类似于一个reg_0
		List<Producttype> children = proTypeMapChildren.get(parentId);// 通过初始id，去map中寻找子节点的list集合
		if (children != null && children.size() > 0) {
			for (Producttype child : children) {
				TreeNode childNode = buildProTypeTreeNode(child);// 将节点实体转化为TreeNode类型（设置前段需要的属性）
				childNode.setParentNode(parent);// 给子节点设置父节点实体
				childNode.setParentId(parent.getId());// 给字节点设置父节点id
				if (checkedIds != null) {// 编辑半选操作
					for (String strPath : strPathList) {
						if (strPath.contains(child.getStrPath())) {// 如果包含，说明要半选
							childNode.setHalfCheck(true);
						}
					}
					
				}
				// 没有子节点的产品类型且没有子节点的产品需要被隐藏
				if (proTypeMapChildren.get(Integer.parseInt(childNode.getId().split("_")[1])) == null
				        && productMapChildren.get(Integer.parseInt(childNode.getId().split("_")[1])) == null) {
					childNode.setIsHidden(true);
				} else if (!proTypeSonHasProduct(child, productMapChildren)) {// 如果没有产品类型节点的子孙节点没有产品也要被隐藏
					childNode.setIsHidden(true);
				} else {
					childNode.setIsParent(true);// 反之需要添加可展开
				}
				parent.addChildren(childNode);
			}
		}
		// 2，构建产品子节点
		List<Product> products = productMapChildren.get(parentId);
		if (products != null && products.size() > 0) {
			for (Product product : products) {
				TreeNode productNode = buildProductTreeNode(product, checkedIds);
				productNode.setParentNode(parent);
				productNode.setParentId(parent.getId());
				parent.addChildren(productNode);
			}
		}
	}
	
	/**
	 * 判断类型节点的子孙节点没有产品
	 * @author jinxindong 2016年3月28日 上午9:37:35
	 * @param child
	 * @param productMapChildren
	 * @return
	 */
	private Boolean proTypeSonHasProduct(Producttype child, Map<Integer, List<Product>> productMapChildren) {
		Boolean hasChannel = false;
		// 通过strpath找到子孙节点
		List<Producttype> proTypes = productTypeDao.getLikeStrPath(child);
		// 遍历子孙节点是否在deviceChannelMapChildren中含有信道
		for (Producttype producttype : proTypes) {
			if (null != productMapChildren.get(producttype.getId())) {
				hasChannel = true;
			}
		}
		return hasChannel;
	}
    /** ---------------------------- 产品树的私有方法 end-------------------------------- **/
    
	//======================getter/setter============================
    public IRegionDao getRegionDao() {
    	return regionDao;
    }
	
    public void setRegionDao(IRegionDao regionDao) {
    	this.regionDao = regionDao;
    }

	
    public IRolePrivilegeMappingDao getRolePrivilegeMappingDao() {
    	return rolePrivilegeMappingDao;
    }

	
    public void setRolePrivilegeMappingDao(IRolePrivilegeMappingDao rolePrivilegeMappingDao) {
    	this.rolePrivilegeMappingDao = rolePrivilegeMappingDao;
    }

	
    public IUserregionmappingDao getUserregionmappingDao() {
    	return userregionmappingDao;
    }

	
    public void setUserregionmappingDao(IUserregionmappingDao userregionmappingDao) {
    	this.userregionmappingDao = userregionmappingDao;
    }

	
    public IUserrolemappingDao getUserrolemappingDao() {
    	return userrolemappingDao;
    }

	
    public void setUserrolemappingDao(IUserrolemappingDao userrolemappingDao) {
    	this.userrolemappingDao = userrolemappingDao;
    }

	
    public IUserdevicemappingDao getUserdevicemappingDao() {
    	return userdevicemappingDao;
    }

	
    public void setUserdevicemappingDao(IUserdevicemappingDao userdevicemappingDao) {
    	this.userdevicemappingDao = userdevicemappingDao;
    }

	
    public IDeviceDao getDeviceDao() {
    	return deviceDao;
    }

	
    public void setDeviceDao(IDeviceDao deviceDao) {
    	this.deviceDao = deviceDao;
    }

	
    public IDeviceChannelDao getDeviceChannelDao() {
    	return deviceChannelDao;
    }

	
    public void setDeviceChannelDao(IDeviceChannelDao deviceChannelDao) {
    	this.deviceChannelDao = deviceChannelDao;
    }

	
    public IProductTypeDao getProductTypeDao() {
    	return productTypeDao;
    }

	
    public void setProductTypeDao(IProductTypeDao productTypeDao) {
    	this.productTypeDao = productTypeDao;
    }

	
    public IProductDao getProductDao() {
    	return productDao;
    }

	
    public void setProductDao(IProductDao productDao) {
    	this.productDao = productDao;
    }

	
    public IUsercrtificatemappingDao getUsercrtificatemappingDao() {
    	return usercrtificatemappingDao;
    }

	
    public void setUsercrtificatemappingDao(IUsercrtificatemappingDao usercrtificatemappingDao) {
    	this.usercrtificatemappingDao = usercrtificatemappingDao;
    }

	
    public ICertProductMappingDao getCertProductMappingDao() {
    	return certProductMappingDao;
    }

	
    public void setCertProductMappingDao(ICertProductMappingDao certProductMappingDao) {
    	this.certProductMappingDao = certProductMappingDao;
    }

	
    public IProductService getProductService() {
    	return productService;
    }

	
    public void setProductService(IProductService productService) {
    	this.productService = productService;
    }

	
    public IPrivilegeDao getPrivilegeDao() {
    	return privilegeDao;
    }

	
    public void setPrivilegeDao(IPrivilegeDao privilegeDao) {
    	this.privilegeDao = privilegeDao;
    }






	















    
    
}
