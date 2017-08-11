<?php 
header("Content-type:text/html;Charset=utf-8");
session_start();
include("conn/conn.php");
include ("conn/readini.php");
include "conn/socket.php";

function decode_json($json)
{
	$msm = array();
	$lines = explode('&', $json);
	foreach($lines as $l)
	{
		$arr = explode('=', $l);
		$Item = strtolower(addslashes($arr[0]));//转换成小写
		$Value = str_replace('\"', "", addslashes($arr[1]));
		$msm[$Item] = $Value;
	}
	return $msm;
  }
$json_str = file_get_contents('php://input');
$json_str=urldecode($json_str);
$arr=array();
$arr=decode_json($json_str);
if (strncmp($arr['function'],'QueryDevice',strlen('QueryDevice'))==0)
{
	
	$settings = new Settings_INI;
	$settings->load('recsvr/Recorder.ini');
	$devname=$settings->get('FocusManage.DevNumber');
	$serialnum=$settings->get('Version.Serial');
	$webport=$settings->get('WebCtrl.WebPort');
	if ($webport==-1) $webport=80;
	$query=mysql_query("select Pwd from tb_webuserinfo where UserName='admin'");
	$info=mysql_fetch_array($query);
	$password=$info[Pwd];
	
	header("Content-type: text/xml");  
	echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";  
	echo "<body>";
		echo "<devname>".$devname."</devname>";
		echo "<serialnum>".$serialnum."</serialnum>";
		echo "<webport>".$webport."</webport>";
		echo "<username>admin</username>";
		echo "<password>".$password."</password>";
	echo "</body>";
}
else if (strncmp($arr['function'],'QueryRecord',strlen('QueryRecord'))==0)
{
	$startdate=$arr['startdate'];
	$enddate=$arr['enddate'];
	$callerid=$arr['callerid'];
	$calleeid=$arr['calleeid'];
	$channelmin=$arr['channelmin'];
	$channelmax=$arr['channelmax'];
	$direction=$arr['direction'];
	$clientname=$arr['clientname'];
	$clientcode=$arr['clientcode'];
	$seatnumber=$arr['seatnumber'];
	$seatname=$arr['seatname'];
	$seatbranch=$arr['seatbranch'];
	$caculatetotalnum=$arr['caculatetotalnum'];
	$startindex=$arr['startindex'];
	$pagesize=$arr['pagesize'];
	
	$sqltmp="";
	$sqltmp.=" and RecStartTime >='$startdate'";
	$sqltmp.=" and RecStartTime <='$enddate'";
	$sqltmp.=" and ChID>='$channelmin' and ChID<='$channelmax'";
	if ($callerid!="")
		$sqltmp.=" and CallerID like '%$callerid%'";
	if ($calleeid!="")
		$sqltmp.=" and CalleeID like '%$calleeid%'";
	if ($direction=="0" || $direction=="1")
		$sqltmp.=" and InOrOut='$direction'";
	if ($clientname!="")
		$sqltmp.=" and ClientName like '%$clientname%'";
	if ($clientcode!="")
		$sqltmp.=" and ClientPhone like '%$clientcode%'";	
	if ($seatnumber!="")
		$sqltmp.=" and AgentID like '%$seatnumber%'";
	if ($seatname!="")
		$sqltmp.=" and AgentName like '%$seatname%'";
	if ($seatbranch!="")
		$sqltmp.=" and AgentBranch like '%$seatbranch%'";
	/*
  		$handle = fopen("111.txt", "wb");
  		fwrite($handle, $totalnum);
  		fclose($handle);
  		*/
	header("Content-type: text/xml");  
	echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";  
	if ($caculatetotalnum=="1")
	{
  		$temp ="select count(ID) as TotalNum from tb_recordinfo where 1=1".$sqltmp;
  		$sql1=mysql_query($temp,$conn);	
  		$info=mysql_fetch_array($sql1);
  		$totalnum=$info[TotalNum];
  		echo "<body>";
  			echo "<totalnum>".$totalnum."</totalnum>";
  		echo "</body>";
	}
	else 
	{
		$temp="select ChID,RecStartTime,HangupTime,RecTimeLen,CallerID,CalleeID,RecFilePath from tb_recordinfo where 1=1".$sqltmp;
		$temp.="limit ".$startindex.",".$pagesize;
		$sql=mysql_query($temp,$conn);
		$recordum=mysql_num_rows($sql);
			
		echo "<body>";
			echo "<RecordNum>".$recordum."</RecordNum>";
		while($info=mysql_fetch_array($sql))
		{
			echo "<Record>";
				echo "<ChID>".$info[ChID]."</ChID>";
				echo "<RecStartTime>".$info[RecStartTime]."</RecStartTime>";
				echo "<RecStopTime>".$info[HangupTime]."</RecStopTime>";
				echo "<RecTimeLen>".$info[RecTimeLen]."</RecTimeLen>";
				echo "<CallerID>".$info[CallerID]."</CallerID>";
				echo "<CalleeID>".$info[CalleeID]."</CalleeID>";
				echo "<RecFilePath>".$info[RecFilePath]."</RecFilePath>";
			echo "</Record>";
		}
		echo "</body>";
	}
}
?>