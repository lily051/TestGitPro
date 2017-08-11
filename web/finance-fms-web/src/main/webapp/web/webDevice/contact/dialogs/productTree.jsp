<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>

<div class="iScroll">
	<div class="treeview <s:if test="flag==1">hidden</s:if>" style="padding-top:10px;width:400px">
	    <ul id="productTree" class="ztree"></ul>
	</div>
	<div class="wordTemp <s:if test="flag==0">hidden</s:if>">
		<textarea id="wordTemp" style="width:448px;height:280px;" readonly="readonly">${tip}</textarea>
	</div>
</div>

<script>
	$(function(){
		var flag="${flag}";
		if(flag==0){
			FMS.dataBusiness.initProductTree();
		}else{
			$("button.dosure").hide();
			$(".selectOther").show();
		}
		$("#wordTemp").keydown(function(event) {
			if(event.keyCode==8){
				FMS.globalFun._stopIt(event)
			}
		});
	});
</script>