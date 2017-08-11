<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>
<%@ page import="com.hikvision.finance.fms.common.util.Constants"%>
<style>
	.formLabel120 .control-label{
		font-weight: bold;
	}
	.controls span.notEdit{
		width:100%;
	}
	#newOrderForm .controls{
		height:28px;
	}
	.halfField{
		width:330px;
	}
</style>
<div class="iScroll">
	<form class="form-horizontal form-fixed formLabel140 clearfix" id="newOrderForm">
		<div class="height20"></div>
		<fieldset class="noLegend halfField">
			<div class="control-group">
				<label class="control-label">业务单类型：</label>
				<div class="controls">
					<span id="signvideoSpan" style="display:inline;">签约单
						<s:if test="saleInfo.status==0"></s:if>
						<s:if test="saleInfo.status==1">（已投诉）</s:if>
					</span>
					<span id="complainvideoSpan" style="display:none;">
						<s:if test="saleInfo.status==0">签约单</s:if>
						<s:if test="saleInfo.status==1">投诉单</s:if>
					</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">业务单流水号：</label>
				<div class="controls">
					<span class="notEdit" title="${saleInfo.nproId }">${saleInfo.nproId }</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">交易网点：</label>
				<div class="controls">
					<span class="notEdit" title="${saleInfo.inputRegionName}">${saleInfo.inputRegionName}</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">交易网点代码：</label>
				<div class="controls">
					<span class="notEdit" title="${saleInfo.inputRegionCode }">${saleInfo.inputRegionCode}</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">产品名称：</label>
				<div class="controls" style="position:relative;">
					<span class="notEdit" title="${saleInfo.productName }">${saleInfo.productName }</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">产品编码：</label>
				<div class="controls">
					<span class="notEdit" title="${saleInfo.productCode }">${saleInfo.productCode }</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">产品发行机构：</label>
				<div class="controls">
					<span class="notEdit" title="${saleInfo.strGrade }">${saleInfo.strGrade }</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">产品风险等级：</label>
				<div class="controls">
					<span>
						<s:if test="saleInfo.strRisklevel ==1"><%=Constants.RiskLevel.RISKLEVEL_ONE%></s:if>
						<s:if test="saleInfo.strRisklevel ==2"><%=Constants.RiskLevel.RISKLEVEL_TWO%></s:if>
						<s:if test="saleInfo.strRisklevel ==3"><%=Constants.RiskLevel.RISKLEVEL_THREE%></s:if>
						<s:if test="saleInfo.strRisklevel ==4"><%=Constants.RiskLevel.RISKLEVEL_FOUR%></s:if>
						<s:if test="saleInfo.strRisklevel ==5"><%=Constants.RiskLevel.RISKLEVEL_FIVE%></s:if>
						<s:if test="saleInfo.strRisklevel ==6"><%=Constants.RiskLevel.RISKLEVEL_SIX%></s:if>
						<s:if test="saleInfo.strRisklevel ==0"><%=Constants.RiskLevel.RISKLEVEL_NONE%></s:if>
					</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">产品兑付后保存期限：</label>
				<div class="controls">
					<span class="notEdit" title="${saleInfo.productSaveMonth }">${saleInfo.productSaveMonth }
						<s:if test='saleInfo.productSaveMonth !=null'>
							<s:if test="saleInfo.productSaveType==0">年</s:if>
							<s:if test="saleInfo.productSaveType==1">月</s:if>
							<s:if test="saleInfo.productSaveType==2">日</s:if>
						</s:if>
					</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">产品保障期限：</label>
				<div class="controls">
					<span class="notEdit" title="${saleInfo.guaranteeYears }">${saleInfo.guaranteeYears }
					<s:if test='saleInfo.guaranteeYears !=null'>
						<s:if test="saleInfo.guaranteeType==0">年</s:if>
						<s:if test="saleInfo.guaranteeType==1">月</s:if>
						<s:if test="saleInfo.guaranteeType==2">日</s:if>
					</s:if>
					</span>
				</div>
			</div>
			
			<div class="control-group <s:if test="saleInfo.compTime==null">hidden</s:if>">
				<label class="control-label">投诉时间：</label>
					<div class="controls">
					<span class="notEdit" title="${saleInfo.compTime }"><fmt:formatDate value="${saleInfo.compTime}" pattern="yyyy-MM-dd HH:mm:ss"/></span>
				</div>
			</div>
			<div class="control-group <s:if test="saleInfo.compReason==null">hidden</s:if>">
				<label class="control-label">投诉信息：</label>
				<div class="controls" style="height:auto">
					<textarea name="saleInfo.compReason" readonly="readonly" style="resize:none;height:100px;width:475px;border:0;box-shadow:none;">${saleInfo.compReason}</textarea>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">备注：</label>
				<div class="controls" style="height:auto">
					<textarea name="businessorder.strRemarks" readonly="readonly" style="resize:none;height:100px;width:475px;border:0;box-shadow:none;">${saleInfo.strRemarks}</textarea>
				</div>
			</div>
		</fieldset>
		<fieldset class="noLegend halfField">
			<div class="control-group">
				<label class="control-label">客户姓名：</label>
				<div class="controls">
					<span class="notEdit" title="${saleInfo.name }">${saleInfo.name }</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">客户证件类型：</label>
				<div class="controls">
					<span class="notEdit" title="${saleInfo.creditCodeName }">${saleInfo.creditCodeName}</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">客户证件号：</label>
				<div class="controls">
					<span class="notEdit" title="${saleInfo.creditCode }">${saleInfo.creditCode }</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">客户电话：</label>
				<div class="controls">
					<span class="notEdit" title="${saleInfo.strCreditPhone }">${saleInfo.strCreditPhone }</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">客户银行卡号：</label>
				<div class="controls">
					<span class="notEdit" title="${saleInfo.strBankNo }">${saleInfo.strBankNo }</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">购买金额：</label>
				<div class="controls">
					<span class="notEdit" title="${saleInfo.strBuyMoney }">${saleInfo.strBuyMoney }<s:if test='saleInfo.strBuyMoney !=""'>元</s:if></span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">办理时间：</label>
				<div class="controls">
					<span class="notEdit" title="${saleInfo.businessTime }"><fmt:formatDate value="${saleInfo.businessTime}" pattern="yyyy-MM-dd HH:mm:ss"/></span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">业务单审批状态：</label>
				<div class="controls">
					<span class="notEdit">
					<s:if test='saleInfo.lockClient == 1'>审批通过</s:if>
					<s:elseif test='saleInfo.lockClient == 2'>审批退回</s:elseif>
					<s:else>待审批</s:else></span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">业务员工号：</label>
				<div class="controls">
					<span class="notEdit" title="${saleInfo.proUserNo }">${saleInfo.proUserNo }</span>
				</div>
			</div>
			
			<div class="control-group">
				<label class="control-label">产品到期日：</label>
				<div class="controls">
					<span class="notEdit" title="${saleInfo.strEndDate }"><fmt:formatDate value='${saleInfo.strEndDate }' pattern='yyyy-MM-dd' /></span>
				</div>
			</div>
		</fieldset>
	</form>
</div>