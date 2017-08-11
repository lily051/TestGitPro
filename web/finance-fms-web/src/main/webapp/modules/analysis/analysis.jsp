<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<head>
    <meta charset="utf-8">
    <%@ include file="/common/commons.jsp" %>
    <%@ include file="/common/commons-extend.jsp" %>
    <link  href="/modules/auth/css/auth.css" rel="stylesheet"></link>
    <link  href="/modules/video/css/video.css" rel="stylesheet"></link>
    <script type="text/javascript" src="/js/echarts/echarts-all.js"></script>
    <script type="text/javascript" src="/modules/analysis/js/analysis.js"></script>
</head>

<body>
    <div id="wrapper">
    <%@ include file="/common/commons-businessmenu.jsp" %>
        <div id="content" class="layout">
            <div class="container wrapper grid-s9m0 FMS_container">
                <div class="col-main " id="main-wrap"> 
                    
                </div>
                 <div class="col-sub"> 
                    <jsp:include page="/common/common-tree.jsp" flush="true" >
                      <jsp:param name="className" value="orgTree" />
                    </jsp:include>
                </div> 
            </div>
        </div>
        <div id="footer"></div>
    </div>
</body>
</html>