<!DOCTYPE html>
<%@page import="com.hikvision.finance.core.util.StringUtils"%>
<%@page import="com.hikvision.finance.fwork.util.CookieUtil"%>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<c:set var="ctx" value="${pageContext.request.contextPath}" />
<html>
<head>
<div id="license" sys_jkn="," style="display:none"></div>
<meta charset="utf-8"/>
<meta http-equiv="X-UA-Compatible" content="IE=edge" />
<title>${Sysconfig_6000}</title>
<link href="/favicon.ico" rel="shortcut icon" />
<link rel="stylesheet" type="text/css" href="${ctx}/baseui/themes/default/css/reset.css"/>
<link rel="stylesheet" type="text/css" href="${ctx}/baseui/themes/default/css/base.css"/>
<link rel="stylesheet" type="text/css" href="${ctx}/baseui/themes/default/css/login.css"></link>
<link href="${ctx}/baseui/js/plugins/popup/skin/default/pop.css" rel="stylesheet"/>
<link  href="/css/base.css" rel="stylesheet"></link>
<%String style =StringUtils.defaultIfBlank(CookieUtil.getCookieValue(request, "HIK_COOKIE_STYLE"), "default");
if(!StringUtils.equals(style, "default")){
%>
<link href="${ctx}/baseui/themes/<%=style %>/css/base.css" rel="stylesheet"/>
<link href="${ctx}/baseui/themes/<%=style %>/css/login.css" rel="stylesheet"/>
<%} %>
<script type="text/javascript" src="${ctx}/baseui/js/language/messages_zh_cn.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/base.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/jquery.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/underscore.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/jquery-override.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/ui/jquery.alerts.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/ui/jquery.placeholder.js"></script>
<script type="text/javascript" src="/js/sha256.js"></script>
<script type="text/javascript" src="${ctx }/financeui/js/md5.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/jquery-ui.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/popup/pop.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/forms/jquery.uniform.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/forms/jquery.ibutton.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/ui/jquery.tipsy.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/ui/jquery.timeentry.min.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/forms/jquery.form.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/forms/jquery.validate.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/ui/jquery.cookie.js"></script>
<script type="text/javascript" src="${ctx}/financeui/js/ivms-js-override.js"></script>
<script type="text/javascript" src="${ctx}/financeui/js/utils/FinanceUtil.js"></script>
<script src="/js/language/message_zh_cn.js"></script>
<script src="/js/globalsetting.js"></script>
</head>
<style type="text/css">
	.login-wrapper .header .logo{
		font-family:'Microsoft Yahei';
		font-weight: 700;
		font-size: 20px;
		color: #3a3a3a;
		letter-spacing: 2px;
		margin: 0; 
		display: inline-block;
  		vertical-align: middle;
	}
	.header{
		_padding:0;
		width:1024px;
	}
	.logoPic{
		display: inline-block;
		_float: left;
  		vertical-align: middle;
	}
	.login-content{
		padding: 0;
	}
	.mainBg{
		background: url("${Sysconfig_6004}") no-repeat center;
		/* filter: progid:DXImageTransform.Microsoft.AlphaImageLoader(src='${Sysconfig_6004}',sizingMethod='scale'); */
	}
	.login-panel{
		background-color: #fff;
		_margin-left:230px;
		margin:0 0 0 230px;
		top:63px;
	}
	.copyright{
		color: #a0a0a0;
	}
	.login-panel h3{
		color: #7D7D7D;
	}
	.btn{
		background: #39A1EA;
		color: #fff;
	}
	.btn:hover{
		background: #1689D6;
		color: #fff;
	}
	#usernameIcon{
		background: url(/images/icons/username.png) no-repeat 5px center #fff !important ;
		position: absolute;
		width: 32px;
		height: 32px;
		top: 2px;
		left: 1px;
	}
	#passwordIcon{
		background: url(/images/icons/password.png) no-repeat 5px center #fff !important;
		position: absolute;
		width: 32px;
		height: 32px;
		top: 2px;
		left: 1px;
	}
	input.input-text{
		width: 220px;
		box-sizing: border-box;
		height: 35px;
		_height:23px;
		padding: 6px 0 5px 32px!important;
		_line-height: 23px;
	}
	.placeholder {
		padding: 6px 0 5px 32px!important;
		_height:23px;
		_line-height: 23px;
	}
	.login_btn {
	    margin-bottom: 10px;
	    margin-top:10px;
	    padding: 8px;
	    display: block;
	    background: #39A1EA;
	    color: #fff;
	    font-size: 16px;
	    font-weight: bold;
	    text-align: center;
	    border-radius: 3px;
	    clear: both;
	    _width:100%;
	    _padding-left:24px;
	}
	.login_btn:hover { color: #fff; opacity: 0.9; filter: alpha( opacity = 90 ); }
	.login_btn:active { opacity: 1; filter: alpha( opacity = 100 ); }
</style>


<body>
<div id="wrapper">

	<div class="login-wrapper">
		<div class="header">
			<img src="${Sysconfig_6002 }" width="48" class="logoPic">
			<h1 class="logo" style="margin-left:10px;">${Sysconfig_6000 }</h1>
		</div>
		<div class="login">
			<div class="login-content">
				<div style="margin:0 auto;height:460px;width: 100%;" class="mainBg">
            	</div>
				<div class="login-panel">
					<h3><span style="color:#596A7E">欢迎登录</span></h3>
					<div class="login-form">
						<div id="login_msg">
							<i></i><span></span>
						</div>
						<div class="login-input">
							<span id="usernameIcon"></span>
							<p class="placeholder">用户名</p>
							<input id="username" class="input-text" maxlength="50" name="usernameN" type="text" value="<%=StringUtils.defaultIfBlank(CookieUtil.getCookieValue(request, "HIK_COOKIE_NAME"), "")%>">
						</div>
						<div class="login-input">
							<span id="passwordIcon"></span>
							<p class="placeholder">密码</p>
							<input id="password" class="input-text" maxlength="64" name="password" type="password">
						</div>
						<div id="kaptcha" class="clearfix login-input hidden">
							<p class="placeholder">请输入验证码</p>
							<input class="input-text" type="text" id="kaptcha_input" style="width:140px;_width:110px;_float:left;" maxlength="4">
							<img src="/randomCode.jpg" id="kaptchaImage" style="width:70px;height:34px;float:right;cursor:pointer"/>   
						</div>
						<script>
							var errorTimes = $.cookie("errorTimes");
							if(errorTimes == null){
								setCookie(0);
								errorTimes = Number($.cookie("errorTimes"));
							}else{
								errorTimes = Number($.cookie("errorTimes"));
								if(errorTimes>2){
									$('#kaptchaImage').click();
									$("#kaptcha").removeClass('hidden');
								}
							}
							function setCookie(times){
								var expiresDate= new Date();
								expiresDate.setTime(expiresDate.getTime() + (60 * 60 * 1000));
								$.cookie("errorTimes",times,{expires: expiresDate});
							}
						</script>
						<div id="login_btn"><a href="#" class="login_btn">登录</a></div>
						<div class="diviver"></div>
					</div>
				</div>
			</div>
		</div>
		<div class="footer">
			<div class="copyright">${Sysconfig_6001 }</div>
		</div>
	</div>
</div>



<script type="text/javascript">
	$(document).ready(function(){
		var errorCode = <%=request.getParameter("errorCode")%>;
		if(errorCode == -3){
			$('#login_msg').css("visibility", "visible");
			$('#login_msg span').html("用户未登录或登录过期");
		}
		//防止页面被嵌套在Iframe里
        function frameBuster(){
            if (window != top)
                top.location.href = location.href;
        }
        window.onload = frameBuster;
        $('input').focus(function(){
            $(this).addClass('active').prev('.placeholder').hide();
        }).blur(function(){
            $(this).removeClass('active').val() == "" ? $(this).prev('.placeholder').show() : $(this).prev('.placeholder').hide();
        });
        $('.placeholder').hover(function(){
            $(this).next('input').addClass('hover');
        },function(){
            $(this).next('input').removeClass('hover');
        }).click(function(){
            $(this).hide().next('input').trigger('focus');
        });
        if($('#username').val() == ''){
            $('#username').trigger('focus');
        } else {
        	$('#username').trigger('focus');
        	$('#password').trigger('focus');
        }
		$('#login_btn a').click(login);
		$("input").keydown(function(event) {
			if (event.which == 13) { //13等于回车键(Enter)键值,ctrlKey 等于 Ctrl
				login();
			}
		});
		$('#kaptchaImage').click(function(){$(this).hide().attr('src','/randomCode.jpg?'+Math.floor(Math.random()*100)).fadeIn(100);});
		function login() {
			if($('#username').val() == ''){
				$('#username').addClass('error');
				$('#login_msg').css("visibility", "visible");
				$('#login_msg span').html("用户名不能为空");
				errorTimes++;
				setCookie(errorTimes);
				if(errorTimes>2){
					$('#kaptchaImage').click();
					$("#kaptcha").removeClass('hidden');
				}
				return;
			}else{
                $('#username').removeClass('error');
            }
			if($('#password').val() == ''){
				$('#password').addClass('error');
				$('#login_msg').css("visibility", "visible");
				$('#login_msg span').html("密码不能为空");
				errorTimes++;
				setCookie(errorTimes);
				if(errorTimes>2){
					$("#kaptcha").removeClass('hidden')
				}
				return;
			}else{
                $('#password').removeClass('error');
            }
			var url = document.location.href;
			var ip = url.substring(url.indexOf('://')+3);
			ip = ip.substring(0,ip.indexOf('/'));
			var loginInfo ={
				"entity.strName": $("#username").val(),
				"entity.strPassword": SHA256($("#password").val()),
				'randomCode':'yzm'
			}

			if(!$("#kaptcha").hasClass('hidden')){
				loginInfo.randomCode = $("#kaptcha_input").val();
			}
			$.ajax({
				url:'/web/login.action',
				type:'post',
				dataType:'json',
				async:false,
				data:loginInfo,
				success:function(result){
						if(!result.success){
							$('#login_msg').css("visibility", "visible");
							$('#login_msg span').html(result.msg);
							if(result.data.type=='PASSWORD_UNFRESH'){
								showPwdChange(1);
							}else if(result.data.type=='PASSWORD_DEFAULT'){
								$("#password").val('');
								showPwdChange(2);
							}else{
								errorTimes++;
								setCookie(errorTimes);
								if(errorTimes>2){
									$('#kaptchaImage').click();
									$("#kaptcha").removeClass('hidden')
								}
							}
						} else {
							errorTimes=0;
							setCookie(errorTimes);
							$("#password").val(SHA256($("#password").val()));
							location.href = "/web/index.action";
						}
				}
			});
		}
		/**
		 * 修改密码框
		 * @param  {[type]} type 1是密码保鲜，2是初始密码修改
		 * @return {[type]}      [description]
		 */
		function showPwdChange(type){
			var titleText;
			if(type==1){
				titleText = '密码保鲜已过期，请修改密码';
			}else{
				titleText = '密码是初始密码，请修改密码'
			}
			FinanceUtil.dialog({
				id: 'changePWD',
				title: titleText,
				overlay: true,
				draggable: true,
				width: 450,
				height: 330,
				iframeFix:false,
				ajax: {
				  url: '/web/toPasswordUnfresh.action',//TODO 跳转到首次修改密码页面
				  success: function() {
				    FinanceUtil.initFormUI('#changePWD');
				    FMS.globalFun.isIE6ChangeTipText('#changePWD');
				  }
				},
				buttons: {
				  "ok": {
				    text: '确认',
				    'class': 'bPrimary',
				    click: function() {
				      var form = $('#changePWD').find('form');
				      var iRank = $('#iRank').val();
				      var value = $('#userPassword').val();
				      var strName = $('#username').val();
				      var curIR = getPwdRank(value,strName);
				      form.ajaxSubmit({
				          beforeSubmit: function() {
				            if(curIR < iRank){
				              jAlert('密码安全等级不符合要求', '错误', 'error');
				              return false;
				            }
				            if (FinanceUtil.validate(form)) {
				              return true;
				            }else {
				              jAlert('信息输入有误', '错误', 'error');
				              return false;
				            }
				          },
				          url: '/web/changePasswordNoLogin.action',//TODO 修改密码
				          type: 'post',
				          dataType: 'json',
				          data: {
				          	  'username':strName,
				          	  'oldPwd':SHA256($('#oldPassword').val()),
							  'newPwd': SHA256($('#userPassword').val())
						  },
				          success: function(data) {
				            if (data && data.success) {
				              	$.sticky('修改成功', {
    								type: "ok",
    								title: '操作成功'
	    						});
	    						$('#login_msg').css("visibility", "hidden");
	    						errorTimes=0;
	    						setCookie(errorTimes);
				                $('#changePWD').dialog('close');
				            } else if (data && data.msg) {
				                jAlert(data.msg, '错误' , 'error');
				            }
				          }
				      });
				    }
				  },
				  "cancel": {
				    text: '取消',
				    click: function() {
				      $('#changePWD').dialog('close');
				    }
				  }
				}
			});
		}
		 
		// 获取授权信息
 		$.ajax({
			url:'/web/getLicense.action',
			type:'post',
			dataType:'json',
			async:false,
			success:function(result){
				var license = result.data.notesId + "," + result.data.licenseNo;
				$('#license').attr("sys_jkn", license);
			}
		});
	});
</script>
</body>
</html>