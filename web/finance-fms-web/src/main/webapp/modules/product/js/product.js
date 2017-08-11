/*
 * @file FMS 权限侧边栏逻辑
 */
var FMS = FMS || {};
FMS.product = {
	init:function(){
		this.globalSetting();
		this.productTreeInit();
		this.bindEvent();
	},
	globalSetting:function(){
		$(".searchInput").placeholder();
		FinanceUtil.initFormUI("#filterFormBox");
		this.conditions = {
			parentId : "",
			searchAssign : 0,
			nenabled : $("#enableStatus").val(),
			strCode : $("#productCode").val(),
			strName : $("#productName").val(),
			strRisklevel : $("#riskLevel").val()
		};
		try{//重置的时候给regionId赋值
			this.conditions.parentId = FMS.product.getSelectedTreeNode().id.split("_")[1];
		}catch(e){
		}
	},
	productTreeInit:function(){

		var treeInfo = refresh();
		function refresh(type){
			var tree = $("#productTree").tree({
				ajax:{
					url:'/web/tree/getProductTypeTree.action'
				},
				callback: {
					onNodeCreated:function(event, treeId, treeNode){
						if (treeNode.getParentNode() == null) {
							var treeObj = $.fn.zTree.getZTreeObj(treeId);
							treeObj.selectNode(treeNode);
							var node = treeObj.getSelectedNodes()[0];
							treeObj.expandNode(node, true,false ,true);
							FMS.product.conditions.parentId = treeNode.id.split('_')[1];
							if(type==1){
								$("#dataTable").grid("dofiltler");
							}else{
								FMS.product.productDrid();
							}
						}
					},
					onClick:function(event, treeId, treeNode){
						FMS.product.conditions.parentId = treeNode.id.split('_')[1];
						$('#gridSearch').val('');
						$("#dataTable").grid("dofiltler");
					}
				}
			});
			return tree;
		}
		$(".reload").click(function(){
			$('#keyValue').val('');
			$(".treeDelete").hide();
			treeInfo = refresh(1);
		});
		$("#treeSearch").on('click', function(event) {
			searchTree('productTree','label');
		});
		$(".treeDelete").on('click', function(event) {
			$('#keyValue').val('');
			$(this).hide();
			//$(this).next().click();
		});
		$('#keyValue').keyup(function(e) {
			var key = e.keyCode;
			if (key == 13) {
				$(this).prev().click();
				if($(this).val().length>0){
					$(".treeDelete").show();
				}
				return false;
			} else {
				if($(this).val().length>0){
					$(".treeDelete").show();
				}else{
					$(".treeDelete").hide();
				}
				
			}
		});
		function searchTree(treeId, label){
			var value = $('#keyValue').val();
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
	 * 获取产品权限树
	 * @param  {[type]} type edit是编辑的时候的设置，detail是详情的时候的设置
	 * @return {[type]}      [description]
	 */
	initProductTree:function(type){
		var url,
			certId = '';//证书id
		if(type == "edit"||type=="detail"){
			url = '/web/tree/getProductTreeByCertId.action';
			certId = $("#crtificateInfoId").val();
		}else{
			url = '/web/tree/getProductTree.action';//添加
		}
		var resTree = $('#productCertTree').tree({
			async: {
				enable: true,
				url: url,
				autoParam: ['id=strId'],//根据strId加载异步树
				otherParam: ["crtificateInfo.id", certId] //提交参数crtificateInfo.id = certId
			},
			check: {
				enable: true,
				nocheckInherit: true
			},
			callback: {
				onNodeCreated: function(e, treeId, node){
					if(node.getParentNode() == null){
						resTree.expandNode(node, true, false, false, true);
					}
					if(node.halfCheck==true||node.checked==true){
						//node.checked=true;
						//resTree.expandNode(node, true, false, false, true);
					}
					var parentNode = node.getParentNode();
					if(parentNode == undefined)return;
					if(parentNode.checked == false&&parentNode.halfCheck == false){
						node.checked = false;
						node.halfCheck = false;
						setParentNotCheck(node);
					}else if(parentNode.checked){
						node.checked = true;
						node.halfCheck = false;
					}
					
				},
				onAsyncSuccess: function(e,treeId,node){
					if(node!=null&&node.hasChecked){
						$.fn.zTree.getZTreeObj(treeId).checkNode(node,node.getCheckStatus().checked,true);
						setNodeHasCheck(node);
					}
				},
				onCheck:function(e,treeId,node){
					setNodeHasCheck(node);
					if(node.checked == false){
						setChildrenNotCheck(node);
						setParentNotCheck(node);
					}else{
						setChildrenCheck(node);
						checkSetParentCheck(e,treeId,node);
					}
					$.fn.zTree.getZTreeObj(treeId).updateNode(node);
				},
				onExpand:function(e,treeId,node){
					setParentCheck(e,treeId,node);
				},
				beforeCheck:function(){
					//详情的时候不能选择
					if(type=="detail")
					return false;
				}
			}
		});
		return resTree;
		/**
		 * 勾选的时候设置父节点的选中状态
		 * @param  {[type]} e      [description]
		 * @param  {[type]} treeId [description]
		 * @param  {[type]} node   [description]
		 * @return {[type]}        [description]
		 */
		function checkSetParentCheck(e,treeId,node){
			var zTree = $.fn.zTree.getZTreeObj(treeId);
			var parentNode = node.getParentNode();
			if(parentNode==undefined)return;
			var children = parentNode.children,isAllCheck = true;
			for(var i=0;i<children.length;i++){
				if(children[i].checked==false&&!children[i].isHidden){
					isAllCheck = false;
					return;
				}
			}
			if(isAllCheck){
				parentNode.halfCheck = false;
				parentNode.checked = true;
				zTree.updateNode(parentNode);
				var parentNodes = parentNode.getParentNode();
				if(parentNodes == undefined)return;
				checkSetParentCheck(e,treeId,parentNode)
			}
		}
		/**
		 * 展开的时候根据子节点判断父节点的选中状态
		 * @param {[type]} e      [description]
		 * @param {[type]} treeId [description]
		 * @param {[type]} node   [description]
		 */
		function setParentCheck(e,treeId,node){
			var zTree = $.fn.zTree.getZTreeObj(treeId);
			var children = node.children,isAllCheck = true;
			if(children == undefined)return;
			for(var i=0;i<children.length;i++){
				if(children[i].checked==true){
					continue;
				}else{
					if(!children[i].isHidden){
						isAllCheck = false;
						return;
					}
				}
			}
			if(isAllCheck){
				node.halfCheck = false;
				node.checked = true;
				zTree.updateNode(node);
				var parentNode = node.getParentNode();
				if(parentNode == undefined)return;
				setParentCheck(e,treeId,parentNode)
			}
		}
		/**
		 * 节点选中的时候设置子节点为选中状态
		 * @param {[type]} node [description]
		 */
		function setChildrenCheck(node){
			var children = node.children;
			if(children == undefined)return;
			for(var i=0;i<children.length;i++){
				var selfSpan = $("#"+children[i].tId+"_check");
				if(selfSpan.hasClass('checkbox_true_part')){
					selfSpan.removeClass('checkbox_true_part').addClass('checkbox_true_full');
					try{
						if(children[i].children.length>0){
							setChildrenCheck(children[i]);
						}
					}catch(e){

					}
					
				}
			}
		}
		/**
		 * 节点不选中的时候把子节点的半选样式改为全不选
		 * @param {[type]} node [description]
		 */
		function setChildrenNotCheck(node){
			var children = node.children;
			if(children == undefined)return;
			for(var i=0;i<children.length;i++){
				var selfSpan = $("#"+children[i].tId+"_check");
				if(selfSpan.hasClass('checkbox_false_part')){
					selfSpan.removeClass('checkbox_false_part').addClass('checkbox_false_full');
					try{
						if(children[i].children.length>0){
							setChildrenNotCheck(children[i]);
						}
					}catch(e){

					}
				}
			}
		}
		/**
		 * [setNodeHasCheck description]
		 * @param {[type]} node [description]
		 */
		function setNodeHasCheck(node){
			if(node.checked==true||node.halfCheck==true){
				node.halfCheck=false;
			}else if(node.checked==false){
				//setParentNotCheck(node);
			}
			node.hasChecked = true;
			var childs=node.children;
			if(childs){
				for(var j=0;j<childs.length;j++){
					setNodeHasCheck(childs[j]);
				}
			}
		}
		/**
		 * 设置父节点的半选状态
		 * @param {[type]} node [description]
		 */
		function setParentNotCheck(node){
			if(node.parentId!=null){
				var parentNode = node.getParentNode();
				var childs = parentNode.children,
					childArr = [],
					childrenCheck=false;
				for(var i=0;i<childs.length;i++){
					if(childs[i].checked==true||childs[i].halfCheck==true){
						childrenCheck=true;
						return;
					}
					//getAllChildren(childArr,childs[i]);
				}
				/*for(var j=0;j<childArr.length;j++){
					if(childArr[j].checked==true||childArr[j].halfCheck==true){
						childrenCheck=true;
						return;
					}
				}*/
				//alert(childrenCheck)
				if(childrenCheck==false){
					parentNode.halfCheck=false;
					parentNode.check_Child_State=-1;
					$("#"+parentNode.tId+"_check").removeClass('checkbox_false_part').addClass('checkbox_false_full');
					setParentNotCheck(parentNode);
				}
			}
		}
		/**
		 * 获取该节点的所有子节点
		 * @param  {[type]} childArr 子节点数组
		 * @param  {[type]} node     往下递归的节点
		 * @return {[type]}          [description]
		 */
		function getAllChildren(childArr,node){
			childArr.push(node);
			if(node.isParent){
				var childs = node.children;
				if(childs){
					for(var i=0;i<childs.length;i++){
						getAllChildren(childArr,childs[i]);
					}
				}
				
			}
			
		}
	},
	getSelectedTreeNode:function(){
		var treeInfo = $.fn.zTree.getZTreeObj('productTree');
		var selNode = treeInfo.getSelectedNodes()[0];
		return selNode;
	},
	bindEvent:function(){
		var that = this;
		var treeInfo = $.fn.zTree.getZTreeObj('productTree');
		$(".oprAddPro").on('click', function(event) {
			event.preventDefault();
			var selNode = treeInfo.getSelectedNodes()[0];
			if(selNode == null){
				$.sticky("请选择要添加哪种产品类型的产品", '提示', 'attention');
				return;
			}else if(selNode.id.split("_")[1] == 1){
				$.sticky("根节点下不能添加产品", '提示', 'attention');
				return;
			}
			that.addPro(selNode.id.split("_")[1]);
			//that.addPro();
		});
		/*$(".oprDeletePro").on('click', function(event) {
			event.preventDefault();
			that.multDeletePro();
		});*/
		$("#productAble").on('click', function(event) {
			event.preventDefault();
			that.productAble();
		});
		$("#productDisable").on('click', function(event) {
			event.preventDefault();
			that.productDisable();
		});
		$(".oprImport").on('click', function(event) {
			event.preventDefault();
			that.importFun();
		});
		$(".oprExport").on('click', function(event) {
			event.preventDefault();
			if(!FMS.globalFun.exportLimit('.ui-paging-info')) return false;
			that.exportFun();
		});
		$(".oprAssign").on('click', function(event) {
			event.preventDefault();
			that.assignUser();
		});
		$(".oprCertificate").on('click', function(event) {
			event.preventDefault();
			FMS.certManage.certificateManage();
		});
		$("#search").on('click', function(event) {
			that.globalSetting();
			$("#dataTable").grid("dofiltler");		
		});
		$("#clear").on('click', function(event) {
			$("#filterFormBox").find('input').val('');
			$("#filterFormBox").find('select').val('');
			that.globalSetting();
			$("#dataTable").grid("dofiltler");
		});
		$("#filtrate").on('click', function(event) {
			FMS.globalFun.showSearchCondition($(this),'dataTable');
		});
		$(".addProType").on('click', function(event) {
			var selNode = treeInfo.getSelectedNodes()[0];
			if(selNode == null){
				$.sticky("请选择要添加的产品类型", '提示', 'attention');
				return;
			}
			that.addProType(selNode.id.split("_")[1]);
			//that.addProType();
		});
		$(".editProType").on('click', function(event) {
			var selNode = treeInfo.getSelectedNodes()[0];
			if(selNode == null){
				$.sticky("请选择要编辑的产品类型", '提示', 'attention');
				return;
			}else if(selNode.id.split("_")[1] == 1){
				$.sticky("根节点不能编辑", '提示', 'attention');
				return;
			}
			that.editProType(selNode.id.split("_")[1]);
			//that.editProType();
		});
		$(".delProType").on('click', function(event) {
			var selNode = treeInfo.getSelectedNodes()[0];
			if(selNode == null){
				$.sticky("请选择要删除的产品类型", '提示', 'attention');
				return;
			}else if(selNode.id.split("_")[1] == 1){
				$.sticky("根节点不能删除", '提示', 'attention');
				return;
			}
			that.delProType(selNode.id.split("_")[1]);
			//that.delProType();
		});
	},
	productDrid:function(){
		var colName = ['ids','产品名称','产品类型','状态','产品编码','发行机构','风险等级','到期日','保障期限','复核员','证书','操作'],colModel = [{
				name:'id',
				width:'1',
				hidden:true
			},{
				name:'strName',
				index:'strName',
				formart:function(d, fn){
					return '<a class="productName">' + d + '</a>';
				},
				attachEvent:{
					click:{
						el:'a.productName',
						callback:function(a, b, e){
							FMS.globalFun.stopPropagation(e);
							var index = b.indexOf("_");
							var id = b.substring(index+1);
							FMS.product.editPro(id);
						}
					}
				},
				width:130
			},{
				name:'productTypeName',
				index:'productTypeName',
				width:100
			},{
				name: 'nenabled',
				index: 'nenabled',
				width: 60,
				formart: function(d, fn){
					if(d==0){
						return "<span class='unableStyle'>禁用</span>";
					}else if(d == 1){
						return "<span class='enableStyle'>启用</span>";
					}else{
						return '未知';
					}
				},
				showTitle:false
			},{
				name: 'strCode',
				index: 'strCode',
				width:100
			},{
				name:'strGrade',
				index:'strGrade',
				width:100
			},{
				name:'strRisklevel',
				index:'strRisklevel',
				formart:function(d, fn){
					if(d == 1){
						return Constants.RiskLevel.RISKLEVEL_ONE;
					}else if(d == 2){
						return Constants.RiskLevel.RISKLEVEL_TWO;
					}else if(d == 3){
						return Constants.RiskLevel.RISKLEVEL_THREE;
					}else if(d == 4){
						return Constants.RiskLevel.RISKLEVEL_FOUR;
					}else if(d == 5){
						return Constants.RiskLevel.RISKLEVEL_FIVE;
					}else if(d == 6){
						return Constants.RiskLevel.RISKLEVEL_SIX;
					}else if(d == 0){
						return Constants.RiskLevel.RISKLEVEL_NONE;
					}else{
						return '未知';
					}
				},
				width:80,
				showTitle:false
			},{
				name:'strEndDate',
				index:'strEndDate',
				width:110
			},{
				name:'guaranteeYears',
				width:110,
				formart: function(d, fn){
					if(fn['guaranteeYears']){
						var text={'0':'年','1':'月','2':'日'};
						return fn['guaranteeYears']+text[fn['guaranteeType']];
					}
					return "";
				}
			},{
				name:'checkUserName',//复核员
				index:'checkUserName',
				width:80
			},{
				name:'relateStatus',
				index:'relateStatus',
				formart:function(d, fn){
					if(d==0){
						return '<a class="oprRelate">关联</a>';
					}else{
						return '<a class="oprRelate">已关联</a>';
					}
					
				},
				attachEvent:{
					click:{
						el:'a.oprRelate',
						callback:function(a, b, e){
							FMS.globalFun.stopPropagation(e);
							var index = b.indexOf("_");
							var id = b.substring(index+1);
							if(e=="关联"){
								FMS.product.relatePro(id);
							}else{
								FMS.product.relatePro(id);
							}
						}
					}
				},
				width:80,
				showTitle:false
			},{
				name:'operation',
				index:'operation',
				width:80,
				formart:function(d, fn){
					if(modeParam.productReview=="0"){
						return '<a class="icon gridIconFix iconEdit gridEdit"></a>';
					}else{
						return '<a class="icon gridIconFix iconEdit gridEdit"></a><a class="icon gridIconFix iconAssign gridAssign"></a>';
					}
					
				},
				attachEvent:{
					click:{
						el:'a',
						callback:function(a, b, e){
							var index = b.indexOf("_");
							var id = b.substring(index+1);
							if((a.srcElement == undefined ? a.target.className.indexOf("gridEdit") : a.srcElement.className.indexOf("gridEdit")) > 0){
							    FMS.product.editPro(id);
							} else if((a.srcElement == undefined ? a.target.className.indexOf("gridAssign") : a.srcElement.className.indexOf("gridAssign")) > 0){
								FMS.product.assignUser(id);
							}
						}
					}
				}
			}];
		if(modeParam.productReview=="0"){
			colModel.splice(9,1);
			colName.splice(9,1);
		}
		$("#dataTable").grid({
			colName:colName,
			colModel:colModel,
			colModelTmp:{
				sortable: false
			},
			headerFixed: true,
			resizable: false,
			multiSelect: true,
			multiSelectWidth:30,
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
			sortorder: 'desc',
			height: '100%',
			ajaxOptions: {
				type: 'post'
			},
			dataType: 'ajax',
			url: '/modules/product/getDataPage.action',
			pager: '#dataTablePage',
			pagerFixed: true,
			scrollWidth: 18,
			headTitle: true,
			ajaxDynamic: function(){
				return [{
					'name':'productInfo.pid',
					value : FMS.product.conditions.parentId
				},{
					'name' : 'productInfo.strName',
					value : FMS.product.conditions.strName
				},{
					'name' : 'productInfo.nenabled',
					value : FMS.product.conditions.nenabled
				},{
					'name' : 'productInfo.strCode',
					value : FMS.product.conditions.strCode
				},{
					'name' : 'productInfo.strRisklevel',
					value : FMS.product.conditions.strRisklevel
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
	/**
	 * 弹出框确认刷新树的按钮
	 * @param  {[type]} dialogId 弹出框的id
	 * @param  {[type]} url      后台接收的url
	 * @return {[type]}          [description]
	 */
	proTypeDialogButtons:function(dialogId, url){
		var obj = {
				"ok": {
					text: '确认',
					'class': 'bPrimary',
					click: function() {
						var form = $('#'+dialogId).find('form');
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
							url: url,
							type: 'post',
							dataType: 'json',
							success: function(data) {
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$(".reload").click();
									$('#'+ dialogId).dialog('close');
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
						$('#' + dialogId).dialog('close');
					}
				}
			}
		return obj;
	},
	addProType:function(id){
		FinanceUtil.dialog({
			id:"addProTypeDialog",
			title:"添加产品类型",
			overlay:true,
			draggable:true,
			width:420,
			height:250,
			iframeFix:false,
			ajax:{
				url: '/modules/product/toAddProTypePage.action',
				param: {
					'producttypeInfo.id':id
				},
				success:function(){
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI("#addProTypeDialog");
					FMS.globalFun.isIE6ChangeTipText('#addProTypeDialog');
				}
			},
			close: function(){
            	FMS.globalFun.isIE6ShowSelect("#wrapper");
            },
			buttons:FMS.product.proTypeDialogButtons('addProTypeDialog', '/modules/product/addProType.action')
		});
	},
	editProType:function(id){
		FinanceUtil.dialog({
			id:"editProTypeDialog",
			title:"编辑产品类型",
			overlay:true,
			draggable:true,
			width:420,
			height:250,
			iframeFix:false,
			ajax:{
				url: '/modules/product/toUpdateProTypePage.action',
				param: {
					'producttypeInfo.id':id
				},
				success:function(){
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI("#editProTypeDialog");
					FMS.globalFun.isIE6ChangeTipText('#editProTypeDialog');
				}
			},
			close: function(){
            	FMS.globalFun.isIE6ShowSelect("#wrapper");
            },
			buttons:FMS.product.proTypeDialogButtons('editProTypeDialog', '/modules/product/updateProType.action')
		});
	},
	delProType: function(id){
		jConfirm('确认要删除选中的产品类型吗？',{
			title:'提示',
			callback:function(value){
				if (value) {
					$.ajax({
						url: '/modules/product/deleteProType.action',
						data: {
							'producttypeInfo.id': id
						},
						success:function(data){
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
					});
				}
			}
		});
	},
	addPro:function(id){
		FinanceUtil.dialog({
			id:"addPro",
			title:"添加理财产品",
			overlay:true,
			draggable:true,
			width:460,
			height:540,
			iframeFix:false,
			ajax:{
				url: '/modules/product/toAddProPage.action',
				param: {
					'productInfo.pid': id//id是产品类型的id
				},
				success:function(){
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI("#addPro");
					FMS.globalFun.isIE6ChangeTipText("#addPro");
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
						var input = $('input[name=deadline]:checked').parents("label").next('input[type=text]')
						var value = input.val();
						if($.trim(value).length == 0){
							jAlert("请选择产品期限","错误",'error');
							return;
						}
						var form = $('#addPro').find('form');
						form.ajaxSubmit({
							beforeSubmit: function() {
								if (FinanceUtil.validate(form)) {
									if(!isIE6)FinanceUtil.loader.show('请稍候...');
									return true;
								} else {
									jAlert('信息输入有误', '错误', 'error');
									return false;
								}
							},
							url: '/modules/product/addProduct.action',
							type: 'post',
							dataType: 'json',
							success: function(data) {
								if(!isIE6)FinanceUtil.loader.hide();
								if (data && data.success) {
									$.sticky('操作成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
											
										}
									});
									$('#dataTable').grid('reload');
									$('#addPro').dialog('close');
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
						$('#addPro').dialog('close');
					}
				}
			}
		});
	},
	editPro:function(id){
		FinanceUtil.dialog({
			id:"editProDialog",
			title:"编辑产品",
			overlay:true,
			draggable:true,
			width:460,
			height:540,
			iframeFix:false,
			ajax:{
				url: '/modules/product/toUpdateProPage.action',
				param: {
					'productInfo.id':id
				},
				success:function(){
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI("#editProDialog");
					FMS.globalFun.isIE6ChangeTipText("#editProDialog");
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
						var form = $('#editProDialog').find('form');
						var input = $('input[name=deadline]:checked').parents("label").next('input[type=text]')
						var value = input.val();
						if($.trim(value).length == 0){
							jAlert("请选择产品期限","错误",'error');
							return;
						}
						form.ajaxSubmit({
							beforeSubmit: function() {
								if (FinanceUtil.validate(form)) {
									if(!isIE6) FinanceUtil.loader.show('请稍候...');
									return true;
								} else {
									jAlert('信息输入有误', '错误', 'error');
									return false;
								}
							},
							url: '/modules/product/updateProduct.action',
							type: 'post',
							dataType: 'json',
							success: function(data) {
								if(!isIE6) FinanceUtil.loader.hide();
								if (data && data.success) {
									$.sticky('操作成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
											
										}
									});
									$('#dataTable').grid('reload');
									$('#editProDialog').dialog('close');
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
						$('#editProDialog').dialog('close');
					}
				}
			}
		});
	},
	productAble:function(){
		var row = $('#dataTable').grid('getSelRow');
		var ids = $('#dataTable').grid('getDataByID',row,'id');
		if(!ids.length){
			jAlert("请选择要启用的产品！");
		} else {
			jConfirm('确认要启用选中的产品吗？',{
				title:'提示',
				callback:function(value){
					if (value) {
						$.ajax({
							url: '/modules/product/enableProducts.action',
							data: {
								'ids': ids
							},
							success:function(data){
								FMS.globalFun.doSuccess(data,'dataTable','启用成功')
							}
						});
					}
				}
			});
		}
	},
	//产品禁用
	productDisable:function(){
		var row = $('#dataTable').grid('getSelRow');
		var ids = $('#dataTable').grid('getDataByID',row,'id');
		if(!ids.length){
			jAlert("请选择要禁用的产品！");
		} else {
			jConfirm('确认要禁用选中的产品吗？',{
				title:'提示',
				callback:function(value){
					if (value) {
						$.ajax({
							url: '/modules/product/disableProducts.action',
							data: {
								'ids': ids
							},
							success:function(data){
								FMS.globalFun.doSuccess(data,'dataTable','禁用成功');
							}
						});
					}
				}
			});
		}
	},
	importFun:function(){
		FinanceUtil.dialog({
			id:'importDialog',
			title:'导入产品',
			overlay:true,
			draggable: true,
			width: 500,
			height: 240,
			iframeFix: false,
			ajax:{
				url:'/modules/product/toCheckOutPage.action',
				success:function(){
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI("#importDialog");
				}
			},
			close: function(){
            	FMS.globalFun.isIE6ShowSelect("#wrapper");
            },
			buttons:{
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
								url: '/modules/product/productUploadExcel.action',
								type: 'post',
								dataType: 'json',
								success: function(data) {
									FinanceUtil.loader.hide();
									if(data&&data.fieldErrors.proFile){
										$("#importDialog input").val("");
										jAlert(data.fieldErrors.proFile[0], '错误', 'error');
										return;
									}
									var resMap = data.resMap;
									if (resMap.errorNo) {
										$('#dataTable').grid('reload');
										$('#importDialog').dialog('close');
										if(resMap.errorInfo&&resMap.errorInfo != null){
											FinanceUtil.dialog({
												id:'importMsgDialog',
												title:'未导入的产品',
												overlay:true,
												draggable: true,
												width: 630,
												height: 430,
												iframeFix: false,
												ajax:{
													url:'/modules/product/toAlreadyImport.action',
													success:function(){
														$("#importMsgDialog #message").html(resMap.errorInfo.replace(/&/g, "<br/>"));
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
									} else {
										jAlert(resMap.errorInfo, '错误', 'error');
									}
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
	exportFun:function(){
		$.ajax({
			url:'/modules/product/exportProductsExcel.action',
			type:'post',
			dataType:'json',
			data:{
				'productInfo.pid':FMS.product.conditions.parentId,
				'productInfo.strName':FMS.product.conditions.strName,
				'productInfo.nenabled':FMS.product.conditions.nenabled,
				'productInfo.strCode':FMS.product.conditions.strCode,
				'productInfo.strRisklevel':FMS.product.conditions.strRisklevel
			},
			success:function(data){
				if(data && data.success){
					window.location.href = data.data.url+'?rnd=' + new Date().getTime();
				}else{
					jAlert(data.msg,'错误','error');
				}
				
			}
		});
	},
	//指定复核员
	assignUser:function(productId){
		var ids;
		if(typeof productId == "undefined"){
			var row = $('#dataTable').grid('getSelRow');
			ids = $('#dataTable').grid('getDataByID', row, 'id');
		}else{
			ids = productId;
		}
		if(!ids.length){
			jAlert("请选择要指定的产品！");
		}else{
			FinanceUtil.dialog({
				id:'assignUserDialog',
				title:'指定复核员',
				overlay:true,
				draggable: true,
				width: 350,
				height: 150,
				iframeFix: false,
				ajax:{
					url:'/modules/product/toAddCheckUserPage.action',
					success:function(){
						FMS.globalFun.isIE6HiddenSelect("#wrapper");
						FinanceUtil.initFormUI("#assignUserDialog");
					}
				},
				close: function(){
	            	FMS.globalFun.isIE6ShowSelect("#wrapper");
	            },
				buttons:{
					"ok": {
						text: '确认',
						'class': 'bPrimary assignButton',
						click: function() {
							var form = $('#assignUserDialog').find('form');
							var id = $("#checkId").val();
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
								url: '/modules/product/addCheckUser.action',
								type: 'post',
								dataType: 'json',
								data:{
									'userInfo.id':id,
									'ids':ids
								},
								success: function(data) {
									FinanceUtil.loader.hide();
									if (data && data.success) {
										$('#dataTable').grid('reload');
										$('#assignUserDialog').dialog('close');
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
							$('#assignUserDialog').dialog('close');
						}
					}
				}
			});
		}
		
	},
	//关联产品
	relatePro:function(productId){
		FinanceUtil.dialog({
			id:'relateProDialog',
			title:'关联证书',
			overlay:true,
			draggable:true,
			width:650,
			height:440,
			iframeFix:false,
			ajax:{
				url:'/modules/product/toCertConnectPage.action',
				param:{
					'productInfo.id':productId
				},
				success:function(){
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI("#relateProDialog");
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
						var ids = $(".related .certCard").map(function(index, elem) {
							return $(this).data("id");
						}).get();
						$.ajax({
							url: '/modules/product/addOrDelCertConnect.action',
							type: 'post',
							dataType: 'json',
							data: {
								'ids':ids,
								'productInfo.id':productId
							},
							beforesend:function(){
								FinanceUtil.loader.show('请稍候...');
							},
							success:function(data){
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$('#dataTable').grid('reload');
									$('#relateProDialog').dialog('close');
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
						$('#relateProDialog').dialog('close');
					}
				}
			}
		});
	}
};
$(function(){
	FMS.product.init();
	if(modeParam.productReview=="0"){
		$(".oprAssign").parent('a').remove();
	}
});