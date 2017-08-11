<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags" %>
<div class="auth-channel">
        <div class="permissionBox">
            <div class='tab-pane' id='tabFunction'>
                <ul id="functionTree" class="ztree"></ul>
            </div> 
        </div>
        <div class="previewVideo" id="previewVideo" style="width:350px;height:250px;"></div>
</div>
<script>
$(function(){
    var flag = $('#chooseAuth').data('diff');
    var userId = $('#userInfoId').val(),
        type;
    if(flag === 'admin'){
        $(".previewVideo").remove();
        $(".permissionBox").css("width","610px");
        var regionId = $('#regionId').val();
        FMS.user.initPowerTree(regionId,'functionTree');//TODO
    }else if(flag === 'manager'){
        setTimeout(function(){
            initOcx({ocxId:'ocxPreview',divId:'previewVideo',ocxType:0,lLayoutMode:1});
            judgeOcxAble('ocxPreview')
        },200);
        if(userId){
            type = 'edit';
        }
        FMS.user.initResouseTree(type);
    }
    /*var offset = $("#chooseAuthDialogs").offset();
    offset_x = offset.left+370;
    offset_y = offset.top+20;
     $(".previewVideo").css({"top":offset_y,"left":offset_x});*/
    /*$("#chooseAuthDialogs").scroll(function(event) {
        var scrollTop = $(this).scrollTop()+20;
        $(".previewVideo").css("top",scrollTop);
    });*/
});
</script>