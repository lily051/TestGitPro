<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<div>
<div class="msg-b msg-b-attention" style="padding-top: 8px;padding-bottom: 10px;margin-top: 0;">
    <div class="msg-cnt">
        <span>红色<em>*</em>为必填项</span>
    </div>
</div>
<form class="form-horizontal form-fixed formLabel140" enctype="multipart/form-data" method="post">
        <fieldset class="noLegend">
            <div class="control-group">
                <label class="control-label"><em>*</em>Excel文件：</label>
                <div class="controls">
                    <input type="file" name="proFile" class="staffFile" />  
                </div> 
            </div>
            <div class="control-group">
                <label class="control-label"></label>
                <div class="controls">
                    <a class="templDown"><span class="managerOpr oprDownTemp"><i>模板下载</i></span></a>
                </div> 
            </div>
        </fieldset>
</form>
</div>
<script>
	$(function(){
		$(".templDown").on("click",function(){
			$.ajax({
				url:'/modules/product/downLoadTemplate.action',//TODO 
				type:'post',
				dataType:'json',
				success:function(data){
					if(data&&data.success){
						window.location.href=data.data.url;
					}
				},
				error:function(xhr){
					
				}
			});
			
		});
	});
</script>