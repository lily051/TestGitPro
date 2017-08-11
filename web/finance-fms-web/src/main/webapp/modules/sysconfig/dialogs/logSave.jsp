<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<div id="logSave">
	<form class="form-horizontal form-fixed  form-center">
		 <fieldset class="noLegend">
			<div class="control-group">
				<label class="control-label"><em>*</em>配置日志：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="num" minValue="10" maxValue="365" maxLength="3" name="logParamInfo.configLog" value="${logParamInfo.configLog }" required="true" data-icon="true" self-focus-info="请输入10至365的有效数字">
					<span class="right">天</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>系统日志：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="num" minValue="10" maxValue="365" maxLength="3" name="logParamInfo.systemLog" value="${logParamInfo.systemLog}" required="true" data-icon="true" self-focus-info="请输入10至365的有效数字">
					<span class="right">天</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>删除日志：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="num" minValue="10" maxValue="365" maxLength="3" name="logParamInfo.ndeleteLog" value="${logParamInfo.ndeleteLog}" required="true" data-icon="true" self-focus-info="请输入10至365的有效数字">
					<span class="right">天</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>回放日志：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="num" minValue="10" maxValue="365" maxLength="3" name="logParamInfo.reviewLog" value="${logParamInfo.reviewLog}" required="true" data-icon="true" self-focus-info="请输入10至365的有效数字">
					<span class="right">天</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>操作日志：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="num" minValue="10" maxValue="365" maxLength="3" name="logParamInfo.operaLog" value="${logParamInfo.operaLog}" required="true" data-icon="true" self-focus-info="请输入10至365的有效数字">
					<span class="right">天</span>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>报警日志：</label>
				<div class="controls">
					<input type="text" class="auto-input" vtype="num" minValue="10" maxValue="365" maxLength="3" name="logParamInfo.alarmLog" value="${logParamInfo.alarmLog}" required="true" data-icon="true" self-focus-info="请输入10至365的有效数字">
					<span class="right">天</span>
				</div>
			</div>
			<div class="sysBottom">
				<button type="button" class="sys-bPrimary" id="logApp">应用</button>
				<button type="button" class="sys-cancel cancelEdit">取消修改</button>
			</div>
		</fieldset>
	</form>
</div>

<script type="text/javascript">
	$(function(){
		FinanceUtil.initFormUI("#logSave");
		$("#logApp").on('click', function(event) {
			FMS.sysconfig.logSaveApp();
		});
		$(".cancelEdit").on('click', function(event) {
			FMS.sysconfig.cancelEdit();
		});
	});
</script>