<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>

<div class="iScroll">
    <div class="msg-b msg-b-attention" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;margin-bottom:0">
        <div class="msg-cnt">
            <p class="popInfo"><span>红色<em>*</em>为必填项</span></p>
        </div>
    </div>
<form class="form-horizontal form-fixed clearfix" style="margin-left:13px;margin-top:10px"> 
        
        <div class="legendTitle"><label class="showInfo" id="ownInfo"><span class="trigonClose trigonOpen"></span><em class="redPoint">*</em>个人信息</label></div>
        <div id="ownInfoDiv" data-open="open">
            <fieldset class="noLegend halfField">
                <div class="control-group">
                    <label class="control-label"><em>*</em>用户名：</label>
                    <div class="controls">
                        <input type="text" vtype="common" required="true" class="auto-input" maxLength="50" data-icon="true"
                            isoverflown="true" name="userInfo.strName" id="strName">
                    </div>
                </div>
                <div class="control-group">
                    <label class="control-label"><em>*</em>性别：</label>
                    <div class="controls">
                        <label><span class="check"><input name="userInfo.nsex" type="radio" value="1" checked></span><span>男</span></label>
                        <label><span class="check"><input name="userInfo.nsex" type="radio" value="0"></span><span>女</span></label>
                    </div>
                </div>
                <div class="control-group">
                    <label class="control-label"><em>*</em>所在网点：</label>
                    <div class="controls">
                        <input type="hidden" name="userInfo.netId" id="userNet_id">
                        <input type="text" required="true" class="auto-input" data-icon="true" id="userNet" 
                            isoverflown="true" name="userInfo.net" readonly="readonly">
                    </div>
                </div>
                <div class="control-group">
                    <label class="control-label"><em>*</em>用户类型：</label>
                    <div class="controls">
                        <select id="bindUser" class="auto-input" required="true" data-icon="true" isOverflown="true" name="userInfo.sysRole">
                            <option value="0">中心管理员</option>
                            <option value="1">理财经理</option>
                        </select>
                    </div>
                </div>
            </fieldset>
            <fieldset class="noLegend halfField">
                <div class="control-group">
                   <label class="control-label"><em>*</em>真实姓名：</label>
                    <div class="controls">
                        <input type="text" vtype="common" required="true" class="auto-input" maxLength="50" data-icon="true"
                            isoverflown="true" name="userInfo.strCode">
                    </div>
                </div>
                <div class="control-group">
                     <label class="control-label"><em>*</em>员工编号：</label>
                    <div class="controls">
                        <input type="text" vtype="common" required="true" class="auto-input" maxLength="50" data-icon="true"
                            isoverflown="true" name="userInfo.userNo">
                    </div>
                </div>
                 <div class="control-group">
                    <label class="control-label"><em>*</em>到期时间：</label>
                    <div class="controls">
                        <input type="text" name="userInfo.strDate" isoverflown="true" hikui="calendar" isShowClear="false" showtime="false" name="invalidDate" mindate="%y-%M-%d" class="auto-input hikui-calendar Wdate datetimepicker" hikmap="_hik_2_" readonly="readonly" value="<fmt:formatDate value='${userInfo.strDate }' pattern='yyyy-MM-dd' />" data-icon="true" required='true'>
                    </div>
                </div>
                <div class="control-group">
                    <label class="control-label"><em>*</em>角色分配：</label>
                    <div class="controls">
                        <select id="bindRole" class="auto-input" required="true" isOverflown="true" data-icon="true" name="userInfo.roleId">
                        </select>
                    </div>
                </div>
            </fieldset>
        </div>
        <div class="cf"></div>
        <div class="legendTitle otherTitle" style="display:none"><label class="showInfo" id="chooseCertAdd" data-type="add"><span class="trigonClose"></span>关联证书</label></div>
        <div id="relateCert" data-open="close">
            
        </div>
        <div class="legendTitle otherTitle"><label class="showInfo" id="chooseAuth" data-diff=""><span class="trigonClose"></span><em class="redPoint" id="authRed">*</em>配置权限</label></div>
        <div id="channelChoose" data-open="close"></div>
        <!-- <fieldset class="noLegend">
            <div class="control-group" id="certBox">
                <label class="control-label">拥有证书：</label>
                <div class="controls">
                    <span class="chooseDialog" id="chooseCertAdd">设置/修改</span>
                    <div id="certListId">
                    </div>
                </div>
            </div>
            <div class="control-group">
                <label class="control-label"><em>*</em>权限区域：</label>
                <div class="controls" id="authShow">
                    <span class="chooseDialog" id="chooseAuth" data-diff="" data-type="add" data-open="close">设置/修改</span>
                    <div id="AuthBox"></div>
                </div>
            </div>
            
        </fieldset> -->
        <div class="legendTitle otherTitle"><label class="showInfo" id="moreInfo"><span class="trigonClose"></span>更多信息</label></div>
        <div id="moreInfoDiv" data-open="close" style="display:none">
            <fieldset class="noLegend halfField">
                 <div class="control-group">
                     <label class="control-label">证件号：</label>
                    <div class="controls">
                        <input type="text" vtype="common" class="auto-input" maxLength="50" data-icon="true"
                            isoverflown="true" name="userInfo.strCredentialNo">
                    </div>
                </div>
                 <div class="control-group">
                     <label class="control-label">邮件：</label>
                    <div class="controls">
                        <input type="text" vtype="email" class="auto-input" maxLength="50" 
                        data-icon="true" isoverflown="true" name="userInfo.strEmail">
                    </div>
                </div>
                <div class="control-group">
                     <label class="control-label">备注：</label>
                    <div class="controls">
                        <input type="text" vtype="common"  class="auto-input" maxLength="200" data-icon="true"
                            isoverflown="true" name="userInfo.strComment">
                    </div>
                </div>
            </fieldset>
            <fieldset class="noLegend halfField">
                 <div class="control-group">
                    <label class="control-label">职务：</label>
                    <div class="controls">
                       <input type="text" vtype="common" class="auto-input" maxLength="50" data-icon="true"
                            isoverflown="true" name="userInfo.strDuty">
                    </div>
                </div>
                 <div class="control-group">
                     <label class="control-label">手机：</label>
                    <div class="controls">
                        <input type="text" vtype="mobile" class="auto-input" data-icon="true"
                            isoverflown="true" name="userInfo.strTel" maxLength="11" self-focus-info="请输入手机号（11数字）">
                    </div>
                </div>
            </fieldset>
        </div>
</form>
</div>
<script type="text/javascript" src="/modules/user/js/userDialog.js"></script>
<script>
    $('#bindRole').iselect({
         url: '/modules/user/getRolesList.action',
        root: "data.roleList",
        displayName: 'strName',
        valueName: 'id'
    });
</script>