<!DOCTYPE HTML>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html>
<head>
	<meta charset="utf-8">
    <title>FMS</title>
	<%@ include file="/common/commons.jsp" %>
	<%@ include file="/common/commons-extend.jsp" %>
	<link  href="/css/base.css" rel="stylesheet"></link>
</head>
<body>
	<div id="wrapper">
		<!-- Top begins -->
		<%@ include file="/common/commons-basemenu.jsp"%>
	</div>
	<script type="text/javascript">
		//var url = $("ul.nav li").eq(0).find("a").data("url");
		$("ul.nav li").eq(0).click();
	</script>
</body>
</html>