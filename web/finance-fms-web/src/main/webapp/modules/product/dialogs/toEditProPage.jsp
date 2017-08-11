<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ taglib prefix='fmt' uri="http://java.sun.com/jsp/jstl/fmt" %>
<%@ page import="com.hikvision.finance.fms.common.util.Constants"%>
<div class="iScroll" id="iScroll">
	<form class="form-horizontal form-fixed formLabel140">
		<div class="msg-b msg-b-attention" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;">
		    <div class="msg-cnt">
		        <span>红色<em>*</em>为必填项</span>
		    </div>
		</div>
		<div class="legendTitle"><span class="titleLine"></span>产品信息</div>
		<fieldset class="noLegend">
			<div class="control-group">
				<label class="control-label">产品类型：</label>
				<input type="hidden" name="productInfo.id" value="${productInfo.id}"></input>
				<div class="controls"><span class="notEdit" title="${productInfo.productTypeName}">${productInfo.productTypeName}</span></div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>产品名称：</label>
				<div class="controls">
					<input type="text" vtype="common" data-icon="true" required="true" class="auto-input" maxLength="50" isoverflown="true" 
						id="productName" name="productInfo.strName" value="${productInfo.strName }">
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>产品编码：</label>
				<div class="controls">
					<input type="text" class="auto-input" id="productCode" vtype="common" required="true" maxLength="30"
						isoverflown="true" data-icon="true" name="productInfo.strCode" value="${productInfo.strCode }">
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>发行机构：</label>
				<div class="controls">
					<input type="text" class="auto-input" id="issueAgency" vtype="common" required="true" maxLength="50"
						isoverflown="true" data-icon="true" name="productInfo.strGrade" value="${productInfo.strGrade }">					
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>风险等级：</label>
				<div class="controls">
					<select name="productInfo.strRisklevel" id="riskLevel">
						<option value="1" <s:if test="productInfo.strRisklevel==1">selected="selected"</s:if>><%=Constants.RiskLevel.RISKLEVEL_ONE%></option>
						<option value="2" <s:if test="productInfo.strRisklevel==2">selected="selected"</s:if>><%=Constants.RiskLevel.RISKLEVEL_TWO%></option>
						<option value="3" <s:if test="productInfo.strRisklevel==3">selected="selected"</s:if>><%=Constants.RiskLevel.RISKLEVEL_THREE%></option>
						<option value="4" <s:if test="productInfo.strRisklevel==4">selected="selected"</s:if>><%=Constants.RiskLevel.RISKLEVEL_FOUR%></option>
						<option value="5" <s:if test="productInfo.strRisklevel==5">selected="selected"</s:if>><%=Constants.RiskLevel.RISKLEVEL_FIVE%></option>
						<option value="6" <s:if test="productInfo.strRisklevel==6">selected="selected"</s:if>><%=Constants.RiskLevel.RISKLEVEL_SIX%></option>
						<option value="0" <s:if test="productInfo.strRisklevel==0">selected="selected"</s:if>><%=Constants.RiskLevel.RISKLEVEL_NONE%></option>
					</select>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>产品期限：</label>
				<div class="controls"  id="productLeftTime">
					<label><span class="check"><input type="radio" name="deadline" value="0" <s:if  test="productInfo.strEndDate!=null">checked="checked"</s:if>   ></span><span id="dueSpan">到期日</span></label>
					<input type="text" hikui="calendar"  style="width:135px !important;_width:102px !important;" 
						id="dueDate" readonly="readonly" name="productInfo.strEndDate" value=" <fmt:formatDate value="${productInfo.strEndDate}" pattern="yyyy-MM-dd"/>">
					<label><span class="check"><input type="radio" name="deadline" value="1" <s:if  test="productInfo.guaranteeYears!=null">checked="checked"</s:if>  ></span><span id="guaranteeSpan">保障期限</span></label>
					<input type="text" vtype="num"  class="auto-input" minValue="1" maxValue="365" maxLength="3" isoverflown="true" style="width:78px;_width:92px;" data-icon="true" id="guaranteeYear" name="productInfo.guaranteeYears" value="${productInfo.guaranteeYears }" self-focus-info="请输入1至365的有效数字">
					<select id="guaranteeType" name="productInfo.guaranteeType">
						<option value="0" <s:if test="productInfo.guaranteeType==0">selected="selected"</s:if>>年</option>
						<option value="1" <s:if test="productInfo.guaranteeType==1">selected="selected"</s:if>>月</option>
						<option value="2" <s:if test="productInfo.guaranteeType==2">selected="selected"</s:if>>日</option>
					</select>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">产品兑付后保存期限：</label>
				<div class="controls unit">
					<input type="text" class="auto-input" id="saveTime" vtype="num" minValue="1" maxValue="365" maxLength="3" 
						 name="productInfo.saveYears" isoverflown="true" data-icon="true" value="${productInfo.saveYears}" self-focus-info="请输入1至365的有效数字"
						 style="width:157px;_width:92px" />
					<select id="saveType" name="productInfo.saveType">
						<s:if test="#productInfo.saveType==0"></s:if>
					    <option value="0" <s:if test="productInfo.saveType==0">selected="selected"</s:if>>年</option>
					    <option value="1" <s:if test="productInfo.saveType==1">selected="selected"</s:if>>月</option>
					    <option value="2" <s:if test="productInfo.saveType==2">selected="selected"</s:if>>日</option>
					</select>
				</div>
			</div>
			<div class="legendTitle">
				<span class="titleLine"></span>话术模板
			</div>
			<div class="control-group">
				<div class="controls" style="margin-left:0">
					<textarea style="width: 350px" name="productInfo.wordTemplate" id="wordTemplate" type="text" not-validator-paste=false 
						maxLength="2500" vtype="common" isoverflown="true" data-icon="true" class="auto-input wordTemplate">${productInfo.wordTemplate }</textarea>
				</div>
			</div>
		</fieldset>
		
	</form>
</div>

<script type="text/javascript">
	$(function(){
		if($('input[name="deadline"]:checked').val()==0){
			$("#guaranteeYear").prop("disabled",true);
			$("#guaranteeSpan").addClass('span_disabled');
		}else{
			$("#dueDate").prop("disabled",true);
			$("#dueSpan").addClass('span_disabled');
		}
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
		});
		$("#wordTemplate").focus(function(){
			$("#wordTemplate").siblings(".info.tip").css({"left":"30px"});
		});
	});
</script>