/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-15 上午10:26:12
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.datacollect.action;

import java.util.List;

import com.hikvision.finance.fms.common.cache.HeartbeatCache;
import com.hikvision.finance.fms.common.model.UserSession;
import com.hikvision.finance.fms.common.util.SessionUtil;
import com.hikvision.finance.fms.model.Devicechannel;
import com.hikvision.finance.fms.model.Prosaverecord;
import com.hikvision.finance.fms.model.Records;
import com.hikvision.finance.fms.webdevice.datacollect.dto.BasicRecordInfo;
import com.hikvision.finance.fms.webdevice.datacollect.dto.DataCollectInfo;
import com.hikvision.finance.fms.webdevice.datacollect.dto.RecordsInfo;
import com.hikvision.finance.fms.webdevice.datacollect.dto.Usbrinfo;
import com.hikvision.finance.fms.webdevice.datacollect.service.IDatacollectService;
import com.hikvision.finance.fwork.www.action.BaseAction;

/**
 * <p>录像数据Action</p>
 * @author xujiangfei 2016-4-15 上午10:26:12
 * @version V1.0 
 */
public class DatacollectAction extends BaseAction<Records>{

	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = 1303231606470024652L;
	private IDatacollectService datacollectService;
    private DataCollectInfo dataCollectInfo;
	private Records records;
	private List<BasicRecordInfo> basicRecordInfoList;// 视频文件列表
	private RecordsInfo recordsInfo;
	private Prosaverecord prosaverecord;
	private Usbrinfo usbrinfo;
	
	private String ids;//用于接收多个数据的id
	private int type;// (1-直接读数据库，2-读缓存)(1-视频文件列表回放，2-业务关联中回放)

	
	
	/**
	 * 返回用户关联的IPC通道List
	 * @author jinchenwu 2016年4月19日 下午3:40:11
	 * @return
	 */
	public String getChannels(){
		String ip = getRequest().getRemoteAddr();//客户端ip
		List<Devicechannel> deviceChannelList = datacollectService.getChannels(ip);
		ajaxData.put("root", deviceChannelList);
		return AJAX;
	}
	
	/**
	 * 设置用户DVR信息和IPC通道信息
	 * @author jinchenwu 2016年4月19日 下午4:40:10
	 * @return
	 */
	public String setChannel(){
		ajaxData = datacollectService.saveChannel(dataCollectInfo);	
		return AJAX;
	}
	
	/**
	 * 保存useb信息到sessino中
	 * @author xujiangfei 2016-5-4 下午4:54:33
	 * @return
	 */
	public String saveUsbInfo(){
		UserSession userSession = SessionUtil.getUserSession();
		userSession.setUsbCamIndex(usbrinfo.getUsbCamIndex());
		userSession.setUsbCamName(usbrinfo.getUsbCamName());
		userSession.setUsbMicIndex(usbrinfo.getUsbMicIndex());
		userSession.setUsbMicName(usbrinfo.getUsbMicName());
		return AJAX;
	}
	
	/**
	 * 开始录像和结束录像时存储录像信息（正常录像）
	 * @author jinchenwu 2016年4月28日 上午10:11:25
	 * @return
	 */
	public String saveRecords(){
		String ip = getRequest().getRemoteAddr();//当前浏览器的ip
		datacollectService.saveRecords(records,ip);
		return AJAX;
	}
	
	/**
	 * 存储补录录像信息
	 * @author xujiangfei 2016-5-3 上午10:12:33
	 * @return
	 */
	public String supplementVideo(){
		datacollectService.saveSupplementVideo(records);
		return AJAX;
	}
	
	/**
	 * 数据采集页面上传录像(重新上传)
	 * @author jinchenwu 2016年5月10日 下午6:36:03
	 * @return
	 */
	public String uploadRecord(){
		datacollectService.uploadRecord(ids);
		return AJAX;
	}
	
	/**
	 * 删除录像记录
	 * @author xujiangfei
	 * @return
	 */
	@SuppressWarnings("unchecked")
    public String deleteRecords(){
		ajaxData = datacollectService.deleteRecords((List<String>)list);
		return AJAX;
	}
	
	/**
	 * 修改录像名称
	 * @author xujiangfei 2016-4-21 下午7:07:17
	 * @return
	 */
	public String editRecordsName() {
		datacollectService.editRecordsName(records);
		ajaxData.setSuccess(success);
		return AJAX;
	}
	
	/**
	 * 添加本地录像文件信息（USB录像文件）
	 * @author jinchenwu 2016年5月14日 下午2:35:25
	 * @return
	 */
	public String saveLocalRecord() {
		ajaxData = datacollectService.saveLocalRecord(records);
		return AJAX;
	}
	
	/**
	 * 获取视频文件列表（records表）,直接读数据库, 或者从缓存中读
	 * @author jinchenwu 2016年5月12日 下午5:04:38
	 * @return
	 */
	public String getVideoList(){
		basicRecordInfoList = datacollectService.getVideoList(type);
		ajaxData.put("videoList", basicRecordInfoList);
		return AJAX;
	}
	
	/**
	 * 视频文件列表和业务关联中打开回放页面
	 * @author xujiangfei
	 * @return
	 */
	public String toOpenPageRecordPlayBack(){
		recordsInfo = datacollectService.getrecordsPlaybackbyid(id.intValue(), type);
		operPage = "/web/webDevice/datacollect/dialogs/recordPlayBack.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 异常录像设置
	 * @author jinchenwu 2016年5月17日 下午3:41:46
	 * @param records
	 * @return
	 */
	public String saveEndTime(){
		datacollectService.saveEndTime(records);
		return AJAX;
	}
	
	/**
	 * 设置用户心跳
	 * @author jinchenwu 2016年5月12日 下午7:54:47
	 */
	public void setHeartbeat() {
		UserSession usersession = SessionUtil.getUserSession();
		HeartbeatCache.setHeartBeatTime(usersession.getUserId(), usersession.getDeviceChannelId());
	}
	
	/**
	 * 链接到ocx页面
	 * @author jinchenwu 2016年5月12日 下午7:54:47
	 */
	public String toOcx(){
		operPage = "/web/ocx.jsp";
		return DISPATCHER;
	}
 
	// *************************setter--getter***********************************//
	public IDatacollectService getDatacollectService() {
		return datacollectService;
	}
	
	public void setDatacollectService(IDatacollectService datacollectService) {
		this.datacollectService = datacollectService;
	}
	
	public Records getRecords() {
		return records;
	}
	
	public void setRecords(Records records) {
		this.records = records;
	}
	
	public DataCollectInfo getDataCollectInfo() {
		return dataCollectInfo;
	}
	
	public void setDataCollectInfo(DataCollectInfo dataCollectInfo) {
		this.dataCollectInfo = dataCollectInfo;
	}
	
	public RecordsInfo getRecordsInfo() {
		return recordsInfo;
	}
	
	public void setRecordsInfo(RecordsInfo recordsInfo) {
		this.recordsInfo = recordsInfo;
	}
	
	public Prosaverecord getProsaverecord() {
		return prosaverecord;
	}
	
	public void setProsaverecord(Prosaverecord prosaverecord) {
		this.prosaverecord = prosaverecord;
	}
	
	public Usbrinfo getUsbrinfo() {
		return usbrinfo;
	}
	
	public void setUsbrinfo(Usbrinfo usbrinfo) {
		this.usbrinfo = usbrinfo;
	}
	
	public String getIds() {
		return ids;
	}
	
	public void setIds(String ids) {
		this.ids = ids;
	}
	
	public List<BasicRecordInfo> getBasicRecordInfoList() {
		return basicRecordInfoList;
	}
	
	public void setBasicRecordInfoList(List<BasicRecordInfo> basicRecordInfoList) {
		this.basicRecordInfoList = basicRecordInfoList;
	}
	
	public int getType() {
		return type;
	}
	
	public void setType(int type) {
		this.type = type;
	}

	
	
}
