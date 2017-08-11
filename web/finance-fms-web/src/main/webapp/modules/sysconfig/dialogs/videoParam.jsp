<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ taglib prefix='fmt' uri="http://java.sun.com/jsp/jstl/fmt" %>
<style>
div.selector{
	width:74px !important;
}
.videoSelect{
	width:84px !important;
}
</style>
<div id="videoParam">
	<div>
	<form class="form-horizontal form-fixed form-center">
		 <fieldset class="noLegend">
			<div class="control-group">
				<label class="control-label"><em>*</em>最大录像时长：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="num" minValue="1" maxValue="480" maxLength="3" name="recordParamInfo.maximumVideoLength" value="${recordParamInfo.maximumVideoLength }" required="true" data-icon="true" self-focus-info="请输入1至480的有效数字">
					<span class="right" style="left:inherit;_left:150px;">分钟</span>
					<!-- <div class="video">
						<input type="text" name="" class="auto-input" value="" vtype="num" maxLength="5">
						<span>小时</span>
					</div> -->
				</div>
			</div>
			<!-- <div class="control-group">
				<label class="control-label"><em>*</em>同时回放录像数：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="num" maxLength="10" name="recordParamInfo.playBackVideoNum" value="${recordParamInfo.playBackVideoNum }" required="true" data-icon="true">
					<span class="right">个</span>
				</div>
			</div> -->
			<div class="control-group">
				<label class="control-label"><em>*</em>录像回放带宽：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="bandwidth" name="recordParamInfo.videoPlayBackBandWidth" value="${recordParamInfo.videoPlayBackBandWidth}" required="true" data-icon="true">
					<span class="right" style="left:inherit">kbps</span>
				</div>
			</div>
			<!-- <div class="control-group">
				<label class="control-label"><em>*</em>上传录像带宽：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="num" maxLength="10" name="recordParamInfo.videoUploadBandWidth" value="${recordParamInfo.videoUploadBandWidth }" required="true" data-icon="true">
					<span class="right">兆</span>
				</div>
			</div> -->
 			<div class="control-group">
				<label class="control-label"><em>*</em>守卫服务工作时间段：</label>
				<div class="controls">
					<input type="text" hikui="calendar" id="startTime"  name="recordParamInfo.uploadTimeStart"  value="${recordParamInfo.uploadTimeStart}" readonly="readonly" datefmt="HH:mm" class="auto-input uploadTime" required="true" data-icon="true">
					<span class="select-span">—</span>
					<input type="text" hikui="calendar" id="endTime" name="recordParamInfo.uploadTimeEnd"  value="${recordParamInfo.uploadTimeEnd}" readonly="readonly" datefmt="HH:mm" class="auto-input uploadTime" required="true" data-icon="true">
				</div>
			</div> 
			<div class="control-group">
				<label class="control-label"><em>*</em>录像到期提示天数：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="num" minValue="10" maxValue="365" maxLength="3" name="recordParamInfo.videoDeleteDays" value="${recordParamInfo.videoDeleteDays }" required="true" data-icon="true" self-focus-info="请输入10至365的有效数字">
					<span class="right">天</span>
				</div>
			</div>
			<div class="sysBottom" style="">
				<button type="button" id="videoApp" class="sys-bPrimary">应用</button>
				<button type="button" class="sys-cancel cancelEdit">取消修改</button>
			</div>
		</fieldset>
	</form>
	</div>
</div>

<script type="text/javascript">
	$(function(){
		FinanceUtil.initFormUI("#videoParam");
		$("#videoApp").on('click', function(event) {
			FMS.sysconfig.videoParamApp();
		});
		$(".cancelEdit").on('click', function(event) {
			FMS.sysconfig.cancelEdit();
		});
		/*$.extend($.vtype,{
		     endtime : {
		         validator : function(val){
		         	var startTime = $("#startTime").val().split(":");
		         	var startSecond = startTime[0]*60+startTime[1];
		         	var endTime = val.split(":");
		         	var endSecond = endTime[0]*60+endTime[1];
		         	if((endSecond - startSecond)<=0){
		         		return false;
		         	}else{
		         		return true;
		         	} 
		         },
		         msg: '结束时间不能比上传时间早'
		     }
		});*/
		$.extend($.vtype,{
		     bandwidth : {
		         validator : function(val){
		            if(val<1024*0.25||val>1024*10){
		            	return false;
		            }else{
		            	return true;
		            }
		         },
		         msg: '',
		         info : '带宽限制范围在256~10240kbps之间，只支持DVR和NVR'
		     }
		});
	});
</script>