<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<head>
    <meta charset="utf-8">
	<%@ include file="/common/commons.jsp" %>
	<%@ include file="/common/commons-extend.jsp" %>
	<link  href="/modules/region/css/region.css" rel="stylesheet" />
	<script type="text/javascript" src="/modules/region/js/region.js"></script>
    <style>
        body { behavior: url("csshover.htc"); }   
    </style>
</head>
<body>
    <div id="wrapper">

        <!-- Top begins -->
		<%@ include file="/common/commons-basemenu.jsp"%>
		<!-- Top ends -->

        <div id="content" class="layout">
            <div class="container wrapper grid-s9m0 CVTMS_container FMS_container">
                <div class="col-main"> 
                    <div class="main-wrap wrapFixed">
                        <div class="fullfit">
                            <div class="toolbar">
                                <a href="#"><span class="managerOpr oprAdd oprAddReg"><i>添加</i></span></a>
                                <a href="#"><span class="managerOpr oprDelete oprDeleteReg"><i>删除</i></span></a>
                                <a href="#"><span class="managerOpr oprImport"><i>导入</i></span></a>
                                <a href="#"><span class="managerOpr oprExport"><i>导出</i></span></a>
                                <label class="showChild"><span class="check"><input type="checkbox" class="showsub" checked="checked"></span><span class="showSubStyle">显示所有子机构</span></label>
                              
                                <div class="header-search gird_search" >
                                    <input type="hidden"/> 
                                    <span class="icon iconSearch gridSearch"></span>
                                    <input id="regionGridSearch" name="keyWord" type="text" placeholder="搜索" class="searchInput form-control treeSearch" value="">
                                </div>
                            </div>
                            <div class="regionDatatable datatableStyle">
                                <table id='dataTable'></table>
                                <div id="dataTablePage"></div>
                            </div>
                        </div>
                    </div>
                </div> 
                <div class="col-sub"> 
                   <jsp:include page="/common/common-tree.jsp" flush="true" >
                       <jsp:param name="className" value="orgTree" />
                   </jsp:include>
                </div>
            </div>
        </div>
    </div>
</body>
</html>


