/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月19日 下午7:46:52
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.model;

import java.util.Calendar;
import java.util.List;


/**
 * <p>缓存数据</p>
 * @author jinchenwu 2016年4月19日 下午7:46:52
 * @version V1.0 
 */
public class CacheData {
	
	// 最后更新时间
	private Calendar lastUpdateTime;
	private List<?> dataList;
	
	public List<?> getDataList() {
		return dataList;
	}
	
	public void setDataList(List<?> dataList) {
		this.dataList = dataList;
	}
	
	public Calendar getLastUpdateTime() {
		return lastUpdateTime;
	}
	
	public void setLastUpdateTime(Calendar lastUpdateTime) {
		this.lastUpdateTime = lastUpdateTime;
	}
	
}
