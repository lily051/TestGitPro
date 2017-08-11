<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>

<html lang="zh">
<head>
    <meta charset="utf-8">
	<script type="text/javascript" src="/js/ocxVersion/initOCX.js"></script>
	<script type="text/javascript" src="/modules/device/channelMng/js/channelMng.js"></script>
</head>

<body>
	<div class="fullfit">
	<div class="toolbar">
		<a href="#"><span class="managerOpr oprDelete oprDeleteChannel"><i>删除</i></span></a>
		<label class="showChild"><span class="check"><input type="checkbox" class="showsub" checked="checked"></span><span class="showSubStyle">显示所有子机构</span></label>
		<span class="filtrateIcon" id="filtrate" data-mutex="close"></span>
	</div>
	<div class="filterFormBox ieStyle clearfix" style="margin:0;height:0;" id="filterFormBox">
	    <form class="form-horizontal clearfix" id="filterForm">
	    	<fieldset class="noLegend fieldset_25">
	    	    <div class="control-group">
	    	        <label class="control-label">设备名称：</label>
	    	        <div class="controls conditions">
	    	            <input type="text" name="" class='' value="" id="deviceName">
	    	        </div>
	    	    </div>
	    	    <div class="control-group">
	    	        <label class="control-label">通道名称：</label>
	    	        <div class="controls conditions">
	    	            <input type="text" name="" class='' value="" id="channelName">
	    	        </div>
	    	    </div>
	    	</fieldset>
	    	<fieldset class="noLegend fieldset_25" id = "searchUserIp">
    		    <div class="control-group" >
    		        <label class="control-label">理财电脑IP：</label>
    		        <div class="controls conditions">
    		            <input type="text" name="" class='' value="" id="userIp">
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
		<div id="channelDatatablePage"></div>
	</div>
</div>
</body>