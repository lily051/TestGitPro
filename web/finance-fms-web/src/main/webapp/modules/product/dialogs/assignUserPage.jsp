<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<div>
	<form class="form-horizontal form-fixed marginTop20">
		<div class="control-group">
			<label class="control-label"><em>*</em>复核员：</label>
			<div class="controls">
				<div id="assignUserDiv" class="header-search gird_search" style="height:30px;">
					<input type="text" style="display:none;"/> 
					<input type="hidden" name="checkId" id="checkId"/>
					<input type="hidden" name="checkName" id="checkName"/>
					<a class="btn btn-link" id="searchUserButton" style="top:2px;_border:0"><span class="icon iconSearch"></span></a>
					<input id="keyValue" name="keyWord" autocomplete="off" type="text" placeholder="请输入姓名搜索" class="searchInput form-control treeSearch" value="">
					<div class="searchList"></div>
				</div>
			</div>
		</div>
	</form>
</div>

<script>
	$(function(){
		if(!isIE6){
			$("#assignUserDialog #keyValue").placeholder();
		}
		$("#searchUserButton").on('click', function(event) {
			if($.trim($("#assignUserDiv #keyValue").val()).length == 0){
				jAlert("请输入姓名搜索", '错误', 'error');
				return;
			}
			$.ajax({
				url: '/modules/product/getCheckUsers.action',
				type: 'post',
				dataType: 'json',
				data: {
					'userInfo.strName': $.trim($("#assignUserDiv #keyValue").val())
				},
				success:function(data){
					if(data && data.success){
						$(".searchList").empty();
						var list = data.data.userList;
						if(list.length==0){
							jAlert("没有用户信息");
							return;
						}
						var newUl = $('<ul></ul>');
						for(var i=0;i<list.length;i++){
							var newLi = $('<li data-id="' + list[i].id + '">' + list[i].strName + '</li>');
							newUl.append(newLi);
						}
						$(".searchList").append(newUl).show();
					}
					$(".searchList ul li").on('click', function(event) {
						$("#assignUserDiv #keyValue").val($(this).text());
						$("#checkName").val($(this).text())
						$("#checkId").val($(this).data("id"));
						$(".searchList").hide();
						$(".assignButton").show();
					});
					$(document).one('click', function(event) {
						$(".searchList").hide();
					});
				}
			});
			
		});
		$("#assignUserDiv #keyValue").on("keyup",function(){
			if($(this).val()!=$("#checkName").val()){
				$(".assignButton").hide();
			}
		}).on('blur', function(event) {
			event.preventDefault();
			if($(this).val()=="请输入姓名搜索"){
				$(this).val('');
			}
		});
	});
</script>