<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<link rel="stylesheet" href="/css/commons_menu.css">
<div id="header" class="layout" style="overflow:visible;zoom: 1;z-index:1;position: absolute;">
    <div class="container" >
        <div class="navbar-header clearfix" id="navbar-header">
        	<img src="${Sysconfig_6003 }" alt="" id="logoImg" class="navbarHeaderImg"/>
            <a class="navbarBrand" href="/web/index.action" id="logotitle" title="${Sysconfig_6000 }">${Sysconfig_6000 }</a>
            <ul class="navbarTop">
                <li class="link" id="navbarIndex"><a href="/web/index.action">首页</a></li>
                <li class="link" id="navbarBusiness"><a class="route" data-route="2">业务应用</a></li>
                <li class="link" id="navbarBase"><a class="route" data-route="1">基础配置</a></li>
                <li class="sys" style="margin-left:30px;margin-right:0">|</li>
                <li class="sys" id="top_name" style="margin-left:20px;cursor:default;margin-right:0;width: 80px;overflow: hidden;white-space: nowrap;text-overflow: ellipsis;">
                	<span class='userIcon' title="${userName }">${userName }</span>
                </li>
                <li class="sys dropmove" id="downLoad" style="height:48px;padding-left:20px;">
                   	<span class="oprDown">下载</span>
                </li>
                <li class="sys" id="pwdID"><span class="oprChange">修改密码</span></li>
              	<li class="sys" id="logout"><span class="oprExit">退出</span></li>
            </ul>
        </div>
    </div> 
</div>
<div class="dropdown downloadInfo">
  <div class="mainInfo">
    <div class="rowInfo">
      <a href="/download/Package.exe" class="downImg">
        <img src="/images/collect/vs2008.png" alt="" >
      </a>
      <p class="mainDes"><a href="/download/Package.exe">VS2008环境安装包下载</a></p>
      <p class="subDes">系统插件安装运行都基于VS2008</p>
    </div>
    <div class="rowInfo ie8Icon">
      <a href="/download/IE8-WindowsXP-x86-CHS.exe" class="downImg">
        <img src="/images/collect/ie8.png" alt="" >
      </a>
      <p class="mainDes"><a href="/download/IE8-WindowsXP-x86-CHS.exe">浏览器IE8下载</a></p>
      <p class="subDes">IE8为系统推荐浏览器，浏览效果最佳</p>
     
    </div>
    <div class="rowInfo">
      <a href="/download/VSPlayer.exe" class="downImg">
        <img src="/images/collect/vsplayer.png" alt="" style="width:30px;height:30px;margin:2.5px;">
      </a>
      <p class="mainDes"><a href="/download/VSPlayer.exe">海康播放器下载</a></p>
      <p class="subDes">海康播放视频的插件</p>
    </div>
    <div class="rowInfo lastRow">
      <a id="setupState" class="downImg">
        <img src="/images/collect/setup.png" alt="" >
      </a>
      <p class="mainDes lastDec"><a>安装说明</a></p>
    </div>
  </div>
</div>
<div class="dropdownIcon"><img src="/images/index/iconArrow.png" class="iconArrow" alt=""></div>
<script src="/js/common/menu-top.js"></script>