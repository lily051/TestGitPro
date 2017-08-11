<!DOCTYPE html>
<%@page import="com.hikvision.finance.core.util.StringUtils"%>
<%@page import="com.hikvision.finance.fwork.util.CookieUtil"%>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<c:set var="ctx" value="${pageContext.request.contextPath}" />
<html>
<head>
<meta charset="utf-8"/>
<title>${Sysconfig_6000 }</title>
<link href="/fav.ico" rel="shortcut icon"/>
<link rel="stylesheet" type="text/css" href="${ctx}/baseui/themes/default/css/reset.css"/>
<link rel="stylesheet" type="text/css" href="${ctx}/baseui/themes/default/css/base.css"/>
<link rel="stylesheet" type="text/css" href="${ctx}/baseui/themes/default/css/login.css"></link>
<%String style =StringUtils.defaultIfBlank(CookieUtil.getCookieValue(request, "HIK_COOKIE_STYLE"), "default");
if(!StringUtils.equals(style, "default")){
%>
<link href="${ctx}/baseui/themes/<%=style %>/css/base.css" rel="stylesheet"/>
<link href="${ctx}/baseui/themes/<%=style %>/css/login.css" rel="stylesheet"/>
<%} %>
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
		background: url(<%=request.getParameter("loginUrl") %>) no-repeat center;
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
		height: 33px;
		top: 1px;
		left: 1px;
	}
	#passwordIcon{
		background: url(/images/icons/password.png) no-repeat 5px center #fff !important;
		position: absolute;
		width: 32px;
		height: 33px;
		top: 1px;
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
<div class="wrapper">
	<div class="login-wrapper">
		<div class="header">
			<img src='<%=request.getParameter("logoUrl")%>' width="48" class="logoPic">
			<h1 class="logo">${Sysconfig_6000 }</h1>
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
							<input id="username" class="input-text" maxlength="64" name="usernameN" type="text" disabled>
						</div>
						<div class="login-input">
							<span id="passwordIcon"></span>
							<p class="placeholder">密码</p>
							<input id="password" class="input-text" maxlength="64" name="password" type="password" disabled>
						</div>
						<div id="login_btn"><a href="#" class="btn login_btn">登录</a></div>
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
<script type="text/javascript" src="${ctx}/baseui/js/language/messages_zh_cn.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/base.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/jquery.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/underscore.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/jquery-override.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/ui/jquery.alerts.js"></script>
<script type="text/javascript" src="${ctx}/baseui/js/plugins/ui/jquery.placeholder.js"></script>
<script type="text/javascript" src="${ctx }/financeui/js/md5.js"></script>
<script type="text/javascript">
	$(document).ready(function(){
		var platformDiscribe = '<%=request.getParameter("platformParamInfo.platformDiscribe")%>',
			platformCopyright = '<%=request.getParameter("platformParamInfo.platformCopyright")%>';
		$('.logo').text(decodeURIComponent(decodeURIComponent(platformDiscribe)));
		$(document).attr("title",decodeURIComponent(decodeURIComponent(platformDiscribe)));
		$(".copyright").text(decodeURIComponent(decodeURIComponent(platformCopyright)));
		//防止页面被嵌套在Iframe里
        function frameBuster(){
            if (window != top)
                top.location.href = location.href;
        }
        window.onload = frameBuster;
	});
</script>
</body>
</html>