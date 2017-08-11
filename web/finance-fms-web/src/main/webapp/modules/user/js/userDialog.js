$(function(){
    $(".check :checkbox").uniform();
    
    //根据用户类型显示相应信息
    var user = $('#bindUser').val();
    if(user == '0'){
        $('#certBox').hide();
        $('#chooseAuth').data('diff','admin');
        $('#bindRole').prop('disabled',false);
    }else if(user == '1'){
        $('#chooseAuth').data('diff','manager');
        $('#bindRole').prop('disabled',true).parents('.control-group').hide();
        if(modeParam.usbAndIpc=="usb"){
            $("#authShow").parents(".control-group").hide();
        }
    }
    $("#ownInfo,#moreInfo").on('click', function(event) {
        var ownInfoDiv = $(this).parent('div').next('div'),
            open = ownInfoDiv.data("open");
        var position = $(this).position();
        if(open=="open"){
            ownInfoDiv.hide();
            $(".iScroll").scrollTop(position.top);
            ownInfoDiv.data("open",'close');
            $(this).find('span').removeClass('trigonOpen');
        }else{
            ownInfoDiv.show();
            $(".iScroll").scrollTop(position.top);
            ownInfoDiv.data("open",'open');
            $(this).find('span').addClass('trigonOpen');
        }
    });
    /*$("#moreInfo").on('click', function(event) {
        var moreInfoDiv = $("#moreInfoDiv"),
            open = ownInfoDiv.data("open");
        if(open=="open"){
            ownInfoDiv.hide();
            ownInfoDiv.data("open",'close');
            $(this).find('span').removeClass('trigonOpen');
        }else{
            ownInfoDiv.show();
            ownInfoDiv.data("open",'open');
            $(this).find('span').addClass('trigonOpen');
        }
    });*/
    $('#bindUser').on('change',function(){
        $("#channelChoose").empty().hide().data("open","close");
        $("#chooseCertAdd,#chooseAuth").find('span').removeClass('trigonOpen');
        var val = $(this).val();
        $('#AuthBox').empty();//清空区域内容
        if(val == '0'){
            $("#authRed").show();
            $("#chooseCertAdd").parent('div').hide();
            $("#relateCert").empty().hide().data("open","close");
            $('#ResourseBox').hide();
            $('#bindRole').prop('disabled',false).parents('.control-group').show();
            $('#chooseAuth').data('diff','admin');
            $('#prependOption').remove();
            if(modeParam.usbAndIpc=="usb"){
                $("#chooseAuth").parents('.legendTitle').show();
            }
        }else if(val == '1'){
            $("#authRed").hide();
            $("#chooseCertAdd").parent('div').show();
            $('#bindRole').prop('disabled',true).parents('.control-group').hide();
            $('#chooseAuth').data('diff','manager');
            $('#ResourseBox').show();
            if(modeParam.usbAndIpc=="usb" || modeParam.channelIpBind == "true"){
                $("#chooseAuth").parents('.legendTitle').hide();
            }
        }
        $("#bindRole").uniform();
    });
    //选择所属网点
    $("#userNet").on('click', function(event) {
        FinanceUtil.dialog({
            id: 'chooseNetDialogs',
            title: '所属网点',
            overlay: true,
            draggable: true,
            width: 350,
            height: 400,
            iframeFix: true,
            ajax: {
                url: '/modules/user/toChooseNetPage.action',//TODO 选择网点弹出框
                success: function() {
                    //$(".previewVideo").remove();
                	FMS.globalFun.isIE6HiddenSelect(".dialog");
                    FinanceUtil.initFormUI('#chooseNetDialogs');
                }
            },
            close: function(){
            	FMS.globalFun.isIE6ShowSelect(".dialog");
            },
            buttons: {
                "ok": {
                    text: '完成',
                    'class': 'bPrimary',
                    click: function() {
                        selectedResNodes = $.fn.zTree.getZTreeObj("netTree").getSelectedNodes()[0];
                        $("#userNet").val(selectedResNodes.label).focus().blur();
                        $("#userNet_id").val(selectedResNodes.id.split("_")[1]);
                        $('#chooseNetDialogs').dialog('close');
                    }
                },
                "cancel": {
                    text: '取消',
                    click: function() {
                        $('#chooseNetDialogs').dialog('close');
                    }
                }
            }
        });
    });
    //添加页面选择证书
    $('#chooseCertAdd').on('click',function(){
        var relateCert = $("#relateCert"),
            dataOpen = relateCert.data('open'),
            type=$(this).data("type");
        if(dataOpen=="close"){
            if(relateCert.children().length>0){
                relateCert.show();
                $(".iScroll").scrollTop(0);
                var position=$('#chooseCertAdd').position();
                $(".iScroll").scrollTop(position.top);
            }else{
                if(type=="add"){
                    relateCert.load('/modules/user/toCertsAddPage.action',function(){
                        relateCert.show();
                        $(".iScroll").scrollTop(0);
                        var position=$('#chooseCertAdd').position();
                        $(".iScroll").scrollTop(position.top);
                    }); 
               }else{
                    var userInfoId = $('#userInfoId').val();
                    relateCert.load('/modules/user/toCertsEditPage.action',{"userInfo.id": userInfoId},function(){
                       relateCert.show();
                       $(".iScroll").scrollTop(0);
                       var position=$('#chooseCertAdd').position();
                       $(".iScroll").scrollTop(position.top);
                    });  
               }
               
            }
            relateCert.data('open','open');
            $(this).find('span').addClass('trigonOpen');
        }else if(dataOpen=="open"){
            relateCert.hide();
            relateCert.data('open','close');
            $(this).find('span').removeClass('trigonOpen');
        }
      
       /* FinanceUtil.dialog({
            id: 'chooseCertDialogs',
            title: '选择证书',
            overlay: true,
            draggable: true,
            width: 620,
            height: 400,
            iframeFix: iframeFix,
            ajax: {
                url: '/modules/user/toCertsAddPage.action',
                success: function() {
                    FinanceUtil.initFormUI('#chooseCertDialogs');
                }
            },
            buttons: {
                "ok": {
                    text: '完成',
                    'class': 'bPrimary',
                    click: function() {
                        var data = [];
                        var $target = $('#certListId');
                        $('#certListId').empty();
                        $('.certCard.selected').each(function(){
                            var _self = $(this);
                            data.push({"ncrtificateId":_self.data('id')});
                            $target.append('<div class="certListBox">'+_self.text()+'</div>');
                        })
                        $('#chooseCertAdd').data('cert',data);
                        $('#chooseCertDialogs').dialog('close');
                    }
                },
                "cancel": {
                    text: '取消',
                    click: function() {
                        $('#chooseCertDialogs').dialog('close');
                    }
                }
            }
        });*/
    });
    //编辑页面选择证书
//    $('#chooseCertEdit').on('click',function(){
//        var userInfoId = $('#userInfoId').val();
//        FinanceUtil.dialog({
//            id: 'chooseCertDialogs',
//            title: '选择证书',
//            overlay: true,
//            draggable: true,
//            width: 620,
//            height: 400,
//            iframeFix: iframeFix,
//            ajax: {
//                url: '/modules/user/toCertsEditPage.action',
//                param:{
//                    "userInfo.id": userInfoId
//                },
//                success: function() {
//                    FinanceUtil.initFormUI('#chooseCertDialogs');
//                }
//            },
//            buttons: {
//                "ok": {
//                    text: '完成',
//                    'class': 'bPrimary',
//                    click: function() {
//                        var data = [];
//                        var $target = $('#certListId');
//                        $('#certListId').empty();
//                        $('.certCard.selected').each(function(){
//                            var _self = $(this);
//                            data.push({"ncrtificateId":_self.data('id')});
//                            $target.append('<div class="certListBox">'+_self.text()+'</div>');
//                        })
//                        $('#chooseCertEdit').data('cert',data);
//                        $('#chooseCertDialogs').dialog('close');
//                    }
//                },
//                "cancel": {
//                    text: '取消',
//                    click: function() {
//                        $('#chooseCertDialogs').dialog('close');
//                    }
//                }
//            }
//        });
//    });
    //选择权限区域
    $('#chooseAuth').on('click',function(event){
        var flag = $('#chooseAuth').data('diff');
        var type = $('#chooseAuth').data('type');
        var width=350,height = 400;
        if(flag == "manager"){
            width = 800;
            height = 550;
        }else{
            width = 350;
            height = 400;
        }
        var channelChoose = $("#channelChoose"),
            dataOpen = channelChoose.data('open');
        if(dataOpen=="close"){
            if(channelChoose.children().length>0){
                channelChoose.show();
                $(".iScroll").scrollTop(0);
                var position=$('#chooseAuth').position();
                $(".iScroll").scrollTop(position.top);
            }else{
                channelChoose.load('/modules/user/toChooseAuthPage.action',function(){
                   channelChoose.show();
                   $(".iScroll").scrollTop(0);
                   var position=$('#chooseAuth').position();
                   $(".iScroll").scrollTop(position.top);
                });
            }
            channelChoose.data('open','open');
            $(this).find('span').addClass('trigonOpen');
        }else if(dataOpen=="open"){
            channelChoose.hide();
            channelChoose.data('open','close');
            $(this).find('span').removeClass('trigonOpen');
        }
    });
});