<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1" />
<%@ include file="/common/commons.jsp" %>
<%@ include file="/common/commons-extend.jsp" %>
<meta http-equiv="pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<script src="/js/pwdVerification.js"></script>
<style>
	.pop_overlay{
		filter: progid:DXImageTransform.Microsoft.Alpha(opacity=30);
	}
</style>
<body>

	<div class="popWrap popWin ui-draggable" tabindex="-1"
		style="z-index: 1002; width: 440px; height: 330px; top: 49%; left: 50%;margin-left:-220px;margin-top:-165px; ">
		<div class="popInner">
			<div class="popTitle">
				<h3 title="修改密码">修改密码</h3>
			</div>
			<div class="popContent">
				<div class="dialog"
					style="overflow: hidden; width: auto; min-height: 0px; height: 241px;"
					id="changePWD" scrolltop="0" scrollleft="0">

					<div>
						<form class="form-horizontal form-fixed formLabel120">
							<div class="msg-b msg-b-attention"
								style="padding-top: 8px; padding-bottom: 10px; margin-top: 0;">
								<div class="msg-cnt">
									<p class="popInfo">
										<span>红色<em>*</em>为必填项 最低密码安全等级：<em id="pwdLvl">风险密码</em></span>
									</p>
									<p class="popInfo">可鼠标悬浮对应密码等级块查看详细校验规则。</p>
								</div>
							</div>
							<input type="hidden" name="strName" value="${username}" id="strName">
							<fieldset class="noLegend">
								<div class="control-group">
									<label class="control-label"><em>*</em>当前密码：</label>
									<div class="controls">
										<input type="password" required="true" class="auto-input"
											maxlength="64" data-icon="true" isoverflown="true"
											id="oldPassword" autocomplete="off">
									</div>
								</div>
								<div class="control-group">
									<label class="control-label"><em>*</em>新密码：</label>
									<div class="controls">
										<input type="password" required="true" class="auto-input"
											maxlength="64" data-icon="true" isoverflown="true"
											id="userPassword" autocomplete="off">
									</div>
								</div>
								<div class="control-group">
									<input type="hidden" name="iRank" value="${passwordSecurityLevel}" id="iRank">
									<label class="control-label"></label>
									<div class="controls">
										<span class="password">密码等级</span>
										<div class="rePWD" id="colorPWD">
											<span class=""
												title="密码长度小于8位，或者只包含数字，小写字母，大写字母，特殊字符4类字符中的任意一类，或者密码与用户名一样或是用户名的倒写(空格不算字符)"></span>
											<span class=""
												title="密码长度大于8位，包含两类字符，且组合为（数字+小写字母）或（数字+大写字母）(空格不算字符)"></span>
											<span class=""
												title="密码长度大于8位，包含两类字符，且组合不能为（数字+小写字母）和（数字+大写字母)(空格不算字符)"></span>
											<span class=""
												title="密码长度大于8位，包含数字，小写字母，大写字母，特殊字符三类字符及以上(空格不算字符)"></span>
											<i id="iRankText"></i>
										</div>
									</div>
								</div>
								<div class="control-group">
									<label class="control-label"><em>*</em>确认密码：</label>
									<div class="controls">
										<input id="confirmPassword" type="password" autocomplete="off"
											class="auto-input" isoverflown="true" vtype="confirmPWD"
											required="true" maxlength="64" confirmpwd="userPassword"
											data-icon="true" self-focus-info="请再次输入密码">
									</div>
								</div>
							</fieldset>
						</form>
					</div>

				</div>
			</div>
			<div class="popButton">
				<button type="button" class="bPrimary" id="submit">确认</button>
				<button type="button" id="cancel">取消</button>
			</div>
		</div>
	</div>
	<div class="pop_overlay"
		style="width: 1920px; height: 901px; z-index: 1001; opacity: 0.25;"></div>
</body>
<script>
//$("#strName").val($("#username").val());
	//安全等级显示
	showPwdLevel();
	//修改密码方法
$('#submit').on('click',function(){
	var form = $('#changePWD').find('form');
    var iRank = $('#iRank').val();
    var value = $('#userPassword').val();
    var strName = $('#strName').val();
    var curIR = getPwdRank(value,strName);
	form.ajaxSubmit({
        beforeSubmit: function() {
          if(curIR < iRank){
              jAlert('密码安全等级不符合要求', '错误', 'error');
              return false;
          }
          if (FinanceUtil.validate(form)) {
              FinanceUtil.loader.show('请稍候...');
              return true;
          }else {
          	  jAlert('信息输入有误', '错误', 'error');
              return false;
          }
        },
        url: '/web/csChangePwd.action',//TODO 
        type: 'post',
        dataType: 'json',
        data: {
				'newPwd': SHA256($('#confirmPassword').val()),
				'oldPwd': SHA256($('#oldPassword').val()),
				'username':$('#strName').val()
			},
        success: function(data) {
            FinanceUtil.loader.hide();
            if (data && data.success) {
              	jAlert("修改密码成功", '成功' , 'ok', function(){
              		window.opener=null;
              		window.open('','_self');
              		window.close();
              	});
            } else if (data && data.msg) {
                jAlert(data.msg, '错误' , 'error');
            }
        }
    });
});	
	
$('#cancel').on('click',function(){
	window.opener=null;
	window.open('','_self');
	window.close();
});
</script>
</html>