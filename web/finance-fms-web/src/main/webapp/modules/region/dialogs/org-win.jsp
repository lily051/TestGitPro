<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<div>
<form class="form-horizontal form-fixed">
        <div class="msg-b msg-b-attention" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;">
            <div class="msg-cnt">
                <span>红色<em>*</em>为必填项</span>
            </div>
        </div>
        <fieldset class="noLegend">
        <input type="hidden" name="regionInfo.nparentId" value="${regionInfo.nparentId}"/>
        <input type="hidden" name="regionInfo.id" value="${regionInfo.id}"/>
        <input type="hidden" name="regionInfo.strInterAreaCode" value="${regionInfo.strInterAreaCode}"/>
            <div class="control-group parentReg">
                <label class="control-label">父组织：</label>
                <div class="controls">
                    <span title="${regionInfo.nparentName}" class="notEdit">${regionInfo.nparentName}</span>
                </div>
            </div>
            <div class="control-group">
                <label class="control-label"><em>*</em>组织名称：</label>
                <div class="controls">
	                <input type="text" vtype="common" maxLength="50" required="true" class='auto-input' data-icon="true" isoverflown="true" name="regionInfo.strName" value="${regionInfo.strName}"> 
                </div>
            </div>
            <div class="control-group">
                <label class="control-label"><em>*</em>组织代码：</label>
                <div class="controls">
                    <input type="text" vtype="common" maxLength="50" required="true" class='auto-input' data-icon="true" isoverflown="true" name="regionInfo.strCode" value="${regionInfo.strCode}"> 
                </div>
            </div>
            <div class="control-group">
                <label class="control-label">备注：</label>
                <div class="controls">
                	<textarea name="regionInfo.strTxt" style="height:60px" class="auto-input" type="text" not-validator-paste=false maxLength="200" vtype="common" isoverflown="true" data-icon="true" style="_width:300px"><s:property value="regionInfo.strTxt"/></textarea>            
           		</div>
           	 </div>
        </fieldset>
</form>
</div>