<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<head>
    <meta charset="utf-8">
    <%@ include file="/common/commons.jsp" %>
	<%@ include file="/common/commons-extend.jsp" %>
	<link  href="/modules/video/css/video.css" rel="stylesheet" />
	<script src="/modules/duerecord/js/duerecord.js"></script>
</head>
<body>
	<div id="wrapper">
		<%@ include file="/common/commons-businessmenu.jsp"%>
		<div id="content" class="layout">
			<div class="container wrapper grid-s9m0 FMS_container">
				<div class="col-main">
					<div class="main-wrap wrapFixed">
						<div class="fullfit">
							<div class="filterFormBox ieStyle" style="margin-top:20px">
							    <form class="form-horizontal" id="filterForm">
							        <fieldset class="noLegend fieldset_25">
								    	<div class="control-group">
								    		<label class="control-label">录像状态：</label>
								    		<div class="controls conditions">
								    			<select name="nisPay" id="nisPay">
								    				<option value="">全部</option>
								    				<option value="1">已到期</option>
								    				<option value="0">未到期</option>
								    			</select>
								    		</div>
								    	</div>
								    	<div class="control-group">
								    		<label class="control-label">客户姓名：</label>
								    		<div class="controls conditions">
								    			<input type="text" class="auto-input" name="" id="name">
								    		</div>
								    	</div>
								    </fieldset>
								    <fieldset class="noLegend fieldset_25">
								    	<div class="control-group">
								    		<label class="control-label">业务单号：</label>
								    		<div class="controls conditions">
								    			<input type="text" class="auto-input" name="" id="nproId">
								    		</div>
								    	</div>
								    	<div class="control-group">
								    		<label class="control-label">产品名称：</label>
								    		<div class="controls conditions">
								    			<input type="text" class="auto-input" name="" id="productName">
								    		</div>
								    	</div>
								    </fieldset>
								    <fieldset class="noLegend fieldset_25">
								    	<div class="control-group">
								    		<label class="control-label">业务员姓名：</label>
								    		<div class="controls conditions">
								    			<input type="text" class="auto-input" name="" id="proName">
								    		</div>
								    	</div>
								    </fieldset>
							        <div class="filterDIV filterBtn">
							            <span class="btn btn-submit blockBtn" id="search">查询</span>
							            <span class="btn btn-cancel blockBtn btnGap" id="clear">重置</span>
							        </div>
							    </form>
							</div>
							<div class="toolbar" style="padding:5px 0">
								<a href="#"><span class="managerOpr oprDelay"><i>延时处理</i></span></a>
								<a href="#"><span class="managerOpr oprSignRead"><i>标记为已读</i></span></a>
								<a href="#"><span class="managerOpr oprExport"><i>导出</i></span></a>
								<label class="showChild"><span class="check"><input type="checkbox" class="showsub" checked="checked"></span><span class="showSubStyle">显示所有子机构</span></label>
							</div>
							<div class="datatableStyle" style="top:154px">
								<table id="dataTable"></table>
								<div id="dataTablePage"></div>
							</div>
						</div>
					</div>
				</div>
				<div class="col-sub">
					<jsp:include page="/common/common-tree.jsp" flush="true" >
						<jsp:param name="className" value="orgTree" />
					</jsp:include>
				</div>
			</div>
		</div>
	</div>
</body>
</html>


