<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<div >
<form class="form-horizontal form-fixed">
        <div class="msg-b msg-b-attention msg-b-4" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;">
            <div class="msg-cnt">
                <span>红色<em>*</em>为必填项</span>
            </div>
        </div>
        <fieldset class="noLegend">
            <div class="control-group">
                <label class="control-label"><em>*</em>角色名称：</label>
                <div class="controls">
                    <input id="roleName" type="text" vtype="common" required="true" class="auto-input" maxLength="50" data-icon="true" isoverflown="true">
                </div>
            </div>
            <!-- <div class="control-group">
                <label class="control-label"><em>*</em>角色类型：</label>
                <div class="controls">
                    <select name="" id="roleType">
                        <option value="1">理财经理</option>
                        <option value="0">中心管理员</option>
                    </select>
                </div>
            </div> -->
            <div class="control-group">
                <label class="control-label"><em>*</em>角色描述：</label>
                <div class="controls">
                    <input id="strRemarks" type="text" vtype="common" required="true" class="auto-input" maxLength="50" data-icon="true" isoverflown="true">
                </div>
            </div>
            <div class="control-group">
                <label class="control-label"><em>*</em>权限配置：</label>
                <div class="permissionBox">
                    <div class='tab-pane' id='tabFunction'>
                        <ul id="functionTree" class="ztree"></ul>
                    </div> 
                </div>
            </div>
        </fieldset>
</form>
</div>
<script>
	addRoleInitPage();
</script>