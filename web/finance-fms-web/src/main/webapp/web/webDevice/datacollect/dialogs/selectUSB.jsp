<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<script src="${ctx}/baseui/js/plugins/ui/jquery.linkagesel.js"></script>
<style>
	.conditions select{
		width:198px !important;
	}
	.conditions .selector{
		width:190px !important;
	}
</style>
<div>
	<div class="height20"></div>
	<form class="form-horizontal form-fixed">
		<fieldset class="noLegend">
			<div class="control-group" id="usbDiv">
				<label class="control-label"><em>*</em>usb摄像头：</label>
				<div class="controls conditions">
					<select name="" id="usbCamInfo"></select>
					
				</div>
			</div>
			<div class="control-group">
				<label class="control-label"><em>*</em>usb麦克风：</label>
				<div class="controls conditions">
					<select name="" id="usbMicInfo"></select>
				</div>
			</div>
		</fieldset>
	</form>
</div>
<script>
	$(function(){
		if(judgeOcxAble("ocxRecord")){
			try{
				var usbXml = document.getElementById('ocxRecord').GetAllUSBCameraInfo(),
					usbInfo = FMS.globalFun.loadXML(usbXml),
					cameras = usbInfo.getElementsByTagName("Camera"),
					microPhones = usbInfo.getElementsByTagName("MicroPhone"),
					cameraInfo=[],microPhoneInfo=[];
					//cameras.children;所有子节点
					for(var i=0,length = cameras.length;i<length;i++){
						var camIndex = cameras[i].getElementsByTagName("CamIndex")[0].firstChild.nodeValue,
							camName = cameras[i].getElementsByTagName("CamName")[0].firstChild.nodeValue;
						cameraInfo.push({"camIndex":camIndex},{"camName":camName});
						var newOption = $('<option value='+camIndex+' data-type="0">'+camName+'</option>');
						$("#usbCamInfo").append(newOption);
					}
					for(var j=0,length = microPhones.length;j<length;j++){
						var micIndex = microPhones[j].getElementsByTagName("MicIndex")[0].firstChild.nodeValue,
							micName = microPhones[j].getElementsByTagName("MicName")[0].firstChild.nodeValue;
						microPhoneInfo.push({micIndex:micIndex},{micName:micName});
						var newOption = $('<option value='+micIndex+' data-type="1">'+micName+'</option>');
						$("#usbMicInfo").append(newOption);
					}
				//document.getElementById("ocxRecord").SetConfigData(FMS.datacollect.picturePath);
			}catch(e){
				
			}
		}
		
	});	
</script>