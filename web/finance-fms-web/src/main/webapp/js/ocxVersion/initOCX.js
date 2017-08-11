/**
 * 判断控件是否支持
 * @param  {[type]} id 控件object的id
 */
function judgeOcxAble(id){
	var ocx = document.getElementById(id);
	if(ocx!=null&&ocx.object!=null){
		return true;
	}else{
		//var parent = $("#"+id).parent("div");
		//parent.empty().append("123123")
		var html = '<DIV class="msg-b msg-b-stop"><I></I>'+
				   '<DIV class=msg-cnt><SPAN>控件异常！</SPAN>'+
				   '<P>1.请下载控件。<A href="/download/ActiveX.exe">下载</A></P>'+
				   '<P>2.下载完毕关闭浏览器 安装下载的控件。</P></DIV></DIV>';
		$("#"+id).replaceWith(html, function() {
			
		});
		return false;
	}
}
function configOcxDialog(id){
	var ocx = document.getElementById(id);
	if(ocx!=null&&ocx.object!=null){
		return true;
	}else{
		var html = '<DIV class="msg-b msg-b-stop"><I></I>'+
				   '<DIV class=msg-cnt><SPAN>控件异常！</SPAN>'+
				   '<P>1.请下载控件。<A href="/download/ActiveX.exe">下载</A></P>'+
				   '<P>2.下载完毕关闭浏览器 安装下载的控件。</P></DIV></DIV>';
		jAlert("ocx版本过低或浏览器不支持控件，请<A href='/download/ActiveX.exe'>下载</A>控件。");
		return false;
	}
}
/**
 * 初始化ocx插件
 * @param  {[type]} obj ocxType:0是预览，1是回放，2是配置
 * @return {[type]}     [description]
 */
var initOcx = function(obj){
	if(typeof obj != "undefined"){
		var ocxId  = obj.ocxId,
			divId = obj.divId,
			ocxType = obj.ocxType;
		if(ocxType == 0){
			var version = PreviewOCX,
				lLayoutMode = obj.lLayoutMode||0,
				osdEnable = modeParam.osdEnable;
			if(modeParam.usbAndIpc=="usb"){
				osdEnable = 0;
			}
			var ocxObject = '<object id='+ocxId+'  CLASSID="CLSID:04655FF8-9979-4FFA-8F77-C3A539EA7370" codeBase="#version='+version+'" width="100%" height="100%">'+
			'<param name="lLayoutMode" value='+lLayoutMode+' /><param name="lSkinType" value="0" /><param name="lBtnRights" value='+osdEnable+' /></object>';
			$("#"+divId).empty().append(ocxObject);
			judgeOcxAble(ocxId);
		}else if(ocxType == 1){
			var version = PlaybackOCX,
				modelType = obj.modelType,
				lModuleStyle = 1;
			if(modelType=="ipc"){
				lModuleStyle = 0;
			}else if(modelType=="usb"){
				lModuleStyle = 2;
			}
			var ocxObject = '<object id='+ocxId+'  CLASSID="CLSID:07EBE387-935C-45DA-A121-AE19F18CEF44" codeBase="#version='+version+'" width="100%" height="100%">'+
			'<param name="lLayout" value="1" /><param name="lSkinType" value="0" /><param name="lpstrPath" value='+Constants.record.picturePath+' />'+
			'<param name="lPicType" value="0" /><param name="lModuleStyle" value='+lModuleStyle+' /><param name="lBtnRights" value="11" /></object>';
			$("#"+divId).empty().append(ocxObject);
			judgeOcxAble(ocxId);
		}else if(ocxType == 2){
			var version = RPConfigOCX;
			var ocxObject = '<object id='+ocxId+' classid="clsid:A4660BF5-7356-4A3B-8A34-5859ED6C4F2F" codeBase="#version='+version+'" Width=1 Height=1></object>';
			$("#"+divId).append(ocxObject);
		}
	}
};	
	