<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<head>
    <meta charset="utf-8">
    <title>理财风险管控系统IVMS-8000</title>
	<link  href="/modules/role/css/role.css" rel="stylesheet"></link>
	<script type="text/javascript" src="/modules/role/js/role.js"></script>
	<script type="text/javascript" src="/modules/role/js/roleConfig.js"></script>
</head>
<body>
   <div class="main-wrap wrapFixed">
       <div class="fullfit">
           <div class="toolbar">
               <a href="#"><span class="managerOpr oprAdd oprAddRole"><i>添加</i></span></a>
               <a href="#"><span class="managerOpr oprDelete oprDeleteRole"><i>删除</i></span></a>
               <a href="#"><span class="managerOpr oprCopy oprCopyRole"><i>复制</i></span></a>
               <div class="header-search gird_search">
                   <input type="hidden" /> 
                   <span class="icon iconSearch gridSearch"></span>
                   <input id="roleGridSearch" name="keyWord" type="text" placeholder="搜索" class="searchInput form-control treeSearch" value="">
               </div>
           </div>
           <div class="datatableStyle">
               <table id='roleDataTable'></table>
               <div id="roleDataTablePage"></div>
           </div>
       </div>
   </div>
</body>
</html>


