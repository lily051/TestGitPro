<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<body>
	<div class="main-wrap wrapFixed">
		<div class="fullfit">
			<div class="filterFormBox ieStyle" style="margin-top:20px">
			    <form class="form-horizontal" id="filterForm">
			        <fieldset class="noLegend fieldset_25">
				    	<div class="control-group">
				    		<label class="control-label">开始时间：</label>
				    		<div class="controls conditions">
				    			<input type="text" class="auto-input" hikui="calendar" isShowClear="false" readonly="readonly" id="startTime"
				    			datefmt="yyyy-MM-dd HH:mm:ss" name="startTime"maxdate="#F{$dp.$N('endTime');}">
				    		</div>
				    	</div>
				    	<div class="control-group">
				    		<label class="control-label">操作模块：</label>
				    		<div class="controls conditions">
				    			<select name="" id="ntype">
				    			</select>
				    		</div>
				    	</div>
				    </fieldset>
				    <fieldset class="noLegend fieldset_25">
				    	<div class="control-group">
				    		<label class="control-label">结束时间：</label>
				    		<div class="controls conditions">
				    			<input type="text" class="auto-input" hikui="calendar" isShowClear="false" readonly="readonly" id="endTime"
				    			datefmt="yyyy-MM-dd HH:mm:ss" name="endTime" mindate="#F{$dp.$N('startTime');}">
				    		</div>
				    	</div>
				    	<div class="control-group">
				    		<label class="control-label">操作类型：</label>
				    		<div class="controls conditions">
				    			<select name="" id="nactionType">
				    				<option value="">全部</option>
				    				<option value="0">添加</option>
				    				<option value="1">删除</option>
				    				<option value="2">修改</option>
				    				<option value="3">导入</option>
				    				<option value="4">导出</option>
				    			</select>
				    		</div>
				    	</div>
				    </fieldset>
				    <fieldset class="noLegend fieldset_25">
				    	<div class="control-group">
				    		<label class="control-label">操作用户：</label>
				    		<div class="controls conditions">
				    			<input type="text" class="auto-input" name="" id="strUser">
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
				<a href="#"><span class="managerOpr oprExport"><i>导出</i></span></a>
			</div>
			<div class="datatableStyle" style="top:153px">
				<table id="dataTable"></table>
				<div id="dataTablePage"></div>
			</div>
		</div>
	</div>
	<script>
		$(function(){
			var data1 = [["" , "全部"], ["0", "添加"], ["1", "删除"], ["2", "修改"], ["3", "导入"], ["4", "导出"]],
				data2 = [["" , "全部"], ["0", "添加"], ["1", "删除"], ["2", "修改"]],
				data3 = [["" , "全部"], ["2", "修改"]],
				data4 = [["" , "全部"], ["2", "修改"], ["4", "导出"]],
				data5 = [["" , "全部"], ["1", "删除"], ["2", "修改"]],
				data6 = [["" , "全部"]],
				data7 = [["" , "全部"], ["4", "导出"]];
			//FMS.logSearch.destroyTree();
			$('#ntype').iselect({
			    url: '/modules/log/getNtypeList.action',
			    root: "data.privilegeList",
			    displayName: 'strName', //显示名字
			    valueName: 'strCode',//实际要传的id
			    success:function(){
			    	FMS.logSearch.globalSetting();
			    	FMS.logSearch.bindEvent();
			    	FMS.logSearch.dataGrid(3);
			    }
			}).change(function(event) {
				var selectValue = Number($(this).val()),localData = data1;
				switch(selectValue){
					case 101:localData = data2;break;//服务管理
					case 105:localData = data2;break;//参数配置
					case 106:case 110:localData = data4;break;//理财录像，到期管理
					case 107:case 111:localData = data7;break;//销售查询，日志
					case 109:localData = data5;break;//报警管理
					case 108:localData = data6;break;//统计分析
					default:localData = data1;break;
				}
				$("#nactionType").iselect({
					localData: localData
				});
			});
		});
	</script>
</body>
</html>


