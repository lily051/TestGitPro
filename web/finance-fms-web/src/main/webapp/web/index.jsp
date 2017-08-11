<!DOCTYPE HTML>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html>
<head>
	<meta charset="utf-8">
	<%@ include file="/common/commons.jsp" %>
	<%@ include file="/common/commons-extend.jsp" %>
	<link  href="/css/base.css" rel="stylesheet" />
	<link  href="/css/index.css" rel="stylesheet" />
	<script type="text/javascript" src="/js/echarts/echarts-all.js"></script>
	<script type="text/javascript" src="/js/index.js"></script>
	<!--[if lte IE 6]>
       <style type="text/css">
	        body{
				 behavior:url("/css/csshover.htc");
			}
       </style>
	<![endif]-->
</head>
<body>
	<div id="wrapper">
		<%@ include file="/common/commons-menu-top.jsp"%>
		<div id="content" class="layout" style="top:70px;min-height:830px;_height:830px">
			<div class="container wrapper grid-s9m0" id="content-inner" style="position:absolute;left:50%;margin-left:-640px;top:0;bottom:0">
				<div class="col-main">
					<div class="home-wrap">
						<div class="module home-wrap-module">
							<div class="header">功能导航</div>
							<div class="content">
								<div class="info"><span>业务应用</span><div class="dash"></div></div>
								<div class="linkIcon">
									<s:iterator value="getSession().getAttribute('menus')" var="menu" status="status">
										<s:if test="#menu.url == '/web/record.action'">
                                              <a href="/web/record.action" class="linkIcon_a"><img src="/images/index/video.png" data-src="/images/index/video_hover.png">
                                              <div><s:property value="#menu.name"/></div></a>
										</s:if>
										<s:if test="#menu.url == '/web/sale.action'">
                                             <a href="/web/sale.action" class="linkIcon_a"><img src="/images/index/sale.png" data-src="/images/index/sale_hover.png">
                                             <div><s:property value="#menu.name"/></div></a>
										</s:if>
									    <s:if test="#menu.url == '/web/analysis.action'">
                                             <a href="/web/analysis.action" class="linkIcon_a"><img src="/images/index/statistic.png" data-src="/images/index/statistic_hover.png">
                                             <div><s:property value="#menu.name"/></div></a>
										</s:if>
										<s:if test="#menu.url == '/web/alarm.action'">
                                             <a href="/web/alarm.action" class="linkIcon_a"><img src="/images/index/alarm.png" data-src="/images/index/alarm_hover.png">
                                             <div><s:property value="#menu.name"/></div></a>
										</s:if>
										<s:if test="#menu.url == '/web/duerecord.action'">
                                             <a href="/web/duerecord.action" class="linkIcon_a"><img src="/images/index/expire.png" data-src="/images/index/expire_hover.png">
                                             <div><s:property value="#menu.name"/></div></a>
										</s:if>
										<s:if test="#menu.url == '/web/log.action'">
                                             <a href="/web/log.action" class="linkIcon_a"><img src="/images/index/log.png" data-src="/images/index/log_hover.png">
                                             <div><s:property value="#menu.name"/></div></a>
										</s:if>
									</s:iterator>									
								</div>
								<div class="info"><span>基础配置</span><div class="dash"></div></div>
								<div class="linkIcon">
									<s:iterator value="getSession().getAttribute('menus')"var="menu" status="status">
										<s:if test="#menu.url == '/web/reg.action'"><a href="/web/reg.action" class="linkIcon_a"><img src="/images/index/region.png" data-src="/images/index/region_hover.png">
											<div><s:property value="#menu.name"/></div></a>
										</s:if>
										<s:if test="#menu.url == '/web/server.action'"><a href="/web/server.action" class="linkIcon_a"><img src="/images/index/server.png" data-src="/images/index/server_hover.png">
											<div><s:property value="#menu.name"/></div></a>
										</s:if>
										<s:if test="#menu.url == '/web/device.action'"><a href="/web/device.action" class="linkIcon_a"><img src="/images/index/device.png" data-src="/images/index/device_hover.png">
											<div><s:property value="#menu.name"/></div></a>
										</s:if>
										<s:if test="#menu.url == '/web/product.action'"> <a href="/web/product.action" class="linkIcon_a"><img src="/images/index/product.png" data-src="/images/index/product_hover.png">
											<div><s:property value="#menu.name"/></div></a>
										</s:if>
										<s:if test="#menu.url == '/web/auth.action'"> <a href="/web/auth.action" class="linkIcon_a"><img src="/images/index/auth.png" data-src="/images/index/auth_hover.png">
											<div><s:property value="#menu.name"/></div></a>
										</s:if>
										<s:if test="#menu.url == '/web/configure.action'"><a href="/web/configure.action" class="linkIcon_a"><img src="/images/index/configure.png" data-src="/images/index/configure_hover.png">
											<div><s:property value="#menu.name"/></div></a>
										</s:if>
									</s:iterator>
								</div>
							</div>
						</div>
						<div class="module home-wrap-module module-ab home-wrap-module-ab" id="chartOut">
							<div class="header clearfix"><span style="*float:left">统计分析</span>
							<s:iterator value="getSession().getAttribute('menus')" var="menu" status="status"><s:if test="#menu.url == '/web/analysis.action'"><a style="float:right;" href="/web/analysis.action" class="moreInfo" title="更多">更多</a></s:if></s:iterator>
							</div>
							<div class="chartTitle">
								<div style="width:30%;float:left">本周签约录像检查结果</div>
								<div style="width:70%;float:right">本周各组织合规率</div>
							</div>
							<div style="width:30%;float:left;" class="indexChart">
								<div id="checkStatistic" style="width:100%;float:left;height:280px"></div>
								<div id="checkStatisticLegend" style="width:100%;float:left;height:20px;margin-top:-90px;">
									<div class="pieLegendDiv" style="margin-left:67px;_margin-left:33px"><span class="pieLegend"></span>检查</div>
									<div class="pieLegendDiv" style="color:#98CDFA"><span class="pieLegend otherColor"></span>未检查</div>
								</div>
							</div>
							<div id="complainceComp" class="indexChart" style="width:70%;float:right;"></div>
						</div>
					</div>
				</div>
				<div class="col-sub sub-wrap">
					<div class="home-wrap sub-home-wrap">
						<div class="module home-wrap-module">
                              <div class="header clearfix"><span style="_float:left">报警信息</span>
                              <s:iterator value="getSession().getAttribute('menus')" var="menu" status="status"><s:if test="#menu.url == '/web/alarm.action'"><a style="float:right;" href="/web/alarm.action" class="moreInfo" title="更多">更多</a></s:if></s:iterator>
                              </div>						
							<div class="show-info home-wrap-show-info">
								<ul id="alarmInfo" class="show-info-ul"></ul>
							</div>
						</div>
						<div class="module home-wrap-module module-ab" id="dueInfo">
							<div class="header clearfix"><span style="_float:left">到期信息</span>
							 <s:iterator value="getSession().getAttribute('menus')" var="menu" status="status"><s:if test="#menu.url == '/web/duerecord.action'"><a style="float:right;" href="/web/duerecord.action" class="moreInfo" title="更多">更多</a></s:if></s:iterator>
							</div>
							<div class="show-info home-wrap-show-info">
								<ul id="dueInfoList" class="show-info-ul"></ul>
							</div>
						</div>
					</div>
				</div>
			</div>
		</div>
		<%@ include file="/common/commons-footer.jsp" %>
	</div>
</body>
</html>
