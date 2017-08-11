<?php
header("Content-type:text/html;Charset=utf-8");
/**    
 * 发送post请求    
 * @param string $url 请求地址    
 * @param array $post_data post键值对数据    
 * @return string    
 */    
function send_post($url, $post_data) {    
      $postdata = http_build_query($post_data);    
      $options = array(    
            'http' => array(    
                'method' => 'POST',    
                'header' => 'Content-type:application/x-www-form-urlencoded',    
                'content' => $postdata,    
                'timeout' => 1 * 60 // 超时时间（单位:s）    
            )    
        );    
        $context = stream_context_create($options);    
        $result = file_get_contents($url, false, $context);   
        return $result;    
}


function QueryDevice($url)
{
	$post_data=array(
	'function' => 'QueryDevice');
	
	$DeviceInfo=send_post($url, $post_data);
	return $DeviceInfo;
}
/*
$ret=QueryDevice('http://201.123.115.158/query.php');
echo $ret;
*/
 

/* startdate>enddate  return 1
	 startdate<enddate return -1
	 startdate=enddate return 0*/
function compatedate($startdate,$enddate)
{
		if ($startdate[year]<$enddate[year])
			return -1;
		else if ($startdate[year]>$enddate[year])
			return 1;
		
		if ($startdate[month]<$enddate[month])
			return -1;
		else if ($startdate[month]>$enddate[month])
			return 1;
			
		if ($startdate[day]<$enddate[day])
			return -1;
		else if ($startdate[day]>$enddate[day])
			return 1;
			
		if ($startdate[hour]<$enddate[hour])
			return -1;
		else if ($startdate[hour]>$enddate[hour])
			return 1;
			
		if ($startdate[minute]<$enddate[minute])
			return -1;
		else if ($startdate[minute]>$enddate[minute])
			return 1;
			
		if ($startdate[second]<$enddate[second])
			return -1;
		else if ($startdate[second]>$enddate[second])
			return 1;
		return 0;
}

/*******************函数说明*********************************************************
 * 函数名:QueryRecord
 * 功能:根据查询条件查询录音记录
 * 函数入参说明
$url		string  查询URL 例如 http://201.123.115.158/query.php 不能为空
$StartDate	string	数据的开始日期，不能为空，并且必须传入正确日期格式。格式为：YYYY-MM-DD HH:mm:ss,如：2014-03-11 10:30:10	   
$EndDate	string	数据的结束日期，不能为空，并且必须传入正确日期格式。，日期格式为：YYYY-MM-DD HH:mm:ss,如：2014-03-11 12:30:10	   
$callerid	string	主叫号码,值入空表示忽略此条件，支持模糊查询
$calleeid	string	被叫号码,值入空表示忽略此条件，支持模糊查询	
$channelmin	string	最小通道, 最小为1，不能为空	   
$channelmax	string	最大通道,最大为24，不能为空	   
$direction	string	呼入呼出方向，空表示全部. 0表示呼入，1表示呼出  其他值无效
$clientname	string	客户姓名,值入空表示忽略此条件，支持模糊查询	   
$clientcode	string	客户号码,值入空表示忽略此条件，支持模糊查询	      
$seatnumber string	席坐工号,值入空表示忽略此条件，支持模糊查询	   
$seatname	string	席坐姓名,值入空表示忽略此条件，支持模糊查询	   
$seatbranch	string	席坐部门,值入空表示忽略此条件，支持模糊查询
$caculateTotalnum string 查询记录总条数，取值范围0/1 0表示已经查询完记录总条数，开始查询具体记录 1表示为先查询记录总条数	   
(查询记录分两步：1.查询满足条件的总条数 2.查询具体的记录（设定查询起始offset和查询条数pagesize）)
$startindex  string 具体查询的其实offset值  (limit offfset,pagesize)
$pagesize    string 具体查询的条数 pagesize值 (limit offset,pagesize)
 * 返回值说明
 查询成功：返回查询的总条数或者返回查询到的所有记录
 查询失败：
 返回值:-1 $url为空
 	   -2 $StartDate为空或者格式错误
 	   -3 $EndDate为空或者格式错误
 	   -4 $StartDate比$EndDate大
 	   -5 $channelmin $channelmax为空 或者通道范围不再1-24 或者$channelmin>$channelmax
 	   -6 $direction 取值范围 不是""/0/1
 	   -7 $caculateTotalnum 取值不是0/1
 	   -8 $startindex 起始索引错误
 	   -9 $pagesize   查询条数错误
 	   -10 $callerid $calleeID 不是数字组成的字符串
	   -11 $clientcode 不是数字组成字符串
	   -20 参数检查合格 但是查询失败
*/
function QueryRecord($url,$StartDate,$EndDate,$callerid,$calleeid,$channelmin,$channelmax,
					 $direction,$clientname,$clientcode,$seatnumber,$seatname,$seatbranch,
					 $caculateTotalnum,$startindex,$pagesize)
{
		if($url=="")
			return -1;
		//1.startdate and enddate check
		if ($StartDate=="")
			return -2;
		if ($EndDate=="")
			return -3;
		$startdatearr=array();
		$startdatearr=date_parse_from_format('Y-m-d H:i:s',$StartDate);	
		if ($startdatearr[error_count]!=0 || $startdatearr[warning_count]!=0)
			return -2;
		$enddatearr=array();
		$enddatearr=date_parse_from_format('Y-m-d H:i:s',$EndDate);	
		if ($enddatearr[error_count]!=0 || $enddatearr[warning_count]!=0)
			return -3;
		if (compatedate($startdatearr,$enddatearr)>0)
			return -4;
			
		// channelmin and channelmax check
		if ($channelmin=="" || $channelmax=="")
			return -5;
			
		$channelmintmp=(int)$channelmin;
		$channelmaxtmp=(int)$channelmax;
		if ($channelmintmp<1 || $channelmintmp>24)
			return -5;
			
		if ($channelmaxtmp<1 || $channelmaxtmp>24)
			return -5;
		if ($channelmaxtmp<$channelmintmp)
			return -5;
		
		//direction check
		if ($direction!="" && $direction!="0" && $direction!="1")
			return -6;
		
		//index and pagesize check
		if ($caculateTotalnum!="0" && $caculateTotalnum!="1")
			return -7;
		if ($caculateTotalnum=="0")//总数已经查询过，现在查询记录
		{
			$intstartindex=(int)$startindex;
			if ($intstartindex<0)
				return -8;
			$intpagesize=(int)$pagesize;
			if ($intpagesize<0 || $intpagesize>100)
				return -9;
		}
		
		//callerid and calleeid check
		if ($callerid!="")
		{
			if (is_numeric($callerid)==false)
			{
				return -10;
			}
		}
		if ($calleeid!="")
		{
			if(is_numeric($calleeid)==false)
			{
				return -10;
			}
		}
		
		
		//check clientcode
		if ($clientcode!="")
		{
			if (is_numeric($clientcode)==false)
				return -11;
		}
			
		//check .......还不完善
		
		//2.组装数据
		$post_data=array(
		'function'  => 'QueryRecord',
		'startdate' => $StartDate,
		'enddate'   => $EndDate,
		'callerid' => $callerid,
		'calleeid'=> $calleeid,
		'channelmin'=> $channelmin,
		'channelmax'=> $channelmax,
		'direction' => $direction,
		'clientname'=> $clientname,
		'clientcode'=> $clientcode,
		'seatnumber'=> $seatnumber,
		'seatname'  => $seatname,
		'seatbranch'=> $seatbranch,
		'caculatetotalnum'=>$caculateTotalnum,
		'startindex'=> $startindex,
		'pagesize'  => $pagesize
		);	
		
		//3.POST数据获取结果
		$recordinfo=send_post($url, $post_data);
		if ($recordinfo==false)
			return -20;
		return $recordinfo;
}

$StartDate="2016-04-29 08:30:30";
$EndDate="2016-04-29 23:30:30";
$localcode="";
$remotecode="";
$channelmin="1";
$channelmax="24";
$direction="";
$clientname="";
$clientcode="";
$seatnumber="";
$seatname="";
$seatbranch="";
$url='http://201.123.115.158/query.php';
$caculateTotalnum=0;
$startindex=0;
$pagesize=20;
$ret=QueryRecord($url,$StartDate,$EndDate,$localcode,$remotecode,$channelmin,$channelmax,
					 $direction,$clientname,$clientcode,$seatnumber,$seatname,$seatbranch,
					 $caculateTotalnum,$startindex,$pagesize);
$ret=urldecode($ret);
echo $ret;
?>