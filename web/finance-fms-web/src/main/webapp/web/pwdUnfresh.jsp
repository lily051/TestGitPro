<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<script src="/js/pwdVerification.js"></script>
<div>
	<form class="form-horizontal form-fixed formLabel120">
		<div class="msg-b msg-b-attention" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;">
		    <div class="msg-cnt">
		        <p class="popInfo"><span>红色<em>*</em>为必填项 &emsp;&emsp;最低密码安全等级：<em id="pwdLvl"></em></span></p>
		        <p class="popInfo">可鼠标悬浮对应密码等级块查看详细校验规则。</p>
		    </div>
		</div>
		<input type="hidden" name="strName" value="" id="strName"/>
		 <fieldset class="noLegend">
		 	 <div class="control-group">
		 	    <label class="control-label"><em>*</em>当前密码：</label>
		 	    <div class="controls">
		 	        <input type="password" required="true" class="auto-input" maxLength="64" data-icon="true"
		 	            isoverflown="true" id="oldPassword" autocomplete="off">
		 	    </div>
		 	</div>
				<div class="control-group">
				    <label class="control-label"><em>*</em>新密码：</label>
				    <div class="controls">
				       <input type="password" required="true" class="auto-input" maxLength="64" data-icon="true"
	                        isoverflown="true" id="userPassword" autocomplete="off" >
				    </div>
				</div>
				<div class="control-group">
					<input type="hidden" name="iRank" value="${passwordSecurityLevel}" id="iRank"/>
					<label class="control-label"></label>
				    <div class="controls">
				    	<span class="password">密码等级</span>
				    	<div class="rePWD" id="colorPWD">
				    	    <span class="" title="密码长度小于8位，或者只包含数字，小写字母，大写字母，特殊字符4类字符中的任意一类，或者密码与用户名一样或是用户名的倒写(空格不算字符)"></span>
				    	    <span class="" title="密码长度大于8位，包含两类字符，且组合为（数字+小写字母）或（数字+大写字母）(空格不算字符)"></span>
				    	    <span class="" title="密码长度大于8位，包含两类字符，且组合不能为（数字+小写字母）和（数字+大写字母)(空格不算字符)"></span>
				    	    <span class="" title="密码长度大于8位，包含数字，小写字母，大写字母，特殊字符三类字符及以上(空格不算字符)"></span>
				    	    <i id="iRankText"></i>
				    	</div>
				    </div>
				</div>
				<div class="control-group">
				    <label class="control-label"><em>*</em>确认密码：</label>
				    <div class="controls">
				        <input id="confirmPassword" type="password" autocomplete="off" class="auto-input" vtype="confirmPWD" required="true" isoverflown="true" maxlength="64" confirmpwd="userPassword" data-icon="true" self-focus-info="请再次输入密码">
				    </div>
				</div>
		 </fieldset>
	</form>
</div>

<script>
$("#strName").val($("#username").val());
	//安全等级显示
	showPwdLevel();
</script>