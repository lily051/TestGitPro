var FMS = FMS || {};
FMS.regionTree = {
	init:function(id,treeUrl,nodeCreate,nodeClick){
		$('#keyValue').val('');
		var treeInfo = refresh();
		function refresh(type){
			var tree = $("#"+id).tree({
				ajax:{
					url:treeUrl//TODO 
				},
				callback: {
					onNodeCreated:function(event, treeId, treeNode){
						/*if (treeNode.getParentNode() == null) {
							var treeObj = $.fn.zTree.getZTreeObj(treeId);
							treeObj.selectNode(treeNode);
							var node = treeObj.getSelectedNodes()[0];
							treeObj.expandNode(node, true,false ,true);
							FMS.video.conditions.regionId = treeNode.id.split('_')[1];
							if(type==1){
								$("#videoDatatable").grid("reload");
							}else{
								FMS.video.videoGrid();
							}
						}*/
						nodeCreate(event, treeId, treeNode,type)
					},
					onClick:function(event, treeId, treeNode){
						nodeClick(event, treeId, treeNode)
					}
				}
			});
			return tree;
		}
		$(".reload").click(function(){//刷新树
			$("#keyValue").val('');
			$(".treeDelete").hide();
			treeInfo = refresh(1);
		});
		$("#orgTreeSearch").on('click', function(event) {
			searchTree(id,'label');
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
	}
};
