 /**
 * @file FMS组织管理逻辑 
 * @author   Eli Pei
 */
var FMS = FMS || {};

FMS.region = {
	init: function() {
		this.initConstant();
		this.globleSetting();
		this.orgInit(); //组织初始化
		this.eventBind();
	},
	initConstant: function() {
		parentId = ""; //保存选中节点id
		keyword = "";
		SHOWSUBSTAFF = 1;
	},
	globleSetting: function() {
		//placeHolder
		$('input, textarea').placeholder();
		$("select").uniform();
	},
	//begin 组织初始化
	orgInit: function() {
		function nodeCreate(event, treeId, treeNode,type){
			if (treeNode.getParentNode() == null) {
				var treeObj = $.fn.zTree.getZTreeObj(treeId);
				treeObj.selectNode(treeNode);
				var node = treeObj.getSelectedNodes()[0];
				treeObj.expandNode(node, true,false ,true);
				parentId = treeNode.id.split('_')[1];
				if(type == 1){
					$('#dataTable').grid('dofiltler');
				}else{
					FMS.region.regionGrid();
				}
			}
		}
		function nodeClick(event, treeId, treeNode){
			parentId = treeNode.id.split('_')[1];
									
			$("#orgTree_id_reg_1_a span#editReg").remove();
			keyword = "";
			$('.gridSearch').val('');
			$('#dataTable').grid('dofiltler');
		}
		FMS.regionTree.init('orgTree','/web/tree/getRegionTree.action',nodeCreate,nodeClick);
	},
	eventBind: function(){
		var orgTree = $.fn.zTree.getZTreeObj('orgTree');
		$('.gridSearch').on('click', function() {
			keyword = $(this).next().val();
			var $showsub = $(this).parents('.toolbar').find('.showChild .showsub');
			if($showsub.prop('checked')){
					SHOWSUBSTAFF = 1;
			}else{
				SHOWSUBSTAFF = 0;
			}
			$('#dataTable').grid('dofiltler');
		});
		$('#regionGridSearch').keydown(function(e) {
			var key = e.keyCode;
			if (key == 13) {
				$(this).prev().click();
			}
		});
		$('.oprAddReg').on('click', function() {
			var selOrg = orgTree.getSelectedNodes()[0];
			if (selOrg == null) {
				$.sticky('请选择需要添加的组织节点', '提示', 'attention');
				return;
			}
			FMS.region.addReg(selOrg.id.split('_')[1]);
		});
		$('.oprDeleteReg').on('click', function() {
			FMS.region.multDelete();
		});
		$('.oprImport').on('click', function() {
			FMS.region.importFun();
		});
		$('.oprExport').on('click', function() {
			if(!FMS.globalFun.exportLimit('.ui-paging-info')) return false;
			FMS.region.exportFun();
		});
		//绑定checkbox change 事件
		$('.showsub').change(function(e){
			// 显示子机构 0 没勾选 1 勾选
			var _self = $(this);
			if(_self.prop('checked')){
				SHOWSUBSTAFF = 1;
			}else{
				SHOWSUBSTAFF = 0;
			}
			$('#dataTable').grid('dofiltler');
		});
	},
	regionGrid: function(){
		var orgTree = $.fn.zTree.getZTreeObj('orgTree');
		$('#dataTable').grid({
			colName: ['ida', '组织名称', '组织代码', '备注', '操作'],
			colModel: [{
				name: 'id',
				index: 'id',
				hidden: true
			}, {
				name: 'strName',
				index: 'strName',
				formart: function(d, fn) {
					return '<a class="vehName">' + d + '</a>';
				},
				attachEvent: {
					click: {
						el: 'a.vehName',
						callback: function(a, b,e) {
							var id = $.trim($("#" + b).find('td').eq(1).text());
							//EDIT 
							FMS.region.editReg(id);
						}
					}
				}
			}, {
				name: 'strCode',
				index: 'strCode'
			}, {
				name: 'strTxt',
				index: 'strTxt',
				showTitle:true
			}, {
				name: 'operation',
				index: 'operation',
				formart: function(d, fn) {
					if(fn.id==1){
						return '<a class="icon gridIconFix iconEdit gridEdit">';
					}else{
						return '<a class="icon gridIconFix iconEdit gridEdit"></a><a class="icon gridIconFix iconDelete gridDelete"></a>';
					}
					
				},
				attachEvent: {
					click: {
						el: 'a',
						callback: function(a, b,e) {
							var id = $.trim($("#" + b).find('td').eq(1).text());
							if((a.srcElement == undefined ? a.target.className.indexOf("gridEdit") : a.srcElement.className.indexOf("gridEdit"))> 0){
								//编辑
								FMS.region.editReg(id);
							} else if((a.srcElement == undefined ? a.target.className.indexOf("gridDelete") : a.srcElement.className.indexOf("gridDelete")) > 0){
								//删除
								FMS.region.deleteReg(id);
							}
						}
					}
				}
			}],
			colModelTmp:{
				sortable: false
			},
			headerFixed: true,
			resizable: false,
			multiSelect: true,
			multiSelectWidth:25,
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
			url: '/modules/region/getDataPage.action',
			pager: '#dataTablePage',
			pagerFixed: true,
			scrollWidth: 18,
			headTitle: true,
			ajaxDynamic: function() {
				return [{
					'name': 'regionInfo.id',
					value: parentId
				}, {
					'name': 'regionInfo.strName',
					value: keyword
				}, {
					'name': 'regionInfo.isShow',
					value: SHOWSUBSTAFF
				}];
			},
			events:{
				init:function(a, b){
					FMS.globalFun.addVerticalLine();
				},
				onEndRequest:function(event,elm){
					FMS.globalFun.removeGridColumn('dataTable',elm._pageData.totalPage);
				}
			}
		});
	},
	//添加
	addReg: function(selId){
		FinanceUtil.dialog({
			id: 'addReg',
			title: '添加组织',
			overlay: true,
			draggable: true,
			width: 500,
			height: 350,
			iframeFix: false,
			ajax: {
				url: '/modules/region/toAddRegPage.action',
				param: {
					'regionInfo.nparentId': selId
				},
				success: function() {
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI('#addReg');
					FMS.globalFun.isIE6ChangeTipText('#addReg');
				}
			},
			close: function(){
            	FMS.globalFun.isIE6ShowSelect("#wrapper");
            },
			buttons: {
				"ok": {
					text: '确认',
					'class': 'bPrimary',
					click: function() {
						var form = $('#addReg').find('form');
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
							url: '/modules/region/addReg.action',
							type: 'post',
							dataType: 'json',
							success: function(data) {
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$.sticky('操作成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
											
										}
									});
									$('.reload').click();
									$('#addReg').dialog('close');
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
						$('#addReg').dialog('close');
					}
				}
			}
		});
	},
	//多重删除
	multDelete: function(){
		var row = $('#dataTable').grid('getSelRow');
		var ids = $('#dataTable').grid('getDataByID',row,'id');
		if (!ids.length) {
			jAlert('请选择需要删除的数据！');
		} else {
			for(var i=ids.length-1;i>=0;i--){
				if(ids[i] == 1){
					jAlert("根节点不能被删除！");
					return;
				}
			}
			jConfirm('是否确定删除？', {
				title: '提示',
				callback: function(value) {
					if (value) {
						$.ajax({
							url: '/modules/region/deleteReg.action',
							data: {
								'ids': ids
							},
							success: function(data) {
								if (data && data.success) {
									
									$.sticky('操作成功', {
										type: "ok",
										title: '操作成功'
									});
									$('.reload').click();
								} else {
									jAlert(data.msg, '错误', 'error');
								}
							}
						});
					}
				}
			});
		};
	},
	editReg: function(selId){
		FinanceUtil.dialog({
			id: 'orgEditDialog',
			title: '编辑组织',
			overlay: true,
			draggable: true,
			width: 500,
			height: 350,
			iframeFix: false,
			ajax: {
				url: '/modules/region/toEditRegPage.action',
				param: {
					'regionInfo.id': selId
				},
				success: function() {
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI('#orgEditDialog');
					$('.parentReg').hide();//隐藏父区域
					$(".msg-b").addClass('msg-b-4');
					$("textarea[name='regionInfo.strTxt']").css({"height": '90'});
					FMS.globalFun.isIE6ChangeTipText('#orgEditDialog');
				}
			},
			close: function(){
            	FMS.globalFun.isIE6ShowSelect("#wrapper");
            },
			buttons: {
				"ok": {
					text: '确认',
					'class': 'bPrimary',
					click: function() {
						var form = $('#orgEditDialog').find('form');
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
							url: '/modules/region/editReg.action',
							type: 'post',
							dataType: 'json',
							success: function(data) {
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$(".reload").click();
									$.sticky('操作成功', {
										type: "ok",
										title: '操作成功'
									});
									$('#orgEditDialog').dialog('close');
								} else if (data && data.msg) {
									jAlert(data.msg, '错误' , 'error');
								}
							}
						});
					}
				},
				"cancel": {
					text: '取消',
					click: function() {
						$('#orgEditDialog').dialog('close');
					}
				}
			}
		});
	},
	//单条删除
	deleteReg: function(id){
		jConfirm('是否确定删除？', {
			title: '提示',
			callback: function(value) {
				if (value) {
					$.ajax({
						url: '/modules/region/deleteReg.action',
						data: {
							'ids':id
						},
						success: function(data) {
							if (data && data.success) {
								$('.reload').click();
								$.sticky('操作成功', {
									type: "ok",
									title: '操作成功'
								});
							} else {
								jAlert(data.msg, '错误', 'error');
							}
						}
					})
				} else {
				}
			}
		});
	},
	importFun: function(){
		FinanceUtil.dialog({
			id: 'importDialog',
			title: '导入组织',
			overlay: true,
			draggable: true,
			width: 500,
			height: 240,
			iframeFix: false,
			ajax: {
				url: '/modules/region/toCheckOutPage.action',
				success: function() {
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
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
								url: '/modules/region/regionUploadExcel.action',
								type: 'post',
								dataType: 'json',
								success: function(data) {
									FinanceUtil.loader.hide();
									if(data&&data.fieldErrors.regFile){
										$("#importDialog input").val("");
										jAlert(data.fieldErrors.regFile[0], '错误', 'error');
										return;
									}
									var resMap = data.resMap;
									if (resMap.errorNo) {
										var failRegionName = resMap.failRegionName||"";
										if(failRegionName.length>0){
											FinanceUtil.dialog({
												id:'importMsgDialog',
												title:'未导入的组织',
												overlay:true,
												draggable: true,
												width: 630,
												height: 430,
												iframeFix: iframeFix,
												ajax:{
													url:'/modules/user/toAlreadyImport.action',
													success:function(){
														$("#importMsgDialog #message").html(failRegionName.replace(/&/g, "<br/>"));
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
											$.sticky('导入成功', {type: "ok",title: '操作成功'});
										}
										$('.reload').click();
										$('#importDialog').dialog('close');
									} else {
										jAlert(resMap.errorInfo, '错误', 'error');
									}
								},
								error:function(xhr){
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
	exportFun: function(){
		$.ajax({
			url:'/modules/region/exportRegionExcel.action',
			type:'post',
			dataType:'json',
			data:{
				'regionInfo.id':parentId,
				'regionInfo.strName':keyword,
				'regionInfo.isShow':SHOWSUBSTAFF
			},
			success:function(data){
				if(data && data.success){
					window.location.href = data.data.url+'?rnd=' + new Date().getTime();
				}else{
					jAlert(data.msg,'错误','error');
				}
			}
		});
	}
};
$(function() {
	FMS.region.init();
});
var maxsize = 2*1024*1024;//2M  
        var errMsg = "上传的附件文件不能超过2M！！！";  
        var tipMsg = "您的浏览器暂不支持计算上传文件的大小，确保上传文件不要超过2M，建议使用IE、FireFox、Chrome浏览器。";  
        var  browserCfg = {};  
        var ua = window.navigator.userAgent;  
        if (ua.indexOf("MSIE")>=1){  
            browserCfg.ie = true;  
        }else if(ua.indexOf("Firefox")>=1){  
            browserCfg.firefox = true;  
        }else if(ua.indexOf("Chrome")>=1){  
            browserCfg.chrome = true;  
        }  
        function checkfile(){  
            try{  
                var obj_file = document.getElementById("regFile");  
                if(obj_file.value==""){  
                    alert("请先选择上传文件");  
                    return;  
                }  
                var filesize = 0;  
                if(browserCfg.firefox || browserCfg.chrome ){  
                    filesize = obj_file.files[0].size;  
                }else if(browserCfg.ie){  
                    var obj_img = document.getElementById('tempimg'); 
                    obj_img.dynsrc=obj_file.value;  
                    filesize = obj_img.fileSize;  
                }else{  
                    alert(tipMsg);  
                return;  
                }  
                if(filesize==-1){  
                    alert(tipMsg);  
                    return;  
                }else if(filesize>maxsize){  
                    alert(errMsg);  
                    return;  
                }else{  
                    alert("文件大小符合要求");  
                    return;  
                }  
            }catch(e){  
                alert(e);  
            }  
        }  
