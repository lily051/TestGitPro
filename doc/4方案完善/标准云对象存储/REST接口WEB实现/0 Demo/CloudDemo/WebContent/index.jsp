<%@ page language="java" contentType="text/html;charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
	<title>标准云对象存储接口测试</title>
	<link rel="stylesheet" type="text/css" href="js/webuploader/webuploader.css" />
	<link rel="stylesheet" type="text/css" href="css/cloud.css" />
</head>

<body>
	<h2>标准云对象存储接口测试</h2>
	<hr />
	<!-- 1、云存储信息 -->
	<div class="cloud-info">
		<h3>云存储信息</h3>
		<span>&nbsp;&nbsp;I&nbsp;P&nbsp;地址：</span><input type="text" id="ip" value="10.33.34.20" /><br />
		<span>文件端口：</span><input type="text" id="port" value="6201" /><br />
		<span>访问密钥：</span><input type="text" id="accesskey" value="7kA404e117n3Z32Do91vNlro7u4gvxeSS0k7g0U729x8970r2gCwEIV9FXA8xwC" /><br />
		<span>加密密钥：</span><input type="text" id="secretkey" value="dS6YJC8V0e486DU3728Xv3cUNe7A67dxC5G0a016Z73j8s62338c0263N2WEPK2" /><br />
	</div>
	<hr />
	<!-- 2、JAVA接口：获取Bucket数据 -->
	<h3>
		JAVA接口：<input type="button" value="获取Bucket数据" onclick="getBucketData()"/>
	</h3>
	<div id="bucketdata" class="bucketdata" style="height:80px;"></div>
	<hr />
	<!-- 3、REST接口：上传，JAVA接口：下载、删除 -->
	<h3>
		REST接口：<input type="button" value="上传" onclick="uploadFileToCloud()" /><input type="button" value="下载" onclick="downloadFile()" />&nbsp;&nbsp;
		JAVA接口：<input type="button" value="删除" onclick="deleteFile()" />
	</h3>
	<label style="float:left;line-height:20px">文件名称：</label>
	<input id="fileDisplayName" type="text" style="height:20px;float:left;margin-right:2px;" disabled="disabled">
	<div id="picker">选择</div><br />
	<hr />
	<!-- 4、文件信息 -->
	<h3>文件列表</h3>
	<div id="filedata" class="filedata"></div>
</body>
<script type="text/javascript" src="jQuery/jquery-1.7.1.js"></script>
<script type="text/javascript" src="js/webuploader/webuploader.js"></script>
<script type="text/javascript" src="js/cloud.js"></script>
</html>