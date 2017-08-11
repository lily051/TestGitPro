<!DOCTYPE html>
<%@ page language="java" contentType="text/html; charset=UTF-8"%>
<html lang="zh">
<head>
    <meta charset="utf-8">
    <%@ include file="/common/commons.jsp" %>
    <%@ include file="/common/commons-extend.jsp" %>
    <link  href="/modules/server/css/server.css" rel="stylesheet"></link>
    <!--[if lte IE 6]>
        <script type="text/javascript">  
            DD_belatedPNG.fix('div.serverIc,div.addServer');  
        </script>
    <![endif]-->
</head>

<body>
    <div id="wrapper">
    <%@ include file="/common/commons-basemenu.jsp" %>
        <div id="content" class="layout">
			
		</div>
    </div>
    <div id="footer"></div>
    </div>
</body>
<script>
    $('#content').load('/modules/server/getServerPage.action',function(){
        $('.serverNtype').each(function(){
            var _self = $(this);
            var type = _self.data('type')
            if(type == '20004'){ // SERVICE_UPLOAD: 20004, // 上传服务,
                _self.parent().addClass('uploadServer');
                $(".uploadServer").find("li.timeout").remove();
                if(modeParam.usbAndIpc=="usb"){
                	$(".uploadServer").find('.imgBox').removeClass('serverOffLine');
                    $(".uploadServer").find(".serverStatus,li.must").remove();
                }
            }else if(type == '20200'){ // SERVICE_WEB: 20200,     //WEB服务
                _self.parent().addClass('WebServer');
            }else if(type == '20001'){ // SERVICE_TIMING: 20001, // 校时服务
                _self.parent().addClass('timingServer');
            }else if(type == '20002'){ // SERVICE_RECGUARD: 20002  //录像守卫服务
                _self.parent().addClass('recGuardServer');
            }
            //不同服务设置不同class uploadServer WebServer timingServer recGuardServer           
        });
        if($(".serverLength").length==4){
            $("#addServer").hide();
        }
    });
</script>
</html>