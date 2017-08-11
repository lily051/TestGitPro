var FMS = FMS || {};
FMS.card = {
	init:function(){
		this.bindEvent();
		this.cardGrid();
	},
	bindEvent:function(){
		$(".oprAddCard").on('click', function(event) {
			FMS.card.addCard();
		});
		$(".oprDeleteCard").on('click', function(event) {
			var row = $('#cardDataTable').grid('getSelRow');
			var ids = $('#cardDataTable').grid('getDataByID', row, 'id');
			if (!ids.length) {
				jAlert('请选择要删除的数据！');
			} else {
				FMS.card.multDeleteCard(ids);
			}
			
		});
		$("#cardAble").on('click', function(event) {
			var row = $('#cardDataTable').grid('getSelRow');
			var ids = $('#cardDataTable').grid('getDataByID', row, 'id');
			if (!ids.length) {
				jAlert('请选择要启用的证件！');
				return;
			}
			FMS.card.cardAble(ids);
		});
		$("#cardDisable").on('click', function(event) {
			var row = $('#cardDataTable').grid('getSelRow');
			var ids = $('#cardDataTable').grid('getDataByID', row, 'id');
			if (!ids.length) {
				jAlert('请选择要禁用的证件！');
				return;
			}
			FMS.card.cardDisable(ids);
		});
	},
	addCard:function(){
		FinanceUtil.dialog({
			id:"addCardDialog",
			title:"添加证件",
			overlay:true,
			draggable:true,
			width:440,
			height:250,
			iframeFix:false,
			ajax:{
				url:'/modules/card/toAddCardPage.action',
				success:function(){
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI("#addCardDialog");
					FMS.globalFun.isIE6ChangeTipText('#addCardDialog');
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
						var form = $('#addCardDialog').find('form');
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
							url: '/modules/card/addCard.action',
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
									$('#cardDataTable').grid('reload');
									$('#addCardDialog').dialog('close');
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
						$('#addCardDialog').dialog('close');
					}
				}
			}
		});
	},
	cardAble:function(ids){
		jConfirm('确认要启用选中的证件吗？',{
			title:'提示',
			callback:function(value){
				if (value) {
					$.ajax({
						url: '/modules/card/enableCards.action',//TODO
						data: {
							'ids': ids
						},
						success:function(data){
							FMS.globalFun.doSuccess(data,'cardDataTable','启用成功');
						}
					});
				}
			}
		})
	},
	cardDisable:function(ids){
		jConfirm('确认要禁用选中的证件吗？',{
			title:'提示',
			callback:function(value){
				if (value) {
					$.ajax({
						url: '/modules/card/disableCards.action',//TODO
						data: {
							'ids': ids
						},
						success:function(data){
							FMS.globalFun.doSuccess(data,'cardDataTable','禁用成功');
						}
					});
				}
			}
		})
	},
	editCard:function(id){
		FinanceUtil.dialog({
			id:"editCardDialog",
			title:"编辑证件",
			overlay:true,
			draggable:true,
			width:440,
			height:250,
			iframeFix:false,
			ajax:{
				url:'/modules/card/toUpdateCardPage.action',
				param:{
					'paramdictionaryInfo.id':id //TODO 编辑证件id
				},
				success:function(){
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI("#editCardDialog");
					FMS.globalFun.isIE6ChangeTipText('#editCardDialog');
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
						var form = $('#editCardDialog').find('form');
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
							url: '/modules/card/updateCard.action',
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
									$('#cardDataTable').grid('reload');
									$('#editCardDialog').dialog('close');
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
						$('#editCardDialog').dialog('close');
					}
				}
			}
		});
	},
	multDeleteCard:function(ids){
		jConfirm('是否确定删除？',{
			title:'提示',
			callback:function(value){
				if (value) {
					$.ajax({
						url: '/modules/card/deleteCards.action',//TODO delete
						data: {
							'ids': ids
						},
						success:function(data){
							FMS.globalFun.doSuccess(data,'cardDataTable','删除成功');
						}
					});
					
				};
			}
		});
	},
	cardGrid:function(){
		$("#cardDataTable").grid({
			colName:['ids','证件类型','证件编号','启用状态','操作'],
			colModel:[{
				name:'id',
				width:'1',
				hidden:true
			},{
				name:'strName',
				index:'strName',
				sortable:false,
				formart:function(d, fn){
					return '<a class="cardName">' + d + '</a>';
				},
				attachEvent:{
					click:{
						el:'a.cardName',
						callback:function(a, b, e){
							var index = b.indexOf("_");
							var id = b.substring(index+1);
							FMS.card.editCard(id);
						}
					}
				},
				width:'10'
			},{
				name:'strCode',
				index:'strCode',
				sortable:false,
				width:'10'
			},{
				name:'enable',
				index:'enable',
				sortable:false,
				width:'10',
				showTitle:false,
				formart:function(d, fn){
					if(d==1){
						return '<span class="enableStyle">启用</span>';
					}else{
						return '<span class="unableStyle">禁用</span>';
					}
				}
			},{
				name:'operation',
				index:'operation',
				sortable:false,
				width:'10',
				formart:function(d, fn){
					return '<a class="icon gridIconFix iconEdit gridEdit"></a><a class="icon gridIconFix iconDelete gridDelete"></a>';
				},
				attachEvent:{
					click:{
						el:'a',
						callback:function(a, b, e){
							var index = b.indexOf("_");
							var id = b.substring(index+1);
							if((a.srcElement == undefined ? a.target.className.indexOf("gridEdit") : a.srcElement.className.indexOf("gridEdit")) > 0){
								FMS.card.editCard(id);
							}else if((a.srcElement == undefined ? a.target.className.indexOf("gridDelete") : a.srcElement.className.indexOf("gridDelete")) > 0){
								FMS.card.multDeleteCard([id]);
							}
						}
					}
				}
			}],
			headerFixed: true,
			resizable: false,
			multiSelect: true,
			multiSelectWidth:28,
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
			url: '/modules/card/getDataPage.action',//TODO 列表
			pager: '#cardDataTablePage',
			pagerFixed: true,
			scrollWidth: 18,
			headTitle: true,
			ajaxDynamic: function(){
				
			},
			events:{
				init:function(a, b){
					FMS.globalFun.addVerticalLine();
				},
				onEndRequest:function(event,elm){
					FMS.globalFun.removeGridColumn('cardDataTable',elm._pageData.totalPage);
				}
			}
		});
	}
};

$(function(){
	if(isIE6){
		$(".datatableStyle").height($('#content').height()-70);
	}
	FMS.card.init();
});