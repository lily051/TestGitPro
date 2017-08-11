<%@ page language="java" contentType="text/html; charset=UTF-8" %>
<%
	String platformStr = (String)request.getSession().getAttribute("platformStr");
	String skin = (String)request.getSession().getAttribute("skin");
%>
<!-- 该jsp引入平台扩展UI框架功能的css与js文件 -->
<link rel="stylesheet" type="text/css" href="${ctx}/css/base.css">
<link rel="stylesheet" type="text/css" href="${ctx}/css/icon.css">
<!--[if lte IE 6]>
	<link  href="/css/ie6.css" rel="stylesheet" />
<![endif]-->
<%-- <script type="text/javascript" src="${ctx}/ivms-js/financeui/messages_<%=request.getSession().getAttribute("Locale-KEY")%>.js"></script> --%>

<script src="${ctx}/financeui/js/ivms-js-override.js"></script>
<script src="${ctx}/financeui/js/utils/FinanceUtil.js"></script>
<script src="${ctx}/financeui/js/utils/constants.js"></script>
<script src="${ctx}/financeui/js/plugins/ivms-plugins.js"></script>
<script src="${ctx}/financeui/js/sea/sea-override.js"></script>
<script src="${ctx}/js/Constants.js"></script>
<script src="${ctx}/js/ocxversion.js"></script>
<script src="${ctx}/download/ocxversion.js"></script>
<script src="${ctx}/js/version.js"></script>
<script src="${ctx}/js/ocxVersion/initOCX.js"></script>
<script src="/js/sha256.js"></script>
<script src="${ctx}/financeui/js/md5.js"></script>
<script src="${ctx}/js/language/message_zh_cn.js"></script>
<script src="${ctx}/js/globalsetting.js"></script>
<script src="${ctx}/js/pwdVerification.js"></script>
<!--[if lte IE 6]>
	<script src="${ctx}/js/ie6/DD_belatedPNG.js"></script>
	<script type="text/javascript">  
    	DD_belatedPNG.fix('span.managerOpr,a.popClose,.navbarBrand,li.sys span,img.iconArrow,span.icon,.filtrateIcon,span.iconBox');  
    </script> 
<![endif]-->
<!-- <script src="/js/jquery.pngFix.pack.js"></script> -->