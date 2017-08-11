var FMS = FMS || {};
FMS.storageDevice = {
	init:function(){
		if(isIE6){
			var parentH = $("#content").height();
			$(".deviceMain").height(parentH-70);
			$(".datatableStyle").height(parentH-120);
		}
		this.storageGrid();
	},
	//新增存储设备
	addStorage:function(orgId){
		FinanceUtil.dialog({
			id:'Dialog',
			title: '添加存储设备',
			overlay:true,
			draggable:true,
			width:420,
			height:600,
			iframeFix:false,
			ajax:{
				url:'/modules/device/toAddStoPage.action',
				param: {
					'deviceInfo.nregionId':orgId
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
						var data = {};//分卷信息
						var cloudPoolInfoList = $(".viewInfo").data("poolInfo")||[];
						/*if(cloudPoolInfoList == undefined ||cloudPoolInfoList==''){
							jAlert("请获取分卷信息！",'错误','error');
							return false;
						}*/
						data.cloudPoolInfoList = cloudPoolInfoList;
						var access_Key = FMS.device.base64($("#accessKey").val());
						var secret_Key = FMS.device.base64($("#secretKey").val());
						var dev_pwd = FMS.device.base64($("#dev_pwd").val());
						var form = $("#Dialog").find('form');
						if (FinanceUtil.validate(form)){
							FinanceUtil.loader.show('请稍等...');
						}else{
							jAlert('信息输入有误', '错误', 'error');
							return;
						}
						if($('#ntpEnable').val()==1){//启用NTP校时
							var deviceInfo = {ntype:$("#deviceType").val(),strIp:$("#deviceIp").val(),nport:$("#dev_port").val(),strUser:$("#dev_user").val(),strPassword:dev_pwd}
							var ntpParamInfo = {nenable:$('#ntpEnable').val(),ntpAddress:$('#ntp_addr').val(), ntpPortNo:$('#ntp_port').val(), ntpInterval:$('#ntp_interval').val()}
							var result = FMS.device.rpNtpDevice(deviceInfo,ntpParamInfo);
							if(!result.success){
								FinanceUtil.loader.hide();
								jConfirm('NTP配置失败，确定要添加设备？',{
									title:'提示',
									callback:function(value){
										if(value){
											form.ajaxSubmit({
												url:'/modules/device/addSto.action',//TODO 添加存储设备
												type:'post',
												dataType:'json',
												data:{
													'jsonStr':$.jsonToStr(data),
													'deviceInfo.strPassword':dev_pwd,
													'deviceInfo.accessKey':access_Key,
													'deviceInfo.secretKey':secret_Key
												},
												success:function(data){
													FinanceUtil.loader.hide();
													if (data && data.success) {
														jAlert('操作成功', null, 'attention', function(){  
															if (data.data.isObjectStorage && data.data.isObjectStorage != null) {
																jAlert('对象存储未获取分卷信息将无法上传与回放录像');
															}
															$('#deviceDatatable').grid('reload');
															$('#Dialog').dialog('close');
														});
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
									url:'/modules/device/addSto.action',//TODO 添加存储设备
									type:'post',
									dataType:'json',
									data:{
										'jsonStr':$.jsonToStr(data),
										'deviceInfo.strPassword':dev_pwd,
										'deviceInfo.accessKey':access_Key,
										'deviceInfo.secretKey':secret_Key
									},
									success:function(data){
										FinanceUtil.loader.hide();
										if (data && data.success) {
//											$.sticky('操作成功', {type: "ok",title: '操作成功'});
											jAlert('操作成功', null, 'attention', function(){  
												if (data.data.isObjectStorage && data.data.isObjectStorage != null) {
													jAlert('对象存储未获取分卷信息将无法上传与回放录像');
												}
												$('#deviceDatatable').grid('reload');
												$('#Dialog').dialog('close');
											});
											
											
										}else if(data && data.msg){
											jAlert(data.msg, '错误' , 'error');
										}
									}
								});
							}
						}else {
							form.ajaxSubmit({
								url:'/modules/device/addSto.action',//TODO 添加存储设备
								type:'post',
								dataType:'json',
								data:{
									'jsonStr':$.jsonToStr(data),
									'deviceInfo.strPassword':dev_pwd,
									'deviceInfo.accessKey':access_Key,
									'deviceInfo.secretKey':secret_Key
								},
								success:function(data){
									FinanceUtil.loader.hide();
									if (data && data.success) {
										jAlert('操作成功', null, 'attention', function(){  
											if (data.data.isObjectStorage && data.data.isObjectStorage != null) {
												jAlert('对象存储未获取分卷信息将无法上传与回放录像');
											}
											$('#deviceDatatable').grid('reload');
											$('#Dialog').dialog('close');
										});
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
	//编辑存储设备
	editStorage:function(deviceId, devTypeStr, isRelated){
		FinanceUtil.dialog({
			id:'Dialog',
			title: '编辑存储设备',
			overlay:true,
			draggable:true,
			width:440,
			height:600,
			iframeFix:false,
			ajax:{
				url:'/modules/device/toUpdateStoPage.action',
				param: {
					'deviceInfo.id':deviceId
				},
				success:function(){
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI("#Dialog");
					FMS.globalFun.isIE6ChangeTipText('#Dialog');
					var poolIdOld = [], viewInfo = $('#Dialog .viewInfo');
					viewInfo.find('p').each(function(i){
						poolIdOld.push($(this).data('poolid'));
					});
					viewInfo.data('poolIdOld',poolIdOld);
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
						var access_Key = FMS.device.base64($("#accessKey").val());
						var secret_Key = FMS.device.base64($("#secretKey").val());
						var data = {};//分卷信息
						var cloudPoolInfoList = [];
						data.cloudPoolInfoList = [];
						/*var isEdit = {dev_ip:0,dev_port:0,dev_user:0,dev_pwd:0};
						isEdit.dev_ip = $("#deviceIp").val() == $("#deviceIp").prop("defaultValue")?0:1;
						isEdit.dev_port = $("#dev_port").val() == $("#dev_port").prop("defaultValue")?0:1;
						isEdit.dev_user = $("#dev_user").val() == $("#dev_user").prop("defaultValue")?0:1;
						isEdit.dev_pwd = $("#dev_pwd").val() == $("#dev_pwd").prop("defaultValue")?0:1;
						if(isEdit.dev_ip==1||isEdit.dev_port==1||isEdit.dev_user==1||isEdit.dev_pwd==1){
							jAlert("请重新获取分卷信息","提示");
							return;
						}*/
						if ($("#getInfo").data("click") == "1") {
							cloudPoolInfoList = $(".viewInfo").data("poolInfo");
							if(cloudPoolInfoList == undefined ||cloudPoolInfoList==''){
								data.cloudPoolInfoList = [];
							}else{
								data.cloudPoolInfoList = cloudPoolInfoList;
							}
							//如果存储池减少给出提示
							var poolIdOlds = $(".viewInfo").data('poolIdOld');
							var poolIds = $(".viewInfo").data('poolId');
							if(poolIdOlds && poolIdOlds.length>0){
								if(poolIds && poolIds.length>0){
									//循环判断存储池变更
									var flag = false;
									for(var i=0;i<poolIdOlds.length;i++){
										var poolId = poolIdOlds[i];
										if($.inArray(Number(poolId), poolIds)==-1){
											flag = true;
											jConfirm('存储池减少，对应的录像记录将被删除？',{
												title:'提示',
												callback:function(value){
													if(value){
														ajaxEditStorage();
													}
												}
											});
											break;
										}
									}
									if(flag){return; };
								}
							}
						}
						
						ajaxEditStorage();
						return;
						
						function ajaxEditStorage(){
							var dev_pwd = FMS.device.base64($("#dev_pwd").val());
							var form = $("#Dialog").find('form');
							if (FinanceUtil.validate(form)){
								FinanceUtil.loader.show('请稍等...');
							}else{
								jAlert('信息输入有误', '错误', 'error');
								return false;
							}
							var ntpEnable = $('#ntpEnable');
							if(ntpEnable && ntpEnable.val() != ntpEnable.data("ntp")){//NTP校时变更
								var deviceInfo = {ntype:$("#deviceType").val(),strIp:$("#deviceIp").val(),nport:$("#dev_port").val(),strUser:$("#dev_user").val(),strPassword:dev_pwd}
								var ntpParamInfo = {nenable:$('#ntpEnable').val(),ntpAddress:$('#ntp_addr').val(), ntpPortNo:$('#ntp_port').val(), ntpInterval:$('#ntp_interval').val()}
								var result = FMS.device.rpNtpDevice(deviceInfo,ntpParamInfo);
								if(!result.success){
									FinanceUtil.loader.hide();
									jConfirm('修改NTP校时状态失败(检查NTP配置和设备信息)，确定要编辑设备？',{
										title:'提示',
										callback:function(value){
											if(value){
												if (ntpEnable.val() == '1') {
													$('#ntpEnable').val('0');
												}else{
													$('#ntpEnable').val('1');
												}
												form.ajaxSubmit({
													url:'/modules/device/updateSto.action',//TODO 更新存储设备
													type:'post',
													dataType:'json',
													data:{
														'jsonStr':$.jsonToStr(data),
														'deviceInfo.strPassword':dev_pwd,
														'deviceInfo.accessKey':access_Key,
												   		'deviceInfo.secretKey':secret_Key,
												   		'deviceInfo.ntpEnable':$('#ntpEnable').val()
													},
													success:function(data){
														FinanceUtil.loader.hide();
														if (data && data.success) {
															jAlert('操作成功', null, 'attention', function(){  
																if (data.data.isObjectStorage && data.data.isObjectStorage != null) {
																	jAlert('对象存储未获取分卷信息将无法上传与回放录像');
																}
																$('#deviceDatatable').grid('reload');
																$('#Dialog').dialog('close');
															});
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
										url:'/modules/device/updateSto.action',//TODO 更新存储设备
										type:'post',
										dataType:'json',
										data:{
											'jsonStr':$.jsonToStr(data),
											'deviceInfo.strPassword':dev_pwd,
											'deviceInfo.accessKey':access_Key,
									   		'deviceInfo.secretKey':secret_Key
										},
										success:function(data){
											FinanceUtil.loader.hide();
											if (data && data.success) {
												jAlert('操作成功', null, 'attention', function(){  
													if (data.data.isObjectStorage && data.data.isObjectStorage != null) {
														jAlert('对象存储未获取分卷信息将无法上传与回放录像');
													}
													$('#deviceDatatable').grid('reload');
													$('#Dialog').dialog('close');
												});
											}else if(data && data.msg){
												jAlert(data.msg, '错误' , 'error');
											}
										}
									});
								}
							}else {
								form.ajaxSubmit({
									url:'/modules/device/updateSto.action',//TODO 更新存储设备
									type:'post',
									dataType:'json',
									data:{
										'jsonStr':$.jsonToStr(data),
										'deviceInfo.strPassword':dev_pwd,
										'deviceInfo.accessKey':access_Key,
								   		'deviceInfo.secretKey':secret_Key
									},
									success:function(data){
										FinanceUtil.loader.hide();
										var object = data.data.isObjectStorage;
										if (data && data.success) {
											jAlert('操作成功', null, 'attention', function(){  
												if (data.data.isObjectStorage && data.data.isObjectStorage != null) {
													jAlert('对象存储未获取分卷信息将无法上传与回放录像');
												}
												$('#deviceDatatable').grid('reload');
												$('#Dialog').dialog('close');
											});
										}else if(data && data.msg){
											jAlert(data.msg, '错误' , 'error');
										}
									}
								});
							}
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

	multDeleteStorage:function(){
		var row = $('#deviceDatatable').grid('getSelRow');
		var ids = $('#deviceDatatable').grid('getDataByID', row, 'id');
		if (!ids.length) {
			jAlert('请选择要删除的设备！');
		} else {
			jConfirm('确定删除选中的录像存储设备？',{
				title:'请谨慎删除',
				callback:function(value){
					if (value) {
						$.ajax({
							url: '/modules/device/deleteStos.action',
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
	storageGrid:function(){
		$("#deviceDatatable").grid({
			colName:['ids','设备用户名','设备密码','设备编码','设备名称','设备类型','设备IP','设备端口','密码等级','总容量(G)','可用容量(G)','在线状态','关联存储池','所属区域','校时'],
			colModel:[{
				name:'id',
				width:'1',
				hidden:true
			},{
				name:'strUser',
				width:'0',
				hidden:true
			},{
				name:'strPassword',
				width:'0',
				hidden:true
			},{
				name:'strIndexCode',
				width:'0',
				hidden:true
			},{
				name:'strName',
				index:'strName',
				sortable: false,
				formart:function(d, fn){
					return '<a class="storageName">' + d + '</a>';
				},
				width:140,
				attachEvent:{
					click:{
						el:'a.storageName',
						callback:function(a, b, e){
							var devTypeStr = $.trim($("#" + b).find('td').eq(6).text());
							var isRelated = $.trim($("#" + b).find('td').eq(13).text());
							FMS.globalFun.stopPropagation(e);
							var index = b.indexOf("_");
							var id = b.substring(index+1);
							FMS.storageDevice.editStorage(id, devTypeStr, isRelated);
						}
					}
				}
			},{
				name: 'ntype',
				index: 'ntype',
				sortable: false,
				formart: function(d, fn){//TODO 设备类型
					if(d==Constants.device.DEV_TYPE_CVR){
						return "<span class='gridTips' title='CVR'>CVR</span>";
					}else if(d == Constants.device.DEV_TYPE_CLOUD_STORAGE){
						return "<span class='gridTips' title='云存储'>云存储</span>";
					}else if(d == Constants.device.DEV_TYPE_KMS){
						return "<span class='gridTips' title='KMS'>KMS</span>";
					} else if(d == Constants.device.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE){
						return "<span class='gridTips' title='云存储-对象存储'>云存储-对象存储</span>";
					}else{
						return '未知';
					}
				},
				width:140
			},{
				name: 'strIp',
				index: 'strIp',
				sortable: false,
				width:140
			},{
				name:'nport',
				index:'nport',
				sortable: false,
				width:100
			},{
				name:'strPasswordLevel',
				index:'strPasswordLevel',
				sortable: false,
				formart: function(d, fn){//TODO 密码等级
					if(d==0){
						return "<span class='gridTips' title='风险密码'>风险密码</span>";
					}else if(d == 1){
						return "<span class='gridTips' title='弱密码'>弱密码</span>";
					}else if(d == 2){
						return "<span class='gridTips' title='中密码'>中密码</span>";
					}else if(d == 3){
						return "<span class='gridTips' title='强密码'>强密码</span>";
					}else if(d == 4){
						return "<span class='gridTips' title='无密码'>无密码</span>";
					}else{
						return '未知';
					}
				},
				width:100
			},{
				name:'uiTotalSpace',
				index:'uiTotalSpace',
				sortable: false,
				width:120
			},{
				name:'uiFreeSpace',
				index:'uiFreeSpace',
				sortable: false,
				width:130
			},{
				name:'online',
				index:'online',
				sortable: false,
				showTitle:false,
				formart:function(d, fn){
					if(d==0){
						return '<span>离线</span>';
					}else if(d == 1){
						return '<span class="onlineStyle">在线</span>';
					}else{
						return '未知';
					}
				},
				width:100
			},{
				name:'storageRelate',
				index:'storageRelate',
				sortable: false,
				showTitle:false,
				formart:function(d, fn){
					if(d==0){
						return '<span class="unableStyle">未关联</span>';
					}else if(d == 1){
						return '<span class="onlineStyle">已关联</span>';
					}else{
						return '<span class="unableStyle">未关联</span>';
					}
				},
				width:120
			},{
				name:'regionName',
				index:'regionName',
				sortable: false,
				width:100,
				formart:function(d, fn){
					return '<span>'+d+'</span>';
				}
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
			url: '/modules/device/getStoDataPage.action',//TODO 存储设备列表
			pager: '#deviceDatatablePage',
			pagerFixed: true,
			scrollWidth: 18,
			headTitle: true,
			ajaxDynamic: function(){
				return [{
					'name':'deviceInfo.strName',//parentId = deviceName = ip = passwordLevel = status = channeldata = "" ;//初始化过滤条件
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
				}
			}
		});
	}
};
$(function(){
	FMS.device.globalSetting();
	FinanceUtil.initFormUI("#filterFormBox");
	FMS.device.bindEvent();
	FMS.storageDevice.init();
});