/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月30日 下午7:15:51
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sysconfig.dto;


/**
 * <p>模式参数包装类</p>
 * @author jinxindong 2016年3月30日 下午7:15:51
 * @version V1.0 
 */
public class ModeParamInfo {
	private  String  verificTionCode; //启用禁用验证码
	private  String  uploadVideoAudit; //启用禁用录像审核后上传
	private  String  productReview; //启用禁用产品复核
	private  String  overlayOsd; //启用禁用osd叠加
	private  String  cardReaderVideo; //启用禁用身份证读卡器刷卡开启录像
	private  String  choiceUsbIpc; //业务单录入方式
	private  String  nipLimit; //IP限制,对应channelIpBind
	private  String  operName; //操作用户名
	private  String  nonCenterStorage;//无中心存储/有中心存储
	private  String  logReport;//c/s客户端日志上报
	private  String  logReportMethod;//c/s客户端日志上报方式
	
    public String getVerificTionCode() {
    	return verificTionCode;
    }
	
    public void setVerificTionCode(String verificTionCode) {
    	this.verificTionCode = verificTionCode;
    }
	
    public String getUploadVideoAudit() {
    	return uploadVideoAudit;
    }
	
    public void setUploadVideoAudit(String uploadVideoAudit) {
    	this.uploadVideoAudit = uploadVideoAudit;
    }
	
    public String getProductReview() {
    	return productReview;
    }
	
    public void setProductReview(String productReview) {
    	this.productReview = productReview;
    }
	
	

	
    
    
    public String getOverlayOsd() {
    	return overlayOsd;
    }

	
    public void setOverlayOsd(String overlayOsd) {
    	this.overlayOsd = overlayOsd;
    }

	public String getCardReaderVideo() {
    	return cardReaderVideo;
    }

	
    public void setCardReaderVideo(String cardReaderVideo) {
    	this.cardReaderVideo = cardReaderVideo;
    }


	
    
    public String getChoiceUsbIpc() {
    	return choiceUsbIpc;
    }

	
    public void setChoiceUsbIpc(String choiceUsbIpc) {
    	this.choiceUsbIpc = choiceUsbIpc;
    }

	public String getNipLimit() {
    	return nipLimit;
    }
	
    public void setNipLimit(String nipLimit) {
    	this.nipLimit = nipLimit;
    }

	
    public String getOperName() {
    	return operName;
    }

	
    public void setOperName(String operName) {
    	this.operName = operName;
    }

	public String getNonCenterStorage() {
		return nonCenterStorage;
	}

	public void setNonCenterStorage(String nonCenterStorage) {
		this.nonCenterStorage = nonCenterStorage;
	}

	public String getLogReport() {
		return logReport;
	}

	public void setLogReport(String logReport) {
		this.logReport = logReport;
	}
	
	public String getLogReportMethod() {
		return logReportMethod;
	}
	
	public void setLogReportMethod(String logReportMethod) {
		this.logReportMethod = logReportMethod;
	}
}
