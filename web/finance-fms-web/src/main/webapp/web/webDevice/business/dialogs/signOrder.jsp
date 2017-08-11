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
		<div class="msg-b msg-b-attention" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;padding-left:55px;">
		    <div class="msg-cnt">
		        <span>红色<em>*</em>为必填项</span>
		    </div>
		</div>
		<fieldset class="noLegend">
			<div class="control-group">
				<label class="control-label">业务单流水号：</label>
				<div class="controls">
					<input type="hidden" id="nproId" name="businessorder.nproId" value="${businessorder.nproId }">
					<span id="nproId_span" class="show_span">${businessorder.nproId }</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>交易网点：</label>
				<div class="controls" style="position:relative">
					<input type="hidden" name="businessorder.inputRegionId" value="${region.id}" id="inputRegionId">
					<input type="text" class="auto-input" required="true" id="regionName" data-icon="true" value="${region.strName}" readonly="readonly" data-clicknum="0" >
					<div class="input_dropdown" id="regionInfo">
						<div class="header-search" style="width:210px;margin:5px">
							<span class="treeDelete" style="right:25px;top:7px;"></span>
						    <span class="icon iconSearch" id="regionSearch" style="top:7px;right:5px;"></span>
						    <input id="dropdown_regionName" name="keyWord" type="text" placeholder="搜索" class="searchInput form-control treeSearch" value="" style="width:150px!important">
						</div>
						<div class="treeview" style="height:255px;">
						    <ul id="regionTree" class="ztree"></ul>
						</div>
					</div>
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.inputRegionCodeEnable==0">hidden</s:if>">
				<label class="control-label">交易网点代码：</label>
				<div class="controls" style="width: 205px;">
					<input type="hidden" class="auto-input" id="regionNo" value="${region.strCode}" readonly="readonly">
					<span id="regionNo_span" class="show_span">${region.strCode}</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>产品名称：</label>
				<div class="controls" style="position:relative;">
					<input type="text" required="true" id="productName" class="auto-input" readonly="readonly" UNSELECTABLE='on'
					isoverflown="true" data-icon="true" name="businessorder.productName" value="${businessorder.productName }"> 
					<div class="input_dropdown" id="productInfo">
						<div class="header-search" style="width:210px;margin:5px">
							<span class="treeDelete" style="right:25px;top:7px;"></span>
						    <span class="icon iconSearch" id="orgTreeSearch" style="top:7px;right:5px"></span>
						    <input id="dropdown_productName" name="keyWord" type="text" placeholder="搜索" class="searchInput form-control treeSearch" value="" style="width:150px!important">
						</div>
						<div class="treeview" style="height:255px;">
						    <ul id="productTree" class="ztree"></ul>
						</div>
					</div>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">产品编码：</label>
				<div class="controls">
					<!-- <input type="text" class="auto-input" id="strCode"  readonly="readonly"
						 name="businessorder.productCode" value="${businessorder.productCode }"> -->
					<!-- <span id="strCode_span" class="show_span">${businessorder.productCode }</span> -->
					<input type="hidden" value="${businessorder.productId }" name="businessorder.productId" id="productId">
					<input type="hidden" value="${businessorder.productTypeId }" name="businessorder.productTypeId" id="productTypeId">
					<input type="hidden" value="${businessorder.productCode }" name="businessorder.productCode" id="productCode">
					<select name="" id="productCode_combobox"></select>
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.strGradeEnable==0">hidden</s:if>">
				<label class="control-label">产品发行机构：</label>
				<div class="controls" style="width: 205px;">
					<input type="hidden" id="strGrade" readonly="readonly" 
						 name="businessorder.strGrade" value="${businessorder.strGrade }">	
					<span id="strGrade_span" class="show_span">${businessorder.strGrade }</span>
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.strRisklevelEnable==0">hidden</s:if>">
				<label class="control-label">产品风险等级：</label>
				<div class="controls">
					<input type="hidden" name="businessorder.strRisklevel" id="strRisklevel_input" value="${businessorder.strRisklevel}">
					<!-- <select name="businessorder.strRisklevel" id="strRisklevel" disabled>
						<option value="2" <s:if test="businessorder.strRisklevel ==2">selected</s:if> >高</option>
						<option value="1" <s:if test="businessorder.strRisklevel ==1">selected</s:if> >中</option>
						<option value="0" <s:if test="businessorder.strRisklevel ==0">selected</s:if> >低</option>
					</select> -->
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
					<input type="hidden" class="auto-input"  readonly="readonly" UNSELECTABLE='on' id="strEndDate"
						 name="businessorder.strEndDate" value="${businessorder.strEndDate }">
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
				<label class="control-label"><em>*</em>客户姓名：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="common" required="true" maxLength="32" id="customerName"
						isoverflown="true" data-icon="true" name="businessorder.customerName" value="${businessorder.customerName }">
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>客户证件类型：</label>
				<div class="controls">
					<select name="businessorder.creditId" id="certType" >
						
					</select>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>客户证件号：</label>
				<div class="controls">
					<input type="text" class="auto-input" required="true" vtype="regex" maxLength="30" id="creditCode" regex="^[0-9a-zA-Z]+$"
						isoverflown="true" data-icon="true" regexinfo="提示信息：只能输入数字或字母" name="businessorder.creditCode" value="${businessorder.creditCode }">
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.strCreditPhoneEnable==0">hidden</s:if>">
				<label class="control-label">客户电话：</label>
				<div class="controls">
					<input type="text" class="auto-input" maxLength="16" vtype="num"
						isoverflown="true" data-icon="true" name="businessorder.strCreditPhone" value="${businessorder.strCreditPhone }">
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.strBankNoEnable==0">hidden</s:if>">
				<label class="control-label">客户银行卡号：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="num" maxLength="32"  
						isoverflown="true" data-icon="true" name="businessorder.strBankNo" value="${businessorder.strBankNo }">
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.strBuyMoneyEnable==0">hidden</s:if>">
				<label class="control-label">购买金额：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="num" minValue="0" maxValue="2147483648" maxLength="10" self-focus-info="请输入0至2147483648的有效数字" 
						isoverflown="true" data-icon="true" name="businessorder.strBuyMoney" value="${businessorder.strBuyMoney }">
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.proUserNoEnable==0">hidden</s:if>">
				<label class="control-label">业务员工号：</label>
				<div class="controls" style="width: 205px;">
					<span class="notEdit" title="${user.userNo}">${user.userNo}</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>办理时间：</label>
					<div class="controls">
					<input type="text" class="notEdit" hikui="calendar" readonly="readonly" id="businessTime" 
						datefmt="yyyy-MM-dd HH:mm:ss" isoverflown="true" data-icon="true" name="businessorder.businessTime" 
						value="<fmt:formatDate value="${businessorder.businessTime}" pattern="yyyy-MM-dd HH:mm:ss"/>">
				</div>
			</div>
			<div class="control-group <s:if test="businessorder.complainTime==null">hidden</s:if>">
				<label class="control-label">投诉时间：</label>
					<div class="controls">
					<input type="text" class="notEdit" hikui="calendar" readonly="readonly" id="complainTime" 
						datefmt="yyyy-MM-dd HH:mm:ss" isoverflown="true" data-icon="true" name="businessorder.complainTime" 
						value="<fmt:formatDate value="${businessorder.complainTime}" pattern="yyyy-MM-dd HH:mm:ss"/>">
				</div>
			</div>
			<div class="control-group <s:if test="businessorder.compReason==null">hidden</s:if>">
				<label class="control-label">投诉信息：</label>
				<div class="controls" style="height:auto">
					<textarea name="businessorder.compReason" id="" type="text" not-validator-paste=false maxLength="2000" vtype="common" isoverflown="true" data-icon="true" style="resize:none;height:100px;width:188px;">${businessorder.compReason}</textarea>
				</div>
			</div>
			<div class="control-group <s:if test="orderEnableInfo.strRemarksEnable==0">hidden</s:if>">
				<label class="control-label">备注：</label>
				<div class="controls">
					<textarea name="businessorder.strRemarks" id="" type="text" not-validator-paste=false maxLength="2000" vtype="common" isoverflown="true" data-icon="true" style="resize:none;height:100px;width:188px;">${businessorder.strRemarks}</textarea>
				</div>
			</div>
		</fieldset>
		
	</form>
</div>
<script>
	$(function(){
		var productCode = $("#productCode").val();
		
		$.ajax({
			url:'/webdevice/contact/getPermitProductCodeList.action',//TODO 
			type:'post',
			dataType:'json',
			async:false,
			success:function(data){
				if(data&&data.success){
					var productCode = data.data.permitProductCodeList;
					$("#productCode_combobox").combobox({
					    source: productCode,
					    toolTipTime : 2E3,
					    autocompleteFilter : function(array, term){    //自定义过滤函数
					        var matcher = new RegExp($.ui.autocomplete.escapeRegex(term), "i");
					        return $.grep(array, function(value) {
					            return matcher.test(value.label || value.value || value);
					        });
					    },
					    select:function(event,obj){
					    	var item = obj.item;
					    	if(item.tagName){
					    		$.ajax({
					    			url:'/webdevice/contact/getBasicProductbyId.action',//TODO 
					    			type:'post',
					    			dataType:'json',
					    			data:{
					    				'id':item.value
					    			},
					    			success:function(data){
					    				if(data&&data.success){
					    					var proInfo = data.data.basicProductInfo;
					    					proInfo.productId = proInfo.id;
					    					for(var key in proInfo){
					    						var keyInfo = proInfo[key],
					    							keyInfo = keyInfo==null?'':keyInfo;
					    						if(key == "strRisklevel"){
					    							if(keyInfo==1){
					    								$("#strRisklevel_span").text(Constants.RiskLevel.RISKLEVEL_ONE);
					    							}else if(keyInfo==2){
					    								$("#strRisklevel_span").text(Constants.RiskLevel.RISKLEVEL_TWO);
					    							}else if(keyInfo==3){
					    								$("#strRisklevel_span").text(Constants.RiskLevel.RISKLEVEL_THREE);
					    							}else if(keyInfo==4){
					    								$("#strRisklevel_span").text(Constants.RiskLevel.RISKLEVEL_FOUR);
					    							}else if(keyInfo==5){
					    								$("#strRisklevel_span").text(Constants.RiskLevel.RISKLEVEL_FIVE);
					    							}else if(keyInfo==6){
					    								$("#strRisklevel_span").text(Constants.RiskLevel.RISKLEVEL_SIX);
					    							}else if(keyInfo==0){
					    								$("#strRisklevel_span").text(Constants.RiskLevel.RISKLEVEL_NONE);
					    							}
					    							$("#strRisklevel_input").val(keyInfo);
					    							continue;
					    						}
					    						if(key == "strName"){
					    							$("#productName").val(keyInfo);
					    							continue;
					    						}
					    						if(key == "pid"){
					    							$("#productTypeId").val(keyInfo);
					    							continue;
					    						}
					    						if(key == "strCode"){
					    							$(".custom-combobox input,#productCode","#signOrderDialog").val(keyInfo);
					    							continue;
					    						}
					    						$("#"+key,"#signOrderDialog").val(keyInfo);
					    						
					    						if (key=="saveType" || key=="guaranteeType") {
					    							var text={0:'年',1:'月',2:'日'};
					    							var type = text[keyInfo];
					    						}
					    						if(key == "saveYears"&&keyInfo!=""){
					    							$("#"+key+"_span","#signOrderDialog").text(keyInfo+type);
					    							continue;
					    						}
					    						
					    						if(key == "guaranteeYears"&&keyInfo!=""){
					    							$("#"+key+"_span","#signOrderDialog").text(keyInfo+type);
					    							continue;
					    						}
					    						$("#"+key+"_span","#signOrderDialog").text(keyInfo);
					    					}
					    				}
					    			},
					    			error:function(xhr){
					    			
					    			}
					    		});
					    	}
					    }
					});
				}else{

				}
			},
			error:function(xhr){
				
			}
		});
        $(".custom-combobox input").val(productCode);
		FMS.dataBusiness.getCertType('#certType',"${businessorder.creditId}");
		$(".pop_overlay,.popWrap").on('click', function(event) {
			var eve = event.target || event.srcElement;
			if($(eve).closest('.input_dropdown').length){
				return;
			}else{
				$(".input_dropdown").hide();
				if(isIE6){
					$("#certType").show();
					$(".controls").each(function(index, el) {
						$(el).css("z-index","1");
					});
				}
			}
		});
		$("#productName","#signOrderDialog").click(function(event) {
			FMS.globalFun.stopPropagation(event);
			if(isIE6){
				$("#certType").hide();
				$(".controls").each(function(index, el) {
					$(el).css("z-index","-1");
				});
				$(this).parent(".controls").css("z-index","1");
			}
			$("#productInfo").show();
			$(".ui-autocomplete").hide();
		});
		$("#regionName").click(function(event) {
			FMS.globalFun.stopPropagation(event);
			$("#productInfo,.ui-autocomplete").hide();
			var _this = $(this);
			if(isIE6){
				$("#certType").hide();
				$(".controls").each(function(index, el) {
					$(el).css("z-index","-1");
				});
				_this.parent(".controls").css("z-index","1");
			}
			if(_this.data("clicknum")=="0"){
				_this.data("clicknum","1");
				FMS.dataBusiness.initRegionTree();
			}else{
				$("#regionInfo").show();
			}
			
		});
		$(".input_dropdown .searchInput").keydown(function(event) {
			var key = event.keyCode;
			if (key == 13) {
				$(this).prev().click();
				return false;
			}
		});
	});	
</script>