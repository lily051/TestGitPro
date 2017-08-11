<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>
<link rel="stylesheet" href="/">
<div style="margin-top:10px;display:none;" id="abnormal">
	<form class="form-horizontal form-fixed formLabel140 clearfix" id="recordForm">
		<div class="control-group" style="width:60%;float:left;">
			<label class="control-label">录像结束时间：</label>
			<div class="controls">
				<input type="hidden" name="records.id" id="recordFormId" value="${recordsInfo.id}">
				<input type="hidden" hikui="calendar" name="recordBegin" datefmt="yyyy-MM-dd HH:mm:ss" 
			  		value="${recordsInfo.startTime}" id="recordBegin">
				<input type="text" class="auto-input" hikui="calendar" required="true" readonly="readonly" id="recordEnd" 
					datefmt="yyyy-MM-dd HH:mm:ss" isoverflown="true" mindate="#F{$dp.$N('recordBegin');}" maxdate="#F{$dp.$N('recordBegin',{H:3});}" data-icon="true" name="records.endTime">
			</div>
		</div>
		<span class="btn btn-submit blockBtn" id="recordSubmit" style="float:left;">提交</span>
	</form>
</div>
<div style="width:100%;height:100%;" id="dialogPlayBackDiv">
</div>

<script>
	initOcx({ocxId:'dialogPlayBackOcx',divId:"dialogPlayBackDiv",ocxType:1,modelType:modeParam.usbAndIpc});
	var recordsInfo,xml;
	if(judgeOcxAble("dialogPlayBackOcx")){
		try{
			if(modeParam.usbAndIpc=="ipc"){
				recordsInfo = {ip:"${recordsInfo.strCollectIp}",port:"${recordsInfo.dvrPort}",user:"${recordsInfo.dvrUserName}",password:"${recordsInfo.dvrPwd}",channel:"${recordsInfo.ncollectChan}",startTime:"${recordsInfo.startTime}",stopTime:"${recordsInfo.endTime}",nchannelName:"${recordsInfo.nchannelName}"};
				xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><Param><RecordType>255</RecordType><SearchType>0</SearchType><WndNo>0</WndNo><PlayType>0</PlayType><ManType>0</ManType><DvrType>100</DvrType><Right>255</Right><LoginInfo><IP>"+recordsInfo.ip+"</IP><Port>"+recordsInfo.port+"</Port><User>"+recordsInfo.user+"</User><Password>"+recordsInfo.password+"</Password><Rev>999</Rev></LoginInfo><PlayInfo><CamID>1001</CamID><CamName>"+recordsInfo.nchannelName+"</CamName><Channel>"+recordsInfo.channel+"</Channel><StartTime>"+recordsInfo.startTime+"</StartTime><StopTime>"+recordsInfo.stopTime+"</StopTime><Rev>999</Rev><lBwControl>"+modeParam.recordBandwidth+"</lBwControl></PlayInfo></Param>";
				document.getElementById("dialogPlayBackOcx").PB_Playback(xml);
			}else{
				recordsInfo = "${recordsInfo.strUrl}";
				xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><Param><VideoType>0</VideoType><FilePath>"+recordsInfo+"</FilePath></Param>";
				document.getElementById("dialogPlayBackOcx").PB_LocalPlayback(xml);
			}
		}catch(e){
		}
	}
	$("#recordSubmit").click(function(event) {
		var form = $("#recordForm");
		form.ajaxSubmit({
			beforeSubmit:function(){
				if (FinanceUtil.validate(form)){
					FinanceUtil.loader.show('请稍等...');
					return true;
				}else{
					jAlert('结束时间不能为空', '错误', 'error');
					return false;
				}
			},
			url:'/webdevice/datacollect/saveEndTime.action',
			type:'post',
			dataType:'json',
			success:function(data){
				$("#abnormal").hide();
				FinanceUtil.loader.hide();
				if (data && data.success) {
					$.sticky('操作成功', {type: "ok",title: '操作成功'});
					FMS.datacollect.getVideoList(1);
				}else if(data && data.msg){
					jAlert(data.msg, '错误' , 'error');
				}
			}
		});
	});
</script>
