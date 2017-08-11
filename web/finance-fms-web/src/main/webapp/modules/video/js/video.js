var FMS = FMS || {};
FMS.video = {
	editAble:0,
	isFlag:1,
	isComplain:0,//0是签约录像，1是投诉录像
	totalPage:0,//总页数,用于随机检查
	init:function(){
		this.globalSetting();
		this.initOrgTree();//组织区域树
	},
	globalSetting:function(){
		FinanceUtil.initFormUI("#filterForm");
		var stateType = $('.tool li.active').index()+1;//1是签约录像，2是投诉
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
			recordCheck:'',
			status:'',
			productCode:'',
			productName:'',
			nproId:'',
			proUserNo:'',
			proName:'',
			creditCode:'',
			name:'',
			upload:'',
			stateType:stateType
		};
		try{//重置的时候给regionId赋值
			this.conditions.regionId = FMS.video.getSelectedTreeNode().id.split("_")[1];
		}catch(e){
		}
	},
	bindEvent:function(){
		var that = this;
		var treeInfo = $.fn.zTree.getZTreeObj('orgTree');
		var index = $('.tool li.active').index();
		$(".oprRandom").on('click', function(event) {
			var selNode = FMS.video.getSelectedTreeNode();
			var selId = selNode.id.split("_")[1];
			that.randomCheck(selId, 1);
		});
		$(".oprCompliance").on('click', function(event) {
			var row = $('#videoDatatable').grid('getSelRow');
			var ids = $('#videoDatatable').grid('getDataByID', row, 'videoId');
			if (!ids.length) {
				jAlert('请选择要检查的录像！');
				return;
			}else if(ids.length>1){
				jAlert('每次只能检查一个录像！');
				return;
			}else{
				var upload = $('#videoDatatable').grid('getDataByID', row, 'upload');
				if(upload==0){
					jAlert('只能检查已上传的录像！');
					//$(".cbox[type='checkbox']:checked").click();
					return;
				}
				that.isComplain=0;
				that.videoPlayBack(ids, 1);
			}
			
		});
		$(".oprDelay").on('click', function(event) {
			var row = $('#videoDatatable').grid('getSelRow');
			var ids = $('#videoDatatable').grid('getDataByID', row, 'busiId');
			if (!ids.length) {
				jAlert('请选择要延时的录像！');
				return;
			}else{
				that.delayVideo(ids);
			}
		});
		$(".oprComplaint").on('click', function(event) {
			var row = $('#videoDatatable').grid('getSelRow');
			var ids = $('#videoDatatable').grid('getDataByID', row, 'videoId');
			if (!ids.length) {
				jAlert('请选择要处理的投诉录像！');
				return;
			}else if(ids.length>1){
				jAlert('每次只能处理一个录像！');
				return;
			}else{
				var upload = $('#videoDatatable').grid('getDataByID', row, 'upload');
				if(upload==0){
					jAlert('只能处理已上传的录像！');
					
					return;
				}
				that.isComplain=1;
				that.videoPlayBack(ids,1);
				//that.complaintDeal(ids);
			}
		});
		$(".oprExport").on('click', function(event) {
			if(!FMS.globalFun.exportLimit('.ui-paging-info')) return false;
			that.exportFun(index);
		});
		$("#search").on('click', function(event) {
			var conditions = that.conditions;
			for(var key in conditions){
				if(key == "regionId"|| key == "stateType"){
					continue;
				}
				conditions[key] = $("#"+key).val();
			}
			var judge = FMS.globalFun.compareTime(conditions.startTime,conditions.endTime);
			if(judge){
				$('#videoDatatable').grid("dofiltler");
			}else{
				jAlert("开始时间和结束时间间隔不能大于1年！");
			}
			
		});
		$("#clear").on('click', function(event) {
			$("#filterForm").find('input').val('');
			$("#filterForm").find('select').val('');
			that.globalSetting();
			$('#videoDatatable').grid("dofiltler");
		});
		$(".openMore").on('click', function(event) {
			FMS.globalFun.busiShowCondition($(this),'videoDatatable')
		});
		$(".showsub").change(function(event) {
			var _this = $(this);
			if(_this.attr("checked")){
				that.isFlag = 1;
			}else{
				that.isFlag = 0;
			}
			$("#videoDatatable").grid("dofiltler");
		});
	},
	/**
	 * 录像随机检查
	 * @param  {[type]} orgId [description]
	 * @param  {[type]} type  [description]
	 * @return {[type]}       [description]
	 */
	randomCheck:function(orgId, type){
		var conditions = FMS.video.conditions;
		$.ajax({
			url:'/modules/record/getRandomData.action',
			type:'post',
			dataType:'json',
			data:{
				'recordInfo.regionId':conditions.regionId,
				'recordInfo.startTime':conditions.startTime,
				'recordInfo.endTime':conditions.endTime,
				'recordInfo.recordCheck':conditions.recordCheck,
				'recordInfo.status':conditions.status,
				'recordInfo.productCode':conditions.productCode,
				'recordInfo.productName':conditions.productName,
				'recordInfo.nproId':conditions.nproId,
				'recordInfo.proUserNo':conditions.proUserNo,
				'recordInfo.proName':conditions.proName,
				'recordInfo.creditCode':conditions.creditCode,
				'recordInfo.name':conditions.name,
				'recordInfo.upload':conditions.upload,
				'recordInfo.stateType':conditions.stateType,
				'recordInfo.isFlag':FMS.video.isFlag,
				'recordInfo.pageLenth':FMS.video.totalPage
			},
			success:function(data){
				if(data && data.success){
					var id = data.data.randomRecordInfo.videoId;
					if(id==null){
						jAlert("没有录像！",'错误','error');
					}else{
						FMS.video.isComplain = 0;//设置是签约录像
						FMS.video.videoPlayBack(id, 1);
					}
				}else{
					jAlert(data.msg);
				}
			}
		});
	},
	/**
	 * 录像回放
	 * @param  {[type]} id    [description]
	 * @param  {[type]} able  左侧是否可以编辑 0不可以，1可以
	 * @param  {[type]} rowId [description]
	 * @return {[type]}       [description]
	 */
	videoPlayBack:function(id, able, rowId){
		this.editAble = able;
		FinanceUtil.dialog({
			id:'playBackDialog',
			title: '录像回放',
			overlay:true,
			className:"adfsd",
			draggable:true,
			width:1050,
			height:680,
			iframeFix:false,
			beforeClose:function(){
				try{
					document.getElementById("ocxPlayBack").PB_StopAll();
				}catch(e){
				}
			},
			ajax:{
				url:'/modules/record/getData.action',
				param:{
					'recordInfo.videoId':id
				},
				success:function(){
					if(FMS.video.isComplain == 1){
						$(".notcomplain").remove();
						var value = $("#selectCheck").val();
						if(value == 0){
							$("span[name='checkText']").text("未检查").css("color","#bb2c32");
						}else if(value == 3){
							$("span[name='checkText']").text("存在投诉").css("color","#bb2c32");
						}else if(value == 4){
							$("span[name='checkText']").text("投诉关闭").css("color","#7ABC7A");
						}else{
							$("span[name='checkText']").text("未知");
						}
					}else{
						//#bb2c32  #7ABC7A
						$(".complain").remove();
						var value = $("#selectCheck").val();
						if(value == 0){
							$("span[name='checkText']").text("未检查").css("color","#bb2c32");
						}else if(value == 1){
							$("span[name='checkText']").text("不合规").css("color","#bb2c32");
						}else if(value == 2){
							$("span[name='checkText']").text("合规").css("color","#7ABC7A");
						}else{
							$("span[name='checkText']").text("未知");
						}
					}
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI("#playBackDialog");
					FMS.globalFun.isIE6ChangeTipText('#playBackDialog');
					/*var rowInfo = $("#videoDatatable").grid('getDataByID', rowId)[0];
					for(var key in rowInfo){
						$('#playBackDialog span[name='+key+']').text(rowInfo[key]);
					}*/
					$("#playBackDialog").parents(".popInner").find("div.popTitle").find("h3").css("text-align","center");
					$("#playBackDialog").parents(".popInner").find("div.popButton").css({"padding":0,'border':0});
					
				}
			},
			close: function(){
            	FMS.globalFun.isIE6ShowSelect("#wrapper");
            }
		});
	},
	/**
	 * 录像回放时的处理
	 * @return {[type]} [description]
	 */
	videoCheck:function(){
		if($("input[name='recordInfo.recordCheck']:checked").val()==undefined){
			jAlert('请选择处理意见');
			return;
		}
		var form = $("#videoCheck");
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
			url:'/modules/record/setRecordCheck.action',//TODO 提交回放检查
			type:'post',
			dataType:'json',
			success:function(data){
				FinanceUtil.loader.hide();
				if (data && data.success) {
					$.sticky('操作成功', {type: "ok",title: '操作成功'});
					$('#videoDatatable').grid('reload');
					$('#playBackDialog').dialog('close');
				}else if(data && data.msg){
					jAlert(data.msg, '错误' , 'error');
				}
			}
		});
	},
	exportFun:function(index){
		var conditions = FMS.video.conditions;
		$.ajax({
			url:'/modules/record/exportExcel.action',
			type:'post',
			dataType:'json',
			data:{
				'recordInfo.regionId':conditions.regionId,
				'recordInfo.startTime':conditions.startTime,
				'recordInfo.endTime':conditions.endTime,
				'recordInfo.recordCheck':conditions.recordCheck,
				'recordInfo.status':conditions.status,
				'recordInfo.productCode':conditions.productCode,
				'recordInfo.productName':conditions.productName,
				'recordInfo.nproId':conditions.nproId,
				'recordInfo.proUserNo':conditions.proUserNo,
				'recordInfo.proName':conditions.proName,
				'recordInfo.creditCode':conditions.creditCode,
				'recordInfo.name':conditions.name,
				'recordInfo.upload':conditions.upload,
				'recordInfo.stateType':conditions.stateType,
				'recordInfo.isFlag':FMS.video.isFlag
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
	initOrgTree:function(){
		function nodeCreate(event, treeId, treeNode,type){
			if (treeNode.getParentNode() == null) {
				var treeObj = $.fn.zTree.getZTreeObj(treeId);
				treeObj.selectNode(treeNode);
				var node = treeObj.getSelectedNodes()[0];
				treeObj.expandNode(node, true,false ,true);
				FMS.video.conditions.regionId = treeNode.id.split('_')[1];
				if(type==1){
					$("#videoDatatable").grid("dofiltler");
				}else{
					$("#videoMain").load($(".tool ul li").eq(0).data("url"));
				}
			}
		}
		function nodeClick(event, treeId, treeNode){
			FMS.video.conditions.regionId = treeNode.id.split('_')[1];
			$("#videoDatatable").grid("dofiltler");
		}
		FMS.regionTree.init('orgTree','/web/tree/getRegionTree.action',nodeCreate,nodeClick);
	},
	getSelectedTreeNode:function(){
		var treeInfo = $.fn.zTree.getZTreeObj('orgTree');
		var selNode = treeInfo.getSelectedNodes()[0];
		return selNode;
	},
	//延时处理
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
				url:'/modules/record/todelayVideoPage.action',
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
							url:'/modules/record/setDelayDays.action',//TODO 延时处理
							type:'post',
							dataType:'json',
							data:{
								'recordInfo.busiIds':ids
							},
							success:function(data){
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$.sticky('操作成功', {type: "ok",title: '操作成功'});
									$('#videoDatatable').grid('reload');
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
	//查看业务单
	viewBusiness:function(videoId, selId,flag){
		FinanceUtil.dialog({
			id:'viewBusinessDialog',
			title: '业务单详情',
			overlay:true,
			draggable:true,
			width:700,
			height:560,
			iframeFix:false,
			ajax:{
				url:'/modules/record/getBizOrderInfo.action',
				param: {
					'saleInfo.id':selId,//TODO 传业务单id
					'saleInfo.videoId':videoId,
					'saleInfo.videoType':flag //signvideo, complainvideo
				},
				success:function(){
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					$("#viewBusinessDialog").parents('.popInner').find(".popButton").css("padding",0);
					FinanceUtil.initFormUI("#viewBusinessDialog");
					if("signvideo" == flag){
						$("#viewBusinessDialog").parents('.popInner').find("#signvideoSpan").css("display","inline");
						$("#viewBusinessDialog").parents('.popInner').find("#complainvideoSpan").css("display","none");
					}else{
						$("#viewBusinessDialog").parents('.popInner').find("#signvideoSpan").css("display","none");
						$("#viewBusinessDialog").parents('.popInner').find("#complainvideoSpan").css("display","inline");
					}
					
				}
			},
			close: function(){
            	FMS.globalFun.isIE6ShowSelect("#wrapper");
            },
			button:false
		});
	},
	videoGrid:function(){
		isFirstLoad = true;//录像列表初始化时设置为第一次加载（ie6用到）
		var colName,colModel;
		if(this.conditions.stateType == 1){
			colName = ['ids','业务单id','upload','交易网点','业务员姓名','客户姓名','产品名称','录像时间','录像时长','是否合规','操作'];
			colModel = [{
				name:'videoId',
				width:'1',
				hidden:true
			},{
				name:'busiId',
				width:'1',
				hidden:true
			},{
				name:'upload',
				width:'1',
				hidden:true
			},{
				name:'inputRegionName'
			},{
				name:'proName',
				width:100
			},{
				name:'name',
				width:100
			},{
				name:'productName'
			},{
				name:'startTime',
				width:180
			},{
				name:'recordLength',
				width:100
			},{
				name:'recordCheck',
				formart:function(d, fn){//是否合规
					if (d===0) {
						return '<span>未检查</span>';
					} else if(d === 1){
						return '<span class="unableStyle">不合规</span>';
					} else if(d === 2){
						return '<span class="enableStyle">合规</span>';
					} else{
						return '未知';
					}			
				},
				width:100,
				showTitle:false
			},{
				name:'operation',
				formart: function(d, fn) {
					if(fn.upload==1){
						return '<a class="icon gridIconFix iconRecord gridEdit" title="合规检查"></a><a class="icon gridIconFix iconDetail gridDelete" title="查看业务单详情"></a>';
					}else{
						return '<a class="icon gridIconFix iconDetail gridDelete" style="margin-left:26px;" title="查看业务单详情"></a>';
					}
				},
				attachEvent: {
					click: {
						el: 'a',
						callback: function(a, b, e) {
							if((a.srcElement == undefined ? a.target.className.indexOf("gridEdit") : a.srcElement.className.indexOf("gridEdit")) > 0){
								var id = $('#videoDatatable').grid('getDataByID', b, 'videoId');
								FMS.video.isComplain=0;
								FMS.video.videoPlayBack(id,1);
							} else if((a.srcElement == undefined ? a.target.className.indexOf("gridDelete") : a.srcElement.className.indexOf("gridDelete")) > 0){
								var busiId = $('#videoDatatable').grid('getDataByID', b, 'busiId');
								var videoId = $('#videoDatatable').grid('getDataByID', b, 'videoId');
								FMS.video.viewBusiness(videoId, busiId,"signvideo");
							}
						}
					}
				},
				width:100
			}]
		}else{
			colName = ['ids','业务单id','upload','交易网点','业务员姓名','客户姓名','产品名称','录像时间','录像时长','录像状态','处理意见','操作'];
			colModel = [{
				name:'videoId',
				width:'1',
				hidden:true
			},{
				name:'busiId',
				width:'1',
				hidden:true
			},{
				name:'upload',
				width:'1',
				hidden:true
			},{
				name: 'inputRegionName'
			},{
				name:'proName',
				width:110
			},{
				name:'name',
				width:100
			},{
				name:'productName'
			},{
				name:'startTime'
			},{
				name:'recordLength',
				width:100
			},{
				name:'recordCheck',
				formart:function(d, fn){
					if(d == 0){
						return '<span>未检查</span>';
					}else if(d == 3){
						return '<span class="unableStyle">存在投诉</span>';
					}else if(d == 4){
						return '<span class="enableStyle">投诉关闭</span>';
					}else{
						return '未知';
					}
				},
				width:'90',
				showTitle:false
			},{
				name:'checkRemark'
			},{
				name:'operation',
				formart: function(d, fn) {
					if(fn.upload==1){
						return '<a class="icon gridIconFix iconRecord gridEdit" title="投诉处理"></a><a class="icon gridIconFix iconDetail gridDelete" title="查看业务单详情"></a>';
					}else{
						return '<a class="icon gridIconFix iconDetail gridDelete" style="margin-left:26px;" title="查看业务单详情"></a>';
					}
				},
				attachEvent: {
					click: {
						el: 'a',
						callback: function(a, b, e) {
							if((a.srcElement == undefined ? a.target.className.indexOf("gridEdit") : a.srcElement.className.indexOf("gridEdit")) > 0){
								var id = $('#videoDatatable').grid('getDataByID', b, 'videoId');
								FMS.video.isComplain=1;
								FMS.video.videoPlayBack(id,1);
							} else if((a.srcElement == undefined ? a.target.className.indexOf("gridDelete") : a.srcElement.className.indexOf("gridDelete")) > 0){
								var busiId = $('#videoDatatable').grid('getDataByID', b, 'busiId');
								var videoId = $('#videoDatatable').grid('getDataByID', b, 'videoId');
								FMS.video.viewBusiness(videoId, busiId,"complainvideo");
							}
						}
					}
				},
				width:100
			}];
		}
		$("#videoDatatable").grid({
			colName:colName,
			colModel:colModel,
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
			url: '/modules/record/getDataPage.action',//TODO 录像列表
			pager: '#videoDataTablePage',
			pagerFixed: true,
			scrollWidth: 18,
			headTitle: true,
			ajaxDynamic: function(){
				var conditions = FMS.video.conditions;
				return [{
					'name':'recordInfo.regionId',
					value:conditions.regionId
				},{
					'name':'recordInfo.startTime',
					value:conditions.startTime
				},{
					'name':'recordInfo.endTime',
					value:conditions.endTime
				},{
					'name':'recordInfo.recordCheck',
					value:conditions.recordCheck
				},{
					'name':'recordInfo.status',
					value:conditions.status
				},{
					'name':'recordInfo.productCode',
					value:conditions.productCode
				},{
					'name':'recordInfo.productName',
					value:conditions.productName
				},{
					'name':'recordInfo.nproId',
					value:conditions.nproId
				},{
					'name':'recordInfo.proUserNo',
					value:conditions.proUserNo
				},{
					'name':'recordInfo.proName',
					value:conditions.proName
				},{
					'name':'recordInfo.creditCode',
					value:conditions.creditCode
				},{
					'name':'recordInfo.name',
					value:conditions.name
				},{
					'name':'recordInfo.upload',
					value:conditions.upload
				},{
					'name':'recordInfo.stateType',
					value:conditions.stateType
				},{
					'name':'recordInfo.isFlag',
					value:FMS.video.isFlag
				}];
			},
			events:{
				init:function(a, b){
					FMS.globalFun.addVerticalLine();
				},
				onEndRequest:function(event,elm){
					FMS.video.totalPage = elm._pageData.totalPage;
					FMS.globalFun.removeGridColumn('videoDatatable',elm._pageData.totalPage);
				}
			}
		});		   
	},
	//判断如果是点击录像名称看回放的不能修改
	viewOnly:function(){
		if(this.editAble == 0){
			$("#playBackDialog .btn-submit").addClass('hide');
			$("#playBackDialog .control-group.notcomplain,.control-group.complain").remove();
			$("#videoTextarea").attr("readonly","readonly").removeAttr('maxLength');
		}
	},
	destroyTree:function(){
		$.fn.zTree.destroy("orgTree");
	}
};
$(function(){
	FMS.video.init();
	$(".tool ul li").on('click', function(event) {
		if(isIE6){
			isFirstLoad = true;
		}
	    $(".tool ul li").removeClass('active');
	    $(this).addClass('active');
	    var url = $(this).data("url");
	    FMS.video.isFlag=1;
	    FMS.video.totalPage = 0;//切换页面的时候把总页数清0
	    var stateType = $('.tool li.active').index()+1;//1是签约录像，2是投诉
	    FMS.video.stateType = stateType;
	    $("#videoMain").load(url);
	});
});