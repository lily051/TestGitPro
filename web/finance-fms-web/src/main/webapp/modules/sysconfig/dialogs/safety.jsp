<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<style>
	.table td,th{
		_padding:8px;
	}
</style>
<div id="safety">
	<form class="form-horizontal platformForm" style="margin-left:20px;margin-top:20px;margin-right:20px;" id="safeStrategyForm">
	    <fieldset>
	    	<legend>
				<span>密码安全等级配置</span>
			</legend>
			<div class="datatable" style="padding-bottom:30px;">
				<table cellspacing="0" cellpadding="0" width="100%" class="table">
					<colgroup>
						<col width="8%">
						<col width="8%">
						<col>
					</colgroup>
					<thead>
						<tr>
							<th><b>密码等级</b><span class="safeStrategyLine1">|</span></th>
							<th><b>是否选择</b><span class="safeStrategyLine1">|</span></th>
							<th><b>备注</b></th>
						</tr>
					</thead>
					<tbody>
						<tr>
							<td>
								<span>风险密码</span>
							</td>
							<td>
								<label>
									<span class="check">
										<input type="radio" value="0" name="passwordParamInfo.passwordSecruityLevel" <s:if test="passwordParamInfo.passwordSecruityLevel==0">checked</s:if>/>
									</span>
								</label>
							</td>
							<td>
								<span>密码长度小于8位，或者只包含数字，小写字母，大写字母，特殊字符4类字符中的任意一类，或者密码与用户名一样或是用户名的倒写(空格不算字符)</span>
							</td>
						</tr>
						<tr class="eventr">
							<td>
								<span>弱密码</span>
							</td>
							<td>
								<label>
									<span class="check">
										<input type="radio" value="1" name="passwordParamInfo.passwordSecruityLevel" <s:if test="passwordParamInfo.passwordSecruityLevel==1">checked</s:if>/>
									</span>
								</label>
							</td>
							<td>
								<span>密码长度大于8位，包含两类字符，且组合为（数字+小写字母）或（数字+大写字母）(空格不算字符)</span>
							</td>
						</tr>
						<tr>
							<td>
								<span>中密码</span>
							</td>
							<td>
								<label>
									<span class="check">
										<input type="radio" value="2" name="passwordParamInfo.passwordSecruityLevel" <s:if test="passwordParamInfo.passwordSecruityLevel==2">checked</s:if>/>
									</span>
								</label>
							</td>
							<td>
								<span>密码长度大于8位，包含两类字符，且组合不能为（数字+小写字母）和（数字+大写字母)(空格不算字符)</span>
							</td>
						</tr>
						<tr class="eventr">
							<td>
								<span>强密码</span>
							</td>
							<td>
								<label>
									<span class="check">
										<input type="radio" value="3" name="passwordParamInfo.passwordSecruityLevel" <s:if test="passwordParamInfo.passwordSecruityLevel==3">checked</s:if>/>
									</span>
								</label>
							</td>
							<td>
								<span>密码长度大于8位，包含数字，小写字母，大写字母，特殊字符三类字符及以上(空格不算字符)</span>
							</td>
						</tr>
					</tbody>								 
				</table>
			</div>
			<div><p style="color:red;">注：密码安全等级限制平台用户密码输入的强度</p></div>
	    </fieldset>
	    <fieldset>
	    	<legend>
	    		<span>平台初始密码配置</span>
	    	</legend>
	    	<div class="control-group">
	    		<label class="control-label textR width130"><em>*</em>密码保鲜天数：</label>
	    		<div class="controls" style="position:relative;">
	    			<input name="passwordParamInfo.pwdFreshDays" value="${passwordParamInfo.pwdFreshDays}" type="text" required="true" class="auto-input" vtype="num" minValue="1" maxValue="999" maxLength="3" data-icon="true"  self-focus-info="请输入1至999的有效数字"/>
	    			<span style="position:absolute;left:190px;_left:166px;top:0">天</span>
	    		</div>
	    	</div>
<%-- 修改提示天数先保留，后续可能会用到 --%>
<%-- 	    	<div class="control-group">
	    		<label class="control-label textR width130"><em>*</em>密码修改提示天数：</label>
	    		<div class="controls" style="margin-left:0;">
	    			<input name="passwordParamInfo.pwdModfiyDays" value="${passwordParamInfo.pwdModfiyDays}" type="text" class="auto-input" vtype="num" required="true" maxLength="2" data-icon="true"/>
	    		</div>
	    	</div> --%>
	    	<div class="control-group">
	    		<label class="control-label textR width130"><em>*</em>初始密码：</label>
	    		<div class="controls" style="margin-left:0;">
	    			<input id="passwordDefault"  value="${passwordParamInfo.defaultPassword}" type="password" autocomplete="off" class="auto-input" required="true" maxLength="64" data-icon="true"/>
	    			<input id="passwordDefaultOld" value="${passwordParamInfo.defaultPassword}" type="hidden" class="auto-input" />
	    		</div>
	    	</div>
	    </fieldset>
	    <fieldset>
	    	<legend>
	    		<span>账户锁定配置</span>
	    	</legend>
	    	<div class="control-group">
	    		<label class="control-label textR width130"><em>*</em>登录次数：</label>
	    		<div class="controls" style="position:relative;">
	    			<input name="passwordParamInfo.loginTimes" value="${passwordParamInfo.loginTimes}" type="text" required="true" class="auto-input" vtype="num" minValue="5" maxValue="20" maxLength="2" data-icon="true" self-focus-info="请输入5至20的有效数字"/>
	    			<span style="position:absolute;left:190px;_left:166px;top:0">次</span>
	    		</div>
	    	</div>
	    	<div class="control-group">
	    		<label class="control-label textR width130"><em>*</em>登录锁定时间：</label>
	    		<div class="controls" style="position:relative;">
	    			<input name="passwordParamInfo.lockTime" value="${passwordParamInfo.lockTime}" type="text" required="true" class="auto-input" vtype="num" minValue="10" maxValue="60" maxLength="2" data-icon="true" self-focus-info="请输入10至60的有效数字"/>
	    			<span style="position:absolute;left:178px;_left:166px;top:0">分钟</span>
	    		</div>
	    	</div>
	    </fieldset>
	    <div class="sysBottom" style="margin-top:55px;margin-left:135px"><button type="button" id="safetyApp" class="sys-bPrimary">应用</button></div>
	</form>
</div>
<script>
	$(function(){
		FinanceUtil.initFormUI("#safety");
		$("#safetyApp").on('click', function(event) {
			FMS.sysconfig.safetyApp();
		});
	});
</script>