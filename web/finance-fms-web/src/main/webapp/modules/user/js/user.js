/*
 * @file FMS角色管理逻辑
 * @author Eli Pei
 */
var FMS = FMS || {};
FMS.user = {
	init:function() {
		userName = staffNo = net = onlineStatus = "" ;//初始化过滤条件
		this.globleSetting();
		this.bindEvent();
		this.userGrid();
	},
	//placeHolder
	globleSetting: function() { 
		$('input, textarea').placeholder(); 
		$('select','.filterDIV').uniform({selectAutoWidth:true});
	},
	bindEvent:function(){
		$('.gridSearch').on('click', function() {
			keyword = $(this).next().val();
			$('#userDataTable').grid('reload');
		});
		$('#gridSearch').keydown(function(e) {
			var key = e.keyCode;
			if (key == 13) {
				$(this).prev().click();
			}
		});
		//点击查询响应事件
		$('#search').on('click',function() {
			userName = $('#username').val();
			staffNo = $('#staffNo').val();
			net = $('#dept').val();
			onlineStatus = $('#onlineStatus').val();
			$('#userDataTable').grid('dofiltler');
		});
		//点击重置响应事件
		$('#clear').on('click',function() {
			userName = staffNo = net = onlineStatus = "";
			$('#username').val('');
			$('#staffNo').val('');
			$('#dept').val('');
			$('#filterForm select').each(function(){
				$(this).find('option:first').attr('selected','selected');
			});
			$('select','.filterDIV').uniform({selectAutoWidth: true});
			$('#userDataTable').grid('dofiltler');
		});
		$('.oprAddUser').on('click',function() {
			FMS.user.addUserFun();
		});
		$('.oprAbleUser').on('click',function(){
			FMS.user.ableUser();
		});
		$('.oprDisableUser').on('click',function(){
			FMS.user.disableUser();
		});
		$('.oprImport').on('click', function() {
			FMS.user.importFun();
		});
		$('.oprExport').on('click', function() {
			if(!FMS.globalFun.exportLimit('.ui-paging-info')) return false;
			FMS.user.exportFun();
		});
		$('.oprDeleteUser').on('click',function(){
			FMS.user.multDelete();
		});
		$('.oprResetUser').on('click',function(){
		 	FMS.user.resetPassword();
		});
		//过滤键的动态效果实现
		$('#filtrate').on('click',function(){
			FMS.globalFun.showSearchCondition($(this),'userDataTable');
		});
	},
	//加载组织树
	initPowerTree: function(nodeId,id){
		if(!nodeId){
			var tree = $('#'+id).tree({
				ajax: {
					url: '/web/tree/getRegionTree.action'
				},
				check:{
					enable: false
				},
				callback: {
					onNodeCreated: function(e, treeId, node) {
						if (node.getParentNode() == null) {
							var treeObj = $.fn.zTree.getZTreeObj(treeId);
							treeObj.selectNode(node);
							var node = treeObj.getSelectedNodes()[0];
							treeObj.expandNode(node, true,false ,true);
						}
					}
				}
			});
			return tree;
		}else{
			var regid = 'reg_' + nodeId;
			var tree = $('#'+id).tree({
				ajax: {
					url:'/web/tree/getRegionTreeByUserId.action',
					data:{
						'usersInfo.id':$("#userInfoId").val()
					}
				},
				callback: {
					onNodeCreated: function(e, treeId, node) {
						if (node.getParentNode() == null) {
							var treeObj = $.fn.zTree.getZTreeObj(treeId);
							treeObj.expandNode(node, true,false ,true);
						}
						if (node.id == regid) {
							$('#'+id).tree().selectNode(node, false);
							//var treeObj = $.fn.zTree.getZTreeObj(treeId);
							//FMS.user.hasParentNode(node,treeObj);
						}
					}
				}
			});
			return tree;
		}
		
	},
	hasParentNode:function(node,treeObj) { 
		var currnode = node.getParentNode();
		treeObj.showNode(currnode);
		if(currnode){
			FMS.user.hasParentNode(currnode,treeObj);
		}else{
		    treeObj.expandNode(node, true, true, true);
		}
	},
	//加载监控点树
	initResouseTree: function(type){
		var url;
		var userId = $('#userInfoId').val();
		if(type == 'edit'){
			url = '/web/tree/getChannelTreeByUserId.action';
		}else{
			url = '/web/tree/getInitChannelTree.action';
		}
		var resTree = $('#functionTree').tree({
			async: {
				enable: true,
				url: url,
				autoParam: ['id=strId'],
				otherParam: ["usersInfo.id", userId]
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
					var treeObj = $.fn.zTree.getZTreeObj(treeId);
					if(node!=null&&node.extra.hasCheck){
						treeObj.checkNode(node,node.getCheckStatus().checked,true);
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
				onClick:function(e,treeId,node){
					if(node.id.indexOf("channel_")>-1){
						var channelId = node.id.split('_')[1];
						try{
							$.ajax({
								url:'/modules/user/getDevChannelInfo.action',
								type:'post',
								dataType:'json',
								data:{
									'recordPojo.channelId':channelId
								},
								success:function(data){
									var recordPojo = data.data.recordPojo;
									var sendXml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><Preview><CamType>"+recordPojo.camType+"</CamType><CamID>1</CamID>"+
									"<CamName>"+recordPojo.camName+"</CamName><DevType>0</DevType><RegionID>"+recordPojo.regionId+"</RegionID>"+
									"<DevIP>"+recordPojo.devIp+"</DevIP><DevPort>"+recordPojo.devPort+"</DevPort><User>"+recordPojo.user+"</User>"+
									"<Pwd>"+recordPojo.pwd+"</Pwd><Channel>"+recordPojo.channel+"</Channel><RealPlayType>"+recordPojo.realPlayType+"</RealPlayType><VideoFormat>0</VideoFormat>"+
									"<CamRight></CamRight><StreamIP></StreamIP><StreamPort>9557</StreamPort><Protocol>0</Protocol><UserID>3</UserID>"+
									"<CollectorIP></CollectorIP><PhonePort>9527</PhonePort><VedioPort>9537</VedioPort><TimeOut>45</TimeOut><DelayTime>15</DelayTime></Preview>";
									if(judgeOcxAble('ocxPreview')){
										try{
											var osdXML = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><BussInfo><OSDNum>4</OSDNum></BussInfo>";
											document.getElementById("ocxPreview").StartPreview(sendXml);
											document.getElementById("ocxPreview").SetShowString(osdXML);
										}catch(e){
										}
									}
								}
							});
						}catch(e){

						}
					}
					
				}
			}
		});
		return resTree;
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
		function setNodeHasCheck(node){
			if(node.checked==true||node.halfCheck==true){
				node.halfCheck=false;
			}else if(node.checked==false){
				//setParentNotCheck(node);
			}
			node.extra.hasCheck = true;
			var childs=node.children;
			if(childs){
				for(var i=0;i<childs.length;i++){
					setNodeHasCheck(childs[i])
				}
			}
		}
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
				}
				if(childrenCheck==false){
					parentNode.halfCheck=false;
					parentNode.check_Child_State=-1;
					$("#"+parentNode.tId+"_check").removeClass('checkbox_false_part').addClass('checkbox_false_full');
					setParentNotCheck(parentNode);
				}
			}
		}
	},
	getCheckedOrg: function(){
		var resourceTree = $('#functionTree').tree(),
			selectedResNodes,
			resList = [];
		if(resourceTree != null){
			selectedResNodes = resourceTree.getSelectedNodes()[0];
			resList.push({
				nregionId: new Number(selectedResNodes.id.slice(Constants.PREFIX.ORG.length)).valueOf()
			});
		}
		return resList;
	},
	/**
	 * 获取选中的通道
	 * @return {[type]} [description]
	 */
	getCheckedChannel:function(){
		if(modeParam.usbAndIpc=="usb"){
			return false;
		}
		var resTree = $.fn.zTree.getZTreeObj('functionTree');
		var selectedNodes={}, deviceList = [] , regionIds = [],channelName=[],checkedRegNodes=[],checkedChanNodes=[];
		var nodes = resTree.transformToArray(resTree.getNodes());
		for(var i=0; i < nodes.length;i++){
		    var node=nodes[i];
		    if(node.checked){
		        var idInfo = node.id;
		        if(idInfo.indexOf("reg")>-1){
		            checkedRegNodes.push(idInfo.split("_")[1]);
		            //regionIds.push({"id":idInfo.split("_")[1],"status":true});
		        }else if(idInfo.indexOf("channel")>-1){
		            checkedChanNodes.push(idInfo.split("_")[1]);
		            //deviceList.push({"devChannel":idInfo.split("_")[1],"status":true,'devId':Number(node.extra.devId).valueOf()});
		        }
		       
		    }
		    if(!node.isHidden){
		        if(!node.open){
		            if(node.extra.hasCheck){
		                if(node.id.slice(0,Constants.PREFIX.REGION.length) == 'reg_'){//选的是组织，则push到regionIdList
		                    regionIds.push({"id":Number(node.id.slice(Constants.PREFIX.REGION.length)).valueOf(),"status":node.getCheckStatus().checked});
		                }else if(node.id.slice(0,Constants.PREFIX.CHANNEL.length) == 'channel_'){//选的是监控点，则push到deviceList
		                    deviceList.push({"devChannel":Number(node.extra.devChannel).valueOf(),"status":node.getCheckStatus().checked,'devId':Number(node.extra.devId).valueOf()});
		                    channelName.push(node.label);
		                }
		            }
		        }
		    }
		}
		selectedNodes.channelInfo = {'regionIdList':regionIds,'deviceList':deviceList};
		selectedNodes.checkedNodes = {'checkedRegNodes':checkedRegNodes,'checkedChanNodes':checkedChanNodes};
		return selectedNodes;
	},
	/**
	 * 获取选中的证书
	 * @return {[type]} [description]
	 */
	getSelectedCerts:function(){
		var data=[];
		$('.certCard.selected').each(function(){
		    var _self = $(this);
		    data.push({"ncrtificateId":_self.data('id')});
		});
		return data;
	},
	//user grid begin
	userGrid: function() {
		var colName = ['ida', '用户名', '员工编号','真实姓名','用户类型','角色分配','启用状态','所在网点','到期时间','权限关联','操作'],
			colModel = [{
			name: 'id',
			index: 'id',
			width: "1",
			hidden: true
		}, {
			name: 'strName',
			index: 'strName',
			formart: function(d, fn) {
				return '<a class="userName">' + d + '</a>';
			},
			attachEvent: {
				click: {
					el: 'a.userName',
					callback: function(a, b,e) {
						var id = $.trim($("#" + b).find('td').eq(1).text());
						if(id === "1"){
							$.sticky('管理员不可进行此操作！', '提示', 'attention');
							return;
						}
						FMS.user.editUserFun(id);
					}
				}
			},
			width: 70
		},{
			name: 'userNo',
			index: 'userNo',
			width: 80
		},{
			name: 'strCode',
			index: 'strCode',
			width: 80
		},{
			name: 'sysTypeName',
			index: 'sysTypeName',
			width: 90
		},{
			name: 'roleName',
			index: 'roleName',
			width: 90
		},{
			name: 'nstate',
			index: 'nstate',
			width: 70,
			showTitle: false,
			formart: function(d, fn) {
				if(fn.nstate == 1){
					return "<span class='unableStyle'>禁用</span>";
				}else if(fn.nstate == 0){
					return "<span class='enableStyle'>启用</span>";
				}else{
					return '未知';
				}
			}
		},{
			name: 'net',
			index: 'net',
			width: 110
		},{
			name: 'strDate',
			index: 'strDate',
			width: 150
		},{
			name: 'authRelate',
			width: 70,
			showTitle: false,
			formart:function(d, fn){
				if (modeParam.usbAndIpc == "ipc" && modeParam.channelIpBind == "true" && d != 1) {
					return "<span class='unableStyle'>无</span>";
				} else {
					if(d==0){
						return "<span class='unableStyle'>未关联</span>";
					}else if(d == 1){
						return "<span class='enableStyle'>已关联</span>";
					}else{
						return "<span class='unableStyle'>未关联</span>";
					}
				}
			}
		},{
			name: 'operation',
			index: 'operation',
			width: 70,
			formart: function(d, fn) {
				return '<a class="icon gridIconFix iconEdit gridEdit"></a><a class="icon gridIconFix iconDelete gridDelete"></a>';
			},
			attachEvent: {
				click: {
					el: 'a',
					callback: function(a, b, e) {
						var id = $.trim($("#" + b).find('td').eq(1).text());
						if((a.srcElement == undefined ? a.target.className.indexOf("gridEdit") : a.srcElement.className.indexOf("gridEdit")) > 0){
							FMS.user.editUserFun(id);
						} else if((a.srcElement == undefined ? a.target.className.indexOf("gridDelete") : a.srcElement.className.indexOf("gridDelete")) > 0){
							FMS.user.deleteSingle(id);
						}
					}
				}
			}
		}];
		if(modeParam.usbAndIpc=="usb"){
			colModel.splice(9,1);
			colName.splice(9,1);
		}
		$('#userDataTable').grid({
			colName: colName,
			colModel: colModel,
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
			url: '/modules/user/getDataPage.action',
			pager: '#userDataTablePage',
			pagerFixed: true,
			scrollWidth: 18,
			headTitle: true,
			ajaxDynamic: function() {
				return [{
					name: 'userInfo.strName',
					value: userName
				},{
					name: 'userInfo.userNo',
					value: staffNo
				},{
					name: 'userInfo.net',
					value: net
				},{
					name: 'userInfo.nonline',
					value: onlineStatus
				}];
			},
			events:{
				init:function(a, b){
					FMS.globalFun.addVerticalLine();
				},
				onEndRequest:function(event,elm){
					FMS.globalFun.removeGridColumn('userDataTable',elm._pageData.totalPage);
				}
			}
		});
	},
	addUserFun:function(){
		FinanceUtil.dialog({
			id: 'addUser',
			title: '添加用户',
			overlay: true,
			draggable: true,
			width: 753,
			height: 580,
			iframeFix: iframeFix,
			beforeClose:function(){
				try{
					document.getElementById("ocxPreview").StopPreview(0);
				}catch(e){
				}
			},
			ajax: {
				url: '/modules/user/toAddUserPage.action',
				success: function() {
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI('#addUser');
					FMS.globalFun.isIE6ChangeTipText('#addUser');
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
						var form = $('#addUser').find('form'),data={},user=$('#bindUser').val();
						/**
						 * 0是中心管理员 1是理财经理
						 * @param  {[type]} user [description]
						 * @return {[type]}      [description]
						 */
						if(user == '0'){
							var regionList = FMS.user.getCheckedOrg();//得到选中的组织
							if(regionList.length>0){
								 data.regionList = regionList;
							}else{
								jAlert('请配置权限', '错误', 'error');
								return false;
							}
						}else if(user == '1'){
							var authInfo,certInfo;
							if($("#channelChoose").children().length>0){//判断有没加载通道树
								authInfo = FMS.user.getCheckedChannel();
								data = authInfo.channelInfo||{};
							}
							certInfo = FMS.user.getSelectedCerts();
							/*if(channelInfo.checkedNodes.checkedRegNodes.length==0&&channelInfo.checkedNodes.checkedChanNodes.length==0){
								jAlert('请选择权限区域', '错误', 'error');
							}*/
							if(certInfo.length>0){
								data.crtificateList = certInfo;
							}
						}
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
							url: '/modules/user/addUser.action',
							type: 'post',
							dataType: 'json',
							data:{'jsonStr': $.jsonToStr(data)},
							success: function(data) {
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$.sticky('操作成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
											
										}
									});
									$('#addUser').dialog('close');
									$('#userDataTable').grid('reload');
								} else if (data && data.msg) {
									jAlert(data.msg, lan.text('dialog.title.error'), 'error');
								}
							}
						});
					}
				},
				"cancel": {
					text: '取消',
					click: function() {
						$('#addUser').dialog('close');
					}
				}
			}
		});
	},
	editUserFun: function(id){
		if(id==1){
			$.sticky('管理员不能编辑！', '提示', 'attention');
			return;
		}
		FinanceUtil.dialog({
			id: 'editUser',
			title: '编辑用户',
			overlay: true,
			draggable: true,
			width: 753,
			height: 580,
			iframeFix: iframeFix,
			beforeClose:function(){
				try{
					document.getElementById("ocxPreview").StopPreview(0);
				}catch(e){
				}
			},
			ajax: {
				url: '/modules/user/toUpdatePage.action',
				param: {
					'userInfo.id': id
				},
				success: function() {
					FMS.globalFun.isIE6HiddenSelect("#wrapper");
					FinanceUtil.initFormUI('#editUser');
					FMS.globalFun.isIE6ChangeTipText('#editUser');
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
						var form = $('#editUser').find('form');
						var power = $('#chooseAuth').data('power');
						var cert = $('#chooseCertEdit').data('cert');
						var data = {};
						var user = $('#bindUser').val();
						if(!cert){//用户未进行证书选择，保存原数据
							var ids = $('.certIds').toArray();
							cert = [];
							$.each(ids,function(){
								cert.push({"ncrtificateId":Number($(this).val())});
							})
						}
						if($("#relateCert").children().length>0){
							cert = FMS.user.getSelectedCerts();
						}
						if($("#channelChoose").children().length>0){
							if(user=='1'){
								power = FMS.user.getCheckedChannel().channelInfo;
							}else{
								power = FMS.user.getCheckedOrg()
							}
						}
                        if(user == '0'){
                            data.regionList = power;
                        }else if(user == '1'){
                            data = power;
                            try{
                            	data.crtificateList = cert;
                            }catch(e){
                            	
                            }
                        }
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
							url: '/modules/user/updateUser.action',
							type: 'post',
							dataType: 'json',
							data:{'jsonStr': $.jsonToStr(data)/*,'userInfo.strPassword':strPwd*/},
							success: function(data) {
								FinanceUtil.loader.hide();
								if (data && data.success) {
									$.sticky('操作成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
											
										}
									});
									$('#editUser').dialog('close');
									$('#userDataTable').grid('reload');
								} else if (data && data.msg) {
									jAlert(data.msg, '错误', 'error');
								}
							},
							error:function(xhr){
								//jAlert("12312", '错误', 'error');
							}
						});
					}
				},
				"cancel": {
					text: '取消',
					click: function() {
						$('#editUser').dialog('close');
					}
				}
			}
		});
	},
	//启用账户
	ableUser: function() {
		var row = $('#userDataTable').grid('getSelRow');
		var ids = $('#userDataTable').grid('getDataByID',row,'id');
		if($.inArray(1, ids) > -1){ //判断多选时候包括系统管理员
			$.sticky('管理员不可进行此操作！', '提示', 'attention');
			return;
		}
		if (!ids.length) {
			jAlert('请选择需要启用的用户！');
		} else {
			jConfirm('确认要启用当前用户吗？', {
				title: '提示',
				callback: function(value) {
					if (value) {
						$.ajax({
							url: '/modules/user/enableUsers.action',
							data: {
								'ids': ids
							},
							success: function(data) {
								if (data && data.success) {
									$.sticky('操作成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
											$('#userDataTable').grid('reload');
										}
									});
								} else {
									jAlert(data.msg, '错误', 'error');
								}
							}
						});
					}
				}
			});
		}
	},
	//禁用账户
	disableUser: function() {
		var row = $('#userDataTable').grid('getSelRow');
		var ids = $('#userDataTable').grid('getDataByID',row,'id');
		if($.inArray(1, ids) > -1){ //判断多选时候包括系统管理员
			$.sticky('管理员不可进行此操作！', '提示', 'attention');
			return;
		}
		if (!ids.length) {
			jAlert('请选择需要禁用的用户！');
		} else {
			jConfirm('确认要禁用当前用户吗？', {
				title: '提示',
				callback: function(value) {
					if (value) {
						$.ajax({
							url: '/modules/user/disableUsers.action',
							data: {
								'ids': ids
							},
							success: function(data) {
								if (data && data.success) {
									$.sticky('操作成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
											$('#userDataTable').grid('reload');
										}
									});
								} else {
									jAlert(data.msg, '错误', 'error');
								}
							}
						});
					}
				}
			});
		}
	},
	//重置密码 
	resetPassword: function() {
		var row = $('#userDataTable').grid('getSelRow');
		var ids = $('#userDataTable').grid('getDataByID',row,'id');
		if($.inArray(1, ids) > -1){ //判断多选时候包括系统管理员
			$.sticky('管理员不可重置密码！', '提示', 'attention');
			return;
		}
		console.info(ids);
		if (!ids.length) {
			jAlert('请选择需要重置密码的数据！');
		} else {
			jConfirm('确认要重置当前记录的密码吗？', {
				title: '提示',
				callback: function(value) {
					if (value) {
						$.ajax({
							url: '/modules/user/resetPassword.action',
							data: {
								'ids': ids
							},
							success: function(data) {
								if (data && data.success) {
									$.sticky('操作成功', {
										type: "ok",
										title: '操作成功',
										afterclose: function() { //'完成'
											$('#userDataTable').grid('reload');
										}
									});
								} else {
									jAlert(data.msg, lan.text('dialog.title.error'), 'error');
								}
							}
						});
					}
				}
			});
		}
	},
	importFun: function(){
		FinanceUtil.dialog({
			id: 'importDialog',
			title: '导入用户',
			overlay: true,
			draggable: true,
			width: 500,
			height: 240,
			iframeFix: iframeFix,
			ajax: {
				url: '/modules/user/toCheckOutPage.action',
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
								url: '/modules/user/userUploadExcel.action',
								type: 'post',
								dataType: 'json',
								success: function(data) {
									FinanceUtil.loader.hide();
									if(data&&data.fieldErrors.userFile){
										$("#importDialog input").val("");
										jAlert(data.fieldErrors.userFile[0], '错误', 'error');
										return;
									}
									var resMap = data.resMap;
									if (resMap.errorNo) {
										$('#userDataTable').grid('reload');
										$('#importDialog').dialog('close');
										if(resMap.errorInfo&&resMap.errorInfo != null){
											FinanceUtil.dialog({
												id:'importMsgDialog',
												title:'未导入的用户',
												overlay:true,
												draggable: true,
												width: 630,
												height: 430,
												iframeFix: iframeFix,
												ajax:{
													url:'/modules/user/toAlreadyImport.action',
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
									}else {
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
	exportFun: function(){
		$.ajax({
			url:'/modules/user/exportUsersExcel.action',
			type:'post',
			dataType:'json',
			data:{
				'userInfo.strName':userName,
				'userInfo.userNo':staffNo,
				'userInfo.net':net,
				'userInfo.nonline':onlineStatus
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
	//单一删除 
	deleteSingle: function(id) {
		if(id == 1){
			$.sticky('管理员不允许删除！', '提示', 'attention');
			return;
		}
		jConfirm('确认要删除当前记录吗？', {
			title: '提示',
			callback: function(value) {
				if (value) {
					$.ajax({
						url: '/modules/user/deleteUsers.action',
						data: {
							'ids': [id]
						},
						success: function(data) {
							if (data && data.success) {
								$('#userDataTable').grid('reload');
								$.sticky('操作成功', {
									type: "ok",
									title: '操作成功',
									afterclose: function() { //'完成'
									}
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
	//删除多条角色方法
	multDelete: function() {
		var row = $('#userDataTable').grid('getSelRow');
		var ids = $('#userDataTable').grid('getDataByID',row,'id');
		if($.inArray(1, ids) > -1){ //判断多选时候包括系统管理员
			$.sticky('管理员不允许删除！', '提示', 'attention');
			return;
		}
		console.info(ids);
		if (!ids.length) {
			jAlert('请选择需要删除的数据！');
		} else {
			jConfirm('确认要删除当前记录吗？', {
				title: '提示',
				callback: function(value) {
					if (value) {
						$.ajax({
							url: '/modules/user/deleteUsers.action',
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
									$('#userDataTable').grid('reload');
								} else {
									jAlert(data.msg, lan.text('dialog.title.error'), 'error');
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
	FMS.user.init();
});