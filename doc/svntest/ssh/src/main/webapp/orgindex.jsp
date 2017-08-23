<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
  <head>
    <title>人员管理</title>
  </head>
  <script src="jquery-1.9.1.min.js"></script>
  <link rel="stylesheet" href="css/common.css" type="text/css"/>
  <link rel="stylesheet" href="ztree/css/zTreeStyle/zTreeStyle.css" type="text/css">
  <script type="text/javascript" src="ztree/js/jquery.ztree.core.min.js"></script>
  <script type="text/javascript" src="ztree/js/jquery.ztree.excheck.min.js"></script>
  <style>
      ul{
          list-style:none
      }
      #add,#update,#anode,#asubnode,#updates,#updatenode{
          display: none;
          position: absolute;
          left:40%;
          top:40%;
          width:425px;
          height:200px;
          text-align: center;
          border-right: 1px solid #cccccc;
          border-top: 1px solid #cccccc;
          border-left: 1px solid #cccccc;
          border-bottom: 1px solid #cccccc;
          background-color: #fff;
      }
      #addcen,#updatecen,#addnode,#addsub,#updatescen,#nodeupdate{
          position: absolute;
          left:20%;
          top:18%;
      }
   Table{
       border-collapse: collapse;
       width: 600px; ;
       border:1px ;
      text-align: center;
      }
   #userTable{
       background-color: #ffffff;
   }
      #userTable td{
          border:#CCC solid 1px;
      }

      #userlist,#headtr{
          margin-top: 5px;
          text-align: center;
          background-color: dodgerblue;
          with:100%
      }
      #userlist td,#headtr td{
          height:32px;
          line-height: 32px;
      }
      input{
          autocomplete:off
      }

  </style>
  <script>
      var nodeid = "1";
      var uid;
      var parentid;
      var subnodeid;
    function show(){
        document.getElementById("add").style.display="block";
        $("#addscen #name").val("");
        $("#addscen #age").val("");
        $("#addscen #phonenum").val("");
        $("#addscen #address").val("");
    }
    function saveuser(){
        var name = $("#name").val();
        var age = $("#age").val();
        var phonenum = $("#phonenum").val();
        var address = $("#address").val();
        //console.log(11);
        $.ajax({
            url:"userAction!adduser.action",
            type:"post",
            data:{"nodeid":nodeid,name:name,age:age,phonenum:phonenum,address:address},
            success:function(data){
                if(data.flag){
                    showAll(nodeid);
                    document.getElementById("add").style.display="none";
                }else{
                    $("#addmsg").html(data.msg);
                }

            }
        })

    }
      function update(id,name,age,phonenum,address){
          uid = id;
          document.getElementById("update").style.display="block";
          $("#updatecen #uname").val(name);
          $("#updatecen #uage").val(age);
          $("#updatecen #uphonenum").val(phonenum);
          $("#updatecen #uaddress").val(address);
      }
    function updateuser(){
        var name = $("#uname").val();
        var age = $("#uage").val();
        var phonenum = $("#uphonenum").val();
        var address = $("#uaddress").val();
        //console.log(99);
        $.ajax({
            url:"userAction!updateuser.action",
            type:"post",
            data:{"nodeid":nodeid,id:uid,name:name,age:age,phonenum:phonenum,address:address},
            success:function(){
                showAll(nodeid);
                document.getElementById("update").style.display="none";
            }
        })
    }
    function deleteuser(id) {
        $.ajax({
            url:"userAction!deleteuser.action",
            type:"post",
            data:{"id":id},
            success:function(){
                showAll(nodeid);
           }
        })
    }
      function updates(nodeid,id,name,age,phonenum,address){
          uid = id;
          subnodeid = nodeid;
          console.log(333);
          console.log(nodeid);
          document.getElementById("updates").style.display="block";
          $("#updatescen #uuname").val(name);
          $("#updatescen #uuage").val(age);
          $("#updatescen #uuphonenum").val(phonenum);
          $("#updatescen #uuaddress").val(address);
      }
      function updatesuser(){
          var name = $("#uuname").val();
          console.log(name);
          var age = $("#uuage").val();
          var phonenum = $("#uuphonenum").val();
          var address = $("#uuaddress").val();
          //console.log(99);
          $.ajax({
              url:"userAction!updateuser.action",
              type:"post",
              data:{"nodeid":subnodeid,id:uid,name:name,age:age,phonenum:phonenum,address:address},
              success:function(){
                  showsubuser();
                  document.getElementById("updates").style.display="none";
              }
          })
      }
      function deletesuser(id) {
          $.ajax({
              url:"userAction!deleteuser.action",
              type:"post",
              data:{"id":id},
              success:function(){
                  showsubuser();
              }
          })
      }

    function showsubuser(){
        $.ajax({
            url:"userAction!findsubuser.action",
            type:"post",
            data:{"nodeid":nodeid},
            success:function(data){
                    console.log(data);
                    var subuser0 = data.subuserList0;
                    $("#userTable").html("");
                    var str = '<tr id="userlist"><td>姓名</td><td>年龄</td><td>手机号码</td><td>地址</td><td>操作</td></tr>';
                    $("#userTable").append(str);
                    for (var i=0;i<subuser0.length;i++){
                        var value = subuser0[i];
                        var nodetbl = value.nodetbl;
                        if(value.age==null){
                            value.age="";
                        }
                        //var str = "<tr><td>"+value.name+"</td><td>"+value.age+"</td><td>"+value.phonenum+"</td><td>"+value.address+"</td><td><a href='#' onclick='updates("+value.id+")'>修改</a><a href='#' onclick='deletesuser("+value.id+")'>删除</a></td></tr>";
                        var str = '<tr><td>'+value.name+'</td><td>'+value.age+'</td><td>'+value.phonenum+'</td><td>'+value.address+'</td><td><a href="#"onclick="updates('+"'"+nodetbl.nodeid+"'"+','+"'"+value.id+"'"+','+"'"+value.name+"'"+','+"'"+value.age+"'"+','+"'"+value.phonenum+"'"+','+"'"+value.address+"'"+')">修改</a><a href="#" onclick="deletesuser('+value.id+')">删除</a></td></tr>';
                        $("#userTable").append(str);
                    }
                    var subuser1 = data.subuserList1;
                    for(var i=0;i<subuser1.length;i++){
                        var value = subuser1[i];
                        var nodetbl = value.nodetbl;
                        if(value.age==null){
                            value.age="";
                        }
                        var str = '<tr><td>'+value.name+'</td><td>'+value.age+'</td><td>'+value.phonenum+'</td><td>'+value.address+'</td><td><a href="#" onclick="updates('+"'"+nodetbl.nodeid+"'"+","+"'"+value.id+"'"+','+"'"+value.name+"'"+','+"'"+value.age+"'"+','+"'"+value.phonenum+"'"+','+"'"+value.address+"'"+')">修改</a><a href="#" onclick="deletesuser('+value.id+')">删除</a></td></tr>';
                        $("#userTable").append(str);
                    }

            }
        })
    }

    function addnode(){
            document.getElementById("anode").style.display="block";
            $("#addnode #nodename").val("");
    }

      function nodeadd(){
          var nodename = $("#nodename").val();
          parentid=nodeid;
          $.ajax({
              url:"nodeAction!addnode.action",
              type:"post",
              data:{nodename:nodename,"parentid":nodeid},
              success:function(data){
                    if(data.flag){
                        showTree();
                        document.getElementById("anode").style.display="none";
                    }else {
                        $("#msg").html(data.msg);
                    }

              }
          })
      }
      function updatenode(){
          if(nodeid==1){
              //$("#nodeupdate #upnodename").val("Root");
              alert("根节点不可修改");
          }else{
              document.getElementById("updatenode").style.display="block";
              $("#nodeupdate #upnodename").val(nodename);
          }
      }
      function nodeupdate(){
          var name = $("#upnodename").val();
          console.log(name)
          $.ajax({
              url:"nodeAction!updatenode.action",
              type:"post",
              data:{nodename:name,"nodeid":nodeid},
              success:function(data){
                  if(data.flag){
                      console.log(data.flag);
                      showTree();
                      document.getElementById("updatenode").style.display="none";
                      $("#upmsg").html("");
                  }else {
                      $("#upmsg").html(data.msg);
                  }
              }
          })
      }
      showTree();
    function showTree() {
        $.ajax({
            url:"nodeAction!shownodetree.action",
            type: "post",
            success:function(data){
                var zTree;
                var zNodes=data.jsonArray;
                var setting = {
                    view: {
                        dblClickExpand: false,//双击节点时，是否自动展开父节点的标识
                        showLine: true,//是否显示节点之间的连线
                        fontCss:{'color':'black','font-weight':'bold','font-size':'medium'},//字体样式函数
                        selectedMulti: false, //设置是否允许同时选中多个节点
                        showIcon: false
                    },
                    data: {
                        simpleData: {//简单数据模式
                            enable:true,
                            idKey: "id",
                            pIdKey: "pId",
                            rootPId: ""
                        }
                    },
                    callback: {
                        beforeClick: function(treeId, treeNode) {
                            zTree = $.fn.zTree.getZTreeObj("nodetree");
                            if (treeNode.isParent) {
                                //zTree.expandNode(treeNode);//如果是父节点，则展开该节点
//                                treeNode.addClass("selectedNode");
                                nodeid=treeNode.id;
                                nodename=treeNode.name;
                                showAll(nodeid)
                            }else{
                                //zTree.checkNode(treeNode, !treeNode.checked, true, true);//单击勾选，再次单击取消勾选
                                //zTree.addClass("selectedNode");
                                nodeid=treeNode.id;
                                nodename=treeNode.name;
                                showAll(nodeid)
                            }
                        }
                    },

                };

                $.fn.zTree.init($("#nodetree"), setting, zNodes);
            }

        })
    }

    function  showAll(id) {
        nodeid=id;
        //console.log(nodeid);
        $.ajax({
            url:"userAction!findall.action",
            type:"post",
            data:{"nodeid":id},
            success:function (data) {
                $("#userTable").html("");
                var str = '<tr id="userlist"><td>姓名</td><td>年龄</td><td>手机号码</td><td>地址</td><td>操作</td></tr>';
                $.each(data, function(n, value){
                    var nodetbl = value.nodetbl;
                    if(value.age==null){
                        value.age="";
                    }
                   str += '<tr><td>'+value.name+'</td><td>'+value.age+'</td><td>'+value.phonenum+'</td><td>'+value.address+'</td><td><a href="#" onclick="updates('+"'"+nodetbl.nodeid+"'"+','+"'"+value.id+"'"+','+"'"+value.name+"'"+','+"'"+value.age+"'"+','+"'"+value.phonenum+"'"+','+"'"+value.address+"'"+')">修改</a><a href="#" onclick="deletesuser('+value.id+')">删除</a></td></tr>';
                });
                $("#userTable").append(str);
            }
        })
    }
    function deletesubnode() {
        $.ajax({
            url:"nodeAction!deletesubnode.action",
            type:"post",
            data:{"nodeid":nodeid},
            success:function(data){
                if(data.flag){
                    showTree();
                }else {
                    alert(data.msg);
                }
            }
        })
    }
    function canadd(){
        showAll(nodeid);
        document.getElementById("add").style.display="none";
    }
      function canup(){
          showAll(nodeid);
          document.getElementById("update").style.display="none";
      }
      function cansup(){
          showsubuser();
          document.getElementById("updates").style.display="none";
      }
      function cannode(){
          showAll(nodeid);
          document.getElementById("anode").style.display="none";
      }
      function canupnode(){
          showAll(nodeid);
          document.getElementById("updatenode").style.display="none";
      }
      function findsub(){
          $.ajax({
              url:"findbynodeid.action"}
          )
      }
      function logout(){
          $.ajax({
              url:"roleAction!logout.action",
              success:function(){
                  location.href="index.jsp";
              }
          })
      }
  </script>
  <body>
  <div id="nav" style="height: 30px">
      <s:if test="%{modelnum==1}">
          <a href="${pageContext.request.contextPath}?currentModelCode=${modelnum}">组织资源</a>
      </s:if>
      <s:if test="%{modelnum==2}">
          <a href="${pageContext.request.contextPath}?currentModelCode=${modelnum}">用户管理</a>
      </s:if>
      <s:if test="%{modelnum==0}">
          <a href="${pageContext.request.contextPath}?currentModelCode=${orgcode}">组织资源</a>
          <a href="${pageContext.request.contextPath}?currentModelCode=${rolecode}">用户管理</a>
      </s:if>
    <%--<a class="orgSource" id="source" href="${pageContext.request.contextPath}?currentModelCode=${modelnum}">组织资源</a>--%>
    <%--<a class="userManage" id="manager" onclick="rightcontrol()" href="#">用户管理</a>--%>
    <a href="#" id="logout" onclick="logout();">退出</a>
  </div>
  <div id="mainarea" class="out-center" style="height: 839px;">
  <div id="left" class="main-left" style="height: 842px;">
      <input type="button" value="添加" onclick="addnode();">
      <input type="button" value="修改" onclick="updatenode();">
      <input type="button" value="删除" onclick="deletesubnode();">
    <ul id="nodetree" class="ztree" style="overflow-y: scroll;overflow-x: scroll;height: 805px">
    </ul>
  </div>
  <div id="right" class="main-right" style="width: 1667px;height: 842px;">
   <!--<span>${rolename}</span>-->
   <input type="button" value="添加" onclick="show();">
   <input type="button" value="查询" onclick="showsubuser();">
   <table  id="userTable" >
     <tr id="headtr" >
       <td>姓名</td>
       <td>年龄</td>
       <td>手机号码</td>
       <td>地址</td>
       <td>操作</td>
     </tr>

      <!--
      <tr>
        <td>ww</td>
        <td>23</td>
        <td>15968136346</td>
        <td>weffg</td>
        <td><input type="button" value="修改"><input type="button" value="删除"></td>
      </tr>
      -->
    </table>
  </div>
  </div>

  <div id="add">
      <div><span id="addmsg"></span></div>
      <div id="addcen">
          姓名:<input type="text" id="name"></br>
          年龄:<input type="text" id="age"></br>
          手机号码:<input type="text" id="phonenum"></br>
          地址:<input type="text" id="address"></br>
          <input type="button" value="保存" onclick="saveuser();">
          <input type="button" value="取消" onclick="canadd();">
      </div>
  </div>
  <div id="update">
      <div id="updatecen">
          用户名:<input type="text" id="uname"></br>
          年龄:<input type="text" id="uage"></br>
          手机号码:<input type="text" id="uphonenum"></br>
          地址:<input type="text" id="uaddress"></br>
          <input type="button" value="保存" onclick="updateuser();">
          <input type="button" value="取消" onclick="canup();">
      </div>
  </div>
  <div id="updates">
      <div id="updatescen">
          用户名:<input type="text" id="uuname"></br>
          年龄:<input type="text" id="uuage"></br>
          手机号码:<input type="text" id="uuphonenum"></br>
          地址:<input type="text" id="uuaddress"></br>
          <input type="button" value="保存" onclick="updatesuser();">
          <input type="button" value="取消" onclick="cansup();">
      </div>
  </div>
  <div id="anode">
      <div id="addnode">
          <div><span id="msg"></span></div>
          <div>
              节点名:<input type="text" id="nodename"></br>
              <input type="button" value="保存" onclick="nodeadd();">
              <input type="button" value="取消" onclick="cannode();">
          </div>
      </div>
  </div>
  <div id="updatenode">
      <div id="nodeupdate">
          <div><span id="upmsg"></span></div>
          <div>
              节点名:<input type="text" id="upnodename"></br>
              <input type="button" value="保存" onclick="nodeupdate();">
              <input type="button" value="取消" onclick="canupnode();">
          </div>
      </div>
  </div>
  </body>
</html>
