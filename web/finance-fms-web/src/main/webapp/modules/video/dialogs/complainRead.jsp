<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>

<div>
	<form class="form-horizontal form-fixed formLabel140">
		<div class="msg-b msg-b-attention" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;">
		    <div class="msg-cnt">
		        <span>红色<em>*</em>为必填项</span>
		    </div>
		</div>
		<fieldset class="noLegend">
			<div class="control-group">
				<label class="control-label"><em>*</em>用户：</label>
				<div class="controls">
					<!-- <input type="text" required="true" class="auto-input" vtype="num" maxLength="20"  data-icon="true" 
					isoverflown="true" name="" value=""> -->
					<span></span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>用户电话：</label>
				<div class="controls">
					<!-- <input type="text" required="true" vtype="common" class="auto-input" maxLength="20"  data-icon="true" 
					isoverflown="true" name="" value=""> -->
					<span></span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>备注：</label>
				<div class="controls">
					<textarea name="" id="" class="auto-input" maxLength="200" data-icon="true" vtype="common"
					isoverflown="true" style="resize:none;height:80px;width:188px;" type="text" not-validator-paste=false></textarea>
				</div>
			</div>
		</fieldset>
	</form>
</div>