<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>

<div class="iScroll">
	<div class="treeview hidden">
	    <ul id="productTree" class="ztree"></ul>
	</div>
	<div class="wordTemp hidden">
		<textarea id="wordTemp" style="width:448px;height:280px;" readonly="readonly">${tip}</textarea>
	</div>
</div>

<script>
	$(function(){
		FMS.dataBusiness.initProductTree();
	});
</script>