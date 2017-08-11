
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


#define BUSINESS_TYPE_SIGNED_1    _T("ǩԼ")
#define BUSINESS_TYPE_COMPLAIN_2  _T("Ͷ��")
#define RECORD_TYPE_NORMAL_1        _T("����¼��")
#define RECORD_TYPE_RECORD_2        _T("��¼¼��")

#define RECORD_STAUS_UPLOAD_0     _T("δ�ϴ�")
#define RECORD_STAUS_UPLOADED_1   _T("���ϴ�")
#define RECORD_STAUS_UPLOADING_3   _T("�ϴ���")

#define RECORD_STAUS_UNUPLOAD    0
#define RECORD_STAUS_UPLOADED    1

#define TABLE_CLIENTINPUT      0
#define TABLE_PROSAVERECORD    1
#define TABLE_RECORD    2
#define TABLE_DEVICE    3

#define  ERR_ITEM_INDEX 9


//¼�����ͣ�1-����¼��2-��¼¼��
//¼���Ƿ��ϴ� 0-δ�ϴ� 1-δ�ϴ�


struct Iteminfo
{
    int type;
    std::wstring Dbname;
    std::wstring listname;

};
extern std::vector<Iteminfo> Itemlist;

struct  CHANNEL_INFO
{
    char    chChanName[ITEM_SIZE];     //ͨ������
    int		iChanIndex;			//���ͨ���� = ��������+startchan
    BOOL    bEnable;              //�Ƿ���Ч
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
    LONG iLoginId;       //��¼���غ�
    int type;
    char name[ITEM_SIZE];//�豸����
    char ip[ITEM_SIZE];//�豸ip
    int port;// �豸�˿�
    char user[ITEM_SIZE];//�豸�û���
    char password[PASSWORD_SIZE];//�豸����
    NET_DVR_NTPPARA ntpInfo;        //NTPУʱ��Ϣ 
    int		iDeviceChanNum;		    //�豸��ͨ����
    int		iStartChan;				//�豸��ʼͨ����
    int 	iIPChanNum;				//�������ͨ������
    int 	iIPStartChan;			//����ͨ����ʼͨ����
    int		iEnableChanNum;			//��Чͨ����
    BOOL bpwdEnable;		        //�ӽ����Ƿ���Ч  
    BOOL errorno;		            //�ӽ����Ƿ���Ч  
    BOOL beOpenNTP;		           //�Ƿ���NTP  
    std::string errMsg; 
    std::vector<CHANNEL_INFO> chanList;
    DevicInfo()
    {
        iLoginId            = 0;
        type                = 0;
        name[0]             ='\0';//�豸����
        ip[0]               ='\0';//�豸ip
        port                = 0;// �豸�˿�
        user[0]             ='\0';//�豸�û���
        password[0]         ='\0';//�豸����
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
    int Businessid ;        //ҵ��id
    char name[ITEM_SIZE];//¼������
    int  type;//¼�����ͣ�1-����¼��2-��¼¼��
    int status;//¼���Ƿ��ϴ� 0-δ�ϴ� 1-���ϴ�
    char startTime[ITEM_SIZE];//��ʼʱ��
    char endTime[ITEM_SIZE];//����ʱ��
    char streamNo[ITEM_SIZE];//����ʱ��
    int  cvrid;//�洢id

    int devid;//�豸id
    int channel;//�豸ͨ��
    DevicInfo store;
    DevicInfo camera;
    BOOL bCameraExist; ////ǰ���豸 ���ڱ�־
    BOOL bStoreExist; //�洢���� ���ڱ�־
    char errMsg[ERROR_SIZE];//�쳣��Ϣ
    char reserve[ITEM_SIZE];//��ʱ���ҵ��id

    
};



struct BusinessInfo
{
    int id;                   //���ݿ����id��
    char number[ITEM_SIZE];  //ҵ�񵥺�
    int  status;    //ҵ��״̬��0-ǩԼ��1-Ͷ��
    char regionName[ITEM_SIZE];//�������� 
    char productTypeName[ITEM_SIZE];//��Ʒ��������
    char productName[ITEM_SIZE];//��Ʒ����  
    char proName[ITEM_SIZE];  //ҵ��Ա����
    char inputTime[ITEM_SIZE];//¼��ʱ��
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
