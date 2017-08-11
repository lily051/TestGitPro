var isIE=!!window.ActiveXObject;
var isIE6=isIE&&!window.XMLHttpRequest,
	iframeFix = false,
	isFirstLoad = true;
	if(isIE6){
		iframeFix = true;
	}
var FMS = FMS || {};
FMS.globalFun = {
	setContentWidth:function(){
		if(isIE6){
			var contentHeight = $('#content').height();
			$(".datatableStyle").height(contentHeight-70);
			$(".fullfit .treeview").height(contentHeight-46);
			if($('body').width()<1280){
				$('#content').width(1280);
			}else{
				$('#content').width($('body').width());
			}
		}
	},
	_stopIt:function(e){
		if(e.returnValue){  
		    e.returnValue = false ;  
		}  
		if(e.preventDefault ){  
		    e.preventDefault();  
		}                 
		return false;  
	},
	doSuccess:function(data, id, optionTip){
		if (data && data.success) {
			$('#'+id).grid('reload');
			$.sticky(optionTip, {
				type: "ok",
				title: '操作成功'
			});
		} else {
			jAlert(data.msg, '错误', 'error');
		}
	},
	stopPropagation:function(e){
		if(e.stopPropagation){
			e.stopPropagation();
		}else{
			e.cancelBubble = true;
		}
	},
	isIE6ChangeTipText:function(id){
		if(isIE6){
			$(id).find("input,textarea").removeAttr('isoverflown');
		}
	},
	isIE6HiddenSelect:function(id){
		if(isIE6){
			$(id).find("select").css("visibility", "hidden");
		}
	},
	isIE6ShowSelect:function(id){
		if(isIE6){
			$(id).find("select").css("visibility", "visible");
		}
	},
	/**
	 * ie6 td的内容外面加span标签 并且移除查询后多出的第一行数据
	 * @param  {[type]} tableId [description]
	 * @param  {[type]} totalPage 页数
	 * @return {[type]}         [description]
	 */
	removeGridColumn:function(tableId, totalPage){
		if(isIE6){
			$("#"+tableId+" td").each(function(index, el) {
				$(el).wrapInner("<span></span>");
			});
			if(!isFirstLoad){
				var firstTr = $("#"+tableId).find('tbody').eq(0).find('tr').eq(0);
				var trId = firstTr.attr('id');
				$("#"+trId).remove();
				if($(".nodata").length>0){//查询出来无结果的时候重新定义为第一次载入
					isFirstLoad = true;
				}
			}else{
				if(totalPage!=0){
					isFirstLoad = false;
				}
			}
		}else{
			return false;
		}
	},
	/**
	 * 时间1年比较
	 * @param  {[type]} startTime [description]
	 * @param  {[type]} endTime   [description]
	 * @return {[type]}           [description]
	 */
	compareTime:function(startTime,endTime){
		var year = endTime.split("-")[0]-1;
		var newEnd = year+"-"+endTime.split("-")[1]+"-"+endTime.split("-")[2];
		var sTime = Date.parse(startTime.replace(/-/g, '/')),
			eTime = Date.parse(newEnd.replace(/-/g, '/'));
		if(sTime-eTime>=0){
			return true;
		}else{
			return false;
		}
	},
	/**
	 * 基础配置显示查询条件的方法	
	 * @param  {[type]} _slef       [点击的对象]
	 * @param  {[type]} tableId     [表格的id]
	 * @return {[type]}             [description]
	 */
	showSearchCondition:function(_self,tableId){
		var $Mutex = _self.data('mutex'),
			top_default = _self.parent("div").outerHeight(),
			filterH = $("#filterForm").height()+10;
		if($Mutex == 'close'){
			$(".deviceMain").css("overflow-y",'hidden');
			$('#filterFormBox').stop().animate({height: filterH});
			if(isIE6){
				var tableHeight = $(".datatableStyle").height();
				$('.datatableStyle').stop().animate({top: top_default+filterH,height:tableHeight-filterH},function(){
					$("#"+tableId).grid("reload");
				});
			}else{
				$('.datatableStyle').stop().animate({top: top_default+filterH},function(){
					$("#"+tableId).grid("resizeWidth");
				});
			}
			_self.data('mutex','open');
			_self.addClass('filtrateIconOpen');
		}else if($Mutex == 'open'){
			$('#filterFormBox').stop().animate({height: 0});
			if(isIE6){
				var tableHeight = $(".datatableStyle").height();
				$('.datatableStyle').stop().animate({top: top_default,height:tableHeight+filterH},function(){
					$("#"+tableId).grid("reload");
				});
			}else{
				$('.datatableStyle').stop().animate({top: top_default},function(){
					$("#"+tableId).grid("resizeWidth");
				});
			}
			_self.data('mutex','close');
			_self.removeClass('filtrateIconOpen');
		}
	},
	/**
	 * 业务应用显示查询条件的方法
	 * @param  {[type]} _self   点击的对象
	 * @param  {[type]} tableId 表格的id
	 * @return {[type]}         [description]
	 */
	busiShowCondition:function(_self, tableId){
		var toogle = _self.data("toogle"),
			hideH = $("#hideCondition").find('fieldset').eq(0).height(),
			top_default = Number($(".datatableStyle").css("top").replace(/[^0-9]/ig,""));
		if(toogle == "down"){
			$("#hideCondition").animate({height:hideH});
			if(isIE6){
				var tableHeight = $(".datatableStyle").height();
				$(".datatableStyle").animate({top:top_default+hideH,height:tableHeight-hideH},function(){
					$("#"+tableId).grid("reload");
				});
			}else{
				$(".datatableStyle").animate({top:top_default+hideH},function(){
					$("#"+tableId).grid("resizeWidth");
				});
			}
			_self.text("收起更多").data("toogle","up").css("background",'url("/images/icons/open-up.png") no-repeat right');
		}else if(toogle == "up"){
			$("#hideCondition").animate({height:0},function(){
				//$("#hideCondition").find("input").val('').end().find("select").val('');
				FinanceUtil.initFormUI("#hideCondition");
			});
			if(isIE6){
				var tableHeight = $(".datatableStyle").height();
				$(".datatableStyle").animate({top:top_default-hideH,height:tableHeight+hideH},function(){
					$("#"+tableId).grid("reload");
				});
			}else{
				$(".datatableStyle").animate({top:top_default-hideH},function(){
					$("#"+tableId).grid("resizeWidth");
				});
			}
			_self.text("展开更多").data("toogle","down").css("background",'url("/images/icons/open-down.png") no-repeat right');
		}
	},
	getSelectedTreeNode:function(id){
		var treeInfo = $.fn.zTree.getZTreeObj(id);
		var selNode = treeInfo.getSelectedNodes()[0];
		return selNode;
	},
	addVerticalLine:function(){
		$('.ui-jqgrid-hbox').find('.ui-jqgrid-resize').replaceWith('<span class="ui-jqgrid-resize-ltr">|</span>');
		$('.ui-jqgrid-hbox').find("tr.ui-jqgrid-labels").find("th").last().prepend('<span class="ui-jqgrid-resize-ltr">|</span>');
	},
	tableHeight_ie6:function(){
		if(isIE6){
			var parentH = $("#content").height(),preHeight=0;
			$(".datatableStyle").prevAll().each(function(index, el) {
				preHeight+=$(el).outerHeight();
			});
			$(".datatableStyle").height(parentH-preHeight-40);
		}
	},
	loadXML:function(xmlString){
		var xmlDoc=null;
		//判断浏览器的类型
		//支持IE浏览器 
		if(!window.DOMParser && window.ActiveXObject){   //window.DOMParser 判断是否是非ie浏览器
		    var xmlDomVersions = ['MSXML.2.DOMDocument.6.0','MSXML.2.DOMDocument.3.0','Microsoft.XMLDOM'];
		    for(var i=0;i<xmlDomVersions.length;i++){
		        try{
		            xmlDoc = new ActiveXObject(xmlDomVersions[i]);
		            xmlDoc.async = false;
		            xmlDoc.loadXML(xmlString); //loadXML方法载入xml字符串
		            break;
		        }catch(e){
		        }
		    }
		}
		//支持Mozilla浏览器
		else if(window.DOMParser && document.implementation && document.implementation.createDocument){
		    try{
		        /* DOMParser 对象解析 XML 文本并返回一个 XML Document 对象。
		         * 要使用 DOMParser，使用不带参数的构造函数来实例化它，然后调用其 parseFromString() 方法
		         * parseFromString(text, contentType) 参数text:要解析的 XML 标记 参数contentType文本的内容类型
		         * 可能是 "text/xml" 、"application/xml" 或 "application/xhtml+xml" 中的一个。注意，不支持 "text/html"。
		         */
		        domParser = new  DOMParser();
		        xmlDoc = domParser.parseFromString(xmlString, 'text/xml');
		    }catch(e){
		    }
		}
		else{
		    return null;
		}

		return xmlDoc;
	},
	/**
	 * 添加导出表格判断，超过5000行不能导出
	 */
	exportLimit:function(selector){
		var exportLimit = 5000;
		var exportCount = $(selector).text().replace(/[^0-9]/ig,"");
		if(exportCount > exportLimit){
			jAlert('导出的数据不能超过5000条！');
			return false;
		}
		return true;
	}
};
$(function(){
	if(isIE6){
		var contentHeight = $('#content').height();
		$(".datatableStyle").height(contentHeight-70);
		$(".fullfit .treeview").height(contentHeight-46);
	}
	/*setContentWidth();
	$(window).resize(function(event) {
		setContentWidth();
	});*/
	//IE下textarea中maxLength的问题
	$(document).on('keyup','textarea[maxLength]',function(){
		 var area=$(this); 
		    var max=parseInt(area.attr("maxLength"),10); //获取maxlength的值 
		    if(max>0){ 
			    if(area.val().length>max){ //textarea的文本长度大于maxlength 
			        area.val(area.val().substr(0,max)); //截断textarea的文本重新赋值 
			    } 
			}
	});
	//复制的字符处理问题 
	$(document).on('blur','textarea[maxLength]',function(){
		var area=$(this); 
		area.val($.trim(area.val()));
		var max=parseInt(area.attr("maxLength"),10); //获取maxlength的值 
		if(max>0){ 
			if(area.val().length>max){ //textarea的文本长度大于maxlength 
				area.val(area.val().substr(0,max)); //截断textarea的文本重新赋值 
			} 
		} 
	});
	//替换日历中的css文件
	$(document).on('focus','input[hikui="calendar"]',function(){
		$($('#_my97DP iframe').prop('contentWindow').document).find("link").attr("href","/css/calendar.css");
	});
	
	$(document).keydown(function(event) {
		//获取事件对象  
        var elem = event.relatedTarget || event.srcElement || event.target ||event.currentTarget;   
          
        if(event.keyCode==8){//判断按键为backSpace键  
          
            //获取按键按下时光标做指向的element  
            var elem = event.srcElement || event.currentTarget;   
              
            //判断是否需要阻止按下键盘的事件默认传递  
            var name = elem.nodeName;  
              
            if(name!='INPUT' && name!='TEXTAREA'){  
                return _stopIt(event);  
            }  
            var type_e = elem.type.toUpperCase();  
            if(name=='INPUT' && (type_e!='TEXT' && type_e!='TEXTAREA' && type_e!='PASSWORD' && type_e!='FILE')){  
                    return _stopIt(event);  
            }  
            if(name=='INPUT' && (elem.readOnly==true || elem.disabled ==true)){  
                    return _stopIt(event);  
            }
        } 
	});
	
}); 
//hik csrf token 
$(function() {
	$('body').ajaxSend(function(e, request, options){
		var hik_token_name = $('meta[name=hik_token_name]').attr('content');
		var hik_token = $('meta[name=hik_token]').attr('content');
		if(hik_token_name&&hik_token){
			request.setRequestHeader("hik_token_name", hik_token_name);
	        request.setRequestHeader("hik_token", hik_token);
		}
	}).ajaxComplete(function(e, request, response) {
		try{
           	var hik_token_name = request.getResponseHeader('hik_token_name');
           	var hik_token = request.getResponseHeader('hik_token');
           	if(hik_token_name&&hik_token){
           		$('meta[name=hik_token_name]').attr('content',hik_token_name);
               	$('meta[name=hik_token]').attr('content',hik_token);
           	}
           	
       }catch(e){
              //后台没有设置responseHeader则不做处理
       }
	});
});

Date.prototype.Format = function(fmt)   
{ //author: meizz  
  var o = {   
    "M+" : this.getMonth()+1,                 //月份   
    "d+" : this.getDate(),                    //日   
    "h+" : this.getHours(),                   //小时   
    "m+" : this.getMinutes(),                 //分   
    "s+" : this.getSeconds(),                 //秒   
    "q+" : Math.floor((this.getMonth()+3)/3), //季度   
    "S"  : this.getMilliseconds()             //毫秒   
  };   
  if(/(y+)/.test(fmt))   
    fmt=fmt.replace(RegExp.$1, (this.getFullYear()+"").substr(4 - RegExp.$1.length));   
  for(var k in o)   
    if(new RegExp("("+ k +")").test(fmt))   
  fmt = fmt.replace(RegExp.$1, (RegExp.$1.length==1) ? (o[k]) : (("00"+ o[k]).substr((""+ o[k]).length)));   
  return fmt;   
}  
loadXML = function(xmlString){
    var xmlDoc=null;
    //判断浏览器的类型
    //支持IE浏览器 
    if(!window.DOMParser && window.ActiveXObject){   //window.DOMParser 判断是否是非ie浏览器
        var xmlDomVersions = ['MSXML.2.DOMDocument.6.0','MSXML.2.DOMDocument.3.0','Microsoft.XMLDOM'];
        for(var i=0;i<xmlDomVersions.length;i++){
            try{
                xmlDoc = new ActiveXObject(xmlDomVersions[i]);
                xmlDoc.async = false;
                xmlDoc.loadXML(xmlString); //loadXML方法载入xml字符串
                break;
            }catch(e){
            }
        }
    }
    //支持Mozilla浏览器
    else if(window.DOMParser && document.implementation && document.implementation.createDocument){
        try{
            /* DOMParser 对象解析 XML 文本并返回一个 XML Document 对象。
             * 要使用 DOMParser，使用不带参数的构造函数来实例化它，然后调用其 parseFromString() 方法
             * parseFromString(text, contentType) 参数text:要解析的 XML 标记 参数contentType文本的内容类型
             * 可能是 "text/xml" 、"application/xml" 或 "application/xhtml+xml" 中的一个。注意，不支持 "text/html"。
             */
            domParser = new  DOMParser();
            xmlDoc = domParser.parseFromString(xmlString, 'text/xml');
        }catch(e){
        }
    }
    else{
        return null;
    }

    return xmlDoc;
};

