<%@page import="com.hikvision.finance.core.util.StringUtils"%>
<%@page import="com.hikvision.finance.fwork.util.CookieUtil"%>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<c:set var="ctx" value="${pageContext.request.contextPath}" /><c:set var="debug" value="true" scope="request" />
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>错误</title>
<link href="${ctx}/baseui/themes/default/css/reset.css" rel="stylesheet"/>
<link href="${ctx}/baseui/themes/default/css/base.css" rel="stylesheet"/>
<%String style =StringUtils.defaultIfBlank(CookieUtil.getCookieValue(request, "HIK_COOKIE_STYLE"), "default");
if(!StringUtils.equals(style, "default")){
%>
<link href="${ctx}/baseui/themes/<%=style %>/css/base.css" rel="stylesheet"/>
<%} %>
	<script type="text/javascript">
		//防止页面被嵌套在Iframe里
        function frameBuster(){
            if (window != top)
                top.location.href = location.href;
        }
	</script>
</head>
<body class="page">
	<div class="msg-h-weak msg-h-attention">
		<i></i>
		<div class="msg-cnt">
			<span>您尚未登录。</span>
			<a href="/index.jsp" target="_top">返回登录页面</a>
		</div>
	</div>
</body>
</html>