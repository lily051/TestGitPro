<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<div class="serverWin" id="serverWin">
	<s:iterator id="serverType" value="serverTypeList">
		<div data-type="${serverType.nkey}">${serverType.strValue}</div>
	</s:iterator>
</div>