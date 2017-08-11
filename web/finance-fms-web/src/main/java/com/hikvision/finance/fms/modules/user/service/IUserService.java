/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月3日 上午10:47:20
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.service;

import java.io.File;
import java.util.List;
import java.util.Map;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.common.excel.model.ExcelHead;
import com.hikvision.finance.fms.model.Crtificate;
import com.hikvision.finance.fms.model.Userlock;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.role.dto.RoleInfo;
import com.hikvision.finance.fms.modules.user.dto.UserInfo;
import com.hikvision.finance.fms.modules.user.model.RecordPojo;
import com.hikvision.finance.fms.modules.user.qo.UsersQo;
import com.hikvision.finance.fwork.util.AjaxData;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年3月3日 上午10:47:20
 * @version V1.0
 */
public interface IUserService extends IBaseService<Users, UsersQo> {
	
	/**
	 * 添加用户
	 * @author jinxindong 2016年3月3日 上午11:04:39
	 * @param jsonStr
	 * @return
	 */
	public AjaxData addUser(UserInfo userInfo,String jsonStr);
	
	/**
	 * 删除或者批量删除用户
	 * @author jinxindong 2016年3月3日 上午11:08:17
	 * @param ids
	 * @return
	 */
	public AjaxData deleteUsers(List<Integer> ids);
	
	/**
	 * 编辑用户
	 * @author jinxindong 2016年3月3日 上午11:12:03
	 * @param jsonStr
	 * @return
	 */
	public AjaxData updateUser(UserInfo info,String jsonStr);
	
	/**
	 * 重置密码
	 * @author jinxindong 2016年3月3日 上午11:20:05
	 * @param ids
	 * @return
	 */
	public AjaxData resetPassword(List<Integer> ids);
	
	/**
	 * 启用用户
	 * @author jinxindong 2016年3月3日 上午11:20:25
	 * @param ids
	 * @return
	 */
	public AjaxData enableUsers(List<Integer> ids);
	
	/**
	 * 禁用用户
	 * @author jinxindong 2016年3月3日 上午11:20:38
	 * @param ids
	 * @return
	 */
	public AjaxData disableUsers(List<Integer> ids);
	
	
	/**
	 * 获取组织分页数据
	 * @author jinxindong 2016年3月4日 上午10:09:44
	 * @param limit
	 * @param start
	 * @param strName
	 * @param userNo
	 * @param nonline
	 * @return
	 */
	public Page getDataPage(Integer limit,Integer start,UserInfo userInfo);
	
	/**
	 * 
	 * @author jinxindong 2016年3月4日 下午1:51:32
	 * @param userId
	 * @return
	 */
	public UserInfo getUserById(Integer userId);
	
	/**
	 * 获取角色列表
	 * @author jinxindong 2016年3月4日 下午2:12:49
	 * @return
	 */
	public List<RoleInfo> getRolesList(); 
	
	/**
	 * 获取证书列表
	 * @author jinxindong 2016年3月10日 上午9:08:30
	 * @return
	 */
	public List<Crtificate> getCertsList(); 
	
	/**
	 * 根据用户id获取用户的证书列表
	 * @author jinxindong 2016年3月10日 上午9:47:08
	 * @param userId
	 * @return
	 */
	public List<Crtificate> getCertsListByUserId(Integer userId); 
	
	/**
	 * 修改密码处理
	 * @author jinxindong 2016年4月1日 下午5:34:47
	 * @param userId
	 * @param password
	 * @param newpassword
	 * @return
	 */
	AjaxData updatePassword(Integer userId, String password, String newpassword);
	
	/**
	 * 修改密码处理
	 * @author jinxindong 2016年4月1日 下午5:36:08
	 * @param username
	 * @param password
	 * @param newpassword
	 * @return
	 */
	AjaxData updatePassword(String username, String password, String newpassword);
	
	/**
	 * 首次登录修改密码
	 * @author jinxindong 2016年4月5日 上午10:20:28
	 * @param userId
	 * @param newpassword
	 * @return
	 */
	AjaxData updateFristPassword(Integer userId, String newpassword);

	/**
	 * 根据信道id获取回放视频所需信息
	 * @author jinxindong 2016年4月20日 上午9:28:26
	 * @param channelId
	 * @return
	 */
	public RecordPojo getDevChannelInfo(Integer channelId);
	
	/**
	 * 导出用户
	 * @author jinxindong 2016年4月20日 下午2:23:24
	 * @param userInfo
	 * @param titleName
	 * @param attrName
	 * @param path
	 * @return
	 */
	public boolean exportUsersExcel(UserInfo userInfo, String[] titleName, String[] attrName, String path);
	
	/**
	 * 下载模板
	 * @author jinxindong 2016年4月25日 上午9:48:57
	 * @param path
	 * @return
	 */
	public boolean downLoadTemplate(String path);
	
	/**
	 * 导入表格
	 * @author jinxindong 2016年4月25日 上午11:21:37
	 * @param userFile
	 * @param userFileFileName
	 * @param excelHead
	 * @return
	 */
	public Map<String,Object> addUserFromExcel(File userFile, String userFileFileName, ExcelHead excelHead);
	
	/**
	 * 导入表格校验字段
	 * @author jinxindong 2016年6月2日 上午10:39:18
	 * @param userFile
	 * @param userFileFileName
	 * @param excelHead
	 * @return
	 */
	public AjaxData addAndValidateExcel(File userFile, String userFileFileName, ExcelHead excelHead);
	
	/**
	 * 导入添加用户数据
	 * @author jinxindong 2016年4月25日 上午11:35:26
	 * @param userInfos
	 * @return
	 */
	public AjaxData addAllUsers(List<UserInfo> userInfos);
	
	/**
	 * 根据用户id获取用户账户锁定信息
	 * @author zhoujiajun 2016年11月3日 上午11:14:31
	 * @param userId
	 * @return
	 */
	public Userlock getUserLockInfo(Integer userId);
	
	/**
	 * 根据用户名获取用户信息
	 * @author zhoujiajun 2016年11月26日 下午3:46:29
	 * @param username
	 * @return
	 */
	public Users getUserByName(String username);
}
