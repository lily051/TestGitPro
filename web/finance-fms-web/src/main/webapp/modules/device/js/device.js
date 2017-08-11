var FMS = FMS || {};
FMS.device = {
	isFlag:1,
	init:function(){
		initOcx({ocxId:'RPConfig_OCX',divId:'wrapper',ocxType:2});
		//rpConfigOCX('RPConfig_OCX','#wrapper');
		this.globalSetting();
		this.initOrgTree();//组织区域树
		//this.bindEvent();
	},
	globalSetting:function(){
		FinanceUtil.initFormUI("#filterFormBox");
		parentId = deviceName = ip = passwordLevel = onlineStatus = channeldata = channelName = "" ;//初始化过滤条件
		try{
			parentId = FMS.device.getSelectedTreeNode().id.split("_")[1];
		}catch(e){

		}
	},
	bindEvent:function(){
		var that = this;
		var treeInfo = $.fn.zTree.getZTreeObj('orgTree');
		var index = $('.devicetool li.active').index();
		$(".toolbar").on('click', '.oprAddDevice', function(event) {
			var selNode = FMS.device.getSelectedTreeNode();
			if(selNode == null){
				$.sticky("请选择要在哪个区域下添加设备", '提示', 'attention');
				return;
			}/*else if(selNode.id.split("_")[1] == 1){
				$.sticky("根节点下不能添加设备", '提示', 'attention');
				return;
			}*/
			if(index == 0){
				FMS.storageDevice.addStorage(selNode.id.split("_")[1]);
			}else{
				channeldata = ""
				FMS.collectDevice.addCollect(selNode.id.split("_")[1]);
			}
		});
		$(".toolbar").on('click', '.oprDeleteDevice', function(event) {
			if(index == 0){
				FMS.storageDevice.multDeleteStorage();
			}else{
				FMS.collectDevice.multDeleteCollect();
			}
		});
		$(".toolbar").on('click', '.oprImport', function(event) {
			that.importFun(index);
		});
		$(".toolbar").on('click', '.oprExport', function(event) {
			if(!FMS.globalFun.exportLimit('.ui-paging-info')) return false;
			that.exportFun(index);
		});
		$(".toolbar").on('click', '.oprRemote', function(event) {
			that.Remote();
		});
		$(".toolbar").on('click', '.oprNtp', function(event) {
			that.ntpTime();
		});
		$(".toolbar").on('click', '#filtrate', function(event) {
			FMS.globalFun.showSearchCondition($(this),'deviceDatatable');
			
		});
		$(".toolbar").on('change', '.showsub', function(event) {
			var _this = $(this);
			if(_this.attr("checked")){
				that.isFlag = 1;
			}else{
				that.isFlag = 0;
			}
			$("#deviceDatatable").grid("dofiltler");
		});
		$("#search").on('click', function(event) {
			deviceName = $("#deviceName").val();
			ip = $("#ip").val();
			userIp = $("#userIp").val();
			channelName = $("#channelName").val();
			passwordLevel = $("#passwordLevel").val();
			onlineStatus = $("#onlineStatus").val();
			//$("tr.ui-widget-content").remove();
			$("#deviceDatatable").grid("dofiltler");
		});
		$("#clear").on('click', function(event) {
			$("#deviceName,#ip,#passwordLevel,#onlineStatus,#channelName").val("");
			that.globalSetting();
			$("#deviceDatatable").grid("dofiltler");
		});
	},
	initOrgTree:function(){
		function nodeCreate(event, treeId, treeNode,type){
			if (treeNode.getParentNode() == null) {
				var treeObj = $.fn.zTree.getZTreeObj(treeId);
				treeObj.selectNode(treeNode);
				var node = treeObj.getSelectedNodes()[0];
				treeObj.expandNode(node, true,false ,true);
				parentId = treeNode.id.split('_')[1];
				if(type==1){
					$("#deviceDatatable").grid("dofiltler");
				}
			}
		}
		function nodeClick(event, treeId, treeNode){
			parentId = treeNode.id.split('_')[1];
			//$("#deviceDatatable").grid("reload");
			$("#search").click();
		}
		FMS.regionTree.init('orgTree','/web/tree/getRegionTree.action',nodeCreate,nodeClick);
	},
	getSelectedTreeNode:function(){
		var treeInfo = $.fn.zTree.getZTreeObj('orgTree');
		var selNode = treeInfo.getSelectedNodes()[0];
		return selNode;
	},
	exportFun:function(index){
		var url;
		if(index == 0){
			url = "/modules/device/exportStoDevExcel.action";
		}else{
			url = "/modules/device/exportCollDevExcel.action";
		}
		$.ajax({
			url:url,
			type:'post',
			dataType:'json',
			data:{
				'deviceInfo.strName':deviceName,
				'deviceInfo.strIp':ip,
				'deviceInfo.strPasswordLevel':passwordLevel,
				'deviceInfo.online':onlineStatus,
				'deviceInfo.nregionId':parentId,
				'deviceInfo.isFlag':FMS.device.isFlag
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
	importFun:function(index){
		var titleInfo,url,fileName,downTemp,alreadyUrl;
		if(index == 0){
			titleInfo = "导入存储设备";
			downTemp = "/modules/device/downLoadStoTemplate.action";
			url="/modules/device/stoUploadExcel.action";
			alreadyUrl = "/modules/device/toAlreadyStoImport.action";
		}else{
			titleInfo = "导入采集设备";
			downTemp = "/modules/device/downLoadCollTemplate.action";
			url="/modules/device/collUploadExcel.action";
			alreadyUrl = "/modules/device/toAlreadyStoImport.action";
		}
		FinanceUtil.dialog({
			id: 'importDialog',
			title: '导入',
			overlay: true,
			draggable: true,
			width: 500,
			height: 240,
			iframeFix: false,
			ajax: {
				url: '/modules/device/toCheckOutStoPage.action',
				success: function() {
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					$("#importDialog .templDown").data("url",downTemp)
					FinanceUtil.initFormUI('#importDialog');
				}
			},
			close: function(){
            	FMS.globalFun.isIE6ShowSelect("#wrapper");
            },
			buttons: {
				"ok": {
					text: '上传',
					'class': 'bPrimary',
					click: function() {
						var form = $('#importDialog').find('form');
						var fileName = form.find('input[type="file"]').val(),
							fileNameIndex = fileName.lastIndexOf("."),
							fileNameExt = fileName.substring(fileNameIndex).toLowerCase();
						if(fileNameExt==".xls"||fileNameExt==".xlsx"){
							form.ajaxSubmit({
								beforeSubmit: function() {
									if (FinanceUtil.validate(form)) {
										FinanceUtil.loader.show('请稍候...');
										return true;
									} else {
										jAlert('信息输入有误', '错误', 'error');
										return false;
									}
								},
								url: url,//TODO 上传设备文件
								type: 'post',
								dataType: 'json',
								success: function(data) {
									FinanceUtil.loader.hide();
									if(data&&data.fieldErrors.devFile){
										$("#importDialog input").val("");
										jAlert(data.fieldErrors.devFile[0], '错误', 'error');
										return;
									}
									var resMap = data.resMap;
									if (resMap.errorNo) {
										$('#deviceDatatable').grid('reload');
										$('#importDialog').dialog('close');
										if(resMap.errorInfo&&resMap.errorInfo != null){
											FinanceUtil.dialog({
												id:'importMsgDialog',
												title:'未导入的设备',
												overlay:true,
												draggable: true,
												width: 630,
												height: 430,
												iframeFix: false,
												ajax:{
													url:alreadyUrl,
													success:function(){
														$("#importMsgDialog #message").html(resMap.errorInfo.replace(/&/g, "<br/>"));
														FinanceUtil.initFormUI("#importMsgDialog");
													}
												},
												buttons:{
													"cancel": {
														text: '取消',
														click: function() {
															$('#importMsgDialog').dialog('close');
														}
													}
												}
											});
										}else{
											//$.sticky('导入成功', {type: "ok",title: '操作成功'});
											jAlert('导入成功', null, 'attention', function(){  
												if (resMap.isObjectStorage && resMap.isObjectStorage != null) {
													jAlert('对象存储未获取分卷信息将无法上传与回放录像');
												}
											});
										}
									}else {
										jAlert(resMap.errorInfo, '错误', 'error');
									}
								}
							});
						}else{
							jAlert("文件格式不是xls,xlsx不允许上传",'错误', 'error');
							return false;
						}
					}
				},
				"cancel": {
					text: '取消',
					click: function() {
						$('#importDialog').dialog('close');
					}
				}
			}
		});
	},
	/**
	 * [base64 加密
	 * @param  {[type]} pwd [description]
	 * @return {[type]}     [description]
	 */
	base64:function(pwd){
		var base = new Base64();  
		var str = base.encode(pwd);
		return str;
	},
	base64Decode:function(pwd){
		var base = new Base64();
		var str = base.decode(pwd);
		return str;
	},
	Remote:function(index){	
		var row = $('#deviceDatatable').grid('getSelRow');
		var ids = $('#deviceDatatable').grid('getDataByID', row, 'id');
		if(ids.length>1){
			jAlert("每次只能配置一个设备！");
			return;
		}else if(ids.length == 0){
			jAlert("请选择要配置的设备！");
			return;
		}
		var ip = $('#deviceDatatable').grid('getDataByID', row, 'strIp');
		window.open("http://"+ip);
		
	},
	ntpTime:function(){
		var t, ntpLoopFlag = true;;//setTimeout
		var that = this;
		var row = $('#deviceDatatable').grid('getSelRow');
		var ids = $('#deviceDatatable').grid('getDataByID', row, 'id');
		var names = $('#deviceDatatable').grid('getDataByID', row, 'strName');
		if(ids.length == 0){
			jAlert("请选择要校时的设备！");
			return ;
		}
		if(!configOcxDialog('RPConfig_OCX')){
			return ;
		}
		var ntpParamInfo = getNtpParam();
		if(ntpParamInfo.nenable != 1){
			jAlert("请设置NTP服务器信息！");
			return ;
		}
		var sum = ids.length, fail = 0, failArray = [] ;
		if(sum==1){
			FinanceUtil.loader.show('请稍等...');
			var result = ntp(ids[0], names[0]);
			setTimeout(function(){
				if(result.success){
					jAlert('设备校时成功',function(){$('#deviceDatatable').grid('reload')});
				}else{
					jAlert(result.msg, '错误', 'error');
				}
				FinanceUtil.loader.hide();},100);
			return ;
		}
		
		language["ui.progress.runBack"] = "停止校时";
		progress.show('0','开始校时');
		var i=0;
		setTimeout(ntpLoop, 100);
		
		function ntpLoop(){
			if(i < sum){
				var id = ids[i], deviceName = names[i];
				var index = i+1;
				progress.show(Math.floor(index*100/sum),'正在校时'+index+'/'+sum+'，失败<span style="color:#F00;">'+fail+'</span>');
				var result = ntp(id, names[i]);
				if(!result.success){
					fail += 1;
					failArray.push(result);
				}
				if(index > 10){
					progress.showButton(function(){
						i = sum;
					});
				}
				i++;
				setTimeout(ntpLoop, 100);
			}else{
				ntpLoopFlag = false;
				progress.hide(function(){
					if(fail > 0){
						var failmsg = "", len = failArray.length;
						for(var i=0;i<len;i++){
							var failRst = failArray[i];
							failmsg += failRst.deviceName+" "+failRst.msg+"<br>";
						}
						if(len>10){
							failmsg = '<div style="height: 200px;overflow: auto;">'+failmsg+'</div>';
						}
						jAlert('校时完成，成功 '+Math.floor(sum-fail)+'/'+sum+'，失败 <span style="color:#F00;">'+fail+'</span><br><br>'+failmsg,
								function(){
									$('#deviceDatatable').grid('reload');
								});
					}else{
						jAlert('校时完成，成功 '+Math.floor(sum-fail)+'/'+sum+'，失败 <span style="color:#F00;">'+fail+'</span>',
								function(){
									$('#deviceDatatable').grid('reload');
								});
					}
				});
			}
		}
		
		function ntp(id, deviceName){
			var result = {'success':false, 'deviceName':deviceName};
			$.ajax({
				url: '/modules/device/getDeviceJson.action',//获取信息
				type: 'post',
				dataType: 'json',
				data: {'id' : id},
				async:false, //同步
				success:function(data){
					if(data&&data.success){
						var deviceInfo = data.data.deviceInfo;
						//result.deviceName = deviceInfo.strName;
						//NTP校时
						result = $.extend(result, that.rpNtpDevice(deviceInfo, ntpParamInfo));
						if(result.success){
							$.ajax({
								url: '/modules/device/updateDeviceNtp.action',//
								type: 'post',
								dataType: 'json',
								data: {'id' : id},
								async:false, //同步
								success:function(data){
									if(data&&data.success){
										result.success = true;
									}else{
										result.success = false;
										result.msg = data.msg;
									}
								},
								error:function(xhr){
									//jAlert(data.msg);
								}
							});
						}
					}else{
						result.success = false;
						result.msg = data.msg;
					}
				},
				error:function(xhr){
					//jAlert(data.msg);
				}
			})
			return result;
		}
		
		function getNtpParam(){
			var ntpParamInfo = {};
			$.ajax({
				url: '/modules/sysconfig/getNtpParamInfoJson.action',
				type: 'post',
				dataType: 'json',
				async:false, //同步
				success:function(data){
					if(data&&data.success){
						ntpParamInfo = data.data.ntpParamInfo;
					}
				},
				error:function(xhr){}
			})
			return ntpParamInfo;
		}
	},
	
	/**
	 * 初始化密码强度
	 * @return {[type]} [description]
	 */
	initLevel:function(){
		var value = $('#dev_pwd').val();
		var strName = $('#dev_user').val();
		$('#strPasswordLevel').val(getPwdRank(value,strName));
		var iRank = $("#strPasswordLevel").val();
		iRankColor(iRank, value);
	},
	rpNtpDevice : function(deviceInfo, ntpParamInfo){
		try {
			if(deviceInfo.ntype == Constants.device.DEV_TYPE_KMS ){
				return {success:false,msg:"KMS不支持NTP校时！"};
			}else if(deviceInfo.ntype==Constants.device.DEV_TYPE_CLOUD_STORAGE){
				return {success:false,msg:"云存储请手动配置NTP校时！"};
			}else if(deviceInfo.ntype==Constants.device.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE){
				return {success:false,msg:"云存储-对象存储不支持NTP校时！"};
			}
			
			if(!configOcxDialog('RPConfig_OCX')){
				FinanceUtil.loader.hide();
				return ;
			}
			//0-海康；1-CVR；2-云存储
			var type ={"50000":"0","50012":"0","50011":"1","50015":"2"};
			
			var rpConfig = document.getElementById("RPConfig_OCX");
			rpConfig.RP_InitLib();
			var sendXML = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?> <ROOT><CODE>8103</CODE>"
				+"<DEV_TYPE>"+type[deviceInfo.ntype]+"</DEV_TYPE>"
				+"<DEV_IP>"+deviceInfo.strIp+"</DEV_IP><DEV_PORT>"+deviceInfo.nport+"</DEV_PORT>"
				+"<DEV_USER>"+deviceInfo.strUser+"</DEV_USER><DEV_PWD>"+deviceInfo.strPassword+"</DEV_PWD>"
				+"<DEV_CHANNEL>2</DEV_CHANNEL><DEV_SHOW_IP_MODE>0</DEV_SHOW_IP_MODE>"
				+"<NTP_SERVER>"+ntpParamInfo.ntpAddress+"</NTP_SERVER><NTP_PORT>"+ntpParamInfo.ntpPortNo+"</NTP_PORT><NTP_INTERVAL>"+ntpParamInfo.ntpInterval+"</NTP_INTERVAL><ENABLE_NTP>"+ntpParamInfo.nenable+"</ENABLE_NTP>"
				+"</ROOT>";
			var xml = rpConfig.RP_Function1(sendXML);
			if(xml != "1"){
				return {success:false,msg:xml};
			}else{
				return {success:true};
			}
		} catch (e) {
			return {success:false,msg:"校时失败"};
			return false;
		}
	}
};
$(function(){
	if(modeParam.usbAndIpc=="usb"){
		$(".devicetool").hide();
		$(".deviceMain").css("top",0);
	}
	FMS.device.init();
	$(".deviceMain").load($(".devicetool ul li").eq(0).data("url"));
	$(".devicetool ul li").on('click', function(event) {
		if(isIE6){
			isFirstLoad = true;
		}
	    $(".devicetool ul li").removeClass('active');
	    $(this).addClass('active');
	    FMS.device.isFlag=1;
	    var url = $(this).data("url");
	    $(".deviceMain").load(url);
	});
});
