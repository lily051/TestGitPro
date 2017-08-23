<%--
  Created by IntelliJ IDEA.
  User: churui
  Date: 2017/7/26
  Time: 15:45
  To change this template use File | Settings | File Templates.
--%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<body>
<s:form action="addnode" >
    <table>
        <tr><s:textfield name="nodename" label="节点名称"/></tr>
        <tr><s:submit value="保存"/> </tr>
    </table>
</s:form>
</body>
</html>