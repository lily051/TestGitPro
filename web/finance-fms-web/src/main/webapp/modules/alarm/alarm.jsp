<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<head>
    <meta charset="utf-8">
    <%@ include file="/common/commons.jsp" %>
	<%@ include file="/common/commons-extend.jsp" %>
	<link  href="/modules/alarm/css/alarm.css" rel="stylesheet"></link>
	<link  href="/modules/video/css/video.css" rel="stylesheet"></link>
	<script type="text/javascript" src="/modules/alarm/js/alarm.js"></script>
</head>
<body>
	<div id="wrapper">
		<%@ include file="/common/commons-businessmenu.jsp"%>
		<div id="content" class="layout">
			<div class="container wrapper grid-s9m0 FMS_container" style="position:relative;">
				<div class="col-main">
					<div class="main-wrap wrapFixed">
						<div class="fullfit">
							<div class="filterFormBox ieStyle" style="margin-top:20px">
							    <form class="form-horizontal" id="filterForm">
							        <fieldset class="noLegend fieldset_25">
								    	<div class="control-group">
								    		<label class="control-label">开始时间：</label>
								    		<div class="controls conditions">
								    			<input type="text" class="auto-input" hikui="calendar" isShowClear="false" readonly="readonly" id="beginTime"
								    			datefmt="yyyy-MM-dd HH:mm:ss" name="beginTime" maxdate="#F{$dp.$N('endTime');}">
								    		</div>
								    	</div>
								    	<div class="control-group">
								    		<label class="control-label">处理状态：</label>
								    		<div class="controls conditions">
								    			<select name="" id="dealStatus">
								    				<option value="">全部</option>
								    				<option value="1">已处理</option>
								    				<option value="0">未处理</option>
								    			</select>
								    		</div>
								    	</div>
								    </fieldset>
								    <fieldset class="noLegend fieldset_25">
								    	<div class="control-group">
								    		<label class="control-label">结束时间：</label>
								    		<div class="controls conditions">
								    			<input type="text" class="auto-input" hikui="calendar" isShowClear="false" readonly="readonly" id="endTime"
								    			datefmt="yyyy-MM-dd HH:mm:ss" name="endTime" mindate="#F{$dp.$N('beginTime');}">
								    		</div>
								    	</div>
								    	<div class="control-group">
					        	    		<label class="control-label">报警类型：</label>
					        	    		<div class="controls conditions">
					        	    			<select name="" id="alarmType">
					        	    				<option value="">全部</option>
					        	    				<option value="61002">上传服务失败</option>
					        	    				<option value="62002">录像守卫服务失败</option>
					        	    				<option value="63002">系统巡检服务失败</option>
					        						<option value="64002" name="ipc">采集设备不在线</option>
					        						<option value="64003" name="ipc">采集设备时间和本地时间差很大</option>
					        						<option value="64004" name="ipc">采集设备视频信号丢失</option>
					        						<option value="64005" name="ipc">采集设备硬盘异常</option>
					        						<option value="64007" name="ipc nonCenterStorage">采集设备磁盘满</option>
					        						<option value="65001" name="ipc haveCenterStorage">CVR不在线</option>
					        						<option value="65003" name="ipc haveCenterStorage">CVR磁盘满</option>
					        						<option value="66001" name="ipc haveCenterStorage">Cloud不在线</option>
					        						<option value="66004" name="ipc haveCenterStorage">Cloud磁盘满</option>
					        						<option value="67001" name="usb">KMS不在线</option>
					        						<option value="67002" name="usb">KMS磁盘满</option>
					        						<option value="69001" name="usb">云存储-对象存储不在线</option>
					        						<option value="69002" name="usb">云存储-对象存储磁盘满</option>
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
							<div class="toolbar" style="padding:5px 0;">
								<a href="#"><span class="managerOpr oprAlarm" id="alarmDeal"><i>报警处理</i></span></a>
								<a href="#"><span class="managerOpr oprDelete"><i>删除</i></span></a>
								<label class="showChild"><span class="check"><input type="checkbox" class="showsub" checked="checked"></span><span class="showSubStyle">显示所有子机构</span></label>
							</div>
							<div class="datatableStyle" style="width:1010px;top:151px">
								<table id="dataTable"></table>
								<div id="dataTablePage"></div>
							</div>
						</div>
					</div>
				</div>
				<div class="col-sub">
					<jsp:include page="/common/common-tree.jsp" flush="true" >
						<jsp:param name="className" value="orgTree" />
					</jsp:include>
				</div>
			</div>
		</div>
	</div>
</body>
</html>


