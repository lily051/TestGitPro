var FMS = FMS || {};
FMS.datacollect = {
	recordStatus:1,
	videoTimeId:'',
	picturePath:"<?xml version=\"1.0\" encoding=\"UTF-8\"?><CfgData><PicPath>"+Constants.record.picturePath+"</PicPath><RecPath>"+Constants.record.recordPath+"</RecPath><MaxRecSize>0</MaxRecSize><PicType>1</PicType></CfgData>",
	conditions:{
		startTime:'',
		endTime:''
	},
	url:"",
	init:function(){
		initOcx({ocxId:'ocxRecord',divId:'previewOcx',ocxType:0});
		this.bindEvent();
		this.globalSetting();
		this.getVideoList(Constants.record.dataBase);
		/**
		 * 已选过通道后不弹出选择框
		 * modeParam.usbAndIpc == ipc/usb
		 */
		if(devInfo.strCollectIp == "null"&&devInfo.usbCamIndex== "null"){
			if(modeParam.usbAndIpc == "ipc"){
				if(judgeOcxAble('ocxRecord')){
					$.ajax({
						url:'/webdevice/datacollect/getChannels.action',//TODO 
						type:'post',
						dataType:'json',
						success:function(data){
							if(data&&data.success){
								var channelInfo = data.data.root;
								//只有一个通道直接选中那个通道
								if(channelInfo.length==1){
									$.ajax({
										url:'/webdevice/datacollect/setChannel.action',//TODO 提交选择的通道
										type:'post',
										dataType:'json',
										data:{
											'dataCollectInfo.deviceChannelId':channelInfo[0].id
										},
										success:function(data){
											FinanceUtil.loader.hide();
											if (data && data.success) {
												var recordsInfo = data.data.recordsInfo;
												
												FMS.datacollect.setDevInfo(recordsInfo);
												FMS.datacollect.previewRecord(recordsInfo);
											}else if(data && data.msg){
												jAlert(data.msg,'错误','error');
											}
										}
									});
								}else if(channelInfo.length==0){
									jAlert('您的账号未关联通道！');
								}else{
									FMS.datacollect.setChannel();
								}
							}
						},
						error:function(xhr){
							
						}
					});
					
				}
			}else{
				try{
					var usbXml = document.getElementById('ocxRecord').GetAllUSBCameraInfo(),
						usbInfo = FMS.globalFun.loadXML(usbXml),
						cameras = usbInfo.getElementsByTagName("Camera"),
						microPhones = usbInfo.getElementsByTagName("MicroPhone");
						if(cameras.length==1&&microPhones.length==1){
							var usbInfo = {};
								usbInfo.usbCamIndex = cameras[0].getElementsByTagName("CamIndex")[0].firstChild.nodeValue,
								usbInfo.usbCamName = cameras[0].getElementsByTagName("CamName")[0].firstChild.nodeValue,
								usbInfo.usbMicIndex = microPhones[0].getElementsByTagName("MicIndex")[0].firstChild.nodeValue,
								usbInfo.usbMicName = microPhones[0].getElementsByTagName("MicName")[0].firstChild.nodeValue;
							$.ajax({
								url:'/webdevice/datacollect/saveUsbInfo.action',
								type:'post',
								dataType:'json',
								data:{
									'usbrinfo.usbCamIndex':usbInfo.usbCamIndex,
									'usbrinfo.usbCamName':usbInfo.usbCamName,
									'usbrinfo.usbMicIndex':usbInfo.usbMicIndex,
									'usbrinfo.usbMicName':usbInfo.usbMicName
								},
								success:function(data){
								}
							});
							FMS.datacollect.previewRecord(usbInfo);
						}else if(cameras.length==0){
							jAlert("未检测到usb摄像头信息！");
						}else if(microPhones.length==0){
							jAlert("未检测到usb麦克风信息！");
						}else{
							this.getUsbInfo();
						}
				}catch(e){
					jAlert("未检测到usb摄像头信息！");
				}
				
			}
		}else{
			this.previewRecord(devInfo);
		}

	},
	globalSetting:function(){
		var startDate = new Date(new Date().getTime()-60*60*1000).Format("yyyy-MM-dd hh:mm:ss");
		var date=new Date().Format("yyyy-MM-dd hh:mm:ss");
		$("#startTime").val(startDate);
		$("#endTime").val(date);
		this.conditions.startTime = $("#startTime").val();
		this.conditions.endTime = $("#endTime").val();
	},	
	bindEvent:function(){
		var that = this;
		//绑定点击录像名回放的事件
		$("#list_content").on('click', "label.videoName",function(event) {
			var recordId = $(this).prev("input").val(),
				nrecordStatus = $(this).parent("li").data('status');
			FMS.dataBusiness.recordPlayBack(recordId,nrecordStatus,1);
		});
		$(".nav li").on('click', function(event) {
			var index = $(this).index();
			$(".nav li").removeClass('active');
			$(this).addClass('active');
			$(".main-wrap").find(".fullfit").addClass("hidden").eq(index).removeClass('hidden');
			if(index == 1){
				that.globalSetting();
				try{
					if(FMS.datacollect.recordStatus == 0){
						document.getElementById("ocxRecord").StopRecord(0);
					}else{
						document.getElementById("ocxRecord").StopPreview(0);
					}
					initOcx({ocxId:'recordBack',divId:'playBackOcx',ocxType:1,modelType:1});
				}catch(e){
					initOcx({ocxId:'recordBack',divId:'playBackOcx',ocxType:1,modelType:1});
					judgeOcxAble('recordBack');
				}
			}else{
				try{
					document.getElementById("recordBack").PB_StopAll();
				}catch(e){}
				$("#reRecord").remove();
				that.previewRecord(devInfo);
			}
		});
		
		$("#osdChange").on('click', function(event) {
			var $this = $(this);
			if($this.data("osd")==8){
				$this.text('8行水印').data("osd",4);
			}else{
				$this.text('4行水印').data("osd",8);
			}
			that.getOSDInfo();
		});
		$('#businessTip').on('click', function(event) {
			that.showProductTree();
		});
		$("#toolbar").on('click','span', function(event) {
			var target = event.target || event.srcElement;
			switch(target.id){
				case "oprRecord":FMS.dataBusiness.selectOrder();break;
				case "oprRelate":
								 var checkedInfo = that.getSelectCheckbox(),
								 	ids = checkedInfo.ids;
								 if(ids.length==0){
								 	jAlert("请选择要关联的录像！");
								 	break;
								 }else if(ids.length>1){
								 	jAlert("每次只能关联一个录像！");
								 	break;
								 }else{
								 	if(checkedInfo.checkedVideo.parent('li').data('status')!=1){
								 		jAlert('只能关联采集完成并且未关联的录像！');
								 		break;
								 	}else{
								 		FMS.dataBusiness.relateOrder(ids.join());//弹出签约单来关联录像
								 	}	
								 }break;
				case "oprUpload":that.uploadRecords();break;
				case "oprDelete":that.deleteRecords();break;
				case "oprPrint":
								var checkedInfo = that.getSelectCheckbox(),
									ids = checkedInfo.ids,
									checkedVideo = checkedInfo.checkedVideo;
								if(ids.length>1){
									jAlert("每次只能打印一个录像！");
									break;
								}else if(ids.length==0){
									jAlert("请选择要打印的一个录像！");
									break;
								}else{
									var nrecordStatus = checkedInfo.nrecordStatus.join();
									if(nrecordStatus<2 || nrecordStatus == 8){
										jAlert("只能打印已关联的录像！");
										break;
									}
									FMS.dataBusiness.printPicture(ids.join(),'1');break;
								}
								
				case "oprEditName":
									var checkedInfo = that.getSelectCheckbox(),
										ids = checkedInfo.ids,
										checkedVideo = checkedInfo.checkedVideo;
									if(ids.length>1){
										jAlert("每次只能编辑一个录像名！");
									}else if(ids.length==0){
										jAlert("请选择要编辑的一个录像！");
									}else{
										if(checkedVideo.parent('li').data('status')>1&&checkedVideo.parent('li').data('status')!=8){
											jAlert('已关联的录像不能修改名字！');
											checkedVideo.attr("checked",false);
											break;
										}else{
											FMS.dataBusiness.editVideoName(ids.join(),checkedVideo.next("label").text());
										}
									}break;
				case "offlineUpload":that.offlineUpload();break;
				default:break;

			}
		});
		$("#headRecord").on('click', '#recordSearch', function(event) {
			that.recordSearch();
		});
		$("#headRecord").on('click', '#reRecord', function(event) {
			that.addRecord();
		});
	},
	/**
	 * 获取右侧录像列表
	 * @param  {[type]} type 1、从数据库获取，2是从缓存中获取
	 * @return {[type]}      [description]
	 */
	getVideoList:function(type){
		$.ajax({
			url:'/webdevice/datacollect/getVideoList.action',//TODO 获取录像列表
			type:'post',
			dataType:'json',
			data:{
				'type':type
			},
			success:function(data){
				if(data&&data.success){
					var list_li = $("#list_content").find("input[name='videoBox']");
					var recordList = {};
					if(list_li.length){
						for(var i=0;i<list_li.length;i++){
							recordList[list_li[i].id] = list_li[i].checked;
						}
					}
					$("#list_ul").empty();
					var newUl = $("<ul id='list_ul'></ul>");
					var list = data.data.videoList;//获取到的数据
					
					for(var i=list.length-1;i>=0;i--){
						var nrecordStatus = list[i].nrecordStatus,//录像状态
							status_class = 'collecting',
							status_text = '正在采集',
							videoLabelClass='videoName';
						
						switch(nrecordStatus){
							case 0:status_class='collecting';videoLabelClass='collecting';status_text = '正在采集';break;
							case 1:status_class='collecting';status_text = '未关联';break;
							case 2:status_class='related';status_text = '已关联';break;
							case 3:status_class='uploaded';status_text = '上传中';break;
							case 4:status_class='uploaded';status_text = '已上传';break;
							case 8:status_class='unusual';status_text = '异常记录';break;
							case 9:status_class='unusual';status_text = '上传失败';break;
							default:break;
						}
						
						var checked = recordList['record_'+list[i].id] == undefined?false:recordList['record_'+list[i].id];
						var newLi = $('<li class="list_li" data-status='+nrecordStatus+'><input name="videoBox" type="checkbox" class="list_input" id="record_'+list[i].id+'" value='+list[i].id+'>'+
							'<label class='+videoLabelClass+' title="'+list[i].strRecordName+'">'+list[i].strRecordName+'</label><span class="list_status '+status_class+'">'+status_text+'</span></li>');
						newLi.find("input").attr("checked",checked);
						if(nrecordStatus==0)newLi.find("input").prop("disabled",true);
						if(i%2==0){
							newLi.addClass("odd");
						}
						if(nrecordStatus==9){newLi.find('span').attr("title",list[i].errorMsg)}
						newUl.append(newLi);
					}
					$("#list_content").empty().append(newUl);
						
					clearTimeout(FMS.datacollect.videoTimeId);
					FMS.datacollect.videoTimeId = setTimeout(function(){
						FMS.datacollect.getVideoList(Constants.record.cache);
					},15000);
				}
			}
		})
	},
	
	
	//视频补录回放查询
	recordSearch:function(){
		if(devInfo.nchannelNo=="null"){
			jAlert("未选择通道不能补录！");
			return;
		}
		this.conditions.startTime = $("#startTime").val();
		this.conditions.endTime = $("#endTime").val();
		var backXml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><Param><RecordType>255</RecordType><SearchType>0</SearchType>"+
		"<WndNo>0</WndNo><PlayType>0</PlayType><ManType>0</ManType><DvrType>100</DvrType><Right>255</Right>"+
		"<LoginInfo><IP>"+devInfo.strCollectIp+"</IP><Port>"+devInfo.dvrPort+"</Port><User>"+devInfo.dvrUserName+"</User><Password>"+devInfo.dvrPwd+"</Password><Rev>999</Rev></LoginInfo>"+
		"<PlayInfo><CamID>1001</CamID><CamName>9116</CamName><Channel>"+devInfo.nchannelNo+"</Channel>"+
		"<StartTime>"+FMS.datacollect.conditions.startTime+"</StartTime><StopTime>"+FMS.datacollect.conditions.endTime+"</StopTime>"+
		"<Rev>999</Rev><lBwControl>"+modeParam.recordBandwidth+"</lBwControl></PlayInfo></Param>";
		try{
			var sBack = document.getElementById("recordBack").PB_Playback(backXml);
			if(sBack == 1){
				if($("#reRecord").length==0)
				$("#headRecord").append('<span class="btn btn-cancel blockBtn btnGap" id="reRecord" style="_margin-top:-8px;">补录</span>');
			}else{
				$("#reRecord").remove();
			}
		}catch(e){

		}
	},
	/*视频补录向后台发信息*/
	addRecord:function(){
		this.conditions.startTime = $("#startTime").val();
		this.conditions.endTime = $("#endTime").val();
		var startTime = Date.parse(FMS.datacollect.conditions.startTime.replace(/-/g, '/')),
			endTime = Date.parse(FMS.datacollect.conditions.endTime.replace(/-/g, '/'));
		if((endTime-startTime)>60*60*1000*3){
			jAlert("补录时间不能超过3小时！");
			return;
		}
		$.ajax({
			url:'/webdevice/datacollect/supplementVideo.action',//TODO 
			type:'post',
			dataType:'json',
			data:{
				'records.startTime':FMS.datacollect.conditions.startTime,
				'records.endTime':FMS.datacollect.conditions.endTime
			},
			success:function(data){
				if(data&&data.success){
					$.sticky('视频补录成功', {type: "ok",title: '操作成功'});
					FMS.datacollect.getVideoList(Constants.record.dataBase);
				}
			},
			error:function(xhr){
				
			}
		});
	},
	deleteRecords:function(){
		var checkedInfo = this.getSelectCheckbox();
		if(checkedInfo.ids.length==0){
			jAlert("请选择要删除的录像！");
			return;
		}
		var nrecordStatus = checkedInfo.nrecordStatus;
		for(var i=0;i<nrecordStatus.length;i++){
			if(nrecordStatus[i]>1&&nrecordStatus[i]!=8){
				jAlert("已关联的录像不能删除！");
				return;
			}
		}
		jConfirm('你确定要删除选中的录像吗？',{
			title:'提示',
			callback:function(value){
				if(value){
					$.ajax({
						url:'/webdevice/datacollect/deleteRecords.action',//TODO 删除录像
						type:'post',
						dataType:'json',
						data:{
							'list':checkedInfo.ids
						},
						success:function(data){
							if(data&&data.success){
								$.sticky('删除成功', {type: "ok",title: '操作成功'});
								FMS.datacollect.getVideoList(Constants.record.dataBase);
							}else{
								jAlert(data.msg,'错误','error');
							}
						}
					});
				}
			}
		});
	},
	/**
	 * 上传录像
	 */
	uploadRecords:function(){
		var checkedInfo = this.getSelectCheckbox(),
			ids = checkedInfo.ids.join(",");
		
		
		if(ids.length==0){
			jAlert("请选择要上传的录像！");
			return;
		}else{
			var nrecordStatus=checkedInfo.nrecordStatus;
			for(var i=0;i<nrecordStatus.length;i++){
				if(nrecordStatus[i]!=9){
					jAlert("只能对上传失败的录像进行上传！");
					return;
				}
			}
		}
		
		$.ajax({
			url:'/webdevice/datacollect/uploadRecord.action',//TODO 上传录像
			type:'post',
			dataType:'json',
			data:{
				'ids':ids  
			},
			success:function(data){
				if(data&&data.success){
					$.sticky('操作成功', {type: "ok",title: '操作成功'});
					FMS.datacollect.getVideoList(Constants.record.dataBase);
				}
			},
			error:function(xhr){

			}
		});
	},
	/**
	 * 话术模板获取产品树
	 * @return {[type]} [description]
	 */
	showProductTree:function(){
		FinanceUtil.dialog({
			id:'productTreeDialog',
			title: '查看话术模板',
			overlay:true,
			draggable:false,
			width:480,
			height:400,
			iframeFix:true,
			modal:false,
			show:false,
			close:function(){
				$(".newOverLay").hide();
				if(isIE6)$("object").removeClass('hidden');
				//$("#videoRecord").removeClass('hidden');
			},
			ajax:{
				url:'/webdevice/contact/toOpenPageProductTree.action', //  /web/webDevice/datacollect/dialogs/productTree.jsp
				success:function(){
					//$("#videoRecord").addClass('hidden');
					$(".newOverLay").show();
					if(isIE6)$("object").addClass('hidden');
					FinanceUtil.initFormUI("#productTreeDialog");
				}
			},
			buttons:{
				"ok":{
					'text':"话术模板",
					'class':'bPrimary dosure',
					click:function(){
						var proTreeNode = FMS.dataBusiness.getSelectedTreeNode();
						if(!proTreeNode.isParent){
							var proId = proTreeNode.id.split("_")[1];
							$.ajax({
								url:'/webdevice/contact/getWordTemplate.action',//TODO 获取话术模板
								type:'post',
								dataType:'json',
								data:{
									'id':proId
								},
								success:function(data){
									if(data&&data.success){
										var wordInfo = data.data.wordTemplate;
										$("#wordTemp").val(wordInfo);
										$(".treeview,button.dosure").hide();
										$(".wordTemp,.selectOther").show();
									}else{
										jAlert(data.msg,'错误','error');
									}
								},
								error:function(xhr){
								}
							});				
						}else{
							jAlert("请选择产品查看话术模板")
						}
						
					}
				},
				'next':{
					'text':'选择其他模板',
					'class':'bPrimary selectOther',
					click:function(){
						if($("#productTree").children().length==0){
							FMS.dataBusiness.initProductTree();
						}
						$(".wordTemp,.selectOther").hide();	
						$(".treeview,button.dosure").show();
					}
				},
				'cancel':{
					text:'取消',
					click:function(){
						$('#productTreeDialog').dialog('close');
					}	
				}
			}
		})
	},
	setChannel:function(){
		FinanceUtil.dialog({
			id:'channelDialog',
			title: '请选择通道',
			overlay:true,
			draggable:false,
			width:400,
			height:150,
			modal:false,
			show:false,
			//closeButton:false,
			iframeFix:true,
			close:function(){
				$(".newOverLay").hide();
				if(isIE6)$("object").removeClass('hidden');
			},
			ajax:{
				url:'/webdevice/datacollect/openChannelPage.action',
				success:function(){
					$(".newOverLay").show();
					if(isIE6)$("object").addClass('hidden');
					//$("#channelDialog").parents(".popInner").find("a.popClose").remove();
					FinanceUtil.initFormUI("#channelDialog");
				}
			},
			buttons:{
				'ok':{
					text:'确定',
					'class':'bPrimary oneChannel',
					click:function(){
						var form = $("#channelDialog").find('form');
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
							url:'/webdevice/datacollect/setChannel.action',//TODO 提交选择的通道
							type:'post',
							dataType:'json',
							success:function(data){
								FinanceUtil.loader.hide();
								if (data && data.success) {
									var recordsInfo = data.data.recordsInfo;
									$.sticky('选择通道成功', {type: "ok",title: '操作成功'});
									$('#channelDialog').dialog('close');
									FMS.datacollect.setDevInfo(recordsInfo);
									FMS.datacollect.previewRecord(recordsInfo);
								}else if(data && data.msg){
									jAlert(data.msg,'错误','error');
								}
							}
						});
					}
				}
			}
		});
	},
	getUsbInfo:function(){
		FinanceUtil.dialog({
			id:'USBDialog',
			title: '请选择usb信息',
			overlay:true,
			draggable:false,
			width:400,
			height:200,
			//closeButton:false,
			iframeFix:true,
			modal:false,
			show:false,
			close:function(){
				//$("#videoRecord").removeClass('hidden');
				$(".newOverLay").hide();
				if(isIE6)$("#videoRecord object").removeClass('hidden');
			},
			ajax:{
				url:'/webdevice/datacollect/toOpenPageSelectUsb.action',//TODO  选择usb信息的页面  /web/webDevice/datacollect/dialogs/selectUSB.jsp
				success:function(){
					//$("#videoRecord").addClass('hidden');
					$(".newOverLay").show();
					if(isIE6)$("#videoRecord object").addClass('hidden');
					FinanceUtil.initFormUI("#USBDialog");
				}
			},
			buttons:{
				'ok':{
					text:'确定',
					'class':'bPrimary',
					click:function(){
						var form = $("#USBDialog").find('form');
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
							url:'/webdevice/datacollect/saveUsbInfo.action',//TODO 提交usb选择的通道 保存到session里面
							type:'post',
							dataType:'json',
							data:{
								'usbrinfo.usbCamIndex':$("#usbCamInfo").val(),//camIndex
								'usbrinfo.usbCamName':$("#usbCamInfo option:selected").text(),//camName
								'usbrinfo.usbMicIndex':$("#usbMicInfo").val(),//micIndex
								'usbrinfo.usbMicName':$("#usbMicInfo option:selected").text()//micName
							},
							success:function(data){
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$.sticky('选择成功', {type: "ok",title: '操作成功'});
									var usbInfo = {usbCamIndex:$("#usbCamInfo").val(),usbCamName:$("#usbCamInfo option:selected").text(),usbMicIndex:$("#usbMicInfo").val(),
													usbMicName:$("#usbMicInfo option:selected").text()};
									FMS.datacollect.previewRecord(usbInfo);
									$('#USBDialog').dialog('close');
								}else if(data && data.msg){
									jAlert(data.msg,'错误','error');
								}
							}
						});
					}
				}
			}
		});
	},
	/**
	 * 获取后台OSD信息
	 * @return {[type]} [description]
	 */
	getOSDInfo:function(){
		if(modeParam.osdEnable == "0"){
			var sendXml = "<?xml version=\"1.0\"encoding=\"UTF-8\"?><BussInfo><OSDNum>8</OSDNum></BussInfo>";
			try{
				document.getElementById("ocxRecord").SetShowString(sendXml);
			}catch(e){

			}
			return;
		}
		$.ajax({
			url:'/webdevice/contact/getosdInfo.action',//获取业务单信息
			type:'post',
			dataType:'json',
			success:function(data){
				if(data&&data.success){
					//给控件发消息
					var osdInfo = data.data.businessorder,
						userNo = data.data.user.userNo;

					if(osdInfo!=null){
						FMS.datacollect.showOSDInfo(osdInfo,userNo);
					}else{
						var sendXml = "<?xml version=\"1.0\"encoding=\"UTF-8\"?><BussInfo><OSDNum>8</OSDNum></BussInfo>";
						try{
							document.getElementById("ocxRecord").SetShowString(sendXml);
						}catch(e){

						}
						
					}
				}else{
					jAlert(data.msg,'错误','error');
				}
			}
		});
	},
	/**
	 * 预览显示OSD信息
	 * @param  {[type]} osdInfo 组装好的OSD信息
	 * @return {[type]}         [description]
	 */
	showOSDInfo:function(osdInfo,userNo){
		if(judgeOcxAble("ocxRecord")){
			try{
				var sendXml,osdXML,osdType;
				if ($("#osdChange").data("osd") == 8) {
					if(osdInfo.status==0){
						osdType="签约单";
					}else if(osdInfo.status==1){
						osdType="投诉单";
					}
					osdXML = "<Node>业务员工号:"+userNo+"</Node><Node>产品名称:"+osdInfo.productName+"</Node>"+
							"<Node>客户姓名:"+osdInfo.customerName+"</Node><Node>客户证件号:"+osdInfo.creditCode+"</Node>"+
							"<Node>业务单流水号:"+osdInfo.nproId+"</Node><Node>业务类型:"+osdType+"</Node>"+
							"<Node>办理时间:"+osdInfo.businessTime+"</Node><Node>产品编码:"+osdInfo.productCode+"</Node>";
					sendXml = "<?xml version=\"1.0\"encoding=\"UTF-8\"?><BussInfo><OSDNum>8</OSDNum>"+osdXML+"</BussInfo>";
				}else{
					osdXML = "<Node>业务员工号:"+userNo+"</Node><Node>产品名称:"+osdInfo.productName+"</Node>"+
							"<Node>客户姓名:"+osdInfo.customerName+"</Node><Node>客户证件号:"+osdInfo.creditCode+"</Node>";
					sendXml = "<?xml version=\"1.0\"encoding=\"UTF-8\"?><BussInfo><OSDNum>4</OSDNum>"+osdXML+"</BussInfo>";
				}
				if(modeParam.osdEnable == "0"){
					sendXml = "<?xml version=\"1.0\"encoding=\"UTF-8\"?><BussInfo><OSDNum>8</OSDNum></BussInfo>";
				}
				try{
					var osdAble = document.getElementById("ocxRecord").SetShowString(sendXml);
					/*if(osdAble==0){
						sendXml = "<?xml version=\"1.0\"encoding=\"UTF-8\"?><BussInfo><OSDNum>4</OSDNum>"+osdXML+"</BussInfo>";
						document.getElementById("ocxRecord").SetShowString(sendXml);
					}*/
				}catch(e){

				}
			}catch(e){
				jAlert("该浏览器不支持ocx控件，请换IE浏览器！");
			}
		}
	},
	/**
	 * 设置IPC模式预览的xml
	 */
	setIpcPreviewXml:function(deviceInfo){
		var previewXml;
		if(modeParam.usbAndIpc == "ipc"){
			previewXml =  "<?xml version=\"1.0\" encoding=\"UTF-8\"?><Preview><CamType>0</CamType><CamID>6</CamID><CamName>101_02</CamName>"+
						"<DevType>0</DevType><RegionID>2</RegionID><DevIP>"+deviceInfo.strCollectIp+"</DevIP><DevPort>"+deviceInfo.dvrPort+"</DevPort><User>"+deviceInfo.dvrUserName+"</User>"+
						"<Pwd>"+deviceInfo.dvrPwd+"</Pwd><Channel>"+deviceInfo.nchannelNo+"</Channel><RealPlayType>"+deviceInfo.streamType+"</RealPlayType><VideoFormat>0</VideoFormat><CamRight></CamRight>"+
						"<StreamIP></StreamIP><StreamPort>9557</StreamPort><Protocol>0</Protocol><UserID>3</UserID>"+
 						"<CollectorIP></CollectorIP><PhonePort>9527</PhonePort><VedioPort>9537</VedioPort><TimeOut>"+modeParam.maxVideo+"</TimeOut><DelayTime>"+modeParam.maxVideo+"</DelayTime></Preview>";
		}else if(modeParam.usbAndIpc == "usb"){
			previewXml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><Preview><CamType>2</CamType><CamIndex>"+deviceInfo.usbCamIndex+"</CamIndex>"+
			"<CamName>"+deviceInfo.usbCamName+"</CamName><MicIndex>"+deviceInfo.usbMicIndex+"</MicIndex><MicName>"+deviceInfo.usbMicName+"</MicName><TimeOut>"+modeParam.maxVideo+"</TimeOut><DelayTime>"+modeParam.maxVideo+"</DelayTime></Preview>";
		}
		return previewXml;
	},
	/**
	 * 离线文件上传
	 * @return {[type]} [description]
	 */
	offlineUpload:function(){
		FinanceUtil.dialog({
			id: 'importDialog',
			title: '上传离线录像',
			overlay: true,
			draggable: false,
			width: 500,
			height: 200,
			iframeFix: true,
			modal:false,
			show:false,
			close:function(){
				//$("#videoRecord").removeClass('hidden');
				$(".newOverLay").hide();
				if(isIE6)$("object").removeClass('hidden');
			},
			ajax: {
				url: '/webdevice/datacollect/toOpenPageLocalRecord.action',
				success: function() {
					//$("#videoRecord").addClass('hidden');
					$(".newOverLay").show();
					if(isIE6)$("object").addClass('hidden');
					FinanceUtil.initFormUI('#importDialog');
				}
			},
			buttons: {
				"ok": {
					text: '上传',
					'class': 'bPrimary',
					click: function() {
						var form = $('#importDialog').find('form#strUrlForm');
						var fileName = $('input[type="file"]').val(),
							fileNameIndex = fileName.lastIndexOf("."),
							fileNameExt = fileName.substring(fileNameIndex).toLowerCase();
						if(fileNameExt==".mp4"){
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
								url: '/webdevice/datacollect/saveLocalRecord.action',//TODO
								type: 'post',
								dataType: 'json',
								success: function(data) {
									FinanceUtil.loader.hide();
									if (data && data.success) {
										$.sticky('上传成功', {type: "ok",title: '操作成功'});
										FMS.datacollect.getVideoList(Constants.record.dataBase);
										$('#importDialog').dialog('close');
									} else if (data && data.msg) {
										jAlert(data.msg, '错误', 'error');
									}
								},
								error:function(xhr){
								}
							});
						}else{
							jAlert("文件格式不是mp4不允许上传",'错误', 'error');
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
	 * 设置缓存中的dvr信息
	 * @param recordsInfo
	 */
	setDevInfo:function(recordsInfo){
		devInfo.strCollectIp = recordsInfo.strCollectIp;
		devInfo.dvrPort = recordsInfo.dvrPort;
		devInfo.dvrUserName = recordsInfo.dvrUserName;
		devInfo.dvrPwd = recordsInfo.dvrPwd;
		devInfo.nchannelNo = recordsInfo.nchannelNo;
		devInfo.streamType = recordsInfo.streamType;
	},
	/**
	 * 预览录像modeParam.usbAndIpc:ipc/usb
	 * @return {[type]} [description]
	 */
	previewRecord:function(deviceInfo){
		var previewXml = this.setIpcPreviewXml(deviceInfo);
		if(judgeOcxAble("ocxRecord")){
			try{
				var ocxRecordID = document.getElementById("ocxRecord");
				var host = window.location.hostname;
				ocxRecordID.SetTrustedSite(host);
				ocxRecordID.StartPreview(previewXml);
				ocxRecordID.SetConfigData(FMS.datacollect.picturePath);
				//FMS.datacollect.getOSDInfo();
			}catch(e){
				//jAlert("该浏览器不支持ocx控件，请换IE浏览器！");
			}
		}
	},
	/**
	 * 获取选中的录像的信息
	 * @return {[type]} [description]
	 */
	getSelectCheckbox:function(){
		var checkedInfo={};
		var checkedVideo = $("input[name=videoBox]:checked");
		var ids = checkedVideo.map(function(index, elem) {
				return $(elem).val();
			}).get(),
			selectedLi = checkedVideo.parent('li');
		var nrecordStatus=selectedLi.map(function(index, el) {
				return $(el).data("status");
			}).get();
		checkedInfo.checkedVideo = checkedVideo;
		checkedInfo.ids = ids;
		checkedInfo.nrecordStatus = nrecordStatus;
		return checkedInfo;
	}
};
$(function(){
	if(isIE6){
		$("#videoRecord .fullfit").height($("#content").height());
		$("#previewOcx,#playBackOcx").height($("#content").height()-40);
		$(".newOverLay").height($("#wrapper").height());
		$(".moreInfo").hover(function() {
			$(".editInfo").show();
		}, function() {
			$(".editInfo").hide();
		});
	}
	if(modeParam.usbAndIpc == "usb"){
		$("#usbRemoveLi,#usbRemoveDiv").remove();
		$("#osdChange").hide();
		$("#offlineUpload").show();
	}else{

		$("#offlineUpload").remove();
	}
	if(modeParam.osdEnable == "0"){
		$("#osdChange").hide();
	}
	FMS.datacollect.init();
	$("#datacollect").addClass('active');
});
window.onbeforeunload = function(){
	if(FMS.datacollect.recordStatus == 0&&exitFlag==0&&$("#complainInfoOnBefore").length==0){
		event.returnValue="正在录像，请结束录像以后离开页面！";
	}
}
window.onunload = function(){
	/*if(FMS.datacollect.recordStatus == 0 && modeParam.usbAndIpc == "usb"){
		document.getElementById("ocxRecord").StopRecord(0);
	}*/
}