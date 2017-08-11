<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ page import="com.hikvision.finance.fms.common.util.Constants"%>
<div class="iScroll">
	<form class="form-horizontal form-fixed formLabel140" id="addProDialog">
		<div class="msg-b msg-b-attention" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;">
		    <div class="msg-cnt">
		        <span>红色<em>*</em>为必填项</span>
		    </div>
		</div>
		<div class="legendTitle"><span class="titleLine"></span>产品信息</div>
		<fieldset class="noLegend">
			<div class="control-group">
				<label class="control-label">产品类型：</label>
				<input type="hidden" name="productInfo.pid" value="${productInfo.pid}"></input>
				<div class="controls"><span class="notEdit" title="${productInfo.productTypeName}">${productInfo.productTypeName}</span></div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>产品名称：</label>
				<div class="controls">
					<input type="text" required="true" vtype="common" class="auto-input" maxLength="50"  data-icon="true" 
					isoverflown="true" id="productName" name="productInfo.strName">
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>产品编码：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="common" id="productCode" required="true" maxLength="30"
						isoverflown="true" data-icon="true" name="productInfo.strCode">
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>发行机构：</label>
				<div class="controls">
					<input type="text" class="auto-input" id="issueAgency" vtype="common" required="true" maxLength="50"
						isoverflown="true" data-icon="true" name="productInfo.strGrade">					
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>风险等级：</label>
				<div class="controls">
					<select name="productInfo.strRisklevel" id="riskLevel">
						<option value="1"><%=Constants.RiskLevel.RISKLEVEL_ONE%></option>
						<option value="2"><%=Constants.RiskLevel.RISKLEVEL_TWO%></option>
						<option value="3"><%=Constants.RiskLevel.RISKLEVEL_THREE%></option>
						<option value="4"><%=Constants.RiskLevel.RISKLEVEL_FOUR%></option>
						<option value="5"><%=Constants.RiskLevel.RISKLEVEL_FIVE%></option>
						<option value="6"><%=Constants.RiskLevel.RISKLEVEL_SIX%></option>
						<option value="0"><%=Constants.RiskLevel.RISKLEVEL_NONE%></option>
					</select>
				</div>
			</div>
			<div class="control-group" id="productLeftTime">
				<label class="control-label"><em>*</em>产品期限：</label>
				<div class="controls">
					<label><span class="check"><input type="radio" name="deadline" checked value="0"></span><span id="dueSpan">到期日</span></label>
					<input type="text"  hikui="calendar"  style="width:135px !important;_width:102px !important;" 
						name="productInfo.strEndDate" id="dueDate" readonly="readonly">	
					<label><span class="check"><input type="radio" name="deadline" value="1"></span><span id="guaranteeSpan" class="span_disabled">保障期限</span></label>
					<input type="text"  class="auto-input" disabled vtype="num" minValue="1" maxValue="365" maxLength="3" isoverflown="true" data-icon="true" style="width:78px;_width:92px" id="guaranteeYear" name="productInfo.guaranteeYears" self-focus-info="请输入1至365的有效数字">
					<select id="guaranteeType" name="productInfo.guaranteeType">
						<option value="0">年</option>
						<option value="1" selected="selected">月</option>
						<option value="2">日</option>
					</select>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">产品兑付后保存期限：</label>
				<div class="controls unit">
					<input type="text" class="auto-input" id="saveTime" vtype="num" minValue="1" maxValue="365" maxLength="3" 
						isoverflown="true" data-icon="true" name="productInfo.saveYears" self-focus-info="请输入1至365的有效数字"
						style="width:157px;_width:92px" />
					<select id="saveType" name="productInfo.saveType">
					    <option value="0">年</option>
					    <option value="1" selected="selected">月</option>
					    <option value="2">日</option>
					</select>
				</div>
			</div>
			<div class="legendTitle">
				<span class="titleLine"></span>话术模板
			</div>
			<div class="control-group">
			    <div class="controls" style="margin-left:0">
			        <textarea style="width: 350px" name="productInfo.wordTemplate" id="wordTemplate" vtype="common" type="text" not-validator-paste=false 
					maxLength="2500" isoverflown="true"  class="auto-input wordTemplate"></textarea>
			    </div>
				
			</div>
		</fieldset>
		
	</form>
</div>

<script type="text/javascript">
	$(function(){
		$(".check input").on("change",function(){
			var value = $(this).val();
			if(value == 0){
				$("#productLeftTime").find("div.success").removeClass("success").end().find("div.error").removeClass("error").end().find("input").removeClass("error");
				$("#guaranteeYear").prop("disabled",true);
				$("#guaranteeSpan").addClass('span_disabled');
				$("#dueDate").prop("disabled",false);
				$("#dueSpan").removeClass('span_disabled');
			}else{
				$("#guaranteeYear").prop("disabled",false);
				$("#guaranteeSpan").removeClass('span_disabled');
				$("#dueDate").prop("disabled",true);
				$("#dueSpan").addClass('span_disabled');
			}
			$("#dueDate,#guaranteeYear").val('');
		});
		$("#wordTemplate").focus(function(){
			$("#wordTemplate").siblings(".info.tip").css({"left":"30px"});
		});
	});
</script>