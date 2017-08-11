var FMS = FMS || {};
FMS.sysconfig = {
	init:function(){
		this.bindEvent();
	},
	bindEvent:function(){
		$(".sysMain").load($(".systool ul li").eq(0).data("url"));
		$(".systool ul li").on('click', function(event) {
		    $(".systool ul li").removeClass('active');
		    $(this).addClass('active');
		    var url = $(this).data("url");
		    $(".sysMain").load(url);
		});
	},
	uploadPic:function(ext, id, url, errorFile){
		if(ext == ".jpg"||ext == ".png"||ext == ".bmp"){
			var form = $('#sysPicConfigForm');
			form.ajaxSubmit({
				beforeSubmit: function(){
					form.find(id).loading('show');
				},
				url:url,
				type:'post',
				dataType:'json',
				success:function(data){
					form.find(id).loading('hide');
					var fieldErrors=data.fieldErrors;
					if(!$.isEmptyObject(fieldErrors)){
						//$("#importDialog input").val("");
						switch(errorFile){
							case "navbarFile":jAlert(data.fieldErrors.navbarFile[0], '错误', 'error');break;
							case "logoFile":jAlert(data.fieldErrors.logoFile[0], '错误', 'error');break;
							case "loginFile":jAlert(data.fieldErrors.loginFile[0], '错误', 'error');break;
							default:break;
						}
						return;
					}
					if(data&&data.success){
						var photo = form.find(id);
						photo.attr("src",data.msg);//返回的图片路径
					}else{
						$(this).val("");
						jAlert(data.msg, '错误', 'error');
					}
				},
				error:function(){
					form.find(id).loading('hide');
				}
			});
		}else{
			jAlert("类型不符合,请上传jpg、png、bmp格式的图片", '错误', 'error');
			$(this).val("");
			return false;
		}
	},
	platformApp: function() {
		var logoFile = $('input[name="navbarFile"]').val();
		var loginFile = $('input[name="logoFile"]').val();
		var loginpageLogoFile = $('input[name="loginFile"]').val();
		if(logoFile!=""){
			var i = logoFile.lastIndexOf(".");
			var ext = logoFile.substring(i).toLowerCase();
			if(ext != ".jpg"&&ext != ".png"&&ext != ".bmp"){
				jAlert("平台Logo图片类型不符合,请上传jpg、png格式的图片", '错误', 'error');
				return;
			}
		}else if(loginFile!=""){
			var i = loginFile.lastIndexOf(".");
			var ext = loginFile.substring(i).toLowerCase();
			if(ext != ".jpg"&&ext != ".png"&&ext != ".bmp"){
				jAlert("登录页图片类型不符合,请上传jpg、png、bmp格式的图片", '错误', 'error');
				return;
			}
		}else if(loginpageLogoFile!=""){
			var i = loginpageLogoFile.lastIndexOf(".");
			var ext = loginpageLogoFile.substring(i).toLowerCase();
			if(ext != ".jpg"&&ext != ".png"&&ext != ".bmp"){
				jAlert("登录页图片类型不符合,请上传jpg、png、bmp格式的图片", '错误', 'error');
				return;
			}				
		}
		var form = $('#sysPicConfigForm');
		form.ajaxSubmit({
			beforeSubmit: function(){
				if(FinanceUtil.validate(form)){
					FinanceUtil.loader.show('请稍候...');
					return true;
				}else{
					jAlert('信息输入有误', '错误', 'error');
					return false;
				}
			},
			url:'/modules/sysconfig/updatePlatformParam.action',//TODO　平台信息应用
			type:'post',
			dataType:'json',
			success:function(data){
				FinanceUtil.loader.hide();
				if(data&&data.success){
					$.sticky('操作成功',{type:"ok",title:'完成'});
					FMS.sysconfig.cancelEdit();
					var src_logoimage=$('#navbarUrl').attr('src');
					$("#logoImg", parent.document).attr('src',src_logoimage);
					$("#logotitle").text($("#dscbstr").val());
					$("#logotitle").attr("title",$("#dscbstr").val());  //将修改后的平台描述文字赋值给属性title
				}else if(data&&data.actionError){
					jAlert(data.actionError[0], '错误', 'error');
				}else if(data&&data.fieldErrors){
					jAlert(data.fieldErrors.logoFile[0], '错误', 'error');
				}
				return false;
			},
			error:function(){
					FinanceUtil.loader.hide();
			}
		});
	},
	platformPreview: function() {
		var dscbstr = encodeURIComponent(encodeURIComponent($("#dscbstr").val()));
		var copyrtstr = encodeURIComponent(encodeURIComponent($("#copyrtstr").val()));
		window.open("/modules/sysconfig/preview.action?" //TODO 平台预览
					+"loginUrl="+$('#loginUrl').attr("src")
					+"&navbarUrl="+$('#navbarUrl').attr("src")
					+"&logoUrl="+$('#logoUrl').attr("src")
					+"&platformParamInfo.platformDiscribe="+dscbstr
					+"&platformParamInfo.platformCopyright="+copyrtstr);
	},
	renewDefault:function() {
		$.ajax({
			url: '/modules/sysconfig/resetPlatformParam.action',
			type:'post',
			dataType:'json',
			success:function(data){
				if(data && data.success){
					FMS.sysconfig.cancelEdit();
				}else if(data && data.msg){
					jAlert('操作错误','错误','error');
				}
			}
		});
	},
	videoParamApp:function(){
		var form = $("#videoParam").find("form");
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
			url: '/modules/sysconfig/updateRecordParam.action',//TODO 录像参数应用
			type: 'post',
			dataType: 'json',
			success: function(data) {
				FinanceUtil.loader.hide();
				if (data && data.success) {//录像参数应用成功后
					$.sticky('操作成功', {
						type: "ok",
						title: '操作成功',
						afterclose:function(){
							FMS.sysconfig.cancelEdit();
						}
					});
				} else if (data && data.msg) {
					jAlert(data.msg, '错误' , 'error');
				}
			}
		});
	},
	modelParamApp:function(){
		jConfirm('如果修改模式参数将改变系统模式，建议在系统首次配置时应用。是否继续？',{
			title:'提示',
			callback:function(value){
				if(value){
					var form = $("#modelParam").find("form");
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
						url: '/modules/sysconfig/updateModeParam.action',//TODO 模式参数应用
						type: 'post',
						dataType: 'json',
						success: function(data) {
							FinanceUtil.loader.hide();
							if (data && data.success) {//模式参数应用成功后
								$.sticky('操作成功', {
									type: "ok",
									title: '操作成功',
									afterclose:function(){
										FMS.sysconfig.cancelEdit();
									}
								});
							} else if (data && data.msg) {
								jAlert(data.msg, '错误' , 'error');
							}
						}
					});
				}
			}
		});
		
	},
	safetyApp:function(){
		var strPwd = $('#passwordDefault').val() === $('#passwordDefaultOld').val() ? $('#passwordDefault').val() : SHA256($('#passwordDefault').val());
		var form = $("#safety").find("form");
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
			url: '/modules/sysconfig/updatePasswordParam.action',//TODO 安全策略应用
			type: 'post',
			dataType: 'json',
			data:{
				"passwordParamInfo.defaultPassword":strPwd
			},
			success: function(data) {
				FinanceUtil.loader.hide();
				if (data && data.success) {//模式参数应用成功后
					$.sticky('操作成功', {
						type: "ok",
						title: '操作成功',
						afterclose:function(){
							FMS.sysconfig.cancelEdit();
						}
					});
				} else if (data && data.msg) {
					jAlert(data.msg, '错误' , 'error');
				}
			}
		});
	},
	bussinessParamAdd:function(){
		var keys = $(".leftBox input[type=checkbox]:checked").map(function(index, elem) {
			return $(this).val();
		}).get();
		if(keys.length == 0){
			jAlert("请选择要添加的参数！");
			return;
		}else{
			$.ajax({
				url: '/modules/sysconfig/enableBussinessParam.action',//TODO 添加业务参数
				type: 'post',
				dataType: 'json',
				data: {
					'keys': keys
				},
				success:function(data){
					//TODO 加载业务参数页面
					if(data && data.success){
						$.sticky('操作成功', {
							type: "ok",
							title: '操作成功'
						});
						FMS.sysconfig.cancelEdit();
					}else{
						jAlert(data.msg, '错误', 'error');
					}
				}
			});
		}
	},
	bussinessParamRemove:function(){
		var keys = $(".rightBox input[type=checkbox]:checked").map(function(index, elem) {
			return $(this).val();
		}).get();
		if(keys.length == 0){
			jAlert("请选择要移除的参数！");
			return;
		}else{
			$.ajax({
				url: '/modules/sysconfig/disableBussinessParam.action',//TODO 移除业务参数
				type: 'post',
				dataType: 'json',
				data: {
					'keys': keys
				},
				success:function(data){//TODO 加载业务参数页面
					if(data && data.success){
						$.sticky('操作成功', {
							type: "ok",
							title: '操作成功'
						});
						FMS.sysconfig.cancelEdit();
					}else{
						jAlert(data.msg, '错误', 'error');
					}
				}
			});
		}
	},
	bussinessParamEdit:function(value ,id){
		$.ajax({
			url: '/modules/sysconfig/editBussinessName.action',//TODO  编辑业务参数
			type: 'post',
			dataType: 'json',
			data: {
				'sysconfig.strDescribe': value,//编辑的值
				'sysconfig.nkey':id //编辑的id
			},
			success:function(data){
				//TODO 加载业务参数页面
				if(data && data.success){
					$.sticky('操作成功', {
						type: "ok",
						title: '操作成功'
					});
					FMS.sysconfig.cancelEdit();
				}else{
					jAlert(data.msg, '错误', 'error');
				}
				
			},
			error:function(){
				FMS.sysconfig.cancelEdit();
			}
		});
	},
	logSaveApp:function(){
		var form = $("#logSave").find("form");
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
			url: '/modules/sysconfig/updateLogParam.action',//TODO 日志保存天数应用
			type: 'post',
			dataType: 'json',
			success: function(data) {
				FinanceUtil.loader.hide();
				if (data && data.success) {//日志保存天数应用成功后
					$.sticky('操作成功', {
						type: "ok",
						title: '操作成功',
						afterclose:function(){
							FMS.sysconfig.cancelEdit();
						}
					});
				} else if (data && data.msg) {
					jAlert(data.msg, '错误' , 'error');
				}
			}
		});
	},
	ntpParamApp:function(){
		if (!FinanceUtil.validate($("#ntpForm").find("form"))) {
			jAlert('信息输入有误', '错误', 'error');
			return;
		}
		var ntpserverold = $('#ntpAddress').data('old')+$('#ntpPortNo').data('old')+$('#ntpInterval').data('old');
		var ntpserver = $('#ntpAddress').val()+$('#ntpPortNo').val()+$('#ntpInterval').val();
		
		if(ntpserverold && ntpserverold != "" && ntpserver != ntpserverold){
			jConfirm('NTP校时服务器信息已经变更，保存后需要手动更新设备NTP校时，是否继续？',{
				title:'提示',
				callback:function(value){
					if(value){
						_submit();
					}
				}
			});
		}else{
			_submit();
		}
		
		function _submit(){
			var form = $("#ntpForm").find("form");
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
				url: '/modules/sysconfig/updateNtpParam.action',
				type: 'post',
				dataType: 'json',
				data:{
					"ntpParamInfo.nenable":$('#ntpEnable').val()
				},
				success: function(data) {
					FinanceUtil.loader.hide();
					if (data && data.success) {
						$.sticky('操作成功', {
							type: "ok",
							title: '操作成功',
							afterclose:function(){
								FMS.sysconfig.cancelEdit();
							}
						});
					} else if (data && data.msg) {
						jAlert(data.msg, '错误' , 'error');
					}
				}
			});
		}
	},
	cancelEdit:function(){
		var url = $('.systool ul li.active').data("url");
		$(".sysMain").load(url);
	}
}
$(function(){
	if(isIE6){
		$(".sysMain").height($("#content").height()-50);
	}
	FMS.sysconfig.init();
});