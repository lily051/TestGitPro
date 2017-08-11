var FMS = FMS || {};
FMS.channelMng = {
	init:function(){
		initOcx({ocxId:'ocxPreview',divId:'previewVideo',ocxType:0,lLayoutMode:1});
		if(isIE6){
			var parentH = $("#content").height();
			$(".deviceMain").height(parentH-70);
			$(".datatableStyle").height(parentH-120);
		}
		this.initConstants();
		this.bindEvent();
		if (modeParam.usbAndIpc == "ipc" && modeParam.channelIpBind == "true") {//开启通道ip绑定功能，通道管理页面显示理财电脑ip
			this.channelGridChannelIpBind();			
		} else {//关闭通道ip绑定功能，通道管理页面不显示通道ip,查询条件中不显示通道ip
			this.channelGrid();
			document.getElementById("searchUserIp").innerHTML = '';
		}
	},
	initConstants: function(){
		FinanceUtil.initFormUI("#filterFormBox");
		deviceName = userIp = channelName = '';
	},
	bindEvent: function(){
		$(".toolbar").unbind('click').on('click', '.oprDeleteChannel', function(event) {
			FMS.channelMng.multDeleteChannel();
		});
		$("#clear").on('click', function(event) {
			$("#ndeviceName,#userIp,#nchannelName").val("");
			FMS.channelMng.initConstants();
			$("#deviceDatatable").grid("dofiltler");
		});
		$(".toolbar").on('click', '#filtrate', function(event) {
			FMS.globalFun.showSearchCondition($(this),'deviceDatatable');
			
		});
	},
	channelGridChannelIpBind:function(){//有通道ip绑定
		$("#deviceDatatable").grid({
			colName:['ids','通道名称','通道号','所属设备', '理财电脑IP', '所属区域', '操作'],
			colModel:[{
				name:'id',
				width:'1',
				hidden:true
			},{
				name:'nchannelName',
				index:'nchannelName',
				width:200,
				formart:function(d, fn){
					return '<a class="nchannelNameInGrid">' + d + '</a>';
				},
				attachEvent:{
					click:{
						el:'a.nchannelNameInGrid',
						callback:function(a, b, e){
							var index = b.indexOf("_");
							var id = b.substring(index+1);
							FMS.channelMng.editChannel(id);
						}
					}
				}
			},{
				name: 'nchannelNo',
				index: 'nchannelNo',
				width:100
			},{
				name: 'ndeviceName',
				index: 'ndeviceName',
				width:200
			},{
				name: 'userIp',
				index: 'userIp',
				width:200
			},{
				name: 'regionName',
				index: 'regionName',
				width:150
			},{
				name: 'operation',
				index: 'operation',
				width: 70,
				formart: function(d, fn) {
					return '<a class="icon gridIconFix iconPreview gridPreview"></a><a class="icon gridIconFix iconDelete gridDelete"></a>';
				},
				attachEvent: {
					click: {
						el: 'a',
						callback: function(a, b, e) {
							var id = $.trim($("#" + b).find('td').eq(1).text());
							var channelName = $.trim($("#" + b).find('td').eq(2).text()); 
							if((a.srcElement == undefined ? a.target.className.indexOf("gridPreview") : a.srcElement.className.indexOf("gridPreview")) > 0){
								FMS.channelMng.getDevChannelInfo(id, channelName);
							} else if((a.srcElement == undefined ? a.target.className.indexOf("gridDelete") : a.srcElement.className.indexOf("gridDelete")) > 0){
								FMS.channelMng.deleteChannelSingle(id);
							}
						}
					}
				}
			}],
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
			url: '/modules/device/getChannelsDataPage.action',//获取设备通道信息列表
			pager: '#channelDatatablePage',
			pagerFixed: true,
			scrollWidth: 18,
			headTitle: true,
			ajaxDynamic: function(){
				return [{
					'name':'deviceChannelInfo.ndeviceName',
					value:deviceName
				},
				{
					'name':'deviceChannelInfo.nchannelName',
					value:channelName
				},
				{
					'name':'deviceChannelInfo.userIp',
					value:userIp
				},
				{
					'name':'deviceChannelInfo.nregionId',
					value:parentId
				},
				{
					'name':'deviceChannelInfo.isFlag',
					value:FMS.device.isFlag
				}];
			},
			events:{
				init:function(){
					FMS.globalFun.addVerticalLine();
				},
				onEndRequest:function(event,elm){
					FMS.globalFun.removeGridColumn('deviceDatatable',elm._pageData.totalPage);
				}
			}
		});
	},
	channelGrid:function(){//无通道ip绑定
		$("#deviceDatatable").grid({
			colName:['ids','通道名称','通道号','所属设备', '所属区域', '操作'],
			colModel:[{
				name:'id',
				width:'1',
				hidden:true
			},{
				name:'nchannelName',
				index:'nchannelName',
				width:200,
				formart:function(d, fn){
					return '<a class="nchannelNameInGrid">' + d + '</a>';
				},
				attachEvent:{
					click:{
						el:'a.nchannelNameInGrid',
						callback:function(a, b, e){
							var index = b.indexOf("_");
							var id = b.substring(index+1);
							FMS.channelMng.editChannel(id);
						}
					}
				}
			},{
				name: 'nchannelNo',
				index: 'nchannelNo',
				width:100
			},{
				name: 'ndeviceName',
				index: 'ndeviceName',
				width:200
			},{
				name: 'regionName',
				index: 'regionName',
				width:200
			},{
				name: 'operation',
				index: 'operation',
				width: 70,
				formart: function(d, fn) {
					return '<a class="icon gridIconFix iconPreview gridPreview"></a><a class="icon gridIconFix iconDelete gridDelete"></a>';
				},
				attachEvent: {
					click: {
						el: 'a',
						callback: function(a, b, e) {
							var id = $.trim($("#" + b).find('td').eq(1).text());
							var channelName = $.trim($("#" + b).find('td').eq(2).text()); 
							if((a.srcElement == undefined ? a.target.className.indexOf("gridPreview") : a.srcElement.className.indexOf("gridPreview")) > 0){
								FMS.channelMng.getDevChannelInfo(id, channelName);
							} else if((a.srcElement == undefined ? a.target.className.indexOf("gridDelete") : a.srcElement.className.indexOf("gridDelete")) > 0){
								FMS.channelMng.deleteChannelSingle(id);
							}
						}
					}
				}
			}],
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
			url: '/modules/device/getChannelsDataPage.action',//获取设备通道信息列表
			pager: '#channelDatatablePage',
			pagerFixed: true,
			scrollWidth: 18,
			headTitle: true,
			ajaxDynamic: function(){
				return [{
					'name':'deviceChannelInfo.ndeviceName',
					value:deviceName
				},
				{
					'name':'deviceChannelInfo.nchannelName',
					value:channelName
				},
				{
					'name':'deviceChannelInfo.nregionId',
					value:parentId
				},
				{
					'name':'deviceChannelInfo.isFlag',
					value:FMS.device.isFlag
				}];
			},
			events:{
				init:function(){
					FMS.globalFun.addVerticalLine();
				},
				onEndRequest:function(event,elm){
					FMS.globalFun.removeGridColumn('deviceDatatable',elm._pageData.totalPage);
				}
			}
		});
	},
	editChannel: function(id){
		FinanceUtil.dialog({
			id:'Dialog',
			title: '编辑通道基本信息',
			overlay:true,
			draggable:true,
			width:440,
			height:340,
			iframeFix:false,
			ajax:{
				url:'/modules/device/toUpdateChannel.action',
				param:{'id':id},
				success:function(){
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI("#Dialog");
					FMS.globalFun.isIE6ChangeTipText('#Dialog');
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
						var form = $('#Dialog').find('form');
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
							url:'/modules/device/updateChannel.action',
							type:'post',
							dataType:'json',
							data:{},
							success:function(data){
								FinanceUtil.loader.hide();
								$('#Dialog').dialog('close');
								if(data && data.success){
									$.sticky('操作成功', {type: "ok",title: '操作成功',afterclose: function() {}});
									$("#deviceDatatable").grid("reload");
								}else{
									jAlert(data.msg,'错误','error');
								}
							}
						});
					}
				},
				'cancel':{
					text:'取消',
					click:function(){
						$('#Dialog').dialog('close');
					}
				}
			}
		});
	},
	//删除多个通道
	multDeleteChannel: function(){
		var row = $('#deviceDatatable').grid('getSelRow');
		var ids = $('#deviceDatatable').grid('getDataByID', row, 'id');
		if (!ids.length) {
			jAlert('请选择要删除的通道！');
			return;
		}
		jConfirm('确认删除选中的通道？',{
			title:'提示',
			callback:function(value){
				if (value) {
					$.ajax({
						url: '/modules/device/deleteChannel.action',
						data: {
							'ids': ids
						},
						success:function(data){
							//$("#deviceDatatable").grid("reload");
							FMS.globalFun.doSuccess(data,'deviceDatatable','删除成功');
						}
					});
				};
			}
		});
	},
	//删除单个通道
	deleteChannelSingle: function(id){
		jConfirm('是否确定删除？', {
			title: '提示',
			callback: function(value) {
				if (value) {
					$.ajax({
						url: '/modules/device/deleteChannel.action',
						data: {
							'ids':id
						},
						success: function(data) {
							FMS.globalFun.doSuccess(data,'deviceDatatable','删除成功');
						}
					})
				}
			}
		});
	},
	//获取通道预览
	getDevChannelInfo :function(id, channelName) {
		FinanceUtil.dialog({
			id:'Dialog123',
			title: channelName,
			overlay:true,
			draggable:false,
			width:550,
			height:500,
			iframeFix:false,
			ajax:{
				url:'/modules/device/getDevChannelInfo.action',
				param:{'id':id},
				success:function(){
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI("#Dialog123");
					FMS.globalFun.isIE6ChangeTipText('#Dialog123');
				}
			},
			beforeClose:function(){
				try{
					document.getElementById("ocxPreview").StopPreview(0);
				}catch(e){
				}
			},
			close: function(){
            	FMS.globalFun.isIE6ShowSelect("#wrapper");
            	$('#Dialog123').dialog('close');
            }
		});
	}
};
$(function(){
	FMS.device.globalSetting();
	FMS.device.bindEvent();
	FMS.channelMng.init();
});