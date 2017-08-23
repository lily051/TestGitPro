
<%@ taglib prefix="s" uri="/struts-tags" %>
<%--
  Created by IntelliJ IDEA.
  User: churui
  Date: 2017/6/29
  Time: 15:46
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
  <script src="jquery-1.9.1.js"></script>
  <script>

      function addinfo(){
          var param = window.location.search;
          console.log(param);
          var nodeid = param.substring(7);
          console.log(nodeid);
          var name = $("#name").val();
          var age = $("#age").val();
          var phonenum = $("#phonenum").val();
          var address = $("#address").val();
          $.ajax({
              url:"usersave.action",
              type:"post",
              data:{"nodeid":nodeid,name:name,age:age,phonenum:phonenum,address:address},
              success:function(){
                  window.open('save.jsp','newwindow','height=400,width=200,top=0,left=0');
              }
          })

      }
  </script>
</head>
<body>
  <%--<s:form action="usersave" >--%>
    <%--<table>--%>
      <%--<tr><s:textfield name="name" label="员工姓名"/></tr>--%>
      <%--<tr><s:textfield name="age" label="年龄"/></tr>--%>
      <%--<tr><s:textfield name="phonenum" label="手机号码"/></tr>--%>
      <%--<tr><s:textfield name="address" label="家庭住址"/></tr>--%>
      <%--<tr><s:submit value="保存"/> </tr>--%>
    <%--</table>--%>
  <%--</s:form>--%>
  <table>
    <tr><s:textfield name="name"  id="name" label="员工姓名"/></tr>
    <tr><s:textfield name="age" id="age" label="年龄"/></tr>
    <tr><s:textfield name="phonenum" id="phonenum" label="手机号码"/></tr>
    <tr><s:textfield name="address" id="address" label="家庭住址"/></tr>
  </table>
  <div><input type="button" value="保存" onclick="addinfo();"></div>
</body>
</html>