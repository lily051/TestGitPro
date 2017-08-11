var FMS = FMS || {};
FMS.collectDevice = {
	init:function(){
		if(isIE6){
			var parentH = $("#content").height();
			$(".deviceMain").height(parentH-70);
			$(".datatableStyle").height(parentH-120);
		}
		this.collectGrid();
	},
	//新增采集设备
	addCollect:function(orgId){
		FinanceUtil.dialog({
			id:'Dialog',
			title: '添加采集设备',
			overlay:true,
			draggable:true,
			width:1080,
			height:document.body.clientHeight*0.5,
			iframeFix:false,
			ajax:{
				url:'/modules/device/toAddCollPage.action',
				param: {
					'deviceInfo.nregionId':orgId//TODO 传id
				},
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
						var form = $("#Dialog").find('form');
						/*if($("#subsection").val().length==0){
							jAlert("请获取关联存储设备的存储卷！");
							return;
						}*/
						var data = {};//  通道信息
						var deviceChannelList=[];
						if(channeldata!=""){
							var list = channeldata.result;
							for(var i=0;i<list.length;i++){
								deviceChannelList.push({"nchannelNo":list[i].nchannelNo,"nchannelName":list[i].nchannelName});
							}
						}/*else{
							jAlert("请获取通道信息！");
							return;
						}*/
						data.deviceChannelList = deviceChannelList;
						var dev_pwd = FMS.device.base64($("#dev_pwd").val());
						if (!FinanceUtil.validate(form)){
							jAlert('信息输入有误', '错误', 'error');
							return false;
						}
						FinanceUtil.loader.show('请稍等...');
						if($('#ntpEnable').val()==1){//启用NTP校时
							var deviceInfo = {ntype:$("#deviceType").val(),strIp:$("#deviceIp").val(),nport:$("#dev_port").val(),strUser:$("#dev_user").val(),strPassword:dev_pwd}
							var ntpParamInfo = {nenable:$('#ntpEnable').val(),ntpAddress:$('#ntp_addr').val(), ntpPortNo:$('#ntp_port').val(), ntpInterval:$('#ntp_interval').val()}
							var result = FMS.device.rpNtpDevice(deviceInfo,ntpParamInfo);
							if(!result.success){
								FinanceUtil.loader.hide();
								jConfirm('配置NTP校时失败，确定要添加设备？',{
									title:'提示',
									callback:function(value){
										if(value) {
											form.ajaxSubmit({
												url:'/modules/device/addColl.action',//TODO 添加存储设备
												type:'post',
												dataType:'json',
												data:{
													'jsonStr':$.jsonToStr(data),
													'deviceInfo.strPassword':dev_pwd
												},
												success:function(data){
													FinanceUtil.loader.hide();
													if (data && data.success) {
														$.sticky('操作成功', {type: "ok",title: '操作成功',afterclose: function() {}});
														$('#deviceDatatable').grid('reload');
														$('#Dialog').dialog('close');
													}else if(data && data.msg){
														jAlert(data.msg, '错误' , 'error');
													}
												}
											});
										}else{
											return;
										}
									}
								});
							}else{
								form.ajaxSubmit({
									url:'/modules/device/addColl.action',//TODO 添加存储设备
									type:'post',
									dataType:'json',
									data:{
										'jsonStr':$.jsonToStr(data),
										'deviceInfo.strPassword':dev_pwd
									},
									success:function(data){
										FinanceUtil.loader.hide();
										if (data && data.success) {
											$.sticky('操作成功', {type: "ok",title: '操作成功',afterclose: function() {}});
											$('#deviceDatatable').grid('reload');
											$('#Dialog').dialog('close');
										}else if(data && data.msg){
											jAlert(data.msg, '错误' , 'error');
										}
									}
								});
							}
						}else {
							form.ajaxSubmit({
								beforeSubmit:function(){
								},
								url:'/modules/device/addColl.action',//TODO 添加存储设备
								type:'post',
								dataType:'json',
								data:{
									'jsonStr':$.jsonToStr(data),
									'deviceInfo.strPassword':dev_pwd
								},
								success:function(data){
									FinanceUtil.loader.hide();
									if (data && data.success) {
										$.sticky('操作成功', {type: "ok",title: '操作成功',afterclose: function() {}});
										$('#deviceDatatable').grid('reload');
										$('#Dialog').dialog('close');
									}else if(data && data.msg){
										jAlert(data.msg, '错误' , 'error');
									}
								}
							});
						}
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
	//编辑采集设备
	editCollect:function(deviceId){
		FinanceUtil.dialog({
			id:'Dialog',
			title: '编辑采集设备',
			overlay:true,
			draggable:true,
			width:1080,
			height:document.body.clientHeight*0.5,
			iframeFix:false,
			ajax:{
				url:'/modules/device/toUpdateCollPage.action',
				param: {
					'deviceInfo.id':deviceId//TODO 传设备id
				},
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
						var form = $("#Dialog").find('form');
						/*if($("#subsection").val().length==0){
							jAlert("请获取关联存储设备的存储卷！");
							return;
						}*/
						var data = {};//  通道信息
						var deviceChannelList=[];
						if(channeldata!=""){
							var list = channeldata.result;
							for(var i=0;i<list.length;i++){
								deviceChannelList.push({"nchannelNo":list[i].nchannelNo,"nchannelName":list[i].nchannelName});
							}
						}
						data.deviceChannelList = deviceChannelList;
						var dev_pwd = FMS.device.base64($("#dev_pwd").val());
						if (!FinanceUtil.validate(form)){
							jAlert('信息输入有误', '错误', 'error');
							return false;
						}
						var ntpEnable = $('#ntpEnable');
						if(ntpEnable && ntpEnable.val() != ntpEnable.data("ntp")){//NTP校时变更
							FinanceUtil.loader.show('请稍等...');
							var deviceInfo = {ntype:$("#deviceType").val(),strIp:$("#deviceIp").val(),nport:$("#dev_port").val(),strUser:$("#dev_user").val(),strPassword:dev_pwd}
							var ntpParamInfo = {nenable:$('#ntpEnable').val(),ntpAddress:$('#ntp_addr').val(), ntpPortNo:$('#ntp_port').val(), ntpInterval:$('#ntp_interval').val()}
							var result = FMS.device.rpNtpDevice(deviceInfo,ntpParamInfo);
							if(!result.success){
								FinanceUtil.loader.hide();
								jConfirm('修改NTP校时状态失败(检查NTP配置和设备信息)，确定要编辑设备？',{
									title:'提示',
									callback:function(value){
										if(value) {
											if (ntpEnable.val() == '1') {
												$('#ntpEnable').val('0');
											}else{
												$('#ntpEnable').val('1');
											}
											form.ajaxSubmit({
												url:'/modules/device/updateColl.action',//TODO 编辑采集设备
												type:'post',
												dataType:'json',
												data:{
													'jsonStr':$.jsonToStr(data),
													'deviceInfo.strPassword':dev_pwd,
													'deviceInfo.ntpEnable':$('#ntpEnable').val()
												},
												success:function(data){
													FinanceUtil.loader.hide();
													if (data && data.success) {
														$.sticky('操作成功', {type: "ok",title: '操作成功',afterclose: function() {}});
														$('#deviceDatatable').grid('reload');
														$('#Dialog').dialog('close');
													}else if(data && data.msg){
														jAlert(data.msg, '错误' , 'error');
													}
												}
											});
										}else{
											return;
										}
									}
								});
							}else{
								form.ajaxSubmit({
									url:'/modules/device/updateColl.action',//TODO 编辑采集设备
									type:'post',
									dataType:'json',
									data:{
										'jsonStr':$.jsonToStr(data),
										'deviceInfo.strPassword':dev_pwd
									},
									success:function(data){
										FinanceUtil.loader.hide();
										if (data && data.success) {
											$.sticky('操作成功', {type: "ok",title: '操作成功',afterclose: function() {}});
											$('#deviceDatatable').grid('reload');
											$('#Dialog').dialog('close');
										}else if(data && data.msg){
											jAlert(data.msg, '错误' , 'error');
										}
									}
								});
							}
						}else{
							form.ajaxSubmit({
								url:'/modules/device/updateColl.action',//TODO 编辑采集设备
								type:'post',
								dataType:'json',
								data:{
									'jsonStr':$.jsonToStr(data),
									'deviceInfo.strPassword':dev_pwd
								},
								success:function(data){
									FinanceUtil.loader.hide();
									if (data && data.success) {
										$.sticky('操作成功', {type: "ok",title: '操作成功',afterclose: function() {}});
										$('#deviceDatatable').grid('reload');
										$('#Dialog').dialog('close');
									}else if(data && data.msg){
										jAlert(data.msg, '错误' , 'error');
									}
								}
							});
						}
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
	multDeleteCollect:function(){
		var row = $('#deviceDatatable').grid('getSelRow');
		var ids = $('#deviceDatatable').grid('getDataByID', row, 'id');
		if (!ids.length) {
			jAlert('请选择要删除的设备！');
		} else {
			jConfirm('确定删除选中的视频采集设备？',{
				title:'请谨慎删除',
				callback:function(value){
					if (value) {
						$.ajax({
							url: '/modules/device/deleteColls.action',//TODO delete采集设备
							data: {
								'ids': ids
							},
							success:function(data){
								FMS.globalFun.doSuccess(data,'deviceDatatable','删除成功');
							}
						});
						
					};
				}
			});
		}
	},
	collectGrid:function(){
		$("#deviceDatatable").grid({
			colName:['ids','设备用户名','设备密码','设备编码','设备名称','设备类型','设备IP','设备端口','密码等级','模拟通道数','IP通道数','在线状态','关联存储设备','所属区域','校时'],
			colModel:[{
				name:'id',
				hidden:true
			},{
				name:'strUser',
				hidden:true
			},{
				name:'strPassword',
				hidden:true
			},{
				name:'strCode',
				hidden:true
			},{
				name:'strName',
				formart:function(d, fn){
					return '<a class="storageName">' + d + '</a>';
				},
				attachEvent:{
					click:{
						el:'a.storageName',
						callback:function(a, b, e){
							var index = b.indexOf("_");
							var id = b.substring(index+1);
							channeldata = "";
							FMS.collectDevice.editCollect(id);
						}
					}
				},
				width:130
			},{
				name: 'ntype',
				formart: function(d, fn){//TODO 设备类型
					if(d==Constants.device.DEV_TYPE_DVR){
						return "<span>DVR</span>";
					}else if(d == Constants.device.DEV_TYPE_NVR){
						return "<span>NVR</span>";
					}else{
						return '未知';
					}
				},
				showTitle:false,
				width:90
			},{
				name: 'strIp',
				width:120
			},{
				name:'nport',
				width:90
			},{
				name:'strPasswordLevel',
				formart: function(d, fn){//TODO 密码等级
					if(d==0){
						return "<span>风险密码</span>";
					}else if(d == 1){
						return "<span>弱密码</span>";
					}else if(d == 2){
						return "<span>中密码</span>";
					}else if(d == 3){
						return "<span>强密码</span>";
					}else{
						return '未知';
					}
				},
				showTitle:false,
				width:100
			},{
				name:'nchanNum',
				width:110
			},{
				name:'telChanNum',
				width:100
			},{
				name:'online',
				index:'online',
				formart:function(d, fn){//TODO 在线状态
					if(d==0){
						return "<span>离线</span>";
					}else if(d == 1){
						return "<span>在线</span>";
					}else{
						return '未知';
					}
				},
				showTitle:false,
				width:90
			},{
				name:'collectRelate',
				index:'collectRelate',
				formart:function(d, fn){
					if (modeParam.usbAndIpc == "ipc" && modeParam.nonCenterStorage == "true") {
						return '<span class="unableStyle">无</span>';
					} else {
						if(d==0){
							return '<span class="unableStyle">未关联</span>';
						}else if(d == 1){
							return '<span class="onlineStyle">已关联</span>';
						}else{
							return '<span class="unableStyle">未关联</span>';
						}
					}
				},
				showTitle:false,
				width:120
			},{
				name:'regionName',
				width:100
			},{
				name:'ntpEnable',
				width:100,
				formart:function(d, fn){
					if(d==1){
						return '<span class="onlineStyle">启用</span>';
					}else{
						return '<span class="unableStyle">关闭</span>';
					}
				}
			}],
			colModelTmp:{sortable:false},
			headerFixed: true,
			resizable: true,
			multiSelect: true,
			multiSelectWidth:20,
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
			url: '/modules/device/getCollDataPage.action',//TODO 采集设备列表
			pager: '#deviceDatatablePage',
			pagerFixed: true,
			scrollWidth: 18,
			headTitle: true,
			ajaxDynamic: function(){
				return [{
					'name':'deviceInfo.strName',//初始化过滤条件
					value:deviceName
				},
				{
					'name':'deviceInfo.strIp',
					value:ip
				},
				{
					'name':'deviceInfo.strPasswordLevel',
					value:passwordLevel
				},
				{
					'name':'deviceInfo.online',
					value:onlineStatus
				},
				{
					'name':'deviceInfo.nregionId',
					value:parentId
				},
				{
					'name':'deviceInfo.isFlag',
					value:FMS.device.isFlag
				}];
			},
			events:{
				init:function(){
					FMS.globalFun.addVerticalLine();
				},
				onEndRequest:function(event,elm){
					FMS.globalFun.removeGridColumn('deviceDatatable',elm._pageData.totalPage);
					if(isIE6){
						if($('a.storageName','#deviceDatatable').length>0){
							$('a.storageName','#deviceDatatable').width(130);
						}
					}
				}
			}
		});
	},
	/**
	 * 获取设备通道信息
	 * @param  {[type]} dataType  [description]
	 * @param  {[type]} tabledata [description]
	 * @param  {[type]} type      [description]
	 * @return {[type]}           [description]
	 */
	channelGrid:function(dataType, tabledata, type){
		var tableId = "";
		if(dataType == 'local'){
			channeldata = tabledata;
			if(channeldata!=""&&type=='edit'&&channeldata!=false){
				$.sticky("获取通道成功", '提示', 'attention');
			}
			$(".ajaxTable").remove();
			tableId = "channelDatatable";
			$(".datatableStyle").removeClass('hidden');
		}else{
			channeldata = "";
			tableId = "ajaxDatatable";
		}
		$(".clickA").remove();
		$(".channelTable").show();
		$("#"+tableId).grid({
			colName:['通道号','通道类型','通道名称'],
			colModel:[{
				name: 'nchannelNo',
				index: 'nchannelNo',
				width: '300'
			},{
				name: 'nchannelType',
				index: 'nchannelType',
				width:'300',
				formart:function(d, fn){
					if(fn.nchannelNo<32){
						return '模拟通道';
					}else if(fn.nchannelNo>31&&fn.nchannelNo<64){
						return 'IP通道';
					}else{
						return '未知';
					}
				}
			},{
				name:'nchannelName',
				index:'nchannelName',
				width:'300'
			}],
			colModelTmp:{
				sortable:false
			},
			headerFixed: true,
			resizable: false,
			multiSelect:false,
			//rowNum: 15,
			ajaxRequestReader: {
				page: 'start',
				rowNum: "limit"
			},
			ajaxResponseReader: {
				rows: 'result'
			},
			rowMark: true,
			rowMarkWidth:40,
			sortorder: 'desc',
			height: '100%',
			ajaxOptions: {
				type: 'post'
			},
			dataType: 'local',
			data:tabledata,
			scrollWidth: 18,
			headTitle: true,
			events:{
				init:function(a, b){
					FMS.globalFun.addVerticalLine();
					$("div#gdd_"+tableId+"_rl").prepend('序号');
					$("#"+tableId).parent('div').css("overflow-x","hidden");
				}
			}
		});
	},
	/**
	 * 编辑弹出框初始化获取通道信息再用设备获取的方法展示
	 * @return {[type]} [description]
	 */
	initChannelGrid:function(){
		var device_id =[],result=[];
		device_id.push($("#editChannel").val());
		$.ajax({
			url:'/modules/device/getDevChannelByIds.action',
			type:'post',
			dataType:'json',
			data:{
				'ids':device_id
			},
			success:function(data){
				if(data&&data.success){
					var list = data.data.deviceChannelList;
					for(var i=0;i<list.length;i++){
						result.push(list[i]);
					}
					var ajaxData={};
					ajaxData.result = result;
					FMS.collectDevice.channelLoaGrid(ajaxData);
					channeldata="";
				}
			}
		});
	},
	channelLoaGrid:function(data){
		if(!data){
			$("input[name='deviceInfo.nchanNum'],input[name='deviceInfo.telChanNum']").val('');
			$("#sChannel,#ipChannel").text('');
			$("#channelDatatable").find('tbody').remove();
			return;
		}
		channeldata=data;
		var tableData = data.result;
		var tbody=$("<tbody></tbody>");
		for(var i=0;i<tableData.length;i++){
			var newTr = $("<tr></tr>");
			newTr.append('<td>'+(i+1)+'</td>');
			newTr.append('<td>'+tableData[i].nchannelNo+'</td>');
			if(tableData[i].nchannelNo<32){
				newTr.append('<td>模拟通道</td>');
			}else if(tableData[i].nchannelNo>31&&tableData[i].nchannelNo<64){
				newTr.append('<td>IP通道</td>');
			}else{
				newTr.append('<td>未知通道</td>');
			}
			newTr.append('<td>'+tableData[i].nchannelName+'</td>');
			if(i%2==1)newTr.addClass('oddTr');
			tbody.append(newTr);
		}
		$("#channelDatatable").find('tbody').remove();
		$("#channelDatatable").append(tbody);
	}
};
$(function(){
	FMS.device.globalSetting();
	FinanceUtil.initFormUI("#filterFormBox");
	FMS.device.bindEvent();
	FMS.collectDevice.init();
});