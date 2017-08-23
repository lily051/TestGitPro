<%--
  Created by IntelliJ IDEA.
  User: churui
  Date: 2017/8/10
  Time: 15:01
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>Title</title>
    <script src="jquery-1.9.1.min.js"></script>
    <link rel="stylesheet" href="ztree/css/zTreeStyle/zTreeStyle.css" type="text/css">
    <script type="text/javascript" src="ztree/js/jquery.ztree.core.min.js"></script>
    <script type="text/javascript" src="ztree/js/jquery.ztree.excheck.min.js"></script>
    <script>
        window.onload=function(){
            showtree();
        }
        function showtree(){
            var zTree;
            var setting = {
            view: {
                dblClickExpand: false,//双击节点时，是否自动展开父节点的标识
                showLine: false,//是否显示节点之间的连线
                fontCss:{'color':'black','font-weight':'bold'},//字体样式函数
                selectedMulti: false, //设置是否允许同时选中多个节点
                showIcon: false
            },
//            check:{
//                //chkboxType: { "Y": "ps", "N": "ps" },
//                chkStyle: "checkbox",//复选框类型
//                enable: true //每个节点上是否显示 CheckBox
//            },
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
                    zTree = $.fn.zTree.getZTreeObj("user_tree");
                    if (treeNode.isParent) {
                        zTree.expandNode(treeNode);//如果是父节点，则展开该节点
                    }else{
                        zTree.checkNode(treeNode, !treeNode.checked, true, true);//单击勾选，再次单击取消勾选
                    }
                }
            }
            };
            var zNodes =[
                { id:1, pId:0, name:"test 1", open:true},
                { id:11, pId:1, name:"test 1-1", open:true},
                { id:111, pId:11, name:"test 1-1-1"},
                { id:112, pId:11, name:"test 1-1-2"},
                { id:12, pId:1, name:"test 1-2", open:true}
            ];
            $.fn.zTree.init($("#user_tree"), setting, zNodes);
        }
    </script>
</head>
<body>
<div class="zTreeDemoBackground left">
    <ul id="user_tree" class="ztree" style="border: 1px solid #617775;overflow-y: scroll;height: 842px;"></ul>
</div>
</body>
</html>
