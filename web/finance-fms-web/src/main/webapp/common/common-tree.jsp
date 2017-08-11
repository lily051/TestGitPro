<%@ page language="java" contentType="text/html; charset=UTF-8" %>
<div class="accordion accordion-border">
	<div class="subSearch fullfit">
		<div class="header-search">
			<span class="treeDelete"></span>
		    <span class="icon iconSearch" id="orgTreeSearch"></span>
		    <input id="keyValue" name="tree" type="text" placeholder="搜索" class="searchInput form-control treeSearch" value="">
		    <a aria-disabled="false" title="刷新" class="orgLoadIcon"><span class="icon iconReload reload"></span></a>
		</div>
		<div class="fullfit" style="padding-bottom:46px;box-sizing:border-box">
		    <div class="treeview">
		        <ul id="<%=request.getParameter("className")%>" class="ztree"></ul>
		    </div>
		</div>
	</div>
</div>
<script>
	$(".searchInput").placeholder();
	$('#keyValue').keydown(function(e) {
		var key = e.keyCode;
		if (key == 13) {
			$(this).prev().click();
		}
	});
</script>
<script src="/js/common-tree.js"></script>