<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<%@page import="com.hikvision.finance.fms.common.util.SessionUtil,com.hikvision.finance.fms.common.model.UserSession"%>
<%
	UserSession userSession = SessionUtil.getUserSession();
%>
<html lang="zh">
<head>
    <meta charset="utf-8">
    <%@ include file="/common/commons.jsp" %>
	<%@ include file="/common/commons-extend.jsp" %>
	<link  href="/web/webDevice/datacollect/css/datacollect.css" rel="stylesheet" />
	<script>
		var devInfo = {strCollectIp:"<%=userSession.getDeviceIp()%>",dvrPort:"<%=userSession.getDevicePort()%>",
		dvrUserName:"<%=userSession.getDeviceUserName()%>",dvrPwd:"<%=userSession.getDevicePassword()%>",nchannelNo:"<%=userSession.getDeviceChannelNo()%>",streamType:"<%=userSession.getStreamType()%>",usbCamIndex:"<%=userSession.getUsbCamIndex()%>",
		usbCamName:"<%=userSession.getUsbCamName()%>",usbMicIndex:"<%=userSession.getUsbMicIndex()%>",usbMicName:"<%=userSession.getUsbMicName()%>"};
	</script>
	<script src="/js/ocxVersion/initOCX.js"></script>
	<script src="/web/webDevice/datacollect/js/datacollect.js"></script>
	<script src="/web/webDevice/datacollect/js/dataBusiness.js"></script>
	<script language="javascript" type="text/javascript" for=ocxRecord event="RecordFile(lpstrXML)">
		var xmlDoc = FMS.globalFun.loadXML(lpstrXML),
			recordStatus = xmlDoc.getElementsByTagName("RecordStatus")[0].firstChild.nodeValue,
			startTime = xmlDoc.getElementsByTagName("StartTime")[0].firstChild.nodeValue,
			stopTime = xmlDoc.getElementsByTagName("StopTime")[0].firstChild.nodeValue,
			picPath = xmlDoc.getElementsByTagName("PicPath")[0].firstChild.nodeValue.replace(/\\/g,'\\\\');
		FMS.datacollect.recordStatus = recordStatus;
		var fileName = "",fileSize="";
		if(modeParam.usbAndIpc=="usb"){
			fileName = xmlDoc.getElementsByTagName("FileName")[0].firstChild.nodeValue;
			fileSize = xmlDoc.getElementsByTagName("FileSize")[0].firstChild.nodeValue;
		}
		if(recordStatus==1){
			var sendXml = "<?xml version=\"1.0\"encoding=\"UTF-8\"?><BussInfo><OSDNum>8</OSDNum></BussInfo>";
			try{
				document.getElementById("ocxRecord").SetShowString(sendXml);
			}catch(e){

			}
		}
		
		var newStartTime = new Date();
		newStartTime.setTime(startTime * 1000);
		newStartTime = newStartTime.format("yyyy-MM-dd hh:mm:ss");
		
		var newStopTime = new Date();
		newStopTime.setTime(stopTime * 1000);
		newStopTime = newStopTime.format("yyyy-MM-dd hh:mm:ss");
		
		$.ajax({
			url:'/webdevice/datacollect/saveRecords.action',
			type:'post',
			async:false,
			dataType:'json',
			data:{
				'records.nrecordStatus':recordStatus,
				'records.startTime':newStartTime,
				'records.endTime':newStopTime,
				'records.strUrl':fileName,
				"records.screenshotSrc":picPath,
				"records.nFileSize":fileSize
			},
			success:function(data){
				if(data&&data.success){
					FMS.datacollect.getVideoList(1);
				}
			},
			error:function(xhr){
				setTimeout(function(){
					document.getElementById("ocxRecord").StopRecord(0);
				},1500);
			}
		});
	   // alert(lpstrXML);
	</script>
	
</head>
<body>
	<div id="wrapper">
		<%@ include file="/common/commons-collect-top.jsp"%>
		<div id="navbar" class="layout">
			<div class="container">
				<ul class="nav">
					<li class="active"><a>视频录像</a></li>
					<li id="usbRemoveLi"><a>视频补录</a></li>
				</ul>
			</div>
		</div>
		<div id="content" class="layout">
			<div class="container wrapper grid-m0s9 FMS_container">
				<div class="col-main collect-main">
					<div class="main-wrap wrapFixed" id="videoRecord">
						<div class="fullfit">
							<div class="header">
								<span class="tip" id="businessTip">话术模板</span>
								<span class="tip" id="osdChange" data-osd='8'>4行水印</span>
							</div>
							<div class="ocxRecord" id="previewOcx">
								<!-- <OBJECT ID="ocxRecord" CLASSID="CLSID:04655FF8-9979-4FFA-8F77-C3A539EA7370" width="100%" height="100%">
									<param name="lLayoutMode" value="0" />
									<param name="lSkinType" value="0" />
								</OBJECT> -->
							</div>
						</div>
						<div class="fullfit hidden" id="usbRemoveDiv">
							<div class="header" style="height:80px;">
								<form >
									<div class="header-group" id="headRecord">
											<input type="text" class="auto-input" hikui="calendar" readonly="readonly" id="startTime"
											datefmt="yyyy-MM-dd HH:mm:ss" name="startTime" maxdate="#F{$dp.$N('endTime');}">
											<label>-</label>
											<input type="text" class="auto-input" hikui="calendar" readonly="readonly" id="endTime"
											datefmt="yyyy-MM-dd HH:mm:ss" name="endTime" mindate="#F{$dp.$N('startTime');}" maxdate="%y-%M-%d %H:%m:%s">
											<span class="btn btn-submit blockBtn" id="recordSearch">查询</span>
									</div>
								</form>
								<div class="header-group">
									<label for="" class="title">远程回放</label>
								</div>
							</div>
							<div class="ocxRecord" style="top:80px;" id="playBackOcx">
								<!-- <OBJECT ID="recordBack" CLASSID="CLSID:07EBE387-935C-45DA-A121-AE19F18CEF44" width="100%" height="100%">
									<param name="lLayout" value="1" />
									<param name="lSkinType" value="0" />
									<param name="lpstrPath" value="c:\lala3\" />
									<param name="lPicType" value="0" />
									<param name="lModuleStyle" value="255" />
									<param name="lBwControl" value="1" />
								</OBJECT> -->
							</div>
						</div>
					</div>
				</div>
				<div class="col-sub collect-sub">
					<div class="fullfit" style="overflow:visible;">
						<div class="list_header">
							<label class="title list-title">视频文件列表</label>
						</div>
						<div class="toolbar" id="toolbar">
							<a href="#" style="margin-left:20px;_display:inline"><span class="managerOpr oprRecord" id="oprRecord">录单</span></a>
							<a href="#"><span class="managerOpr oprRelate" id="oprRelate">关联</span></a>
							<a href="#"><span class="managerOpr oprPrint" id="oprPrint">打印</span></a>
							<a href="#"><span class="managerOpr oprUpload" id="oprUpload">上传</span></a>
							<a href="#" class="moreInfo dropmove">
								<span class="managerOpr oprMore">更多</span>
								<div class="dropdown editInfo">
									<span class="managerOpr oprDelete" id="oprDelete">删除</span>
									<span class="managerOpr oprEditName" id="oprEditName">编辑录像名</span>
									<span class="managerOpr oprUpload" id="offlineUpload" style="display:none;">离线上传</span>
								</div>
							</a>
						</div>
						<div class="list_content" id="list_content">
							<!-- <ul id="list_ul">
								<li data-status="0" class="list_li"><input name="videoBox" type="checkbox" class="list_input" value="1"><label class="videoName">新录像_0-2016-04-12 13-11-58</label><span class="list_status collecting">正在采集</span></li>
								<li data-status="2" class="list_li"><input name="videoBox" type="checkbox" class="list_input" value="2"><label class="videoName">新录像_0-2016-04-12 13-11-58</label><span class="list_status related">已关联</span></li>
								<li data-status="4" class="list_li"><input name="videoBox" type="checkbox" class="list_input" value="3"><label class="videoName">新录像_0-2016-04-12 13-11-58</label><span class="list_status uploaded">已上传</span></li>
								<li data-status="3" class="list_li"><input name="videoBox" type="checkbox" class="list_input" value="4"><label class="videoName">新录像_0-2016-04-12 13-11-58</label><span class="list_status uploaded">上传..90%</span></li>
								<li data-status="4" class="list_li"><input name="videoBox" type="checkbox" class="list_input" value="5"><label class="videoName">新录像_0-2016-04-12 13-11-58</label><span class="list_status uploaded">已上传</span></li> 
							</ul> -->
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>
</body>
</html>


