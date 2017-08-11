//密码强度检测
function passwordColor(password,loginStrName) {
    var iRank =  getPwdRank(password,loginStrName);
    iRankColor(iRank,password);
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
//        $colorPWD.attr("class","rePWD");
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
    
    
    var txtArr = ['风险密码', '弱密码', '中密码', '强密码'];
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