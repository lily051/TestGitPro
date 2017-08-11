<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>
<%@ page import="com.hikvision.finance.fms.common.util.Constants"%>
<style>
.ui-state-highlight{
	display: none!important;
}
</style>
<div class="iScroll">
	<form class="form-horizontal form-fixed formLabel140">
		<fieldset class="noLegend" style="padding-top: 20px;">
			<div class="control-group">
				<label class="control-label">业务单流水号：</label>
				<div class="controls" style="width: 205px;">
					<span id="" class="show_span">${businessorder.nproId }</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">交易机构：</label>
				<div class="controls" style="width: 205px;">
					<span id="" class="show_span">${region.strName }</span>
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.inputRegionCodeEnable==0">hidden</s:if>">
				<label class="control-label">交易机构代码：</label>
				<div class="controls" style="width: 205px;">
					<span id="regionNo_span" class="show_span">${region.strCode}</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">产品名称：</label>
				<div class="controls" style="width:205px;">
					<span id="" class="show_span">${businessorder.productName }</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">产品编码：</label>
				<div class="controls" style="width:205px;">
					<span id="" class="show_span">${businessorder.productCode }</span>
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.strGradeEnable==0">hidden</s:if>">
				<label class="control-label">产品发行机构：</label>
				<div class="controls" style="width:205px;">
					<span id="strGrade_span" class="show_span">${businessorder.strGrade }</span>
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.strRisklevelEnable==0">hidden</s:if>">
				<label class="control-label">产品风险等级：</label>
				<div class="controls">
					<span id="strRisklevel_span" class="show_span">
						<s:if test="businessorder.strRisklevel ==1"><%=Constants.RiskLevel.RISKLEVEL_ONE%></s:if>
						<s:if test="businessorder.strRisklevel ==2"><%=Constants.RiskLevel.RISKLEVEL_TWO%></s:if>
						<s:if test="businessorder.strRisklevel ==3"><%=Constants.RiskLevel.RISKLEVEL_THREE%></s:if>
						<s:if test="businessorder.strRisklevel ==4"><%=Constants.RiskLevel.RISKLEVEL_FOUR%></s:if>
						<s:if test="businessorder.strRisklevel ==5"><%=Constants.RiskLevel.RISKLEVEL_FIVE%></s:if>
						<s:if test="businessorder.strRisklevel ==6"><%=Constants.RiskLevel.RISKLEVEL_SIX%></s:if>
						<s:if test="businessorder.strRisklevel ==0"><%=Constants.RiskLevel.RISKLEVEL_NONE%></s:if>
					</span>
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.strVideoEndDateEnable==0">hidden</s:if>">
				<label class="control-label">产品到期日：</label>
				<div class="controls">
					<span id="strEndDate_span" class="show_span">${businessorder.strEndDate }</span>
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.guaranteeYearsEnable==0">hidden</s:if>">
				<label class="control-label">产品保障期限：</label>
				<div class="controls">
					<input type="hidden" class="auto-input" readonly="readonly" UNSELECTABLE='on' id="guaranteeYears"
						 name="businessorder.guaranteeYears" value="${businessorder.guaranteeYears }">
					<input type="hidden" name="businessorder.guaranteeType" value="${businessorder.guaranteeType }">
					<span id="guaranteeYears_span" class="show_span">${businessorder.guaranteeYears }
					<s:if test='businessorder.guaranteeYears !=null'>
						<s:if test='businessorder.guaranteeYears !=null'>
						<s:if test="businessorder.guaranteeType==0">年</s:if>
						<s:if test="businessorder.guaranteeType==1">月</s:if>
						<s:if test="businessorder.guaranteeType==2">日</s:if>
						</s:if>
					</s:if>
					</span>
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.productSaveMonthEnable==0">hidden</s:if>">
				<label class="control-label">产品兑付后保存期限：</label>
				<div class="controls">
					<input type="hidden" class="auto-input" id="saveYears"  readonly="readonly" UNSELECTABLE='on' 
						 name="businessorder.productSaveMonth" value="${businessorder.productSaveMonth }">
					<input type="hidden" name="businessorder.productSaveType" value="${businessorder.productSaveType }">
					<span id="saveYears_span" class="show_span">${businessorder.productSaveMonth }
					<s:if test='businessorder.productSaveMonth !=null'>
						<s:if test='businessorder.productSaveMonth !=null'>
						<s:if test="businessorder.productSaveType==0">年</s:if>
						<s:if test="businessorder.productSaveType==1">月</s:if>
						<s:if test="businessorder.productSaveType==2">日</s:if>
						</s:if>
					</s:if>
					</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">客户姓名：</label>
				<div class="controls" style="width: 205px;">
					<span id="" class="show_span">${businessorder.customerName }</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">客户证件类型：</label>
				<div class="controls" style="width: 205px;">
					<span id="creditCodeE" class="show_span">${businessorder.creditCode }</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">客户证件号：</label>
				<div class="controls" style="width: 205px;">
					<span id="" class="show_span">${businessorder.creditId }</span>
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.strCreditPhoneEnable==0">hidden</s:if>">
				<label class="control-label">客户电话：</label>
				<div class="controls">
					<span id="" class="show_span">${businessorder.strCreditPhone }</span>
<!-- 					<input type="text" class="auto-input" vtype="mobile"  readonly="true" -->
<%-- 						isoverflown="true" data-icon="true" name="businessorder.strCreditPhone" value="${businessorder.strCreditPhone }"> --%>
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.strBankNoEnable==0">hidden</s:if>">
				<label class="control-label">客户银行卡号：</label>
				<div class="controls" style="width: 205px;">
				<span id="" class="show_span">${businessorder.strBankNo }</span>
<!-- 					<input type="text" class="auto-input" vtype="num" maxLength="32" readonly="true" -->
<%-- 						isoverflown="true" data-icon="true" name="businessorder.strBankNo" value="${businessorder.strBankNo }"> --%>
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.strBuyMoneyEnable==0">hidden</s:if>">
				<label class="control-label">购买金额：</label>
				<div class="controls">
				<span id="" class="show_span">${businessorder.strBuyMoney }</span>
<!-- 					<input type="text" class="auto-input" vtype="num" readonly="true" minValue="0" maxValue="2147483648" maxLength="10" self-focus-info="请输入0至2147483648的有效数字"  -->
<%-- 						isoverflown="true" data-icon="true" name="businessorder.strBuyMoney" value="${businessorder.strBuyMoney }"> --%>
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.proUserNoEnable==0">hidden</s:if>">
				<label class="control-label">业务员工号：</label>
				<div class="controls" style="width: 205px;">
					<span class="notEdit" title="${user.userNo}">${user.userNo}</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">办理时间：</label>
				<div class="controls">
					<span id="" class="show_span"><fmt:formatDate value="${businessorder.businessTime}" pattern="yyyy-MM-dd HH:mm:ss"/></span>
				</div>
			</div>
			<div class="control-group <s:if test="businessorder.complainTime==null">hidden</s:if>">
				<label class="control-label">投诉时间：</label>
					<div class="controls">
					<span class="notEdit" title="${businessorder.complainTime }"><fmt:formatDate value="${businessorder.complainTime}" pattern="yyyy-MM-dd HH:mm:ss"/></span>
				</div>
			</div>
			<div class="control-group <s:if test="businessorder.compReason==null">hidden</s:if>">
				<label class="control-label">投诉信息：</label>
				<div class="controls" style="width: 205px;">
					<span id="" class="notEdit" title = "${businessorder.compReason}">${businessorder.compReason}</span>
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.strRemarksEnable==0">hidden</s:if>">
				<label class="control-label">备注：</label>
				<div class="controls" style="width: 205px;">
					<span id="" class="notEdit" title = "${businessorder.strRemarks}">${businessorder.strRemarks}</span>
				</div>
			</div>
		</fieldset>
		
	</form>
</div>
<script>
	$(function(){
		var creditCode = "${businessorder.creditCode}";
		$.ajax({
			url:'/webdevice/contact/getCardList.action',
			type:'post',
			dataType:'json',
			success:function(data){
				for (var i = 0; i < data.data.cardList.length; i++) {
					if(creditCode == data.data.cardList[i].id){
						$("#creditCodeE").text(data.data.cardList[i].strName);
						return;
					}
				}
			}
		});
	});
</script>