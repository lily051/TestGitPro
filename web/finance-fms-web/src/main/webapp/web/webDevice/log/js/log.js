var FMS = FMS || {};
FMS.logSearch = {
	conditions:{
		startTime:'',
		endTime:'',
		ntype:''
	},
	init:function() {
		this.globalSetting();
		this.logGrid();
		this.bindEvent();
	},
	globalSetting:function(){
		FinanceUtil.initFormUI("#filterForm");
		
		var date = new Date().Format("yyyy-MM-dd");
		var beginDate = new Date(date.replace(/-/g, '/'));
		beginDate.setDate(beginDate.getDate() - 2); // 前3天
		var startDate = beginDate.Format("yyyy-MM-dd");
		$("#startTime").val(startDate +" 00:00:00");
		$("#endTime").val(date +" 23:59:59");
		
		this.conditions={
			startTime:$("#startTime").val(),
			endTime:$("#endTime").val(),
			ntype:$("#ntype").val()
		};
	},
	bindEvent:function(){
		var that = this;
		$("#search").on('click', function(event) {
			that.conditions={
				startTime:$("#startTime").val(),
				endTime:$("#endTime").val(),
				ntype:$("#ntype").val()
			};
			var judge = FMS.globalFun.compareTime(that.conditions.startTime,that.conditions.endTime);
			if(judge){
				$("#dataTable").grid("dofiltler");
			}else{
				jAlert("开始时间和结束时间间隔不能大于1年！");
			}
		});
		$("#clear").on('click', function(event) {
			$('input,select').val('');
			that.globalSetting();
			$("#dataTable").grid("dofiltler");
		});
	},
	logGrid:function(){
		$("#dataTable").grid({
			colName:['操作时间','操作类型','备注'],
			colModel:[{
				name:'operTime',
				width:10
			},{
				name:'operType',
				width:10,
				formart:function(d, fn){
					var text = "";
					switch(d){
						case 1:text="开始录像";break;
						case 2:text="结束录像";break;
						case 3:text="视频补录";break;
						case 4:text="添加离线录像";break;
						case 5:text="编辑录像名";break;
						case 6:text="删除录像";break;
						case 7:text="处理异常录像";break;
						case 11:text="录单";break;
						case 12:text="关联业务单";break;
						case 13:text="编辑业务单";break;
						default:text="未知";break;
					}
					return text;
				},
				showTitle:false
			},{
				name:'note',
				width:50
			}],
			colModelTmp:{sortable:false},
			headerFixed: true,
			resizable: false,
			multiSelect: false,
			multiSelectWidth:28,
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
			rowMark: true,
			rowMarkWidth:40,
			sortorder: 'desc',
			height: '100%',
			ajaxOptions: {
				type: 'post'
			},
			dataType: 'ajax',
			url: '/webdevice/log/getDatePage.action',//TODO 日志列表
			pager: '#dataTablePage',
			pagerFixed: true,
			scrollWidth: 18,
			headTitle: true,
			ajaxDynamic: function(){
				var conditions = FMS.logSearch.conditions;
				return [{
					'name':'operlogInfo.startoperTime',
					value:conditions.startTime
				},{
					'name':'operlogInfo.endoperTime',
					value:conditions.endTime
				},{
					'name':'operlogInfo.operType',
					value:conditions.ntype
				}];
			},
			events:{
				init:function(a, b){
					FMS.globalFun.addVerticalLine();
					$("div#gdd_dataTable_rl").prepend('序号');
				},
				onEndRequest:function(event,elm){
					FMS.globalFun.removeGridColumn('dataTable',elm._pageData.totalPage);
				}
			}
		});
	}
};
$(function(){
	if(isIE6){
		FMS.globalFun.tableHeight_ie6();
	}
	$("#logSearch").addClass('active');
	FMS.logSearch.init();
});