<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<link rel="stylesheet" href="/modules/device/css/collectDevice.css">
<script src="/modules/device/collect/js/deviceDialog.js"></script>
<style type="text/css">
	.dialog{
		overflow-y:scroll !important;
	}
</style>

<div>
	<div class="msg-b msg-b-attention msg-b-4" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;padding-left:49px;">
	    <div class="msg-cnt">
	        <label class="popInfo"><span>红色<em>*</em>为必填项</span></label>
	    </div>
	</div>
	<form class="form-horizontal form-fixed" style="margin-left:10px">
		<fieldset class="noLegend thirtyField">
			<div class="legendTitle"><span class="titleLine"></span>基本信息</div>
			<div class="control-group">
				<label class="control-label">所属区域：</label>
				<input type="hidden" name="deviceInfo.nregionId" value="${deviceInfo.nregionId }"></input>
				<div class="controls"><span class="notEdit" title="${deviceInfo.regionName}">${deviceInfo.regionName}</span></div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>设备名称：</label>
				<div class="controls">
					<input type="text" class="auto-input" id="addDeviceName" vtype="common" required="true" maxLength="20"
						isoverflown="true" data-icon="true" name="deviceInfo.strName" >
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">设备厂商：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="common" maxLength="50" isoverflown="true" data-icon="true" name="deviceInfo.strManufacture" value="海康威视">			
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>设备端口：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="num" required="true" maxValue="65535" minValue="1" maxLength="5" value="8000" 
						isoverflown="true"  data-icon="true" id="dev_port" name="deviceInfo.nport" self-focus-info="请输入1至65535的有效数字">
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>设备用户名：</label>
			    <div class="controls">
			       <input type="text" class="auto-input" vtype="common" required="true" maxLength="50" 
						isoverflown="true"  data-icon="true" id="dev_user" name="deviceInfo.strUser" value="admin">
			    </div>
			</div>
			<div class="control-group" id="ntpGroup">
				<label class="control-label">NTP校时：</label>
			    <div class="controls">
			    	<span class="check">
					<input type="checkbox" name="deviceInfo.ntpEnable" id="ntpEnable" checked="checked" value="1">
					</span>
					<input type="hidden" id="ntp_enable" value="${ntpParamInfo.nenable }">
					<input type="hidden" id="ntp_addr" value="${ntpParamInfo.ntpAddress }">
					<input type="hidden" id="ntp_port" value="${ntpParamInfo.ntpPortNo }">
					<input type="hidden" id="ntp_interval" value="${ntpParamInfo.ntpInterval }">
			    </div>
			</div>
		</fieldset>
		<fieldset class="noLegend thirtyField">
			<div class="legendTitle"><span class="titleLine" style="background:white"></span></div>
			<div class="control-group">
				<label class="control-label">设备类型：</label>
				<div class="controls">
					<select name="deviceInfo.ntype" id="deviceType">
						<option value="50000">DVR</option>
						<option value="50012">NVR</option>
					</select>
				</div>
			</div>
			
			<div class="control-group">
				<label class="control-label"><em>*</em>设备编码：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="common" required="true" maxLength="20"
						isoverflown="true" data-icon="true" name="deviceInfo.strIndexCode">					
				</div>
			</div>
			
			<div class="control-group">
				<label class="control-label"><em>*</em>设备IP：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="ip" required="true" maxLength="15"
						isoverflown="true" data-icon="true" id="deviceIp" name="deviceInfo.strIp"  autocomplete="off">
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">码流类型：</label>
				<div class="controls">
					<select name="deviceInfo.nstreamType" id="">
						<option value="0">主码流</option>
						<option value="1">子码流</option>
					</select>
				</div>
			</div>
			
			<div class="control-group">
				<label class="control-label"><em>*</em>设备密码：</label>
			    <div class="controls">
			       <input type="password" class="auto-input" required="true" maxLength="20" 
						isoverflown="true" id="dev_pwd"  data-icon="true" autocomplete="off" name="">
			    </div>
			</div>
			<div class="control-group">
				<label class="control-label"></label>
			    <div class="controls">
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
		</fieldset>
		<fieldset class="noLegend thirtyField" id = "relateStorageDevice">
			<div class="legendTitle"><span class="titleLine"></span>关联存储设备</div>
			<div class="control-group">
				<label class="control-label">设备名称：</label>
			    <div class="controls" style="position:relative;" id="selectControls" data-type="add">
			       <input id="selectDevice" type="text" value="" class="auto-input" placeholder="点击选择" readonly="readonly" name="">
			      <input type="hidden" id="storageId" name="deviceInfo.ndevStoId" value="${deviceInfo.ndevStoId}">
			       <div class="input-select">
			       		<div class="treeview" style="width:auto;_width:192px;">
			       		    <ul id="storageTree" class="ztree"></ul>
			       		</div>
			       </div>
			    </div>
			</div>
			<div class="control-group groupShow">
			    <div class="controls">
			    	<input type="hidden" id="subsection" name="deviceInfo.groupIndexId" value="">
			        <div class="subInfo">
			       		<div class="subInfo1">分卷信息</div>
			       		<div class="addcollect">
			       		</div>
			        </div>
			    </div>
			</div>
		</fieldset>
		<fieldset class="noLegend" style="float:left;width:100%;_width:1044px">
			<div class="legendTitle"><span class="titleLine"></span>通道信息</div>
			<div class="channelInfo">
				<div class="clickA"><a>点击获取通道信息</a></div>
				<div class="channelTable">
					<fieldset class="noLegend thirtyField">
						<div class="control-group">
							<label class="control-label"><em>*</em>模拟通道数：</label>
							<div class="controls" id="sChannel"></div>
							<input type="hidden" name="deviceInfo.nchanNum" value="">
						</div>
					</fieldset>
					<fieldset class="noLegend thirtyField">
						<div class="control-group">
							<label class="control-label"><em>*</em>IP通道数：</label>
							<div class="controls" id="ipChannel"></div>
							<input type="hidden" name="deviceInfo.telChanNum" value="">
						</div>
					</fieldset>
					<div style="width:972px;_height:35px;float:left">
						<table class="table">
							<col width="10%"/>
							<col width="30%"/>
							<col width="30%"/>
							<col width="30%"/>
							<thead>
								<tr>
									<th>序号</th>
									<th><span class="spanVer">|</span>通道号</th>
									<th><span class="spanVer">|</span>通道类型</th>
									<th><span class="spanVer">|</span>通道名称</th>
								</tr>
							</thead>
						</table>
					</div>
					<div style="width:100%;height:119px;_height:119px;overflow-y:auto;overflow-x:hidden;float:left;">
						<table class="table" id="channelDatatable" style="width:972px;">
							<col width="10%"/>
							<col width="30%"/>
							<col width="30%"/>
							<col width="30%"/>
						</table>
					</div>
				</div>
			</div>
		</fieldset>
		
	</form>
	
</div>
