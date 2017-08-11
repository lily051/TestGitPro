<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<script src="${ctx}/baseui/js/plugins/ui/jquery.linkagesel.js"></script>
<div>
	<div class="height20"></div>
	<form class="form-horizontal form-fixed">
		<fieldset class="noLegend">
			<div class="control-group" id="channelDiv">
				<label class="control-label"><em>*</em>通道名称：</label>
				<div class="controls">
					<select name="dataCollectInfo.deviceChannelId" id="channelInfo">
					</select>
				</div>
			</div>
		</fieldset>
	</form>
</div>
<script>
	$(function(){
		$("#channelInfo").iselect({
		    url: '/webdevice/datacollect/getChannels.action',
		    root: "data.root",//返回数据的根属性
		    displayName: 'nchannelName', //显示名字
		    valueName: 'id'//实际要传的id
		});
	});	
</script>