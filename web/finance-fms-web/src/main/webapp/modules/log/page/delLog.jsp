<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<body>
	<div class="main-wrap wrapFixed">
		<div class="fullfit">
			<div class="filterFormBox ieStyle" style="margin-top:20px">
			    <form class="form-horizontal" id="filterForm">
			        <fieldset class="noLegend fieldset_25">
				    	<div class="control-group">
				    		<label class="control-label">开始时间：</label>
				    		<div class="controls conditions">
				    			<input type="text" class="auto-input" hikui="calendar" isShowClear="false" readonly="readonly" id="startTime"
				    			datefmt="yyyy-MM-dd HH:mm:ss" name="startTime"maxdate="#F{$dp.$N('endTime');}">
				    		</div>
				    	</div>
				    	<div class="control-group">
				    		<label class="control-label">产品名称：</label>
				    		<div class="controls conditions">
				    			<input type="text" class="auto-input" name="" id="productName">
				    		</div>
				    	</div>
				    </fieldset>
				    <fieldset class="noLegend fieldset_25">
				    	<div class="control-group">
				    		<label class="control-label">结束时间：</label>
				    		<div class="controls conditions">
				    			<input type="text" class="auto-input" hikui="calendar" isShowClear="false" readonly="readonly" id="endTime"
				    			datefmt="yyyy-MM-dd HH:mm:ss" name="endTime" mindate="#F{$dp.$N('startTime');}">
				    		</div>
				    	</div>
				    	<div class="control-group">
				    		<label class="control-label">客户证件号：</label>
				    		<div class="controls conditions">
				    			<input type="text" class="auto-input" name="" id="creditId">
				    		</div>
				    	</div>
				    </fieldset>
				    <fieldset class="noLegend fieldset_25">
				    	<div class="control-group">
				    		<label class="control-label">业务单号：</label>
				    		<div class="controls conditions">
				    			<input type="text" class="auto-input" name="" id="nproId">
				    		</div>
				    	</div>
				    	<div class="control-group">
				    		<label class="control-label">客户姓名：</label>
				    		<div class="controls conditions">
				    			<input type="text" class="auto-input" name="" id="name">
				    		</div>
				    	</div>
				    </fieldset>
			        <div class="filterDIV filterBtn">
			            <span class="btn btn-submit blockBtn" id="search">查询</span>
			            <span class="btn btn-cancel blockBtn btnGap" id="clear">重置</span>
			        </div>
			    </form>
			</div>
			<div class="toolbar" style="padding:5px 0">
				<a href="#"><span class="managerOpr oprExport"><i>导出</i></span></a>
			</div>
			<div class="datatableStyle" style="top:153px">
				<table id="dataTable"></table>
				<div id="dataTablePage"></div>
			</div>
		</div>
	</div>
	<script>
		$(function(){
			//FMS.logSearch.destroyTree();
			FMS.logSearch.globalSetting();
			FMS.logSearch.bindEvent();
			FMS.logSearch.dataGrid(2);
		});
	</script>
</body>
</html>


