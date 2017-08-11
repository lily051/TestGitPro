/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月3日 上午10:49:20
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.service.impl;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import org.apache.poi.hssf.usermodel.DVConstraint;
import org.apache.poi.hssf.usermodel.HSSFDataValidation;
import org.apache.poi.hssf.util.CellRangeAddressList;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.ss.usermodel.WorkbookFactory;
import org.apache.struts2.ServletActionContext;

import com.google.common.collect.Lists;
import com.google.common.collect.Maps;
import com.hikvision.cms.cache.core.tools.StringUtil;
import com.hikvision.cms.cache.manager.DataManager;
import com.hikvision.finance.core.util.BeanUtils;
import com.hikvision.finance.core.util.JsonUtils;
import com.hikvision.finance.core.util.ListUtil;
import com.hikvision.finance.core.util.SercurityUtil;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.common.enums.LogModuledEnum;
import com.hikvision.finance.fms.common.enums.SysRoleEnum;
import com.hikvision.finance.fms.common.excel.ExcelHelper;
import com.hikvision.finance.fms.common.excel.ExcelHelperFactory;
import com.hikvision.finance.fms.common.excel.model.ExcelHead;
import com.hikvision.finance.fms.common.model.UserSession;
import com.hikvision.finance.fms.common.util.Base64Utils;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.DeviceType;
import com.hikvision.finance.fms.common.util.Constants.OperStatus;
import com.hikvision.finance.fms.common.util.ExcelUtil;
import com.hikvision.finance.fms.common.util.SessionUtil;
import com.hikvision.finance.fms.common.util.ValidateUtil;
import com.hikvision.finance.fms.model.Crtificate;
import com.hikvision.finance.fms.model.Device;
import com.hikvision.finance.fms.model.Devicechannel;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.model.Role;
import com.hikvision.finance.fms.model.Usercrtificatemapping;
import com.hikvision.finance.fms.model.Userdevicemapping;
import com.hikvision.finance.fms.model.Userlock;
import com.hikvision.finance.fms.model.Userregionmapping;
import com.hikvision.finance.fms.model.Userrolemapping;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.device.dao.IDeviceChannelDao;
import com.hikvision.finance.fms.modules.device.dao.IDeviceDao;
import com.hikvision.finance.fms.modules.log.dao.IConfigLogDao;
import com.hikvision.finance.fms.modules.product.dao.ICertDao;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fms.modules.region.dto.RegionInfo;
import com.hikvision.finance.fms.modules.role.dao.IRoleDao;
import com.hikvision.finance.fms.modules.role.dto.RoleInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.SysconfigDto;
import com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService;
import com.hikvision.finance.fms.modules.user.dao.IUsercrtificatemappingDao;
import com.hikvision.finance.fms.modules.user.dao.IUserdevicemappingDao;
import com.hikvision.finance.fms.modules.user.dao.IUserregionmappingDao;
import com.hikvision.finance.fms.modules.user.dao.IUserrolemappingDao;
import com.hikvision.finance.fms.modules.user.dao.IUsersDao;
import com.hikvision.finance.fms.modules.user.dto.UserInfo;
import com.hikvision.finance.fms.modules.user.dto.UserdevicemappingInfo;
import com.hikvision.finance.fms.modules.user.dto.UsersDto;
import com.hikvision.finance.fms.modules.user.model.RecordPojo;
import com.hikvision.finance.fms.modules.user.qo.UsersQo;
import com.hikvision.finance.fms.modules.user.service.IUserService;
import com.hikvision.finance.fms.webdevice.contact.dao.IBusinessOrderDao;
import com.hikvision.finance.fms.webdevice.datacollect.dao.IRecordsDao;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.AjaxData;
import com.hikvision.finance.fwork.util.GlobalMessageUtil;

/**
 * <p>用户管理Service</p>
 * @author jinxindong 2016年3月3日 上午10:49:20
 * @version V1.0
 */
@SuppressWarnings("deprecation")
public class UserServiceImpl extends BaseServiceImpl<Users, UsersQo, IUsersDao> implements IUserService {
	
	private IUsersDao usersDao;
	private IUserrolemappingDao userrolemappingDao;
	private IUsercrtificatemappingDao usercrtificatemappingDao;
	private IUserdevicemappingDao userdevicemappingDao;
	private IUserregionmappingDao userregionmappingDao;
	private IRoleDao roleDao;
	private ISysconfigService sysconfigService;
	private ICertDao certDao;
	private IRegionDao regionDao;
	private IDeviceChannelDao deviceChannelDao;
	private IDeviceDao deviceDao;
	private DataManager dataManager;
	private IConfigLogDao configLogDao;
	private IBusinessOrderDao businessOrderDao;
	private IRecordsDao recordsDao;
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.extend.service.IBaseService#deleteByIds(java.io.Serializable[])
	 */
	@Override
	public void deleteByIds(Serializable[] paramArrayOfSerializable) {
		
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.extend.service.BaseServiceImpl#getDao()
	 */
	@Override
	protected IUsersDao getDao() {
		return this.getUsersDao();
	}
	
    /**
     * 获取区域和信道关系map
     * @author jinxindong 2016年7月13日 下午4:36:55
     * @param productList
     * @return
     */
	Map<Integer, List<Devicechannel>> buildDevChannelMap(List<Devicechannel> devicechannels) {
		Map<Integer, List<Devicechannel>> devChannel = Maps.newHashMap();
		for (Devicechannel devicechannel : devicechannels) {
			Integer regionId = devicechannel.getNregionId();
			List<Devicechannel> devicechannelList = devChannel.get(regionId);
			if (devicechannelList == null) {
				devicechannelList = Lists.newArrayList();
				devChannel.put(regionId, devicechannelList);
			}
			devicechannelList.add(devicechannel);
		}
		return devChannel;
	}

	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.user.service.IUserService#addUser(java.lang.String)
	 */
	@Override
	public AjaxData addUser(UserInfo info,String jsonStr) {
		AjaxData ajaxData = new AjaxData(false);
		UsersDto userDto = JsonUtils.json2Object(jsonStr, UsersDto.class);
		Users user = new Users();
		//验证用户名的唯一性
		List<Users> users = usersDao.findBy("strName", info.getStrName().trim());
		if(users != null && users.size()>0){
			return ajaxData.setFailureMsg("已存在用户名为" + info.getStrName());
		}		

		//验证员工编号唯一性
		List<Users> user2 = usersDao.findBy("userNo", info.getUserNo().trim());
		if(user2 != null && user2.size()>0){
			return ajaxData.setFailureMsg("已存在员工编号为" + info.getUserNo());
		}
		
		user.setStrName(info.getStrName());
		user.setStrCode(info.getStrCode());
		user.setSysRole(info.getSysRole());
		//获取默认密码添加到数据库
		SysconfigDto sysInfo = sysconfigService.getSysconfig();
		user.setStrPassword(sysInfo.getPasswordDefault());
		user.setStrTel(null==info.getStrTel()?"":info.getStrTel());
		user.setStrEmail(null==info.getStrEmail()?"":info.getStrEmail());
		user.setStrComment(null==info.getStrComment()?"":info.getStrComment());
		user.setNstate(Constants.UserStatus.NORMAL);
		user.setNonline(Constants.UserStatus.NOTONLINE);
		user.setStrDate(info.getStrDate());
		user.setStrUpdatePwdTime(new Date());
		user.setNsex(info.getNsex());
		user.setNetId(info.getNetId());
		user.setNet(info.getNet());
		user.setStrDuty(null==info.getStrDuty()?"":info.getStrDuty());
		user.setUserNo(info.getUserNo());
		UserSession userSession = SessionUtil.getUserSession();
		user.setNowner(userSession.getUserId());
		user.setStrCredentialNo(null==info.getStrCredentialNo()?"":info.getStrCredentialNo());
		Integer userId = (Integer)usersDao.save(user);
		String code = getUserSysType(userId);
		//如果是中心管理员
		if(code.equals(SysRoleEnum.CENTRY_ADMIN_0.getCode())){
			// 中心管理员，1:添加用户和角色的关联关系(id);
			String roleId =info.getRoleId();
			if(StringUtils.isBlank(roleId)){
				throw new ExpectedException("", "未获取到角色内容");
			}
			Userrolemapping userrolemapping = new Userrolemapping();
			userrolemapping.setNroleId(Integer.valueOf(roleId));
			userrolemapping.setNuserId(userId);
			userrolemappingDao.save(userrolemapping);
			//2:用户和区域的关系
			List<Userregionmapping> regionList = userDto.getRegionList(); 
			if(regionList != null && regionList.size()>0){
				for(Userregionmapping userregionmapping : regionList){
					userregionmapping.setNuserId(userId);
					userregionmappingDao.save(userregionmapping);
				}
			}			
		}else if(code.equals(SysRoleEnum.FINANC_MANAGER_1.getCode())){	
			// 理财经理，1:添加用户和信道的关联关系；
			List<UserdevicemappingInfo> deviceList = userDto.getDeviceList();	    		
			StringBuffer buffer1 = new StringBuffer();
			StringBuffer buffer2 = new StringBuffer();
			StringBuffer buffer3 = new StringBuffer();
			int i=0,j=0,k=0;
			if (deviceList != null && deviceList.size()>0) {
				for (UserdevicemappingInfo userDevice : deviceList) {
					if (userDevice.getStatus()) {
						buffer1.append("("+userId+", "+userDevice.getDevId()+","+userDevice.getDevChannel()+"),");
						i++;
					}
				}
			}
			if(i>0){
				String strsql = buffer1.deleteCharAt(buffer1.length() - 1).toString();
				userdevicemappingDao.saveMapping(strsql);	
			}
			//根据区域的id找到信道，并关联
			List<RegionInfo> regionIdList = userDto.getRegionIdList();
			if (regionIdList != null && regionIdList.size()>0) {
				List<Devicechannel> devicechannelList = deviceChannelDao.getAll();
				Map<Integer, List<Devicechannel>> devChannelMap = buildDevChannelMap(devicechannelList);
				for (RegionInfo regionInfo : regionIdList) {
					if (regionInfo.getStatus()) {
						// 每个regionId去找strpath相同的节点（子节点）
						Region region2 = regionDao.get(regionInfo.getId());
						List<Region> regions = regionDao.getLikeStrPath(region2);
						for (Region region : regions) {
							List<Devicechannel> devicechannels = devChannelMap.get(region.getId());
							// 如果存在监控点
							if (devicechannels != null) {
								for (Devicechannel devicechannel : devicechannels) {
									buffer2.append("("+userId+", "+devicechannel.getNdeviceId()+","+devicechannel.getId()+"),");
									j++;
								}
							}
						}
					}
				}
				if(j>0){
					String strsql2 = buffer2.deleteCharAt(buffer2.length() - 1).toString();
					userdevicemappingDao.saveMapping(strsql2);	
				}
			}

			
			//2:用户和证书的关联关系
			List<Usercrtificatemapping> crtificateList = userDto.getCrtificateList();
			if(crtificateList != null && crtificateList.size()>0){
				for(Usercrtificatemapping userCrtificate : crtificateList){
					buffer3.append("("+userCrtificate.getNcrtificateId()+", "+userId+" ),");
					k++;
				}
				if(k>0){
					String strsql3 = buffer3.deleteCharAt(buffer3.length() - 1).toString();
					usercrtificatemappingDao.saveMapping(strsql3);
				}
			}
		}
		
		configLogDao.setLogContent(OperStatus.ADD, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.users.add",new String[] {info.getStrName()}));
		return ajaxData;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.user.service.IUserService#deleteUsers(java.util.List)
	 */
	@Override
	public AjaxData deleteUsers(List<Integer> ids) {
		// 理财经理，删除用户和设备的关联关系；用户和证书的关联关系
		// 中心管理员，删除用户和角色的关联关系；用户和区域的关系
		AjaxData ajaxData = new AjaxData(false);
		List<String> names = new ArrayList<String>();
		// 1.如果id中包含自己的id，那么不能删除
		UserSession userSession = SessionUtil.getUserSession();
		if (ids.contains(userSession.getUserId())) {
			ajaxData.setSuccess(false);
			ajaxData.setMsg("不能删除含有自己id的用户");
			return ajaxData;
		}
		
		
		// 2.没有含有自己的id 先删除关联关系		
		delUserDeviceMapping(ids);
		delUserCrtificateMapping(ids);
		delUserRoleMapping(ids);
		delUserRegionMapping(ids);
		
		for (Integer id : ids) {
			// 3.没有含有自己的id 再删除用户对应的临时业务单和临时录像
			businessOrderDao.deleteBy("nuserId", id);//删除临时业务单
			/*
			 	删除临时录像 注释掉原因, 如果存在上传中的录像, 录像数据被删除, 从而导致服务不能更新录像上传状态, 如果有上传成功中心端就不能进行回访, 
				针对于录像上传失败的, 用户不存在的通过定时任务来删除对应的脏数据
			*/
//			recordsDao.deleteBy("nuserId", id);//删除临时录像 
			
			
			Users user = usersDao.get(id);
			names.add(user.getStrName());
			this.usersDao.deleteById(id);
			try {
				if(user.getUserSessionId()!=null){
					dataManager.remove(user.getUserSessionId());
				}
	        } catch (Exception e) {
	        	throw new ExpectedException("", "删除用户出错");
	        }
		}
		if(ids.size()>5){
			configLogDao.setLogContent(OperStatus.DELETE, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.users.delete.batch"));	
		}else{
			configLogDao.setLogContent(OperStatus.DELETE, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.users.delete", new String[] {ListUtil.listToStr(names)}));	
		}
		
		return ajaxData;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.user.service.IUserService#updateUser(java.lang.Integer)
	 */
	@Override
	public AjaxData updateUser(UserInfo info,String jsonStr) {
		AjaxData ajaxData = new AjaxData(false);
		UsersDto userDto = JsonUtils.json2Object(jsonStr, UsersDto.class);
		Users user = usersDao.get(info.getId());
		Integer userId = SessionUtil.getUserSession().getUserId();
		if(info.getId().equals(userId)){
			ajaxData.setFailureMsg("自己本身不允许修改自己的信息");
			return ajaxData;
		}
		// 验证员工编号的唯一性
		List<Users> user2 = usersDao.findBy("userNo", info.getUserNo());
		for (Users u : user2) {
			if (!u.getId().equals(info.getId())) {
				ajaxData.setFailureMsg("已存在员工编号为"+u.getUserNo());
				return ajaxData;
			}
		}		
		if(info.getStrName() != null){
			// 验证用户名的唯一性
			List<Users> users = usersDao.findBy("strName", info.getStrName());
			for (Users u : users) {
				if (!u.getId().equals(info.getId())) {
					ajaxData.setFailureMsg("已存在用户名为"+u.getStrName());
					return ajaxData;
				}
			}
			user.setStrName(info.getStrName());
		}
		user.setStrCode(info.getStrCode());
		if (info.getSysRole() != null) {
			user.setSysRole(info.getSysRole());
		}
		user.setStrTel(null==info.getStrTel()?"":info.getStrTel());
		user.setStrEmail(null==info.getStrEmail()?"":info.getStrEmail());
		user.setStrComment(null==info.getStrComment()?"":info.getStrComment());
		if (info.getStrDate() != null) {
			user.setStrDate(info.getStrDate());
		}
		user.setNsex(info.getNsex());
		user.setNetId(info.getNetId());
		user.setNet(info.getNet());
		user.setStrDuty(null==info.getStrDuty()?"":info.getStrDuty());
		user.setUserNo(info.getUserNo());
		user.setStrCredentialNo(null==info.getStrCredentialNo()?"":info.getStrCredentialNo());
		usersDao.update(user);
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.users.update",new String[] {user.getStrName()}));
		String code = getUserSysType(info.getId());
		//如果是中心管理员(更新的操作是先删除，后添加)
		if(code.equals(SysRoleEnum.CENTRY_ADMIN_0.getCode())){
			// 中心管理员，1：更新用户和角色的关联关系；
			List<Integer> userRoleIds = new ArrayList<Integer>();
			userRoleIds.add(info.getId());
			delUserRoleMapping(userRoleIds);
			
			Userrolemapping userrolemapping = new Userrolemapping();
			String roleId =info.getRoleId();
			userrolemapping.setNroleId(Integer.valueOf(roleId));
			userrolemapping.setNuserId(info.getId());
			userrolemappingDao.save(userrolemapping);
			
			//2：更新用户和区域的关系(如果前端传过来是非空的则需要更新)
			List<Userregionmapping> regionList = userDto.getRegionList();
			if(regionList != null && regionList.size()>0){
				List<Integer> userRegionIds = new ArrayList<Integer>();
				userRegionIds.add(info.getId());
				delUserRegionMapping(userRegionIds);
				for(Userregionmapping userregionmapping : regionList){
					userregionmapping.setNuserId(info.getId());
					userregionmappingDao.save(userregionmapping);
				}
			}
			
		}else if(code.equals(SysRoleEnum.FINANC_MANAGER_1.getCode())){
			StringBuffer buffer1 = new StringBuffer();
			StringBuffer buffer2 = new StringBuffer();
			StringBuffer buffer3 = new StringBuffer();
			StringBuffer buffer4 = new StringBuffer();
			int i=0,j=0,k=0,m=0;
			// 理财经理，1:更新用户和监控点的关联关系；
			//1.1：获取监控点的状态，状态为true的添加，为false的删除
			List<UserdevicemappingInfo> deviceList = userDto.getDeviceList();
			if(deviceList != null && deviceList.size()>0){
				for(UserdevicemappingInfo userdevicemappingInfo:deviceList){
					Boolean status = userdevicemappingInfo.getStatus();
					buffer1.append(userdevicemappingInfo.getDevChannel()+",");
					i++;
					if(status){//如果是true是添加，否则是删除
						buffer2.append("("+info.getId()+", "+userdevicemappingInfo.getDevId()+","+userdevicemappingInfo.getDevChannel()+"),");
						j++;

					}
				}
				if(i>0){
					userdevicemappingDao.deleteMapping(info.getId(), buffer1.deleteCharAt(buffer1.length() - 1).toString());	
				}
				if(j>0){
					String strsql2 = buffer2.deleteCharAt(buffer2.length() - 1).toString();
					userdevicemappingDao.saveMapping(strsql2);	
				}
			}
						
			//1.2:根据区域的id的状态判断：信道的添加或者删除 			
			List<RegionInfo> regionIdList = userDto.getRegionIdList();
			if(regionIdList != null && regionIdList.size()>0){
				List<Devicechannel> devicechannelList = deviceChannelDao.getAll();
				Map<Integer, List<Devicechannel>> devChannelMap = buildDevChannelMap(devicechannelList);
				for (RegionInfo regionInfo : regionIdList) {
					Boolean status = regionInfo.getStatus();
					// 每个regionId去找strpath相同的节点（子节点）
					Region region2 = regionDao.get(regionInfo.getId());
					List<Region> regions = regionDao.getLikeStrPath(region2);
					for (Region region : regions) {
						List<Devicechannel> devicechannels  = devChannelMap.get(region.getId());
						// 如果存在监控点,对监控点进行增加或者删除
						if (devicechannels != null) {
							for (Devicechannel devicechannel : devicechannels) {
								buffer3.append("("+devicechannel.getId()+"),");
								k++;
								if (status) {
									buffer4.append("("+info.getId()+", "+devicechannel.getNdeviceId()+","+devicechannel.getId()+"),");
									m++;
								} 
							}							
						}
					}
				}
				if(k>0){
					userdevicemappingDao.deleteMapping(info.getId(), buffer3.deleteCharAt(buffer3.length() - 1).toString());	
				}
				if(m>0){
					String strsql4 = buffer4.deleteCharAt(buffer4.length() - 1).toString();
					userdevicemappingDao.saveMapping(strsql4);
				}
			}
			
			//2:用户和证书的关联关系
			List<Usercrtificatemapping> crtificateList = userDto.getCrtificateList();
			List<Integer> userCrtificateIds = new ArrayList<Integer>();
			userCrtificateIds.add(info.getId());
			delUserCrtificateMapping(userCrtificateIds);
			if(crtificateList != null && crtificateList.size()>0){
				for(Usercrtificatemapping userCrtificate : crtificateList){
					userCrtificate.setUserId(info.getId());
					usercrtificatemappingDao.save(userCrtificate);
				}
			}
		}
		
		return ajaxData;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.user.service.IUserService#resetPassword(java.util.List)
	 */
	@Override
	public AjaxData resetPassword(List<Integer> ids) {
		AjaxData ajaxData = new AjaxData(false);
		SysconfigDto sysInfo = sysconfigService.getSysconfig();
		usersDao.resetPassword(ids, sysInfo.getPasswordDefault());
		List<String> names = new ArrayList<String>();
		for(Integer id :ids){
			Users user = usersDao.get(id);
			names.add(user.getStrName());	
		}
		if(ids.size()>5){
			configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.users.repwd.batch"));
		}else{
			configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.users.repwd",new String[] {ListUtil.listToStr(names)}));	
		}
		return ajaxData;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.user.service.IUserService#enableUsers(java.util.List)
	 */
	@Override
	public AjaxData enableUsers(List<Integer> ids) {
		AjaxData ajaxData = new AjaxData(false);
		List<String> names = new ArrayList<String>();
		UserSession userSession = SessionUtil.getUserSession();
		Boolean isFlag = true;
		for (Integer id : ids) {
			if (id.equals(userSession.getUserId())) {
				ajaxData.setSuccess(false);
				ajaxData.setMsg("不能启用含有自己id的用户");
				isFlag = false;
				break;
			}
		}
		if (isFlag) {
			usersDao.enableUsers(ids);
			for(Integer id :ids){
				Users user = usersDao.get(id);
				names.add(user.getStrName());	
			}
			if(ids.size()>5){
				configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.users.enable.batch"));
			}else{
				configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.users.enable",new String[] {ListUtil.listToStr(names)}));	
			}
		}
		return ajaxData;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.user.service.IUserService#disableUsers(java.util.List)
	 */
	@Override
	public AjaxData disableUsers(List<Integer> ids) {
		AjaxData ajaxData = new AjaxData(false);
		List<String> names = new ArrayList<String>();
		UserSession userSession = SessionUtil.getUserSession();
		Boolean isFlag = true;
		for (Integer id : ids) {
			if (id.equals(userSession.getUserId())) {
				ajaxData.setSuccess(false);
				ajaxData.setMsg("不能禁用含有自己id的用户");
				isFlag = false;
				break;
			}
		}
		if (isFlag) {
			usersDao.disableUsers(ids);
			for(Integer id :ids){
				Users user = usersDao.get(id);
				names.add(user.getStrName());	
			}
			if(ids.size()>5){
				configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.users.disable.batch"));
			}else{
			    configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.users.disable",new String[] {ListUtil.listToStr(names)}));
			}
		}
		return ajaxData;
	}
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.service.IUserService#getDataPage(java.lang.Integer, java.lang.Integer, java.lang.String, java.lang.String, java.lang.String, java.lang.Integer)
     */
    @SuppressWarnings("unchecked")
    @Override
	public Page getDataPage(Integer limit, Integer start, UserInfo userInfo) {
		//1.获取分页数据
		Page page = usersDao.getDataPage(limit, start, userInfo);
		
		// 3.获取userRoleMap<nuserId,Userrolemapping>
		List<Userrolemapping> userrolemappings = userrolemappingDao.getAll();
		Map<Integer, Userrolemapping> userRoleMap =  usersDao.buildUserRoleMap(userrolemappings);
		
		// 4.获取roleMap<id,Role>
		List<Role> roles = roleDao.getAll();
		Map<Integer, Role> roleMap = usersDao.buildRoleMap(roles);
		
		//5.获取userRegionMap<nuserId,Userregionmapping>
		List<Userregionmapping> userregionmappings =  userregionmappingDao.getAll();
		Map<Integer,Userregionmapping> userRegionMap = Maps.newHashMap();
		for(Userregionmapping userregionmapping:userregionmappings){
			userRegionMap.put(userregionmapping.getNuserId(), userregionmapping);
		}
		
		//6.循环迭代用户类型名，分配角色名,关联关系等
		List<UserInfo> userInfos = (List<UserInfo>)page.getResult();
		List<UserInfo> userInfoList = new ArrayList<UserInfo>();
		String typrName = "";
		Role role = null;
		for (UserInfo userOld : userInfos) {
			if (SysRoleEnum.CENTRY_ADMIN_0.getCode().equals(userOld.getSysRole().toString())) {
				Userrolemapping userrolemapping = userRoleMap.get(userOld.getId());
				if (userrolemapping == null) {
					throw new ExpectedException("", "用户未关联角色");
				}
				role = roleMap.get(userrolemapping.getNroleId());
				Userregionmapping userregionmapping = userRegionMap.get(userOld.getId());
				if (userregionmapping != null || "admin".equals(userOld.getStrName())) {
					userOld.setAuthRelate(Constants.CONS_1);// 判断是否关联区域
				} else {
					userOld.setAuthRelate(Constants.CONS_0);
				}
				userOld.setRoleName(role.getStrName());
				userOld.setRoleId(role.getId().toString());
				typrName = SysRoleEnum.CENTRY_ADMIN_0.getDescribe();
			} else {
				List<Userdevicemapping> list = userdevicemappingDao.findBy("nuserId", userOld.getId());//因为多对多关系，只能放入for循环操作数据库
				if (list != null && list.size() > 0) {
					userOld.setUserChannel(Constants.CONS_1); // 关联信道
					userOld.setAuthRelate(Constants.CONS_1);// 判断是否关联区域
				} else {
					userOld.setUserChannel(Constants.CONS_0); // 没有关联信道
					userOld.setAuthRelate(Constants.CONS_0);
				}
				userOld.setRoleName(Constants.ROLE_NAME_NULL);
				userOld.setRoleId(Constants.ROLE_ID_NULL);
				typrName = SysRoleEnum.FINANC_MANAGER_1.getDescribe();
			}
			userOld.setSysTypeName(typrName);
			userInfoList.add(userOld);
		}
		
		page.setResult(userInfoList);
		return page;
	}
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.service.IUserService#getUserById(java.lang.Integer)
     */
    @Override
    public UserInfo getUserById(Integer userId) {
	    Users user = this.usersDao.get(userId);
	    UserInfo userInfo = new UserInfo();
	    try {
	        BeanUtils.copyNotNullProperties(userInfo, user);
        } catch (Exception e) {
	        throw new ExpectedException("", "User拷贝bean出错[getUserById]");
        }
	    //判断用户id是不是当前登录id，是的话不允许做权限类修改和用户名修改
	    UserSession userSession = SessionUtil.getUserSession();
	    if(userId.equals(userSession.getUserId())){
	    	userInfo.setIsOneself(true);
	    }else{
	    	userInfo.setIsOneself(false);
	    }
	    if(SysRoleEnum.CENTRY_ADMIN_0.getCode().equals(userInfo.getSysRole().toString())){
	    /**如果是中心管理员*/
		   //前台界面需显示被选的角色
		   Userrolemapping userrolemapping = userrolemappingDao.findUniqueBy("nuserId", userId);
		   userInfo.setRoleId(userrolemapping.getNroleId().toString());
		   //前台界面需显示组织的id和名称
	 	   Userregionmapping userregionmapping =  userregionmappingDao.findUniqueBy("nuserId", userId);
	 	   if(userregionmapping!=null){
		 	   Region region = regionDao.get(userregionmapping.getNregionId());
		 	   if(region!=null){
			 	   userInfo.setRegionId(userregionmapping.getNregionId());
			 	   userInfo.setRegionName(region.getStrName());  
		 	   } 
	 	   }
	    }else{
	    /**如果是理财经理*/
	 	   //前台界面需显示被选证书的id和名字
	 	   List<Integer> certListMping = usercrtificatemappingDao.getCertIdsByUserId(userId);
	 	   //userInfo.setCertIds(certListMping);
	 	   List<Crtificate> certNames = certDao.getCertNamesByIds(certListMping);
	 	   userInfo.setCertName(certNames);
	    }
	   return userInfo;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.service.IUserService#getRolesList()
     */
    @Override
    public List<RoleInfo> getRolesList() {
    	List<Role> roles = roleDao.getAll();
    	List<RoleInfo> roleInfos = new ArrayList<RoleInfo>();
    	for(Role roleOld : roles){
    		if("admin".equals(roleOld.getStrName())){
    			continue;
    		}
    		RoleInfo roleInfo = new RoleInfo();
    		try {
	            BeanUtils.copyNotNullProperties(roleInfo, roleOld);
            } catch (Exception e) {
	            throw new ExpectedException("", "Role拷贝bean出错[getRolesList]");
            }
    		roleInfos.add(roleInfo);
    	}
	    return roleInfos;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.service.IUserService#getCertList()
     */
    @Override
    public List<Crtificate> getCertsList() {
	    return certDao.getAll();
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.service.IUserService#getCertsListByUserId(java.lang.Integer)
     */
    @Override
	public List<Crtificate> getCertsListByUserId(Integer userId) {
		List<Crtificate> crtificates = new ArrayList<Crtificate>();
		// 1.封装map<id,Crtificate>
		List<Crtificate> crtificateList = certDao.getAll();
		Map<Integer, Crtificate> certMap = Maps.newHashMap();
		for (Crtificate crtificate : crtificateList) {
			certMap.put(crtificate.getId(), crtificate);
		}
		// 2.获取证书
		List<Usercrtificatemapping> usercrtificatemappings = usercrtificatemappingDao.findBy("userId", userId);
		for (Usercrtificatemapping usercrtificatemapping : usercrtificatemappings) {
			Crtificate crtificate = certMap.get(usercrtificatemapping.getNcrtificateId());
			crtificates.add(crtificate);
		}
		return crtificates;
	}
    
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.service.IUserService#updatePassword(java.lang.Integer, java.lang.String, java.lang.String)
     */
    @Override
    public AjaxData updatePassword(Integer userId, String password, String newpassword) {
    	AjaxData ajaxData = new AjaxData(false);
		Users user = usersDao.get(userId);
		if(user.getStrPassword().equals(password)) {
			if(!password.equals(newpassword)) {
				if(newpassword.equals(sysconfigService.getSysconfig().getPasswordDefault())){
					ajaxData.setFailureMsg("新密码与平台初始密码一致");
					return ajaxData;
				}
				// 更新密码、修改密码时间
				user.setStrPassword(newpassword);
				user.setStrUpdatePwdTime(new Date());
				user.setLastTimeOnline(new Date());
				usersDao.update(user);
			} else {
				ajaxData.setFailureMsg("新密码与当前密码相同");
			}
		} else {
			ajaxData.setFailureMsg("当前密码不正确");
		}
		return ajaxData;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.service.IUserService#updatePassword(java.lang.String, java.lang.String, java.lang.String)
     */
    @Override
    public AjaxData updatePassword(String username, String password, String newpassword) {
    	Users user = usersDao.findUniqueBy("strName", username);
		if(user == null) {
			AjaxData ajaxData = new AjaxData(false);
			ajaxData.setFailureMsg("用户名不存在");
			return ajaxData;
		}		 
		return updatePassword(user.getId(), password, newpassword);
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.service.IUserService#updateFristPassword(java.lang.Integer, java.lang.String)
     */
    @Override
	public AjaxData updateFristPassword(Integer userId, String newpassword) {
		AjaxData ajaxData = new AjaxData(false);
		Users user = usersDao.get(userId);		
		// 更新密码、修改密码时间、首次登录标志位改成1  空或者0是
		user.setStrPassword(newpassword);
		user.setStrComment(Constants.CONS_1.toString());
		user.setStrUpdatePwdTime(new Date());
		usersDao.update(user);
		return ajaxData;
	}
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.service.IUserService#getDevChannelInfo(java.lang.Integer)
     */
    @Override
    public RecordPojo getDevChannelInfo(Integer channelId) {
    	RecordPojo recordPojo = new RecordPojo();
	    Devicechannel devicechannel = deviceChannelDao.get(channelId);
	    recordPojo.setChannel(devicechannel.getNchannelNo());
	    recordPojo.setCamName(devicechannel.getNchannelName());
	    recordPojo.setRegionId(devicechannel.getNregionId());
	    
	   Device device =  deviceDao.get(devicechannel.getNdeviceId());
	   //0dvr 1nvr
	   if(DeviceType.DEV_TYPE_DVR == device.getNtype()){
		   recordPojo.setDevType(Constants.CONS_0);
	   }else if(DeviceType.DEV_TYPE_NVR == device.getNtype()){
		   recordPojo.setDevType(Constants.CONS_1);
	   }	   
	   recordPojo.setDevIp(device.getStrIp());
	   recordPojo.setDevPort(device.getNport());
	   recordPojo.setUser(device.getStrUser());
	   String pass = SercurityUtil.decrypt(device.getStrPassword());
	   recordPojo.setPwd(Base64Utils.getBase64(pass));
	   recordPojo.setRealPlayType(device.getNstreamType());
	    return recordPojo;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.service.IUserService#exportUsersExcel(com.hikvision.finance.fms.modules.user.dto.UserInfo, java.lang.String[], java.lang.String[], java.lang.String)
     */
    @Override
	public boolean exportUsersExcel(UserInfo userInfo, String[] titleName, String[] attrName, String path) {
		// 1，依次准备标题、标题映射属性、Sheet以及对应内容
		List<String[]> titleNames = new ArrayList<String[]>();
		List<String[]> attrNames = new ArrayList<String[]>();
		LinkedHashMap<String, List<?>> map = new LinkedHashMap<String, List<?>>();
		titleNames.add(titleName);
		attrNames.add(attrName);
		List<UserInfo> userInfos = usersDao.exportUsersExcel(userInfo);
		map.put("Sheet1", userInfos);
		// 2，创建Excel的数据对象并导出表格
		ExcelUtil.ExcelExportData setInfo = new ExcelUtil.ExcelExportData();
		setInfo.setColumnNames(titleNames);
		setInfo.setFieldNames(attrNames);
		setInfo.setDataMap(map);
		boolean exportResult = false;
		try {
			exportResult = ExcelUtil.export2File(setInfo, path);
			configLogDao.setLogContent(OperStatus.EXPORT, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.users.export"));
		} catch (Exception e) {
			e.printStackTrace();
		} 
		return exportResult;
		
	}
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.service.IUserService#downLoadTemplate(java.lang.String)
     */
    @Override
	public boolean downLoadTemplate(String path) {
		// 输入输出流
		InputStream is = null;
		FileOutputStream out = null;
		try {
			is = new FileInputStream(path);
			// 创建工作空间
			Workbook wb = WorkbookFactory.create(is);
			Sheet sheet = wb.getSheetAt(0);
			// 男女下拉
			String[] arraySex = {"男", "女"};
			sheet = setHSSFValidation(sheet, arraySex, 1, 2000, 3, 3);
			// 用户类型下拉
			String[] arrayUserType = {SysRoleEnum.CENTRY_ADMIN_0.getDescribe(), SysRoleEnum.FINANC_MANAGER_1.getDescribe()};
			sheet = setHSSFValidation(sheet, arrayUserType, 1, 2000, 4, 4);
			
			// 创建文件（输出到目标地址）
			String rootpath = ServletActionContext.getServletContext().getRealPath("/");
			String path1 = Constants.FILEUPLOAD + "excel/" + "usersTemplate.xls";
			File saveFile = new File(rootpath + Constants.FILEUPLOAD + "excel/");
			if (!saveFile.exists()) {
				saveFile.mkdirs();
			}
			out = new FileOutputStream(rootpath + path1);
			// 写出文件
			wb.write(out);
			out.flush();
		} catch (Exception e) {
			e.printStackTrace();
			throw new ExpectedException("", "下载模板出错:" + e.getMessage());
		} finally {
			try {
				if (null != is) {
					is.close();
				}
				if (null != out) {
					out.close();
				}
            } catch (IOException e) {
	            e.printStackTrace();
            }
		}
		return true;
	}
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.service.IUserService#addProFromExcel(java.io.File, java.lang.String, com.hikvision.finance.fms.common.excel.model.ExcelHead)
     */
    @Override
	public Map<String, Object> addUserFromExcel(File userFile, String userFileFileName, ExcelHead excelHead) {
		Map<String, Object> prosMap = Maps.newHashMap();
		prosMap.put("errorNo", true);
		List<UserInfo> pros = Lists.newArrayList();
		ExcelHelper<UserInfo> excelHelper = ExcelHelperFactory.createExcelHelper(userFileFileName);
		pros = excelHelper.importToObjectList(excelHead, userFile, UserInfo.class);
		Map<String, UserInfo> strNameMap = new HashMap<String, UserInfo>();
		Map<String, UserInfo> userNoMap = new HashMap<String, UserInfo>();
		// 校验导入表格中，编码是否有重复
		if (null != pros) {// 因coverity检测出空指针判断缺陷添加
			for (UserInfo info : pros) {
				String strName = null == info.getStrName()?"":info.getStrName();
				String userNo = null == info.getUserNo()?"":info.getUserNo();
				if (StringUtil.isEmpty(strName) || StringUtil.isEmpty(userNo)) {
					// throw new ExpectedException("", "导入表格中名称或者员工编号含有空单元,无法导入");
					prosMap.put("errorNo", false);
					prosMap.put("errorInfo", "导入表格中名称或者员工编号含有空单元,无法导入");
					break;
				}
				UserInfo strNameInfo = strNameMap.get(strName);
				UserInfo userNoInfo = userNoMap.get(userNo);
				if (strNameInfo != null) {
					prosMap.put("errorNo", false);
					prosMap.put("errorInfo", "导入表格中名称有重复,无法导入");
					break;
					// throw new ExpectedException("", "导入表格中名称有重复,无法导入");
				}
				strNameMap.put(strName, info);
				if (userNoInfo != null) {
					prosMap.put("errorNo", false);
					prosMap.put("errorInfo", "导入表格中员工编号有重复,无法导入");
					break;
					// throw new ExpectedException("", "导入表格中员工编号有重复,无法导入");
				}
				userNoMap.put(userNo, info);
			}
		}
		prosMap.put("prosList", pros);
		return prosMap;
	}
    
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.service.IUserService#addAndValidateExcel(java.io.File, java.lang.String, com.hikvision.finance.fms.common.excel.model.ExcelHead)
     */
    @Override
	public AjaxData addAndValidateExcel(File userFile, String userFileFileName, ExcelHead excelHead) {
		AjaxData ajaxData = new AjaxData(false);
		Map<String, Object> prosMap = Maps.newHashMap();
		List<UserInfo> pros = Lists.newArrayList();
		try {
			ExcelHelper<UserInfo> excelHelper = ExcelHelperFactory.createExcelHelper(userFileFileName);
			pros = excelHelper.importToObjectList(excelHead, userFile, UserInfo.class);
			List<Object> mapNameList = Lists.newArrayList();// 存放表格内名称
			List<Object> mapUsernoList = Lists.newArrayList();// 存放表格内员工编号
			if (pros == null || pros.size() < 1) {
				ajaxData.setFailureMsg("导入的表格内关键数据缺失或无数据");
				return ajaxData;
			}
			for (UserInfo info : pros) {
				info.setStrName(info.getStrName().trim()); //人员导入时，用户名去空操作
				// 判断必填项是否为空
				if (StringUtils.isBlank(info.getStrName())) {
					ajaxData.setFailureMsg("用户名列数据不全");
					return ajaxData;
				}
				if (StringUtils.isBlank(info.getUserNo())) {
					ajaxData.setFailureMsg("员工编号列数据不全");
					return ajaxData;
				}
				if (StringUtils.isBlank(info.getStrCode())) {
					ajaxData.setFailureMsg("真实姓名列数据不全");
					return ajaxData;
				}
				if (StringUtils.isBlank(info.getNet())) {
					ajaxData.setFailureMsg("所在网点列数据不全");
					return ajaxData;
				}
				if (info.getStrDate() == null) {
					ajaxData.setFailureMsg("到期时间列数据不全");
					return ajaxData;
				}
				
				// 校验表格内用户名和员工编号是否重复
				if (mapNameList.contains(info.getStrName())) {
					ajaxData.setFailureMsg("用户名列表中存在重复数据：" + info.getStrName());
					return ajaxData;
				}
				
				if (mapUsernoList.contains(info.getUserNo())) {
					ajaxData.setFailureMsg("员工编号列表中存在重复数据：" + info.getUserNo());
					return ajaxData;
				}
				mapNameList.add(info.getStrName());
				mapUsernoList.add(info.getUserNo());
			}
		} catch (Exception e) {
			ajaxData.setFailureMsg("导入文件的内容格式有误,请检查文件内容是否合规");
			return ajaxData;
		}
		prosMap.put("prosList", pros);
		ajaxData.setData(prosMap);
		return ajaxData;
	}
    
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.service.IUserService#addAllUsers(java.util.List)
     */
    @Override
    public AjaxData addAllUsers(List<UserInfo> userInfos) {
    	AjaxData ajaxData = new AjaxData(false);
    	StringBuilder builderStrName = new StringBuilder();
    	//查询用户角色 组装map<id,role>
    	Map<String, Role> roleMap = new HashMap<String, Role>();
    	List<Role> roles = roleDao.getAll();
    	for(Role role: roles){
    		roleMap.put(role.getStrName(), role);
    	}
    	
    	//查询网点 组装map<id,region>
    	Map<String, Region> regionMap = new HashMap<String, Region>();
    	List<Region> regions = regionDao.getAll();
    	for(Region region: regions){
    		regionMap.put(region.getStrName(), region);
    	}
    	
    	//用户信息组装map<用户名称，用户>
    	Map<String, Users> userNameMap = new HashMap<String, Users>();    	
    	//用户信息组装map<员工编号，用户>
    	Map<String, Users> userNoMap = new HashMap<String, Users>();
    	List<Users> users = usersDao.getAll();
    	for(Users user : users){
    		userNameMap.put(user.getStrName(), user);
    		userNoMap.put(user.getUserNo(), user);
    	}
    	SysconfigDto sysInfo = sysconfigService.getSysconfig();
    	Userrolemapping userrolemapping = null;
    	int sucessNum=0;
    	int faileNum=0;
    	
    	//数据有效性校验
    	for(UserInfo  info:userInfos){
			if(!validateCoulm(info)){
				ajaxData.setFailureMsg("文件内容含非法数据或有特殊字符\\/:*?\"<|'%>&");
				return ajaxData;
			}
    	}
    	for(UserInfo  info:userInfos){
    		//判断用户名称是否和数据库重复
    		Users user1 = userNameMap.get(info.getStrName());
    		if(user1 != null){
    			builderStrName.append(info.getStrName()+",用户名:"+info.getStrName()+" 已存在&");
    			faileNum++;
    			continue;
    		}
    		//判断员工编号是否和数据库重复
    		Users user2 = userNoMap.get(info.getUserNo());
    		if(user2 != null){
    			builderStrName.append(info.getStrName()+",员工编号:"+info.getUserNo()+" 已存在&");
    			faileNum++;
    			continue;
    		}

    		//对导入网点进行判断
    		Region region = regionMap.get(info.getNet());
    		if(region == null){
    			builderStrName.append(info.getStrName()+",网点:"+info.getNet()+" 不存在   <br/>&");
    			faileNum++;
    			continue;
    		}
    		//对导入的角色 进行判断
    		if(info.getRoleName()!=null){
        		Role role = roleMap.get(info.getRoleName());
        		if(role == null){
        			builderStrName.append(info.getStrName()+",角色:"+info.getRoleName()+" 不存在&");
        			faileNum++;
        			continue;
        		}
    		}
    		Users user = new Users();
    		user.setStrName(info.getStrName());
    		user.setStrCode(info.getStrCode());
    		user.setSysRole(info.getSysRole());
    		//获取默认密码添加到数据库
    		user.setStrPassword(sysInfo.getPasswordDefault());
    		user.setStrTel(null==info.getStrTel()?"":info.getStrTel());
    		user.setStrEmail(null==info.getStrEmail()?"":info.getStrEmail());
    		user.setStrComment(null==info.getStrComment()?"":info.getStrComment());
    		user.setNstate(Constants.UserStatus.NORMAL);
    		user.setNonline(Constants.UserStatus.NOTONLINE);
    		user.setStrDate(info.getStrDate());
    		user.setStrUpdatePwdTime(new Date());
    		user.setNsex(info.getNsex());
    		user.setNet(info.getNet());
    		user.setNetId(regionMap.get(info.getNet()).getId());//设备网点id
    		user.setStrDuty(null==info.getStrDuty()?"":info.getStrDuty());
    		user.setUserNo(info.getUserNo());
    		UserSession userSession = SessionUtil.getUserSession();
    		user.setNowner(userSession.getUserId());
    		user.setStrCredentialNo(null==info.getStrCredentialNo()?"":info.getStrCredentialNo());
    		Integer userId = (Integer)usersDao.save(user);   
    		//中心管理员需关联角色
    		if(info.getSysRole() == 0){
    			Role nrole =  roleMap.get(info.getRoleName());
    			userrolemapping = new Userrolemapping();
    			userrolemapping.setNuserId(userId);
    			userrolemapping.setNroleId(nrole.getId());
    			userrolemappingDao.save(userrolemapping);
    		}
    		sucessNum++;
    	}
		String msg = "导入失败的用户：失败个数( "+faileNum+" )个。&"+builderStrName.toString();   	

    	if(faileNum>0){
    		ajaxData.setSuccess(true);
    		ajaxData.setMsg(msg);
    	}
		if (sucessNum > 0) {
			configLogDao.setLogContent(OperStatus.IMPORT, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.users.import"));
		}
		return ajaxData;
    }
	// ***********************私有方法*********************************************
    
    /**
     * 校验导入的数据
     * @author jinxindong 2016年6月15日 下午5:02:37
     * @param userInfo
     * @return
     */
	private Boolean validateCoulm(UserInfo userInfo) {

		if(ValidateUtil.validateChar(userInfo.getStrName())){
			return false;
		}
		if(ValidateUtil.validateChar(userInfo.getUserNo())){
			return false;
		}
		if(ValidateUtil.validateChar(userInfo.getStrCode())){
			return false;
		}
		if(ValidateUtil.validateChar(userInfo.getRoleName())){
			return false;
		}
		if(ValidateUtil.validateChar(userInfo.getNet())){
			return false;
		}
		if(ValidateUtil.validateChar(userInfo.getStrCredentialNo())){
			return false;
		}
		if(ValidateUtil.validateChar(userInfo.getStrTel())){
			return false;
		}
		if(ValidateUtil.validateChar(userInfo.getStrEmail())){
			return false;
		}
		if(ValidateUtil.validateChar(userInfo.getStrDuty())){
			return false;
		}
		if(ValidateUtil.validateChar(userInfo.getStrComment())){
			return false;
		}
		if(!ValidateUtil.validateEamil(userInfo.getStrEmail())){
			return false;
		}
		return true;
	}
	/**
	 * 删除用户和设备的关联关系
	 * @author jinxindong 2016年3月3日 下午7:04:28
	 * @param ids
	 * @return
	 */
	private void delUserDeviceMapping(List<Integer> ids) {
		Boolean mappingStatus = false;
		// 迭代判断是否有关联关系
		for (Integer userId : ids) {
			List<Userdevicemapping> list = userdevicemappingDao.findBy("nuserId", userId);
			if (list != null && list.size() > 0) {
				mappingStatus = true;
				break;
			}
		}
		// 判断是否有关联关系，有才删除
		if (mappingStatus) {
			this.userdevicemappingDao.delUserDeviceMapping(ids);
		}
	}
	
	/**
	 * 删除用户和证书的关联关系
	 * @author jinxindong 2016年3月3日 下午7:04:34
	 * @param ids
	 * @return
	 */
	private void delUserCrtificateMapping(List<Integer> ids) {
		Boolean mappingStatus = false;
		// 迭代判断是否有关联关系
		for (Integer userId : ids) {
			List<Usercrtificatemapping> list = usercrtificatemappingDao.findBy("userId", userId);
			if (list != null && list.size() > 0) {
				mappingStatus = true;
				break;
			}
		}
		// 判断是否有关联关系，有才删除
		if (mappingStatus) {
			this.usercrtificatemappingDao.delUserCrtificateMapping(ids);
		}
	}
	
	/**
	 * 删除用户和角色的关联关系
	 * @author jinxindong 2016年3月3日 下午7:04:39
	 * @param ids
	 * @return
	 */
	private void delUserRoleMapping(List<Integer> ids) {
		Boolean mappingStatus = false;
		// 迭代判断是否有关联关系
		for (Integer userId : ids) {
			List<Userrolemapping> list = userrolemappingDao.findBy("nuserId", userId);
			if (list != null && list.size() > 0) {
				mappingStatus = true;
				break;
			}
		}
		// 判断是否有关联关系，有才删除
		if (mappingStatus) {
			this.userrolemappingDao.delUserRoleMapping(ids);
		}
	}
	
	/**
	 * 删除用户和区域的关联关系
	 * @author jinxindong 2016年3月3日 下午7:04:45
	 * @param ids
	 * @return
	 */
	private void delUserRegionMapping(List<Integer> ids) {
		Boolean mappingStatus = false;
		// 迭代判断是否有关联关系
		for (Integer userId : ids) {
			List<Userregionmapping> list = userregionmappingDao.findBy("nuserId", userId);
			if (list != null && list.size() > 0) {
				mappingStatus = true;
				break;
			}
		}
		// 判断是否有关联关系，有才删除
		if (mappingStatus) {
			this.userregionmappingDao.delUserRegionMapping(ids);
		}
	}
	
	/**
	 * 判断用户的身份：中心管理员或者理财经理
	 * @author jinxindong 2016年3月4日 下午2:41:02
	 * @param userId
	 * @return
	 */
	private String getUserSysType(Integer userId) {
		String code = "";
		Users user = this.usersDao.get(userId);
		code = String.valueOf(user.getSysRole());
		return code;
	}
	
    /**
     * 设置下拉
     * @author jinxindong 2016年4月22日 上午11:38:21
     * @param sheet
     * @param textlist
     * @param firstRow
     * @param endRow
     * @param firstCol
     * @param endCol
     * @return
     */
    public Sheet setHSSFValidation(Sheet sheet, String[] textlist, int firstRow, int endRow, int firstCol, int endCol) {
		// 加载下拉列表内容
		DVConstraint constraint = DVConstraint.createExplicitListConstraint(textlist);
		// 设置数据有效性加载在哪个单元格上,四个参数分别是：起始行、终止行、起始列、终止列
		CellRangeAddressList regions = new CellRangeAddressList(firstRow, endRow, firstCol, endCol);
		// 数据有效性对象
		HSSFDataValidation data_validation_list = new HSSFDataValidation(regions, constraint);
		sheet.addValidationData(data_validation_list);
		return sheet;
	}
    
	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.user.service.IUserService#getUserByName(java.lang.String)
	 */
	@Override
	public Users getUserByName(String username) {
		return usersDao.findUniqueBy("strName", username);
	}
	
    /* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.service.IUserService#getUserLockInfo(java.lang.Integer)
     */
    @Override
    public Userlock getUserLockInfo(Integer userId) {
    	return null;
    }
    
	// ***********************getter/setter*********************************************
	
	public IUserrolemappingDao getUserrolemappingDao() {
		return userrolemappingDao;
	}
	
	public IUsersDao getUsersDao() {
		return usersDao;
	}
	
	public void setUsersDao(IUsersDao usersDao) {
		this.usersDao = usersDao;
	}
	
	public void setUserrolemappingDao(IUserrolemappingDao userrolemappingDao) {
		this.userrolemappingDao = userrolemappingDao;
	}
	
	public IUsercrtificatemappingDao getUsercrtificatemappingDao() {
		return usercrtificatemappingDao;
	}
	
	public void setUsercrtificatemappingDao(IUsercrtificatemappingDao usercrtificatemappingDao) {
		this.usercrtificatemappingDao = usercrtificatemappingDao;
	}
	
	public IUserdevicemappingDao getUserdevicemappingDao() {
		return userdevicemappingDao;
	}
	
	public void setUserdevicemappingDao(IUserdevicemappingDao userdevicemappingDao) {
		this.userdevicemappingDao = userdevicemappingDao;
	}
	
	public IUserregionmappingDao getUserregionmappingDao() {
		return userregionmappingDao;
	}
	
	public void setUserregionmappingDao(IUserregionmappingDao userregionmappingDao) {
		this.userregionmappingDao = userregionmappingDao;
	}
	
	public IRoleDao getRoleDao() {
		return roleDao;
	}
	
	public void setRoleDao(IRoleDao roleDao) {
		this.roleDao = roleDao;
	}
	
	public ISysconfigService getSysconfigService() {
		return sysconfigService;
	}
	
	public void setSysconfigService(ISysconfigService sysconfigService) {
		this.sysconfigService = sysconfigService;
	}
	
	public ICertDao getCertDao() {
		return certDao;
	}
	
	public void setCertDao(ICertDao certDao) {
		this.certDao = certDao;
	}
	
	public IRegionDao getRegionDao() {
		return regionDao;
	}
	
	public void setRegionDao(IRegionDao regionDao) {
		this.regionDao = regionDao;
	}
	
	public IDeviceChannelDao getDeviceChannelDao() {
		return deviceChannelDao;
	}
	
	public void setDeviceChannelDao(IDeviceChannelDao deviceChannelDao) {
		this.deviceChannelDao = deviceChannelDao;
	}
	
	public DataManager getDataManager() {
		return dataManager;
	}
	
	public void setDataManager(DataManager dataManager) {
		this.dataManager = dataManager;
	}
	
	public IDeviceDao getDeviceDao() {
		return deviceDao;
	}
	
	public void setDeviceDao(IDeviceDao deviceDao) {
		this.deviceDao = deviceDao;
	}
	
	public IConfigLogDao getConfigLogDao() {
		return configLogDao;
	}
	
	public void setConfigLogDao(IConfigLogDao configLogDao) {
		this.configLogDao = configLogDao;
	}
	
	public IBusinessOrderDao getBusinessOrderDao() {
		return businessOrderDao;
	}
	
	public void setBusinessOrderDao(IBusinessOrderDao businessOrderDao) {
		this.businessOrderDao = businessOrderDao;
	}
	
	public IRecordsDao getRecordsDao() {
		return recordsDao;
	}
	
	public void setRecordsDao(IRecordsDao recordsDao) {
		this.recordsDao = recordsDao;
	}



}
