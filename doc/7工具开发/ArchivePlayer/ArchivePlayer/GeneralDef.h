
#ifndef GENERALDEF_H
#define GENERALDEF_H
#include <string>
#include <vector>
#include <map>
#include "coding.h"
#include "hak.h"
#include "HCNetSDK.h"

#define ITEM_SIZE 64
#define TIME_SIZE 64
#define PASSWORD_SIZE 64
#define ERROR_SIZE 256
#define BUFFERSIZE 512

#define DEVICE_TYPE_CVR   50011
#define DEVICE_TYPE_CVR2_0   50001
#define DEVICE_TYPE_CLOUD 50015
#define DEVICE_TYPE_NVR   50012
#define DEVICE_TYPE_DVR   50000

#define  ITEM_RECORD_REMOTE 15
#define  ITEM_RECORD_LOCAL  14


#define BUSINESS_TYPE_SIGNED_1    _T("签约")
#define BUSINESS_TYPE_COMPLAIN_2  _T("投诉")
#define RECORD_TYPE_NORMAL_1        _T("正常录像")
#define RECORD_TYPE_RECORD_2        _T("补录录像")

#define RECORD_STAUS_UPLOAD_0     _T("未上传")
#define RECORD_STAUS_UPLOADED_1   _T("已上传")
#define RECORD_STAUS_UPLOADING_3   _T("上传中")

#define RECORD_STAUS_UNUPLOAD    0
#define RECORD_STAUS_UPLOADED    1

#define TABLE_CLIENTINPUT      0
#define TABLE_PROSAVERECORD    1
#define TABLE_RECORD    2
#define TABLE_DEVICE    3

#define  ERR_ITEM_INDEX 9


//录像类型，1-正常录像，2-补录录像
//录像是否上传 0-未上传 1-未上传


struct Iteminfo
{
    int type;
    std::wstring Dbname;
    std::wstring listname;

};
extern std::vector<Iteminfo> Itemlist;

struct  CHANNEL_INFO
{
    char    chChanName[ITEM_SIZE];     //通道名称
    int		iChanIndex;			//监控通道号 = 数组索引+startchan
    BOOL    bEnable;              //是否有效
    BOOL    iRecordPlan;
    NET_DVR_RECORD_V40  recordPlan;
    CHANNEL_INFO()
    {
        chChanName[0]       ='\0';
        iChanIndex          = -1;
        bEnable             = FALSE;
        iRecordPlan         = FALSE;
    }
};

struct DevicInfo
{
    LONG iLoginId;       //登录返回号
    int type;
    char name[ITEM_SIZE];//设备名称
    char ip[ITEM_SIZE];//设备ip
    int port;// 设备端口
    char user[ITEM_SIZE];//设备用户名
    char password[PASSWORD_SIZE];//设备密码
    NET_DVR_NTPPARA ntpInfo;        //NTP校时信息 
    int		iDeviceChanNum;		    //设备的通道数
    int		iStartChan;				//设备开始通道号
    int 	iIPChanNum;				//最大数字通道个数
    int 	iIPStartChan;			//数字通道起始通道号
    int		iEnableChanNum;			//有效通道数
    BOOL bpwdEnable;		        //加解密是否有效  
    BOOL errorno;		            //加解密是否有效  
    BOOL beOpenNTP;		           //是否开启NTP  
    std::string errMsg; 
    std::vector<CHANNEL_INFO> chanList;
    DevicInfo()
    {
        iLoginId            = 0;
        type                = 0;
        name[0]             ='\0';//设备名称
        ip[0]               ='\0';//设备ip
        port                = 0;// 设备端口
        user[0]             ='\0';//设备用户名
        password[0]         ='\0';//设备密码
        iDeviceChanNum      = -1;
        iStartChan          = 0;
        iIPChanNum          = 0;
        iEnableChanNum      = -1;
        bpwdEnable          = TRUE;  
        errorno		        = 0;
        beOpenNTP           = FALSE;
        errMsg              = "";
        chanList.clear();
       
    }
    
};
extern std::vector<DevicInfo> g_Devlist;


struct RecordInfo
{
    int Businessid ;        //业务单id
    char name[ITEM_SIZE];//录像名称
    int  type;//录像类型，1-正常录像，2-补录录像
    int status;//录像是否上传 0-未上传 1-已上传
    char startTime[ITEM_SIZE];//开始时间
    char endTime[ITEM_SIZE];//结束时间
    char streamNo[ITEM_SIZE];//结束时间
    int  cvrid;//存储id

    int devid;//设备id
    int channel;//设备通道
    DevicInfo store;
    DevicInfo camera;
    BOOL bCameraExist; ////前端设备 存在标志
    BOOL bStoreExist; //存储中心 存在标志
    char errMsg[ERROR_SIZE];//异常信息
    char reserve[ITEM_SIZE];//临时存放业务单id

    
};



struct BusinessInfo
{
    int id;                   //数据库里的id号
    char number[ITEM_SIZE];  //业务单号
    int  status;    //业务单状态：0-签约，1-投诉
    char regionName[ITEM_SIZE];//交易网点 
    char productTypeName[ITEM_SIZE];//产品类型名称
    char productName[ITEM_SIZE];//产品名称  
    char proName[ITEM_SIZE];  //业务员姓名
    char inputTime[ITEM_SIZE];//录入时间
    //RecordInfo record;
}; 


struct filtrate
{
    int type;
    std::string condition;
    std::string data;
    filtrate()
    {
        type      = -1;
        condition = "";
        data      = "";
    }
}; 

extern std::vector<filtrate> g_filter;
extern std::map<int,BusinessInfo> BusinessInfolist;
extern std::vector<RecordInfo> RecordInfoList;

#endif
