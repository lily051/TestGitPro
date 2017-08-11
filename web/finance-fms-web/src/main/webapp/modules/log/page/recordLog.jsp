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
				    			datefmt="yyyy-MM-dd HH:mm:ss" name="startTime" maxdate="#F{$dp.$N('endTime');}">
				    		</div>
				    	</div>
				    	<div class="control-group">
				    		<label class="control-label">录像类型：</label>
				    		<div class="controls conditions">
				    			<select name="" id="recordType">
				    				<option value="">全部</option>
				    				<option value="1">签约录像</option>
				    				<option value="2">投诉录像</option>
				    			</select>
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
				    		<label class="control-label">业务单号：</label>
				    		<div class="controls conditions">
				    			<input type="text" class="auto-input" name="" id="nproId">
				    		</div>
				    	</div>
				    </fieldset>
				    <fieldset class="noLegend fieldset_25">
				    	<div class="control-group">
				    		<label class="control-label">录像名称：</label>
				    		<div class="controls conditions">
				    			<input type="text" class="auto-input" name="" id="recordName">
				    		</div>
				    	</div>
				    	<div class="control-group">
				    		<label class="control-label">操作用户：</label>
				    		<div class="controls conditions">
				    			<input type="text" class="auto-input" name="" id="checkUserName">
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
				<label class="showChild"><span class="check"><input type="checkbox" class="showsub" checked="checked"></span><span class="showSubStyle">显示所有子机构</span></label>
			</div>
			<div class="datatableStyle" style="top:153px">
				<table id="dataTable"></table>
				<div id="dataTablePage"></div>
			</div>
		</div>
	</div>
	<script>
		$(function(){
			FMS.logSearch.globalSetting();
			if($("#orgTree").children().length>0){
				FMS.logSearch.dataGrid(1);
			}else{
				FMS.logSearch.initOrgTree();
			}
			FMS.logSearch.bindEvent();
		});
	</script>
</body>
</html>


