<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>

<div>
	<form class="form-horizontal form-fixed formLabel120">
		<div class="msg-b msg-b-attention" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;">
		    <div class="msg-cnt">
		        <span>红色<em>*</em>为必填项</span>
		    </div>
		</div>
		<fieldset class="noLegend">
			<div class="control-group">
				<label class="control-label"><em>*</em>证件类型：</label>
				<div class="controls">
					<input type="text" vtype="common" data-icon="true" required="true" class="auto-input" maxLength="30" isoverflown="true" 
							id="collectName" name="paramdictionaryInfo.strName">
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>证件编号：</label>
				<div class="controls">
					<input type="text" vtype="num" data-icon="true" required="true" class="auto-input" maxLength="30" isoverflown="true" 
						id="collectIP" style="ime-mode: disabled;" name="paramdictionaryInfo.strCode">
				</div>
			</div>
		</fieldset>
	</form>
</div>