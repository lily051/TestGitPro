 /**
 * @file FMS 密码检测 
 * @author   Eli Pei
 */

//密码强度检测
function passwordColor(password,loginStrName) {
    var iRank =  getPwdRank(password,loginStrName);
    iRankColor(iRank,password);
    return iRank;
}
function iRankColor(iRank,password) {
    var $colorPWD = $("#colorPWD");
    var $pwdInfo = $("#pwdInfo");
    var iRankText = $("#iRankText");
    if(!password || password.length == 0) {
        $pwdInfo.attr("class","noneRePWD");
        $colorPWD.find("span").removeClass('spanRed spanOrg spanPassword');
        iRankText.text('');
        return;
    } else {
        $pwdInfo.attr("class","rePWD");
    }
    $colorPWD.find("span").each(function(i) {
        if(i <= iRank && iRank <=1 ) {
            $(this).attr("class","spanRed");
        }else if(i <= iRank && iRank ==2 ) {
            $(this).attr("class","spanOrg");
        }else if(i <= iRank && iRank ==3 ) {
            $(this).attr("class","spanPassword");
        } else {
            $(this).attr("class","");
        }
    });
    var txtArr = ['风险', '弱', '中', '强'];
    iRankText.text(txtArr[iRank]);
}
function getPwdRank(szPwd, szUser) {
    var iRank = 0;
    var reverse = szUser.split("").reverse().join("");
    szPwd.match(/[a-z]/g) && iRank++;
    szPwd.match(/[A-Z]/g) && iRank++;
    szPwd.match(/[0-9]/g) && iRank++;
    szPwd.match(/[^a-zA-Z0-9]/g) && iRank++;
    iRank = (iRank > 3 ? 3 : iRank);
    if (szPwd.length < 8 || iRank === 1 || szPwd === szUser || szPwd === reverse) {
       iRank = 0;
    }
    if (iRank === 2) {
       if ((szPwd.match(/[0-9]/g) && szPwd.match(/[a-z]/g)) || (szPwd.match(/[0-9]/g) && szPwd.match(/[A-Z]/g))) {
           iRank = 1;
       }
    }
    return iRank;
}
//初始化显示密码等级
function showPwdLevel(){
    switch($('#iRank').val()){
        case "0":
            $('#pwdLvl').text('风险密码');
            /*$('#userPassword').attr('minLength',6);
            $('#userPassword').attr('self-focus-info', '输入的项的长度不能少于6个字，不能超过64个字');
            $('#confirmPassword').attr('minLength',6);*/
            break;
        case "1":
            $('#pwdLvl').text('弱密码');
            break;
        case "2":
            $('#pwdLvl').text('中密码');
            break;
        case "3":
            $('#pwdLvl').text('强密码');
            break;
    }
    $('#userPassword').on('keyup',function(){
        var value = $('#userPassword').val();
        var strName = $('#strName').val();
        if(/[\u4e00-\u9fa5]+/i.test(value)){
        	jAlert("密码不能出现中文！","错误","error");
        	value = $('#userPassword').val("");
        }
        passwordColor(value,strName);
    }).on('keydown',function(){
        var capsLockKey = event.keyCode ? event.keyCode : event.which; //按键检测 space
        if(capsLockKey == 32){
            return false;
        }
    });
    $("#userPassword").bind('paste', function(e) { 
    	setTimeout(function() { 
    		var value = $('#userPassword').val();
            var strName = $('#strName').val();
            if(/[\u4e00-\u9fa5]+/i.test(value)){
            	jAlert("密码不能出现中文！","错误","error");
            	value = $('#userPassword').val("");
            }
            passwordColor(value,strName);
    	}, 100); 
    }); 
}