<%--
  Created by IntelliJ IDEA.
  User: churui
  Date: 2017/8/8
  Time: 15:52
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib prefix="s" uri="/struts-tags" %>
<html>
<head>
    <title>角色</title>
    <script src="jquery-1.9.1.js"></script>
    <link rel="stylesheet" href="css/common.css" type="text/css"/>
    <link rel="stylesheet" href="ztree/css/zTreeStyle/zTreeStyle.css" type="text/css">
    <script type="text/javascript" src="ztree/js/jquery.ztree.core.min.js"></script>
    <script type="text/javascript" src="ztree/js/jquery.ztree.excheck.min.js"></script>
    <style>
     ul{
         list-style: none;
     }
        #add,#update,#anode,#updates{
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
            overflow-y:scroll;
        }
        #addcen,#updatecen,#addnode,#updatescen {
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
        #roleTable{
            background-color: #ffffff;
        }
        #roleTable td{
            border:#CCC solid 1px;
        }

        #rolelist,#headtr{
            margin-top: 5px;
            text-align: center;
            background-color: dodgerblue;
            with:100%
        }
        #rolelist td,#headtr td{
            height:32px;
            line-height: 32px;
        }
        #subnav{
            margin-left: 10px;
            border:  solid  #1C86EE;
            margin-right: 10px;
        }
        .user{
            margin-left: 20px;
        }
        input{
            autocomplete:off
        }
    </style>
    <script>
        var nodeid;
        var rid;
        var modelnum;
        showTree();
        function showTree() {
            $.ajax({
                url: "nodeAction!shownodetree.action",
                type: "post",
                success: function (data) {
                    var zTree;
                    var zNodes=data.jsonArray1;
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
                            beforeClick: function (treeId, treeNode) {
                                zTree = $.fn.zTree.getZTreeObj("nodetree");
                                nodeid = treeNode.id;
                                nodename = treeNode.name;
                                showAll(nodeid)
                            }
                        }

                    };

                    $.fn.zTree.init($("#nodetree"), setting, zNodes);
                }
            })
        }
        function  showAll(id) {
            nodeid=id;
            $.ajax({
                url:"roleAction!findrolesbynode.action",
                type:"post",
                data:{"nodeid":nodeid},
                success:function (data) {
                    $("#roleTable").html("");
                    var str = '<tr id="rolelist"><td>角色名称</td><td>角色描述</td><td>操作</td></tr>';
                    $.each(data, function(n, value){
                        str += '<tr><td>'+value.rname+'</td><td>'+value.rdesc+'</td><td><a href="#" onclick="update('+"'"+value.rid+"'"+','+"'"+value.rname+"'"+','+"'"+value.rdesc+"'"+')">修改</a><a href="#" onclick="deleteroles('+value.rid+')">删除</a></td></tr>';
                    });
                    $("#roleTable").append(str);
                }
            })
        }
        function deleteroles(id){
            rid = id;
            $.ajax({
                url:"roleAction!deleteroles.action",
                type:"post",
                data:{rid:rid},
                success:function(data){
                    if(data.flag){
                        showAll(nodeid);
                    }else{
                        alert(data.message);
                    }
                }
            })
        }
        function addrole(){
            document.getElementById("add").style.display="block";
            var htmlText = '<label><input name="right" id="allChoose" type="checkbox" value="0" />全选 </label><br />'+
                '<label><input name="right" id="orgSource" type="checkbox" value="1" />组织资源</label><br />'+
                '<label><input name="right" id="userManager" type="checkbox" value="2" />用户管理</label><br />';
            $("#addcen #rolename").val("");
            $("#addcen #roledesc").val("");
            $("#choiceGroup").html(htmlText);
            rightChoose();
        }
        function rightChoose() {
            $('#addcen input[type="checkbox"]').click(function () {
                var value = $(this).val();
                if($(this).attr("checked") == "checked") {
                    $(this).removeAttr("checked").prop("checked",false);
                }else{
                    $(this).attr({"checked":"checked"}).prop("checked",true);
                }

                if (value == 0) {
                    var flag1 = $("#allChoose").attr("checked");
                    if (flag1 == "checked") {
                        $("#orgSource,#userManager").attr({"checked":"checked"}).prop("checked",true);

                    } else {
                        $("#orgSource,#userManager").removeAttr("checked").prop("checked",false);
                    }
                } else {
                    var flag2 = $("#orgSource").attr("checked");
                    var flag3 = $("#userManager").attr("checked");
                    if (flag2 == "checked" && flag3 == "checked") {
                        $("#allChoose").attr({"checked":"checked"}).prop("checked",true);
                    }else{
                        $("#allChoose").removeAttr("checked").prop("checked",false);
                    }
                }
            });
        }
        function uprightChoose() {
            $(' #updatecen input[type="checkbox"]').click(function () {
                var value = $(this).val();
                if($(this).attr("checked") == "checked") {
                    $(this).removeAttr("checked").prop("checked",false);
                }else{
                    $(this).attr({"checked":"checked"}).prop("checked",true);
                }

                if (value == 0) {
                    var flag1 = $("#upallChoose").attr("checked");
                    if (flag1 == "checked") {
                        $("#uporgSource,#upuserManager").attr({"checked":"checked"}).prop("checked",true);

                    } else {
                        $("#uporgSource,#upuserManager").removeAttr("checked").prop("checked",false);
                    }
                } else {
                    var flag2 = $("#uporgSource").attr("checked");
                    var flag3 = $("#upuserManager").attr("checked");
                    if (flag2 == "checked" && flag3 == "checked") {
                        $("#upallChoose").attr({"checked":"checked"}).prop("checked",true);
                    }else{
                        $("#upallChoose").removeAttr("checked").prop("checked",false);
                    }
                }
            });
        }
        function saverole(){
            var rolename = $("#rolename").val();
            if(rolename == "" || rolename == undefined || rolename == null){
                alert("角色名称不能为空!");
                return false;
            }
            var roledesc = $("#roledesc").val();
            if(roledesc == "" || roledesc == undefined || roledesc == null){
                alert("角色描述不能为空!");
                return false;
            }

            var flag1 = $("#allChoose").attr("checked");
            var flag2 = $("#orgSource").attr("checked");
            var flag3 = $("#userManager").attr("checked");
            if (flag2 == undefined && flag3 == undefined && flag1==undefined){
                alert("权限选择不能为空!");
                return false;
            }
            if(flag1 == "checked"){
                modelnum = 0;
            }else{
                if(flag2 == "checked"){
                    modelnum = 1;
                }else if(flag3 == "checked"){
                    modelnum = 2;
                }

            }
            if (flag1 == "checked" && flag2 == "checked" && flag3 == "checked"){
                modelnum=0;
            }
            $.ajax({
                url:"roleAction!addroles.action",
                type:"post",
                data:{nodeid:nodeid,rname:rolename,rdesc:roledesc,modelnum:modelnum},
                success:function(data){
                    console.log(data.flag)
                    if(data.flag){
                        showAll(nodeid);
                        document.getElementById("add").style.display="none";
                    }else{
                        $("#addmsg").html(data.message);
                    }

                }
            })
        }
        function update(id,rname,rdesc){
            rid=id;
            document.getElementById("update").style.display="block";
            var htmlText = '<label><input name="right" id="upallChoose" type="checkbox" value="0" />全选 </label><br />'+
                '<label><input name="right" id="uporgSource" type="checkbox" value="1" />组织资源</label><br />'+
                '<label><input name="right" id="upuserManager" type="checkbox" value="2" />用户管理</label><br />';
            $("#updatecen #rname").val(rname);
            $("#updatecen #rdesc").val(rdesc);
            $("#upchoiceGroup").html(htmlText);
            uprightChoose();

        }
        function updateroles(){
            var rolename = $("#rname").val();
            var roledesc = $("#rdesc").val();
            var flag1 = $("#upallChoose").attr("checked");
            var flag2 = $("#uporgSource").attr("checked");
            var flag3 = $("#upuserManager").attr("checked");
            if (flag2 == undefined && flag3 == undefined && flag1==undefined){
                alert("权限选择不能为空!");
                return false;
            }
            if(flag1 == "checked"){
                modelnum = 0;
            }else{
                if(flag2 == "checked"){
                    modelnum = 1;
                }else if(flag3 == "checked"){
                    modelnum = 2;
                }

            }
            if (flag1 == "checked" && flag2 == "checked" && flag3 == "checked"){
                modelnum=0;
            }
            $.ajax({
                url:"roleAction!updateroles.action",
                type:"post",
                data:{nodeid:nodeid,rname:rolename,rdesc:roledesc,modelnum:modelnum,rid:rid},
                success:function(data){
                   if(data.flag){
                       showAll(nodeid);
                       document.getElementById("update").style.display="none";
                   }else{
                       $("#upmsg").html(data.message);
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
        function logout(){
            $.ajax({
                url:"roleAction!logout.action",
                success:function(){
                    location.href="index.jsp";
                }
            })
        }
    </script>
</head>
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
    <%--<a href="#" class="orgSource" id="source" onclick="rightcontrol()">组织资源</a>--%>
    <%--<a href="${pageContext.request.contextPath}?currentModelCode=${modelnum}" class="userManage" id="manager">用户管理</a>--%>
    <a href="#" id="logout" onclick="logout();">退出</a>
</div>
<div id="subnav" style="height: 30px" >
    <a href="${pageContext.request.contextPath}?currentModelCode=${rolecode}" id="user" class="user" >用户</a>
    <a href="${pageContext.request.contextPath}?currentModelCode=${rolescode}" id="role" >角色</a>
</div>
<div id="mainarea" class="out-center">
    <div id="left" class="main-left">
        <ul id="nodetree" class="ztree" style="overflow-y: scroll;overflow-x: scroll;height: 830px;">
        </ul>
    </div>
    <div id="right" class="main-right">
        <input type="button" value="添加" onclick="addrole();">
        <table width="400" id="roleTable" border="1" cellspacing="0" cellpadding="0">
            <tr id="headtr">
                <td>角色名称</td>
                <td>角色描述</td>
                <td>操作</td>
            </tr>
        </table>
    </div>
    <div id="add">
        <div><span id="addmsg"></span></div>
        <div id="addcen">
            角色名称:<input type="text" id="rolename"></br>
            角色描述:<input type="text" id="roledesc"></br>
            权限选择:
            <div id="choiceGroup">
                <label><input name="right" id="allChoose" type="checkbox" value="0" />全选 </label><br />
                <label><input name="right" id="orgSource" type="checkbox" value="1" />组织资源</label><br />
                <label><input name="right" id="userManager" type="checkbox" value="2" />用户管理</label><br />
            </div>
            <input type="button" value="保存" onclick="saverole();">
            <input type="button" value="取消" onclick="canadd();">
        </div>
    </div>
    <div id="update">
        <div><span id="upmsg"></span></div>
        <div id="updatecen">
            角色名称:<input type="text" id="rname"></br>
            角色描述:<input type="text" id="rdesc"></br>
            权限选择:
            <div id="upchoiceGroup">
            <label><input name="upright" id="upallChoose" type="checkbox" value="0" />全选 </label><br />
            <label><input name="upright" id="uporgSource" type="checkbox" value="1" />组织资源</label><br />
            <label><input name="upright" id="upuserManager" type="checkbox" value="2" />用户管理</label><br />
            </div>
            <input type="button" value="保存" onclick="updateroles();">
            <input type="button" value="取消" onclick="canup();">
        </div>
    </div>
    <div id="anode">
        <div id="addnode">
            节点名:<input type="text" id="nodename"></br>
            <input type="button" value="保存" onclick="nodeadd();">
            <input type="button" value="取消" onclick="cannode();">
        </div>
    </div>
    <div id="updates">
        <div id="updatescen">
            用户名:<input type="text" id="rrrname"></br>
            密码:<input type="password" id="rrpwd"></br>
            用户描述:<input type="text" id="rrdesc"></br>
            备注:<input type="text" id="rrmark"></br>
            <input type="button" value="保存" onclick="updatesrole();">
            <input type="button" value="取消" onclick="cansup();">
        </div>
    </div>
</div>
</body>
</html>
