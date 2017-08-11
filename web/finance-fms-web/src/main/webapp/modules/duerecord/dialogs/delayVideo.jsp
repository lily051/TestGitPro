<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>

<div>
	<div class="height20"></div>
	<form class="form-horizontal form-fixed formLabel140">
		<fieldset class="noLegend">
			<div class="control-group">
				<label class="control-label"><em>*</em>延时天数：</label>
				<div class="controls">
					  <input type="text" class="auto-input" value="" required="true" vtype="num"
                                    minValue="1" maxValue="30" maxLength="2" self-focus-info="请输入1至30的有效数字" isOverflown="true" data-icon="true" name="dueRecordInfo.delayDays"/>
				</div>
			</div>
		</fieldset>		
	</form>
</div>