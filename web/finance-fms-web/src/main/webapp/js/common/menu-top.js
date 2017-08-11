$(function(){
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
    $("#setupState,.lastDec a").on('click', function(event) {
        FinanceUtil.dialog({
            id:'Dialog',
            title:'安装说明',
            overlay: true,
            draggable: true,
            width: 450,
            height: 420,
            iframeFix:false,
            ajax: {
              url: '/web/toUploadDesc.action',
              success: function() {
            	FMS.globalFun.isIE6HiddenSelect("#wrapper");
                FinanceUtil.initFormUI('#Dialog');
              }
            },
            close: function(){
            	FMS.globalFun.isIE6ShowSelect("#wrapper");
            },
            buttons: {
              "cancel": {
                text: '取消',
                click: function() {
                  $('#Dialog').dialog('close');
                }
              }
            }
        });          
    });
    $(".route").on('click', function(event) {
        var flag = $(this).data("route");
        $.ajax({
          url:'/web/getfirstmenu.action',
          type:'post',
          dataType:'json',
          data:{
            'flag':flag
          },
          success:function(data){
            if(data &&data.success){
              if(data.data.privilege == null){
                jAlert("您没有访问该模块的权限！！");
                return;
              }
              var url = data.data.privilege.strHref;
              window.location.href = url;
            }
          }
        });
    });
  	//退出系统
  	$("#logout").click(function(){
    		jConfirm("确定要退出当前登录吗？", {title:"提示", callback:function(value) {
    			if(value) {
      				$.ajax({
        				  url: '/web/logout.action',
        				  success: function(result){
          					  if(result && result.success){
          						    location.href = "/";
          					  }else if(result && result.msg){
          						    jAlert(result.msg, language.text('ajax.error_title'), 'error', function(){
            						  location.href = "/";
          						});
        					  }
        				  }
      				});
    			}else {
    				  	
    				 return false;
    			}
    		}
      });
  	});
    //修改密码方法
    $('#pwdID').on('click',function(){
        FinanceUtil.dialog({
            id: 'changePWD',
            title: "修改密码",
            overlay: true,
            draggable: true,
            width: 450,
            height: 330,
            iframeFix:false,
            ajax: {
                url: '/web/toChangePassword.action',
                success: function() {
            	      FMS.globalFun.isIE6HiddenSelect("#wrapper");
                    FinanceUtil.initFormUI('#changePWD');
                    FMS.globalFun.isIE6ChangeTipText('#changePWD');
                }
            },
            close: function(){
            	  FMS.globalFun.isIE6ShowSelect("#wrapper");
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