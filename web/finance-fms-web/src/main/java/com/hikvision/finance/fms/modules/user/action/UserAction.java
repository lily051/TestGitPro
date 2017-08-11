/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月29日 下午12:09:29
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.action;

import java.io.File;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.struts2.ServletActionContext;

import com.google.common.collect.Lists;
import com.google.common.collect.Maps;
import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.fms.common.excel.model.ExcelColumn;
import com.hikvision.finance.fms.common.excel.model.ExcelHead;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.model.Crtificate;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.role.dto.RoleInfo;
import com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService;
import com.hikvision.finance.fms.modules.user.dto.UserInfo;
import com.hikvision.finance.fms.modules.user.model.RecordPojo;
import com.hikvision.finance.fms.modules.user.service.IUserService;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.www.action.BaseAction;


/**
 * <p>权限管理：用户管理模块</p>
 * @author jinxindong 2016年2月29日 下午12:09:29
 * @version V1.0 
 */
public class UserAction extends BaseAction<Users> {

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 8104821263214487416L;
    private IUserService userService;
    private ISysconfigService sysconfigService;
    private List<Integer> ids;
    private List<RoleInfo> roleList;
    private List<Crtificate> certList;
    private List<Crtificate> certAllList;
    private UserInfo userInfo;
    private String passwordSecurityLevel;
    private RecordPojo recordPojo;
	private File userFile;// 导入机构的文件
	private String userFileFileName;// 导入机构的文件名
	private Map<String,Object> resMap; 
	
	
    public Map<String, Object> getResMap() {
    	return resMap;
    }

	
    public void setResMap(Map<String, Object> resMap) {
    	this.resMap = resMap;
    }
	/**
	 * 获取用户管理页面
	 * @author jinxindong 2016年3月4日 上午10:01:32
	 * @return
	 */
	public String toUsersPage() {
		operPage = "/modules/user/user.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 获取用户分页数据
	 * @author jinxindong 2016年3月4日 上午10:01:50
	 * @return
	 */
	public String getDataPage() {
		page = userService.getDataPage(limit, start, userInfo);
		return PAGE;
	}
	
    /**
     * 用户添加界面
     * @author jinxindong 2016年3月3日 上午10:52:52
     * @return
     */
    public String toAddUserPage() {
    	userInfo = new UserInfo();
    	userInfo.setStrDate(DateUtils.addYears(new Date(), 3));
    	operPage = "/modules/user/dialogs/toAddUserPage.jsp";
		return DISPATCHER;
    }
    
    /**
     * 调到权限树展示界面
     * @author jinxindong 2016年3月10日 下午6:43:27
     * @return
     */
    public String toChooseAuthPage() {
    	operPage = "/modules/user/dialogs/chooseAuth-win.jsp";
    	return DISPATCHER;
    }
    
    /**
     * 选择网点页面
     * @author jinxindong 2016年4月28日 上午10:03:37
     * @return
     */
    public String toChooseNetPage() {
    	operPage = "/modules/user/dialogs/chooseNet.jsp";
    	return DISPATCHER;
    }
    
    /**
     * 添加用户
     * @author jinxindong 2016年3月3日 上午10:54:59
     * @return
     */
    public String addUser() {
    	ajaxData = userService.addUser(userInfo,jsonStr);
		return AJAX;
    }
    
    
	/**
	 * 删除、批量删除用户
	 * @author jinxindong 2016年2月29日 下午3:11:05
	 * @return
	 */
	public String deleteUsers() {
		ajaxData = userService.deleteUsers(ids);
		return AJAX;
	}
	
	/**
	 * 跳转到更新用户的编辑页面
	 * @author jinxindong 2016年3月3日 上午11:09:18
	 * @return
	 */
	public String toUpdatePage(){
		//请求基本信息，请求角色列表，请求组织树（中心管理员），请求监控点树（理财经理），请求证书（理财经理）
		if (userInfo == null || userInfo.getId() == null) {
			throw new ExpectedException("", "未获取到该用户ID");
		}
		passwordSecurityLevel = sysconfigService.getSysconfig().getPasswordSecruityLevel();
		userInfo = userService.getUserById(userInfo.getId());
		operPage = "/modules/user/dialogs/toEditUserPage.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 请求角色列表
	 * @author jinxindong 2016年3月4日 下午2:11:21
	 * @return
	 */
	public String getRolesList() {
		roleList = userService.getRolesList();
		ajaxData.put("roleList",roleList);
		return AJAX;
	}
	
	/**
	 * 跳转到获取证书列表页面(添加)
	 * @author jinxindong 2016年3月15日 上午10:00:04
	 * @return
	 */
	public String toCertsAddPage(){
		certAllList = userService.getCertsList();
		operPage = "/modules/user/dialogs/chooseCert-win.jsp";
    	return DISPATCHER;
	}
		
	/**
	 * 跳转到获取证书列表页(编辑)
	 * @author jinxindong 2016年3月15日 上午10:00:04
	 * @return
	 */
	public String toCertsEditPage(){
		certAllList = userService.getCertsList();
		operPage = "/modules/user/dialogs/chooseCert-win.jsp";
    	return DISPATCHER;
	}
	
	/**
	 * 更新用户
	 * @author jinxindong 2016年3月3日 上午11:15:22
	 * @return
	 */
    public String updateUser() {
		if (userInfo == null || userInfo.getId() == null) {
			throw new ExpectedException("", "未获取到该用户ID");
		}
    	ajaxData = userService.updateUser(userInfo,jsonStr);
		return AJAX;
    }
    
    /**
     * 重置密码
     * 需要从系统参数表中去取初始密码
     * @author jinxindong 2016年3月3日 上午11:17:48
     * @return
     */
	public String resetPassword() {
		ajaxData = userService.resetPassword(ids);
		return AJAX;
	}
	
	/**
	 * 解禁用户（启用）
	 * @author jinxindong 2016年3月3日 上午11:19:04
	 * @return
	 */
	public String enableUsers() {
		ajaxData = userService.enableUsers(ids);
		return AJAX;
	}
	/**
	 * 禁用用户
	 * @author jinxindong 2016年3月3日 上午11:19:10
	 * @return
	 */
	public String disableUsers() {
		ajaxData = userService.disableUsers(ids);
		return AJAX;
	}
	
	/**
	 * 获取信道信息
	 * @author jinxindong 2016年4月20日 上午9:25:22
	 * @return
	 */
	public String getDevChannelInfo() {
		RecordPojo pojo = userService.getDevChannelInfo(recordPojo.getChannelId());
		ajaxData.put("recordPojo", pojo);
		return AJAX;
	}
	
	/**
	 * 导出用户信息
	 * @author jinxindong 2016年4月20日 下午5:02:44
	 * @return
	 */
	public String exportUsersExcel() {
		String[] titleNames = new String[] {"用户名", "员工编号",  "真实姓名", "用户类型", "角色分配", "启用状态", "所在网点", "到期时间"};
		String[] attrNames = new String[] {"strName", "userNo",  "strCode", "sysTypeName", "roleName", "nstateName", "net", "strDate"};
		String rootpath = ServletActionContext.getServletContext().getRealPath("/");
		String path = Constants.FILEUPLOAD + "excel/" + "usersQuery.xls";
		if(userService.exportUsersExcel(userInfo, titleNames, attrNames, rootpath + path)) {
			ajaxData.put("url", "/" + path);
    		success = true;
		} else {
			msg = "导出失败";
    		success = false;
		}
		return AJAX;
	}
	
	/**
	 * 跳转到下载模板界面
	 * @author jinxindong 2016年4月25日 上午9:44:43
	 * @return
	 */
	public String toCheckOutPage() {
		operPage = "/modules/user/dialogs/import.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 下载模板
	 * @author jinxindong 2016年4月25日 上午9:44:38
	 * @return
	 */
	public String downLoadTemplate() {
		String rootpath = ServletActionContext.getServletContext().getRealPath("/");
		String path ="modules/user/doc/" + "usersTemplate.xls";
		String path1 = Constants.FILEUPLOAD + "excel/" + "usersTemplate.xls";
		if(userService.downLoadTemplate(rootpath + path)) {
			ajaxData.put("url", "/" + path1);
    		success = true;
		} else {
			msg = "下载失败";
    		success = false;
		}
		return AJAX;
	}
	
	/**
	 * 上传Excel 
	 * @author jinxindong 2016年4月25日 上午9:44:33
	 * @return
	 */
	@SuppressWarnings({"serial", "unchecked"})
	public String userUploadExcel() {
		resMap = new HashMap<String, Object>();
		if (this.userFile != null) {
			// 1、excel的每行显示名称-数据库名称
			List<ExcelColumn> excelColumns = Lists.newArrayList();
			excelColumns.add(new ExcelColumn(0, "strName", "用户名*"));
			excelColumns.add(new ExcelColumn(1, "userNo", "员工编号*"));
			excelColumns.add(new ExcelColumn(2, "strCode", "真实姓名*"));
			excelColumns.add(new ExcelColumn(3, "nsex", "性别*"));
			excelColumns.add(new ExcelColumn(4, "sysRole", "用户类型*"));
			excelColumns.add(new ExcelColumn(5, "roleName", "角色分配*"));
			excelColumns.add(new ExcelColumn(6, "net", "所在网点*"));
			excelColumns.add(new ExcelColumn(7, "strDate", "到期时间*"));
			excelColumns.add(new ExcelColumn(8, "strCredentialNo", "证件号"));
			excelColumns.add(new ExcelColumn(9, "strTel", "手机"));
			excelColumns.add(new ExcelColumn(10, "strEmail", "邮件"));
			excelColumns.add(new ExcelColumn(11, "strDuty", "职务"));
			excelColumns.add(new ExcelColumn(12, "strComment", "备注"));
			
			// 2、机构级别中显示的名称-数据库中整数
			Map<String, Integer> sexMap = new HashMap<String, Integer>() {
				
				{
					put("男", 1);
					put("女", 0);
				}
			};
			Map<String, Integer> sysTypeMap = new HashMap<String, Integer>() {
				
				{
					put("中心管理员", 0);
					put("理财经理", 1);
				}
			};
			// 3、excel中显示的信息转换为数据库中的值
			Map<String, Map<?, ?>> excelColumnsConvertMap = Maps.newHashMap();
			excelColumnsConvertMap.put("nsex", sexMap);
			excelColumnsConvertMap.put("sysRole", sysTypeMap);
			// 4、组装excel信息
			ExcelHead excelHead = new ExcelHead();
			excelHead.setRowCount(1);// 模板头部所占行数
			excelHead.setColumnCount(11);
			excelHead.setColumns(excelColumns);// 列的定义
			excelHead.setColumnsConvertMap(excelColumnsConvertMap);// 列的转换
			// List<UserInfo> userInfos = userService.addUserFromExcel(userFile, userFileFileName, excelHead);
			// Map<String,Object> prosMap = userService.addUserFromExcel(userFile, userFileFileName, excelHead);
			ajaxData = userService.addAndValidateExcel(userFile, userFileFileName, excelHead);
			if (ajaxData.getSuccess()) {
				// 批量添加prosList
				List<UserInfo> userInfos = (List<UserInfo>)ajaxData.getData().get("prosList");
				ajaxData = userService.addAllUsers(userInfos);
			}
			resMap.put("errorNo", ajaxData.getSuccess());
			resMap.put("errorInfo", ajaxData.getMsg());
		}
		return "upload";
	}
	
	/**
	 * 导入的名称或者员工编号有重复，则跳转到该页面展示
	 * @author jinxindong 2016年4月25日 下午12:49:07
	 * @return
	 */
	public String toAlreadyImport() {
		operPage = "/modules/user/dialogs/alreadyImport.jsp";
		return DISPATCHER;
	}
	//===============================getter/setter=====================================

	
    public IUserService getUserService() {
    	return userService;
    }

	
    public void setUserService(IUserService userService) {
    	this.userService = userService;
    }

	
    public List<Integer> getIds() {
    	return ids;
    }

	
    public void setIds(List<Integer> ids) {
    	this.ids = ids;
    }

	
    public UserInfo getUserInfo() {
    	return userInfo;
    }

	
    public void setUserInfo(UserInfo userInfo) {
    	this.userInfo = userInfo;
    }

	
    public List<RoleInfo> getRoleList() {
    	return roleList;
    }

	
    public void setRoleList(List<RoleInfo> roleList) {
    	this.roleList = roleList;
    }

	
    public List<Crtificate> getCertList() {
    	return certList;
    }

	
    public void setCertList(List<Crtificate> certList) {
    	this.certList = certList;
    }

	
    public List<Crtificate> getCertAllList() {
    	return certAllList;
    }

	
    public void setCertAllList(List<Crtificate> certAllList) {
    	this.certAllList = certAllList;
    }

	
    public String getPasswordSecurityLevel() {
    	return passwordSecurityLevel;
    }

	
    public void setPasswordSecurityLevel(String passwordSecurityLevel) {
    	this.passwordSecurityLevel = passwordSecurityLevel;
    }

	
    public ISysconfigService getSysconfigService() {
    	return sysconfigService;
    }

	
    public void setSysconfigService(ISysconfigService sysconfigService) {
    	this.sysconfigService = sysconfigService;
    }

	
    public RecordPojo getRecordPojo() {
    	return recordPojo;
    }

	
    public void setRecordPojo(RecordPojo recordPojo) {
    	this.recordPojo = recordPojo;
    }

	
    public File getUserFile() {
    	return userFile;
    }

	
    public void setUserFile(File userFile) {
    	this.userFile = userFile;
    }

	
    public String getUserFileFileName() {
    	return userFileFileName;
    }

	
    public void setUserFileFileName(String userFileFileName) {
    	this.userFileFileName = userFileFileName;
    }

	



	

	
}
