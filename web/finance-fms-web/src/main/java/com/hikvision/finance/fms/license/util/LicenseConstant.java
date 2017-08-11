/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2015年8月26日 下午6:55:36
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.license.util;


/**
 * 授权常量
 * @author jinxindong 2016年4月19日 下午3:09:36
 * @version V1.0
 */
public class LicenseConstant {
	
	/**授权更新期限 单位秒*/
	public final static long SOFT_TIME = 50;
	
	/**
	 * 看门狗参数
	 * @author jinxindong 2016年4月19日 下午3:09:46
	 * @version V1.0
	 */
	public final static class DOGTYPE {
		
		/**
		 * 通道上限
		 */
		public final static int DOG_CHANNEL = 12;
		
		/**
		 * 授权类型
		 */
		public final static int DOG_VERSION = 100;
	}
	
}
