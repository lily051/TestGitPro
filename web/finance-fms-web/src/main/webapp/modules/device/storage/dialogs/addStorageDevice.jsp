<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<script src="/modules/device/storage/js/storageDialog.js"></script>
<div class="iScroll">
	<form class="form-horizontal form-fixed formLabel120">
		<div class="msg-b msg-b-attention" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;">
		    <div class="msg-cnt">
		         <label class="popInfo"><span>红色<em>*</em>为必填项</span></label>
		    </div>
		</div>
		<fieldset class="noLegend">
			<div class="control-group">
				<label class="control-label">所属区域：</label>
				<input type="hidden" name="deviceInfo.nregionId" value="${deviceInfo.nregionId }"></input>
				<div class="controls"><span class="notEdit" title="${deviceInfo.regionName}">${deviceInfo.regionName}</span></div>
			</div>
			<div class="control-group">
				<label class="control-label">设备类型：</label>
				<div class="controls">
					<select name="deviceInfo.ntype" id="deviceType">
					</select>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>设备名称：</label>
				<div class="controls">
					<input type="text" class="auto-input" id="addDeviceName" vtype="common" required="true" maxLength="20"
						isoverflown="true" data-icon="true" name="deviceInfo.strName" >
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>设备编码：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="common" required="true" maxLength="20"
						isoverflown="true" data-icon="true" id="deviceCode" name="deviceInfo.strIndexCode">					
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">设备厂商：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="common" maxLength="50" isoverflown="true" data-icon="true" name="deviceInfo.strManufacture" value="海康威视">		
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>设备IP：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="ip" required="true" maxLength="15"
						isoverflown="true" data-icon="true" id="deviceIp" name="deviceInfo.strIp">
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>设备端口：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="num" required="true" minValue="1" maxLength="5" maxValue="65535" 
						isoverflown="true"  data-icon="true" id="dev_port" name="deviceInfo.nport" self-focus-info="请输入1至65535的有效数字">
				</div>
			</div>
			<div class="control-group" id = "dev_name">
				<label class="control-label"><em>*</em>设备用户名：</label>
			    <div class="controls">
			       <input type="text" class="auto-input" vtype="common" required="true" maxLength="50" 
						isoverflown="true"  data-icon="true" id="dev_user" name="deviceInfo.strUser" value="">
			    </div>
			</div>
			<div class="control-group" id = "dev_password">
				<label class="control-label"><em>*</em>设备密码：</label>
			    <div class="controls">
			       <input type="password" class="auto-input" required="true" maxLength="20" 
						isoverflown="true" data-icon="true" autocomplete="off" id="dev_pwd" name="">
			    </div>
			</div>
			<div class="control-group" id = "dev_accessKey">
				<label class="control-label"><em>*</em>访问Key：</label>
			    <div class="controls">
			       <input type="password" class="auto-input" maxLength="100" vtype="common"
						isoverflown="true" data-icon="true" autocomplete="off" id="accessKey" name="">
			    </div>
			</div>
			<div class="control-group" id = "dev_secretKey">
				<label class="control-label"><em>*</em>加密Key：</label>
			    <div class="controls">
			       <input type="password" class="auto-input" maxLength="100" vtype="common"
						isoverflown="true" data-icon="true" autocomplete="off" id="secretKey" name="">
			    </div>
			</div>
			<div class="control-group" >
				<label class="control-label" id="ntpGroup">NTP校时
					<span class="check">
					<input type="checkbox" name="deviceInfo.ntpEnable" id="ntpEnable" checked="checked" value="1">
					</span>
					&nbsp;&nbsp;
					<input type="hidden" id="ntp_enable" value="${ntpParamInfo.nenable }">
					<input type="hidden" id="ntp_addr" value="${ntpParamInfo.ntpAddress }">
					<input type="hidden" id="ntp_port" value="${ntpParamInfo.ntpPortNo }">
					<input type="hidden" id="ntp_interval" value="${ntpParamInfo.ntpInterval }">
				</label>
			    <div class="controls" style="_margin-left:130px;" id="securityLevel">
			    	<span class="password">密码等级</span>
			    	<input type="hidden" name="iRank" value="${passwordSecurityLevel}" id="iRank"/>
			    	<input type="hidden" name="deviceInfo.strPasswordLevel" id="strPasswordLevel">
			    	<div class="rePWD" id="colorPWD">
			    	    <span class="" title="密码长度小于8位，或者只包含数字，小写字母，大写字母，特殊字符4类字符中的任意一类，或者密码与用户名一样或是用户名的倒写(空格不算字符)"></span>
			    	    <span class="" title="密码长度大于8位，包含两类字符，且组合为（数字+小写字母）或（数字+大写字母）(空格不算字符)"></span>
			    	    <span class="" title="密码长度大于8位，包含两类字符，且组合不能为（数字+小写字母）和（数字+大写字母)(空格不算字符)"></span>
			    	    <span class="" title="密码长度大于8位，包含数字，小写字母，大写字母，特殊字符三类字符及以上(空格不算字符)"></span>
			    	    <i id="iRankText"></i>
			    	</div>
			    </div>
			</div>
			<div class="control-group">
				<label class="control-label"></label>
			    <div class="controls">
			       <a id="getInfo">点击获取分卷信息</a>
			    </div>
			</div>
			<div class="control-group groupHidden">
			    <div class="controls">
			       <div class="subInfo">
			       		<div class="subInfo1">分卷信息</div>
			       		<div class="viewInfo">
			       		</div>
			       </div>
			    </div>
			</div>
		</fieldset>
		
	</form>
	
</div>

<script>
	$(function(){
		var	usbAndIpc = modeParam.usbAndIpc;
		if(usbAndIpc == "ipc"){
			var data = [[Constants.device.DEV_TYPE_CVR,"CVR"],[Constants.device.DEV_TYPE_CLOUD_STORAGE,"云存储"]];
		}
		
		if(usbAndIpc == "usb"){
			data = [[Constants.device.DEV_TYPE_KMS,"KMS"], [Constants.device.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE,"云存储-对象存储"]];
		}
		$("#deviceType").iselect({
			localData: data
		}).change(function(){
			setStoragePort($('#deviceType').val());
			ntpGroupControl();
			cloudStorageObjectStorageControl();
		});
		setStoragePort($('#deviceType').val());
		ntpGroupControl();
		cloudStorageObjectStorageControl();
	});
	
	function setStoragePort(type){
		var port = 8000;
		var admin = 'admin';
		switch(parseInt(type)){
			case Constants.device.DEV_TYPE_CVR: 		port = 8000; admin = 'admin'; break;
			case Constants.device.DEV_TYPE_CLOUD_STORAGE: port = 6021; admin = 'root'; break;
			case Constants.device.DEV_TYPE_KMS: 		port = 8080; admin = 'admin'; break;
			case Constants.device.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE: port = 6201; admin = 'whhik123'; break;
		}
		$("#dev_port").val(port);
		$("#dev_user").val(admin);
	}
	
	function ntpGroupControl(){
		//kms 云存储不支持ntp校时
		if($('#deviceType').val()==Constants.device.DEV_TYPE_KMS||$('#deviceType').val()==Constants.device.DEV_TYPE_CLOUD_STORAGE || $('#deviceType').val()==Constants.device.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE){
			$('#ntpGroup').hide();
			$('#ntpEnable').val('0');
		}else{
			$('#ntpGroup').show();
			$('#ntpEnable').val('1');
		}
	}
	
	function cloudStorageObjectStorageControl() {
		//云存储-对象存储 隐藏设备密码、密码等级
		if ($('#deviceType').val()==Constants.device.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE) {
			$('#dev_pwd').attr("required",false);
			$('#accessKey').attr("required",true);
			$('#secretKey').attr("required",true);
			
			$('#dev_password').hide();
			$('#dev_accessKey').show();
			$('#dev_secretKey').show();
			$('#securityLevel').hide();
		} else {
			$('#dev_password').show();
			$('#dev_accessKey').hide();
			$('#dev_secretKey').hide();
			$('#securityLevel').show();
		}
	}
</script>