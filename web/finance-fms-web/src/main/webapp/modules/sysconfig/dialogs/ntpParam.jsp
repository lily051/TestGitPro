<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<div id="ntpForm">
	<form class="form-horizontal form-fixed form-center">
		 <fieldset>
		 	<legend>
				<label>NTP校时</label>
			</legend>
			<div class="control-group">
				<label class="control-label textR width128">
					NTP校时：
				</label>
				<div class="controls" >
					<span class="check" style="display:block;float:left;padding-top:4px;"><input type="checkbox" name="ntpParamInfo.nenable" id="ntpEnable" checked="checked" value="${ntpParamInfo.nenable }"></span>
					<label for="ntpEnable" style="display:block;float:left">&nbsp;启用NTP校时</label>
				</div>
			</div>
			<div class="control-group">
				<label class="control-label textR width128">
					服务器地址：
				</label>
				<div class="controls">
					<input id="ntpAddress" name="ntpParamInfo.ntpAddress" value="${ntpParamInfo.ntpAddress }" data-old="${ntpParamInfo.ntpAddress }" type="text" class="auto-input"  vtype="ip" maxLength="15" required="true" class='auto-input' data-icon="true" />
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">NTP端口：</label>
				<div class="controls">
					<input id="ntpPortNo" name="ntpParamInfo.ntpPortNo" value="${ntpParamInfo.ntpPortNo }" data-old="${ntpParamInfo.ntpPortNo }" type="text" class="auto-input"  vtype="num" required="true" class='auto-input' data-icon="true" maxvalue="65535" />
				</div>
			</div>
			<div class="control-group">
				<label class="control-label">校时时间间隔：</label>
				<div class="controls">
					<input id="ntpInterval" name="ntpParamInfo.ntpInterval" value="${ntpParamInfo.ntpInterval }" data-old="${ntpParamInfo.ntpInterval }" type="text" class="auto-input"  vtype="num" required="true" class='auto-input' data-icon="true" minvalue="1" maxvalue="10080" self-focus-info="请输入1到10080之间的数字"/>
					<span>(分钟)</span>
				</div>
			</div>
			<div class="sysBottom">
				<button type="button" class="sys-bPrimary" id="ntpFormOk">保存</button>
			</div>
		</fieldset>
	</form>
</div>

<script type="text/javascript">
	$(function(){
		if($('#ntpEnable').val()==0){
			$('#ntpAddress').val("").attr("disabled","disabled");
			$('#ntpPortNo').val("").attr("disabled","disabled");
			$('#ntpInterval').val("").attr("disabled","disabled");
			$("#ntpEnable").removeAttr('checked');
		}
		
		FinanceUtil.initFormUI("#ntpForm");
		$('#ntpEnable').click(function(){
			if($(this).prop('checked')){
				$('#ntpAddress').attr("required",true).removeAttr("disabled");
				$('#ntpPortNo').attr("required",true).removeAttr("disabled").val("123");
				$('#ntpInterval').attr("required",true).removeAttr("disabled").val("10");
				$('#ntpEnable').val('1');
			}else{
				$('#ntpAddress').attr("required",false).val("").attr("disabled","disabled").clearValidate();
				$('#ntpPortNo').attr("required",false).val("").attr("disabled","disabled").clearValidate();
				$('#ntpInterval').attr("required",false).val("").attr("disabled","disabled").clearValidate();
				$('#ntpEnable').val('0');
			}
			$.uniform.update();
		});
		
		$("#ntpFormOk").on('click', function(event) {
			FMS.sysconfig.ntpParamApp();
		});
	});
</script>