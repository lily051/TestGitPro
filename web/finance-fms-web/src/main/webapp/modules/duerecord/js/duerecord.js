var FMS = FMS || {};
FMS.due = {
	isFlag:1,
	init:function(){
		if(isIE6){
			FMS.globalFun.tableHeight_ie6();
		}
		this.globalSetting();
		this.initOrgTree();//组织区域树
		this.bindEvent();
	},
	globalSetting:function(){
		FinanceUtil.initFormUI("#filterForm");
		this.conditions = {
			regionId:'',
			productName:'',
			nproId:'',
			name:'',
			proName:'',
			nisPay:''
		};
		try{//重置的时候给regionId赋值
			this.conditions.regionId = FMS.due.getSelectedTreeNode().id.split("_")[1];
		}catch(e){
		}
	},
	initOrgTree:function(){
		function nodeCreate(event, treeId, treeNode,type){
			if (treeNode.getParentNode() == null) {
				var treeObj = $.fn.zTree.getZTreeObj(treeId);
				treeObj.selectNode(treeNode);
				var node = treeObj.getSelectedNodes()[0];
				treeObj.expandNode(node, true,false ,true);
				FMS.due.conditions.regionId = treeNode.id.split('_')[1];
				if(type==1){
					$("#dataTable").grid("dofiltler");
				}else{
					FMS.due.dueGrid();
				}
			}
		}
		function nodeClick(event, treeId, treeNode){
			FMS.due.conditions.regionId = treeNode.id.split('_')[1];
			$("#dataTable").grid("dofiltler");
		}
		FMS.regionTree.init('orgTree','/web/tree/getRegionTree.action',nodeCreate,nodeClick);
	},
	bindEvent:function(){
		var that = this;
		$(".oprExport").on('click', function(event) {
			if(!FMS.globalFun.exportLimit('.ui-paging-info')) return false;
			that.exportFun();
		});
		$(".oprDelay").on('click', function(event) {
			var row = $('#dataTable').grid('getSelRow');
			var ids = $('#dataTable').grid('getDataByID', row, 'busiId');
			if (!ids.length) {
				jAlert('请选择要延时的录像！');
				return;
			}else{
				var nisPay = $('#dataTable').grid('getDataByID', row, 'nisPay').join('');
				if(nisPay.indexOf('1')>-1){
					jAlert('含有已到期业务单,无法延期处理！');
					return;
				}else{
					that.delayVideo(ids);
				}
			}
		});
		$(".oprSignRead").on('click', function(event) {
			var row = $('#dataTable').grid('getSelRow');
			var ids = $('#dataTable').grid('getDataByID', row, 'busiId');
			if (!ids.length) {
				jAlert('请选择要标记的录像！');
				return;
			}else{
				that.signRead(ids);
			}
		});
		$("#search").on('click', function(event) {
			var conditions = FMS.due.conditions;
			for(var key in conditions){
				if(key == "regionId"){
					continue;
				}else{
					conditions[key] = $("#"+key).val();
				}
				
			}
			$('#dataTable').grid("dofiltler");
		});
		$("#clear").on('click', function(event) {
			$("#filterForm").find('input').val('');
			$("#filterForm").find('select').val('');
			that.globalSetting();
			$('#dataTable').grid("dofiltler");
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
	getSelectedTreeNode:function(){
		var treeInfo = $.fn.zTree.getZTreeObj('orgTree');
		var selNode = treeInfo.getSelectedNodes()[0];
		return selNode;
	},
	delayVideo:function(ids){
		FinanceUtil.dialog({
			id:'delayDialog',
			title: '延时处理',
			overlay:true,
			draggable:true,
			width:420,
			height:170,
			iframeFix:false,
			ajax:{
				url:'/modules/duerecord/todelayVideoPage.action',
				success:function(){
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI("#delayDialog");
					FMS.globalFun.isIE6ChangeTipText('#delayDialog');
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
						var form = $("#delayDialog").find('form');
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
							url:'/modules/duerecord/setDelayDays.action',//TODO 延时处理
							type:'post',
							dataType:'json',
							data:{
								'dueRecordInfo.busiIds':ids
							},
							success:function(data){
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$.sticky('操作成功', {type: "ok",title: '操作成功'});
									$('#dataTable').grid('reload');
									$('#delayDialog').dialog('close');
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
						$('#delayDialog').dialog('close');
					}
				}
			}
		});
	},
	signRead:function(ids){
		$.ajax({
			url:'/modules/duerecord/setReadStatus.action',//TODO 标记已读
			type:'post',
			dataType:'',
			data:{
				'dueRecordInfo.busiIds':ids
			},
			success:function(data){
				if(data && data.success){
					$.sticky('操作成功', {type: "ok",title: '操作成功'});
					$('#dataTable').grid('reload');
				}else{
					jAlert(data.msg,'错误','error');
				}
				
			}
		});
	},
	exportFun:function(){
		var conditions = FMS.due.conditions;
		$.ajax({
			url:'/modules/duerecord/exportExcel.action',//TODO 导出
			type:'post',
			dataType:'',
			data:{
				'dueRecordInfo.regionId':conditions.regionId,//区域
				'dueRecordInfo.nisPay':conditions.nisPay,//是否到期
				'dueRecordInfo.name':conditions.name,
				'dueRecordInfo.nproId':conditions.nproId,//业务单号
				'dueRecordInfo.productName':conditions.productName,//产品名称
				'dueRecordInfo.proName':conditions.proName,//业务员名字
				'dueRecordInfo.isFlag':FMS.due.isFlag
			},
			success:function(data){
				if(data && data.success){
					window.location.href = data.data.url+'?rnd=' + new Date().getTime();
				}else{
					jAlert(data.msg,'错误','error');
				}
				
			}
		});
	},
	viewBusiness:function(busiId){
		FinanceUtil.dialog({
			id:'viewBusinessDialog',
			title: '业务单详情',
			overlay:true,
			draggable:true,
			width:700,
			height:560,
			iframeFix:false,
			button:false,
			ajax:{
				url:'/modules/duerecord/getBizOrderInfo.action',
				param: {
					'saleInfo.id':busiId
				},
				success:function(){
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					$("#viewBusinessDialog").parents('.popInner').find(".popButton").css("padding",0);
					FinanceUtil.initFormUI("#viewBusinessDialog");
				}
			},
			close: function(){
            	FMS.globalFun.isIE6ShowSelect("#wrapper");
            }
		});
	},
	dueGrid:function(){
		$("#dataTable").grid({
			colName:['ids','busiId','业务单号','交易网点','办理时间','业务员姓名','客户姓名','产品名称','录像个数','录像状态','距离到期天数','是否已读'],
			colModel:[{
				name:'videoId',
				hidden:true
			},{
				name:'busiId',
				hidden:true
			},{
				name: 'nproId',
				formart:function(d, fn){
					return '<a class="proInfo">' + d + '</a>';
				},
				attachEvent: {
					click: {
						el: 'a.proInfo',
						callback: function(a, b,e) {
							var busiId = $('#dataTable').grid('getDataByID', b, 'busiId');
							FMS.due.viewBusiness(busiId);
						}
					}
				},
				width:'200'
			},{
				name:'inputRegionName'
			},{
				name:'businessTime',
				width:115
			},{
				name:'proName',
				width:110
			},{
				name:'name',
				width:100
			},{
				name:'productName',
				width:120
			},{
				name:'videoNum',
				width:90
			},{
				name:'nisPay',
				formart:function(d, fn){
					if(d == 0){
						return "<span>未到期</span>";
					}else if(d == 1){
						return '<span class="unableStyle">已到期</span>';
					}else{
						return '未知';
					}
				},
				width:100
			},{
				name:'daysAway',
				formart:function(d, fn){
					if(fn.nisPay == 0){
						return '<span class="unableStyle">'+ d +'</span>';
					}else if(fn.nisPay == 1){
						return '——';
					}else{
						return '未知';
					}
				},
				width:130
			},{
				name:'nisRead',
				formart:function(d, fn){
					if (d==0) {
						return '<span>未读</span>';
					} else if(d == 1){
						return '<span class="enableStyle">已读</span>';
					} else{
						return '未知';
					}			
				},
				width:90
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
			url: '/modules/duerecord/getDataPage.action',//TODO 到期管理
			pager: '#dataTablePage',
			pagerFixed: true,
			scrollWidth: 18,
			headTitle: true,
			ajaxDynamic: function(){
				var conditions = FMS.due.conditions;
				return [{
					'name':'dueRecordInfo.regionId',//区域
					value:conditions.regionId
				},{
					'name':'dueRecordInfo.nisPay',//是否到期
					value:conditions.nisPay
				},{
					'name':'dueRecordInfo.nproId',//业务单号
					value:conditions.nproId
				},{
					'name':'dueRecordInfo.name',//客户姓名
					value:conditions.name
				},{
					'name':'dueRecordInfo.productName',//产品名称
					value:conditions.productName
				},{
					'name':'dueRecordInfo.proName',//业务员名字
					value:conditions.proName
				},{
					'name':'dueRecordInfo.isFlag',//是否显示子节点
					value:FMS.due.isFlag
				}];
			},
			events:{
				init:function(a, b){
					FMS.globalFun.addVerticalLine();
				},
				onEndRequest:function(event,elm){
					if(isIE6){
						FMS.globalFun.removeGridColumn('dataTable',elm._pageData.totalPage);
					}
				}
			}
		});
	}
};
$(function(){
	FMS.due.init();
});