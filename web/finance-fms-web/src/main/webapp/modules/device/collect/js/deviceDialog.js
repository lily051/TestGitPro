var a=0;
$(function(){
	if (modeParam.usbAndIpc == "ipc" && modeParam.nonCenterStorage == "true") {
		$("#relateStorageDevice").hide();		
	}
	//var tableData = {};
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
	$(".clickA a").on('click', function(event) {
		//HIKChannelName();
		//$(this).text("正在获取通道信息,请稍后。。。");
		var form = $("#Dialog").find('form');
		if (FinanceUtil.validate(form)){
		}else{
			jAlert('信息输入有误', '错误', 'error');
			return false;
		}
		var channelInfo = getChannelInfo();
		if(!channelInfo){
			$(this).text("点击获取通道信息");
			return;
		}else{
			$(".clickA").remove();
			$.sticky("获取通道成功", '提示', 'attention');
		}
		FMS.collectDevice.channelLoaGrid(channelInfo);
		$(".channelTable").show();
		//setTimeout(function(){
			//
			
			//$(".channelInfo").load('/modules/device/collect/dialogs/channelInfo.jsp');
		//},2000);
	});
	$("#selectControls").on('click', function(event) {
		FMS.globalFun.stopPropagation(event);
		var eve = event.target || event.srcElement;
		if($(eve).closest('.ztree').length){
			return;
		}
		var $this = $(this);
		var type = $this.data("type");
		var editStorageId = '';
		if(type == 'edit'){
			editStorageId = 'dev_' + $("#storageId").val();
		}
		if($("#storageTree").children().length == 0){
			$("#storageTree").tree({
				ajax:{
					url:'/web/tree/getDeviceTree.action'//TODO 获取存储设备树
				},
				callback:{
					onNodeCreated:function(event, treeId, treeNode){
						if(editStorageId != '' && treeNode.id == editStorageId){
							$("#storageTree").tree().selectNode(treeNode,false);
						}
						$(".input-select").show();
						$(document).on('click', function(event){
							$(".input-select").hide();
							
							//$(document).off('click', hideSelect);
						});
					},
					onClick:function(event, treeId, treeNode){
						FMS.globalFun.stopPropagation(event);
						if(treeNode.id.indexOf("reg")>=0){//只能选择存储设备
							return;
						}else{
							var storageLabel = treeNode.label;
							var storageId = treeNode.id.split('_')[1];
							$("#selectDevice").val(storageLabel).blur();
							$("#storageId").val(storageId)
							$(document).click();
							if(typeof $("#editChannel").val()=="undefined"){
								getPoolInfo(storageId,'add');
							}else{
								getPoolInfo(storageId,'edit');
							}
							
						}
						
						
						
					}
				}
			});
		}else{
			$(".input-select").show();
			$(document).on('click', function(event) {
				$(".input-select").hide();
			});
		}
	});
	$("#getAgain").on('click', function(event) {
		var form = $("#Dialog").find('form');
		if (FinanceUtil.validate(form)){
		}else{
			jAlert('信息输入有误', '错误', 'error');
			return false;
		}
		jConfirm('确定重新获取？',{
				title:'提示',
				callback:function(value){
					if(value){
						$("#channelFlag").val(1);
						var channelInfo = getChannelInfo();
						if(channelInfo){
							$.sticky("获取通道成功", '提示', 'attention');
						}
						FMS.collectDevice.channelLoaGrid(channelInfo);
					}
				}
			});
	});
	$(".addcollect").on('click', 'p', function(event) {
		if(modeParam.usbAndIpc == "usb" && $(this).data("freesize") == 0){
			jSticky("KMS存储容量不允许等于0", "错误", "stop");
			return;
		}
		$(".addcollect p").removeClass('active');
		$(this).addClass('active');
		var selectSub = $(this).data("poolid");
		$("#subsection").val(selectSub);
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
function getChannelInfo(){
	try{
		if(!configOcxDialog('RPConfig_OCX')){
			return false;
		}
		var rpConfig = document.getElementById("RPConfig_OCX");
		rpConfig.RP_InitLib();
		var dev_ip = $("#deviceIp").val(),dev_port = $("#dev_port").val(),
		dev_user = $("#dev_user").val(),dev_pwd = FMS.device.base64($("#dev_pwd").val());
		var sendXML = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?><ROOT><CODE>8101</CODE><DEV_TYPE>0</DEV_TYPE><DEV_IP>"+dev_ip+"</DEV_IP><DEV_PORT>"+dev_port+"</DEV_PORT><DEV_USER>"+dev_user+"</DEV_USER><DEV_PWD>"+dev_pwd+"</DEV_PWD><DEV_CHANNEL>0</DEV_CHANNEL><DEV_SHOW_IP_MODE>0</DEV_SHOW_IP_MODE></ROOT>"
		var xml = rpConfig.RP_Function1(sendXML);
		if(xml == "登录设备失败!"){
			jAlert(xml,"错误","error");
			return false;
		}
		var xmldoc = FMS.globalFun.loadXML(xml);
		var elements = xmldoc.getElementsByTagName("CHANNEL");
		var result = [],channelInfo={},sChannel=0,ipChannel=0;
		for(var i=0;i<elements.length;i++){
			var nchannelNo,nchannelType,nchannelName;
			nchannelNo = Number(elements[i].getElementsByTagName("CHANNEL_ID")[0].firstChild.nodeValue);
			if(nchannelNo<32){
				nchannelType = "模拟通道";
				sChannel++;
			}else if(nchannelNo>31&&nchannelNo<64){
				nchannelType = "IP通道";
				ipChannel++;
			}else{
				nchannelType = "通道未知";
			}
			if(elements[i].getElementsByTagName("CHANNEL_NAME")[0].firstChild){	//避免通道名称为空
				nchannelName = String(elements[i].getElementsByTagName("CHANNEL_NAME")[0].firstChild.nodeValue);
			}else{
				nchannelName = nchannelType+(nchannelNo>=32?nchannelNo-32+1:nchannelNo+1);
			}
			result.push({"nchannelNo":nchannelNo,"nchannelType":nchannelType,"nchannelName":nchannelName});
		}
		$("#sChannel").text(sChannel);$("input[name='deviceInfo.nchanNum']").val(sChannel);
		$("#ipChannel").text(ipChannel);$("input[name='deviceInfo.telChanNum']").val(ipChannel);
		channelInfo.result = result;
		/*$("#deviceIp,#dev_port,#dev_user,#dev_pwd").attr({
			'readonly': 'readonly',
			'UNSELECTABLE': 'on'
		});*/
		return channelInfo;
	}catch(e){
		jAlert("获取通道错误！","错误","error");
		return false;
	}

}
/**
 * 查询数据库分卷信息
 * @param  {[type]} storageId 存储设备id
 * @param  {[type]} type      判断是新增还是编辑
 * @return {[type]}           [description]
 */
function getPoolInfo(storageId,type){
	var data;
	if(type=='add'){
		data = {'deviceInfo.ndevStoId': storageId};
	}else{
		data={
			'deviceInfo.ndevStoId': storageId,
			'deviceInfo.id': $("#editChannel").val()
		};
	}
	$.ajax({
		url: '/modules/device/getPoolsByDevId.action',//TODO
		type: 'post',
		dataType: 'json',
		data: data,
		success:function(data){
			if(data&&data.success){
				$(".addcollect").empty();
				$("#subsection").val('');
				var list = data.data.cloudpoolInfoList;
				for(var i=0;i<list.length;i++){
					var newP = $('<p data-poolid='+list[i].poolId+' data-freesize='+list[i].poolFreeSize+'>'+list[i].poolName+'：<span>剩余'+list[i].poolFreeSize+'G</span>（共'+list[i].poolSize+'G）</p>');
					if(list[i].checkedPoolId == list[i].poolId){
						newP.addClass('active');
						$("#subsection").val(list[i].poolId);
					}
					if(list[i].poolFreeSize==0){
						$('span',newP).css('color','#F00');
					}
					$(".addcollect").append(newP);
				}
			}
		},
		error:function(xhr){
			//jAlert(data.msg);
		}
	})
	/*.done(function() {
		console.log("success");
	})
	.fail(function() {
		console.log("error");
	})*/
}
function showPasswordLevel(){
	//安全等级显示
	switch($("#iRank").val()){
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
	}
}