$(function() {
	uploaderInit();
	getFileList();
});

function uploaderInit() {
	uploader = WebUploader.create({
        swf:'js/webuploader/Uploader.swf',
        duplicate:true,
        pick:'#picker'
    });
    /**
     * 文件添加监听
     */
    uploader.on('fileQueued', function(file) {
		$('#fileDisplayName').val(file.name); // 回显文件名称
        uploader.md5File(file).progress(function(percentage) {
        }).then(function(val) { // 完成md5计算
        	file.md5 = val;
        	upload_file = file;
		});
    });
    /**
     * 文件上传成功监听
     */
    uploader.on('uploadSuccess', function(file, response) {
    	alert("文件上传成功", "提示", "ok");
    	$.ajax({
    		url:"cloud",
    		type:"post",
    		dataType:"json",
    		data:{
    	    	fuc:"uploadFile",
    	    	ip:$("#ip").val(),
		    	port:$("#port").val(),
		    	accesskey:$("#accesskey").val(),
		    	secretkey:$("#secretkey").val(),
		    	bucket:$('input[name="bucket"]:checked').val(),
    	    	fileName:file.md5,
    	    	fileDisplayName:file.name
    	    },
    		success:function(data) {
    			getFileList();
    		}
    	});
    });
    /**
     * 文件上传失败监听
     */
    uploader.on('uploadError', function(file, reason) {
    	alert("文件上传失败", "提示", "error");
    });
}

/**
 * 获取文件列表
 * @auhtor gengqiaolei
 */
function getFileList() {
	$.ajax({
		url:"cloud",
		type:"post",
		dataType:"json",
		data:{
	    	fuc:"getFileList"
	    },
		success:function(data) {
			$("#filedata").html(data.fileList);
			$('input[name="file"]:eq(0)').attr("checked", "checked"); 
		}
	});
}

/**
 * 获取Bucket数据
 * @author gengqiaolei
 */
function getBucketData() {
	var ip = $("#ip").val();
	var port = $("#port").val();
	var accesskey = $("#accesskey").val();
	var secretkey = $("#secretkey").val();
	if(ip == '' || port == '' || accesskey == '' || secretkey == '') {
		alert("请先填写云存储信息");
	} else {
		$.ajax({
			url:"cloud",
			type:"post",
			dataType:"json",
			data:{
		    	fuc:"getBucketData",
		    	ip:ip,
		    	port:port,
		    	accesskey:accesskey,
		    	secretkey:secretkey
		    },
			success:function(data) {
				$("#bucketdata").html(data.msg);
				$('input[name="bucket"]:eq(0)').attr("checked", "checked"); 
			}
		});
	}
}

/**
 * 上传文件至云存储
 * @author gengqiaolei
 */
var uploader;
var upload_file;
function uploadFileToCloud() {
	var ip = $("#ip").val();
	var port = $("#port").val();
	var accesskey = $("#accesskey").val();
	var secretkey = $("#secretkey").val();
	var bucket = $('input[name="bucket"]:checked').val();
	if(ip == '' || port == '' || accesskey == '' || secretkey == '') {
		alert("请先填写云存储信息");
	} else if(bucket == undefined) {
		alert("请先获取Bucket数据！");
	} else if($("#fileDisplayName").val() == "") {
		alert("请先选择文件！");
	} else {
	 	$.ajax({
			url:"cloud",
			type:"post",
			dataType:"json",
			data:{
		    	fuc:"getUploadInfo",
		    	ip:ip,
		    	port:port,
		    	accesskey:accesskey,
		    	secretkey:secretkey,
		    	bucket:bucket,
		    	fileName:upload_file.md5,
		    	fileDisplayName:encodeURI(upload_file.name)
		    },
			success:function(data) {
				if(!data.success) {
	    			alert(data.msg);
	    		} else {
					uploader.options.formData = {"returnType":'json'};
					var url = "http://" + data.bestnode + "/" + data.bucket;
					upload_file.AccessKey = data.accessKey;
					upload_file.Signature = data.Signature;
					upload_file.Date = data.date;
					upload_file.ServerUrl = url;
			    	uploader.upload(upload_file);
	    		}
			}
		});
	}
}

/**
 * 下载文件
 * @author gengqiaolei
 */
function downloadFile() {
	var file = $('input[name="file"]:checked').val();
	var bucket = $('input[name="file"]:checked').attr("bucket");
	if(file == undefined || bucket == undefined) {
		alert("请先选择文件记录！");
	} else {
		$.ajax({
			url:"cloud",
			type:"post",
			dataType:"json",
			async:false,
			data:{
		    	fuc:"getDownloadInfo",
		    	bucket:bucket,
		    	fileName:file
		    },
			success:function(data) {
				if(!data.success) {
	    			alert(data.msg);
	    		} else {
	    			var obj = {filename:data.filename, token:data.token};
					openPostWindow(data.url, "_blank", obj);
	    		}
			}
		});
	}
}

/**
 * 删除文件
 * @author gengqiaolei
 */
function deleteFile() {
	var file = $('input[name="file"]:checked').val();
	var bucket = $('input[name="file"]:checked').attr("bucket");
	if(file == undefined || bucket == undefined) {
		alert("请先选择文件记录！");
	} else {
		$.ajax({
			url:"cloud",
			type:"post",
			dataType:"json",
			data:{
		    	fuc:"deleteFile",
		    	fileName:file,
		    	bucket:bucket
		    },
			success:function(data) {
				if(!data.success) {
	    			alert(data.msg);
	    		} else {
	    			alert("删除成功！");
	    			getFileList();
	    		}
			}
		});
	}
}

/**
 * 打开窗口
 * @author gengqiaolei
 */
function openPostWindow(url, target, obj) {
	var tempForm = document.createElement("form");
	tempForm.id = "downloadForm" + Math.random();
	tempForm.name = "downloadForm" + Math.random();
	tempForm.method = "get";
	tempForm.action = url;
	tempForm.target = target;
	for(var p in obj) {
		var hideInput = document.createElement("input");
		hideInput.type = "hidden";
		hideInput.name = p;
		hideInput.value = obj[p];
		tempForm.appendChild(hideInput);
	}
	if(window.addEventListener) {
		tempForm.addEventListener("onsubmit", function(){openWindow(name);});
	} else if(window.attachEvent) {
		tempForm.attachEvent("onsubmit", function(){openWindow(name);});
	}
	document.body.appendChild(tempForm);
	tempForm.submit();
	document.body.removeChild(tempForm);
}