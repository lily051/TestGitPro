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
				<input type="hidden" name="deviceInfo.id" value="${deviceInfo.id }"></input>
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
						isoverflown="true" data-icon="true" name="deviceInfo.strName"  value="${deviceInfo.strName}">
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>设备编码：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="common" required="true" maxLength="20"
						isoverflown="true" data-icon="true" id="deviceCode" name="deviceInfo.strIndexCode" value="${deviceInfo.strIndexCode }">					
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">设备厂商：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="common" maxLength="50" isoverflown="true" data-icon="true" name="deviceInfo.strManufacture" value="${deviceInfo.strManufacture}">			
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>设备IP：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="ip" required="true" maxLength="15"
						isoverflown="true" data-icon="true" id="deviceIp" name="deviceInfo.strIp" value="${deviceInfo.strIp}">
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>设备端口：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="num" required="true" minValue="1" maxLength="5" maxValue="65535" 
						isoverflown="true"  data-icon="true" id="dev_port" name="deviceInfo.nport" value="${deviceInfo.nport}" self-focus-info="请输入1至65535的有效数字">
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>设备用户名：</label>
			    <div class="controls">
			       <input type="text" class="auto-input" vtype="common" required="true" maxLength="50" 
						isoverflown="true"  data-icon="true" id="dev_user" name="deviceInfo.strUser" value="${deviceInfo.strUser}">
			    </div>
			</div>
			<div class="control-group" id = "dev_password">
				<label class="control-label"><em>*</em>设备密码：</label>
			    <div class="controls">
			       <input type="password" class="auto-input" required="true" maxLength="20" 
						isoverflown="true" data-icon="true" autocomplete="off" id="dev_pwd" name="" value="">
					<input type="hidden" id="dev_pwd_old" value="${deviceInfo.strPassword}">
			    </div>
			</div>
			<div class="control-group" id = "dev_accessKey">
				<label class="control-label"><em>*</em>访问Key：</label>
			    <div class="controls">
			       <input type="password" class="auto-input" maxLength="100" vtype="common"
						isoverflown="true" data-icon="true" autocomplete="off" id="accessKey" name="">
					<input type="hidden" id="accessKey_old" value="${deviceInfo.accessKey}">
			    </div>
			</div>
			<div class="control-group" id = "dev_secretKey">
				<label class="control-label"><em>*</em>加密Key：</label>
			    <div class="controls">
			       <input type="password" class="auto-input" maxLength="100" vtype="common"
						isoverflown="true" data-icon="true" autocomplete="off" id="secretKey" name="">
					<input type="hidden" id="secretKey_old" value="${deviceInfo.secretKey}">
			    </div>
			</div>
			<div class="control-group">
				<label class="control-label" id="ntpGroup">NTP校时
					<span class="check">
					<input type="checkbox" name="deviceInfo.ntpEnable" id="ntpEnable" checked="checked" value="${deviceInfo.ntpEnable }" data-ntp="${deviceInfo.ntpEnable }">
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
			    	<input type="hidden" name="deviceInfo.strPasswordLevel" id="strPasswordLevel" value="${deviceInfo.strPasswordLevel}">
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
			    	<input type="hidden" name="deviceInfo.flag" value="0" id="poolFlag">
			       <a id="getInfo" data-click="0">点击重新获取分卷信息</a>
			    </div>
			</div>
			<div class="control-group groupHidden showBlock">
			    <div class="controls">
			       <div class="subInfo">
			       		<div class="subInfo1">分卷信息</div>
			       		<div class="viewInfo">
			       			<s:iterator id="subInfo" value="deviceInfo.deviceChannelList">
			       			<p data-poolid="${subInfo.nchannelNo }"> ${subInfo.nchannelName } </p>
			       			</s:iterator>
			       		</div>
			       </div>
			    </div>
			</div>
		</fieldset>
	</form>
</div>

<script type="text/javascript">
	var pwd = FMS.device.base64Decode($("#dev_pwd_old").val());
	$("#dev_pwd").val(pwd);
	var stoType={"50011":[["50011","CVR"]],
				 "50015":[["50015","云存储"]],
				 "50050":[["50050","KMS"]],
				 "50060":[["50060","云存储-对象存储"]]}
	var accessKey = FMS.device.base64Decode($("#accessKey_old").val());
	$("#accessKey").val(accessKey);
	var secretKey = FMS.device.base64Decode($("#secretKey_old").val());
	$("#secretKey").val(secretKey);
	var	ntype = "${deviceInfo.ntype}";
	$("#deviceType").iselect({
		localData: stoType[ntype],
		value:ntype
	});

	FMS.device.initLevel();
	if($('#ntpEnable').val()==0){
		$.uniform.update($("#ntpEnable").removeAttr('checked'));
		ntpGroupControl();
	}
	cloudStorageObjectStorageControl();
	if($('#ntp_enable').val()==0){
		$('#ntpGroup').hide();
	}else{
		$('#ntpEnable').click(function(){
			if($(this).prop('checked')){
				$('#ntpEnable').val('1');
			}else{
				$('#ntpEnable').val('0');
			}
			$.uniform.update();
		});
	}
	
	
	function ntpGroupControl(){
		//kms 云存储不支持ntp校时
		if($('#deviceType').val()==Constants.device.DEV_TYPE_KMS||$('#deviceType').val()==Constants.device.DEV_TYPE_CLOUD_STORAGE || $('#deviceType').val()==Constants.device.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE){
			$('#ntpGroup').hide();
		}else{
			$('#ntpGroup').show();
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
		}
	}
</script>