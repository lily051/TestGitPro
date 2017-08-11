<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<%@ include file="/common/commons-menu-top.jsp"%>
<div id="navbar" class="layout">
	<div class="container">
		<ul class="nav">
		   <s:iterator value="getSession().getAttribute('menus')" var="menu" status="status">
			  <s:if test="#menu.nbtype == 2">
				<li data-menuCode="<s:property value="#menu.menuId"/>">
					<a href="<s:property value="#menu.url"/>" target="_self"><span><s:property value="#menu.name"/></span></a>
				</li>
				</s:if>
		  </s:iterator>
		</ul>
	</div>
</div>		 
<script type="text/javascript">
	var href = window.location.pathname;
	$("#navbar ul.nav li").find("a[href='"+href+"']").parent().addClass('active');
	$("#navbarBusiness").addClass("active");
</script>