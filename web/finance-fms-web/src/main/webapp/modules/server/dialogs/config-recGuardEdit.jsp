<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<div>
<form class="form-horizontal form-fixed formLabel120">
        <div class="msg-b msg-b-attention" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;">
            <div class="msg-cnt">
                <span>红色<em>*</em>为必填项</span>
            </div>
        </div>
        <fieldset class="noLegend">
            <div class="control-group ">
                <label class="control-label">服务类型：</label>
                 <input value="20002" type="hidden" name="serverInfo.ntype">
                <div class="controls">
                    <span class="serverType">录像守卫服务</span>
                    <input value="${serverInfo.id}" type="hidden" name="serverInfo.id">               
                </div>
            </div>
            <div class="control-group">
                <label class="control-label"><em>*</em>服务名称：</label>
                <div class="controls">
                    <input type="text" vtype="common" maxLength="50" required="true" class='auto-input' data-icon="true" isoverflown="true" name="serverInfo.strUser" value="${serverInfo.strUser}"> 
                </div>
            </div>
            <div class="control-group">
                <label class="control-label"><em>*</em>服务IP：</label>
                <div class="controls">
                    <input type="text" vtype="ip" maxLength="15" required="true" class='auto-input' data-icon="true" isoverflown="true" name="serverInfo.strIp" value="${serverInfo.strIp}"> 
                </div>
            </div>
<!--             <div class="control-group">
                <label class="control-label">控制端口：</label>
                <div class="controls">
                    <input name="server.nctrlPort" value="" type="text" class="auto-input"
                                 vtype="num" maxValue="65535" minValue="1" maxLength="5"
                                isOverflown="true"  data-icon="true" self-focus-info="请输入1至65535的有效数字"/>
                </div>
            </div>
            <div class="control-group">
                <label class="control-label">数据端口：</label>
                <div class="controls">
                    <input name="server.ndataPort" value="" type="text" class="auto-input"
                                 vtype="num" maxValue="65535" minValue="1" maxLength="5"
                                isOverflown="true"  data-icon="true" self-focus-info="请输入1至65535的有效数字"/>
                </div>
            </div> -->
            <div class="control-group ">
                <label class="control-label"><em>*</em>任务时间间隔(s)：</label>
                <div class="controls">
                    <input name="serverInfo.timeOut" value="${serverInfo.timeOut}" type="text" class="auto-input" required="true" vtype="num"
                                    minValue="2" maxValue="1000" maxLength="4" self-focus-info="请输入2至1000的有效数字" isOverflown="true" data-icon="true" placeholder="2"/>
                </div>
            </div>
            <div class="control-group">
                <label class="control-label"><em>*</em>巡检服务IP：</label>
                <div class="controls">
                    <input type="text" vtype="ip" maxLength="50" required="true" class='auto-input' data-icon="true" isoverflown="true" name="serverInfo.strSysIp" value="${serverInfo.strSysIp}"> 
                </div>
            </div>
            <div class="control-group">
                <label class="control-label"><em>*</em>巡检服务端口：</label>
                <div class="controls">
                    <input name="serverInfo.nctrlSysPort" value="${serverInfo.nctrlSysPort}" type="text" class="auto-input"
                                required="true" vtype="num" maxValue="65535" minValue="1" maxLength="5"
                                isOverflown="true"  data-icon="true" self-focus-info="请输入1至65535的有效数字"/>
                </div>
            </div>
        </fieldset>
</form>
</div>