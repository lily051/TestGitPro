<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<div class="auth-channel">
        <div class="permissionBox">
            <div class='tab-pane' id='tabFunction'>
                <ul id="netTree" class="ztree"></ul>
            </div> 
        </div>
</div>
<script>
(function(){
    var nodeId= $("#userNet_id").val();
    FMS.user.initPowerTree(false,'netTree');//TODO
})();
</script>