<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<%@ page import="com.hikvision.finance.fms.common.util.Constants"%>
<html lang="zh">
<head>
    <meta charset="utf-8">
    <%@ include file="/common/commons.jsp" %>
	<%@ include file="/common/commons-extend.jsp" %>
	
	
	<link  href="/modules/product/css/product.css" rel="stylesheet" />
	<script type="text/javascript" src="/modules/product/js/certManage.js"></script>
	<script type="text/javascript" src="/modules/product/js/product.js"></script>
</head>
<body>
	<div id="wrapper">
		<%@ include file="/common/commons-basemenu.jsp"%>
		<div id="content" class="layout">
			<div class="container wrapper grid-s9m0 FMS_container">
				<div class="col-main">
					<div class="main-wrap wrapFixed">
						<div class="fullfit">
							<div class="toolbar clearfix">
								<a href="#"><span class="managerOpr oprAdd oprAddPro"><i>添加</i></span></a>
<!-- 								<a href="#"><span class="managerOpr oprDelete oprDeletePro"><i>删除</i></span></a> -->
								<a href="#"><span class="managerOpr oprAble oprAbleUser" id="productAble"><i>启用</i></span></a>
								<a href="#">
									<span class="managerOpr oprDisable oprDisableUser" id="productDisable"><i>禁用</i></span></a>
								<a href="#"><span class="managerOpr oprImport"><i>导入</i></span></a>
								<a href="#"><span class="managerOpr oprExport"><i>导出</i></span></a>
								<a href="#"><span class="managerOpr oprAssign"><i>指定复核员</i></span></a>
								<a href="#"><span class="managerOpr oprCertificate"><i>证书管理</i></span></a>
								<span class="filtrateIcon" id="filtrate" data-mutex="close"></span>
								<!-- <div class="header-search gird_search">
									<input type="hidden"/> 
									<button class="btn btn-link gridSearch" data-name="vehiclesDataT"><span class="icon iconSearch"></span></button>
									<input id="gridSearch" name="keyWord" type="text" placeholder="搜索" class="searchInput form-control" value="">
								</div> -->
							</div>
							<div class="filterFormBox ieStyle" style="margin:0;height:0;" id="filterFormBox">
							    <form class="form-horizontal clearfix" id="filterForm">
						    		<fieldset class="noLegend fieldset_25">
						    		    <div class="control-group">
						    		        <label class="control-label">产品名称：</label>
						    		        <div class="controls conditions">
						    		            <input type="text" name="productName" class='username' value="" id="productName">
						    		        </div>
						    		    </div>
						    		    <div class="control-group">
						    		        <label class="control-label">启用状态：</label>
						    		        <div class="controls conditions">
						    		             <select name="enableStatus" id="enableStatus">
							                        <option value="">全部</option>
							                        <option value="1">启用</option>
							                        <option value="0">禁用</option>
							                    </select>
						    		        </div>
						    		    </div>
						    		</fieldset>
						    		<fieldset class="noLegend fieldset_25">
						    		    <div class="control-group">
						    		        <label class="control-label">产品编码：</label>
						    		        <div class="controls conditions">
						    		            <input type="text" name="productCode" vtype="" class='staffNo' value="" id="productCode">
						    		        </div>
						    		    </div>
						    		    <div class="control-group">
						    		        <label class="control-label">风险等级：</label>
						    		        <div class="controls conditions">
						    		            <select name="riskLevel" id="riskLevel">
						    		            	<option value="">全部</option>
							                    	<option value="1"><%=Constants.RiskLevel.RISKLEVEL_ONE%></option>
													<option value="2"><%=Constants.RiskLevel.RISKLEVEL_TWO%></option>
													<option value="3"><%=Constants.RiskLevel.RISKLEVEL_THREE%></option>
													<option value="4"><%=Constants.RiskLevel.RISKLEVEL_FOUR%></option>
													<option value="5"><%=Constants.RiskLevel.RISKLEVEL_FIVE%></option>
													<option value="6"><%=Constants.RiskLevel.RISKLEVEL_SIX%></option>
													<option value="0"><%=Constants.RiskLevel.RISKLEVEL_NONE%></option>
							                    </select>
						    		        </div>
						    		    </div>
						    		</fieldset>
							        <div class="filterDIV filterBtn filterBtnForCol2">
							            <span class="btn btn-submit blockBtn" id="search">查询</span>
							            <span class="btn btn-cancel blockBtn btnGap" id="clear">重置</span>
							        </div>
							    </form>
							</div>
 							<div class="datatableStyle">
								<table id="dataTable"></table>
								<div id="dataTablePage"></div>
							</div>
						</div>
					</div>
				</div>
				<div class="col-sub">
					<div class="accordion accordion-border">
						<div class="subSearch fullfit">
							<div class="header-search">
								<span class="treeDelete"></span>
							    <span class="icon iconSearch" id="treeSearch"></span>
							    <input id="keyValue" name="tree" type="text" placeholder="搜索" class="searchInput form-control treeSearch" value="">
							    <a aria-disabled="false" title="刷新" class="orgLoadIcon"><span class="icon iconReload reload"></span></a>
							</div>
							<div class="header-search" style="margin-top:0;padding-bottom:5px;border:0px;border-bottom:solid 1px #DCDCDC;background:#ECECEC;">
								<span class="icon addProType" title="添加产品类型"></span>
								<span class="icon editProType" title="编辑产品类型"></span>
								<span class="icon delProType" title="删除产品类型"></span>
							</div>
							<div class="fullfit" style="padding-bottom:46px;box-sizing:border-box">
							    <div class="treeview">
							        <ul id="productTree" class="ztree"></ul>
							    </div>
							</div>
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>
</body>
</html>


