<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>

<div class="datatableStyle" style="width:680px;margin-left:20px;top:20px;">
	<table id="videoPlanDatatable"></table>
</div>

<script type="text/javascript">
	$(function(){
		FMS.device.videoIssueGrid();
	});
</script>