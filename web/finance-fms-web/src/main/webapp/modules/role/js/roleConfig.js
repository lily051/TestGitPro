 /**
 * ------------------------------------------------------------------
 * @file
 * @author   Eli Pei
 * ------------------------------------------------------------------
 */
/**
 * 添加角色初始化页面 加载不同的树
 * @param {[type]} type 0是中心管理员 1是理财经理 
 */
function addRoleInitPage(){
	loadPowerTree();
	//加载权限树
	function loadPowerTree() {
		var tree = $('#functionTree').tree({
			view:{
				showIcon:false
			},
			ajax: {
				url: '/web/tree/getALLPowerTree.action'
			},
			check:{
				enable: true
			}
		});
		return tree;
	};
}

//编辑角色初始化页面
function editRoleInitPage(id){
	$('#functionTree').tree({
		view:{
			showIcon:false
		},
		ajax: {
			url: '/web/tree/getPowerTreeByRoleId.action',
			data: {'roleInfo.id': id}
		},
		check:{
			enable: true
		}
	});
}
