#ifndef GENERALDEF_H
#define GENERALDEF_H

#include "HCNetSDK.h"

#define MAX_CHANS      24              //设备支持的最大通道数
#define MAX_DEVICE     10             //最大设备数
#define MAX_OUTPUTS	   16	//16最大的窗口数
#define MAX_PREVIEWS		512	//可预览的最大数
//设备区域配置
#define TREE_ALL_T	 0        //代表根节点
#define DEVICETYPE 	 1		  //代表设备
#define CHANNELTYPE	 2	      //代表通道

#define DEMO_FULL_WIDTH 1024	//demo默认的宽度
#define DEMO_FULL_HEIGHT 768	//demo默认的高度
#define OUTPUT_INTERVAL 4		//每个画面的间隔大小


typedef struct STRU_CHANNEL_INFO
{
    char    chChanName[40];     //通道名称
    int		iChanIndex;			//监控通道号 = 数组索引+startchan
    int		iPicResolution;				//图片分辨率
    int		iPicQuality;				//图片质量
    char	chAccessChanIP[16];     //ip接入通道的ip地址
    BOOL    bEnable;              //是否有效
    NET_DVR_DECODERCFG_V30  struDecodercfg;   //通道的解码器信息
    STRU_CHANNEL_INFO()
    {
        chChanName[0]       ='\0';
        iChanIndex          = -1;
        iPicResolution		= 0;
        iPicQuality			= 2;
        chAccessChanIP[0]	= '\0';	
        bEnable             = FALSE;
    }
}CHANNEL_INFO,*pCHANNEL_INFO;

typedef struct STRU_DEVICE_INFO
{
    LONG    lLoginID;
    int		iDeviceChanNum;		    //设备的通道数
    int		iStartChan;				//设备开始通道号
    int 	iIPChanNum;				//最大数字通道个数
    int 	iIPStartChan;			//数字通道起始通道号
    int		iEnableChanNum;			//有效通道数
    BOOL    bIPRet;                 //是否支持ip接入

    CHANNEL_INFO struChanInfo[MAX_CHANNUM_V30];

    STRU_DEVICE_INFO()
    {
        lLoginID = -1;
        iDeviceChanNum      =  -1;
        iStartChan          = 0;
        iIPChanNum          = 0;
        iEnableChanNum      = -1;
        bIPRet              = FALSE;
    }	 
}LOCAL_DEVICE_INFO,*pLOCAL_DEVICE_INFO;



//下面是回放的额外相关参数

         //定时器时间
#define CYCLE_PREVIEW_TIMER        2    //循环预览定时器
#define REMOTE_PLAY_STATE_TIMER	   8		//远程回放文件播放状态时钟
#define DOWNSTATE_TIMER		       9	 //下载文件时钟
#define LOCAL_PLAY_STATE_TIMER     10
#define PLAYBYTIME_TIMER	       12		// 按时间回放
#define DOWNBYTIME_TIMER		   13		//按时间下载

//远程文件查找返回值
#define NET_DVR_FILE_SUCCESS    1000
#define NET_DVR_FILE_NOFIND     1001
#define NET_DVR_ISFINDING       1002
#define NET_DVR_NOMOREFILE      1003
#define NET_DVR_FILE_EXCEPTION  1004

#endif