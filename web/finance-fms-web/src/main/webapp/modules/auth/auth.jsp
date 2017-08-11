<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<head>
    <meta charset="utf-8">
    <%@ include file="/common/commons.jsp" %>
    <%@ include file="/common/commons-extend.jsp" %>
    <link  href="/modules/auth/css/auth.css" rel="stylesheet"/>
    <script type="text/javascript" src="/modules/auth/js/auth.js"></script>
</head>

<body>
    <div id="wrapper">
    <%@ include file="/common/commons-basemenu.jsp" %>
        <div id="content" class="layout">
            <div class="container wrapper grid-s9m0 FMS_container">
                <div class="col-main" id="main-wrap"> 
                    
                </div>
                 <div class="col-sub"> 
                    <div class="accordion accordion-border sideBarBg">
                        <div class="subView">
                            <ul id="sideBar" class="sideBar">
	                         <s:iterator value="getSession().getAttribute('menus')" id="menu" status="status">
                                <s:if test="#menu.menuId==menuCode">
                                    <s:iterator value="#menu.children" id="child">
                                        <li data-url="<s:property value="#child.url"/>">
                                        	<span class="vertical"></span>
                                           <span class="iconBox serviceIcon_${child.menuId}"><i><s:property value="#child.name"/></i></span>
                                        </li>
                                    </s:iterator>
                                </s:if>
                            </s:iterator>	                            
                            </ul>
                        </div>
                    </div>
                </div> 
            </div>
        </div>
        <div id="footer"></div>
    </div>
</body>
</html>