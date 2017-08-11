<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<head>
	<script type="text/javascript" src="/modules/card/js/card.js"></script>
</head>
<body>
  <div class="main-wrap wrapFixed">
      <div class="fullfit">
            <div class="toolbar">
                <a href="#"><span class="managerOpr oprAdd oprAddCard"><i>添加</i></span></a>
                <a href="#"><span class="managerOpr oprDelete oprDeleteCard"><i>删除</i></span></a>
                <a href="#"><span class="managerOpr oprAble oprAbleUser" id="cardAble"><i>启用</i></span></a>
                <a href="#">
                  <span class="managerOpr oprDisable oprDisableUser" id="cardDisable"><i>禁用</i></span></a>
            </div>
            <div class="datatableStyle">
                <table id='cardDataTable'></table>
                <div id="cardDataTablePage"></div>
            </div>
      </div>
  </div>
</body>
</html>


