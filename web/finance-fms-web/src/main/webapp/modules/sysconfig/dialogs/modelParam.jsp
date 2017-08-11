<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>

<div id="modelParam">
	<form class="form-horizontal form-fixed form-center">
		 <fieldset class="noLegend">
			<div class="control-group">
				<label class="control-label">产品复核：</label>
				<div class="controls">
					<label><span class="check"><input name="modeParamInfo.productReview" type="radio" value="1" <s:if test="modeParamInfo.productReview==1">checked</s:if>></span><span>启用</span></label>
					<label><span class="check"><input name="modeParamInfo.productReview" type="radio" value="0" <s:if test="modeParamInfo.productReview==0">checked</s:if>></span><span>禁用</span></label>
				</div>
			</div>
			<div class="control-group" id = "osdOverlay">
				<label class="control-label">水印叠加：</label>
				<div class="controls">
					<label><span class="check"><input name="modeParamInfo.overlayOsd" type="radio" value="1" <s:if test="modeParamInfo.overlayOsd==1">checked</s:if>></span><span>启用</span></label>
					<label><span class="check"><input name="modeParamInfo.overlayOsd" type="radio" value="0" <s:if test="modeParamInfo.overlayOsd==0">checked</s:if>></span><span>禁用</span></label>
				</div>
			</div> 
			
			<div class="control-group">
				<label class="control-label">通道方式：</label>
				<div class="controls">
					<span id = "isChoiceUsbIpc"></span>
				</div> 
			</div>
			<div class="control-group" id = "nonCenterStorage">
				<label class="control-label">无中心存储：</label>
				<div class="controls">
					<span id = "isNonCenterStorage"></span>
				</div> 
			</div>
			<div class="control-group" id = "channelIpBind">
				<label class="control-label">通道IP绑定：</label>
				<div class="controls">
					<span id = "isChannelIpBind"></span>
				</div> 
			</div>
			<div class="control-group" id = "logReport">
				<label class="control-label">客户端日志上报：</label>
				<div class="controls">
					<span id = "isLogReport"></span>
				</div> 
			</div>
			<div class="control-group" id = "logReportMethod">
				<label class="control-label">客户端日志上报方式：</label>
				<div class="controls">
					<span id = "isLogReportMethod"></span>
				</div> 
			</div>
			<div class="sysBottom">
				<button type="button" class="sys-bPrimary" id="modelApp">应用</button>
				<button type="button" class="sys-cancel cancelEdit">取消修改</button>
			</div>
		</fieldset>
	</form>
</div>

<script type="text/javascript">
	$(function(){
		if ("${modeParamInfo.choiceUsbIpc}" == "ipc") {//ipc模式下，将客户端日志上报 隐藏
			$("#logReport").hide();
			$("#logReportMethod").hide();
		} else {//usb模式下，将无中心存储、通道ip绑定 隐藏
			$("#osdOverlay").hide();
			$("#nonCenterStorage").hide();
			$("#channelIpBind").hide();
			if ("${modeParamInfo.logReport}" == "false") {
				$("#logReportMethod").hide();
			}
		}
		displayParameters();//参数配置-模式参数页面中，展示参数
		
		var operName = "${modeParamInfo.operName}";
		if(operName!="admin"){
			$("#choiceUsbIpc").remove();
		}
		FinanceUtil.initFormUI("#modelParam");
		$("#modelApp").on('click', function(event) {
			FMS.sysconfig.modelParamApp();
		});
		$(".cancelEdit").on('click', function(event) {
			FMS.sysconfig.cancelEdit();
		});
	});
	
	function displayParameters() {
		var isChoiceUsbIpc = "${modeParamInfo.choiceUsbIpc}";
		var isNonCenterStorage = "${modeParamInfo.nonCenterStorage}";
		var isChannelIpBind = "${modeParamInfo.nipLimit}";
		var isLogReport = "${modeParamInfo.logReport}";
		var isLogReportMethod = "${modeParamInfo.logReportMethod}";
		if (isChoiceUsbIpc == "ipc") {$("#isChoiceUsbIpc").html("IPC通道");} else {$("#isChoiceUsbIpc").html("USB通道");}
		if (isNonCenterStorage == "true") {$("#isNonCenterStorage").html("启用");} else {$("#isNonCenterStorage").html("禁用");}
		if (isChannelIpBind == "true") {$("#isChannelIpBind").html("启用");} else {$("#isChannelIpBind").html("禁用");}
		if (isLogReport == "true") {$("#isLogReport").html("启用");} else {$("#isLogReport").html("禁用");}
		if (isLogReportMethod == "http") {$("#isLogReportMethod").html("HTTP方式");} else {$("#isLogReportMethod").html("FTP方式");}
	}
</script>