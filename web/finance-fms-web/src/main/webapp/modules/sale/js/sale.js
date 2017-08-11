var FMS = FMS || {};
FMS.sale = {
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
		var date = new Date().Format("yyyy-MM-dd");
		
		var beginDate = new Date(date.replace(/-/g, '/'));
		beginDate.setDate(beginDate.getDate() - 2); // 前3天
		var startDate = beginDate.Format("yyyy-MM-dd");
		
		$("#startTime").val(startDate +" 00:00:00");
		$("#endTime").val(date +" 23:59:59");
		this.conditions = {
			regionId:'',
			startTime:$("#startTime").val(),
			endTime:$("#endTime").val(),
			productCode:'',
			productName:'',
			nproId:'',
			proUserNo:'',
			proName:'',
			creditCode:'',
			lockClient:'',
			name:''
		};
		try{//重置的时候给regionId赋值
			this.conditions.regionId = FMS.sale.getSelectedTreeNode().id.split("_")[1];
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
				FMS.sale.conditions.regionId = treeNode.id.split('_')[1];
				if(type==1){
					$("#dataTable").grid("dofiltler");
				}else{
					FMS.sale.saleGrid();
				}
			}
		}
		function nodeClick(event, treeId, treeNode){
			FMS.sale.conditions.regionId = treeNode.id.split('_')[1];
			$("#dataTable").grid("dofiltler");
		}
		FMS.regionTree.init('orgTree','/web/tree/getRegionTree.action',nodeCreate,nodeClick);
	},
	bindEvent:function(){
		var that = this;
		$(".openMore").on('click', function(event) {
			FMS.globalFun.busiShowCondition($(this),'dataTable')
		});
		$(".oprExport").on('click', function(event) {
			if(!FMS.globalFun.exportLimit('.ui-paging-info')) return false;
			that.exportFun();
		});
		$("#search").on('click', function(event) {
			var conditions = FMS.sale.conditions;
			for(var key in conditions){
				if(key == "regionId"){
					continue;
				}else{
					conditions[key] = $("#"+key).val();
				}
				
			}
			var judge = FMS.globalFun.compareTime(conditions.startTime,conditions.endTime);
			if(judge){
				$('#dataTable').grid("dofiltler");
			}else{
				jAlert("开始时间和结束时间间隔不能大于1年！");
			}
		});
		$("#clear").on('click', function(event) {
			$("#filterForm").find('input').val('');
			$("#filterForm").find('select').val('');
			that.globalSetting();
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
	getSelectedTreeNode:function(){
		var treeInfo = $.fn.zTree.getZTreeObj('orgTree');
		var selNode = treeInfo.getSelectedNodes()[0];
		return selNode;
	},
	exportFun:function(){
		var conditions = FMS.sale.conditions;
		//window.location.href="/modules/sale/exportSaleExcel.action";
		$.ajax({
			url:'/modules/sale/exportSaleExcel.action',
			type:'post',
			dataType:'json',
			data:{
				'saleInfo.inputRegionId':conditions.regionId,
				'saleInfo.businessTimeStart':conditions.startTime,
				'saleInfo.businessTimeEnd':conditions.endTime,
				'saleInfo.productCode':conditions.productCode,
				'saleInfo.productName':conditions.productName,
				'saleInfo.nproId':conditions.nproId,
				'saleInfo.proName':conditions.proName,
				'saleInfo.proUserNo':conditions.proUserNo,
				'saleInfo.creditCode':conditions.creditCode,
				'saleInfo.name':conditions.name,
				'saleInfo.lockClient':conditions.lockClient,
				'saleInfo.isFlag':this.isFlag
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
				url:'/modules/sale/getBizOrderInfo.action',
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
	saleGrid:function(){
		$("#dataTable").grid({
			colName:['ids','业务单号','交易网点', '业务单审批状态','业务员姓名','客户姓名','产品名称','办理时间','购买金额'],
			colModel:[{
				name:'id',
				width:'1',
				hidden:true
			},{
				name:'nproId',
				formart: function(d, fn){
					return '<a class="business">'+ d +'</a>';
				},
				attachEvent:{
					click:{
						el:'a.business',
						callback:function(a, b, e){
							//var busiId = $('#videodataTable').grid('getDataByID', b, 'busiId');
							//查看业务单
							FMS.sale.viewBusiness(b.split("_")[1]);
						}
					}
				},
				width:'160'
			},{
				name:'inputRegionName'
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
				name:'proName',
				width:'90'
			},{
				name:'name',
				width:'90'
			},{
				name:'productName'
			},{
				name:'businessTime',
				width:'150'
			},{name:'strBuyMoney',width:'100'}],
			colModelTmp:{sortable:false},
			headerFixed: true,
			resizable: false,
			multiSelect: true,
			multiSelectWidth:30,
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
			url: '/modules/sale/getSaleDataPage.action',//TODO 销售查询
			pager: '#dataTablePage',
			pagerFixed: true,
			scrollWidth: 18,
			headTitle: true,
			ajaxDynamic: function(){
				var conditions = FMS.sale.conditions;
				return [{
					'name':'saleInfo.inputRegionId',//区域
					value:conditions.regionId
				},{
					'name':'saleInfo.businessTimeStart',//开始时间
					value:conditions.startTime
				},{
					'name':'saleInfo.businessTimeEnd',//结束时间
					value:conditions.endTime
				},{
					'name':'saleInfo.productCode',//产品编码
					value:conditions.productCode
				},{
					'name':'saleInfo.productName',//产品名称
					value:conditions.productName
				},{
					'name':'saleInfo.nproId',//业务单号
					value:conditions.nproId
				},{
					'name':'saleInfo.proName',//业务员名字
					value:conditions.proName
				},{
					'name':'saleInfo.proUserNo',//业务员工号
					value:conditions.proUserNo
				},{
					'name':'saleInfo.creditCode',//证件号
					value:conditions.creditCode
				},{
					'name':'saleInfo.name',//客户姓名
					value:conditions.name
				},{
					'name':'saleInfo.isFlag',//是否显示子节点
					value:FMS.sale.isFlag
				},{
					'name':'saleInfo.lockClient',//是否锁定
					value:conditions.lockClient
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
	FMS.sale.init();
});