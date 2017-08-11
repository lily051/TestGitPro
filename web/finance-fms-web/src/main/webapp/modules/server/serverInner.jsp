<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<script type="text/javascript" src="/modules/server/js/server.js"></script>
<div class="container wrapper grid-s9m0 FMS_container">
	<div class="server-wrap" id="serverWrap">
		<p class="serverNote" style="padding-left:13px">所有服务只能添加一个，如需更改请直接在原服务上修改或者先删除原服务再添加。</p>

		<div class='serverBox'>
			<s:iterator id="server" value="serverList">
				<div class='serverLi serverLength'>
						<input type="hidden" data-type="${server.ntype}" class="serverNtype">
						<!-- 不同服务设置不同class uploadServer WebServer timingServer recGuardServer -->
						<p class="serverText clearfix"><span title="${server.strUser}" class="serverName">${server.strUser}</span><span class="serverStatus">${server.onlineSatus}</span></p>
						<div class="serverInner">
							<div class='imgBox  <s:if test="#server.nonline!=1">serverOffLine</s:if>'>
								<div class="serverIc serverIc1"></div>
							</div>
							<div class="serverDetail">
								<ul>
									<li class="must">&nbsp;IP：${server.strIp}</li>
									<s:if test="#server.workTimeBegin!=null"><li>上传开始时间：${server.workTimeBegin}</li></s:if>
									<s:if test="#server.workTimeEnd!=null"><li>上传结束时间：${server.workTimeEnd}</li></s:if>
									<s:if test="#server.nctrlPort!=null"><li>控制端口：${server.nctrlPort}</li></s:if>
									<s:if test="#server.ndataPort!=null"><li>数据中心端口：${server.ndataPort}</li></s:if>
									<s:if test="#server.timeOut!=null"><li class="timeout">任务时间间隔：${server.timeOut}s</li></s:if>
								</ul>
							</div>
						</div>
						<input value="${server.id}" type="hidden" name="serverInfo.id">
						<div class='serverOption'>
							<div class="optionPanel">
								<span class="icon serverEdit "></span> 
                                <span class="icon serverDelete "></span> 
                                <!-- <span class="icon serverRestart "></span> -->
							</div>
						</div>
					</div>
			</s:iterator>
			<div class='serverLi' id="addServer">
				<div class="addServer"></div>
			</div>
			<div class="cf"></div>
		</div>
	</div>
</div>

