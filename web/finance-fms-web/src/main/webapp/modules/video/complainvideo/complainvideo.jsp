<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<div class="fullfit">
	<div class="filterFormBox ieStyle">
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
		    		<label class="control-label">客户证件号：</label>
		    		<div class="controls conditions">
		    			<input type="text" class="auto-input" name="" id="creditCode">
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
    	    		<label class="control-label">产品编码：</label>
    	    		<div class="controls conditions">
    	    			<input type="text" class="auto-input" name="" id="productCode">
    	    		</div>
	    	    </div>
		    </fieldset>
		    <fieldset class="noLegend fieldset_25">
		    	<div class="control-group">
					<label class="control-label">客户姓名：</label>
					<div class="controls conditions">
						<input type="text" class="auto-input" name="" id="name">
					</div>
				</div>
				<div class="control-group">
					<label class="control-label">产品名称：</label>
					<div class="controls conditions">
						<input type="text" class="auto-input" name="" id="productName">
					</div>
				</div>
		    </fieldset>
	        <div class="filterDIV filterBtn">
	            <span class="btn btn-submit blockBtn" id="search">查询</span>
	            <span class="btn btn-cancel blockBtn btnGap" id="clear">重置</span>
	        </div>
	        <div class="hideCondition" id="hideCondition">
	            <fieldset class="noLegend fieldset_25">
	    	    	<div class="control-group">
	    	    		<label class="control-label">业务单号：</label>
	    	    		<div class="controls conditions">
	    	    			<input type="text" class="auto-input" name="" id="nproId">
	    	    		</div>
	    	    	</div>
	    	    </fieldset>
	    	    <fieldset class="noLegend fieldset_25">
					<div class="control-group">
						<label class="control-label">业务员姓名：</label>
						<div class="controls conditions">
							<input type="text" class="auto-input" name="" id="proName">
						</div>
					</div>
				</fieldset>
	    	    <fieldset class="noLegend fieldset_25">
					<div class="control-group">
		    		<label class="control-label">业务员工号：</label>
		    		<div class="controls conditions">
		    			<input type="text" class="auto-input" name="" id="proUserNo">
		    		</div>
		    	</div>
				</fieldset>
    	    </div>
	    </form>
	    <div class="form-horizontal">
	    	<div class="control-group">
	    		<label class="control-label"><a class="openMore" data-toogle="down">展开更多</a></label>
	    	</div>
	    </div>
	</div>
	<div class="toolbar" style="padding:5px 0">
		<a href="#"><span class="managerOpr oprComplaint"><i>投诉处理</i></span></a>
		<a href="#"><span class="managerOpr oprDelay"><i>延时处理</i></span></a>
		<a href="#"><span class="managerOpr oprExport"><i>导出</i></span></a>
		<label class="showChild"><span class="check"><input type="checkbox" class="showsub" checked="checked"></span><span class="showSubStyle">显示所有子机构</span></label>
	</div>
	<div class="datatableStyle" style="top:171px">
		<table id="videoDatatable"></table>
		<div id="videoDataTablePage"></div>
	</div>
</div>
<script>
	$(function(){
		if(isIE6){
			var parentH = $('#content').height();
			$(".mainload").height(parentH-70);
			$(".datatableStyle").height(parentH-240);
		}
		FMS.video.globalSetting();
		FMS.video.bindEvent();
		FMS.video.videoGrid();
		FinanceUtil.initFormUI("#filterForm");
	});
</script>