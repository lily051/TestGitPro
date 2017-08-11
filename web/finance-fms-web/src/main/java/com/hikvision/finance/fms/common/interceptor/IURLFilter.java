/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月22日 下午2:17:12
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.interceptor;

import com.hikvision.finance.fms.common.model.UserSession;

/**
 * 对用户访问的url进行过滤类的接口,提供是否需要过滤，是否有权限访问等功能校验。
 * @author jinxindong 2016年2月22日 下午2:17:12
 * @version V1.0
 */
public interface IURLFilter extends com.hikvision.finance.fwork.www.filter.url.IURLFilter {
	
	/**
	 * 判断该用户是否能够访问该url
	 * @author fuqunqing 2015年8月24日 上午10:03:37
	 * @param url 校验url
	 * @param userId 用户id
	 * @param session session信息
	 * @return
	 */
	public boolean canAccess(String url, UserSession session);
}
