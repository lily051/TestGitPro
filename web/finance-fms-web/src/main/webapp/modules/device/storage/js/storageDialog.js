$(function(){
	//showPasswordLevel();
	$('#dev_pwd').on('keyup',function(){
	    var value = $('#dev_pwd').val();
	    var strName = $('#dev_user').val();
	    var iRank = passwordColor(value,strName);
	    $("#strPasswordLevel").val(iRank);
	}).on('keydown',function(){
	    var capsLockKey = event.keyCode ? event.keyCode : event.which; //按键检测 space
	    if(capsLockKey == 32){
	        return false;
	    }
	});
	$('#getInfo').on('click', function(event) {
		$(this).data("click","1");
		$("#poolFlag").val(1);
		var form = $("#Dialog").find('form');
		if (FinanceUtil.validate(form)){
		}else{
			jAlert('信息输入有误', '错误', 'error');
			return false;
		}
		getSubInfo($("#deviceType").val());
		if($("#deviceType").val()!=Constants.device.DEV_TYPE_KMS){
			
		}
	});
	if($('#ntp_enable').val()==0){
		$('#ntpGroup').remove();
	}else{
		$('#ntpEnable').click(function(){
			if($(this).prop('checked')){
				$('#ntpEnable').val('1');
			}else{
				$('#ntpEnable').val('0');
			}
			$.uniform.update();
		});
	}
});
/**
 * 获取存储池容量
 * @param  {[type]} type 判断是kms还是cvr和云存储、云存储-对象存储
 * @return {[type]}      [description]
 */
function getSubInfo(type){
	if(type == Constants.device.DEV_TYPE_KMS){
		var dev_ip = $("#deviceIp").val(),dev_port = $("#dev_port").val(),
		dev_user = $("#dev_user").val(),dev_pwd = FMS.device.base64($("#dev_pwd").val());
		$.ajax({
		   url: "/modules/device/getKmsPoolsInfo.action",// TODO 获取kms容量
		   type: "post",
		   dataType: 'json',
		   async:true,
		   data: {
		   		'deviceInfo.strIp':dev_ip,
		   		'deviceInfo.nport':dev_port,
		   		'deviceInfo.strUser':dev_user,
		   		'deviceInfo.strPassword':dev_pwd
		   },
		   beforeSend:function(xhr){
		   		FinanceUtil.loader.show('请稍等...');
		   },
		   success: function (data) {
		   		FinanceUtil.loader.hide();
		   		if(data&&data.success){
		   			var kmsPool =$.parseJSON(data.data.kmsPool);
		   			var poolId = 0,poolName = "卷A",poolType = "",
		   			poolFreeSize = Math.floor(Number(kmsPool.freeSpace)/1024/1024/1024),
		   			poolSize = Math.floor(Number(kmsPool.totalSpace)/1024/1024/1024);
		   			if(poolFreeSize<1){
		   				jAlert("KMS剩余容量小于1G，存储不能正常使用。");
		   				return;
		   			}
		   			var subInfo = [], poolIds = [];
		   			subInfo.push({"poolId":poolId,"poolName":poolName,"poolType":poolType,"poolSize":poolSize,"poolFreeSize":poolFreeSize});
		   			poolIds.push(poolId);
		   			
		   			var viewInfo = $(".viewInfo");
		   			viewInfo.data("poolInfo",subInfo);
		   			viewInfo.data("poolId",poolIds);
		   			if(subInfo.length>0){
		   				$.sticky("获取存储池成功", '提示', 'attention');
		   				viewInfo.empty();
		   				for (var i = 0; i <  subInfo.length; i++) {
		   					var newP = $('<p>' + subInfo[i].poolName + '：剩余'+subInfo[i].poolFreeSize+'G（共'+subInfo[i].poolSize+'G）</p>');
		   					viewInfo.append(newP);
		   				};
		   				$(".groupHidden").show();
		   				$('.iScroll').scrollTop( $('.iScroll')[0].scrollHeight);
		   				/*$("#deviceIp,#dev_port,#dev_user,#dev_pwd").attr({
		   					'readonly': 'readonly',
		   					'UNSELECTABLE': 'on'
		   				});*/
		   			}
		   		}
		   },
		   complete:function(xhr){
		   	FinanceUtil.loader.hide();
		   },
		   error: function(xhr){
		   	clearPoolInfo();
		   }
		});
	} else if(type == Constants.device.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE){
		var dev_ip = $("#deviceIp").val(),
			dev_port = $("#dev_port").val(),	
			access_Key = FMS.device.base64($("#accessKey").val()),
			secret_Key = FMS.device.base64($("#secretKey").val());
		$.ajax({
		   url: "/modules/device/getCloudStorageObjectStorage.action",// TODO 获取云存储-对象存储的容量
		   type: "post",
		   dataType: 'json',
		   async:true,
		   data: {
		   		'deviceInfo.strIp':dev_ip,
		   		'deviceInfo.nport':dev_port,
		   		'deviceInfo.accessKey':access_Key,
		   		'deviceInfo.secretKey':secret_Key
		   },
		   beforeSend:function(xhr){
		   		FinanceUtil.loader.show('请稍等...');
		   },
		   success: function (data) {
		   		FinanceUtil.loader.hide();
		   		if(data&&data.success){
		   			var cloudPoolInfo = data.data.CloudPoolInfo;
		   			var poolSize = cloudPoolInfo.poolSize;
		   			var poolFreeSize = cloudPoolInfo.poolFreeSize;
		   			var poolName = cloudPoolInfo.poolName;
		   			var subInfo = [];
		   			subInfo.push({"poolName":poolName,"poolSize":poolSize,"poolFreeSize":poolFreeSize});

		   			var viewInfo = $(".viewInfo");
		   			viewInfo.data("poolInfo",subInfo);
		   			if(subInfo.length>0){
		   				$.sticky("获取存储池成功", '提示', 'attention');
		   				viewInfo.empty();
		   				for (var i = 0; i <  subInfo.length; i++) {
		   					var newP = $('<p>' + subInfo[i].poolName + '：剩余'+subInfo[i].poolFreeSize+'G（共'+subInfo[i].poolSize+'G）</p>');
		   					viewInfo.append(newP);
		   				};
		   				$(".groupHidden").show();
		   				$('.iScroll').scrollTop( $('.iScroll')[0].scrollHeight);
		   			}
		   		}
		   },
		   complete:function(xhr){
		   	FinanceUtil.loader.hide();
		   },
		   error: function(xhr){
		   	clearPoolInfo();
		   }
		});
	
	} else{
		//var RPConfig_OCX = document.getElementById('RPConfig_OCX');
		FinanceUtil.loader.show('请稍等...');
		setTimeout(function(){
        	var subInfo = [], poolIds = [];
    		try{
    			if(!configOcxDialog('RPConfig_OCX')){
    				FinanceUtil.loader.show('请稍等...');
    				return false;
    			}
    			var rpConfig = document.getElementById("RPConfig_OCX");
    			rpConfig.RP_InitLib();
    			var dve_type = $("#deviceType").val(),dev_ip = $("#deviceIp").val(),dev_port = $("#dev_port").val(),
    			dev_user = $("#dev_user").val(),dev_pwd = FMS.device.base64($("#dev_pwd").val());
    			if(dve_type  == Constants.device.DEV_TYPE_CVR){
    				dve_type = 1;
    			}else if(dve_type == Constants.device.DEV_TYPE_CLOUD_STORAGE){
    				dve_type = 2;
    			}else if(dve_type == Constants.device.DEV_TYPE_KMS){
    				dve_type = 3;
    			}
    			var sendXml = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?><ROOT><CODE>8100</CODE><DEV_TYPE>"+dve_type+"</DEV_TYPE><DEV_IP>"+dev_ip+"</DEV_IP>"+
    			"<DEV_PORT>"+dev_port+"</DEV_PORT><DEV_USER>"+dev_user+"</DEV_USER><DEV_PWD>"+dev_pwd+"</DEV_PWD><DEV_CHANNEL>2</DEV_CHANNEL><DEV_SHOW_IP_MODE>0</DEV_SHOW_IP_MODE></ROOT>";
    			//var xml = rpConfig.RP_Function1("<?xml version=\"1.0\" encoding=\"UTF-8\" ?> <ROOT><CODE>8100</CODE ><DEV_TYPE>3</DEV_TYPE><DEV_IP>10.10.48.231</DEV_IP><DEV_PORT>6021</DEV_PORT><DEV_USER>root</DEV_USER><DEV_PWD>6b74M23cd87D</DEV_PWD><DEV_CHANNEL>2</DEV_CHANNEL><DEV_SHOW_IP_MODE>0</DEV_SHOW_IP_MODE></ROOT>");
    			var xml = rpConfig.RP_Function1(sendXml);
    			if(xml == "登录CVR设备失败!"){
    				clearPoolInfo();
    				jAlert(xml,'错误','error');
    				FinanceUtil.loader.hide();
    				return;
    			}
    			if(xml == "登录Cloud失败!"){
    				clearPoolInfo();
    				jAlert("登录云存储失败!",'错误','error');
    				FinanceUtil.loader.hide();
    				return;
    			}
    			//xml = "<xml>"+xml.substring(xml.indexOf(">")+1)+"</xml>";
    			var xmldoc = FMS.globalFun.loadXML(xml);
    			var elements = xmldoc.getElementsByTagName("HDINFO");
    			for(var i=0;i<elements.length;i++){
    				var poolId = Number(elements[i].getElementsByTagName("HDNO")[0].firstChild.nodeValue);
    				var poolName = '',poolType='',poolSize='',poolFreeSize='';
    				switch(poolId){
    					case 0:poolName = "卷A";break;
    					case 1:poolName = "卷B";break;
    					case 2:poolName = "卷C";break;
    					case 3:poolName = "卷D";break;
    					case 4:poolName = "卷E";break;
    					case 5:poolName = "卷F";break;
    					case 6:poolName = "卷G";break;
    					case 7:poolName = "卷H";break;
    					case 8:poolName = "卷I";break;
    					case 9:poolName = "卷J";break;
    					default:poolName = "卷Z"+i;break;
    				}
    				poolType = Number(elements[i].getElementsByTagName("HDTYPE")[0].firstChild.nodeValue);
    				/*switch(poolTypeId){
    					case 0:poolType = "本地硬盘";break;
    					case 1:poolType = "ESATA硬盘";break;
    					case 2:poolType = "NAS硬盘";break;
    					case 3:poolType = "iSCSI硬盘";break;
    					case 4:poolType = "Array虚拟硬盘";break;
    					default:break;
    				}*/
    				poolSize = elements[i].getElementsByTagName("HDCAP")[0].firstChild.nodeValue;
    				poolSize = Math.floor(poolSize/1024);
    				poolFreeSize = elements[i].getElementsByTagName("HDFREESPACE")[0].firstChild.nodeValue;
    				poolFreeSize = Math.floor(poolFreeSize/1024);
    				subInfo.push({"poolId":poolId,"poolName":poolName,"poolType":poolType,"poolSize":poolSize,"poolFreeSize":poolFreeSize});
    				poolIds.push(poolId);
    			}
    			//var t = $(xmldoc).find("HDINFO")[0].find("HDNO").text();
    			var viewInfo = $(".viewInfo");
    			viewInfo.data("poolInfo",subInfo);
    			viewInfo.data("poolId",poolIds);
    			if(subInfo.length>0){
    				$.sticky("获取存储池成功", '提示', 'attention');
    				viewInfo.empty();
    				for (var i = 0; i <  subInfo.length; i++) {
    					var newP = $('<p>' + subInfo[i].poolName + '：剩余'+subInfo[i].poolFreeSize+'G（共'+subInfo[i].poolSize+'G）</p>');
    					viewInfo.append(newP);
    				};
    				$(".groupHidden").show();
    				$('.iScroll').scrollTop( $('.iScroll')[0].scrollHeight);
    				/*$("#deviceIp,#dev_port,#dev_user,#dev_pwd").attr({
    					'readonly': 'readonly',
    					'UNSELECTABLE': 'on'
    				});*/
    			}
    		}catch(e){
    			clearPoolInfo();
    		}
    	FinanceUtil.loader.hide();
		},4);
	}
	
}
/**
 * 清空存储池信息
 */
function clearPoolInfo(){
	var viewInfo = $(".viewInfo");
	viewInfo.data('poolInfo','');
	viewInfo.empty();
}
function showPasswordLevel(){
	//安全等级显示
	switch($('#iRank').val()){
	    case "0":
	        $('#pwdLvl').text('风险密码');
	        break;
	    case "1":
	        $('#pwdLvl').text('弱密码');
	        break;
	    case "2":
	        $('#pwdLvl').text('中密码');
	        break;
	    case "3":
	        $('#pwdLvl').text('强密码');
	        break;
	    case "4":
	        $('#pwdLvl').text('无密码');
	        break;
	}
}
