<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<head>
    <meta charset="utf-8">
	<link  href="/modules/user/css/user.css" rel="stylesheet"></link>
	<script type="text/javascript" src="/modules/user/js/user.js"></script>
    <script type="text/javascript" src="/modules/user/js/userConfig.js"></script>
</head>
<body>
  <div class="main-wrap wrapFixed">
      <div class="fullfit">
            <div class="toolbar">
                <a href="#"><span class="managerOpr oprAdd oprAddUser"><i>添加</i></span></a>
                <a href="#"><span class="managerOpr oprDelete oprDeleteUser"><i>删除</i></span></a>
                <a href="#"><span class="managerOpr oprAble oprAbleUser"><i>启用</i></span></a>
                <a href="#"><span class="managerOpr oprDisable oprDisableUser"><i>禁用</i></span></a>
                <a href="#"><span class="managerOpr oprImport"><i>导入</i></span></a>
                <a href="#"><span class="managerOpr oprExport"><i>导出</i></span></a>
                <a href="#"><span class="managerOpr oprReset oprResetUser"><i>重置密码</i></span></a>
                <span class="filtrateIcon" id="filtrate" data-mutex="close"></span>
            </div>
            <div class="filterFormBox ieStyle" style="margin:0;height:0;" id="filterFormBox">
                <form class="form-horizontal clearfix" id="filterForm">
                    <fieldset class="noLegend fieldset_25">
                        <div class="control-group">
                            <label class="control-label">用户名：</label>
                            <div class="controls conditions">
                                <input type="text" name="user" class='username' value="" id="username">
                            </div>
                        </div>
                    </fieldset>
                    <fieldset class="noLegend fieldset_25">
                        <div class="control-group">
                            <label class="control-label">员工编号：</label>
                            <div class="controls conditions">
                                <input type="text" name="staffNo" class='staffNo' value="" id="staffNo">
                            </div>
                        </div>
                    </fieldset>
                     <fieldset class="noLegend fieldset_25">
                        <div class="control-group">
                            <label class="control-label">网点：</label>
                            <div class="controls conditions">
                                <input type="text" name="dept" class='dept' value="" id="dept">
                            </div>
                        </div>
                    </fieldset>
                    <div class="filterDIV filterBtn">
                        <span class="btn btn-submit blockBtn" id="search">查询</span>
                        <span class="btn btn-cancel blockBtn btnGap" id="clear">重置</span>
                    </div>
                </form>
            </div>
            <div class="datatableStyle">
                <table id='userDataTable'></table>
                <div id="userDataTablePage"></div>
            </div>
      </div>
  </div>
</body>
</html>


