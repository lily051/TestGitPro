<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>
<html lang="zh">
<head>
    <meta charset="utf-8">
    <script type="text/javascript" src="${ctx}/baseui/js/language/messages_zh_cn.js"></script>
    <script src="${ctx}/baseui/js/plugins/jquery.js"></script>
	<script src="${ctx}/baseui/js/plugins/bootstrap.js"></script>
	<script src="${ctx}/baseui/js/plugins/underscore.js"></script>
	<script src="${ctx}/baseui/js/plugins/jquery-ui.js"></script>
    <script src="${ctx}/baseui/js/plugins/base.js"></script>
    <script src="${ctx}/baseui/js/plugins/sea.js"></script>
    <script src="${ctx}/baseui/js/plugins/ui/jquery.sticky.js"></script>
    <script src="${ctx}/baseui/js/plugins/popup/pop.js"></script>
    <link href="${ctx}/baseui/js/plugins/popup/skin/default/pop.css" rel="stylesheet"/>
</head>
<style type="text/css" media="print">  
.noprint{display : none } 
td{
	font-size: 24px;
	_font-size:18px;
	height:60px;
	_height:20px;
}
div#content{
    _width:auto;
}
</style>  
<style type="text/css">
#content{
	width:1024px;
	margin:0 auto;
	font-size: 20px;
	_font-size:18px;
}
h1{
	text-align: center;
}
hr{
	border:solid 2px black;
	margin-bottom:10px;
}
table{
	width:98%;
	margin:0 auto;
	border:solid 1px black;
	border-right:0;
	border-bottom:0;
	table-layout:fixed;
}
td{
	padding:10px 8px;
	border-bottom:solid 1px black;
	border-right:solid 1px black;
}
td :focus{
	padding:10px 8px !important;
}
img{
	width:100%;
	height:360px;
	_height:200px; 
}
</style>
<body>
		<div id="content" >
			<h1>理财业务音视频存单</h1><!-- printOrderInfo -->
			<hr />
			<table cellspacing="0" cellpadding="0">
				<tr>
					<td width="20%"><b>客户姓名</b>&nbsp;</td>
					<td width="30%">${printOrderInfo.name }&nbsp;</td>
					<td width="20%"><b>${printOrderInfo.creditname }</b>&nbsp;</td><!-- 证件名 -->
					<td width="30%">${printOrderInfo.creditCode }&nbsp;</td><!-- 证件号 -->
				</tr>
				<tr>
					<td><b>业务单号</b>&nbsp;</td>
					<td colspan="3">${printOrderInfo.nproId }&nbsp;</td>
				</tr>
				<tr>
					<td><b>办理时间</b>&nbsp;</td>
					<td colspan="3"><fmt:formatDate value="${printOrderInfo.businessTime }" pattern="yyyy-MM-dd HH:mm:ss"/>&nbsp;</td>
				</tr>
				<tr>
					<td><b>发行机构</b>&nbsp;</td>
					<td colspan="3">${printOrderInfo.strGrade }&nbsp;</td>
				</tr>
				<tr>
					<td><b>产品类型</b>&nbsp;</td>
					<td>${printOrderInfo.productType }&nbsp;</td>
					<td><b>产品名称</b>&nbsp;</td>
					<td>${printOrderInfo.productName }&nbsp;</td>
				</tr>
				<tr>
					<td><b>业务员姓名</b>&nbsp;</td>
					<td>${printOrderInfo.proName }&nbsp;</td>
					<td><b>业务员编号</b>&nbsp;</td>
					<td>${printOrderInfo.proUserNo }&nbsp;</td>
				</tr>
				<tr>
					<input type="file" id="btn_file" style="display:none">
					<td colspan="2" width="50%"><img id="imgStart" onerror="this.src='/images/nopic.png'" onclick="fileOpenDialogStart()" onmouseover="mouseOverStart()"></td>
					<td colspan="2" width="50%"><img id="imgEnd" onerror="this.src='/images/nopic.png'" onclick="fileOpenDialogEnd()" onmouseover="mouseOverEnd()"></td>
				</tr>
				<tr>
					<td colspan="4"><b>注意事项：</b>&nbsp;
				</tr>
				<tr>
					<td colspan="4">
						<p>1.此业务单的录音录像过程为双方自愿行为，本存单将由理财产品售出方存档；</p>
						<p>2.此存单代表业务人员已对购买方宣读购买须知，并将整个过程进行音视频录像；</p>
						<p>3.此存单双方签字后生效，生效后双方才可进行理财产品购买交易流程</p>
					</td>
				</tr>
				<tr>
					<td colspan="2"><b>客户签字：</b></td>
					<td colspan="2"><b>业务员签字：</b></td>
				</tr>
			</table>
		</div>
		<script LANGUAGE="JavaScript" charset="UTF-8">
		document.getElementById('imgStart').src="${printOrderInfo.pictureStrart }";
		document.getElementById('imgEnd').src="${printOrderInfo.pictureEnd }";
		window.print();
		
		var picturePath = "";
		function fileOpenDialogStart() {
			picturePath = document.getElementById('imgStart').src;
			document.getElementById("btn_file").click();
			picturePath = document.getElementById('btn_file').value;
			id = ${printOrderInfo.prosaverecordId};
			if (picturePath != "") {
				var fileSize = getPictureSize(document.getElementById('btn_file'));
				
				if (fileSize > 2 * 1024 * 1024){
					alert("图片大小不能超过2M");  
					return;
				}
				if (!/\.(jpeg|JPEG|jpg|png|JPG|PNG)$/.test(picturePath)) {  
					alert("图片类型必须是*.jpeg,*.jpg,*.png中的一种");   
		        } else {
					$.ajax({
			        	url: '/webdevice/contact/updateProsaverecord.action',
			        	type:'post',
			            data: {
			            	'printOrderInfo.prosaverecordId': id,
			                'printOrderInfo.pictureStrart':encodeURI(picturePath)
			            },
			            datatype: 'ajax',
			            success: function(data) {
							if (data && data.success) {
								$.sticky('操作成功,请稍等...', {
									type: "ok",
									title: '操作成 功,请稍等...',
									afterclose: function() { //'完成'
										window.location.reload();
									}
								});
							} else if(data && data.msg) {
								jAlert(data.msg, '错误', 'error');
							}
						}
			        });	
		        }
			}
			
		}
		function fileOpenDialogEnd() {
			picturePath = document.getElementById('imgEnd').src;
			document.getElementById("btn_file").click();
			picturePath = "," + document.getElementById('btn_file').value;
			id = ${printOrderInfo.prosaverecordId};
			if (picturePath != ",") {
				var fileSize = getPictureSize(document.getElementById('btn_file'));
				
				if (fileSize > 2 * 1024 * 1024){
					alert("图片大小不能超过2M");  
					return;
				}
				if (!/\.(jpeg|JPEG|jpg|png|JPG|PNG)$/.test(picturePath)) {  
					alert("图片类型必须是*.jpeg,*.jpg,*.png中的一种");   
		        }  else {
					$.ajax({
			            url: '/webdevice/contact/updateProsaverecord.action',
			            type:'post',
			            data: {
			            	'printOrderInfo.prosaverecordId': id,
							'printOrderInfo.pictureEnd':encodeURI(picturePath)
						},
						datatype: 'ajax',
						success: function(data) {
							if (data && data.success) {
								$.sticky('操作成功,请稍等...', {
									type: "ok",
									title: '操作成功,请稍等...',
									afterclose: function() { //'完成'
										window.location.reload();
									}
								});
							} else if(data && data.msg) {
								jAlert(data.msg, '错误', 'error');
						}
						}
			        });
		        } 
			}
		}

		/* function checkImgType() {
			picturePath = "," + document.getElementById('btn_file').value;
			if(picturePath=="") { 
				alert("请选择文件"); 
				return false; 
			} 
			var strTemp = file.split("."); 
			var strCheck = strTemp[strTemp.length-1]; 
			if(strCheck.toUpperCase()=='JPG') { 
				return true; 
			} else { 
				alert('上传文件类型不对！'); 
				return false; 
			} 
		} */
		
		
		function mouseOverStart() {    
			picturePath = document.getElementById('imgStart').src;
			if (picturePath.indexOf("nopic") > 0) {
				//alert("检测到左侧图片不存在，请点击左侧图片，进行手动选择图片操作");
				$("#imgStart").attr("title","检测到左侧图片不存在，请点击左侧图片框，进行手动选择图片操作。");
			}
	    }  
		function mouseOverEnd() {    
			picturePath = document.getElementById('imgEnd').src;
			if (picturePath.indexOf("nopic") > 0) {
				//alert("检测到右侧图片不存在，请点击右侧图片，进行手动选择图片操作");
				$("#imgEnd").attr("title","检测到右侧图片不存在，请点击右侧图片框，进行手动选择图片操作。");
			}
	    } 
		
		//判断图片大小
		function getPictureSize(obj){
		  var fileSize = 0;
		  var isIE = /msie/i.test(navigator.userAgent) && !window.opera;      
		  if (isIE && !obj.files) {     
		     var filePath = obj.value;      
		     var fileSystem = new ActiveXObject("Scripting.FileSystemObject");  
		     var file = fileSystem.GetFile (filePath);        
		     fileSize = file.Size;     
		  }else { 
		     fileSize = obj.files[0].size;   
		  } 
		  return fileSize;
		}

	</script>
</body>
</html>




