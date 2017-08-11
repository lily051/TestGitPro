var FMS = FMS || {};
FMS.certManage = {
	//证书管理
	certificateManage:function(){
		FinanceUtil.dialog({
			id:"certificateDialog",
			title:"证书管理",
			overlay:true,
			draggable:true,
			width:870,
			height:622,
			iframeFix:false,
			ajax:{
				url:'/modules/product/toCertManagerPage.action',//TODO 证书管理页面
				success:function(){
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI("#certificateDialog");
				}
			},
			close: function(){
            	FMS.globalFun.isIE6ShowSelect("#wrapper");
            },
			buttons:{
				"ok": {
					text: '确认',
					'class': 'bPrimary',
					click: function() {
						$('#certificateDialog').dialog('close');
					}
				},
				"cancel": {
					text: '取消',
					click: function() {
						$('#certificateDialog').dialog('close');
					}
				}
			}
		});
	},
	addCertificate:function(){
		//$("#certificateDialog").dialog('close');
		FinanceUtil.dialog({
			id:"addCertificateDialog",
			title:"新增证书",
			overlay:true,
			draggable:true,
			width:590,
			height:460,
			iframeFix:false,
			ajax:{
				url:'/modules/product/toAddCertManagerPage.action',//TODO 新增证书页面
				success:function(){
					FinanceUtil.initFormUI("#addCertificateDialog");
					FMS.globalFun.isIE6ChangeTipText('#addCertificateDialog');
				}
			},
			buttons:{
				"ok":{
					text: '确认',
					'class': 'bPrimary',
					click: function(){
						/*var resTree = $.fn.zTree.getZTreeObj('productCertTree');
						if(resTree.getCheckedNodes().length==0){
							jAlert("请选择产品权限",'错误', 'error');
							return;
						}*/
						var form = $('#addCertificateDialog').find('form');
						var data = FMS.certManage.getAsynTreeInfo();
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
							url: '/modules/product/addCertManager.action',//TODO 新增证书
							type: 'post',
							dataType: 'json',
							data:{
								'jsonStr':$.jsonToStr(data)
							},
							success: function(data) {
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$.sticky('操作成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
											
										}
									});
									$("#addCertificateDialog").dialog('close');
									$("#dataTable").grid("reload");
									FMS.certManage.certificateManageGetUrl();
								} else if (data && data.msg) {
									jAlert(data.msg, '错误' , 'error');
								}
							}
						});
					}
				},
				"cancel":{
					text: '取消',
					click: function(){
						$("#addCertificateDialog").dialog('close');
					}
				}
			}
		});
	},
	/*搜索证书*/
	certificateManageGetUrl:function(){
		var key = $("#dialogSearch").val();
		var url="/modules/product/getCertDate.action";
		$(".certificateContent").load(url,{"crtificateInfo.strName":key});
	},
	/*获取异步产品树操作过的节点*/
	getAsynTreeInfo:function(){
		var resTree = $.fn.zTree.getZTreeObj('productCertTree');
		var producttypeList =[], certProductList = [], data={}, certInfo = {};
		certInfo.strName = $("#strName").val();
		certInfo.strCode = $("#strCode").val();
		data.certInfo = certInfo;
		var nodes = resTree.transformToArray(resTree.getNodes());
		for(var i=0; i < nodes.length;i++){
		    var node=nodes[i];
		    if(!node.isHidden){//isHidden=false 是显示的
		        if(!node.open){//open = false 是折叠的
		            if(node.hasChecked){
		            	if(node.id.slice(0,Constants.PREFIX.PRODUCTTYPE.length) == "productType_"){//选得是产品类型
		            		producttypeList.push({"id":Number(node.id.slice(Constants.PREFIX.PRODUCTTYPE.length)).valueOf(),"status":node.getCheckStatus().checked});
		            	}else if(node.id.slice(0,Constants.PREFIX.PRODUCT.length) == "product_"){//选得是产品
		            		certProductList.push({"nproductId":Number(node.id.slice(Constants.PREFIX.PRODUCT.length)).valueOf(),"status":node.getCheckStatus().checked})
		            	}
		            }
		        }
		    }
		}
		data.producttypeList = producttypeList;
		data.certProductList = certProductList;
		return data;
	},
	/*编辑证书*/
	editCertificate:function(id){
		FinanceUtil.dialog({
			id:"editCertificateDialog",
			title:"证书管理",
			overlay:true,
			draggable:true,
			width:590,
			height:460,
			iframeFix:false,
			show:false,
			ajax:{
				url:'/modules/product/toUpdateCertManagerPage.action',//TODO 编辑证书页面
				param:{
					'crtificateInfo.id':id
				},
				success:function(){
					FinanceUtil.initFormUI("#editCertificateDialog");
					FMS.globalFun.isIE6ChangeTipText('#editCertificateDialog');
				}
			},
			buttons:{
				"ok":{
					text: '确认',
					'class': 'bPrimary',
					click: function(){
						/*var resTree = $.fn.zTree.getZTreeObj('productCertTree');
						if(resTree.getCheckedNodes().length==0){
							jAlert("请选择产品权限",'错误', 'error');
							return;
						}*/
						var form = $('#editCertificateDialog').find('form');
						var data = FMS.certManage.getAsynTreeInfo();
						data.certInfo.id=id;
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
							url: '/modules/product/updateCert.action',//TODO 编辑证书
							type: 'post',
							dataType: 'json',
							data:{
								'jsonStr':$.jsonToStr(data)
							},
							success: function(data) {
								FinanceUtil.loader.hide();
								if (data && data.success) {
									//FMS.product.certificateManage();
									$.sticky('操作成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
											
										}
									});
									$("#editCertificateDialog").dialog('close');
									$("#dataTable").grid("reload");
									FMS.certManage.certificateManageGetUrl();
								} else if (data && data.msg) {
									jAlert(data.msg, '错误' , 'error');
								}
							}
						});
					}
				},
				"cancel":{
					text: '取消',
					click: function(){
						//FMS.product.certificateManage();
						$("#editCertificateDialog").dialog('close');
					}
				}
			}
		});
	}
};