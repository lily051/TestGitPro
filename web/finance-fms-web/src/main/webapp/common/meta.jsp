<%@ page language="java" contentType="text/html; charset=UTF-8" %>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<c:set var="ctx" value="${pageContext.request.contextPath}" />
<c:if test="${param._debug=='true'}">
    <s:set name="debug" value="true" scope="session"/>
</c:if>
<s:if test="#session.debug==true">
    <s:set name="debug" value="'true'" scope="request"/>
</s:if>
<s:else>
    <s:set name="debug" value="'false'" scope="request"/>
</s:else>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1" />
<meta http-equiv="pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">

<script type="text/javascript">

	var pt = {
		ctx : "${ctx}",
		debug : '${debug}' == 'true' ? true : false,
		lang : 'zh_cn',
		history : "${param['_history']}" == "true" ? true : false
	},
	//平台信息参数

	platParam = {
		nplatformDiscribe : "${Sysconfig_6000}",//平台描述文字
		nplatformCopyright : "${Sysconfig_6001}",//平台版权文字
		nimgBar : "${Sysconfig_6002}",//导航栏logo
		nimgLogo : "${Sysconfig_6003}",////登录页LOGO图标
		nimgLogin : "${Sysconfig_6004}"//登录页图片
	},
	//模式参数 
	modeParam = {
		usbAndIpc : "${Sysconfig_6201}", //usb还是ipc 1:IPC 2:USB
		productReview : "${Sysconfig_6202}", //启用禁用产品复核
		osdEnable:"${Sysconfig_6203}" ,//osd是否叠加  0不叠加  1 叠加 
		maxVideo:"${Sysconfig_6100}" ,// 最大录像时长 分钟 
		recordBandwidth:"${Sysconfig_6102}",  // 录像回放带宽
		nonCenterStorage:"${Sysconfig_6207}",  // 无中心存储
		channelIpBind: "${Sysconfig_6206}"  // 通道IP绑定，即 IP限制
	};
	
</script>
<title>${Sysconfig_6000}</title>
<link href="/favicon.ico" rel="shortcut icon"/>
<link href="${ctx}/baseui/themes/default/css/reset.css" rel="stylesheet"/>
<link href="${ctx}/baseui/themes/default/css/grids.css" rel="stylesheet"/>
<link href="${ctx}/baseui/themes/default/css/ui.grid.css" rel="stylesheet"/>
<link href="${ctx}/baseui/themes/default/css/base.css" rel="stylesheet"/>
<link href="${ctx}/baseui/js/plugins/popup/skin/default/pop.css" rel="stylesheet"/>
<!--[if lt IE 9]>
    <link href="${ctx}/baseui/themes/default/css/ie.css" rel="stylesheet"/>
<![endif]-->
<%String style="default";%>
