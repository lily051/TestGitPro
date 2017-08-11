<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<script type="text/javascript" src="/modules/device/storage/js/storageDevice.js"></script>
<div class="fullfit">
	<div class="toolbar">
		<a href="#"><span class="managerOpr oprAdd oprAddDevice"><i>添加</i></span></a>
		<a href="#"><span class="managerOpr oprDelete oprDeleteDevice"><i>删除</i></span></a>
		<a href="#"><span class="managerOpr oprImport"><i>导入</i></span></a>
		<a href="#"><span class="managerOpr oprExport"><i>导出</i></span></a>
		<a href="#"><span class="managerOpr oprNtp"><i>NTP校时</i></span></a>
		<label class="showChild"><span class="check"><input type="checkbox" class="showsub" checked="checked"></span><span class="showSubStyle">显示所有子机构</span></label>
		<span class="filtrateIcon" id="filtrate" data-mutex="close"></span>
	</div>
	<div class="filterFormBox ieStyle clearfix" style="margin:0;height:0;" id="filterFormBox">
	    <form class="form-horizontal clearfix" id="filterForm">
	    	<fieldset class="noLegend fieldset_25">
	    	    <div class="control-group">
	    	        <label class="control-label">设备名称：</label>
	    	        <div class="controls conditions">
	    	            <input type="text" name="deviceName" class='username' value="" id="deviceName">
	    	        </div>
	    	    </div>
	    	    <div class="control-group">
	    	        <label class="control-label">在线状态：</label>
	    	        <div class="controls conditions">
	    	             <select name="onlineStatus" id="onlineStatus">
	                        <option value="">全部</option>
	                        <option value="1">在线</option>
	                        <option value="0">离线</option>
	                    </select>
	    	        </div>
	    	    </div>
	    	</fieldset>
    		<fieldset class="noLegend fieldset_25">
    		    <div class="control-group">
    		        <label class="control-label">IP地址：</label>
    		        <div class="controls conditions">
    		            <input type="text" name="ip" vtype="" class='staffNo' value="" id="ip">
    		        </div>
    		    </div>
    		    <div class="control-group">
    		        <label class="control-label">密码等级：</label>
    		        <div class="controls conditions">
    		             <select name="passwordLevel" id="passwordLevel">
	                        <option value="">全部</option>
	                         <option value="0">风险密码</option>
	                        <option value="1">弱密码</option>
	                        <option value="2">中密码</option>
	                        <option value="3">强密码</option>
	                        <option value="4">无密码</option>
	                        <option value="-1">未知</option>
	                    </select>
    		        </div>
    		    </div>
    		</fieldset>
	        <div class="filterDIV filterBtn filterBtnForCol2">
	            <span class="btn btn-submit blockBtn" id="search">查询</span>
	            <span class="btn btn-cancel blockBtn btnGap" id="clear">重置</span>
	        </div>
	    </form>
	</div>
	<div class="datatableStyle">
		<table id="deviceDatatable"></table>
		<div id="deviceDatatablePage"></div>
	</div>
</div>