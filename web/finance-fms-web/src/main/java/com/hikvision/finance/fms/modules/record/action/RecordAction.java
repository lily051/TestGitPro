/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月6日 上午11:11:49
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.record.action;

import org.apache.struts2.ServletActionContext;

import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.model.Prosaverecord;
import com.hikvision.finance.fms.modules.record.dto.RecordDto;
import com.hikvision.finance.fms.modules.record.dto.RecordInfo;
import com.hikvision.finance.fms.modules.record.service.IRecordService;
import com.hikvision.finance.fms.modules.sale.dto.SaleInfo;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.www.action.BaseAction;

/**
 * <p>理财录像模块</p>
 * @author jinchenwu 2016年4月6日 上午11:11:49
 * @version V1.0
 */
public class RecordAction extends BaseAction<Prosaverecord> {
	
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = -4365715622689475453L;
	private IRecordService recordService;
	private RecordDto recordDto;
	private RecordInfo recordInfo;
	private Clientinput clientinput;
	private SaleInfo saleInfo;
	
	/**
	 * 获取理财录像列表
	 * @author jinchenwu 2016年4月6日 下午3:47:44
	 * @return
	 */
	public String getDataPage() {
		if(recordInfo==null || recordInfo.getRegionId()==null){
			throw new ExpectedException("", "未获取到区域信息");
		}
		page = recordService.getDataPage(recordInfo, start, limit);
		return PAGE;
	}
	
	/**
	 * 获取单条理财录像信息，以及回放控件需要的设备信息，用于录像回放
	 * @author jinchenwu 2016年4月13日 下午8:41:32
	 * @return
	 */
	public String getData(){
		recordDto =  recordService.saveLogAndGetData(recordInfo);
		operPage = "/modules/video/dialogs/videoPlayBack.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 随机查询一条理财录像数据
	 * @author jinchenwu 2016年4月7日 下午4:01:17
	 * @return
	 */
	public String getRandomData(){
		RecordInfo randomRecordInfo = recordService.getRandomData(recordInfo);
		if(randomRecordInfo!=null){
			ajaxData.put("randomRecordInfo",randomRecordInfo);
		}else{
			success = false;
			msg = "没有符合条件的录像";
		}
		return AJAX;
	}
	
	/**
	 * 跳到延迟到期操作界面
	 * @author jinxindong 2016年6月16日 下午5:02:59
	 * @return
	 */
	public String todelayVideoPage(){
		operPage = "/modules/video/dialogs/delayVideo.jsp";
		return DISPATCHER;
	}
	/**
	 * 获取业务单信息
	 * @author jinchenwu 2016年4月12日 下午12:25:45
	 * @return
	 */
	public String getBizOrderInfo(){
		saleInfo = recordService.getOrderInfo(saleInfo);
		operPage = "/modules/video/dialogs/viewBusiness.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 设置录像合规检查状态
	 * @author jinchenwu 2016年4月7日 下午4:05:59
	 * @return
	 */
	public String setRecordCheck(){
		if(recordInfo==null){
			throw new ExpectedException("", "操作失败");
		}
		recordService.saveRecordCheck(recordInfo);
		return AJAX;
	}
	
	/**
	 * 设置录像延期天数
	 * @author jinchenwu 2016年4月12日 下午12:27:33
	 * @return
	 */
	public String setDelayDays(){
		if(recordInfo==null){
			throw new ExpectedException("", "操作失败");
		}
		recordService.saveDelayDays(recordInfo);
		return AJAX;
	}
	
	/**
	 * 导出数据
	 * @author jinchenwu 2016年4月13日 上午9:42:45
	 * @return
	 */
	public String exportExcel(){
		String rootpath = ServletActionContext.getServletContext().getRealPath("/");
		String path = Constants.FILEUPLOAD + "excel/" + "Records.xls";
		if(recordService.saveLogAndexportExcel(recordInfo, rootpath + path)) {
			ajaxData.put("url", "/" + path);
    		success = true;
		} else {
			msg = "导出失败";
    		success = false;
		}
		return AJAX;
	}
	
	// ******************************************getter/setter*************************************************
	public IRecordService getRecordService() {
		return recordService;
	}
	
	public void setRecordService(IRecordService recordService) {
		this.recordService = recordService;
	}
	
	public RecordInfo getRecordInfo() {
		return recordInfo;
	}
	
	public void setRecordInfo(RecordInfo recordInfo) {
		this.recordInfo = recordInfo;
	}
	
	public RecordDto getRecordDto() {
		return recordDto;
	}
	
	public void setRecordDto(RecordDto recordDto) {
		this.recordDto = recordDto;
	}
	
	public Clientinput getClientinput() {
		return clientinput;
	}
	
	public void setClientinput(Clientinput clientinput) {
		this.clientinput = clientinput;
	}

    public SaleInfo getSaleInfo() {
    	return saleInfo;
    }

    public void setSaleInfo(SaleInfo saleInfo) {
    	this.saleInfo = saleInfo;
    }
}
