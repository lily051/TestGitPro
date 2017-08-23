<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib prefix="s" uri="/struts-tags" %>
<html>
<head>
    <title>用户管理</title>
    <script src="jquery-1.9.1.js"></script>
    <link rel="stylesheet" href="css/common.css" type="text/css"/>
    <link rel="stylesheet" href="ztree/css/zTreeStyle/zTreeStyle.css" type="text/css">
    <script type="text/javascript" src="ztree/js/jquery.ztree.core.min.js"></script>
    <script type="text/javascript" src="ztree/js/jquery.ztree.excheck.min.js"></script>
    <style>
        ul{
            list-style:none
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
        var roleid;
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
                        }

                    };

                    $.fn.zTree.init($("#nodetree"), setting, zNodes);
                }

            })
        }
        function  showAll(id) {
            nodeid=id;
            $.ajax({
                url:"roleAction!findrolebynode.action",
                type:"post",
                data:{"nodeid":nodeid},
                success:function (data) {
                    console.log(data);
                    $("#roleTable").html("");
                    var str = '<tr id="rolelist"><td>用户名</td><td>用户描述</td><td>备注</td><td>角色名称</td><td>操作</td></tr>';
                    $.each(data, function(n, value){
                        str += '<tr><td>'+value.rolename+'</td><td>'+value.roledesc+'</td><td>'+value.remark+'</td><td>'+value.roles.rname+'</td><td><a href="#" onclick="update('+"'"+value.roleid+"'"+','+"'"+value.rolename+"'"+','+"'"+value.rolepwd+"'"+','+"'"+value.roledesc+"'"+','+"'"+value.remark+"'"+','+"'"+value.roles.rname+"'"+')">修改</a><a href="#" onclick="deleterole('+value.roleid+')">删除</a></td></tr>';
                    });
                    $("#roleTable").append(str);
                }
            })
        }
        function addrole() {
            showroles();
            document.getElementById("add").style.display="block";
            $("#addscen #rolename").val("");
            $("#addscen #rolepwd").val("");
            $("#addscen #roledesc").val("");
            $("#addscen #remark").val("");
        }

        function showroles(){
            $.ajax({
                url:"roleAction!findrolesbynode.action",
                type:"post",
                data:{nodeid:nodeid},
                success:function(data){
                    console.log(data);
                    $("#rolechoose").html("");
                    var str = "角色:"
                    $.each(data,function (n, value) {
                        str+="<label><input type='radio' name='roles'value='"+value.rname+"'>"+value.rname+"</label><br />"
                        console.log(value.rname);
                    })
                    $("#rolechoose").append(str);
                }
            })
        }
        function updateroles(){
            $.ajax({
                url:"roleAction!findrolesbynode.action",
                type:"post",
                data:{nodeid:nodeid},
                success:function(data){
                    console.log(data);
                    $("#updateroles").html("");
                    var str = "角色:"
                    $.each(data,function (n, value) {
                        str+="<label><input type='radio' name='roles'value='"+value.rname+"'>"+value.rname+"</label><br />"
                        console.log(value.rname);
                    })
                    $("#updateroles").append(str);
                }
            })
        }
        function updatesroles(){
            $.ajax({
                url:"roleAction!findrolesbynode.action",
                type:"post",
                data:{nodeid:nodeid},
                success:function(data){
                    console.log(data);
                    $("#updatesroles").html="";
                    var str = "角色:"
                    $.each(data,function (n, value) {
                        str+="<label><input type='radio' name='roles'value='"+value.rname+"'>"+value.rname+"</label><br />"
                        console.log(value.rname);
                    })
                    $("#updatesroles").append(str);
                }
            })
        }
        function saverole(){
            console.log(nodeid);
            var rolename = $("#rolename").val();
            var rolepwd = $("#rolepwd").val();
            var roledesc = $("#roledesc").val();
            var remark = $("#remark").val();
            var rname = $("input[type=radio]:checked").val();
            console.log(111222);
            console.log(rname);
            $.ajax({
                url:"roleAction!addrole.action",
                type:"post",
                data:{nodeid:nodeid,rolename:rolename,rolepwd:rolepwd,roledesc:roledesc,remark:remark,rname:rname},
                success:function(){
                    showAll(nodeid);
                    document.getElementById("add").style.display="none";
                }
            })
        }
        function showrole() {
            $.ajax({
                url:"roleAction!findallrole.action",
                type:"post",
                data:{},
                success:function (data) {
                    $("#roleTable").html("");
                    var str = '<tr id="rolelist"><td>用户名</td><td>用户描述</td><td>备注</td><td>角色名称</td> <td>操作</td></tr>';
                    $.each(data, function(n, value){
                        str += '<tr><td>'+value.rolename+'</td><td>'+value.roledesc+'</td><td>'+value.remark+'</td><td>'+value.roles.rname+'</td><td><a href="#" onclick="updates('+"'"+value.roleid+"'"+','+"'"+value.rolename+"'"+','+"'"+value.rolepwd+"'"+','+"'"+value.roledesc+"'"+','+"'"+value.remark+"'"+','+"'"+value.roles.rname+"'"+')">修改</a><a href="#" onclick="deleteroles('+value.roleid+')">删除</a></td></tr>';
                    });
                    $("#roleTable").append(str);
                }
            })
        }
        function deleterole(id){
            console.log(id);
            $.ajax({
                url:"roleAction!deleterole.action",
                type:"post",
                data:{"roleid":id,"nodeid":nodeid},
                success:function(data){
                    if(data.flag){
                        showAll(nodeid);
                    }else{
                        alert(data.message);
                    }

                }
            })
        }
        function deleteroles(id){
            console.log(id);
            $.ajax({
                url:"roleAction!deletesrole.action",
                type:"post",
                data:{"roleid":id},
                success:function(data){
                    if(data.flag){
                        showrole();
                    }else{
                        alert(data.message);
                    }

                }
            })
        }
        function update(id, rolename, rolepwd, roledesc, remark,rname){
            updateroles();
            roleid = id;
            document.getElementById("update").style.display="block";
            $("#updatecen #rname").val(rolename);
            $("#updatecen #rpwd").val(rolepwd);
            $("#updatecen #rdesc").val(roledesc);
            $("#updatecen #rmark").val(remark);

        }
        function updaterole(){
           // console.log(roleid);
            var rolename = $("#rname").val();
            var rolepwd = $("#rpwd").val();
            var roledesc = $("#rdesc").val();
            var remark = $("#rmark").val();
            var rname = $("input[type=radio]:checked").val();
            console.log(rname);
            console.log(2233)
            $.ajax({
                url:"roleAction!updaterole.action",
                type:"post",
                data:{nodeid:nodeid,roleid:roleid,rolename:rolename,rolepwd:rolepwd,roledesc:roledesc,remark:remark,rname:rname},
                success:function(){
                    showAll(nodeid);
                    document.getElementById("update").style.display="none";
                }
            })
        }
        function updates(id, rolename, rolepwd, roledesc, remark,rname){
            updatesroles();
            roleid = id;
            document.getElementById("updates").style.display="block";
            $("#updatescen #rrname").val(rolename);
            $("#updatescen #rrpwd").val(rolepwd);
            $("#updatescen #rrdesc").val(roledesc);
            $("#updatescen #rrmark").val(remark);

        }
        function updatesrole(){
            var rolename = $("#rrname").val();
            var rolepwd = $("#rrpwd").val();
            var roledesc = $("#rrdesc").val();
            var remark = $("#rrmark").val();
            var rname = $("input[type=radio]:checked").val();
            $.ajax({
                url:"roleAction!updaterole.action",
                type:"post",
                data:{nodeid:nodeid,roleid:roleid,rolename:rolename,rolepwd:rolepwd,roledesc:roledesc,remark:remark,rname:rname},
                success:function(){
                    showrole();
                    document.getElementById("updates").style.display="none";
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
        function cannode(){
            showAll(nodeid);
            document.getElementById("anode").style.display="none";
        }
        function cansup(){
            showrole();
            document.getElementById("updates").style.display="none";
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
    <%--<a class="orgSource" id="source" onclick="rightcontrol()" href="#">组织资源</a>--%>
    <%--<a class="userManage" id="manager" href="${pageContext.request.contextPath}?currentModelCode=${modelnum}">用户管理</a>--%>
    <a href="#" id="logout" onclick="logout();">退出</a>
</div>
<div id="subnav" style="height: 30px" >
    <a href="${pageContext.request.contextPath}?currentModelCode=${rolecode}" id="user" class="user">用户</a>
    <a href="${pageContext.request.contextPath}?currentModelCode=${rolescode}" id="role" >角色</a>
</div>
<div id="mainarea" class="out-center">
    <div id="left" class="main-left">
        <ul id="nodetree" class="ztree" style="overflow-y: scroll;overflow-x: scroll;height: 830px;">
        </ul>
    </div>
    <div id="right" class="main-right">
        <input type="button" value="添加" onclick="addrole();">
        <%--<input type="button" value="查询" style=" background: olivedrab" onclick="showrole();">--%>
        <table width="400" id="roleTable" border="1" cellspacing="0" cellpadding="0">
            <tr id="headtr">
                <td>用户名</td>
                <td>用户描述</td>
                <td>备注</td>
                <td>角色名称</td>
                <td>操作</td>
            </tr>
        </table>
    </div>
    <div id="add">
        <div id="addcen">
        用户名:<input type="text" id="rolename"></br>
        密码:<input type="password" id="rolepwd"></br>
        用户描述:<input type="text" id="roledesc"></br>
        备注:<input type="text" id="remark"></br>
            <div id="rolechoose"></div>
        <%--角色:<label><input name="role"type="radio"/></label><br />--%>
            <input type="button" value="保存" onclick="saverole();">
            <input type="button" value="取消" onclick="canadd();">
        </div>
    </div>
    <div id="update">
        <div id="updatecen">
            用户名:<input type="text" id="rname"></br>
            密码:<input type="password" id="rpwd"></br>
            用户描述:<input type="text" id="rdesc"></br>
            备注:<input type="text" id="rmark"></br>
            <div id="updateroles"></div>
            <input type="button" value="保存" onclick="updaterole();">
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
            用户名:<input type="text" id="rrname"></br>
            密码:<input type="password" id="rrpwd"></br>
            用户描述:<input type="text" id="rrdesc"></br>
            备注:<input type="text" id="rrmark"></br>
            <div id="updatesroles"></div>
            <input type="button" value="保存" onclick="updatesrole();">
            <input type="button" value="取消" onclick="cansup();">
        </div>
    </div>
</div>
</body>
</html>
