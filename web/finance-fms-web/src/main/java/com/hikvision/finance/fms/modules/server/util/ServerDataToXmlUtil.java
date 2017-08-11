/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月14日 下午2:02:10
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.server.util;

import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.xml.DomNode;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.modules.server.dto.ServerInfo;

/**
 * <p> 服务所需XML格式数据转化工具 </p>
 * @author jinxindong 2016年3月14日 下午2:02:10
 * @version V1.0
 */
public class ServerDataToXmlUtil {
	
	/********************************************* 创建XML ************************************************************/
	
	/**
	 * 创建上传服务的XML
	 * @author jinxindong 2016年3月14日 下午2:50:00
	 * @param serverInfo
	 * @return
	 */
	public static ServerInfo buildUploadXml(ServerInfo serverInfo, String strChoiceUsbIpc, String isNonCenterStorage) {
		
		DomNode domNode = new DomNode("ROOT");
		DomNode paramUpload = domNode.createChild("PARAM_UPLOAD");
		paramUpload.addAttribute("Frequency", serverInfo.getTimeOut());
		paramUpload.addAttribute("TaskCount", null==serverInfo.getTaskCount()?1:serverInfo.getTaskCount());
		
		if ("ipc".equals(strChoiceUsbIpc)) {
			paramUpload.addAttribute("Mode", "1");//serverInfo.getUploadType()获取得到"ipc"，但是c++服务需要"ipc"对应为"1"
			if ("true".equals(isNonCenterStorage)) {//ipc模式下，才会有“有/无中心存储”的概念
				paramUpload.addAttribute("HaveCenterStorage", 0);// 0 无中心存储模式
			} else {
				paramUpload.addAttribute("HaveCenterStorage", 1);// 1 有中心存储模式
			}
		} else {//usb
			paramUpload.addAttribute("Mode", "2");
			paramUpload.addAttribute("HaveCenterStorage", 1);// usb模式默认有中心存储
			paramUpload.addAttribute("Speed", serverInfo.getSpeed());
		}
		
		DomNode paramAtserver = domNode.createChild("PARAM_ATSERVER");
		paramAtserver.addAttribute("ip", serverInfo.getStrSysIp());
		paramAtserver.addAttribute("port", serverInfo.getNctrlSysPort());
		DomNode paramCollect = domNode.createChild("PARAM_COLLECT");
		//< PARAM_COLLECT  startTime=”hh:mm:ss” stopTime=”hh:mm:ss”/>
		paramCollect.addAttribute("startTime", serverInfo.getWorkTimeBegin());
		paramCollect.addAttribute("stopTime", serverInfo.getWorkTimeEnd());
		serverInfo.setStrConfig(domNode.getContent());
		
		return serverInfo;
	}
	
	/**
	 * 创建录像守卫服务的XML
	 * @author jinxindong 2016年3月14日 下午4:14:54
	 * @param serverInfo
	 * @return
	 */
	public static ServerInfo buildRecordXml(ServerInfo serverInfo, String strChoiceUsbIpc, String isNonCenterStorage) {
		
		DomNode domNode = new DomNode("ROOT");
		DomNode paramRecguard = domNode.createChild("PARAM_RECGUARD");
		paramRecguard.addAttribute("Frequency", serverInfo.getTimeOut());
		
		if ("ipc".equals(strChoiceUsbIpc)) {
			if ("true".equals(isNonCenterStorage)) {
				paramRecguard.addAttribute("HaveCenterStorage", 0);// 0 无中心存储模式
			} else {
				paramRecguard.addAttribute("HaveCenterStorage", 1);// 1 有中心存储模式
			}
		} else {
			paramRecguard.addAttribute("HaveCenterStorage", 1);// usb模式 有中心存储模式
		}
		DomNode paramAtserver = domNode.createChild("PARAM_ATSERVER");
		paramAtserver.addAttribute("ip", serverInfo.getStrSysIp());
		paramAtserver.addAttribute("port", serverInfo.getNctrlSysPort());
		serverInfo.setStrConfig(domNode.getContent());
		return serverInfo;
	}
	
	/**
	 * 创建系统巡检服务的XML
	 * @author jinxindong 2016年3月14日 下午4:27:35
	 * @param serverInfo
	 * @return
	 */
	public static ServerInfo buildInspectionXml(ServerInfo serverInfo, String strChoiceUsbIpc, String isNonCenterStorage) {
		
		DomNode domNode = new DomNode("ROOT");
		DomNode paramTiming = domNode.createChild("PARAM_TIMING");
		paramTiming.addAttribute("Model", serverInfo.getModel());//校时模式
		if ("ipc".equals(strChoiceUsbIpc)) {
			if ("true".equals(isNonCenterStorage)) {
				paramTiming.addAttribute("HaveCenterStorage", 0);// 0 无中心存储模式
			} else {
				paramTiming.addAttribute("HaveCenterStorage", 1);// 1 有中心存储模式
			}
		} else {
			paramTiming.addAttribute("HaveCenterStorage", 1);// 1 有中心存储模式
		}
		paramTiming.addAttribute("Condition", serverInfo.getCondition());
		if(serverInfo.getModel() == Constants.CONS_0){
			paramTiming.addAttribute("Time", serverInfo.getTime());
			paramTiming.addAttribute("Frequency", "");
		}else{
			paramTiming.addAttribute("Time", "");
			paramTiming.addAttribute("Frequency", serverInfo.getFixedTime());
		}
		serverInfo.setStrConfig(domNode.getContent());
		return serverInfo;
	}
	
	/********************************************* 解析XML ************************************************************/
	
	
	/**
	 * 解析上传服务的XML
	 * @author jinxindong 2016年3月16日 下午1:41:20
	 * @param serverInfo
	 * @return
	 */
	public static ServerInfo resolveUploadXml(ServerInfo serverInfo) {
		
		DomNode domNode = new DomNode();
		domNode.load(serverInfo.getStrConfig());
		DomNode paramUpload = domNode.getChild("PARAM_UPLOAD");
		String timeOut = paramUpload.getAttribute("Frequency");
		String taskCount = paramUpload.getAttribute("TaskCount");
		String uploadType = paramUpload.getAttribute("Mode");
		String speed = paramUpload.getAttribute("Speed");
		DomNode paramAtserver = domNode.getChild("PARAM_ATSERVER");
		String strSysIp = paramAtserver.getAttribute("ip");
		String nctrlSysPort = paramAtserver.getAttribute("port");		
		DomNode paramCollect = domNode.getChild("PARAM_COLLECT");
		String workTimeBegin = paramCollect.getAttribute("startTime");
		String workTimeEnd = paramCollect.getAttribute("stopTime");

		
		serverInfo.setWorkTimeBegin(workTimeBegin);
		serverInfo.setTaskCount(taskCount);
		serverInfo.setUploadType(uploadType);
		serverInfo.setSpeed(speed);
		serverInfo.setWorkTimeEnd(workTimeEnd);
		serverInfo.setTimeOut(Integer.valueOf(timeOut));
		serverInfo.setStrSysIp(strSysIp);
		serverInfo.setNctrlSysPort(Integer.valueOf(nctrlSysPort));
		return serverInfo;
	}
	
	/**
	 * 解析录像守卫服务的XML
	 * @author jinxindong 2016年3月16日 下午2:07:12
	 * @param serverInfo
	 * @return
	 */
	public static ServerInfo resolveRecordXml(ServerInfo serverInfo) {
		
		DomNode domNode = new DomNode();
		domNode.load(serverInfo.getStrConfig());
		DomNode paramRecguard = domNode.getChild("PARAM_RECGUARD");
		String timeOut = paramRecguard.getAttribute("Frequency");
		DomNode paramAtserver = domNode.getChild("PARAM_ATSERVER");
		String strSysIp = paramAtserver.getAttribute("ip");
		String nctrlSysPort = paramAtserver.getAttribute("port");
		
		serverInfo.setTimeOut(Integer.valueOf(timeOut));
		serverInfo.setStrSysIp(strSysIp);
		serverInfo.setNctrlSysPort(Integer.valueOf(nctrlSysPort));
		return serverInfo;
	}
	
	/**
	 * 解析系统巡检服务的XML
	 * @author jinxindong 2016年3月16日 下午2:07:21
	 * @param serverInfo
	 * @return
	 */
	public static ServerInfo resolveInspectionXml(ServerInfo serverInfo) {
		
		DomNode domNode = new DomNode();
		domNode.load(serverInfo.getStrConfig());
		DomNode paramTiming = domNode.getChild("PARAM_TIMING");
		String model = paramTiming.getAttribute("Model");
		String condition = paramTiming.getAttribute("Condition");
		String time = paramTiming.getAttribute("Time");
		String fixedTime = paramTiming.getAttribute("Frequency");
		
		serverInfo.setModel(Integer.valueOf(model));
		serverInfo.setCondition(Integer.valueOf(condition));
		if (!StringUtils.isEmpty(time) ) {
			//Date timeDate = DateUtils.getDateTime("yyyy-MM-dd", time);
			serverInfo.setTime(time);
		}
		if (!StringUtils.isEmpty(fixedTime)) {
			serverInfo.setFixedTime(Integer.valueOf(fixedTime));
		}
		return serverInfo;
	}
	
}
