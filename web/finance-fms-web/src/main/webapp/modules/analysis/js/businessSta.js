var FMS = FMS || {};
FMS.businessSta = {
	init:function(){

	}
};
$(function(){
	if(isIE6){
		$("#tab-content").height($("#content").height()-$(".tabs").height());
	}
	//FMS.analysis.linkId = $("li.ui-state-active").find('a').attr("href");
	FMS.analysis.initOrgTree();
	FMS.analysis.globalSetting();
	FMS.analysis.bindEvent();
	//设置条件下面的样式
	$(".staInfo .info").each(function(index, el) {
		var _this = $(this);
		var image = _this.find(".top").data("image");
		var color = _this.find(".bottom").data("color");
		_this.find(".top").css("background","url("+image+") no-repeat 90% 50%");
		_this.find(".bottom").css("background",color);
	});
});


