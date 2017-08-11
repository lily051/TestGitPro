<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<div >
    <div class="permissionBox">
        <div class='tab-pane' id='tabFunction'>
            <ul id="functionTree" class="ztree"></ul>
        </div> 
    </div>
</div>
<script>
(function(){
    var flag = $('#chooseAuth').data('diff');
    var userId = $('#userInfoId').val(),
        type;
    if(flag === 'admin'){
        var regionId = $('#regionId').val();
        FMS.user.initPowerTree(regionId,'functionTree');//TODO
    }else if(flag === 'manager'){
        if(userId){
            type = 'edit';
        }
        FMS.user.initResouseTree(type);
    }
})();
</script>