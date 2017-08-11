 /**
 * @file 角色管理逻辑 
 * @author   Eli Pei
 */
var FMS = FMS || {};

FMS.role = {
	init:function() {
		keyword = "";
		this.globleSetting();
		this.bindEvent();
		this.roleGrid();
	},
	//placeHolder
	globleSetting: function() { 
		$('input, textarea').placeholder(); 
		$("select").uniform();
	},
	bindEvent:function(){
		$('.gridSearch').on('click', function() {
			keyword = $(this).next().val();
			$('#roleDataTable').grid('reload');
		});
		$('#roleGridSearch').keydown(function(e) {
			var key = e.keyCode;
			if (key == 13) {
				$(this).prev().click();
			}
		});
		$('.oprAddRole').on('click',function() {
			FMS.role.addRoleFun();
		});
		$('.oprDeleteRole').on('click',function(){
			FMS.role.multDelete();
		});
		$('.oprCopyRole').on('click',function(){
			FMS.role.copyRole();
		});
	},
	getCheckedPower:function(){
		var powerTree = $.fn.zTree.getZTreeObj('functionTree');
		var selectedNodes, powers = [];
		if(powerTree == null){
			return powers;
		}
		selectedNodes = powerTree.getCheckedNodes();
		$.each(selectedNodes, function(i, node){
			if(node.id != null){
				powers.push({
					nprivilegeCode: new Number(node.id).valueOf(),
					nsortId: i
				});
			}
		});
		return powers;
	},

	//role grid begin
	roleGrid: function() {
		$('#roleDataTable').grid({
			colName: ['ida', '名称', '角色类型','角色描述', '创建者','用户数量', '操作'],
			colModel: [{
				name: 'id',
				index: 'id',
				hidden: true,
				sortable: false
			}, {
				name: 'strName',
				index: 'strName',
				sortable: false,
				formart: function(d, fn) {
					return '<a class="roleName">' + d + '</a>';
				},
				attachEvent: {
					click: {
						el: 'a.roleName',
						callback: function(a, b, e) {
							var id = $.trim($("#" + b).find('td').eq(1).text());
							FMS.role.editRoleFun(id);
						}
					}
				}
			}, {
				name: 'ntype',
				index: 'ntype',
				formart: function(d, fn) {
					var strType;
					switch (fn.ntype) {
					case Constants.RoleType.SYSTEM_MANAGER:
						strType = '系统管理员';
						break;
					case Constants.RoleType.CUSTOMIZE_ROLE:
						strType = '自定义角色';
						break;
					default:
						strType = '-';
						break;
					}
					return strType;
				},
				sortable: false,
				showTitle:false
			}, {
				name: 'strComment',
				index: 'strComment',
				sortable: false
			},{
				name: 'nownerName',
				index: 'nownerName',
				sortable: false
			},{
				name: 'userNum',
				index: 'userNum',
				sortable: false,
				width:100
			}, {
				name: 'operation',
				index: 'operation',
				sortable: false,
				formart: function(d, fn) {
					return '<a class="icon gridIconFix iconEdit gridEdit"></a><a class="icon gridIconFix iconDelete gridDelete"></a>';
				},
				attachEvent: {
					click: {
						el: 'a',
						callback: function(a, b, e) {
							var id = $.trim($("#" + b).find('td').eq(1).text());
							if((a.srcElement == undefined ? a.target.className.indexOf("gridEdit") : a.srcElement.className.indexOf("gridEdit"))> 0){
								FMS.role.editRoleFun(id);
							} else if((a.srcElement == undefined ? a.target.className.indexOf("gridDelete") : a.srcElement.className.indexOf("gridDelete")) > 0){
								FMS.role.deleteSingle(id);
							}
						}
					}
				}
			}],
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
			// data: data,
			ajaxOptions: {
				type: 'post'
			},
			dataType: 'ajax',
			url: '/modules/role/getDataPage.action',
			pager: '#roleDataTablePage',
			pagerFixed: true,
			// pagerButtons : false,
			scrollWidth: 18,
			headTitle: true,
			ajaxDynamic: function() {
				return [{
					name: 'roleInfo.strName',
					value: keyword
				}];
			},
			events:{
				init:function(a, b){
					FMS.globalFun.addVerticalLine();
				},
				onEndRequest:function(event,elm){
					FMS.globalFun.removeGridColumn('roleDataTable',elm._pageData.totalPage);
				}
			}
		});
	},
	addRoleFun: function(){
		FinanceUtil.dialog({
			id: 'addRole',
			title: '添加角色',
			overlay: true,
			draggable: true,
			width: 620,
			height: 500,
			iframeFix: false,
			ajax: {
				url: '/modules/role/toAddRole.action',
				success: function() {
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI('#addRole');
					FMS.globalFun.isIE6ChangeTipText('#addRole');
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
						if(FMS.role.getCheckedPower()==""){
							jAlert('请选择权限配置', '错误', 'error');
							return;
						}
						var form = $('#addRole').find('form');
						if (FinanceUtil.validate(form)) {
							FinanceUtil.loader.show('请稍候...');
						} else {
							jAlert('信息输入有误', '错误', 'error');
							return;
						}
						var data = {
							'role': {
								strName: $('#roleName').val(),
								strComment: $('#strRemarks').val().replace(/[\r\n]/g,''),
								ntype: Constants.RoleType.CUSTOMIZE_ROLE
							}
						};
						data.powers = FMS.role.getCheckedPower();
						form.ajaxSubmit({
							url: '/modules/role/addRole.action',
							type: 'post',
							dataType: 'json',
							data: {'jsonStr': $.jsonToStr(data)},
							success: function(data) {
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$.sticky('操作成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
											
										}
									});
									$('#addRole').dialog('close');
									$('#roleDataTable').grid('reload');
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
						$('#addRole').dialog('close');
					}
				}
			}
		});
	},
	editRoleFun: function(id){
		if(id == 1){
			$.sticky('系统管理员不可编辑！', '提示', 'attention');
			return;
		}
		FinanceUtil.dialog({
			id: 'editRole',
			title: '编辑角色',
			overlay: true,
			draggable: true,
			width: 620,
			height: 500,
			iframeFix: false,
			ajax: {
				url: '/modules/role/toUpdateRolePage.action',
				param: {
					'roleInfo.id': id
				},
				success: function() {
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI('#editRole');
					FMS.globalFun.isIE6ChangeTipText('#editRole');
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
						if(FMS.role.getCheckedPower()==""){
							jAlert('请选择权限配置', '错误', 'error');
							return;
						}
						var form = $('#editRole').find('form');
						if (FinanceUtil.validate(form)) {
							FinanceUtil.loader.show('请稍候...');
						} else {
							jAlert('信息输入有误', '错误', 'error');
							return;
						}
						var data = {
							'role': {
								id: $('#roleId').val(),
								strName: $('#roleName').val(),
								strComment: $('#strRemarks').val().replace(/[\r\n]/g,'')
							}
						};
						data.powers = FMS.role.getCheckedPower();
//						data.resList = FMS.role.getCheckedRes();
						form.ajaxSubmit({
							url: '/modules/role/updateRole.action',
							type: 'post',
							dataType: 'json',
							data: {'jsonStr': $.jsonToStr(data)},
							success: function(data) {
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$.sticky('操作成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
											
										}
									});
									$('#editRole').dialog('close');
									$('#roleDataTable').grid('reload');
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
						$('#editRole').dialog('close');
					}
				}
			}
		});
	},
	//单一删除
	deleteSingle: function(id) {
		if(id == 1){
			$.sticky('系统管理员不允许删除！', '提示', 'attention');
			return;
		}
		jConfirm('确认要删除当前记录吗？', {
			title: '提示',
			callback: function(value) {
				if (value) {
					$.ajax({
						url: '/modules/role/deleteRole.action',
						data: {
							'ids': [id]
						},
						success: function(data) {
							if (data && data.success) {
								$('#roleDataTable').grid('reload');
								$.sticky('操作成功', {
									type: "ok",
									title: '操作成功',
									afterclose: function() { //'完成'
										
									}
								});
								$('#roleDataTable').grid('reload');
							} else if(data && data.msg) {
								jAlert(data.msg, '错误', 'error');
							}
						}
					});
				} else {
					
				}
			}
		});
	},
	//删除多条角色方法
	multDelete: function() {
		var row = $('#roleDataTable').grid('getSelRow');
		var ids = $('#roleDataTable').grid('getDataByID',row,'id');
		if($.inArray(1, ids) > -1){ //判断多选时候包括系统管理员
			$.sticky('系统管理员不允许删除！', '提示', 'attention');
			return;
		}
		if (!ids.length) {
			jAlert('请选择需要删除的数据！');
		} else {
			jConfirm('确认要删除当前记录吗？', {
				title: '提示',
				callback: function(value) {
					if (value) {
						$.ajax({
							url: '/modules/role/deleteRole.action',
							data: {
								'ids': ids
							},
							success: function(data) {
								if (data && data.success) {
									$.sticky('操作成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
										}
									});
									$('#roleDataTable').grid('reload');
								} else if(data && data.msg) {
									jAlert(data.msg, '错误', 'error');
								}
							}
						});
					}
				}
			});
		}
	},
	copyRole: function(){
		var row = $('#roleDataTable').grid('getSelRow');
		var ids = $('#roleDataTable').grid('getDataByID',row,'id');
		if($.inArray(1, ids) > -1){ 								//判断多选时候包括系统管理员
			$.sticky('系统管理员不允许复制！', '提示', 'attention');
			return;
		}
		if (!ids.length) {
			jAlert('请选择需要复制的数据！');
		} else {
			jConfirm('确认要复制当前记录吗？', {
				title: '提示',
				callback: function(value) {
					if (value) {
						$.ajax({
							url: '/modules/role/copyRoleForce.action',
							data: {
								'ids': ids
							},
							success: function(data) {
								if (data && data.success) {
									$.sticky('操作成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
										}
									});
									$('#roleDataTable').grid('reload');
								}else {
										jAlert(data.msg, '错误', 'error');
									}
								}
							});
					}
				}
			});
		}
	}
};

$(function() {
	if(isIE6){
		$(".datatableStyle").height($('#content').height()-70);
	}
	FMS.role.init();
});
