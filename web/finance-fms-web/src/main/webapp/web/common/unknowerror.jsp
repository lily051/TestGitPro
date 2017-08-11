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
<link href="${ctx}/baseui/themes/<%=style %>/css/bases.css" rel="stylesheet"/>
<%} %>
</head>
<body class="page">
	<div class="msg-h-weak msg-h-error">
		<i></i>
		<div class="msg-cnt">
			<span>操作失败，平台发生异常，请联系管理员。</span>
			<a href="javascript:history.go(-1);">返回上一页</a>
		</div>
	</div>
</body>
</html>