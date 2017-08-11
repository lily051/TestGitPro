<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<style>
.state{
	margin-left:0;
}
</style>
<div>
	<form class="form-horizontal form-fixed product-certf">
		<div class="msg-b msg-b-attention msg-b-4" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;">
		    <div class="msg-cnt">
		        <span>红色<em>*</em>为必填项</span>
		    </div>
		</div>
		<fieldset class="noLegend halfField">
		    <div class="control-group">
		        <label class="control-label"><em>*</em>证书名称：</label>
		        <div class="controls" style="margin-left:100px;_margin-left:0;">
		            <input type="text" vtype="common" required="true" class="auto-input" maxLength="50" data-icon="true"
		                isoverflown="true"  id="strName" style="width:140px;_width:130px;"  value="${crtificateInfo.strName}">
		        </div>
		        <input type="hidden"  id="crtificateInfoId" name="crtificateInfo.id" value="${crtificateInfo.id}">
		    </div>
		</fieldset>
		<fieldset class="noLegend halfField">
		    <div class="control-group">
		        <label class="control-label"><em>*</em>证书编码：</label>
		        <div class="controls" style="margin-left:100px;_margin-left:0;">
		            <input type="text" vtype="common" required="true" class="auto-input" maxLength="50" data-icon="true"
		                isoverflown="true" id="strCode" style="width:140px;_width:130px;" value="${crtificateInfo.strCode}">
		        </div>
		    </div>
		</fieldset>
		<fieldset class="noLegend pull-left allField">
		    <div class="control-group">
		        <label class="control-label">产品权限：</label>
		        <div class="controls" style="margin-left:100px;_margin-left:0;">
		           <div class="treeview product-certTree">
		               <ul id="productCertTree" class="ztree"></ul>
		           </div>
		        </div>
		    </div>
		</fieldset>
		
	</form>
</div>

<script>
	$(function(){
		FMS.product.initProductTree('edit');
	});
</script>