/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-17 下午5:01:40
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.datacollect.dao;

import java.util.List;

import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Records;
import com.hikvision.finance.fms.webdevice.datacollect.dto.BasicRecordInfo;
import com.hikvision.finance.fms.webdevice.datacollect.qo.RecordsQo;

/**
 * <p>要整理</p>
 * @author xujiangfei 2016-4-17 下午5:01:40
 * @version V1.0 
 */
public interface IRecordsDao extends IBaseDao<Records, RecordsQo>{
	
	/**
	 * 设置异常结束录像状态
	 * @author jinchenwu 2016年5月14日 上午10:49:56
	 * @param userId
	 */
	public void saveErrorRecordStatus(Integer userId);
	
	/**
	 * 视频文件列表
	 * @author jinchenwu 2016年5月12日 下午7:11:40
	 * @param userId
	 * @return
	 */
	public List<BasicRecordInfo> getVideoList(Integer userId);
	
	/**
	 * 删除每日录像，由定时器驱动执行
	 * @author jinchenwu 2016年4月28日 上午9:41:22
	 */
	public void deleteDailyRecord();
	
	/**
	 * 获取录像信息
	 * @author zhoujiajun 2016年11月26日 下午1:35:17
	 */
	public List<BasicRecordInfo> getFailRecord();

}
