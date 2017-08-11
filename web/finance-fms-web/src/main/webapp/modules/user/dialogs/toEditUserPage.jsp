<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ taglib prefix='fmt' uri="http://java.sun.com/jsp/jstl/fmt" %>

<div class="iScroll">
     <div class="msg-b msg-b-attention" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;">
        <div class="msg-cnt">
            <span>红色<em>*</em>为必填项</span>
        </div>
    </div>
<form class="form-horizontal form-fixed" style="margin-left:13px;margin-top:10px">
        <input type="hidden" value="${userInfo.isOneself}" id="disableEdit"><!-- 隐藏域：用于判断是否为本用户登录 -->
        <div class="legendTitle"><label class="showInfo" id="ownInfo"><span class="trigonClose trigonOpen"></span><em class="redPoint">*</em>个人信息</label></div>
        <div id="ownInfoDIv" data-open="open">
            <fieldset class="noLegend halfField">
                <div class="control-group">
                    <label class="control-label"><em>*</em>用户名：</label>
                    <div class="controls">
                        <input type="hidden" name="userInfo.id" value="${userInfo.id}" id="userInfoId"><!-- 隐藏域：用于保存id -->
                        <input type="text" vtype="common" required="true" class="auto-input" maxLength="50" data-icon="true"
                            isoverflown="true" name="userInfo.strName" value="${userInfo.strName}" id="strName" disabled>
                    </div>
                </div>
                <div class="control-group">
                    <label class="control-label"><em>*</em>性别：</label>
                    <div class="controls">
                        <label><span class="check"><input name="userInfo.nsex" type="radio" value="1" <s:if test="userInfo.nsex==1">checked</s:if>></span><span>男</span></label>
                        <label><span class="check"><input name="userInfo.nsex" type="radio" value="0" <s:if test="userInfo.nsex==0">checked</s:if>></span><span>女</span></label>
                    </div>
                </div>
                <div class="control-group">
                    <label class="control-label"><em>*</em>所在网点：</label>
                    <div class="controls">
                        <input type="hidden" name="userInfo.netId" id="userNet_id" value="${userInfo.netId}">
                        <input type="text" required="true" class="auto-input"  data-icon="true" id="userNet"
                            isoverflown="true" name="userInfo.net" value="${userInfo.net}">
                    </div>
                </div>
                <div class="control-group">
                    <label class="control-label"><em>*</em>用户类型：</label>
                    <div class="controls">
                        <select id="bindUser" class="auto-input" required="true" data-icon="true" isOverflown="true" name="userInfo.sysRole">
                            <option value="0" <s:if test="userInfo.sysRole==0">selected="selected"</s:if>>中心管理员</option>
                            <option value="1" <s:if test="userInfo.sysRole==1">selected="selected"</s:if>>理财经理</option>
                        </select>
                    </div>
                </div>
            </fieldset>
            <fieldset class="noLegend halfField">
                <div class="control-group">
                   <label class="control-label"><em>*</em>真实姓名：</label>
                    <div class="controls">
                        <input type="text" vtype="common" required="true" class="auto-input" maxLength="50" data-icon="true"
                            isoverflown="true" name="userInfo.strCode" value="${userInfo.strCode}">
                    </div>
                </div>
                <div class="control-group">
                     <label class="control-label"><em>*</em>员工编号：</label>
                    <div class="controls">
                        <input type="text" vtype="common" required="true" class="auto-input" maxLength="50" data-icon="true"
                            isoverflown="true" name="userInfo.userNo" value="${userInfo.userNo}">
                    </div>
                </div>
                <div class="control-group">
                    <label class="control-label"><em>*</em>到期时间：</label>
                    <div class="controls">
                        <input type="text" name="userInfo.strDate" isoverflown="true" hikui="calendar" isShowClear="false" name="invalidDate" 
                        mindate="%y-%M-%d" class="auto-input hikui-calendar Wdate datetimepicker" hikmap="_hik_2_" readonly="readonly" 
                       value="<fmt:formatDate value="${userInfo.strDate}" pattern="yyyy-MM-dd"/>" required="true" data-icon="true">
                    </div>
                </div>
                
                <div class="control-group">
                    <label class="control-label"><em>*</em>角色分配：</label>
                    <div class="controls">
                        <select id="bindRole" class="auto-input" required="true" data-icon="true" isOverflown="true" name="userInfo.roleId">
                        </select>
                    </div>
                </div>
            </fieldset>
        </div>
        <div class="cf"></div>
        <div class="legendTitle otherTitle" style="display:none;"><label class="showInfo" id="chooseCertAdd" data-type="edit"><span class="trigonClose"></span>关联证书</label></div>
            <s:iterator id="certListIds" value="userInfo.certName">
                <input type="hidden" value="${certListIds.id}" class="certIds">
            </s:iterator>
        <div id="relateCert" data-open="close"></div>
        <div class="legendTitle otherTitle"><label class="showInfo" id="chooseAuth" data-diff=""><span class="trigonClose"></span><em class="redPoint" id="authRed">*</em>配置权限</label></div>
            <input type="hidden" value="${userInfo.regionId}" id="regionId">
            <s:iterator id="channelNoList" value="userInfo.channelNos">
                <input type="hidden" value="${channelNoList.devChannel}" class="channelNo"> <!-- 隐藏域：用于保存上次被选信道号 -->
            </s:iterator>
        <div id="channelChoose" data-open="close"></div>

        
        <div class="legendTitle otherTitle"><label class="showInfo" id="moreInfo"><span class="trigonClose"></span>更多信息</label></div>
        <div id="moreInfoDiv" data-open="close" style="display:none">
            <fieldset class="noLegend halfField">
                <div class="control-group">
                    <label class="control-label">证件号：</label>
                    <div class="controls">
                        <input type="text" vtype="common" class="auto-input" maxLength="50" data-icon="true"
                            isoverflown="true" name="userInfo.strCredentialNo" value="${userInfo.strCredentialNo}">
                    </div>
                </div>
                 <div class="control-group">
                     <label class="control-label">邮件：</label>
                    <div class="controls">
                        <input type="text" vtype="email" class="auto-input" maxLength="50" 
                        data-icon="true" isoverflown="true" name="userInfo.strEmail" value="${userInfo.strEmail}">
                    </div>
                </div>
                <div class="control-group">
                    <label class="control-label">备注：</label>
                    <div class="controls">
                        <input type="text" vtype="common" class="auto-input" maxLength="200" data-icon="true"
                            isoverflown="true" name="userInfo.strComment" value="${userInfo.strComment}">
                    </div>
                </div>
            </fieldset>
            <fieldset class="noLegend halfField">
                 <div class="control-group">
                    <label class="control-label">职务：</label>
                    <div class="controls">
                       <input type="text" vtype="common" class="auto-input" maxLength="50" data-icon="true"
                            isoverflown="true" name="userInfo.strDuty" value="${userInfo.strDuty}">
                    </div>
                </div>
                 <div class="control-group">
                     <label class="control-label">手机号：</label>
                    <div class="controls">
                        <input type="text" vtype="mobile" class="auto-input" data-icon="true" maxLength="11" 
                            isoverflown="true" name="userInfo.strTel" value="${userInfo.strTel}" self-focus-info="请输入手机号（11数字）">
                    </div>
                </div>
                
            </fieldset>
        </div>
</form>
</div>
<script type="text/javascript" src="/js/pwdVerification.js"></script>
<script type="text/javascript" src="/modules/user/js/userDialog.js"></script>
<script>
$(function(){
    //判断用户类型，存储未修改权限数据
    var user = $('#bindUser').val();
    if(user == '0'){//中心管理员
        var resList = [];
        if(new Number($('#regionId').val()).valueOf()!=0){
            resList.push({nregionId: new Number($('#regionId').val()).valueOf()});
        }
        $('#chooseAuth').data('power',resList);
    }else if(user == '1'){//理财经理
        $("#authRed").hide();
        $("#chooseCertAdd").parent('div').show();
        if(modeParam.usbAndIpc=="usb" || (modeParam.usbAndIpc == "ipc" && modeParam.channelIpBind == "true")){
            $("#chooseAuth").parent('div').hide();
        }
        var deviceList = [];
       /* $.each($('.channelNo'),function(){
            deviceList.push({"devChannel":Number($(this).val()),"status":true});
        })*/
        data = {'deviceList':deviceList};
        $('#chooseAuth').data('power',data);
    }
    $('#bindRole').iselect({
        url: '/modules/user/getRolesList.action',
        root: "data.roleList",
        displayName: 'strName', //接受显示字段
        valueName: 'id',//实际要传的字段
        
        
        value: '${userInfo.roleId}'
    });
    //编辑页面不允许 修改用户类型
    $('#bindUser').prop('disabled','true');
    // 本用户登录 用户名  用户类型  角色分配 到期时间 证书  信道树   区域树 不能修改
    if($('#disableEdit').val() === 'true'){
        $('input[name="userInfo.strName"]').prop('disabled','true');
        $('input[name="userInfo.strDate"]').prop('disabled','true');
        $('#bindRole').prop('disabled','true');
        // 本用户登录登录 传入空数组
        $('#chooseAuth').data('power',[]);
    }
})
</script>