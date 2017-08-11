var exitFlag = 0;
if(modeParam.usbAndIpc!="usb"){
    $("#top_name").css("margin-right","20px");
    $("#downLoad").remove();
}else{
    $("#downLoad").show();
}
$(function(){
    setHeartbeat();
  	//退出系统
    $("#logout").on('click', function(event) {
        jConfirm("确定要退出当前登录吗？", {title:"提示", callback:function(value) {
          if(value) {
              $.ajax({
                  url: '/web/logout.action',
                  success: function(result){
                    if(result && result.success){
                        exitFlag = 1;
                        location.href = "/";
                    }else if(result && result.msg){
                      jAlert(result.msg, language.text('ajax.error_title'), 'error', function(){
                        location.href = "/";
                      });
                    }
                  }
              });
          }else{
             exitFlag = 0;
          }
        }
      });
    });
    $("#downLoad").hover(function(event) {
        var offset = $(this).offset(),
            offsetX = offset.left-30;
        $(".downloadInfo,.dropdownIcon").css({"left":offsetX});
        $(".downloadInfo,.dropdownIcon").css('display',"block");
    }, function(event) {
        var eve = event.relatedTarget || event.toElement;
        if($(eve).closest('.downloadInfo').length||$(eve).closest('.dropdownIcon').length){
          return;
        }else{
          $(".downloadInfo,.dropdownIcon").hide();
        }
    });
    $(".downloadInfo,.dropdownIcon").hover(function() {
    }, function(event) {
        var eve = event.relatedTarget || event.toElement;
        if($(eve).closest('.downloadInfo').length||$(eve).closest('.dropdownIcon').length){
          return;
        }else{
          $(".downloadInfo,.dropdownIcon").hide();
        }
    });
    $(".downLoadExe").on('click', function(event) {
      	exitFlag=1;
      	window.location.href="/download/理财上传客户端.exe";
      	setTimeout(function(){
      		exitFlag=0;
      	},100);
    });
    $(".downLoadFmsClientExe").on('click', function(event) {
      	exitFlag=1;
      	window.location.href="/download/双录客户端.exe";
      	setTimeout(function(){
      		exitFlag=0;
      	},100);
    });
   //修改密码方法
    $('#pwdID').on('click',function(){
      FinanceUtil.dialog({
          id: 'changePWD',
          title: "修改密码",
          overlay: true,
          draggable: false,
          width: 450,
          height: 330,
          iframeFix: true,
          modal:false,
          show:false,
          close:function(){
             $(".newOverLay").hide();
             if(isIE6)$("object").removeClass('hidden');
          },
          ajax: {
            url: '/web/toChangePassword.action',
            success: function() {
              $(".newOverLay").show();
              if(isIE6)$("object").addClass('hidden');
              FinanceUtil.initFormUI('#changePWD');
            }
          },
          buttons: {
            "ok": {
              text: '确认',
              'class': 'bPrimary',
              click: function() {
                  var form = $('#changePWD').find('form');
                  var iRank = $('#iRank').val();
                  var value = $('#userPassword').val();
                  var strName = $('#strName').val();
                  var curIR = getPwdRank(value,strName);
                  form.ajaxSubmit({
                      beforeSubmit: function() {
                        if(curIR < iRank){
                          jAlert('密码安全等级不符合要求', '错误', 'error');
                          return false;
                        }
                        if (FinanceUtil.validate(form)) {
                          FinanceUtil.loader.show('请稍候...');
                          return true;
                        }else {
                          jAlert('信息输入有误', '错误', 'error');
//                          $("#userPassword").val('');
//                          $("#confirmPassword").val('');
//                          $("#userPassword")[0].focus();
                          return false;
                        }
                      },
                      url: '/web/changePassword.action',//TODO 
                      type: 'post',
                      dataType: 'json',
                      data: {
                        'newPwd': SHA256($('#confirmPassword').val()),
                        'oldPwd': SHA256($('#oldPassword').val())
                      },
                      success: function(data) {
                        FinanceUtil.loader.hide();
                        if (data && data.success) {
                            $.sticky('修改密码成功', {
                                type: "ok",
                                title: '操作成功',
                                afterclose: function() { //'完成'
                                }
                            });
                            $('#changePWD').dialog('close');
                        } else if (data && data.msg) {
//                            $("#oldPassword").val('');
//                            $("#confirmPassword").val('');
//                            $("#userPassword")[0].focus();
                            jAlert(data.msg, '错误' , 'error');
                        }
                      }
                  });
              }
            },
            "cancel": {
                text: '取消',
                click: function() {
                  $('#changePWD').dialog('close');
                }
            }
          }
        });
      });
});
function setHeartbeat(){
    $.post('/webdevice/datacollect/setHeartbeat.action');
    setTimeout(setHeartbeat,10000);
}