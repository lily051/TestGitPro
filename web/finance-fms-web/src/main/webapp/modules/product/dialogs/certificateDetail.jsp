<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<style>
.state{
	margin-left:0;
}
#certificateDetail .control-label{
	width:85px;
}
#certificateDetail .controls{
	margin-left:95px;
	_margin-left:0px;

}
/* .ztree li span.button.chk.checkbox_false_part,.ztree li span.button.chk.checkbox_false_part_focus,.ztree li span.button.chk.checkbox_true_full,.ztree li span.button.chk.checkbox_true_full_focus{
	background: url("/images/icons/product_checked.png");
}
.ztree li span.button.chk.checkbox_false_full,.ztree li span.button.chk.checkbox_false_full_focus{
	background: url("/images/icons/product_unchecked.png");
} */
/* .ztree li span.button.chk.checkbox_false_part,.ztree li span.button.chk.checkbox_false_part_focus,.ztree li span.button.chk.checkbox_true_full,.ztree li span.button.chk.checkbox_true_full_focus{
	background-image: url("/images/collect/checkbox_true.png") !important;
	background-repeat: no-repeat; 
	background-position: 0px 0px;
}
.ztree li span.button.chk.checkbox_false_full,.ztree li span.button.chk.checkbox_false_full_focus{
	background-image: url("/images/collect/checkbox_false.png") !important;
	background-repeat: no-repeat; 
	background-position: 0px 0px;
}  */
</style>
<div style="background:#F6F6F6;">
	<form class="form-horizontal form-fixed product-certf">
		<fieldset class="noLegend">
		    <div class="control-group">
		        <label class="control-label">证书编码：</label>
		        <div class="controls"><span class="notEdit" id="certCode" title="${crtificateInfo.strCode}" style="width:337px;">${crtificateInfo.strCode}</span></div>
		    </div>
		</fieldset>
		<fieldset class="noLegend pull-left allField">
		    <div class="control-group">
		        <label class="control-label">产品权限：</label>
		    </div>
		    <div class="control-group">
		    	<input type="hidden" id="crtificateInfoId" value="${crtificateInfo.id}"><!-- 放证书id -->
		    	<div class="controls" style="margin-left:24px;overflow:hidden;">
		    	   <div class="treeview detail-certTree">
		    	       <ul id="productCertTree" class="ztree"></ul>
		    	   </div>
		    	</div>
		    </div>
		</fieldset>
		
	</form>
</div>

<script>
	$(function(){
		FMS.product.initProductTree('detail');
	});
</script>