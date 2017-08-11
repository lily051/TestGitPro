//form data to object
$.fn.serializeObject = function()
{
    var o = {};
    var a = this.serializeArray();
    $.each(a, function() {
        if (o[this.name] !== undefined) {
            if (!o[this.name].push) {
                o[this.name] = [o[this.name]];
            }
            o[this.name].push(this.value || '');
        } else {
            o[this.name] = this.value || '';
        }
    });
    return o;
};
/*
 * @Author Eli Pei
 * ipager组件，用以初始化load方法加载的分页器
 * @param	{Object} setting
 * @returns  void
 */
(function($){
	//默认配置
	var defaults = {
		pagerid             : 'ipager',      //divID
		filter 				: 'filterForm',	//过滤表单id
		totalRecords        : 8,	      	//总数据条数
		pno                 : 1,	      	//当前页码
		total               : 1,		  	//总页码
		limit               : 10,			//每页显示
		jumpTo              : 1,			//跳转页码
		url                 : ''  ,			//请求路径
		targetDiv           : 'picSearch',  //需要load的div
	},
	_constsData = {};
	function init(setting){
		this.pno = isNaN(setting.pno) ? 1 : parseInt(setting.pno);
		this.total = isNaN(setting.total) ? 1 : parseInt(setting.total);
		this.totalRecords = isNaN(setting.totalRecords) ? 0 : parseInt(setting.totalRecords);
		if(setting.pagerid){this.pagerid = setting.pagerid;}
		if(setting.limit){this.limit = setting.limit;}
		$('li','#' + this.pagerid).removeClass('disabled').attr("disabled", false);
		if(this.pno <= 1 ){
			$('.first , .prev','#' + this.pagerid).addClass('disabled').attr("disabled", true);
		}
		if(this.pno >= this.total){
			$('.next , .last','#' + this.pagerid).addClass('disabled').attr("disabled", true);
		}
		if(this.totalRecords == 0){
			this.pno = 0;
			this.total = 0;
		}
		//每次页面加载时 jsp都会重新渲染页面 并不需要判定是否恢复按钮disabled属性
		$('.totalRecords','#' + this.pagerid).text(this.totalRecords);
		$('.pno','#' + this.pagerid).text(this.pno);
		$('.total','#' + this.pagerid).text(this.total);
		$('select[name="limit"],select[name="page.pageSize"]','#' + this.pagerid).val(this.limit);
	};
	function getDataAndPost(setting,flag){
		if(flag){
			//如果存在则为过滤方法，把查询条件重置
			$('#'+ setting.pagerid + ' form').find('.jumpTo').val("1");
		}
		var pagerData = $('#'+ setting.pagerid + ' form').serializeObject();
		var allData;
		// allData = _constsData;

		var allData = $('#' + setting.filter ).serializeObject();
		// for (var i=pagerData.length-1; i >= 0; i--) {
		// 	allData.unshift( pagerData[i] );
		// }//合并数组
		
		var allData = $.extend({}, pagerData,allData);//合并object
		pagerData = null;
		if(!$(this).hasClass('disabled')){
			load(setting,allData);
		}
	}
	function load(setting,data){
		$('#'+ setting.targetDiv).load( setting.url ,data);
	}
	$.fn.ipager = function(setting){
		var setting = $.extend({}, defaults, setting || {});
		return this.each(function(){
			init(setting);
			var pager = {
				load: function(param){
					_constsData = param;	//当使用过滤条件时，过滤条件会存放到ipager插件的_constsData对象中
					var flag = true;
					getDataAndPost(setting,flag);
				}
			}
			$(this).data('pager', pager);
			$('.pagerBtn', this).on('click',function(){
				if($(this).hasClass("disabled")){
					return false;
				}
				if($(this).hasClass("next")){
					setting.pno = setting.pno + 1;
				}else if($(this).hasClass("prev")){
					setting.pno = setting.pno - 1;
				}else if($(this).hasClass("first")){
					setting.pno = 1;
				}else if($(this).hasClass("last")){
					setting.pno = setting.total;
				}else if($(this).hasClass("jumpToBtn")){
					var temp = $('input[name="start"],input[name="page.pageNo"]', '#'+ setting.pagerid + ' form').val();
					temp = temp > setting.total ? setting.total : temp;
					setting.pno = temp;
				}
				$('input[name="start"],input[name="page.pageNo"]', '#'+ setting.pagerid + ' form').val(setting.pno);
				_constsData = {};
				getDataAndPost(setting);
			});
			$('select[name="limit"],select[name="page.pageSize"]',this).on('change',function() {
				_constsData = {};
				getDataAndPost(setting);
			});
			
		});
	}
})(jQuery);
