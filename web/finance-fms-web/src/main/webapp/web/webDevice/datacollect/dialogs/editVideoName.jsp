<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<style>
	.height30{
		height:30px;
	}
</style>
<div>
	<div class="height30"></div>
	<form class="form-horizontal form-fixed formLabel140">
		<fieldset class="noLegend">
			<div class="control-group">
				<label class="control-label"><em>*</em>录像名称：</label>
				<div class="controls">
					<input type="text" style="display:none;">
					<input type="hidden" name="records.id" id="recordId">
					<input type="text" required="true" class="auto-input" vtype="common" maxLength="50"  data-icon="true" 
					isoverflown="true" name="records.strRecordName" value="" id="recordName">
				</div>
			</div>
		</fieldset>
	</form>
</div>