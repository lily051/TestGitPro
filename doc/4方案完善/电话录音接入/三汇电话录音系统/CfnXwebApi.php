<?php
header("Content-type:text/html;Charset=utf-8");
session_start();
include ("conn/readini.php");
include "conn/socket.php";
 
function IsLeapYear($year)
{
	if ($year%4!=0)
		return FALSE;
	if ($year%100!=0)
		return TRUE;
	if ($year%400==0)
		return TRUE;
	else 
		return FALSE;	
}

function IsDateFormat($date)
{
	$nparam=sscanf($date,"%d-%d-%d",$year,$month,$day);
	if ($nparam!=3)
		return FALSE;
	if ($year<1999 || $year>9999)
		return FALSE;
	if ($month<1 || $month>12)
		return FALSE;
	$monthday =0;
	switch($month)
	{
		case 4:
		case 6:
		case 9:
		case 11:
			$monthday =30;
			break;
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			$monthday=31;
			break;
		case 2:
			if (IsLeapYear($year)==TRUE){
				$monthday=29;
			}
			else
			{
				$monthday=28;
			}
			break;
		default:
			return FALSE;
	}
	if ($day<1 || $day>$monthday)
		return FALSE;
	return TRUE;
}

function comparedate($startdate,$enddate)
{
	sscanf($startdate,"%d-%d-%d",$syear,$smonth,$sday);
	sscanf($enddate,"%d-%d-%d",$eyear,$emonth,$eday);
	if ($syear>$eyear) return 1;
	if ($syear<$eyear) return -1;
	if ($smonth>$emonth) return 1;
	if ($smonth<$emonth) return -1;
	if ($sday>$eday) return 1;
	if ($sday<$eday) return -1;
	return 0;
}

function IsTimeFormat($time)
{
	$nparam=sscanf($time,"%d:%d:%d",$hour,$minute,$second);
	if ($nparam!=3)
		return FALSE;
	if ($hour<0 || $hour>=24)
		return FALSE;
	if ($minute<0 || $minute>=60)
		return FALSE;
	if ($second<0 || $second>=60)
		return FALSE;
	return TRUE;
}

function comparetime($starttime,$endtime)
{
	sscanf($starttime,"%d:%d:%d",$shour,$sminute,$ssecond);
	sscanf($endtime,"%d:%d:%d",$ehour,$eminute,$esecond);
	if ($shour>$ehour) return 1;
	if ($shour<$ehour) return -1;
	if ($sminute>$eminute) return 1;
	if ($sminute<$eminute) return -1;
	if ($ssecond>$esecond) return 1;
	if ($ssecond<$esecond) return -1;
	return 0;
}

function passwordValied($password)
{
	$len = strlen($password);
	for ($i=0;$i<$len;$i++)
	{
		if (($password[$i]<'#') && ($password[$i]!='!'))
			return 0;
		if ($password[$i]>'~')
			return 0;
	}
	return 1;
}
class HardwareInfo{
	public $TotalChannels;
	public $BoardType;
	public $IpAddress;
	public $Gateway;
	public $Macaddress;
	public $Mask;
	public $Dns;
	public $DevID;
	public $WebVersion;
	public $RecSvrVersion;
	public $SqlVersion;
	public $kernelVersion;
	public $CpldVersion;
}

class TotalChStatus{
	public $cTotalCh;
	public $iStartCh;
	public $arrChStatus;	
}

class ChRunStatus{
	public $cChID;
	public $cChStatus;
	public $cRecDir;
	public $szDtmf;
	public $iRecLen;
	public $szRingTime;
	public $cRingCnt;
	public $szCallerID;
	public $szCalleeID;
}

class GlobalCfg{
	public $icompress;
	public $ifileformat;
	public $szrecpath;
	public $szdevreboottime;
	public $istartch;
	public $itotalch;
	public $cSetReboot;
	public $iRebootHour;
	public $iRebootMin;
}

class ChannelCfg{
	public $iChID;
	public $iRecType;
	public $iRecDir;
	public $iLongRecInterval;
	public $iMaxReclen;
	public $iRecvDtmfTime;
	public $szBindPhoneNum;
	public $szStartDtmf;
	public $szStopDtmf;
}

class TotalChVol{
	public $iTotalCh;
	public $iStartCh;
	public $szChVol;//100瀛楄妭
}

class NetCfg{
	public $szIPAddr;//IP鍦板潃
	public $szMask;//瀛愮綉鎺╃爜
	public $szGateway;//榛樿缃戝叧
	public $szDns;//DNS
}

class MonitorInfo{
	public $ChID;
	public $szRemoteIP;
}

class CalculateInput{
		public $SelectType;
		public $start_year;
		public $start_month;
		public $start_day;
		public $end_year;
		public $end_month;
		public $end_day;
		public $monthwise_year;
		public $monthwise_month;
		public $monthwise_day;
		public $mediaType;
	}
class CalculateRet{
	public $totalcalls;
	public $totalsizeM;
	public $totalsizeKB;
	public $sizeAvailable;
}
class DiskSpace{
	public $szDiskType1;//sd/hd
	public $szDiskName1;// /dev/sda1
	public $iDiskTotalSpace1;
	public $iDiskFreeSpace1;
	public $szDiskType2;//sd/hd
	public $szDiskName2;// /dev/sda1
	public $iDiskTotalSpace2;
	public $iDiskFreeSpace2;
	public $szDiskType3;//sd/hd
	public $szDiskName3;// /dev/sda1
	public $iDiskTotalSpace3;
	public $iDiskFreeSpace3;
}

class SqlResult{
	public $result;
	public $sqlinfo;
}
class CfnXwebApi{
	public $handle;
	public $resource;
	public $debug =0;
	
	private function SqlConnect()
	{
		if (!$this->handle)
		{
			$this->handle=mysql_connect("localhost","root","") or die("鏁版嵁搴撴湇鍔″櫒杩炴帴閿欒".mysql_error());
		     mysql_select_db("db_recorder",$this->handle) or die("鏁版嵁搴撴湇鍔″櫒杩炴帴閿欒".mysql_error());
		     mysql_query("set names utf8");
		     mysql_query("alter database db_recorder default character set 'utf8'");
		     mysql_query("set @@character_set_server='utf8'");
		}
	}

	public function GetHardwareInfo()
	{
		$settings = new Settings_INI;
		$settings->load('recsvr/Recorder.ini');
		$settings1 = new Settings_INI;
		$settings1->load('recsvr/ShConfig.ini');
		
		$hardwareInfo=new HardwareInfo();
		$hardwareInfo->TotalChannels=$settings1->get('AppChToBoardChTable.TotalAppCh');
		$hardwareInfo->BoardType=$settings1->get('BoardId=0.BoardModel');
		$hardwareInfo->IpAddress=$settings->get('IPconfig.Ipaddress');
		$hardwareInfo->Gateway=$settings->get('IPconfig.Gateway');
		$hardwareInfo->Macaddress=$settings->get('Version.HWaddr');
		$hardwareInfo->Mask=$settings->get('IPconfig.Subnet');
		$hardwareInfo->Dns=$settings->get('IPconfig.Firstdns');
		$hardwareInfo->DevID=$settings->get('FocusManage.DevNumber');
		$hardwareInfo->WebVersion=$settings->get('Version.WebV');
		$hardwareInfo->RecSvrVersion=$settings->get('Version.RecSvrV');
		$hardwareInfo->kernelVersion=$settings->get('Version.KernelV');
		$hardwareInfo->CpldVersion=$settings->get('Version.CpldV');
		$this->SqlConnect();
		$sql=mysql_query("select SqlVersion from tb_webuserinfo where UserName='admin'");
		$info=mysql_fetch_array($sql);
		$hardwareInfo->SqlVersion=$info[SqlVersion];
		
		if ($this->debug){
			echo "toalchannels:$hardwareInfo->TotalChannels<br>";
			echo "boardtype:$hardwareInfo->BoardType<br>";
			echo "ipaddress:$hardwareInfo->IpAddress<br>";
			echo "gateway:$hardwareInfo->Gateway<br>";
			echo "Macaddress:$hardwareInfo->Macaddress<br>";
			echo "mask:$hardwareInfo->Mask<br>";
			echo "dns:$hardwareInfo->Dns<br>";
			echo "devid:$hardwareInfo->DevID<br>";
			echo "webv:$hardwareInfo->WebVersion<br>";
			echo "recsvrv:$hardwareInfo->RecSvrVersion<br>";
			echo "kernelv:$hardwareInfo->kernelVersion<br>";
			echo "cpldv:$hardwareInfo->CpldVersion<br>";
			echo "sqlver:$hardwareInfo->SqlVersion<br>";
		}
		return $hardwareInfo;
	}
	
	/* provide current status of all channels as connected/disconnected */
	public function GetChannelStatus()
	{
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$REP=$SC->eachMsg("func:102");
		if ($REP=="-1")
			return FALSE;
		$dataarr=unpack("cchid/lstartch/a25status", $REP);
		$totalchstatus=new TotalChStatus();
		$totalchstatus->cTotalCh=$dataarr[chid];
		$totalchstatus->iStartCh=$dataarr[startch];
		$totalchstatus->arrChStatus=$dataarr[status];
		if (debug){
			print_r($dataarr);
			echo "totalch:$totalchstatus->cTotalCh<br>";
			echo "startch:$totalchstatus->iStartCh<br>";
			echo "status:$totalchstatus->arrChStatus<br>";
		}
		return $totalchstatus;
	}
	
	public function GetChannelRunningStatus($ChID)
	{
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$senddata=pack("a10L","func:101;",$ChID);
		$REP=$SC->eachMsg($senddata);
		if ($REP=="-1"){
			if ($this->debug)	
				echo "GetChannelRunningStatus failed<br>";
			return FALSE;	
		}
		$dataarr=unpack("cchid/cchstatus/crecdir/a200dtmf/ireclen/a50ringtime/cringcnt/a128callerid/a128calleeid", $REP);
		$ChRunStatus=new ChRunStatus();
		$ChRunStatus->cChID=$dataarr[chid];
		$ChRunStatus->cChStatus=$dataarr[chstatus];
		$ChRunStatus->cRecDir=$dataarr[recdir];
		$ChRunStatus->cRingCnt=$dataarr[ringcnt];
		$ChRunStatus->iRecLen=$dataarr[reclen];
		$ChRunStatus->szCalleeID=$dataarr[calleeid];
		$ChRunStatus->szCallerID=$dataarr[callerid];
		$ChRunStatus->szDtmf=$dataarr[dtmf];
		$ChRunStatus->szRingTime=$dataarr[ringtime];
		if ($this->debug){
			print_r($dataarr);
			echo "$ChRunStatus->cChID $ChRunStatus->cChStatus $ChRunStatus->cRecDir $ChRunStatus->cRingCnt $ChRunStatus->iRecLen";
			echo " $ChRunStatus->szCalleeID $ChRunStatus->szCallerID $ChRunStatus->szDtmf $ChRunStatus->szRingTime<br>";
		}
		return $ChRunStatus;
	}
	
	public function GetGlobalCfg()
	{
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$REP=$SC->eachMsg("func:103");
		if ($REP=="-1"){
			//echo "GetGlobalCfg failed<br>";
			return FALSE;	
		}
		$dataarr=unpack("icompress/ifileformat/istartch/itotalch/a260recpath/cReboot/iRebootHour/iReootMin/a50reboottime", $REP);
		$globalcfg= new GlobalCfg();
		$globalcfg->icompress=$dataarr[compress];
		$globalcfg->ifileformat=$dataarr[fileformat];
		$globalcfg->istartch=$dataarr[startch];
		$globalcfg->itotalch=$dataarr[totalch];
		$globalcfg->szdevreboottime=$dataarr[reboottime];
		$globalcfg->szrecpath=$dataarr[recpath];
		$globalcfg->cSetReboot=$dataarr[Reboot];
		$globalcfg->iRebootHour=$dataarr[RebootHour];
		$globalcfg->iRebootMin=$dataarr[ReootMin];
		if ($this->debug)
		{
			print_r($dataarr);
			echo "<br>";
			echo"$globalcfg->icompress $globalcfg->ifileformat $globalcfg->istartch $globalcfg->itotalch $globalcfg->szdevreboottime $globalcfg->szrecpath $globalcfg->cSetReboot,$globalcfg->iRebootHour,$globalcfg->iRebootMin<br>";
		}
		return $globalcfg;
	}
	
	public function SetGlobalCfg($globalset)
	{
		if ($globalset->icompress!=6 && $globalset->icompress!=7 && $globalset->icompress!=17 && $globalset->icompress!=131)
			return FALSE;
		if ($globalset->ifileformat!=0 && $globalset->ifileformat!=1)
			return FALSE;
		if ($globalset->istartch<0)
			return FALSE;
		if ($globalset->cSetReboot!=0 && $globalset->cSetReboot!=1)
			return FALSE;
		if ($globalset->cSetReboot==1)
		{
			if ($globalset->iRebootHour<0 || $globalset->iRebootHour>=24)
				return FALSE;
			if ($globalset->iRebootMin<0 || $globalset->iRebootMin>=60)
				return FALSE;
		}
			
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$senddata=pack("a10LLLLa260cLLa50","func:104",$globalset->icompress,$globalset->ifileformat,$globalset->istartch,$globalset->itotalch,$globalset->szrecpath,$globalset->cSetReboot,$globalset->iRebootHour,$globalset->iRebootMin,$globalset->szdevreboottime);
		$REP=$SC->eachMsg($senddata);
		//echo "rep is $REP<br>";
		if($REP=="0")
			return TRUE;
		else 
			return FALSE;
	}
	
	public function GetChannelCfg($ChID)
	{
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$senddata=pack("a10L","func:105",$ChID);
		$REP=$SC->eachMsg($senddata);
		//echo "rep is $REP<br>";
		if ($REP=="-1")
			return FALSE;
		$dataarr=unpack("Lchid/Lrectype/Lrecdir/LRecInterval/Lmaxreclen/Lrecvdtmftime/a128BindPhoneNum/a32szStartDtmf/a32szStopDtmf", $REP);
		
		$channelcfg = new ChannelCfg();
		$channelcfg->iChID=$dataarr[chid];
		$channelcfg->iRecType=$dataarr[rectype];
		$channelcfg->iRecDir=$dataarr[recdir];
		$channelcfg->iLongRecInterval=$dataarr[RecInterval];
		$channelcfg->iMaxReclen=$dataarr[maxreclen];
		$channelcfg->iRecvDtmfTime=$dataarr[recvdtmftime];
		$channelcfg->szBindPhoneNum=$dataarr[BindPhoneNum];
		$channelcfg->szStartDtmf=$dataarr[szStartDtmf];
		$channelcfg->szStopDtmf=$dataarr[szStopDtmf];
		
		if ($this->debug)
		{
			print_r($dataarr);
			echo "chid:$channelcfg->iChID<br>";
			echo "irectype:$channelcfg->iRecType<br>";
			echo "recdir:$channelcfg->iRecDir<br>";
			echo "iLongRecInterval:$channelcfg->iLongRecInterval<br>";
			echo "maxreclen:$channelcfg->iMaxReclen<br>";
			echo "recvdtmftime:$channelcfg->iRecvDtmfTime<br>";
			echo "bindPhoneNum:$channelcfg->szBindPhoneNum<br>";
			echo "szStartDtmf:$channelcfg->szStartDtmf<br>";
			echo "szStopDtmf:$channelcfg->szStopDtmf<br>";
		}
		return $channelcfg;
	}
	
	public function SetWebPort($WebPort)
	{
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$senddata=pack("a10L","func:119",$WebPort);
		$REP=$SC->eachMsg($senddata);
		//echo "rep is $REP<br>";
		if ($REP=="-1")
			return FALSE;
	
		return TRUE;
	}
	
	
	public function SetChannelCfg($channelcfg)
	{
		$senddata=pack("a10LLLLLLa128a32a32","func:106",$channelcfg->iChID,$channelcfg->iRecType,$channelcfg->iRecDir,$channelcfg->iLongRecInterval,
		$channelcfg->iMaxReclen,$channelcfg->iRecvDtmfTime,$channelcfg->szBindPhoneNum,
		$channelcfg->szStartDtmf,$channelcfg->szStopDtmf);
		
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$REP=$SC->eachMsg($senddata);
		//echo "rep is $REP<br>";
		if ($REP=="0")
			return TRUE;
		else 
			return FALSE;
	}
	
	public function SetDateTime($time)
	{
		$senddata=pack("a10a30","func:107",$time);
		
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$REP=$SC->eachMsg($senddata);
		//echo "rep is $REP<br>";
		if ($REP=="0")
			return TRUE;
		else 
			return FALSE;
	}
	
	public function RebootDevice($cmd)
	{
		if ($cmd!=1 && $cmd!=0)
			return FALSE;
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);	
		$senddata=pack("a10c","func:118",$cmd);
		$REP=$SC->eachMsg($senddata);
		//echo "rep is $REP<br>";
		if ($REP=="0")
			return TRUE;
		else 
			return FALSE;
	}
	
	/* this function return device time information as string */
	public function GetDeviceTime()
	{
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$REP=$SC->eachMsg("func:108");
		if ($REP=="-1")
			return FALSE;
		$dataarr=unpack("a64time",$REP);
		if ($this->debug){
			print_r($dataarr);
			echo "$dataarr[time]<br>";
		}
		return $dataarr[time];
	}
	
	public function GetWebPort()
	{
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$REP=$SC->eachMsg("func:120");
		if ($REP=="-1")
			return -1;
		$dataarr=unpack("iwebport",$REP);
		return $dataarr[webport];
	}
	
	public function GetDiskFreeSpace()
	{
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$REP=$SC->eachMsg("func:111");
		if ($REP=="-1")
			return FALSE;
		$dataarr=unpack("a20type1/a128name1/itotal1/ifree1/a20type2/a128name2/itotal2/ifree2/a20type3/a128name3/itotal3/ifree3",$REP);
		$diskinfo=new DiskSpace();
		$diskinfo->iDiskFreeSpace1=$dataarr[free1];
		$diskinfo->iDiskTotalSpace1=$dataarr[total1];
		$diskinfo->szDiskName1=$dataarr[name1];
		$diskinfo->szDiskType1=$dataarr[type1];
		$diskinfo->iDiskFreeSpace2=$dataarr[free2];
		$diskinfo->iDiskTotalSpace2=$dataarr[total2];
		$diskinfo->szDiskName2=$dataarr[name2];
		$diskinfo->szDiskType2=$dataarr[type2];
		$diskinfo->iDiskFreeSpace3=$dataarr[free3];
		$diskinfo->iDiskTotalSpace3=$dataarr[total3];
		$diskinfo->szDiskName3=$dataarr[name3];
		$diskinfo->szDiskType3=$dataarr[type3];
		
		if ($this->debug){
			print_r($dataarr);
			echo "<br>";
			echo "$diskinfo->iDiskFreeSpace1<br>";
			echo "$diskinfo->iDiskTotalSpace1<br>";
			echo "$diskinfo->szDiskName1<br>";
			echo "$diskinfo->szDiskType1<br>";
			echo "$diskinfo->iDiskFreeSpace2<br>";
			echo "$diskinfo->iDiskTotalSpace2<br>";
			echo "$diskinfo->szDiskName2<br>";
			echo "$diskinfo->szDiskType2<br>";
			echo "$diskinfo->iDiskFreeSpace3<br>";
			echo "$diskinfo->iDiskTotalSpace3<br>";
			echo "$diskinfo->szDiskName3<br>";
			echo "$diskinfo->szDiskType3<br>";
		}
		return $diskinfo;
	}
	
	public function GetLineVol()
	{
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$REP=$SC->eachMsg("func:110");
		if ($REP=="-1")
			return FALSE;
		$dataarr=unpack("Ltotalch/Lstartch/a100chvol",$REP);
		$totalchvol=new TotalChVol();
		$totalchvol->iTotalCh=$dataarr[totalch];
		$totalchvol->iStartCh=$dataarr[startch];
		$totalchvol->szChVol=$dataarr[chvol];
		if ($this->debug){
			print_r($dataarr);
			echo "totalch:$totalchvol->iTotalCh<br>";
			echo "iStartCh:$totalchvol->iStartCh<br>";
			echo "szChVol:$totalchvol->szChVol<br>";
		}
		return $totalchvol;
	}
	
	public function SetNetCfg($NetCfg)
	{
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$senddata=pack("a10a16a16a16a16","func:109",$NetCfg->szIPAddr,$NetCfg->szMask,$NetCfg->szGateway,$NetCfg->szDns);
		$REP=$SC->eachMsg($senddata);
		//echo "rep is $REP<br>";
		if ($REP=="0")
			return TRUE;
		else 
			return FALSE;
	}
	
	
	
	
	public function CalculateData($calInput)
	{
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$senddata=pack("a10cLccLccLccL","func:114",$calInput->SelectType,$calInput->start_year,$calInput->start_month,$calInput->start_day,
		$calInput->end_year,$calInput->end_month,$calInput->end_day,$calInput->monthwise_year,$calInput->monthwise_month,$calInput->monthwise_day,$calInput->mediaType);
		$REP=$SC->eachMsg($senddata);
		if ($REP=="-1")
			return FALSE;
		$dataarr=unpack("Ltotalcalls/LtotalsizeM/LtotalsizeKB/cavailable",$REP);
		$calret = new CalculateRet();
		$calret->totalcalls = $dataarr[totalcalls];
		$calret->totalsizeM = $dataarr[totalsizeM];	
		$calret->totalsizeKB = $dataarr[totalsizeKB];	
		$calret->sizeAvailable = $dataarr[available];
		if ($this->debug)
		{
			print_r($dataarr);
			echo "totalcalls:$calret->totalcalls<br>";
			echo "totalsizeM:$calret->totalsizeM<br>";
			echo "totalsizeKB:$calret->totalsizeKB<br>";
			echo "sizeAvailable:$calret->sizeAvailable<br>";
		}	
		return $calret;
	}
	
	public function CheckMediaConnected()
	{
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$senddata=pack("a10","func:115");
		$REP=$SC->eachMsg($senddata);
		if ($this->debug)
		{
			echo "CheckMediaConnected is $REP<br>";
		}	
		return $REP;
	}
	
	public function CheckMediaAvailable()
	{
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$senddata=pack("a10","func:116");
		$REP=$SC->eachMsg($senddata);
		if ($this->debug)
		{
			echo "CheckMediaAvailable is $REP<br>";
		}	
		return $REP;
	}
	
	public function WriteDVD()
	{
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$senddata=pack("a10","func:117");
		$REP=$SC->eachMsg($senddata);
		if ($this->debug)
		{
			echo "WriteDVD is $REP<br>";
		}	
		if ($REP=="0")
			return TRUE;
		else 
			return FALSE;
	}
	
	public function StartMonitor($monitorInfo)
	{
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$senddata=pack("a10La20","func:112",$monitorInfo->ChID,$monitorInfo->szRemoteIP);
		$REP=$SC->eachMsg($senddata);
		//echo "rep is $REP<br>";
		
		if ($REP=="0")
			return TRUE;
		else 
			return FALSE;
	}
	public function StopMonitor($monitorInfo)
	{
		$serverIP=$_SERVER[SERVER_ADDR];
		$SC=new Socket($serverIP,1001);
		$senddata=pack("a10La20","func:113",$monitorInfo->ChID,$monitorInfo->szRemoteIP);
		$REP=$SC->eachMsg($senddata);
		//echo "rep is $REP<br>";
		if ($REP=="0")
			return TRUE;
		else 
			return FALSE;
	}
	public function GetDevID()
	{
		$settings = new Settings_INI;
		$settings->load('recsvr/Recorder.ini');
		$devID=$settings->get('FocusManage.DevNumber');
		if ($this->debug) echo "devID :$devID<br>";
		return $devID;
	}
	
	public function DeleteFile($filename)
	{
		$ret= unlink($filename);
		$this->SqlConnect();
		mysql_query("delete from tb_recordinfo where RecFilePath='$filename'");
		mysql_query("delete from tb_recordinfow where RecFilePath='$filename'");		
		return $ret;
	}
	
	public function DelRecordFile($StartDate,$StartTime,$EndDate,$EndTime,$StartChnl,$EndChnl)
	{
		if ((IsDateFormat($StartDate)!=TRUE) || (IsDateFormat($EndDate)!=TRUE))
			return -1;
		$result=comparedate($StartDate, $EndDate);
		if ($result==1)
			return -1;
		if ((IsTimeFormat($StartTime)!=TRUE) || (IsTimeFormat($EndTime)!=TRUE))
			return -1;
		if ($result==0){
			if (comparetime($StartTime, $EndTime)==1)
				return -1;
		}
		
		$settings = new Settings_INI;
		$settings->load('recsvr/Recorder.ini');
		$settings1 = new Settings_INI;
		$settings1->load('recsvr/ShConfig.ini');
		$maxch=$settings1->get('AppChToBoardChTable.TotalAppCh');
		$startCh=$settings->get('RecSvr.StartChannel');
		//echo "startch:$startCh maxch:$maxch<br>";
		
		if($StartChnl<$startCh || $StartChnl >=$startCh+$maxch)
			return -1;
			
		if($EndChnl<$startCh || $EndChnl >=$startCh+$maxch)
			return -1;
		if ($StartChnl>$EndChnl)
			return -1;
		
		$this->SqlConnect();
		$StartChnl=$StartChnl-$startCh;
		$EndChnl=$EndChnl-$startCh;
		$sqlmsg="select RecFilePath from tb_recordinfo where RecStartTime>='$StartDate $StartTime' and RecStartTime<='$EndDate $EndTime' and ChID>=$StartChnl and ChID<=$EndChnl limit 100;";
	
		/* query 100 records,then write to file,then delete file and update tb_recordinfo */
		$TmpFile="tmpdelfile";
		$cnt=0;
		do{
			$cnt=0;
			$sql=mysql_query($sqlmsg);
			$fp=fopen($TmpFile,'w+');
			while($info=mysql_fetch_array($sql)){
				fputs($fp,$info[RecFilePath]."\r\n");
				$cnt++;	
			}
			fclose($fp);
			
			$fp=fopen($TmpFile,'r');
			while($delfielname=fgets($fp,512)){
				if ($this->debug) echo "del $delfielname<br>";
				
				$tmpname=strstr($delfielname,"\r\n",TRUE);
				unlink($tmpname);
				mysql_query("delete from tb_recordinfo where RecFilePath='$tmpname'");
				mysql_query("delete from tb_recordinfow where RecFilePath='$tmpname'");			
			}
			fclose($fp);
		}while($cnt>=100);
		unlink($TmpFile);
		
		return 0;
	}
	
	
	public function SetAdminPassword($OldPassword,$NewPassword)
	{
		if ($OldPassword=="" || $NewPassword=="")
			return FALSE;
		if (strlen($OldPassword)>20)
			return FALSE;
		if (strlen($NewPassword)>20)
			return FALSE;
			
		if (passwordValied($OldPassword)!=1)
			return FALSE;
		if (passwordValied($NewPassword)!=1)
			return FALSE;
		$this->SqlConnect();
		$sql=mysql_query("select Pwd from tb_webuserinfo where UserName ='admin';");
		$info=mysql_fetch_array($sql);
		if ($info[Pwd]!=$OldPassword)
			return FALSE;
		$result=mysql_query("update tb_webuserinfo set Pwd='$NewPassword' where UserName='admin'");
		return $result;
	}
	
	public function QueryRecord($StartDate,$StartTime,$EndDate,$EndTime,$StartChnl,$EndChnl,$startindex)//这个是按日期查询
	{
		$sqlret = new SqlResult();
		$sqlret->sqlinfo="";
		$sqlret->result=-1;
		
		if ($startindex<0)
			return $sqlret;
		
		if ((IsDateFormat($StartDate)!=TRUE) || (IsDateFormat($EndDate)!=TRUE))
			return $sqlret;
		$result=comparedate($StartDate, $EndDate);
		if ($result==1)
			return $sqlret;
		if ((IsTimeFormat($StartTime)!=TRUE) || (IsTimeFormat($EndTime)!=TRUE))
			return $sqlret;
		if ($result==0){
			if (comparetime($StartTime, $EndTime)==1)
				return $sqlret;
		}
		
		$settings = new Settings_INI;
		$settings->load('recsvr/Recorder.ini');
		$settings1 = new Settings_INI;
		$settings1->load('recsvr/ShConfig.ini');
		$maxch=$settings1->get('AppChToBoardChTable.TotalAppCh');
		$startCh=$settings->get('RecSvr.StartChannel');
		
		if($StartChnl<$startCh || $StartChnl >=$startCh+$maxch)
			return $sqlret;
			
		if($EndChnl<$startCh || $EndChnl >=$startCh+$maxch)
			return $sqlret;
		
		if ($StartChnl>$EndChnl)
			return $sqlret;
		
		$StartChnl=$StartChnl-$startCh;
		$EndChnl=$EndChnl-$startCh;
		$this->SqlConnect();
		$sqlmsg="select RecFilePath from tb_recordinfo where RecStartTime>='$StartDate $StartTime' and RecStartTime<='$EndDate $EndTime' and ChID>=$StartChnl and ChID<=$EndChnl limit $startindex,100";
		$this->resource=mysql_query($sqlmsg);
		
		$retmsg="";
		while($info=mysql_fetch_array($this->resource))
		{
			$retmsg=$retmsg.$info[RecFilePath].";";
		}	
		$sqlret->result=0;
		$sqlret->sqlinfo=$retmsg;
		return $sqlret;
	}
	
	public function ExecuteSQL($SqlQuerymsg)//这个就相当于mysql查询接口
	{
		$sqlret = new SqlResult();
		$sqlret->sqlinfo="";
		$sqlret->result=-1;
		
		$this->SqlConnect();
		$result=mysql_query($SqlQuerymsg);

		if (is_resource($result)==FALSE){
			if ($result==TRUE)
				$sqlret->result=0;
			return $sqlret;
		}
		
		/* 瀵逛簬resouce璧勬簮瑕佽繘琛岄亶鍘?*/
		$fieldsnum=mysql_num_fields($result);
		$sqlinfo="";
		 while($info=mysql_fetch_array($result)) {
        	for($i=0;$i<$fieldsnum;$i++)
        	{
        		if ($i==0){
        			$sqlinfo=$sqlinfo."$info[$i]";
        		}
        		else {
        			$sqlinfo=$sqlinfo.",$info[$i]";
        		}
        	}
        	$sqlinfo=$sqlinfo.";";
    	}
    	$sqlret->sqlinfo=$sqlinfo;
		$sqlret->result=0;
    	return $sqlret;
	}
}

/*
$WebServiceApi = new CfnXwebApi();
$WebServiceApi->debug=1;
$WebServiceApi->GetGlobalCfg();

$WebServiceApi->GetChannelCfg(2);
$channelcfg = new ChannelCfg();
$channelcfg->iChID=2;
$channelcfg->iMaxReclen=60;
$channelcfg->iLongRecInterval=30;
$channelcfg->iRecDir=2;
$channelcfg->iRecType=2;
$channelcfg->iRecvDtmfTime=0;
$channelcfg->szBindPhoneNum="123456";
$channelcfg->szStartDtmf="6868*#";
$channelcfg->szStopDtmf="2525#*";
$WebServiceApi->SetChannelCfg($channelcfg);

$calInput = new CalculateInput();
$calInput->SelectType=0;
$calInput->start_year=1999;
$calInput->start_month=1;
$calInput->start_day=1;
$calInput->end_year=2015;
$calInput->end_month=8;
$calInput->end_day=1;
$calInput->monthwise_year=2015;
$calInput->monthwise_month=7;
$WebServiceApi->CalculateData($calInput);*/
//$WebServiceApi->WriteDVD();
//$WebServiceApi->DelRecordFile("2015-05-01", "00:00:00", "2015-06-01", "00:00:00", 10, 25);
/*
$monitor=new MonitorInfo();
$monitor->ChID=1;
$monitor->szRemoteIP="201.123.115.52";
$WebServiceApi->StartMonitor($monitorInfo);*/
/* api example */
/*
$WebServiceApi = new synapi();
$WebServiceApi->GetHardwareInfo();

$result = $WebServiceApi->QueryRecord("2015-06-01", "00:20:00", "2015-06-12", "00:00:00", 0, 15);
if ($result==1)
while ($WebServiceApi->QueryRecordResult()!="");

$ret=$WebServiceApi->ExecuteSQL("insert into tb_recordinfo (id,RecFilePath) values ('15','/mnt/hd/hd1/Record20150612/wangyanfeng.wav')");
$WebServiceApi->GetDevID();

$WebServiceApi->DeleteFile("/mnt/hd/hd1/temp/16.vox");

$WebServiceApi->DelRecordFile("2015-06-01", "00:20:00", "2015-06-12", "23:59:00", 0, 15);

$WebServiceApi->SetAdminPassword('wyf','admin');



//$WebServiceApi->CheckMediaAvailable();

//$WebServiceApi->CheckMediaConnected();

$calInput = new CalculateInput();
$calInput->SelectType=1;
$calInput->start_year=2015;
$calInput->start_month=1;
$calInput->start_day=1;
$calInput->end_year=2015;
$calInput->end_month=7;
$calInput->end_day=10;
$calInput->monthwise_year=2015;
$calInput->monthwise_month=5;
$WebServiceApi->CalculateData($calInput);

$WebServiceApi->WriteDVD();
//$WebServiceApi->GetChannelStatus();

//$WebServiceApi->GetChannelRunningStatus(0);

//$WebServiceApi->GetGlobalCfg();
*/
/*
$globalset=new GlobalCfg();
$globalset->icompress=7;
$globalset->ifileformat=1;
$globalset->istartch=10;
$ret=$WebServiceApi->SetGlobalCfg($globalset);
echo "setglobalcfg result is $ret<br>";
*/
//$WebServiceApi->GetChannelCfg(11);

/*
$channelcfg = new ChannelCfg();
$channelcfg->iChID=11;
$channelcfg->iMaxReclen=900;
$channelcfg->iRecDir=2;
$channelcfg->iRecType=0;
$channelcfg->iRecvDtmfTime=10;
$channelcfg->szCtlDtmf="2356";
$WebServiceApi->SetChannelCfg($channelcfg);
*/
//$channelcfg= new ChannelCfg();
//$channelcfg->iChID=10;
//$WebServiceApi->SetChannelCfg($channelcfg);

//$WebServiceApi->SetDateTime("2015-05-23 08:20:20");

//$WebServiceApi->GetDeviceTime();

/*
$netcfg=new NetCfg();
$netcfg->szIPAddr="201.123.115.7";
$netcfg->szGateway="201.123.115.254";
$netcfg->szMask="255.255.255.0";
$netcfg->szDns="192.168.1.100";
$WebServiceApi->SetNetCfg($netcfg);
*/

//$WebServiceApi->RebootDevice();

//$WebServiceApi->GetLineVol();
//$WebServiceApi->GetDiskFreeSpace();
/*
//$monitor=new MonitorInfo();
//$monitor->ChID=11;
//$monitor->szRemoteIP="201.123.115.52";
//$WebServiceApi->StartMonitor($monitor);
//$WebServiceApi->StopMonitor($monitor);


*/
?>