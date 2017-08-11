<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>
<script src="/modules/device/channelMng/js/channelMng.js"></script>
<body>
	<div style="width:100%;height:110%;" id="previewVideo">
		<!-- <OBJECT ID="dialogPlayBackOcx" CLASSID="CLSID:07EBE387-935C-45DA-A121-AE19F18CEF44" width="100%" height="100%">
			<param name="lLayout" value="1" />
			<param name="lSkinType" value="0" />
			<param name="lpstrPath" value="c:\FMS8000\"/>
			<param name="lPicType" value="0" />
			<param name="lModuleStyle" value="255" />
			<param name="lBwControl" value="1" />
		</OBJECT> -->
	</div>
</body>

<script>
	var recordPojo = {
			camType:"${recordPojo.camType}", channelId:"${recordPojo.channelId}", camName:"${recordPojo.camName}",
			devType:"${recordPojo.devType}", regionId:"${recordPojo.regionId}", devIp:"${recordPojo.devIp}",		
			devPort:"${recordPojo.devPort}", user:"${recordPojo.user}", pwd:"${recordPojo.pwd}",	
			channel:"${recordPojo.channel}", realPlayType:"${recordPojo.realPlayType}"
	};
	
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
</script>
