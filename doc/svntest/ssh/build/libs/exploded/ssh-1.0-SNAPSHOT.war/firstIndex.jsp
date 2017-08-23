
<%@ taglib prefix="s" uri="/struts-tags"%>
<%--
  Created by IntelliJ IDEA.
  User: churui
  Date: 2017/8/21
  Time: 10:20
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>Title</title>
</head>
<body>
    <div>
        <ul>
            <s:if test="%{modelnum==1}">
                <li><a href="${pageContext.request.contextPath}?currentModelCode=${modelnum}">组织资源</a></li>
            </s:if>
            <s:if test="%{modelnum==2}">
                <li><a href="${pageContext.request.contextPath}?currentModelCode=${modelnum}">用户管理</a></li>
            </s:if>
            <s:if test="%{modelnum==0}">
                <li><a href="${pageContext.request.contextPath}?currentModelCode=${orgcode}">组织资源</a></li>
                <li><a href="${pageContext.request.contextPath}?currentModelCode=${rolecode}">用户管理</a></li>
            </s:if>
        </ul>
    </div>
</body>
</html>
