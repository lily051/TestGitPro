var FMS = FMS || {};
FMS.logSearch = {
	exportType:0,
	isFlag:1,
	globalSetting:function(){
		if(isIE6){
			$(".datatableStyle").height($("#content").height()-160);
		}
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
			recordType:'',
			nproId:'',
			recordName:'',
			checkUserName:'',
			productName:'',
			productType:'',
			creditId:'',
			name:'',
			inputUser:'',
			strUser:'',
			ntype:'',
			nactionType:'',
			nmoduleType:''
		};
		try{//重置的时候给regionId赋值
			this.conditions.regionId = FMS.logSearch.getSelectedTreeNode().id.split("_")[1];
		}catch(e){
		}
	},
	initOrgTree:function(){
		$("#tree").show();$('#keyValue').val('');
		var treeInfo = refresh();
		function refresh(type){
			var tree = $("#orgTree").tree({
				ajax:{
					url:'/web/tree/getRegionTree.action'//TODO 
				},
				callback: {
					onNodeCreated:function(event, treeId, treeNode){
						if (treeNode.getParentNode() == null) {
							var treeObj = $.fn.zTree.getZTreeObj(treeId);
							treeObj.selectNode(treeNode);
							var node = treeObj.getSelectedNodes()[0];
							treeObj.expandNode(node, true,false ,true);
							FMS.logSearch.conditions.regionId = treeNode.id.split('_')[1];
							if(type==1){
								$("#dataTable").grid("dofiltler");
							}else{
								FMS.logSearch.dataGrid(1);
							}
						}
					},
					onClick:function(event, treeId, treeNode){
						FMS.logSearch.conditions.regionId = treeNode.id.split('_')[1];
						$("#search").click();
					}
				}
			});
			return tree;
		}
		$(".reload").click(function(){//刷新树
			$("#keyValue").val('');
			$(".treeDelete").hide();
			treeInfo = refresh(1);
		});
		$("#orgTreeSearch").on('click', function(event) {
			searchTree('orgTree','label');
		});
		$(".treeDelete").on('click', function(event) {
			$(this).next().next('input').val('');
			$(this).hide();
			//$(this).next().click();
		});
		$('#keyValue').keyup(function(e) {
			var key = e.keyCode;
			if (key == 13) {
				$(this).prev().click();
				if($(this).val().length>0){
					$(".treeDelete").show();
				}
				return false;
			} else {
				if($(this).val().length>0){
					$(".treeDelete").show();
				}else{
					$(".treeDelete").hide();
				}
				
			}
		});
		function searchTree(treeId, label){
			var value = $('#keyValue').val();
			if(value == ""){
				treeInfo = refresh();
			}else{
		        var treeObj = $.fn.zTree.getZTreeObj(treeId);  
		        var nodes = treeObj.transformToArray(treeObj.getNodes());
				treeObj.hideNodes(nodes);//隐藏所有节点
		        var nodeList = treeObj.getNodesByParamFuzzy(label, value,null); //查询出来的节点
		        for(var index=0;index<nodeList.length;index++){
		        	treeObj.showNode(nodeList[index]);
		        	hasParentNode(nodeList[index],treeObj);
		        }
			}
		}
		//判断父节点是否可以显示
		function hasParentNode(node,treeObj) {  
			var currnode = node.getParentNode();
			treeObj.showNode(currnode);
			if(currnode){
				hasParentNode(currnode,treeObj);
			}else{
			    treeObj.expandNode(node, true, true, true);
			}
		}
	},
	destroyTree:function(){
		$.fn.zTree.destroy("orgTree");
		$("#tree").hide();
	},
	getSelectedTreeNode:function(){
		var treeInfo = $.fn.zTree.getZTreeObj('orgTree');
		var selNode = treeInfo.getSelectedNodes()[0];
		return selNode;
	},
	bindEvent:function(){
		var that = this;
		$(".oprExport").on('click', function(event) {
			if(!FMS.globalFun.exportLimit('.ui-paging-info')) return false;
			that.exportFun(that.exportType);
		});
		$("#search").on('click', function(event) {
			var conditions = FMS.logSearch.conditions;
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
	/**
	 * 日志导出
	 * @param  {[type]} type 导出是哪个日志
	 * @return {[type]}      [description]
	 */
	exportFun:function(type){
		var url,searchData,conditions = FMS.logSearch.conditions;
		switch(type){
			case 1 : url='/modules/log/exportRecordLogExcel.action';
					 searchData = { 'recordlogInfo.regionId':conditions.regionId,'recordlogInfo.nproId':conditions.nproId,
								    'recordlogInfo.operDateStart':conditions.startTime,'recordlogInfo.operDateEnd':conditions.endTime,
								    'recordlogInfo.recordName':conditions.recordName,'recordlogInfo.recordType':conditions.recordType,
								    'recordlogInfo.checkUserName':conditions.checkUserName,'recordlogInfo.isFlag':FMS.logSearch.isFlag};
				     break;
			case 2 : url='/modules/log/exportDelLogExcel.action';
					 searchData = { 'clientdeletelogInfo.nproId':conditions.nproId,'clientdeletelogInfo.name':conditions.name,
								    'clientdeletelogInfo.deleteTimeStart':conditions.startTime,'clientdeletelogInfo.deleteTimeEnd':conditions.endTime,
								    'clientdeletelogInfo.productName':conditions.productName,'clientdeletelogInfo.creditId':conditions.creditId};
				     break;
			case 3 : url='/modules/log/exportConfiglogExcel.action';
					 searchData = { 'configlogInfo.strUser':conditions.strUser,'configlogInfo.ntype':conditions.ntype,
				   				    'configlogInfo.strDateStart':conditions.startTime,'configlogInfo.strDateEnd':conditions.endTime,
				   				    'configlogInfo.nactionType':conditions.nactionType};
					
				     break;
			case 4 : url='/modules/log/exportLogsysExcel.action';
					 searchData = { 'logsysInfo.strUser':conditions.strUser,'logsysInfo.ntype':conditions.ntype,
				   					'logsysInfo.strDateStart':conditions.startTime,'logsysInfo.strDateEnd':conditions.endTime,
				   					'logsysInfo.nmoduleType':conditions.nmoduleType};
				     break;
			default:return;break;
		}
		$.ajax({
			url:url,
			type:'post',
			dataType:'json',
			data:searchData,
			success:function(data){
				if(data && data.success){
					window.location.href = data.data.url+'?rnd=' + new Date().getTime();
				}else{
					jAlert(data.msg,'错误','error');
				}
				
			}
		});
	},
	/**
	 * [dataGrid description]
	 * @param  {[type]} type 1是回放日志 2是删除日志 3是配置日志 4是系统日志
	 * @return {[type]}      [description]
	 */
	dataGrid:function(type){
		isFirstLoad = true;//设置为第一次加载列表（ie6用到）
		this.exportType = type;
		var colName,colModel,url;
		switch(type){
			case 1:colName=['ids','业务单号','录像名称','录像类型','回放时间','操作用户'];
				   colModel = [{name:'id',hidden:true},{name:'nproId'},{name:'recordName'},{name:'recordType',showTitle:false,
				   formart:function(d, fn){
				   	if(d == 1){
				   		return "签约录像";
				   	}else if(d == 2){
				   		return "投诉录像";
				   	}else{
			   			return '未知';
			   		}
				   }},{name:'operDate'},{name:'checkUserName'}];
				   url = '/modules/log/getRecordLogDataPage.action';//TODO 回放日志
				   break;
			case 2:colName=['ids','业务单号','产品名称','客户证件号','客户姓名','业务员姓名','删除时间'];
				   colModel = [{name:'id',hidden:true},{name:'nproId'},{name:'productName'},{name:'creditId'},{name:'name'},{name:'inputUser'},{name:'deleteTime'}];
				   url = '/modules/log/getClientDelLogDataPage.action';//TODO 删除日志
				   break;
			case 3:colName=['ids','操作用户','操作模块','IP地址','操作类型','操作备注','操作时间'];
				   colModel = [{name:'id',hidden:true},{name:'strUser'},{name:'ntypeName'},{name:'strIp'},{name:'nactionType',showTitle:false,
				   formart:function(d, fn){
				   		if(d == 0){
				   			return '添加';
				   		}else if(d ==1){
				   			return '删除';
				   		}else if(d == 2){
				   			return "修改";
				   		}else if(d == 3){
				   			return "导入";
				   		}else if(d == 4){
				   			return "导出";
				   		}else{
				   			return '未知';
				   		}
				   }},{name:'strText'},{name:'strDate'}];
				   url = '/modules/log/getConfiglogDataPage.action';//TODO 配置日志
				   break;
			case 4:colName=['ids','操作用户','用户类型','IP地址','操作类型','操作时间'];
				   colModel = [{name:'id',hidden:true},{name:'strUser'},
				   {name:'nmoduleType',
					formart:function(d ,fn){
						if(d == 0){
							return '中心管理员';
						}else if(d ==1){
							return '理财经理';
						}else{
							return '未知';
						}
					}},{name:'strIp'},{name:'ntype',showTitle:false,
				   formart:function(d, fn){
				   	if(d==0){
				   		return '登录';
				   	}else if(d == 1){
				   		return '登出';
				   	}else{
				   		return '未知';
				   	}
				   }},{name:'strDate'}];
				   url = '/modules/log/getLogsysDataPage.action';//TODO 系统日志
				   break;
			default:break;
		}
		$("#dataTable").grid({
			colName:colName,
			colModel:colModel,
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
			url: url,
			pager: '#dataTablePage',
			pagerFixed: true,
			scrollWidth: 18,
			headTitle: true,
			ajaxDynamic: function(){
				var conditions = FMS.logSearch.conditions;
				switch(type){
					case 1:return [{name:'recordlogInfo.regionId',value:conditions.regionId},{name:'recordlogInfo.nproId',value:conditions.nproId},
			   					  {name:'recordlogInfo.operDateStart',value:conditions.startTime},{name:'recordlogInfo.operDateEnd',value:conditions.endTime},
			   					  {name:'recordlogInfo.recordName',value:conditions.recordName},{name:'recordlogInfo.recordType',value:conditions.recordType},
			   					  {name:'recordlogInfo.checkUserName',value:conditions.checkUserName},{name:'recordlogInfo.isFlag',value:FMS.logSearch.isFlag}];
			   					  break;
		   			case 2:return [{name:'clientdeletelogInfo.nproId',value:conditions.nproId},{name:'clientdeletelogInfo.name',value:conditions.name},
			   					  {name:'clientdeletelogInfo.deleteTimeStart',value:conditions.startTime},{name:'clientdeletelogInfo.deleteTimeEnd',value:conditions.endTime},
			   					  {name:'clientdeletelogInfo.productName',value:conditions.productName},{name:'clientdeletelogInfo.creditId',value:conditions.creditId}];
				   				  break;
				   	case 3:return [{name:'configlogInfo.strUser',value:conditions.strUser},{name:'configlogInfo.ntype',value:conditions.ntype},
			   					  {name:'configlogInfo.strDateStart',value:conditions.startTime},{name:'configlogInfo.strDateEnd',value:conditions.endTime},
			   					  {name:'configlogInfo.nactionType',value:conditions.nactionType}];
			   					  break;
			   		case 4:return  [{name:'logsysInfo.strUser',value:conditions.strUser},{name:'logsysInfo.ntype',value:conditions.ntype},
				   					{name:'logsysInfo.strDateStart',value:conditions.startTime},{name:'logsysInfo.strDateEnd',value:conditions.endTime},
				   					{name:'logsysInfo.nmoduleType',value:conditions.nmoduleType}];
				   					break;
				}
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
	},
	getLableHeight:function(){
		var labelH=10;
		$(".special").each(function(index, el) {
			labelH+=$(el).outerHeight();
		});
		return labelH;
	},
	setTreeHeight:function(){
		$("#tree").height($("#content").height()-FMS.logSearch.getLableHeight());
		$("#treefullfit").height($("#tree").height()-46);
		if(isIE6){
			$("#treefullfit .treeView").height($("#treefullfit").height());
		}
	}
};
$(function(){
	FMS.logSearch.setTreeHeight();
	$(window).resize(function(event) {
		FMS.logSearch.setTreeHeight();
	});
	/*$('.sideBar li').on('click',function(){
		var url = $(this).data('url');
		$("#main-wrap").load(url);
		$('#sideBar li,.special').removeClass('active');
		if(!isIE6){
			$("#tree").animate({"height":0,"padding-top":0},function(){
				$(this).css({"overflow":"hidden","border":0});
			});
		}else{
			$("#tree").hide();
		}
		$(this).addClass('active');
	});*/
	$('.special').on('click',function(){
		FMS.logSearch.isFlag=1;
		var _this = $(this);
		var url = _this.data('url');
		$("#main-wrap").load(url);
		if(_this.hasClass('recordLog')){
			if(!isIE6){
				if ($("#tree").css("height") == '0px') {
					$("#tree").animate({"height":$("#content").height()-FMS.logSearch.getLableHeight(),"padding-top":10},function(){
						$(this).css({"overflow":"auto","border-bottom":"solid 1px #DBDBDB"});
					});
					_this.siblings('.special').removeClass('active');
					$(".upDown").addClass("upDownOn");
				}else {
					$("#tree").animate({"height":0,"padding-top":0},function(){
						$(this).css({"overflow":"hidden","border":0});
					});
					_this.siblings('.special').removeClass('active')
					
					$(".upDown").removeClass("upDownOn");
					_this.addClass('active');
				}
			}else{
				$("#tree").show();
			}
		}else{
			if(!isIE6){
				$("#tree").animate({"height":0,"padding-top":0},function(){
					$(this).css({"overflow":"hidden","border":0});
				});
			}else{
				$("#tree").hide();
			}
			_this.siblings('.special').removeClass('active')
			_this.addClass('active');
			$(".upDown").removeClass("upDownOn");
		}
	});
	$(".special").eq(0).click();
	//$('#sideBar li').eq(0).click();
});