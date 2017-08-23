
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
    <title>登录</title>
      <script src="jquery-1.9.1.js"></script>
    <style>
      #index {
        position: absolute;
        top: 30%;
        width: 100%;
        height: 350px;
        text-align: center;
        border-right: 1px solid black;
        border-top: 1px solid black;
        border-left: 1px solid black;
        border-bottom: 1px solid black;
        background-color: #1C86EE;
      }
      #inindex{
        margin-left: 1000px;
        width: 320px;
        height: 320px;
        margin-top: 20px;
        background-color:whitesmoke;
      }
      #indexform{
        margin-left: 10px;
        width: 290px;
        height: 220px;
        padding-top: 40px;
        padding-left: 20px;
        background-color: whitesmoke;
      }
      #submit{
        display: block;
        height: 30px;
        width: 150px;
        margin-left: 85px;
        background-color: #1C86EE;
        font-family: "微软雅黑";
      }
        #h3{
            display: block;
            height: 10px;
            width: 150px;
            margin-left: 65px;
        }
      .yzm{
          height: 25px;
          margin: 0 auto;
          width: 72%;
          line-height: 25px;
          position: relative;
          margin-bottom: 10px;
          margin-top: 3px;
          margin-left: 5px;
          display: none;
      }
      .code{
          width:50%;
          height: 35px;
          border:0;
          border-color: transparent;
          font-size:16px;
          border-radius: 5px;
          padding-left: 2px;
      }
      .code_pic{
          display: block;
          width:40%;
          height:35px;
          background-color: #34495e;
          color:#FFF;
          position: absolute;
          top: 0px;
          left:80%;
          border-radius: 5px;
          text-align: center;
      }

    </style>
      <script>
          var modelnum;
          function login(){
              var name = $("#rolename").val();
              var pwd = $("#rolepwd").val();
//              var codeinput = $("#codeInput").val();
//              var code = $("#code").html();

              $.ajax({
                  //url:"roleAction!login.action",
                  url:"roleAction!login.action",
                  type:"post",
                  data:{rolename:name,rolepwd:pwd},
                  success:function(data){
                      console.log(data.flag);
                      console.log(data.modelnum);
                      console.log(data.message);
                      modelnum=data.modelnum;
                      if(data.flag){
//                          if(modelnum==1){
//                              location.href="userindex.jsp?modelnum="+modelnum;
//                          }else if(modelnum==2){
//                              location.href="roleindex.jsp?modelnum="+modelnum;
//                          }else{
//                              location.href="userindex.jsp?modelnum="+modelnum;
//                          }
                          location.href="/indexAction!gotoIndexpage.action?currentModelCode="+modelnum;

                      }else{
                          $("#message").html(data.message);
                      }
                  }
              })
          }
          window.onload=function(){
              document.getElementById('code').onclick = changeImg;
              changeImg();
          }
          // 声明一个变量用于存储生成的验证码

          function changeImg(){
              // 验证码组成库
              var arrays=new Array(
                  '1','2','3','4','5','6','7','8','9','0',
                  'a','b','c','d','e','f','g','h','i','j',
                  'k','l','m','n','o','p','q','r','s','t',
                  'u','v','w','x','y','z',
                  'A','B','C','D','E','F','G','H','I','J',
                  'K','L','M','N','O','P','Q','R','S','T',
                  'U','V','W','X','Y','Z'
              );
              // 重新初始化验证码
              code ='';
              // 随机从数组中获取四个元素组成验证码
              for(var i = 0; i<4; i++){
                  // 随机获取一个数组的下标
                  var r = parseInt(Math.random()*arrays.length);
                  code += arrays[r];
              }
              // 验证码写入span区域
              document.getElementById('code').innerHTML = code;
          }
      </script>
  </head>
  <body>
  <div id="index">
    <div id="inindex">
      <div id="indexform">
         <h3 id="h3">登录系统</h3>
  <%--<s:form action="roleAction!login" namespace="/">--%>
      <%--<span>${message}</span>--%>
      <%--<s:textfield name="rolename" label="用户名"/>--%>
      <%--&lt;%&ndash;<s:textfield name="rolepwd" label="密码"/>&ndash;%&gt;--%>
      <%--<s:password name="rolepwd" label="密码"/>--%>
      <%--<s:submit  id="submit" value="登录"/>--%>
      <%--<input type="button" value="登录">--%>
  <%--</s:form>--%>
          <br>
              <span id="message"></span></br>
              用户名:<input  type="text" id="rolename"></br>
              密码:<input type="password" id="rolepwd"></br>
              <p class="yzm" id="codevalue"><input type="text" name="code" id="codeInput" class="code" placeholder="验证码">
              <span id="code" class="code_pic" title="看不清，换一张"></span></p>
              <input type="button" id="submit" value="登录" onclick="login();">
          </div>
      </div>
    </div>
  </div>
  </body>
</html>
