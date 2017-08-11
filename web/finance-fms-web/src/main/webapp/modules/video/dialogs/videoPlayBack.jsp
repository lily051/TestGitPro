<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>
<script src="/js/ocxVersion/initOCX.js"></script>
<style>
	.popMain .form-horizontal .controls {
	    height: 32px;
	    margin-left:130px;
	}
	.popMain .form-horizontal .noLegend .control-group{
		padding-left:30px;
		margin-bottom: 13px;
	}
	.popMain .check{
		margin-right:6px;
	}
	.popMain .control-label span{
		display: inline-block;
		text-align: left;
		width:180px;
	}
</style>
<div class="popMain">
	<div class="popleft">
		<div class="formLeft">
			<div class="height20"></div>
			<form class="form-horizontal form-fixed" id="videoCheck">
				<div class="legendTitle"><span class="titleLine"></span>视频信息</div>
				<fieldset class="noLegend">
					<div class="control-group">
						<span>
							<label>关联业务单号：</label><label class="recordInfo" title="${recordDto.recordInfo.nproId}">${recordDto.recordInfo.nproId}</label>
						</span>
					</div>
					<div class="control-group">
							<span>
								<label>业务员工号：</label><label class="recordInfo" title="${recordDto.recordInfo.proUserNo}">${recordDto.recordInfo.proUserNo}</label>
							</span>
					</div>
					<div class="control-group">
							<span>
								<label>业务员姓名：</label><label class="recordInfo" id="proName" title="${recordDto.recordInfo.proName}">${recordDto.recordInfo.proName}</label>
							</span>
					</div>
					<div class="control-group">
							<span>
								<label>产品编码：</label><label class="recordInfo" title="${recordDto.recordInfo.productCode}">${recordDto.recordInfo.productCode}</label>
							</span>
					</div>
					<div class="control-group">
							<span>
								<label>产品名称：</label><label class="recordInfo" title="${recordDto.recordInfo.productName}">${recordDto.recordInfo.productName}</label>
							</span>
					</div>
					<div class="control-group">
							<span>
								<label>客户姓名：</label><label class="recordInfo" title="${recordDto.recordInfo.name}">${recordDto.recordInfo.name}</label>
							</span>
					</div>
					<div class="control-group">
							<span>
								<label>购买金额：</label>
								<label class="recordInfo" title="${recordDto.recordInfo.strBuyMoney}">${recordDto.recordInfo.strBuyMoney}<s:if test='recordDto.recordInfo.strBuyMoney !=""'>元</s:if></label>
							</span>
					</div>
				</fieldset>
				<input type="hidden" id="selectCheck" value="${recordDto.recordInfo.recordCheck}">
				<input type="hidden" name="recordInfo.videoId" value="${recordDto.recordInfo.videoId}">
				
				<div class="legendTitle notcomplain" style="margin-top:5px"><span class="titleLine"></span>录像合规状态</div>
				<div class="legendTitle complain" style="margin-top:5px"><span class="titleLine"></span>录像处理意见</div>
				<div class="control-group notcomplain">
					<label style="margin-left:30px;margin-right:50px;"><span class="check"><input type="radio" value="2" name="recordInfo.recordCheck" <s:if test="recordDto.recordInfo.recordCheck==2">checked</s:if>></span><span>合规</span></label>
					<label><span class="check"><input type="radio" value="1" name="recordInfo.recordCheck" <s:if test="recordDto.recordInfo.recordCheck==1">checked</s:if>></span><span>不合规</span></label>
				</div>
				
				<div class="legendTitle notcomplain" style="margin-top:5px"><span class="titleLine"></span>业务单审批状态</div>
				<div class="control-group notcomplain" id = "lockClient">
					<label style="margin-left:30px;margin-right:25px;"><span class="check"><input id = "lockClientPass" type="radio" value="1" name="recordInfo.lockClient" <s:if test="recordDto.recordInfo.lockClient==1">checked</s:if>></span><span>审批通过</span></label>
					<label><span class="check"><input id = "lockClientSendBack" type="radio" value="2" name="recordInfo.lockClient" <s:if test="recordDto.recordInfo.lockClient==2">checked</s:if>></span><span>审批退回</span></label>
				</div>
				<div id = "unableApprovalNote" class="legendTitle notcomplain" style="color:#F00">&nbsp;含有上传失败的录像，故无法审批。
				</div>
				<input type="hidden" id = "lockClientAnother" name = "recordInfo.lockClient" value = "${recordDto.recordInfo.lockClient}" />
				
				<div class="control-group complain">
					<label style="margin-left:30px;"><span class="check"><input type="radio" value="3" name="recordInfo.recordCheck" <s:if test="recordDto.recordInfo.recordCheck==3">checked</s:if>></span><span>存在投诉</span></label>
					<label><span class="check"><input type="radio" value="4" name="recordInfo.recordCheck" <s:if test="recordDto.recordInfo.recordCheck==4">checked</s:if>></span><span>投诉关闭</span></label>
				</div>
				<div class="legendTitle" style="margin-top:10px"><span class="titleLine" style="background:transparent"></span>备注：</div>
				<div class="control-group" style="padding-left:30px;">
					<textarea name="recordInfo.checkRemark" id="videoTextarea" type="text" vtype="common" not-validator-paste=false maxLength="200" isoverflown="true" class="videoTextarea auto-input">${recordDto.recordInfo.checkRemark}</textarea>
				</div>
				<div class="videoSubmit">
					<span class="btn btn-submit blockBtn" id="videoSubmit">提交</span>
				</div>
			</form>
		</div>
	</div>
	<div class="popRight">
		<div class="height30 notcomplain"><span style="margin-left:20px;font-weight:bold;">合规检查：</span><span name="checkText">
		</span><input type="hidden" value="" id=""/></div>
		<div class="height30 complain"><span style="margin-left:20px;font-weight:bold;">投诉状态：</span><span name="checkText">
		</span><input type="hidden" value="" id=""/></div>
		<div class="videoPlay" id="videoPlay">
			<!-- <OBJECT ID="PlayBack" CLASSID="CLSID:07EBE387-935C-45DA-A121-AE19F18CEF44" width="100%" height="590" style="float:left">
				<param name="lLayout" value="1" />
				<param name="lSkinType" value="0" />
				<param name="lpstrPath" value="c:\FMS8000\" />
				<param name="lPicType" value="0" />
				<param name="lModuleStyle" value="255" />
				<param name="lBwControl" value="1" />
			</OBJECT>  -->
		</div>			
	</div>
	<!-- <div class="closeInfo" id="closeInfo" data-toogle="close">
		<span></span>
	</div> -->
</div>
<script type="text/javascript">
	$(function(){
		if(isIE6){
			$(".popRight").width($(".popRight").parent().width()-$(".popleft").width());
		}
		initOcx({ocxId:'ocxPlayBack',divId:'videoPlay',ocxType:1});
		$("object#ocxPlayBack").attr("height",590).css("float",'left');
		/*$("#closeInfo").on('click', function(event) {
			var _this = $(this);
			var toogle = _this.data("toogle");
			if(toogle == "close"){
				$(".formLeft").animate({"width":0});
				$(".popleft").animate({"margin-left":-220});
				_this.data("toogle","open").addClass("openInfo").animate({"left":0});
				$(".popRight").animate({"left":0});
			}else{
				$(".formLeft").animate({"width":233});
				$(".popleft").animate({"margin-left":0});
				_this.data("toogle","close").removeClass("openInfo").animate({"left":220});
				$(".popRight").animate({"left":223});
			}
		
		});*/
		FMS.video.viewOnly();
		$("#videoSubmit").on('click', function(event) {
			FMS.video.videoCheck();
		});
		
		/**针对录像合规检查，业务单审批。  业务单包含上传失败的录像，则无法审批。 录像合规，默认选中审批通过；录像不合规，默认选中审批退回。 */
		var isCanApproval = ${recordDto.recordInfo.isCanApproval}; // 业务单是否能够审批  0不能审批    1能够审批  
		if (isCanApproval == 0) { // 不能审批
			$("#lockClient").hide();
			$("#lockClientAnother").show();
			$("#unableApprovalNote").show();
		} else { // 能够审批	
			$("#lockClient").show();
			$("#lockClientAnother").remove();
			$("#unableApprovalNote").hide();
			
			$("input[type='radio']").on('click',function(){
				if ($(this).attr("name") == "recordInfo.recordCheck") {
					if ($(this).val() == 1) { // 录像不合规，默认选中审批退回
						$("input[name='recordInfo.lockClient']").eq(1).attr("checked","checked");
			            $("input[name='recordInfo.lockClient']").eq(0).removeAttr("checked");
			            $("input[name='recordInfo.lockClient']").eq(1).click();
					} else { // 录像合规，默认选中审批通过
						$("input[name='recordInfo.lockClient']").eq(0).attr("checked","checked");
			            $("input[name='recordInfo.lockClient']").eq(1).removeAttr("checked");
			            $("input[name='recordInfo.lockClient']").eq(0).click();
					}
				}
			});
		}
	});
	
	cvrplay();
	function cvrplay()
	{
		var playbackInfo={ip:"${recordDto.playbackInfo.ip}",playport:"${recordDto.playbackInfo.port}",user:"${recordDto.playbackInfo.user}",password:"${recordDto.playbackInfo.password}",type:"${recordDto.playbackInfo.type}",index:33000000511310983706,channelName:"${recordDto.playbackInfo.channelName}",strAccessKey:"${recordDto.playbackInfo.strAccessKey}",strSecretKey:"${recordDto.playbackInfo.strSecretKey}"},
			recordInfo = {bucket:"${recordDto.recordInfo.strBucket}",streamNo:"${recordDto.recordInfo.streamNo}",startTime:'<fmt:formatDate value="${recordDto.recordInfo.startTime}" pattern="yyyy-MM-dd HH:mm:ss"/>',endTime:'<fmt:formatDate value="${recordDto.recordInfo.endTime}" pattern="yyyy-MM-dd HH:mm:ss"/>',kmsInfo:"${recordDto.recordInfo.rtspPath}",fileName:"${recordDto.recordInfo.fileName}",fileSize:"${recordDto.recordInfo.nFileSize}",channel:"${recordDto.recordInfo.channel}"},xml;
		/**
		 * [if description]
		 * @param  {[type]} playbackInfo.type 1是cvr, 2是云存储, 3是kms, 4是云存储-对象存储,  0是无中心存储（DVR/NVR）
		 * @return {[type]}                   [description]
		 */
		if(playbackInfo.type==1){
			xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><Param><RecordType>255</RecordType><SearchType>0</SearchType><WndNo>0</WndNo><PlayType>"+playbackInfo.type+"</PlayType><ManType>0</ManType><DvrType>100</DvrType><Right>255</Right><LoginInfo><IP>"+playbackInfo.ip+"</IP><Port>"+playbackInfo.playport+"</Port><User>"+playbackInfo.user+"</User><Password>"+playbackInfo.password+"</Password><Rev>999</Rev></LoginInfo><PlayInfo><CamID>0</CamID><CamName>"+playbackInfo.channelName+"</CamName><StreamID>"+recordInfo.streamNo+"</StreamID><Channel>1</Channel><StartTime>"+recordInfo.startTime+"</StartTime><StopTime>"+recordInfo.endTime+"</StopTime><Rev>999</Rev><lBwControl>"+modeParam.recordBandwidth+"</lBwControl></PlayInfo></Param>";
		}else if(playbackInfo.type==2){
			xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><Param><RecordType>255</RecordType><SearchType>0</SearchType><WndNo>0</WndNo><PlayType>"+playbackInfo.type+"</PlayType><ManType>0</ManType><DvrType>100</DvrType><Right>255</Right><LoginInfo><IP>"+playbackInfo.ip+"</IP><Port>"+playbackInfo.playport+"</Port><User>"+playbackInfo.user+"</User><Password>"+playbackInfo.password+"</Password><Index>"+recordInfo.streamNo+"</Index><Rev>999</Rev></LoginInfo><PlayInfo><CamID>0</CamID><CamName>"+playbackInfo.channelName+"</CamName><Channel>0</Channel><StartTime>"+recordInfo.startTime+"</StartTime><StopTime>"+recordInfo.endTime+"</StopTime><Rev>999</Rev><lBwControl>0</lBwControl></PlayInfo></Param>";
		}else if(playbackInfo.type==3){
			xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><Param><RecordType>255</RecordType><SearchType>1</SearchType><WndNo>0</WndNo><PlayType>"+playbackInfo.type+"</PlayType><ManType>0</ManType><DvrType>100</DvrType><Right>255</Right><LoginInfo><IP>"+playbackInfo.ip+"</IP><Port>"+playbackInfo.playport+"</Port><User>"+playbackInfo.user+"</User><Password>"+playbackInfo.password+"</Password><Rev>"+recordInfo.kmsInfo+"</Rev></LoginInfo><PlayInfo><CamID>0</CamID><CamName>"+playbackInfo.channelName+"</CamName><Channel>0</Channel><StartTime>"+recordInfo.startTime+"</StartTime><StopTime>"+recordInfo.endTime+"</StopTime><FileName>"+recordInfo.fileName+"</FileName><FileSize>"+recordInfo.fileSize+"</FileSize><FileType>255</FileType><Rev>999</Rev><lBwControl>0</lBwControl></PlayInfo></Param>";
		}else if(playbackInfo.type==4){
			xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><Param><RecordType>255</RecordType><SearchType>0</SearchType><WndNo>0</WndNo><PlayType>"+playbackInfo.type+"</PlayType><ManType>0</ManType><DvrType>100</DvrType><Right>255</Right><LoginInfo><IP>"+playbackInfo.ip+"</IP><Port>"+playbackInfo.playport+"</Port><AccessKey>"+playbackInfo.strAccessKey+"</AccessKey><SecretKey>"+playbackInfo.strSecretKey+"</SecretKey><Bucket>"+recordInfo.bucket+"</Bucket><ObjectKey>"+recordInfo.streamNo+"</ObjectKey><Rev>999</Rev></LoginInfo><PlayInfo><CamID>1</CamID><CamName>"+playbackInfo.channelName+"</CamName><Channel>0</Channel><StartTime>"+recordInfo.startTime+"</StartTime><StopTime>"+recordInfo.endTime+"</StopTime><Rev>999</Rev><lBwControl>0</lBwControl></PlayInfo></Param>";
		}else if (playbackInfo.type == 0) { // DVR/NVR
			xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><Param><RecordType>255</RecordType><SearchType>0</SearchType><WndNo>0</WndNo><PlayType>0</PlayType><ManType>0</ManType><DvrType>100</DvrType><Right>255</Right><LoginInfo><IP>"+playbackInfo.ip+"</IP><Port>"+playbackInfo.playport+"</Port><User>"+playbackInfo.user+"</User><Password>"+playbackInfo.password+"</Password><Rev>999</Rev></LoginInfo><PlayInfo><CamID>1001</CamID><CamName>"+playbackInfo.channelName+"</CamName><Channel>"+recordInfo.channel+"</Channel><StartTime>"+recordInfo.startTime+"</StartTime><StopTime>"+recordInfo.endTime+"</StopTime><Rev>999</Rev><lBwControl>"+modeParam.recordBandwidth+"</lBwControl></PlayInfo></Param>";
		}
		if(judgeOcxAble("ocxPlayBack")){
			try{
				document.getElementById("ocxPlayBack").PB_Playback(xml);
			}catch(e){
				//jAlert("该浏览器不支持ocx控件！请换IE浏览器!",'提示');
			}
		}
	}
</script>