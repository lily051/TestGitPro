<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<head>
    <meta charset="utf-8">
    <%@ include file="/common/commons.jsp" %>
	<%@ include file="/common/commons-extend.jsp" %>
	<link  href="/web/webDevice/datacollect/css/datacollect.css" rel="stylesheet" />
	<link  href="/modules/alarm/css/alarm.css" rel="stylesheet" />
	<link  href="/web/webDevice/css/module.css" rel="stylesheet" />
	<script src="/js/ocxVersion/initOCX.js"></script>
	<script type="text/javascript" src="/web/webDevice/business/js/busiRelate.js"></script>
	<script type="text/javascript" src="/web/webDevice/datacollect/js/dataBusiness.js"></script>
	<style>
		.ui-pager-control {
		    width: 1220px;
		}
	</style>
</head>
<body>
	<div id="wrapper">
		<%@ include file="/common/commons-collect-top.jsp"%>
		<div id="content" class="layout" style="top:73px">
			<div class="container wrapper FMS_container" style="position:relative;">
				<div class="main-content">
					<div class="filterAlarmForm ieStyle">
						<form class="form-horizontal" id="filterForm" style="margin-top:15px;">
						    <fieldset class="noLegend fieldset260">
						    	<div class="control-group">
						    		<label class="control-label">开始时间：</label>
						    		<div class="controls">
						    			<input type="text" class="auto-input" hikui="calendar" isShowClear="false" readonly="readonly" id="startTime"
						    			datefmt="yyyy-MM-dd HH:mm:ss" name="startTime" maxdate="#F{$dp.$N('endTime');}">
						    			<!-- <label for="">-</label> -->
						    		</div>
						    	</div>
						    	<div class="control-group">
						    		<label class="control-label">产品名称：</label>
						    		<div class="controls">
						    			<input type="text" class="auto-input"  id="productNameForTable"><!-- 已修改id，防止和编辑业务单中的冲突 -->
						    		</div>
						    	</div>
						    </fieldset>
						    
						    <fieldset class="noLegend fieldset260">
						    	<div class="control-group">
						    		<label class="control-label">结束时间：</label>
						    		<div class="controls">
						    			<input type="text" class="auto-input" hikui="calendar" isShowClear="false" readonly="readonly" id="endTime"
						    			datefmt="yyyy-MM-dd HH:mm:ss" name="endTime" mindate="#F{$dp.$N('startTime');}">
						    		</div>
						    	</div>
						    	<div class="control-group">
						    		<label class="control-label">录像名称：</label>
						    		<div class="controls">
						    			<input type="text" class="auto-input" name="" value="" id="recordName">
						    		</div>
						    	</div>
						    </fieldset>
						    <fieldset class="noLegend fieldset260">
						    	<div class="control-group">
						    		<label class="control-label">客户姓名：</label>
						    		<div class="controls">
						    			<input type="text" class="auto-input" id="customName" >
						    		</div>
						    	</div>
						    	
						    </fieldset>
						    <div class="filterDIV filterBtn">
						        <span class="btn btn-submit blockBtn" id="search">查询</span>
						        <span class="btn btn-cancel blockBtn btnGap" id="clear">重置</span>
						    </div>
						</form>
					</div>
					<div class="datatableStyle" style="top:110px;">
						<table id="datatable"></table>
						<div id="dataTablePage"></div>
					</div>
				</div> 
			</div>
		</div>
	</div>
</body>
</html>


