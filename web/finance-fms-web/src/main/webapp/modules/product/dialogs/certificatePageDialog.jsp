<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<style>
	.certName{
		display: inline-block;
		width:236px;
		text-overflow: ellipsis;
    	overflow: hidden;
    	white-space: nowrap;
    	margin-left:10px;
	}
	.controls span.notEdit{
		width:100%;
	}
	.icon{
		cursor: pointer;
	}
</style>
<s:iterator id="cert" value="certList">
	<div class="certificateInfo" data-id="${cert.id}" title="点击查看详情">
		<input type="hidden" id="certId" name="certId" value="${cert.id}">
		<div class="header">
			<label class="certName">${cert.strName}</label>
		</div>
		<div class="control-group">
			<label class="control-label">证书编号：</label>
			<div class="controls"><span class="notEdit">${cert.strCode}</span></div>
		</div>
		<div class="control-group">
			<label class="control-label">产品权限：</label>
			<div class="controls controls2">${cert.productNameList}</div>
		</div>
		<div class="certOverlay">
			<div>
				<span class="icon certEdit"></span> <span class="icon certDelete"></span>
			</div>
		</div>
	</div>
</s:iterator>

<script>
$(function(){
	$(".certificateInfo").each(function(index ,el){
		if(index%3==2){
			$(this).addClass("even");
		}
	});
	$(".certEdit").on('click', function(event) {
		FMS.globalFun.stopPropagation(event);
		$(".certificateInfo").removeClass('active');
		$(this).parents('.certificateInfo').addClass('active');
		var id = $(this).parents(".certificateInfo").data("id")
		FMS.certManage.editCertificate(id);
	});
	$(".certDelete").on('click', function(event) {
		FMS.globalFun.stopPropagation(event);
		$(".certificateInfo").removeClass('active');
		$(this).parents('.certificateInfo').addClass('active');
		var id = $(this).parents(".certificateInfo").data('id');
		jConfirm('确认要删除该证书吗？', {
			title : '提示',
			callback : function(value) {
				if (value) {
					$.ajax({
						url : '/modules/product/deleteCert.action', //TODO
						data : {
							'crtificateInfo.id' : id
						},
						success : function(data) {
							if(data && data.success){
								$.sticky('操作成功', {
									type: "ok",
									title: '操作成功',
									afterclose: function() { //'完成'
										
									}
								});
								$("#certSearch").click();
							}else{
								jAlert(data.msg,'错误','error')
							}
							
						}
					});
				}
			}
		});
	});
});
</script>