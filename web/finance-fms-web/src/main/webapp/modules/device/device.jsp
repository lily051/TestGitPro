<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<head>
    <meta charset="utf-8">
    <%@ include file="/common/commons.jsp" %>
	<%@ include file="/common/commons-extend.jsp" %>
	<link  href="/modules/device/css/device.css" rel="stylesheet" />
	<script src="/js/ocxVersion/initOCX.js"></script>
	<script type="text/javascript" src="/modules/device/js/base64.js"></script>
	<script type="text/javascript" src="/modules/device/js/device.js"></script>
</head>
<body>
	<div id="wrapper">
		<%@ include file="/common/commons-basemenu.jsp"%>
		<div id="content" class="layout">
			<div class="container wrapper grid-s9m0 FMS_container">
				<div class="col-main">
					<div class="main-wrap wrapFixed">
						<div class="fullfit">
							<div class="devicetool">
					             <ul>
					               <li data-url="/modules/device/toStoPage.action" id = "storageDeviceManage"><span>存储设备管理</span></li>
					               <li data-url="/modules/device/toCollPage.action" id = "collectDeviceManage"><span>采集设备管理</span></li>
					               <li data-url="/modules/device/toChannelMngPage.action" id = "channelManage"><span>通道管理</span></li>
					             </ul>
				            </div>
				           <div class="deviceMain"></div>
						</div>
					</div>
				</div>
				<div class="col-sub">
					<jsp:include page="/common/common-tree.jsp" flush="true" >
						<jsp:param name="className" value="orgTree" />
					</jsp:include>
				</div>
			</div>
		</div>
		<!-- <OBJECT id=RPConfig_OCX classid=clsid:A4660BF5-7356-4A3B-8A34-5859ED6C4F2F codeBase ="#version=3,0,0,49191" Width=1
		Height=1> -->
	</div>
	
</body>
 <script>
	if (modeParam.usbAndIpc == "ipc" && modeParam.nonCenterStorage == "true") {
		function changePage(){ 
			$("#collectDeviceManage").addClass("active");
			$("#storageDeviceManage").hide();
			$("#storageDeviceManage").attr("data-url", "/modules/device/toCollPage.action");
		}
		window.changePage();		
	} else {
		$("#storageDeviceManage").addClass("active");
	}
</script>
</html>


