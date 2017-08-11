<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>
<%@ page import="com.hikvision.finance.fms.common.util.Constants"%>
<link rel="stylesheet" href="/modules/analysis/css/analysis.css">
<style>
	.ui-jqgrid .ui-pager-control{
		position: fixed;
	}
	.ui-pager-control {
		position: fixed;
	    width: 1170px;
	}
	.tab-withoutBorder ul.tabs{
		margin-left:20px;
	}
	.tab-content{
		top:40px;
		bottom:0;
		overflow-x: auto; 
		overflow-y: auto; 
		_height:470px;
	}
	#selectComOrder{
		_height:470px;
		_position:relative;
	}
	ul.tabs {
	    height: 39px;
	}
	.tab-withoutBorder ul.tabs li.ui-state-active{
		height:37px;
	}
	.signTable{
		height:360px !important;
	}
	.ui-jqgrid-bdiv{
		_height:287px !important;
	}
</style>
<div class="complainInfo" id="complainInfoOnBefore">
	<div id="tab-withoutBorder2" class="tab-container tab-withoutBorder">
		<ul class="tabs">
		   <li><a href="#newOrder" data-click='0'>已录业务单</a></li>
		   <li><a href="#selectComOrder" data-click='0'>选择历史业务单</a></li>
		   <span class="filtrateIcon hidden" id="filtrate" data-mutex="close" style="margin-right:0"></span>
		</ul>
		<div class="tab-content" id="tab-content">
		<div id="newOrder">
			<form class="form-horizontal form-fixed formLabel140" id="newOrderForm">
				<div class="height20"></div>
				<fieldset class="noLegend">
					<div class="control-group">
						<label class="control-label">业务单类型：</label>
						<div class="controls">
							<span>
								<s:if test="businessorder.status==0">签约单</s:if>
								<s:if test="businessorder.status==1">投诉单</s:if>
							</span>
						</div>
					</div>
					<div class="control-group">
						<label class="control-label">业务单流水号：</label>
						<div class="controls">
							<input type="text" required="true" class="auto-input" data-icon="true" id="businessId" disabled
							isoverflown="true" name="businessorder.nproId" value="${businessorder.nproId }" readonly="readonly">
						</div>
					</div>
					<div class="control-group">
						<label class="control-label"><em>*</em>交易网点：</label>
						<div class="controls">
							<input type="text" class="auto-input" disabled
								isoverflown="true" data-icon="true" name="" value="${region.strName}">
						</div>
					</div>
					<div class="control-group <s:if test="orderEnableInfo.inputRegionCodeEnable==0">hidden</s:if>">
						<label class="control-label">交易网点代码：</label>
						<div class="controls">
							<input type="text" class="auto-input" disabled 
								isoverflown="true" data-icon="true" name="" value="${region.strCode}">
						</div>
					</div>
					<div class="control-group">
						<label class="control-label"><em>*</em>产品名称：</label>
						<div class="controls" style="position:relative;">
							<input type="text" required="true" class="auto-input" data-icon="true" isoverflown="true" disabled 
							 name="businessorder.productName" value="${businessorder.productName }" readonly="readonly">
							
						</div>
					</div>
					<div class="control-group">
						<label class="control-label">产品编码：</label>
						<div class="controls">
							<input type="text" class="auto-input" readonly="readonly" disabled
								isoverflown="true" data-icon="true" name="businessorder.productCode" value="${businessorder.productCode }">
						</div>
					</div>
					<div class="control-group <s:if test="orderEnableInfo.strGradeEnable==0">hidden</s:if>">
						<label class="control-label">产品发行机构：</label>
						<div class="controls">
							<input type="text" class="auto-input" vtype="common" maxLength="20" disabled
								isoverflown="true" data-icon="true" name="businessorder.strGrade" value="${businessorder.strGrade }">					
						</div>
					</div>
					<div class="control-group <s:if test="orderEnableInfo.strRisklevelEnable==0">hidden</s:if>">
						<label class="control-label">产品风险等级：</label>
						<div class="controls">
							<select name="businessorder.strRisklevel" disabled>
								<option value="1" <s:if test="businessorder.strRisklevel ==1">selected</s:if>><%=Constants.RiskLevel.RISKLEVEL_ONE%></option>
								<option value="2" <s:if test="businessorder.strRisklevel ==2">selected</s:if>><%=Constants.RiskLevel.RISKLEVEL_TWO%></option>
								<option value="3" <s:if test="businessorder.strRisklevel ==3">selected</s:if>><%=Constants.RiskLevel.RISKLEVEL_THREE%></option>
								<option value="4" <s:if test="businessorder.strRisklevel ==4">selected</s:if>><%=Constants.RiskLevel.RISKLEVEL_FOUR%></option>
								<option value="5" <s:if test="businessorder.strRisklevel ==5">selected</s:if>><%=Constants.RiskLevel.RISKLEVEL_FIVE%></option>
								<option value="6" <s:if test="businessorder.strRisklevel ==6">selected</s:if>><%=Constants.RiskLevel.RISKLEVEL_SIX%></option>
								<option value="0" <s:if test="businessorder.strRisklevel ==0">selected</s:if>><%=Constants.RiskLevel.RISKLEVEL_NONE%></option>
							</select>
						</div>
					</div>
					<div class="control-group <s:if test="orderEnableInfo.strVideoEndDateEnable==0">hidden</s:if>">
						<label class="control-label">产品到期日：</label>
						<div class="controls">
							<input type="text" class="auto-input" disabled  
								isoverflown="true" data-icon="true" name="businessorder.strEndDate" value="${businessorder.strEndDate }">
						</div>
					</div>
					<div class="control-group <s:if test="orderEnableInfo.guaranteeYearsEnable==0">hidden</s:if>">
						<label class="control-label">产品保障期限：</label>
						<div class="controls">
							<input type="text" class="auto-input" vtype="num" maxLength="3" disabled 
								isoverflown="true" data-icon="true" name="businessorder.guaranteeYears" value="${businessorder.guaranteeYears }">
								<s:if test='businessorder.guaranteeYears !=null'>
									<s:if test="businessorder.guaranteeType==0">年</s:if>
									<s:if test="businessorder.guaranteeType==1">月</s:if>
									<s:if test="businessorder.guaranteeType==2">日</s:if>
								</s:if>
						</div>
					</div>
					<div class="control-group <s:if test="orderEnableInfo.productSaveMonthEnable==0">hidden</s:if>">
						<label class="control-label">产品兑付后保存期限：</label>
						<div class="controls">
							<input type="text" class="auto-input" vtype="num" maxLength="3" disabled
								isoverflown="true" data-icon="true" name="businessorder.productSaveMonth" value="${businessorder.productSaveMonth }">	
							<s:if test='businessorder.productSaveMonth !=null'>
									<s:if test="businessorder.productSaveType==0">年</s:if>
									<s:if test="businessorder.productSaveType==1">月</s:if>
									<s:if test="businessorder.productSaveType==2">日</s:if>
							</s:if>
						</div>
					</div>
					<div class="control-group">
						<label class="control-label"><em>*</em>客户姓名：</label>
						<div class="controls">
							<input type="text" class="auto-input" vtype="common" required="true" maxLength="20" disabled 
								isoverflown="true" data-icon="true" name="businessorder.customerName" value="${businessorder.customerName }">
						</div>
					</div>
					<div class="control-group">
						<label class="control-label"><em>*</em>客户证件类型：</label>
						<div class="controls">
							<select name="businessorder.creditId" id="certType" disabled>
								
							</select>
						</div>
					</div>
					<div class="control-group">
						<label class="control-label"><em>*</em>客户证件号：</label>
						<div class="controls">
							<input type="text" class="auto-input" required="true" maxLength="30" disabled 
								isoverflown="true" data-icon="true" name="businessorder.creditCode" value="${businessorder.creditCode }">
						</div>
					</div>
					<div class="control-group <s:if test="orderEnableInfo.strCreditPhoneEnable==0">hidden</s:if>">
						<label class="control-label">客户电话：</label>
						<div class="controls">
							<input type="text" class="auto-input" vtype="num" maxLength="11" disabled 
								isoverflown="true" data-icon="true" name="businessorder.strCreditPhone" value="${businessorder.strCreditPhone }">
						</div>
					</div>
					<div class="control-group <s:if test="orderEnableInfo.strBankNoEnable==0">hidden</s:if>">
						<label class="control-label">客户银行卡号：</label>
						<div class="controls">
							<input type="text" class="auto-input" vtype="num" maxLength="20" disabled 
								isoverflown="true" data-icon="true" name="businessorder.strBankNo" value="${businessorder.strBankNo }">
						</div>
					</div>
					<div class="control-group <s:if test="orderEnableInfo.strBuyMoneyEnable==0">hidden</s:if>">
						<label class="control-label">购买金额：</label>
						<div class="controls">
							<input type="text" class="auto-input" vtype="num" maxLength="50" disabled 
								isoverflown="true" data-icon="true" name="businessorder.strBuyMoney" value="${businessorder.strBuyMoney }">
						</div>
					</div>
					<div class="control-group <s:if test="orderEnableInfo.proUserNoEnable==0">hidden</s:if>">
						<label class="control-label">业务员工号：</label>
						<div class="controls">
							<span class="notEdit">${user.userNo}</span>
						</div>
					</div>
					<div class="control-group">
						<label class="control-label">办理时间：</label>
						<div class="controls">
							<input type="text" class="auto-input" hikui="calendar"  readonly="readonly" disabled 
								isoverflown="true" data-icon="true" name="businessorder.businessTime" 
								value="<fmt:formatDate value="${businessorder.businessTime}" pattern="yyyy-MM-dd HH:mm:ss"/>">
						</div>
					</div>
					<div class="control-group <s:if test="businessorder.complainTime==null">hidden</s:if>">
						<label class="control-label">投诉时间：</label>
							<div class="controls">
							<input type="text" class="notEdit" hikui="calendar" readonly="readonly" id="complainTime" disabled
								datefmt="yyyy-MM-dd HH:mm:ss" isoverflown="true" data-icon="true" name="businessorder.complainTime" 
								value="<fmt:formatDate value="${businessorder.complainTime}" pattern="yyyy-MM-dd HH:mm:ss"/>">
						</div>
					</div>
					<div class="control-group <s:if test="businessorder.compReason==null">hidden</s:if>">
						<label class="control-label">投诉信息：</label>
						<div class="controls" style="height:auto">
							<textarea name="businessorder.compReason" id="" type="text" not-validator-paste=false maxLength="2000" vtype="common" disabled isoverflown="true" data-icon="true" style="resize:none;height:100px;width:188px;">${businessorder.compReason}</textarea>
						</div>
					</div>
					<div class="control-group <s:if test="orderEnableInfo.strRemarksEnable==0">hidden</s:if>">
						<label class="control-label">备注：</label>
						<div class="controls">
							<textarea name="businessorder.strRemarks" id="" maxLength="200" vtype="common" disabled isoverflown="true" data-icon="true" style="resize:none;height:100px;width:188px;">${businessorder.strRemarks}</textarea>
						</div>
					</div>
				</fieldset>
			</form>
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
			<div class="showOrderInfo signTable">
				<table id="businessTable"></table>
				<div id="businessTablePage" style="width:1000px"></div>
			</div>
		</div>
	</div>
</div>	
</div>
<script>
	$(function(){
		$("#tab-withoutBorder2").tabs({
			activate: function(e, obj) {
				var selector = obj.newPanel.selector;
				var context = obj.newTab.context;
				if(selector == "#selectComOrder"){
					$('.tab-content').css("overflow-y",'hidden');
					if(isIE6){
						$(".showOrderInfo").height(360);
					}
					FMS.dataBusiness.businessGrid(1);
					if($('body').width()<1200){
						$(".ui-pager-control").width(760);
					}
					//$("#selectComOrder,#filtrate").removeClass('hidden');
				}else{
					//$("#selectComOrder,#filtrate").addClass('hidden');
					$('.tab-content').css("overflow-y",'auto');
				}
			}
		});
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
				$('.showOrderInfo').stop().animate({'height': 461},function(){
					$("#businessTable").grid("reload");
				});
				_slef.data('mutex','open');
				_slef.addClass('filtrateIconOpen');
			}else if($Mutex == 'open'){
				$('#complainForm').stop().animate({'height': 96});
				$('.showOrderInfo').stop().animate({'height': 365},function(){
					$("#businessTable").grid("reload");
				});
				_slef.data('mutex','close');
				_slef.removeClass('filtrateIconOpen');
			}
			//$("#complainForm").animate({"height":0});
		});	
		FMS.dataBusiness.getCertType('#certType','${businessorder.creditId}');
	});
</script>