<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>

<div>
	<div class="msg-b msg-b-attention msg-b-4" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;padding-left:49px;">
	    <div class="msg-cnt">
	        <label class="popInfo"><span>红色<em>*</em>为必填项</span></label>
	    </div>
	</div>
	<form class="form-horizontal form-fixed clearfix" style="margin-left:10px;">
		<input type="hidden" value="${deviceChannelInfo.id }" name="deviceChannelInfo.id" >
		<fieldset class="noLegend">
			<div class="control-group">
				<label class="control-label">所属区域：</label>
				<div class="controls">
					${deviceChannelInfo.regionName }
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">所属设备：</label>
				<div class="controls">
					${deviceChannelInfo.ndeviceName }
				</div>
			</div> 
			<div class="control-group">
				<label class="control-label">通道号：</label>
				<div class="controls">
					${deviceChannelInfo.nchannelNo }
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>通道名称：</label>
				<div class="controls">
					<input type="text" value="${deviceChannelInfo.nchannelName }" name="deviceChannelInfo.nchannelName" class="auto-input" required="true" vtype="common" isoverflown="true" maxLength="20">
				</div>
			</div>
			<div class="control-group" id = "userIp">
				<label class="control-label">理财电脑IP：</label>
				<div class="controls">
					<input type="text" value="${deviceChannelInfo.userIp }" name="deviceChannelInfo.userIp" class="auto-input" vtype="ip" isipwildcard="false"
					 maxLength="15" isoverflown="true" promptposition="topLeft" >
				</div>
			</div>
		</fieldset>
	</form>
</div>
<script type="text/javascript">
	$(function(){
		if (modeParam.usbAndIpc == "ipc" && modeParam.channelIpBind != "true") {
			document.getElementById("userIp").style.visibility="hidden";
		}
	});
</script>