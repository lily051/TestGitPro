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
                <input value="20001" type="hidden" name="serverInfo.ntype">
                <div class="controls">
                    <span class="serverType">系统巡检服务</span>
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
            <div class="control-group">
                <label class="control-label"><em>*</em>控制端口：</label>
                <div class="controls">
                    <input name="serverInfo.nctrlPort" value="${serverInfo.nctrlPort}" type="text" class="auto-input"
                                required="true" vtype="num" maxValue="65535" minValue="1" maxLength="5"
                                isOverflown="true"  data-icon="true" self-focus-info="请输入1至65535的有效数字"/>
                </div>
            </div>
<!--             <div class="control-group">
                <label class="control-label">数据端口：</label>
                <div class="controls">
                    <input name="server.ndataPort" value="" type="text" class="auto-input"
                                 vtype="num" maxValue="65535" minValue="1" maxLength="5"
                                isOverflown="true"  data-icon="true" self-focus-info="请输入1至65535的有效数字"/>
                </div>
            </div> -->
            <div class="control-group">
                <label class="control-label"><em>*</em>校时模式：</label>
                <div class="controls">
                    <select  class="auto-input" isOverflown="true" id="timingMode" name="serverInfo.model">
                        <option value="0" <s:if test="serverInfo.model==0">selected="selected"</s:if>>每天固定时间校时</option>
                        <option value="1" <s:if test="serverInfo.model==1">selected="selected"</s:if>>固定时间间隔校时</option>
                    </select>
                </div>
            </div>
            <div class="control-group" >
                <label class="control-label"><em>*</em>误差上限(分钟)：</label>
                <div class="controls">
                    <input type="text" class="auto-input" minValue="0" maxValue="500" maxLength="3" required="true" vtype="num" isOverflown="true" data-icon="true" name="serverInfo.condition" value="${serverInfo.condition}" placeholder="15" self-focus-info="请输入0至500的有效数字"/>
                </div>
            </div>
            <div class="control-group" id="FT">
                <label class="control-label"><em>*</em>校时时间点：</label>
                <div class="controls">
                    <input type="text" class="hikui-calendar auto-input" required="true" id="startTime"  hikui="calendar"  data-icon="true" name="serverInfo.time" value="${serverInfo.time}" datefmt="HH:mm:ss">
                </div>
            </div>
            <div class="control-group" id="intervalTime">
                <label class="control-label"><em>*</em>时间间隔(小时)：</label>
                <div class="controls">
                    <input type="text" class="auto-input" minValue="1" maxValue="24" maxLength="2" required="true" vtype="num" isOverflown="true" data-icon="true" name="serverInfo.fixedTime" value="${serverInfo.fixedTime}" self-focus-info="请输入1至24的有效数字"/>
                </div>
            </div>
            <!-- <div class="control-group">
                <label class="control-label"><em>*</em>发送报警中心间隔(分钟)：</label>
                <div class="controls">
                    <input type="text" class="auto-input" maxLength="50" required="true" vtype="num" isOverflown="true" data-icon="true" name="serverInfo.alarm" value="${serverInfo.alarm}" placeholder="10"/>
                </div>
            </div> -->
        </fieldset>
</form>
</div>
<script>
    var mode = $('#timingMode').val();
    if(mode == '0'){  //每天固定时间校时
       $('#intervalTime').hide().find('input').removeClass('auto-input');
    }else if(mode == '1'){  //固定时间间隔校时
        $('#FT').hide().find('input').removeClass('auto-input');
    }
    $('#timingMode').on('change',function(){
        var val = $(this).val();
        if(val == '0'){
            $('#intervalTime').hide().find('input').removeClass('auto-input');
            $('#FT').show().find('input').addClass('auto-input');
        }else if(val == '1'){
            $('#FT').hide().find('input').removeClass('auto-input');
            $('#intervalTime').show().find('input').addClass('auto-input');
        }
    })
</script>