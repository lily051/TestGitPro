/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月24日 下午5:37:44
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.system.service;

import java.util.Date;
import java.util.List;

import com.hikvision.finance.fms.common.model.MenuBean;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fwork.util.AjaxData;


/**
 * <p>登录模块service</p>
 * @author jinxindong 2016年2月24日 下午5:37:44
 * @version V1.0 
 */
public interface ILoginService {
	
	/**
	 * 授权检查
	 * @author jinchenwu 2016年10月11日 下午4:49:23
	 * @return
	 */
	String getAuthorizeInfo();
	
	/**
	 * 验证用户登录
	 * @author jinxindong 2016年4月26日 下午2:49:41
	 * @param entity
	 * @param user
	 * @return
	 */
	AjaxData validateLoginUser(Users entity,Users user);
	
	/**
	 * 根据请求的url获取菜单的编码
	 * @author jinxindong 2016年2月25日 下午12:31:48
	 * @param menus
	 * @param requestUrl
	 * @return
	 */
	String getRequestMenuCode(List<MenuBean> menus, String requestUrl);
	
	/**
	 * 获取数据库时间
	 * @author jinxindong 2016年3月25日 下午6:06:47
	 * @return
	 */
	Date getSysDate();

	/**
	 * 采集端登录--用户的验证
	 * @author xujiangfei 2016-4-13 下午1:54:10
	 * @param entity
	 * @return
	 */
	AjaxData enableLogin(Users entity,Users user);

	/**
	 * 登录页获取授权信息
     * @author jinchenwu 2016年11月1日 下午3:57:42
     * @return
     */
    AjaxData getLicense();
	
}
