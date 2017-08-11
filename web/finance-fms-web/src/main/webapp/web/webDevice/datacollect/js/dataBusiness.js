var FMS = FMS || {};
FMS.dataBusiness = {
	conditions:{
		number:'',
		clientName:'',
		creditCode:''
	},
	/**
	 * 选择录入哪种业务单
	 */
	selectOrder:function(){
		FinanceUtil.dialog({
			id:'selectOrderDialog',
			title: '请选择',
			overlay:true,
			draggable:false,
			width:480,
			modal:false,
			show:false,
			height:279,
			iframeFix:true,
			close:function(){
				$(".newOverLay").hide();
				if(isIE6)$("object").removeClass('hidden');
			},
			ajax:{
				url:'/webdevice/contact/toOpenPageSelectOrder.action',
				success:function(){
					$(".newOverLay").show();
					if(isIE6)$("object").addClass('hidden');
					FinanceUtil.initFormUI("#selectOrderDialog");
				}
			},
			buttons:false
		});
	},
	
	/**
	 * 查看业务单		
	 * @param  {[type]} businessId 业务单id
	 */
	viewOrder:function(businessId, proid){
		FinanceUtil.dialog({
			id:'viewOrderDialog',
			title: '业务单详情',
			overlay:true,
			draggable:false,
			width:500,
			height:550,
			iframeFix:false,
			ajax:{
				url:'/webdevice/contact/toOpenPageOrderDetial.action',  
				param:{
					'id':businessId,
					'recordId':proid
				},
				success:function(){
					FMS.globalFun.isIE6ChangeTipText('#viewOrderDialog');
					if(isIE6) $("select","#content").hide();
				}
			},
			close:function(){
				if(isIE6) $("select","#content").show();
			},
			buttons:{
				'ok':{
					'text':'确定',
					'class':'bPrimary',
					click:function(){
						$('#viewOrderDialog').dialog('close');
						if(isIE6) $("select","#content").show();
					}	
				}
			}
		});
	},
	
	/**
	 * 编辑业务单		
	 * @param  {[type]} businessId 业务单id
	 */
	editOrder:function(businessId, proid){
		var that = this;
		FinanceUtil.dialog({
			id:'signOrderDialog',
			title: '编辑业务单',
			overlay:true,
			draggable:false,
			width:500,
			height:600,
			iframeFix:false,
			ajax:{
				url:'/webdevice/contact/toOpenPageEditOrder.action',  
				param:{
					'id':businessId,
					'recordId':proid
				},
				success:function(){
					FinanceUtil.initFormUI("#signOrderDialog");
					if(isIE6) $("select","#content").hide();
					FMS.globalFun.isIE6ChangeTipText('#signOrderDialog');
					that.initProductTree();
				}
			},
			close:function(){
				if(isIE6) $("select","#content").show();
			},
			buttons:{
				"ok":{
					'text':"确定",
					'class':'bPrimary',
					click:function(){
						var form = $("#signOrderDialog").find('form');
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
							url:'/webdevice/contact/editBusinessOrder.action',//TODO 提交编辑签约单    
							type:'post',
							dataType:'json',
							data:{
								'id':businessId,
								'recordId':proid
							},
							success:function(data){
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$.sticky('修改业务单成功', {type: "ok",title: '操作成功'});
									$("#datatable").grid("reload");
									if(isIE6) $("select","#content").hide();
									$('#signOrderDialog').dialog('close');
								}else if(data && data.msg){
									jAlert(data.msg, '错误' , 'error');
								}
							}
						});
					}
				},
				'reset':{
					text:'重置',
					'class':'resetButton',
					click:function(){
						$("#signOrderDialog input").each(function(index, el) {
							if(el.id!="nproId"){
								$(el).val('');
							}
							if(el.id=="businessTime"){
								var date = new Date().Format("yyyy-MM-dd hh:mm:ss");
								$(el).val(date);
							}
						});
						$("#signOrderDialog select").each(function(index, el) {
							$(el).find('option').eq(0).selected();
						});
						$("#signOrderDialog textarea").each(function(index, el) {
							$(el).val('');
						});
						$("#signOrderDialog span.show_span").each(function(index, el) {
							if(el.id!="nproId_span"){
								$(el).text('');
							}
						});
						FinanceUtil.initFormUI("#signOrderDialog");
						var form = $("#signOrderDialog").find('form');
						form.find('input').removeClass('error success');
						form.find('.error,.success').hide();
					}
				},
				'cancel':{
					text:'取消',
					click:function(){
						$('#signOrderDialog').dialog('close');
					}	
				}
			}
		});
	},
	/**
	 * 录入签约单
	 * @param  {[type]} recordId [description]
	 */
	signOrder:function(recordId){
		var that = this;
		FinanceUtil.dialog({
			id:'signOrderDialog',
			title: '签约关联',
			overlay:true,
			draggable:false,
			width:500,
			height:600,
			iframeFix:true,
			modal:false,
			show:false,
			ajax:{
				url:'/webdevice/contact/toOpenPageSignOrder.action',
				success:function(){
//					var date = new Date().Format("yyyy-MM-dd hh:mm:ss");
//					$("#businessTime").val(date);
					FinanceUtil.initFormUI("#signOrderDialog");
					FMS.globalFun.isIE6ChangeTipText('#signOrderDialog');
					that.initProductTree();
				}
			},
			buttons:{
				"ok":{
					'text':"确定",
					'class':'bPrimary',
					click:function(){
							/*提交新的业务单*/
						var form = $("#signOrderDialog").find('form');
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
							url:'/webdevice/contact/addSignOrder.action',//TODO 提交签约单    
							type:'post',
							dataType:'json',
							success:function(data){
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$.sticky('录单成功', {type: "ok",title: '操作成功'});
									FMS.datacollect.getOSDInfo();
									$('#signOrderDialog,#selectOrderDialog').dialog('close');
								}else if(data && data.msg){
									jAlert(data.msg, '错误' , 'error');
								}
							}
						});
					}
				},
				'reset':{
					text:'重置',
					'class':'resetButton',
					click:function(){
						$("#signOrderDialog input").each(function(index, el) {
							if(el.id!="nproId" && el.id!="businessTime"){
								$(el).val('');
							}
//							if(el.id=="businessTime"){
//								var date = new Date().Format("yyyy-MM-dd hh:mm:ss");
//								$(el).val(date);
//							}
						});
						$("#signOrderDialog select").each(function(index, el) {
							$(el).find('option').eq(0).selected();
						});
						$("#signOrderDialog textarea").each(function(index, el) {
							$(el).val('');
						});
						$("#signOrderDialog span.show_span").each(function(index, el) {
							if(el.id!="nproId_span"){
								$(el).text('');
							}
						});
						FinanceUtil.initFormUI("#signOrderDialog");
						var form = $("#signOrderDialog").find('form');
						form.find('input').removeClass('error success');
						form.find('.error,.success').hide();
					}
				},
				'cancel':{
					text:'取消',
					click:function(){
						$('#signOrderDialog').dialog('close');
					}	
				}
			}
		});
	},
	/**
	 * 投诉关联弹出框
	 */
	complainOrder:function(){
		var width = 1200;
		if($('body').width()<1200){
			width=800;
		}
		FinanceUtil.dialog({
			id:'complainOrderDialog',
			title: '投诉关联',
			overlay:true,
			draggable:false,
			width:width,
			height:700,
			iframeFix:true,
			modal:false,
			show:false,
			close:function(){
				FMS.dataBusiness.clearConditions();
			},
			ajax:{
				url:'/webdevice/contact/toOpenPageComplainOrder.action',
				success:function(){
					/*初始化弹出框时把搜索条件置空*/
					FMS.dataBusiness.clearConditions();
					if($('body').width()<1200){
						$(".ui-pager-control").width(760);
					}
					FinanceUtil.initFormUI("#complainForm,#doSureOrder");
					FMS.globalFun.isIE6ChangeTipText("#complainOrderDialog");
					
				}
			},
			buttons:{
				"ok":{
					'text':"确定",
					'class':'bPrimary sureButton',
					click:function(){
						var form = $("#complainOrderDialog #doSureOrder").find('form');
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
							url:'/webdevice/contact/addComplainOrder.action',//TODO 添加投诉单
							type:'post',
							dataType:'json',
							success:function(data){
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$.sticky('投诉关联成功', {type: "ok",title: '操作成功'});
									var row = $('#businessTable').grid('getSelRow'),
										rowInfo = $('#businessTable').grid('getDataByID', row)[0];
										rowInfo.customerName = rowInfo.name;
										rowInfo.status=1;
									FMS.datacollect.showOSDInfo(rowInfo,rowInfo.proUserNo);
									$('#complainOrderDialog,#selectOrderDialog').dialog('close');
								}else if(data && data.msg){
									jAlert(data.msg, '错误' , 'error');
								}
							}
						});
					}
				},
				'next':{
					'text':'下一步',
					'class':'bPrimary nextButton',
					click:function(){
						var row = $('#businessTable').grid('getSelRow');
						var ids = $('#businessTable').grid('getDataByID', row, 'id');
						if(!ids.length){
							jAlert("请选择一条业务记录！");
							return;
						}else if(ids.length>1){
							jAlert("只能选择一条业务记录！");
							return;
						}else{
							if($(".nextButton").data("step") == "preview"){
								$(".complainTitle").removeClass('step_two');
								$(".select_one").removeClass('select_two');
								$(".confirmInfo_one").removeClass('confirmInfo_two');
								$("#doSureOrder input").val('');
								$("#selectComOrder,#filtrate").show();
								$("#doSureOrder,.sureButton").hide();
								$(".nextButton").text("下一步").data("step",'next');
							}else{
								$(".complainTitle").addClass('step_two');
								$(".select_one").addClass('select_two');
								$(".confirmInfo_one").addClass('confirmInfo_two');
								$("#selectComOrder,#filtrate").hide();
								var orderInfo = $('#businessTable').grid('getDataByID', row)[0];
								var date = new Date().Format("yyyy-MM-dd hh:mm:ss");
								$("#complainTime","#doSureOrder").val(date);
								$("#doSureOrder span[name='nproId']").text(orderInfo.nproId);
								$("#doSureOrder span[name='clientName']").text(orderInfo.name);
								$("#doSureOrder span[name='creditCode']").text(orderInfo.creditCode);
								$("#doSureOrder input[name='businessorder.clientId']").val(orderInfo.id);
								$("#doSureOrder input[name='businessorder.nproId']").val(orderInfo.nproId);
								$("#doSureOrder input[name='businessorder.customerName']").val(orderInfo.name);
								$("#doSureOrder input[name='businessorder.creditCode']").val(orderInfo.creditCode);
								$("#doSureOrder input[name='businessorder.status']").val(orderInfo.status);
								$("#doSureOrder input[name='businessorder.nuserId']").val(orderInfo.inputUser);
								$("#doSureOrder input[name='businessorder.complainTime']").val(orderInfo.complainTime);
								$("#doSureOrder,.sureButton").show();
								$(".nextButton").text("上一步").data("step",'preview');
							}
						}
					}
				},
				'cancel':{
					text:'取消',
					click:function(){
						$('#complainOrderDialog').dialog('close');
					}	
				}
			}
		});
	},
	/**
	 * 录像关联业务单
	 * @param  {string} recordId 录像id
	 */
	relateOrder:function(recordId){
		var width = 1200;
		if($('body').width()<1200){
			width=800;
		}
		var closeButton=0;
		FinanceUtil.dialog({
			id:'relateOrderDialog',
			title: '关联业务单',
			overlay:true,
			draggable:false,
			width:width,
			height:600,
			iframeFix:true,
			modal:false,
			show:false,
			close:function(){
				FMS.dataBusiness.clearConditions();
				if(closeButton == 0){
					$(".newOverLay").hide();
					if(isIE6)$("object").removeClass('hidden');
				}
			},
			ajax:{
				url:'/webdevice/contact/toOpenPageViewSignOrder.action',
				success:function(){
					$(".newOverLay").show();
					if(isIE6)$("object").addClass('hidden');
					/*初始化弹出框时把搜索条件置空*/
					FMS.dataBusiness.clearConditions();
					FinanceUtil.initFormUI("#relateOrderDialog");
				}
			},
			buttons:{
				"ok":{
					'text':"关联",
					'class':'bPrimary',
					click:function(){
						if($("#newOrder").attr("aria-hidden")=="false"){//关联录入的业务单
							var form = $('#relateOrderDialog').find('form');
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
								url:'/webdevice/contact/signRelaRecords.action',//TODO 
								type:'post',
								dataType:'json',
								data:{
									'records.id':recordId//被选中的录像的id
								},
								success:function(data){
									FinanceUtil.loader.hide();
									if(data&&data.success){
										if(data.msg){
											jAlert(data.msg,function(){
												$.sticky('关联业务单成功', {type: "ok",title: '操作成功'});
												FMS.datacollect.getVideoList(Constants.record.dataBase);
												$('#relateOrderDialog').dialog('close');
											});
											
										}else{
											$.sticky('关联业务单成功', {type: "ok",title: '操作成功'});
											FMS.datacollect.getVideoList(Constants.record.dataBase);
											$('#relateOrderDialog').dialog('close');
										}
									}
								},
								error:function(xhr){
									FinanceUtil.loader.hide();
								}
							});
						}else{//关联历史业务单
							var row = $('#businessTable').grid('getSelRow');
							var ids = $('#businessTable').grid('getDataByID', row, 'id');
							if(!ids.length){
								jAlert("请选择一条业务记录！");
								return;
							}else if(ids.length>1){
								jAlert("只能选择一条业务记录！");
								return;
							}else{
								var form = $('#relateOrderDialog').find('form');
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
									url:'/webdevice/contact/oldSignRelaRecords.action',//TODO 关联历史签约单
									type:'post',
									dataType:'json',
									data:{
										'records.id':recordId,//被选中的录像的id
										'clientinput.id':ids//被选中的历史业务单的id
									},
									success:function(data){
										FinanceUtil.loader.hide();
										if(data&&data.success){
											if(data.msg){
												jAlert(data.msg,function(){
													$.sticky('关联业务单成功', {type: "ok",title: '操作成功'});
													FMS.datacollect.getVideoList(Constants.record.dataBase);
													$('#relateOrderDialog').dialog('close');
												});
												
											}else{
												$.sticky('关联业务单成功', {type: "ok",title: '操作成功'});
												FMS.datacollect.getVideoList(Constants.record.dataBase);
												$('#relateOrderDialog').dialog('close');
											}
										}
									},
									error:function(xhr){
										FinanceUtil.loader.hide();
									}
								});
							}
						}
					}
				},
				'again':{
					text:'重新录单',
					'class':'bPrimary',
					click:function(){
						FMS.dataBusiness.selectOrder();
						closeButton=1;
						$('#relateOrderDialog').dialog('close');
					}
				},
				'cancel':{
					text:'取消',
					click:function(){
						$('#relateOrderDialog').dialog('close');
					}	
				}
			}
		});
	},
	/**
	 * 编辑录像名称
	 * @param  {[type]} id        录像id
	 * @param  {string} videoName 录像名称
	 */
	editVideoName:function(id, videoName){
		FinanceUtil.dialog({
			id:'editNameDialog',
			title: '编辑录像名',
			overlay:true,
			draggable:false,
			width:480,
			height:170,
			iframeFix:true,
			modal:false,
			show:false,
			close:function(){
				$(".newOverLay").hide();
				if(isIE6)$("object").removeClass('hidden');
			},
			ajax:{
				url:'/webdevice/datacollect/toOpenPageEditVideoName.action',// /web/webDevice/datacollect/dialogs/editVideoName.jsp
				success:function(){
					$("#recordId").val(id);
					$("#recordName").val(videoName);
					$(".newOverLay").show();
					if(isIE6)$("object").addClass('hidden');
					FinanceUtil.initFormUI("#editNameDialog");
					FMS.globalFun.isIE6ChangeTipText('#editNameDialog');
				}
			},
			buttons:{
				'ok':{
					text:'确定',
					'class':'bPrimary',
					click:function(){
						var form = $("#editNameDialog").find('form');
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
							url:'/webdevice/datacollect/editRecordsName.action',//TODO 编辑录像名称
							type:'post',
							dataType:'json',
							success:function(data){
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$.sticky('编辑成功', {type: "ok",title: '操作成功'});
									FMS.datacollect.getVideoList(1);
									$('#editNameDialog').dialog('close');
								}else if(data && data.msg){
									jAlert(data.msg, '错误' , 'error');
								}
							}
						});
					}
				}
			}
		});
	},
	//投诉单号选择
	businessGrid:function(type){
		if(isIE6){
			isFirstLoad = true;
		}
		var url='/webdevice/contact/getDataPage.action';
		$("#businessTable").grid({
			colName:['ids','inputUser','productCode','proUserNo','strGrade','complainTime','流水号','业务类型','客户姓名','证件类型','客户证件号','产品名称','产品到期日','保障期限','签约时间'],
			colModel:[{
				name:'id',
				hidden:true
			},{
				name:'inputUser',
				hidden:true
			},{
				name:'productCode',
				hidden:true
			},{
				name:'proUserNo',
				hidden:true
			},{
				name:'strGrade',
				hidden:true
			},{
				name:'complainTime',
				hidden:true
			},{
				name:'nproId',
				width:'150'
			},{
				name:'status',
				formart:function(d, fn){
					if(d == Constants.record.CLIINPUT_TYPE_SIGN){
						return '签约';
					}else if(d == Constants.record.CLIINPUT_TYPE_COMPLAIN){
						return '投诉';
					}else{
						return '未知';
					}
				},
				width:'80'
			},{
				name:'name',
				width:'80'
			},{
				name:'creditName',
				width:'120'
			},{
				name:'creditCode'
			},{
				name:'productName'
			},{
				name:'strEndDate',
				width:'100'
			},{
				name:'guaranteeYears',
				width:'80',
				formart:function(d, fn){
					if(fn['guaranteeYears']){
						var text={'0':'年','1':'月','2':'日'};
						return fn['guaranteeYears']+text[fn['guaranteeType']];
					}
					return '';
				}
			},{
				name:'businessTime'
			}],
			colModelTmp:{
				sortable:false
			},
			headerFixed: true,
			resizable: false,
			multiSelect: true,
			multiSelectWidth:18,
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
			rowMarkWidth:25,
			sortorder: 'desc',
			height: '100%',
			ajaxOptions: {
				type: 'post'
			},
			dataType: 'ajax',
			url: url,//TODO 投诉关联列表
			pager: '#businessTablePage',
			pagerFixed: true,
			scrollWidth: 18,
			headTitle: true,
			ajaxDynamic: function(){
				return [{
					'name':'clientinput.nproId',
					value:FMS.dataBusiness.conditions.number
				},{
					'name':'clientinput.name',
					value:FMS.dataBusiness.conditions.clientName
				},{
					'name':'clientinput.creditCode',
					value:FMS.dataBusiness.conditions.creditCode
				},{
					'name':'type',
					value:type
				}];
			},
			events:{
				init:function(a, b){
					FMS.globalFun.addVerticalLine();
				},
				onEndRequest:function(event,elm){
					FMS.globalFun.removeGridColumn('businessTable',elm._pageData.totalPage);
				}
			}
		})
	},
	initProductTree:function(){
		var treeInfo = refresh();
		function refresh(type){
			var tree = $("#productTree").tree({
				ajax:{
					url:'/web/tree/getPermitProductTree.action'//TODO 
				},
				callback: {
					onNodeCreated:function(event, treeId, treeNode){
						if (treeNode.getParentNode() == null) {
							var treeObj = $.fn.zTree.getZTreeObj(treeId);
							treeObj.selectNode(treeNode);
							var node = treeObj.getSelectedNodes()[0];
							treeObj.expandNode(node, true,false ,true);
						}
					},
					onClick:function(event, treeId, treeNode){
						if($("#signOrderDialog").length>0){
							//获取产品编码
							if(treeNode.id.indexOf("product_")>-1){
								$("#productName",'#signOrderDialog').val(treeNode.label).blur();
								var proInfo = treeNode.extra;
								proInfo.productId = treeNode.id.split("_")[1];
								proInfo.productTypeId = treeNode.parentId.split("_")[1];
								for(var key in proInfo){
									var keyInfo = proInfo[key] || "";
									if(key == "strRisklevel"){
										if(keyInfo==1){
		    								$("#strRisklevel_span").text(Constants.RiskLevel.RISKLEVEL_ONE);
		    							}else if(keyInfo==2){
		    								$("#strRisklevel_span").text(Constants.RiskLevel.RISKLEVEL_TWO);
		    							}else if(keyInfo==3){
		    								$("#strRisklevel_span").text(Constants.RiskLevel.RISKLEVEL_THREE);
		    							}else if(keyInfo==4){
		    								$("#strRisklevel_span").text(Constants.RiskLevel.RISKLEVEL_FOUR);
		    							}else if(keyInfo==5){
		    								$("#strRisklevel_span").text(Constants.RiskLevel.RISKLEVEL_FIVE);
		    							}else if(keyInfo==6){
		    								$("#strRisklevel_span").text(Constants.RiskLevel.RISKLEVEL_SIX);
		    							}else if(keyInfo==0){
		    								$("#strRisklevel_span").text(Constants.RiskLevel.RISKLEVEL_NONE);
		    							}
										$("#strRisklevel_input").val(keyInfo)
										continue;
									}
									if(key == "strCode"){
										$(".custom-combobox input,#productCode","#signOrderDialog").val(keyInfo);
										continue;
									}
									$("#"+key,"#signOrderDialog").val(keyInfo);
									if(key == "saveYears"&&keyInfo!=""){
										var text={'0':'年','1':'月','2':'日'};
										$("input[name='businessorder.productSaveType']").val(proInfo["saveType"]);
										$("#"+key+"_span").text(keyInfo+text[proInfo["saveType"]]);
										continue;
									}
									if(key == "guaranteeYears"&&keyInfo!=""){
										var text={'0':'年','1':'月','2':'日'};
										$("input[name='businessorder.guaranteeType']").val(proInfo["guaranteeType"]);
										$("#"+key+"_span").text(keyInfo+text[proInfo["guaranteeType"]]);
										continue;
									}
									$("#"+key+"_span").text(keyInfo);
								}
								$(".input_dropdown","#signOrderDialog").hide();
							}
							
						}
					}
				}
			});
			return tree;
		}
		$(".reload").click(function(){//刷新树
			treeInfo = refresh(1);
		});
		$("#orgTreeSearch").on('click', function(event) {
			searchTree('productTree','label');
		});
		$("#productInfo .treeDelete").on('click', function(event) {
			$('#dropdown_productName').val('');
			$(this).hide();
		});
		$('#dropdown_productName').keyup(function(e) {
			var key = e.keyCode;
			if (key == 13) {
				$(this).prev().click();
				if($(this).val().length>0){
					$("#productInfo .treeDelete").show();
				}
				return false;
			} else {
				if($(this).val().length>0){
					$("#productInfo .treeDelete").show();
				}else{
					$("#productInfo .treeDelete").hide();
				}
				
			}
		});
		function searchTree(treeId, label){
			var value = $('#dropdown_productName').val();
			if(value == ""){
				treeInfo = refresh();
			}else{
		        var treeObj = $.fn.zTree.getZTreeObj(treeId);  
		        var nodes = treeObj.transformToArray(treeObj.getNodes());
				treeObj.hideNodes(nodes);//隐藏所有节点
		        var nodeList = treeObj.getNodesByParamFuzzy(label, value,null); //查询出来的节点
		        for(var index=0;index<nodeList.length;index++){
		        	treeObj.showNode(nodeList[index]);
		        	hasParentNode(nodeList[index],treeObj);
		        }
			}
		}
		//判断父节点是否可以显示
		function hasParentNode(node,treeObj) {
			var currnode = node.getParentNode();
			treeObj.showNode(currnode);
			if(currnode){
				hasParentNode(currnode,treeObj);
			}else{
			    treeObj.expandNode(node, true, true, true);
			}
		}
	},
	//加载组织树
	initRegionTree:function(){
		var treeInfo = refresh();
		function refresh(type){
			var tree = $("#regionTree").tree({
				ajax:{
					url:'/web/tree/getAllRegionTree.action'
				},
				callback: {
					onNodeCreated:function(event, treeId, treeNode){
						if (treeNode.getParentNode() == null) {
							var treeObj = $.fn.zTree.getZTreeObj(treeId);
							treeObj.selectNode(treeNode);
							var node = treeObj.getSelectedNodes()[0];
							treeObj.expandNode(node, true,false ,true);
							$("#regionInfo").show();
						}
					},
					onClick:function(event, treeId, treeNode){
						if($("#signOrderDialog").length>0){
							$("#regionName").val(treeNode.label).blur();
							var regionInfo = treeNode.extra;
							regionInfo.regionId = treeNode.id.split("_")[1];
							$("#regionNo").val(regionInfo.strCode);
							$("#regionNo_span").text(regionInfo.strCode);
							$("#inputRegionId").val(regionInfo.regionId);
							$(".input_dropdown","#signOrderDialog").hide();
						}
					}
				}
			});
			return tree;
		}
		$(".reload").click(function(){//刷新树
			$("#dropdown_regionName").val('');
			$(".treeDelete").hide();
			treeInfo = refresh(1);
		});
		$("#regionSearch").on('click', function(event) {
			searchTree('regionTree','label');
		});
		$("#regionInfo .treeDelete").on('click', function(event) {
			$('#dropdown_regionName').val('');
			$(this).hide();
		});
		$('#dropdown_regionName').keyup(function(e) {
			var key = e.keyCode;
			if (key == 13) {
				$(this).prev().click();
				if($(this).val().length>0){
					$("#regionInfo .treeDelete").show();
				}
				return false;
			} else {
				if($(this).val().length>0){
					$("#regionInfo .treeDelete").show();
				}else{
					$("#regionInfo .treeDelete").hide();
				}
				
			}
		});
		function searchTree(treeId, label){
			var value = $('#dropdown_regionName').val();
			if(value == ""){
				treeInfo = refresh();
			}else{
		        var treeObj = $.fn.zTree.getZTreeObj(treeId);  
		        var nodes = treeObj.transformToArray(treeObj.getNodes());
				treeObj.hideNodes(nodes);//隐藏所有节点
		        var nodeList = treeObj.getNodesByParamFuzzy(label, value,null); //查询出来的节点
		        for(var index=0;index<nodeList.length;index++){
		        	treeObj.showNode(nodeList[index]);
		        	hasParentNode(nodeList[index],treeObj);
		        }
			}
		}
		//判断父节点是否可以显示
		function hasParentNode(node,treeObj) {
			var currnode = node.getParentNode();
			treeObj.showNode(currnode);
			if(currnode){
				hasParentNode(currnode,treeObj);
			}else{
			    treeObj.expandNode(node, true, true, true);
			}
		}
	},
	/**
	 * 录像回放
	 * @param  {[type]} recordId 录像id
	 * @param  {[type]} nrecordStatus 录像状态
	 * @param  {[type]} type 1是数据采集回放，2是业务关联回放
	 * @return {[type]}          [description]
	 */
	recordPlayBack:function(recordId,nrecordStatus,type){
		var modal = true;
		if(type==1)modal = false;
		FinanceUtil.dialog({
			id:'recordDialog',
			title: '录像回放',
			overlay:true,
			draggable:false,
			width:700,
			height:600,
			iframeFix:true,
			show:false,
			modal:modal,
			beforeClose:function(){
				try{
					if(modeParam.usbAndIpc=="ipc"){
						document.getElementById("dialogPlayBackOcx").PB_StopAll();
					}else{
						document.getElementById("dialogPlayBackOcx").PB_StopPlayback(0);
					}
				}catch(e){
					
				}
			},
			close:function(){
				$(".newOverLay").hide();
				if(isIE6)$("#videoRecord object").removeClass('hidden');
			},
			ajax:{
				url:'/webdevice/datacollect/toOpenPageRecordPlayBack.action',
				param:{
					'id':recordId,
					'type':type
				},
				success:function(){
					$(".newOverLay").show();
					if(isIE6)$("#videoRecord object").addClass('hidden');
					if(nrecordStatus==8){
						$("#abnormal").show();
						$("#recordFormId").val(recordId);
						var maxDateTime = new Date($("#recordBegin").val().replace(/-/g,"/")).getTime()+3*60*60*1000,
							nowDateTime = new Date().getTime(),recordEndDate;
						if(nowDateTime-maxDateTime>0){
							recordEndDate = new Date(maxDateTime).Format("yyyy-MM-dd hh:mm:ss");
						}else{
							recordEndDate = new Date(nowDateTime).Format("yyyy-MM-dd hh:mm:ss");
						}
						$("#recordEnd").val(recordEndDate);
					}
					FinanceUtil.initFormUI("#recordDialog");
				}
			},
			buttons:false
		});
	},
	/**
	 * 打印业务单
	 * @param  {[type]} id        打印业务单时的id
	 * @param  {[type]} type 1是数据采集的id,2的业务关联打印的id
	 * @return {[type]}            [description]
	 */
	printPicture:function(id, type){
		if(type==1){
			window.open("/webdevice/contact/getPrintInfo.action?id="+id);
		}else{
			window.open("/webdevice/contact/getPermPrintInfo.action?id="+id);
		}
	},
	getSelectedTreeNode:function(){
		var treeInfo = $.fn.zTree.getZTreeObj('productTree');
		var selNode = treeInfo.getSelectedNodes()[0];
		return selNode;
	},
	/**
	 * 获取证件类型
	 * @param  {[type]} id          标签的id
	 * @param  {[type]} selectValue 选中的值
	 * @return {[type]}             [description]
	 */
	getCertType:function(id,selectValue){
			$(id).iselect({
			    url: '/webdevice/contact/getCardList.action',//TODO 获取证件类型下拉框
			    root: "data.cardList",//返回数据的根属性
			    displayName: 'strName', //显示名字
			    valueName: 'id',//实际要传的id
			   	value:selectValue?selectValue:null
			});
	},
	/**
	 * 弹出框列表查询条件赋值
	 * @return {[type]} [description]
	 */
	getConditions:function(){
		FMS.dataBusiness.conditions.number = $("#number").val();
		FMS.dataBusiness.conditions.clientName = $("#clientName").val();
		FMS.dataBusiness.conditions.creditCode = $("#creditCode").val();
	},
	/**
	 * 弹出框列表查询条件清空
	 * @return {[type]} [description]
	 */
	clearConditions:function(){
		FMS.dataBusiness.conditions.number = '';
		FMS.dataBusiness.conditions.clientName = '';
		FMS.dataBusiness.conditions.creditCode = '';
	}
};