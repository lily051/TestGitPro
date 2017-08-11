var FMS = FMS || {};
FMS.busiRelate = {
	conditions:{
		startTime:'',
		endTime:'',
		recordName:'',
		customName:'',
		productName:''
	},
	init:function() {
		this.globalSetting();
		this.busiGrid();
		this.bindEvent();
	},
	globalSetting:function(){
		var date = new Date().Format("yyyy-MM-dd");
		
		var beginDate = new Date(date.replace(/-/g, '/'));
		beginDate.setDate(beginDate.getDate() - 2); // 前3天
		var startDate = beginDate.Format("yyyy-MM-dd");
		
		$("#startTime").val(startDate +" 00:00:00");
		$("#endTime").val(date +" 23:59:59");
		FinanceUtil.initFormUI("#filterForm");
		this.conditions={
			startTime:$("#startTime").val(),
			endTime:$("#endTime").val(),
			recordName:$("#recordName").val(),
			customName:$("#customName").val(),
			productName:$("#productNameForTable").val()
		};
	},
	bindEvent:function(){
		var that = this;
		$("#search").on('click', function(event) {
			that.conditions={
				startTime:$("#startTime").val(),
				endTime:$("#endTime").val(),
				recordName:$("#recordName").val(),
				customName:$("#customName").val(),
				productName:$("#productNameForTable").val()
			};
			var judge = FMS.globalFun.compareTime(that.conditions.startTime,that.conditions.endTime);
			if(judge){
				$("#datatable").grid("dofiltler");
			}else{
				jAlert("开始时间和结束时间间隔不能大于1年！");
			}
		});
		$("#clear").on('click', function(event) {
			$('input,select').val('');
			that.globalSetting();
			$("#datatable").grid("dofiltler");
		});
	},
	deleteRecord:function(ids){
		jConfirm('你确定要删除选中的录像吗',{
			title:'提示',
			callback:function(value){
				if(value){
					$.ajax({
						url:'',//TODO 删除录像
						type:'post',
						dataType:'json',
						data:{
							param:ids
						},
						success:function(data){
							if(data&&data.success){
								FMS.busiRelate.busiGrid("reload");
							}else{
								jAlert(data.msg,'错误','error');
							}
						}
					});
				}
			}
		});
	},
	busiGrid:function(){
		$("#datatable").grid({
			colName:['ids','cliid','recordId','录像名称','业务单号','录像类型','客户姓名','产品名称','业务单审批状态','业务单录入时间','开始时间','结束时间','操作'],
			colModel:[{
				name:'proid',
				width:'0',
				hidden:true
			},{
				name:'cliid',
				width:'0',
				hidden:true
			},{
				name:'recordId',
				hidden:true
			},{
				name:'strRecordName',
				formart:function(d, fn){
					return '<a class="recordName">'+d+'</a>';
				},
				attachEvent:{
					click:{
						el:'a.recordName',
						callback:function(a, b, e){
							var recordId = $('#datatable').grid('getDataByID', b, 'proid');
							FMS.dataBusiness.recordPlayBack(recordId,'',2);
						}
					}
				}
			},{
				name: 'nProId',
				width:'170'
			},{
				name: 'stateType',
				formart:function(d, fn){
					if(d == 1){
						return '签约录像';
					}else if(d == 2){
						return '投诉录像';
					}else{
						return '未知';
					}
				},
				width:'100'
			},{
				name: 'customername',
				width:"100"
			},{
				name: 'productName'
			},{
				name: 'lockClient',
				width:'120',
				formart:function(d, fn){
					if(d == 1){
						return '<span style="color: black;">审批通过</span>';
					}else if (d == 2){
						return '<span style="color: red;">审批退回</span>';
					} else {
						return '<span style="color: black;">待审批</span>';
					}
				}
			},{
				name:'dtBusinessTime'
			},{
				name:'startTime'
			},{
				name: 'endTime'
			},{
				name:'operate',
				formart:function(d,fn){
					if(fn.lockClient == 2){
						return '<a class="icon gridIconFix iconPrint gridAssign" title="打印"></a><a class="icon gridIconFix iconBusiEdit gridEdit" title="编辑业务单"></a>';
					}else{
						return '<a class="icon gridIconFix iconPrint gridAssign" title="打印"></a><a class="icon gridIconFix iconDetail " title="业务单详情"></a>';
					}
					
				},
				attachEvent:{
					click:{
						el:'a',
						callback:function(a, b, e){
							var id = $('#datatable').grid('getDataByID', b, 'proid'),
								businessId = $('#datatable').grid('getDataByID', b, 'cliid'),
								proid = $('#datatable').grid('getDataByID', b, 'proid');
							if((a.srcElement == undefined ? a.target.className.indexOf("iconPrint") : a.srcElement.className.indexOf("iconPrint")) > 0){
								FMS.dataBusiness.printPicture(id,'2');
							}else if((a.srcElement == undefined ? a.target.className.indexOf("iconBusiEdit") : a.srcElement.className.indexOf("iconBusiEdit")) > 0){
								FMS.dataBusiness.editOrder(businessId, proid);
							}else if((a.srcElement == undefined ? a.target.className.indexOf("iconDetail") : a.srcElement.className.indexOf("iconDetail")) > 0){
								FMS.dataBusiness.viewOrder(businessId, proid);
							}
						}
					}
				},
				width:'100'
			}],
			colModelTmp:{sortable:false},
			headerFixed: true,
			resizable: false,
			multiSelect: true,
			multiSelectWidth:28,
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
			rowMarkWidth:40,
			sortorder: 'desc',
			height: '100%',
			ajaxOptions: {
				type: 'post'
			},
			dataType: 'ajax',
			url: '/webdevice/contact/getRecordsDataPage.action',//TODO 业务关联列表
			pager: '#dataTablePage',
			pagerFixed: true,
			scrollWidth: 18,
			headTitle: true,
			ajaxDynamic: function(){
				var conditions = FMS.busiRelate.conditions;
				return [{
					'name':'realOrdersInfo.startTime',//开始时间
					value:conditions.startTime
				},{
					'name':'realOrdersInfo.endTime',//结束时间
					value:conditions.endTime
				},{
					'name':'realOrdersInfo.customername',//客户名称
					value:conditions.customName
				},{
					'name':'realOrdersInfo.strRecordName',//录像名称
					value:conditions.recordName
				},{
					'name':'realOrdersInfo.productName',//产品名称
					value:conditions.productName
				}];
			},
			events:{
				init:function(a, b){
					FMS.globalFun.addVerticalLine();
					$("#gd_datatable_rl").prepend('<span class="ui-jqgrid-resize-ltr">|</span>');
				},
				onEndRequest:function(event,elm){
					FMS.globalFun.removeGridColumn('datatable',elm._pageData.totalPage);
				}
			}
		});
	},
	getSelectCheckbox:function(){
		var row = $('#datatable').grid('getSelRow'),
			rowInfo = $('#datatable').grid('getDataByID', row);
		return rowInfo;
	}
};
$(function(){
	if(isIE6){
		FMS.globalFun.tableHeight_ie6();
	}
	$("#busiRelate").addClass('active');
	FMS.busiRelate.init();
});