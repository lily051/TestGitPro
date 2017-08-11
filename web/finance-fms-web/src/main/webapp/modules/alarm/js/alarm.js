var FMS = FMS || {};
FMS.alarm = {
	isFlag:1,
	init:function(){
		if(isIE6){
			FMS.globalFun.tableHeight_ie6();
		}
		this.globalSetting();
		this.initOrgTree();
		this.bindEvent();
		FinanceUtil.initFormUI("#filterForm");
	},
	globalSetting:function(){
		var date = new Date().Format("yyyy-MM-dd");
		
		var beginDate = new Date(date.replace(/-/g, '/'));
		beginDate.setDate(beginDate.getDate() - 2); // 前3天
		var startDate = beginDate.Format("yyyy-MM-dd");
		
		$("#beginTime").val(startDate + " 00:00:00");
		$("#endTime").val(date + " 23:59:59");
		this.conditions = {
			regionId :'',
			beginTime : $("#beginTime").val(),
			endTime : $("#endTime").val(),
			alarmType :  "",
			dealStatus : ''
		};
		try{//重置的时候给regionId赋值
			this.conditions.regionId = FMS.globalFun.getSelectedTreeNode('orgTree').id.split("_")[1];
		}catch(e){
		}
	},
	bindEvent:function(){
		var that = this;
		$("#alarmDeal").on('click', function(event) {
			var row = $('#dataTable').grid('getSelRow');
			var ids = $('#dataTable').grid('getDataByID', row, 'id');
			if (!ids.length) {
				jAlert('请选择要处理的报警信息！');
				return;
			}else{
				that.alarmDeal(ids);
			}
		});
		$(".oprDelete").on('click', function(event) {
			that.multDelete();
		});
		$("#search").on('click', function(event) {
			that.conditions.beginTime = $("#beginTime").val();
			that.conditions.endTime = $("#endTime").val();
			that.conditions.alarmType = $("#alarmType").val();
			that.conditions.dealStatus = $("#dealStatus").val();
			var judge = FMS.globalFun.compareTime(that.conditions.beginTime,that.conditions.endTime);
			if(judge){
				$("#dataTable").grid("dofiltler");
			}else{
				jAlert("开始时间和结束时间间隔不能大于1年！");
			}
			
		});
		$("#clear").on('click', function(event) {
			that.globalSetting();
			$("#dealStatus,#alarmType").val("");
			FinanceUtil.initFormUI("#filterForm");
			$("#dataTable").grid("dofiltler");
		});
		$(".showsub").change(function(event) {
			var _this = $(this);
			if(_this.attr("checked")){
				that.isFlag = 1;
			}else{
				that.isFlag = 0;
			}
			$("#dataTable").grid("dofiltler");
		});
	},
	initOrgTree:function(){
		function nodeCreate(event, treeId, treeNode,type){
			if (treeNode.getParentNode() == null) {
				var treeObj = $.fn.zTree.getZTreeObj(treeId);
				treeObj.selectNode(treeNode);
				var node = treeObj.getSelectedNodes()[0];
				treeObj.expandNode(node, true,false ,true);
				FMS.alarm.conditions.regionId = treeNode.id.split('_')[1];
				if(type==1){
					$("#dataTable").grid("dofiltler");
				}else{
					FMS.alarm.alarmGrid();
				}
			}
		}
		function nodeClick(event, treeId, treeNode){
			FMS.alarm.conditions.regionId = treeNode.id.split('_')[1];
			$("#search").click();
		}
		FMS.regionTree.init('orgTree','/web/tree/getRegionTree.action',nodeCreate,nodeClick);
	},
	alarmDeal:function(ids){
		FinanceUtil.dialog({
			 id:'alarmDealDialog',
			 title: '报警处理',
			 overlay:true,
			 draggable:true,
			 width:500,
			 height:250,
			 iframeFix:false,
			 ajax:{
			 	url:'/modules/alarm/toUpdateAlarmPage.action',
			 	success:function(){
			 		FMS.globalFun.isIE6HiddenSelect("#wrapper");
			 		FinanceUtil.initFormUI("#alarmDealDialog");
			 		FMS.globalFun.isIE6ChangeTipText('#alarmDealDialog');
			 	}
			 },
			 close: function(){
            	FMS.globalFun.isIE6ShowSelect("#wrapper");
            },
			 buttons:{
			 	'ok':{
			 		text:'确定',
			 		'class':'bPrimary',
			 		click:function(){
			 			var form = $("#alarmDealDialog").find('form');
			 			form.ajaxSubmit({
			 				beforeSubmit:function(){
			 					if (FinanceUtil.validate(form)){
			 						FinanceUtil.loader.show('请稍等...');
			 						return true;
			 					}else{
			 						jAlert('信息输入有误', '错误', 'error');
			 						return false;
			 					}
			 				},
			 				url:'/modules/alarm/updateAlarmLogs.action',//TODO 处理报警信息
			 				type:'post',
			 				dataType:'json',
						 	data: {
						 		'ids':ids//TODO 传报警id
						 	},
			 				success:function(data){
			 					FinanceUtil.loader.hide();
			 					if (data && data.success) {
			 						$.sticky('操作成功', {type: "ok",title: '操作成功'});
			 						$('#dataTable').grid('reload');
			 						$('#alarmDealDialog').dialog('close');
			 					}else if(data && data.msg){
			 						jAlert(data.msg, '错误' , 'error');
			 					}
			 				}
			 			});
			 		}
			 	},
			 	'cancel':{
			 		text:'取消',
			 		click:function(){
			 			$('#alarmDealDialog').dialog('close');
			 		}
			 	}
			 }
		});
	},
	multDelete:function(){
		var row = $('#dataTable').grid('getSelRow');
		var ids = $('#dataTable').grid('getDataByID', row, 'id');
		if (!ids.length) {
			jAlert('请选择要删除的报警信息！');
		} else {
			jConfirm('确定要删除选中的报警信息？',{
				title:'提示',
				callback:function(value){
					if (value) {
						$.ajax({
							url: '/modules/alarm/deleteAlarmLogs.action',//TODO delete报警信息
							data: {
								'ids': ids
							},
							success:function(data){
								FMS.globalFun.doSuccess(data,'dataTable','删除成功');
							}
						});
						
					};
				}
			});
		}
	},
	alarmGrid:function(){
			$("#dataTable").grid({
				colName:['ids','处理人','处理人编号','处理时间','报警类型','报警区域','报警IP','报警内容','报警开始时间','报警最新时间','处理状态','处理人备注'],
				colModel:[{
					name:'id',
					width:'0',
					hidden:true
				},{
					name:'checker',
					width:'0',
					hidden:true
				},{
					name:'userNo',
					width:'0',
					hidden:true
				},{
					name:'dealTime',
					width:'0',
					hidden:true
				},{
					name:'alarmTypeName',
					width:6
				},{
					name:'alarmRegionName',
					width:6
				},{
					name: 'strHost',
					width:6
				},{
					name: 'comments',
					width:16,
					formart:function(d,fn){
						return '<span>'+d+'</span>';
					}
				},{
					name:'alarmbeginTime',
					width:13
				},{
					name:'alarmTime',
					width:13
				},{
					name:'status',
					width:4,
					showTitle:false,
					formart:function(d, fn){//TODO　处理状态
						if(d == 0){
							return '<span class="undeal">未处理</span>';
						}else if(d==1){
							return '<div class="dealhover"><span class="deal">已处理</span></div>';
						}else{
							//<div class="dealInfo"><div><span>处理人：</span><span>'+fn.checker+'</span></div><div><span>处理人编号：</span><span>'+fn.userNo+'</span></div><div><span>处理时间：</span><span>'+fn.dealTime.replace(/T/," ")+'</span></div></div>
							return '未知';
						}
						
					},
					attachEvent:{
						mouseover:{
							el:'div.dealhover',
							callback:function(a, b, e){
								var checker = $('#dataTable').grid('getDataByID', b, 'checker');
								var userNo = $('#dataTable').grid('getDataByID', b, 'userNo');
								var dealTime = $('#dataTable').grid('getDataByID', b, 'dealTime');
								dealTime = String(dealTime).replace(/T/," ");
								var html = '<div class="dealInfo"><div><span class="desc">处理人：</span><span>'+checker+'</span></div><div><span class="desc">处理人编号：</span><span>'+userNo+'</span></div><div><span class="desc">处理时间：</span><span>'+dealTime+'</span></div></div>';
								$("body").append(html);
								var target = a.target || a.srcElement;
								var offset = $(target).parents("td").offset();
								//var x = a.clientX;
								//var y = a.clientY;
								var offsetX = offset.left,offsetY = offset.top
								$(".dealInfo").css({"top":offsetY-120,"left":offsetX-150});
								$("div.dealhover").on('mouseout', function(event) {
									$(".dealInfo").remove();
								});
							}
						}
					}
				},{
					name:'dealcomment',
					width:10
				}],
				colModelTmp:{sortable:false},
				headerFixed: true,
				resizable: false,
				multiSelect: true,
				multiSelectWidth:25,
				//rowNum: 15,
				ajaxRequestReader: {
					page: 'start',
					rowNum: "limit"
				},
				ajaxResponseReader: {
					page: 'pageNo',
					rows: 'result',
					records: 'total',
					totalPage: 'totalPage'
				},
				rowMark: false,
				sortorder: 'desc',
				height: '100%',
				ajaxOptions: {
					type: 'post'
				},
				dataType: 'ajax',
				url: '/modules/alarm/getAlarmDataPage.action',//TODO 报警信息列表
				pager: '#dataTablePage',
				pagerFixed: true,
				scrollWidth: 18,
				headTitle: true,
				ajaxDynamic: function(){
					var conditions = FMS.alarm.conditions;
					return [
					{
						'name':'alarmlogInfo.alarmbeginTimeStart',
						value:conditions.beginTime
					},
					{
						'name':'alarmlogInfo.alarmbeginTimeEnd',
						value:conditions.endTime
					},
					{
						'name':'alarmlogInfo.alarmType',
						value:conditions.alarmType
					},
					{
						'name':'alarmlogInfo.status',
						value:conditions.dealStatus
					},
					{
						'name':'alarmlogInfo.isFlag',
						value:FMS.alarm.isFlag
					},
					{
						'name':'alarmlogInfo.alarmRegionId',
						value:conditions.regionId
					}];
				},
				events:{
					init:function(a, b){
						FMS.globalFun.addVerticalLine();
					},
					onEndRequest:function(event,elm){
						FMS.globalFun.removeGridColumn('dataTable',elm._pageData.totalPage);
					}
				}
			});
	}
};
$(function(){
	if(modeParam.usbAndIpc=="ipc"){
		$("#alarmType").find('option[name="usb"]').remove();
		if (modeParam.nonCenterStorage == "true") {//无中心存储
			$("#alarmType").find('option[name="ipc haveCenterStorage"]').remove();
		} else {//有中心存储
			$("#alarmType").find('option[name="ipc nonCenterStorage"]').remove();
		}
		
	}else{
		$("#alarmType").find('option[name="ipc"]').remove();
		$("#alarmType").find('option[name="ipc nonCenterStorage"]').remove();
		$("#alarmType").find('option[name="ipc haveCenterStorage"]').remove();
	}
	FMS.alarm.init();
});
