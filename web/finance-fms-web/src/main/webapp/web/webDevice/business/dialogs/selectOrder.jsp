<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>

<div class="content">
	<div class="order sign" data-type="signOrder"></div>
	<div class="order complain" data-type="complainOrder"></div>
</div>
<script>
	$(function(){
		$(".content div").on('click', function(event) {
			var type=$(this).data("type");
			//$('#Dialog').dialog('close');
			if(type == "signOrder"){
				FMS.dataBusiness.signOrder();
			}else{
				FMS.dataBusiness.complainOrder();
			}
		});
	})
</script>