/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-17 下午5:05:41
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.datacollect.service;

import java.util.List;

import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.model.Devicechannel;
import com.hikvision.finance.fms.model.Records;
import com.hikvision.finance.fms.webdevice.datacollect.dto.BasicRecordInfo;
import com.hikvision.finance.fms.webdevice.datacollect.dto.DataCollectInfo;
import com.hikvision.finance.fms.webdevice.datacollect.dto.RecordsInfo;
import com.hikvision.finance.fms.webdevice.datacollect.qo.RecordsQo;
import com.hikvision.finance.fwork.util.AjaxData;

/**
 * <p></p>
 * @author xujiangfei 2016-4-17 下午5:05:41
 * @version V1.0 
 */
public interface IDatacollectService extends IBaseService<Records,RecordsQo>{
	
	/**
	 * 获取通道List
	 * @author jinchenwu 2016年5月16日 上午11:11:36
	 * @return
	 */
	public List<Devicechannel> getChannels(String ip);
	
	/**
	 * 设置用户DVR信息和IPC通道信息
	 * @author jinchenwu 2016年4月21日 下午3:45:28
	 * @param dataCollectInfo
	 * @return
	 */
	public AjaxData saveChannel(DataCollectInfo dataCollectInfo);
	
	/**
	 * 保存正常录像信息
	 * @author jinchenwu 2016年4月19日 上午9:53:10
	 * @param records
	 * @param ip 
	 */
	public void saveRecords(Records records, String ip);

	/**
	 * 存储补录录像信息
	 * @author xujiangfei 2016-5-4 下午4:07:29
	 * @param records
	 */
	public void saveSupplementVideo(Records records);
	
	/**
	 * 上传录像(重新上传)
	 * @author jinchenwu 2016年5月10日 下午6:36:29
	 * @param recordids
	 * @return
	 */
	public AjaxData uploadRecord(String recordids);
	
	/**
	 * 删除录像记录
	 * @author xujiangfei 2016-4-21 下午4:54:04
	 * @param ids
	 */
	public AjaxData deleteRecords(List<String> idList);
	
	/**
	 * 修改录像名称
	 * @author jinchenwu 2016年5月10日 下午6:41:59
	 * @param records
	 */
	public void editRecordsName(Records records);
	
	/**
	 * 添加本地录像文件信息（USB录像文件）
	 * @author jinchenwu 2016年5月14日 下午3:07:07
	 * @param records
	 * @return
	 */
	public AjaxData saveLocalRecord(Records records);
	
	/**
	 * 获取视频文件列表（records表）,直接读数据库, 或者从缓存中读
	 * @author jinchenwu 2016年5月12日 下午4:06:05
	 * @param type
	 * @return
	 */
	public List<BasicRecordInfo> getVideoList(int type);
	
	/**
	 * 打开回放页面
	 * @author jinchenwu 2016年5月19日 上午11:32:43
	 * @param id
	 * @param type
	 * @return
	 */
	public RecordsInfo getrecordsPlaybackbyid(Integer id, int type);
	
	/**
	 * 异常录像添加结束时间
	 * @author jinchenwu 2016年5月17日 下午3:44:41
	 * @param records
	 * @return
	 */
	public void saveEndTime(Records records);
	
	/**
	 * 删除每日录像，由定时器驱动执行
	 * @author jinchenwu 2016年4月28日 上午9:41:22
	 */
	public void deleteDailyRecord();
	
	/**
	 * 删除录像信息(用户删除掉, 录像上传失败)，由定时器驱动执行
	 * @author zhoujiajun 2016年11月26日 下午1:35:17
	 */
	public void deleteFailRecord();
}
