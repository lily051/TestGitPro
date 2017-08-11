<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<style>
	.ui-jqgrid .ui-pager-control{
		position: fixed;
	}
	.ui-pager-control {
		position: fixed;
	    width: 1170px;
	}
	.showOrderInfo{
		height:450px;
		overflow: hidden;
	}
	#selectComOrder{
		_height:546px;
	}
	/* .ui-jqgrid-hdiv{
	  background: #7f879e !important;
	}
	 .ui-jqgrid-bdiv{
		overflow-x:hidden !important; 
	}
	#businessTable{
		width:1200px !important;
	}  */
	
</style>
<div class="complainInfo" id="complainInfoOnBefore">
	<div class="complainTitle">
		<span class="select_one">选择单号记录</span>
		<span class="confirmInfo_one">确认投诉信息</span>
		<span class="filtrateIcon" id="filtrate" data-mutex="close"></span>
	</div>
	<div id="selectComOrder">
		<div class="filterAlarmForm ieStyle clearfix" id="complainForm">
			<form class="form-horizontal clearfix" style="margin-top:13px;margin-bottom:5px">
			    <fieldset class="noLegend fieldset_40">
			    	<div class="control-group">
			    		<label class="control-label">流水号：</label>
			    		<div class="controls">
			    			<input type="text" class="auto-input" id="number" name="number">
			    		</div>
			    	</div>
			    	<div class="control-group">
			    		<label class="control-label">客户姓名：</label>
			    		<div class="controls">
			    			<input type="text" class="auto-input" id="clientName" name="clientName">
			    		</div>
			    	</div>
			    </fieldset>
			    <fieldset class="noLegend fieldset_40">
			    	<div class="control-group">
			    		<label class="control-label">客户证件号：</label>
			    		<div class="controls">
			    			<input type="text" class="auto-input" id="creditCode" name="creditCode">
			    		</div>
			    	</div>
			    </fieldset>
			    <div class="filterDIV filterBtn" style="width:100px;height:70px;margin-left:30px;float:left">
			        <span class="btn btn-submit" id="searchOrder">查询</span>
			        <span class="btn btn-cancel btnGap" id="clear">重置</span>
			    </div>
			</form>
		</div>
		<div class="showOrderInfo">
			<table id="businessTable"></table>
			<div id="businessTablePage"></div>
		</div>
	</div>
	<div id="doSureOrder" style="margin-top:20px">
		<form class="form-horizontal form-fixed formLabel140">
			<input type="hidden" name="businessorder.clientId" value="">		<!-- 业务单id -->
			<input type="hidden" name="businessorder.nproId" value="">		<!-- 流水号 -->
			<input type="hidden" name="businessorder.customerName" value="">	<!-- 客户姓名 -->
			<input type="hidden" name="businessorder.creditCode" value="">	<!-- 客户证件号 -->
			
			<input type="hidden" name="businessorder.status" value="">		<!-- 业务单类型 -->
			<input type="hidden" name="businessorder.nuserId" value="">		<!-- 理财经理id -->
			
			<fieldset class="noLegend">
				<div class="control-group">
					<label class="control-label">流水号：</label>
					<div class="controls">
						<span name="nproId"></span>
					</div>
				</div>
				<div class="control-group">
					<label class="control-label">客户姓名：</label>
					<div class="controls">
						<span name="clientName"></span>
					</div>
				</div>
				<div class="control-group">
					<label class="control-label">客户证件号：</label>
					<div class="controls">
						<span name="creditCode"></span>
					</div>
				</div>
				<div class="control-group">
					<label class="control-label"><em>*</em>投诉时间：</label>
					<div class="controls">
						<input type="text" class="auto-input" hikui="calendar" datefmt="yyyy-MM-dd HH:mm:ss" maxdate="%y-%M-%d %H:%m:%s" required="true" readonly="readonly" 
							isoverflown="true" data-icon="true" id="complainTime" name="businessorder.complainTime">
					</div>
				</div>
				<div class="control-group">
					<label class="control-label"><em>*</em>投诉信息：</label>
					<div class="controls">
						<textarea name="businessorder.compReason" id="complainInfo" maxLength="200" class="auto-input" vtype="common" isoverflown="true" required="true" data-icon="true" style="resize:none;height:100px;width:188px;"></textarea>
					</div>
				</div>
			</fieldset>
		</form>
	</div>
</div>
<script>
	$(function(){
		if(isIE6){
			$(".showOrderInfo").height(450);
		}
		FMS.dataBusiness.businessGrid(2);
		$("#searchOrder").on('click', function(event) {
			FMS.dataBusiness.getConditions();
			$("#businessTable").grid("dofiltler");
		});
		$("#clear").on('click', function(event) {
			$("input",'#complainForm').val('');
			FMS.dataBusiness.getConditions();
			$("#businessTable").grid("dofiltler");
		});
		$("#filtrate").on('click', function(event) {
			var $Mutex = $(this).data('mutex');
			var _slef = $(this);
			if($Mutex == 'close'){
				$('#complainForm').stop().animate({'height': 0});
				$('.showOrderInfo').stop().animate({'height': 546},function(){
					if(!isIE6){
						$("#businessTable").grid("resizeWidth");
					}else{
						$("#businessTable").grid("reload");
					}
					
				});
				_slef.data('mutex','open');
				_slef.addClass('filtrateIconOpen');
			}else if($Mutex == 'open'){
				$('#complainForm').stop().animate({'height': 96});
				$('.showOrderInfo').stop().animate({'height': 450},function(){
					if(!isIE6){
						$("#businessTable").grid("resizeWidth");
					}else{
						$("#businessTable").grid("reload");
					}
				});
				_slef.data('mutex','close');
				_slef.removeClass('filtrateIconOpen');
			}
			//$("#complainForm").animate({"height":0});
		});	
	});
</script>