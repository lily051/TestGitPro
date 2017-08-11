<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<head>
    <meta charset="utf-8">
    <%@ include file="/common/commons.jsp" %>
    <%@ include file="/common/commons-extend.jsp" %>
    <link  href="/modules/auth/css/auth.css" rel="stylesheet"></link>
    <link  href="/modules/video/css/video.css" rel="stylesheet" />
    <script type="text/javascript" src="/modules/log/js/log.js"></script>
</head>
<style>
  .special{
    display: block;
    height:40px;
    line-height: 40px;
    border-bottom: 1px solid #DBDBDB;
    color: #333333;
    cursor: pointer;
    font-size: 14px;
  }
  .special span{
    float:left;
  }
  .special:hover,.special.active{
    font-weight: bold;
    color:#3F85E4;
  }
  .special .vertical1{
    float:left;
    width:5px;
    height:100%;
  }
  .special.active .vertical1{
    background: #3F85E4;
  }
  .active .vertical{
    background: #3F85E4;
  }
  .iconBox{
    margin-left:35px;

  }
  .upDown{
    float:right !important;
    width:26px;
    height:16px;
    margin-top:12px;
    margin-right:5px;
    background: url('/images/icons/upDown.png') no-repeat;
  }
  .upDownOn{
    background: url('/images/icons/upDown_active.png') no-repeat;
  }
  /* .special:hover .upDown{
    background-position: 0 -16px;
  }
  .special:active .upDown,.special:focus .upDown{
    background-position: 0 -32px;
  }
  .special.active .upDown{
    background-position: 0 -16px;
  } */
  .specal.active span.recordLogIcon,.special:hover span.recordLogIcon{
   background: url("/images/icons/recordLog_active.png") no-repeat 0px center;
  }
</style>
<body>
    <div id="wrapper">
    <%@ include file="/common/commons-businessmenu.jsp" %>
        <div id="content" class="layout">
            <div class="container wrapper grid-s9m0 FMS_container">
                <div class="col-main " id="main-wrap"> 
                    
                </div>
                 <div class="col-sub"> 
                    <div class="accordion accordion-border sideBarBg">
                        <div class="subView" style="height:100%">    
                             <s:iterator value="getSession().getAttribute('menus')" id="menu" status="status">
                                <s:if test="#menu.menuId==menuCode">
                                    <s:iterator value="#menu.children" id="child">
                                    <s:if test="#child.url=='/web/recordLog.action'">
                                     <label data-url="/web/recordLog.action" class="special recordLog">
    		                                <span class="vertical1"></span>
    		                                <span class="iconBox recordLogIcon"><i>回放日志</i></span>
    		                                <span class="upDown"></span>
		                                </label>
		                              <div id="tree" style="padding-top:10px;background:white;border-bottom: 1px solid #DBDBDB;overflow:hidden">
  		                                <div class="header-search" style="margin-top:0;">
  		                                    <span class="treeDelete"></span>
  		                                    <span class="icon iconSearch" id="orgTreeSearch"></span>
  		                                    <input id="keyValue" name="tree" type="text" placeholder="搜索" class="searchInput form-control treeSearch" value="">
  		                                    <a aria-disabled="false" title="刷新" class="orgLoadIcon"><span class="icon iconReload reload"></span></a>
  		                                </div>
  		                                <div id="treefullfit" style="box-sizing:border-box">
  		                                    <div class="treeview">
  		                                        <ul id="orgTree" class="ztree"></ul>
  		                                    </div>
  		                                </div>
		                              </div>
                                    </s:if>
                                    <s:else>                                  
                                     <label data-url="<s:property value="#child.url"/>" class="special">
                                        <span class="vertical1"></span>
                                        <span class="iconBox serviceIcon_${child.menuId}"><i><s:property value="#child.name"/></i></span>
                                     </label>
                                    </s:else>
                                    </s:iterator>
                                </s:if>
                            </s:iterator>	     
                        </div>
                        
                    </div>
                </div> 
            </div>
        </div>
        <div id="footer"></div>
    </div>
</body>
</html>