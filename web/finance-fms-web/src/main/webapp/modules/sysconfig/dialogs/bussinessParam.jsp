<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<div class="bussnessSelect">
    <div class="leftBox">
       <div class="header"><span>未选参数</span></div>
       <div class="body">
       		<ul>
       		<s:iterator id="listOut" value="sysconfigListOut">
       			<li>
       			<input type="checkbox" class="checkbox" value="${listOut.nkey}"><span class="paramInfo">${listOut.strDescribe}</span>
       			<input class="paramInput" type="text" value="${listOut.strDescribe}"><span class="icon iconEdit iconHidden"></span>
       			</li>
       		</s:iterator>
       		</ul>
       </div>
    </div>
    <div class="dualControl">
       <button type="button" class="sys-bPrimary" id="paramAdd">添加 &gt;&gt;</button>
       <br><br>
       <button type="button" class="sys-cancel" id="paramRemove">&lt;&lt; 移除</button>
    </div>
    <div class="rightBox">
    	 <div class="header"><span>已选参数</span></div>
    	 <div class="body">
	 		<ul>
	 		    <s:iterator id="listIn" value="sysconfigListIn">
	 			 <li>
	 			 <input type="checkbox" class="checkbox" value="${listIn.nkey}"><span class="paramInfo">${listIn.strDescribe}</span>
	 			 </li>
	 			</s:iterator>
	 		</ul>
    	 </div>
    </div>
</div>
<script>
	$(function(){
		if(isIE6){
			$(".bussnessSelect").height($(".bussnessSelect").parents(".fullfit").height()-80);
			$(".body").height($(".bussnessSelect").height()-35);
		}
		//$("input:checkbox").uniform();
		var oldValue;
		/*$(".iconEdit").on('click', function(event) {
			var li = $(this).parent("li");
			li.addClass('active').siblings().removeClass('active');
			li.find(".paramInfo").hide();
			oldValue = li.find('.paramInfo').text();
			li.find('.paramInput').css("display","inline-block").focus().val(oldValue);
		});*/
		$(".paramInput").on('blur', function(event) {
			var newValue = $(this).val();
			if(oldValue == newValue){
				$(this).hide();
				$(this).prev('.paramInfo').show();
				return;
			}else{
				var id = $(this).parent("li").find("input[type=checkbox]").val();
				FMS.sysconfig.bussinessParamEdit(newValue, id);
			}
		});
		$("#paramAdd").on('click', function(event) {
			FMS.sysconfig.bussinessParamAdd();
		});
		$("#paramRemove").on('click', function(event) {
			FMS.sysconfig.bussinessParamRemove();
		});
	});
</script>