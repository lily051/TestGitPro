<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>

<div class="iScroll" id="iScroll">
	<form class="form-horizontal form-fixed formLabel140">
		<div class="msg-b msg-b-attention" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;">
		    <div class="msg-cnt">
		        <span>红色<em>*</em>为必填项</span>
		    </div>
		</div>
		<fieldset class="noLegend">
			<div class="control-group">
				<label class="control-label">所属类型：</label>
				<input type="hidden" value="${producttypeInfo.pid}" name="producttypeInfo.pid"></input>
				<div class="controls"><span class="notEdit" title="${producttypeInfo.nparentName }">${producttypeInfo.nparentName }</span></div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>产品类型名称：</label>
				<div class="controls">
					<input type="text" style="display:none">
					<input type="text" vtype="common" data-icon="true" required="true" class="auto-input" maxLength="50" isoverflown="true" 
						id="proTypeName" name="producttypeInfo.strName">
				</div>
			</div>
		</fieldset>
	</form>
</div>