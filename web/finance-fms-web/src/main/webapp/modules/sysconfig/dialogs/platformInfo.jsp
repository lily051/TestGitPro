<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@page language="java" import="java.util.Date;" %>
<%@ taglib prefix="s" uri="/struts-tags" %>
<div id="platformInfo">
	<form class="platformForm form-input form-horizontal form-fixed" style="margin-left:20px;margin-top:24px;" id="sysPicConfigForm" enctype="multipart/form-data" method="post">	
	    <fieldset class="noLegend">
		<div class="control-group">
			<label class="control-label textR width100"><em>*</em>平台描述文字：</label>
			<div class="controls">
					<input id="dscbstr" name="platformParamInfo.platformDiscribe" value="${platformParamInfo.platformDiscribe}" type="text" class="auto-input platform"  vtype="common" required="true" class='auto-input' data-icon="true" maxLength="50" style="width:300px"/>
			</div>
		</div>
		<div class="control-group">
			<label class="control-label textR width100"><em>*</em>版权描述文字：</label>
			<div class="controls">
				<input id="copyrtstr" name="platformParamInfo.platformCopyright" value="${platformParamInfo.platformCopyright}" type="text" class="auto-input platform" vtype="common" required="true" class='auto-input' data-icon="true" maxLength="50" style="width:300px"/>
			</div>
		</div>
		<div class="control-group">
			<label class="control-label textR width100">
				平台Logo：
			</label>
			<div class="controls">
	            <div>
	            	<label class="buttonS bUpload defaultBtn" id="uploadLogo"><span>上传</span>
	                	<input class="iUpload auto-input" name="navbarFile" type="file" value="" style="font-size: 13px;"/>
	                </label>
	            	<span class="imgDetail">（可上传最大文件大小为2MB，建议尺寸48*48）</span>
	            </div>
	            <div style="width:70px;height:58px;margin-top:5px;">
	            	<img src="${platformParamInfo.imgBar}" alt="没有" class="photo" id="navbarUrl" style="border: 1px solid #b1b1b1;width: 48px;height: 48px;margin-top: 7px;"/>
	            	<!-- /ivms-ui/extend/default/images/icon/${sysConfigInfo.imgSrc}  绝对路径  -->
	            </div>
			</div>
		</div>
		<div class="control-group">
			<label class="control-label textR width100">
				登录页Logo：
			</label>
			<div class="controls">
	            <div>
	            	<label class="buttonS bUpload defaultBtn" id="uploadLoginpageLogo"><span>上传</span>
	                	<input class="iUpload auto-input" name="logoFile" type="file" value="" style="font-size: 13px;"/>
	                </label>
	            	<span class="imgDetail">（可上传最大文件大小为2MB，建议尺寸48*48）</span>
	            </div>
	            <div style="width:70px;height:58px;margin-top:5px;">
	            	<img src="${platformParamInfo.imgLogo}" alt="没有" class="photo" id="logoUrl" style="border: 1px solid #b1b1b1;width: 48px;height: 48px;margin-top: 7px;"/>
	            </div>
			</div>
		</div>
		<div class="control-group">
			<label class="control-label textR width100">
				登录页图片：
			</label>
			<div class="controls">
	            <div>
	            	<label class="buttonS bUpload defaultBtn"><span>上传</span>
	                	<input class="iUpload auto-input" name="loginFile" type="file" value="" style="font-size: 13px;"/>
	                </label>
	            	<span class="imgDetail">（可上传最大文件大小为2MB，建议尺寸1920*460）</span>
	            </div>
	            <div style="width:340px;height:170px;margin-top:5px;">
	            	<img src="${platformParamInfo.imgLogin}" alt="没有" class="photo" id="loginUrl" style="border: 1px solid #b1b1b1;height:170px;width:710px;"/>
	            </div>
			</div>
		</div>
		<div class="sysBottom" style="margin-left:120px;_margin-left:110px;">
			<button type="button" class="sys-bPrimary" id="platformSave">应用</button>
			<button type="button" class="sys-bPrimary" id="platformPreview">预览</button>
			<button type="button" class="sys-cancel renewDefault">恢复默认</button>
		</div>
		</fieldset>
	</form>
</div>
<script>
	$(function(){
		$(".photo").each(function(index, el) {
			var src=$(el).attr('src');
			$(el).attr("src",src + "?t=" + Math.random());
		});
		FinanceUtil.initFormUI("#platformInfo");
		$(".renewDefault").on('click', function(event) {
			FMS.sysconfig.renewDefault();
		});
		//平台logo上传
		$("input[name=navbarFile]").change(function(){
			var i = $(this).val().lastIndexOf(".");
			var ext = $(this).val().substring(i).toLowerCase();
			FMS.sysconfig.uploadPic(ext, '#navbarUrl', '/modules/sysconfig/loginLogoFilePreview.action','navbarFile');//TODO
		});
		//登录页logo图片上传
		$("input[name=logoFile]").change(function(){
			var i = $(this).val().lastIndexOf(".");
			var ext = $(this).val().substring(i).toLowerCase();
			FMS.sysconfig.uploadPic(ext, '#logoUrl', '/modules/sysconfig/logoFilePreview.action','logoFile');//TODO
		});
		//登录页图片上传
		$("input[name=loginFile]").change(function(){
			var i = $(this).val().lastIndexOf(".");
			var ext=$(this).val().substring(i).toLowerCase();
			FMS.sysconfig.uploadPic(ext, '#loginUrl', '/modules/sysconfig/loginFilePreview.action','loginFile');//TODO
		});

		$("#platformSave").on('click', function(event) {
			FMS.sysconfig.platformApp();
		});

		$("#platformPreview").on('click', function(event) {
			FMS.sysconfig.platformPreview();
		});
		$(".bUpload").on('click', function(event) {
			
			//$(this).find('input').click();
		});
	});
</script>