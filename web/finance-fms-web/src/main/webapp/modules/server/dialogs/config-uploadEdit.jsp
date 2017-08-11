<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<div>
<form class="form-horizontal form-fixed formLabel150">
        <div class="msg-b msg-b-attention" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;">
            <div class="msg-cnt">
                <span>红色<em>*</em>为必填项</span>
            </div>
        </div>
        <input type="hidden" name="serverInfo.uploadType" id="uploadType">
        <fieldset class="noLegend">
            <div class="control-group ">
                <label class="control-label">服务类型：</label>
                 <input value="20004" type="hidden" name="serverInfo.ntype">
                <div class="controls">
                    <span class="serverType">上传服务</span>
                    <input value="${serverInfo.id}" type="hidden" name="serverInfo.id">
                </div>
            </div>
            <div class="control-group">
                <label class="control-label"><em>*</em>服务名称：</label>
                <div class="controls">
                    <input type="text" vtype="common" maxLength="50" required="true" class='auto-input' data-icon="true" isoverflown="true" name="serverInfo.strUser" value="${serverInfo.strUser}"> 
                </div>
            </div>
            <div class="control-group usbOrIPC">
                <label class="control-label"><em>*</em>服务IP：</label>
                <div class="controls">
                    <input type="text" vtype="ip" maxLength="15" required="true" class='auto-input' data-icon="true" isoverflown="true" name="serverInfo.strIp" value="${serverInfo.strIp}"> 
                </div>
            </div>
<!--             <div class="control-group">
                <label class="control-label">控制端口：</label>
                <div class="controls">
                    <input name="server.nctrlPort" value="" type="text" class="auto-input"
                                 vtype="num" maxValue="65535" minValue="1" maxLength="5"
                                isOverflown="true"  data-icon="true" self-focus-info="请输入1至65535的有效数字"/>
                </div>
            </div>
            <div class="control-group">
                <label class="control-label">数据端口：</label>
                <div class="controls">
                    <input name="server.ndataPort" value="" type="text" class="auto-input"
                                vtype="num" maxValue="65535" minValue="1" maxLength="5"
                                isOverflown="true"  data-icon="true" self-focus-info="请输入1至65535的有效数字"/>
                </div>
            </div> -->
            <div class="control-group ">
                <label class="control-label"><em>*</em>上传间隔时间(s)：</label>
                <div class="controls">
                    <input type="text" class="auto-input" placeholder="2" id="capturespace" required="true" vtype="num"
                                    minValue="2" maxValue="1000" maxLength="4" isOverflown="true" data-icon="true" name="serverInfo.timeOut" value="${serverInfo.timeOut}" self-focus-info="请输入2至1000的有效数字"/>
                </div>
            </div>
            
            <div class="control-group speed">
                <label class="control-label"><em>*</em>最大上传速度(kbps)：</label>
                <div class="controls">
                	<input type="text" class="auto-input" value="4096" id="" required="true" vtype="num"
                                    minValue="256" maxValue="10240" isOverflown="true" data-icon="true" name="serverInfo.speed" value="${serverInfo.speed}" self-focus-info="请输入256至10240的有效数字" />
                </div>
            </div>
            
            <div class="control-group usbOrIPC">
                <label class="control-label"><em>*</em>同时上传任务数：</label>
                <div class="controls">
                    <input name="serverInfo.taskCount" value="${serverInfo.taskCount}" type="text" class="auto-input"
                                required="true" vtype="num" maxValue="65535" minValue="1" maxLength="5" placeholder="10"
                                isOverflown="true"  data-icon="true" self-focus-info="请输入1至65535的有效数字"/>
                </div>
            </div>
            <div class="control-group">
                <label class="control-label"><em>*</em>上传时间：</label>
                <div class="controls">
                    <input type="text" hikui="calendar" id="startTime" name="serverInfo.workTimeBegin" value="${serverInfo.workTimeBegin}"  readonly="readonly" datefmt="HH:mm" class="auto-input uploadTime" style="width:59px !important;_width:50px !important" required="true" data-icon="true">
                    <span class="select-span">—</span>
                    <input type="text" hikui="calendar" id="endTime" name="serverInfo.workTimeEnd" value="${serverInfo.workTimeEnd}"  readonly="readonly" datefmt="HH:mm" class="auto-input uploadTime" style="width:58px !important;_width:50px !important" required="true" data-icon="true">
                </div>
            </div>
            <div class="control-group">
                <label class="control-label"><em>*</em>巡检服务IP：</label>
                <div class="controls">
                    <input type="text" vtype="ip" maxLength="50" required="true" class='auto-input' data-icon="true" isoverflown="true" name="serverInfo.strSysIp" value="${serverInfo.strSysIp}"> 
                </div>
            </div>
            <div class="control-group">
                <label class="control-label"><em>*</em>巡检服务端口：</label>
                <div class="controls">
                    <input name="serverInfo.nctrlSysPort" value="${serverInfo.nctrlSysPort}" type="text" class="auto-input"
                                required="true" vtype="num" maxValue="65535" minValue="1" maxLength="5"
                                isOverflown="true"  data-icon="true" self-focus-info="请输入1至65535的有效数字"/>
                </div>
            </div>
        </fieldset>
</form>
</div>
<script>
	$("#uploadType").val(modeParam.usbAndIpc);
	if(modeParam.usbAndIpc=="usb"){
	    $('.usbOrIPC').remove();
	} else {
		$('.speed').remove();
	}
</script>