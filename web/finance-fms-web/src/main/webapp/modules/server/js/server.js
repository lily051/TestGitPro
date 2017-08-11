 /**
 * @file FMS服务管理逻辑 
 * @author   Eli Pei
 */
var FMS = FMS || {};

FMS.server = {
	init: function() {
		
		this.eventBind();
	},
	eventBind: function(){
		//悬停出现操作栏方法
		$('#serverWrap').on('mouseenter','.serverLi',function(){
			$(this).find('.serverOption').stop().animate({top:0});
		}).on("mouseleave",".serverLi",function() {
			$(this).find('.serverOption').stop().animate({top:'158px'});
		});
		//添加服务
		$('#addServer').on('click',function(){
			FMS.server.addServer();
		});
		//编辑服务
		$('.serverEdit').on('click',function(){
			var id = $(this).parents('.serverOption').prev().val();
			var target = $(this).parents('.serverLi');
			var x,y,title;//根据不同类型弹出不同大小的dialog
			if(target.is('.uploadServer')){
				x = 400;
				y = 470;
				if(modeParam.usbAndIpc=="usb"){
					y=440;
				}
				title="编辑上传服务";
			}else if(target.is('.WebServer')){//中心web
				x = 400;
				y = 370;
				title="编辑web服务";
			}else if(target.is('.timingServer')){//系统巡检服务
				x = 440;
				y = 440;
				title="编辑系统巡检服务";
			}else if(target.is('.recGuardServer')){
				x = 400;
				y = 400;
				title="编辑录像守卫服务";
			}
			FMS.server.serverEdit(id,x,y,title);
		});
		//删除服务
		$('.serverDelete').on('click',function(){
			var id = $(this).parents('.serverOption').prev().val();
			FMS.server.serverDelete(id);
		});
		//重启服务
		$('.serverRestart').on('click',function(){
			var id = $(this).parents('.serverOption').prev().val();
			FMS.server.serverRestart(id);
		});
	},
	addServer: function(){
		FinanceUtil.dialog({
			id: 'serverSelect',
			title: '选择服务',
			overlay: true,
			draggable: true,
			width: 320,
			height: 300,
			iframeFix: false,
			ajax: {
				url: '/modules/server/getDicByNtype.action',
				success: function() {
					$('#serverWin').on('click','div',function(){
						var targrt = $(this).data('type');
						//根据type不同 调用不同函数
						if(targrt == Constants.ServerType.SERVICE_UPLOAD){
							if($(".uploadServer").length>=1){
								jAlert('已存在相同服务，不可添加', '错误', 'error');
								return false;
							}
							FMS.server.uploadServerAdd();
						}else if(targrt == Constants.ServerType.SERVICE_WEB){
							if ($('.WebServer').length >= 1){
								jAlert('已存在相同服务，不可添加', '错误', 'error');
								return false;
							}
							FMS.server.webServerAdd();
						}else if(targrt == Constants.ServerType.SERVICE_TIMING){
							if ($('.timingServer').length >= 1){
								jAlert('已存在相同服务，不可添加', '错误', 'error');
								return false;
							}
							FMS.server.timingServerAdd();
						}else if(targrt == Constants.ServerType.SERVICE_RECGUARD){
							if ($('.recGuardServer').length >= 1){
								jAlert('已存在相同服务，不可添加', '错误', 'error');
								return false;
							}
							FMS.server.recGuardServerAdd();
						}else{
							jAlert('添加服务错误', '错误', 'error');
							return false;
						}
					})
				}
			},
			buttons: {
				"cancel": {
					text: '取消',
					click: function() {
						$('#serverSelect').dialog('close');
					}
				}
			}
		});
	},
	//上传服务
	uploadServerAdd: function(){
		$('#serverSelect').dialog('close');
		var height=470;
		if(modeParam.usbAndIpc=="usb"){
			height=440;
		}
		FinanceUtil.dialog({
            id: 'uploadServer',
            title: '添加上传服务',
            overlay: true,
            draggable: false,
            width: 400,
            height: height,
            iframeFix: false,
            ajax: {
                url: '/modules/server/toUploadServerPage.action',
                success: function() {
                	$("input").placeholder();
                    FinanceUtil.initFormUI('#uploadServer');
                    FMS.globalFun.isIE6ChangeTipText('#uploadServer');
                }
            },
            buttons: {
                "ok": {
                    text: '完成',
				    'class': 'bPrimary',
                    click: function() {
                    	var form = $('#uploadServer').find('form');
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
							url: '/modules/server/addServer.action',//TODO
							type: 'post',
							dataType: 'json',
							success: function(data) {
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$('#uploadServer').dialog('close');
									$.sticky('添加上传服务成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
											
										}
									});
									FMS.server.reloadPage();
								} else if (data && data.msg) {
									jAlert(data.msg, '错误', 'error');
								}
							}
						});
                    }
                },
                "cancel": {
                    text: '取消',
                    click: function() {
                        $('#uploadServer').dialog('close');
                    }
                }
            }
        });
	},
	//校时服务
	timingServerAdd: function(){
		$('#serverSelect').dialog('close');
		FinanceUtil.dialog({
            id: 'timingServer',
            title: '系统巡检服务',
            overlay: true,
            draggable: false,
            width: 440,
            height: 440,
            iframeFix: false,
            ajax: {
                url: '/modules/server/toTimeServerPage.action',
                success: function() {
                	$("input").placeholder();
                    FinanceUtil.initFormUI('#timingServer');
                    FMS.globalFun.isIE6ChangeTipText('#timingServer');
                }
            },
            buttons: {
                "ok": {
                    text: '完成',
				    'class': 'bPrimary',
                    click: function() {
                    	var form = $('#timingServer').find('form');
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
							url: '/modules/server/addServer.action',
							type: 'post',
							dataType: 'json',
							success: function(data) {
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$('#timingServer').dialog('close');
									$.sticky('添加系统巡检服务成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
											
										}
									});
									FMS.server.reloadPage();
								} else if (data && data.msg) {
									jAlert(data.msg, '错误', 'error');
								}
							}
						});
                    }
                },
                "cancel": {
                    text: '取消',
                    click: function() {
                        $('#timingServer').dialog('close');
                    }
                }
            }
        });
	},
	//录像守卫
	recGuardServerAdd: function(){
		$('#serverSelect').dialog('close');
		FinanceUtil.dialog({
            id: 'recGuardServer',
            title: '录像守卫服务',
            overlay: true,
            draggable: false,
            width: 400,
            height: 400,
            iframeFix: false,
            ajax: {
                url: '/modules/server/toRecordServerPage.action',
                success: function() {
                	$("input").placeholder();
                    FinanceUtil.initFormUI('#recGuardServer');
                    FMS.globalFun.isIE6ChangeTipText('#recGuardServer');
                }
            },
            buttons: {
                "ok": {
                    text: '完成',
				    'class': 'bPrimary',
                    click: function() {
                    	var form = $('#recGuardServer').find('form');
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
							url: '/modules/server/addServer.action',//TODO
							type: 'post',
							dataType: 'json',
							success: function(data) {
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$('#recGuardServer').dialog('close');
									$.sticky('添加录像守卫服务成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
											
										}
									});
									FMS.server.reloadPage();
								} else if (data && data.msg) {
									jAlert(data.msg, '错误', 'error');
								}
							}
						});
                    }
                },
                "cancel": {
                    text: '取消',
                    click: function() {
                        $('#recGuardServer').dialog('close');
                    }
                }
            }
        });
		
	},
	webServerAdd: function(){
		var uploadIndex = $('.WebServer').length;
		if(uploadIndex != 0){
			return false;
		}//只允许一个上传服务
		$('#serverSelect').dialog('close');
		FinanceUtil.dialog({
            id: 'webServer',
            title: '添加服务',
            overlay: true,
            draggable: false,
            width: 400,
            height: 370,
            iframeFix: false,
            ajax: {
                url: '/modules/server/toBaseServerPage.action',
                success: function() {
                	$("input").placeholder();
                    FinanceUtil.initFormUI('#webServer');
                    FMS.globalFun.isIE6ChangeTipText('#webServer');
                }
            },
            buttons: {
                "ok": {
                    text: '完成',
                    'class': 'bPrimary',
                    click: function() {
                    	var form = $('#webServer').find('form');
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
							url: '/modules/server/addServer.action',//TODO
							type: 'post',
							dataType: 'json',
							success: function(data) {
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$('#webServer').dialog('close');
									$.sticky('添加中心web服务成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
											
										}
									});
									FMS.server.reloadPage();
								} else if (data && data.msg) {
									jAlert(data.msg, '错误', 'error');
								}
							}
						});
                    }
                },
                "cancel": {
                    text: '取消',
                    click: function() {
                        $('#webServer').dialog('close');
                    }
                }
            }
        });
	},
	serverEdit: function(id,x,y,title){
		FinanceUtil.dialog({
            id: 'serverEdit',
            title: title,
            overlay: true,
            draggable: false,
            width: x,
            height: y,
            iframeFix: false,
            ajax: {
                url: '/modules/server/toEditServerPage.action',//TODO
                param: {
					'serverInfo.id': id
				},
                success: function() {
                    FinanceUtil.initFormUI('#serverEdit');
                    FMS.globalFun.isIE6ChangeTipText('#serverEdit');
                }
            },
            buttons: {
                "ok": {
                    text: '完成',
				    'class': 'bPrimary',
                    click: function() {
                    	var form = $('#serverEdit').find('form');
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
							url: '/modules/server/updateServer.action',//TODO
							type: 'post',
							dataType: 'json',
							success: function(data) {
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$('#serverEdit').dialog('close');
									$.sticky(title+'成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
											
										}
									});
									FMS.server.reloadPage();
								} else if (data && data.msg) {
									jAlert(data.msg, '错误', 'error');
								}
							}
						});
                    }
                },
                "cancel": {
                    text: '取消',
                    click: function() {
                        $('#serverEdit').dialog('close');
                    }
                }
            }
        });
	},
	serverDelete: function(id){
		jConfirm('确认要删除当前记录吗？', {
			title: '提示',
			callback: function(value) {
				if (value) {
					$.ajax({
						url: '/modules/server/deleteServer.action',
						data: {
							'serverInfo.id': id
						},
						success: function(data) {
							if (data && data.success) {
								$.sticky('操作成功', {
									type: "ok",
									title: '操作成功',
									afterclose: function() { //'完成'
										
									}
								});
								FMS.server.reloadPage();
							}else {
									jAlert(data.msg, '错误', 'error');
								}
							}
						});
				}
			}
		});
	},
	serverRestart: function(id){
		jConfirm('确认要重启当前服务吗？', {
			title: '提示',
			callback: function(value) {
				if (value) {
					$.ajax({
						url: '/modules/server/restartServer.action',
						data: {
							'serverInfo.id': id
						},
						success: function(data) {
							if (data && data.success) {
								$.sticky('操作成功', {
									type: "ok",
									title: '操作成功',
									afterclose: function() { //'完成'
										
									}
								});
								FMS.server.reloadPage();
							}else {
									jAlert(data.msg, '错误', 'error');
								}
							}
						});
				}
			}
		});
	},
	reloadPage: function(){
		$('#content').load('/modules/server/getServerPage.action',function(){
	        $('.serverNtype').each(function(){
	            var _self = $(this);
	            var type = _self.data('type')
	            if(type == '20004'){ // SERVICE_UPLOAD: 20004, // 上传服务,
	                _self.parent().addClass('uploadServer');
	                $(".uploadServer").find("li.timeout").remove();
	                if(modeParam.usbAndIpc=="usb"){
	                    $(".uploadServer").find('.imgBox').removeClass('serverOffLine');
	                    $(".uploadServer").find(".serverStatus,li.must").remove();
	                }
	            }else if(type == '20200'){ // SERVICE_WEB: 20200,     //WEB服务
	                _self.parent().addClass('WebServer');
	            }else if(type == '20001'){ // SERVICE_TIMING: 20001, // 校时服务
	                _self.parent().addClass('timingServer');
	            }else if(type == '20002'){ // SERVICE_RECGUARD: 20002  //录像守卫服务
	                _self.parent().addClass('recGuardServer');
	            }
	            //不同服务设置不同class uploadServer WebServer timingServer recGuardServer           
	        });
	        if($(".serverLength").length==4){
	        	$("#addServer").hide();
	        }
	    });
	}
}

$(function(){
	FMS.server.init();
})