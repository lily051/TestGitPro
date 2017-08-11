<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<head>
    <meta charset="utf-8">
    <%@ include file="/common/commons.jsp" %>
	<%@ include file="/common/commons-extend.jsp" %>
	<link  href="/modules/video/css/video.css" rel="stylesheet" />
	<script type="text/javascript" src="/modules/video/js/video.js"></script>
</head>
<body>
	<div id="wrapper">
		<%@ include file="/common/commons-businessmenu.jsp"%>
		<div id="content" class="layout">
			<div class="container wrapper grid-s9m0 FMS_container">
				<div class="col-main">
					<div class="main-wrap wrapFixed">
						<div class="fullfit">
							<div class="tool">
					             <ul>
					               <li data-url="/modules/record/signvideo.action" class="active"><span>签约录像</span></li>
					               <li data-url="/modules/record/complainvideo.action"><span>投诉录像</span></li>
					             </ul>
				            </div>
				           <div class="mainload" id="videoMain"></div>
						</div>
					</div>
				</div>
				<div class="col-sub">
					<jsp:include page="/common/common-tree.jsp" flush="true" >
						<jsp:param name="className" value="orgTree" />
					</jsp:include>
				</div>
			</div>
		</div>
	</div>
</body>
</html>


