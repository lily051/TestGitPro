/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-3-22 下午7:31:01
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.center;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import com.hikvision.finance.fms.cms.hpp.center.OvertimeRecordProto.ModifyOvertimeRecordService;
import com.hikvision.finance.fms.cms.hpp.center.OvertimeRecordProto.ReqOvertimeRecord;
import com.hikvision.finance.fms.cms.hpp.center.OvertimeRecordProto.RspOvertimeRecord;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;
import com.hikvision.finance.fms.common.exception.CMSException;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao;

/**
 * <p></p>
 * @author xujiangfei 2016-3-22 下午7:31:01
 * @version V1.0 
 */
public class HModifyOvertimeRecordServiceImpl extends ModifyOvertimeRecordService{

	private static Logger logger = LoggerFactory.getLogger(HModifyOvertimeRecordServiceImpl.class);
	private IProSaveRecordDao proSaveRecordDao;
	/* (non-Javadoc)--//CMD_DT_OVERTIMERECORD_LIST--获取过期录像信息
	 * @see com.hikvision.finance.fms.cms.hpp.center.OvertimeRecordProto.ModifyOvertimeRecordService#qyeryAlarmlogID(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.center.OvertimeRecordProto.ReqOvertimeRecord, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void qyeryAlarmlogID(RpcController controller,ReqOvertimeRecord request, RpcCallback<RspOvertimeRecord> done) {
		logger.info("开始【获取过期录像信息】处理:cmd: 【" + CMD.CMD_DT_OVERTIMERECORD_LIST_VALUE + "】; request:【" + request.toString() + "】");
		
		//1.设置返回参数
		RspOvertimeRecord.Builder rspOvertimeRecord = RspOvertimeRecord.newBuilder();
		rspOvertimeRecord.setCmd(CMD.CMD_DT_OVERTIMERECORD_LIST_VALUE);
		try{
			
			List<Clientinput> listovertime = proSaveRecordDao.getOvertimeRecordInfo();
			if(listovertime==null){
				throw new CMSException(CMSException.RECORDS_NO_EXITS, "没有对应录像设备！");
			}
			for(int i=0;i<listovertime.size();i++){
				com.hikvision.finance.fms.cms.hpp.center.OvertimeRecordProto.OvertimeRecord.Builder OvertimeClientinput = com.hikvision.finance.fms.cms.hpp.center.OvertimeRecordProto.OvertimeRecord.newBuilder();
				if(listovertime.get(i).getId()!=null){
					OvertimeClientinput.setId(listovertime.get(i).getId());
				}
				if(listovertime.get(i).getNproId()!=null){
					OvertimeClientinput.setProId(listovertime.get(i).getNproId());
				}
				if(listovertime.get(i).getBusinessTime()!=null){
					OvertimeClientinput.setBusinessTime(changeDateToStr(listovertime.get(i).getBusinessTime(),"yyyy-MM-dd HH:mm:ss"));
				}
				rspOvertimeRecord.addOvertimeRecord(i, OvertimeClientinput);
			}
			//2.处理结果
			done.run(rspOvertimeRecord.build());
			
		}catch(CMSException e){
			done.run(rspOvertimeRecord.build());
			logger.error("结束 获取过期录像信息 请求处理: 异常: 【" + e.getMessage() + "】",e);
			return;
		}
		logger.info("结束 获取过期录像信息  请求处理:cmd: 【" + CMD.CMD_DT_STORAGEDEVICE_LIST.getNumber() + "】; resultInfo:【"
		        + rspOvertimeRecord.build().toString() + "】");
	}
	
	public String changeDateToStr(Date date, String strDateformat) {
		String strdate = null;
		strdate = new SimpleDateFormat(strDateformat).format(date);
		return strdate;
	}
	
	// setter,getter
	
	public IProSaveRecordDao getProSaveRecordDao() {
		return proSaveRecordDao;
	}
	
	public void setProSaveRecordDao(IProSaveRecordDao proSaveRecordDao) {
		this.proSaveRecordDao = proSaveRecordDao;
	}
	
}
