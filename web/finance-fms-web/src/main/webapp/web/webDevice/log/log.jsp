<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<head>
    <meta charset="utf-8">
    <%@ include file="/common/commons.jsp" %>
	<%@ include file="/common/commons-extend.jsp" %>
	<link  href="/modules/alarm/css/alarm.css" rel="stylesheet" />
	<link  href="/web/webDevice/datacollect/css/datacollect.css" rel="stylesheet" />
	<link  href="/web/webDevice/css/module.css" rel="stylesheet" />
	<script type="text/javascript" src="/web/webDevice/log/js/log.js"></script>
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
					<div class="filterAlarmForm ieStyle" style="height:auto;">
						<form class="form-horizontal clearfix" id="filterForm" style="margin-top:15px;">
						    <fieldset class="noLegend fieldset400">
						    	<div class="control-group">
						    		<label class="control-label">操作时间：</label>
						    		<div class="controls">
						    			<input type="text" class="auto-input" hikui="calendar" isShowClear="false" readonly="readonly" id="startTime"
						    			datefmt="yyyy-MM-dd HH:mm:ss" name="startTime" maxdate="#F{$dp.$N('endTime');}">
						    			<label for="">-</label>
						    			<input type="text" class="auto-input" hikui="calendar" isShowClear="false" readonly="readonly" id="endTime"
						    			datefmt="yyyy-MM-dd HH:mm:ss" name="endTime" mindate="#F{$dp.$N('startTime');}">
						    		</div>
						    	</div>
						    </fieldset>
						    <fieldset class="noLegend fieldset260">
						    	<div class="control-group">
						    		<label class="control-label">操作类型：</label>
						    		<div class="controls">
						    			<select name="ntype" id="ntype">
						    				<option value="">全部</option>
						    				<option value="1">开始录像</option>
						    				<option value="2">结束录像</option>
						    				<option value="3">视频补录</option>
						    				<option value="5">编辑录像名</option>
						    				<option value="6">删除录像</option>
						    				<option value="7">处理异常录像</option>
						    				<option value="11">录单</option>
						    				<option value="12">关联业务单</option>
						    				<option value="13">编辑业务单</option>
						    				<option value="4">添加离线录像</option>
						    			</select>
						    		</div>
						    	</div>
						    </fieldset>
						    <div class="filterDIV filterBtn" style="width:auto;height:45px;">
						        <span class="btn btn-submit" id="search">查询</span>
						        <span class="btn btn-cancel btnGap" id="clear" style="margin-top:0">重置</span>
						    </div>
						</form>
					</div>
					<div class="datatableStyle" style="top:75px;">
						<table id="dataTable"></table>
						<div id="dataTablePage"></div>
					</div>
				</div>
			</div>
		</div>
	</div>
</body>
</html>


