<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>

<div class="iScroll">
	<div class="legendTitle" style="font-weight:bold;margin-top:20px;margin-bottom:20px;">产品名称：<span>${productInfo.strName}</span></div>
	<input type="hidden" name="productId" value="${productInfo.id}">
	<div class="legendTitle" style="color:#666"><span class="titleLine"></span>已关联证书</div>
	<div class="certBox related">
	<s:iterator id="certIn" value="certConnectInList">
		<div class="certCard" data-id="${certIn.id}">
			<p class="certName">${certIn.strName}</p>
			<span class="right"></span>
		</div>
	</s:iterator>
		
	</div>
	<div class="legendTitle"><span class="titleLine"></span>未关联证书</div>
	<div class="certBox unrelated">
	<s:iterator id="certOut" value="certConnectOutList">
		<div class="certCard" data-id="${certOut.id}">
			<p class="certName">${certOut.strName}</p>
			<span class="right"></span>
		</div>
		</s:iterator>
		
	</div>
</div>

<script>
	$(function(){
		$(".certCard>.right").on('click', function(event) {
			var clone = $(this).parent('.certCard').clone(true);
			$(this).parent('.certCard').remove();
			$(".unrelated").append(clone);
		});
		$(".certCard").on('click', function(event) {
			var _this = $(this);
			if(_this.parent(".certBox").hasClass('related')){
				return;
			}else{
				var clone = _this.clone(true);
				_this.remove();
				$(".related").append(clone);
			}
		});
	})
</script>