/*
 * @file FMS 参数配置侧边栏逻辑
 */
var FMS = FMS || {};
FMS.configure = {
	init:function() {
		this.bindEvent();
	},
	bindEvent: function(){
		//点击li 加载页面
		$('.sideBar li').on('click',function(){
			if(isIE6){
				isFirstLoad = true;
			}
			var url = $(this).data('url');
			$("#main-wrap").load(url);
			$('#sideBar li').removeClass('active');
			$(this).addClass('active');
		});
		$('#sideBar li').eq(0).click();
	}
};
$(function() {
	FMS.configure.init();
});
