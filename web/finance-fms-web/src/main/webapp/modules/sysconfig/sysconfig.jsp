<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<head>
	<link  href="/modules/sysconfig/css/sysconfig.css" rel="stylesheet"></link>
	<script type="text/javascript" src="/modules/sysconfig/js/sysconfig.js"></script>
</head>
<body>
  <div class="main-wrap wrapFixed">
      <div class="fullfit">
           <div class="systool">
             <ul>
               <li data-url="/modules/sysconfig/toPlatformParamPage.action" class="active"><span>平台信息</span></li>
               <li data-url="/modules/sysconfig/toRecordParamPage.action"><span>录像参数</span></li>
               <li data-url="/modules/sysconfig/toModeParamPage.action"><span>模式参数</span></li>
               <li data-url="/modules/sysconfig/toPasswordParamPage.action"><span>安全策略</span></li>
               <li data-url="/modules/sysconfig/toBussinessParamPage.action"><span>业务参数</span></li>
               <li data-url="/modules/sysconfig/toLogParamPage.action"><span>日志保存天数</span></li>
               <li data-url="/modules/sysconfig/toNtpTimePage.action"><span>NTP校时</span></li>
             </ul>
           </div>
           <div class="sysMain">
             
           </div>
      </div>
  </div>
</body>
</html>

