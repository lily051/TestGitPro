<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<link rel="stylesheet" href="/css/commons_menu.css">
<style>
  #changePWD .popTitle{
    background: #3F85E4;
  }
</style>
<div id="header" class="layout">
    <div class="container">
        <div class="navbar-header clearfix" id="navbar-header">
        	<img src="${Sysconfig_6003 }" alt="" id="logoImg" class="navbarHeaderImg"/>
            <a class="navbarBrand" id="logotitle" title="${Sysconfig_6000 }">${Sysconfig_6000 }</a>
            <ul class="navbarTop">
            	<li class="link" id="datacollect"><a href="/web/index.action">数据采集</a></li>
              <li class="link" id="busiRelate"><a href="/webdevice/contact/toOpenPageBusiRelate.action">业务关联</a></li>
              <li class="link" id="logSearch"><a href="/webdevice/log/toOpenPageLog.action">日志查询</a></li>	
              <li class="sys" style="margin-left:30px;margin-right:0">|</li>
              <li class="sys" id="top_name" style="margin-left:20px;cursor:default;margin-right:0;width: 80px;overflow: hidden;white-space: nowrap;text-overflow: ellipsis;"><span class='userIcon' title="${userName }">${userName }</span></li>
               <li class="sys dropmove" id="downLoad" style="height:48px;zoom:1;overflow:visible;z-index:5;display:none;padding-left:20px;">
                  <span class="oprDown">下载</span>
                </li>
              <li class="sys" id="pwdID"><span class="oprChange">修改密码</span></li>
              <li class="sys"><span class="oprExit" id="logout">退出</span></li>
              <!-- <li class="sys">|</li>
              <li class="sys dropmove">
                <span>系统异常</span>
                <div class="dropdown sysUnusual">
                  <label class="managerOpr oprStorage">上传存储</label>
                  <label class="managerOpr oprDevice">111</label>
                </div>
              </li> -->
            </ul>
            
        </div>
    </div> 
</div>
<div class="dropdown downloadInfo">
  <div class="mainInfo">
    <div class="rowInfo lastRow">
      <a class="downImg downLoadExe">
        <img src="/images/collect/vs2008.png" alt="" >
      </a>
      <p class="mainDes"><a class="downLoadExe">理财上传客户端</a></p>
      <p class="subDes">用于上传本地usb理财录像文件</p>
    </div>
  </div>
  <div class="mainInfo">
    <div class="rowInfo lastRow">
      <a class="downImg downLoadFmsClientExe">
        <img src="/images/collect/vs2008.png" alt="" >
      </a>
      <p class="mainDes"><a class="downLoadFmsClientExe">双录客户端</a></p>
      <p class="subDes">用于理财业务办理，录音录像</p>
    </div>
  </div>
</div>
<div class="dropdownIcon"><img src="/images/index/iconArrow.png" class="iconArrow" alt=""></div>
<div class="newOverLay"></div>
<script src="/js/common/collect-top.js"></script>