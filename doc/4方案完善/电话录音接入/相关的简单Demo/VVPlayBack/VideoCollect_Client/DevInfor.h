#ifndef GENERALDEF_H
#define GENERALDEF_H

#include "HCNetSDK.h"

#define MAX_CHANS      24              //�豸֧�ֵ����ͨ����
#define MAX_DEVICE     10             //����豸��
#define MAX_OUTPUTS	   16	//16���Ĵ�����
#define MAX_PREVIEWS		512	//��Ԥ���������
//�豸��������
#define TREE_ALL_T	 0        //������ڵ�
#define DEVICETYPE 	 1		  //�����豸
#define CHANNELTYPE	 2	      //����ͨ��

#define DEMO_FULL_WIDTH 1024	//demoĬ�ϵĿ��
#define DEMO_FULL_HEIGHT 768	//demoĬ�ϵĸ߶�
#define OUTPUT_INTERVAL 4		//ÿ������ļ����С


typedef struct STRU_CHANNEL_INFO
{
    char    chChanName[40];     //ͨ������
    int		iChanIndex;			//���ͨ���� = ��������+startchan
    int		iPicResolution;				//ͼƬ�ֱ���
    int		iPicQuality;				//ͼƬ����
    char	chAccessChanIP[16];     //ip����ͨ����ip��ַ
    BOOL    bEnable;              //�Ƿ���Ч
    NET_DVR_DECODERCFG_V30  struDecodercfg;   //ͨ���Ľ�������Ϣ
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
    int		iDeviceChanNum;		    //�豸��ͨ����
    int		iStartChan;				//�豸��ʼͨ����
    int 	iIPChanNum;				//�������ͨ������
    int 	iIPStartChan;			//����ͨ����ʼͨ����
    int		iEnableChanNum;			//��Чͨ����
    BOOL    bIPRet;                 //�Ƿ�֧��ip����

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



//�����ǻطŵĶ�����ز���

         //��ʱ��ʱ��
#define CYCLE_PREVIEW_TIMER        2    //ѭ��Ԥ����ʱ��
#define REMOTE_PLAY_STATE_TIMER	   8		//Զ�̻ط��ļ�����״̬ʱ��
#define DOWNSTATE_TIMER		       9	 //�����ļ�ʱ��
#define LOCAL_PLAY_STATE_TIMER     10
#define PLAYBYTIME_TIMER	       12		// ��ʱ��ط�
#define DOWNBYTIME_TIMER		   13		//��ʱ������

//Զ���ļ����ҷ���ֵ
#define NET_DVR_FILE_SUCCESS    1000
#define NET_DVR_FILE_NOFIND     1001
#define NET_DVR_ISFINDING       1002
#define NET_DVR_NOMOREFILE      1003
#define NET_DVR_FILE_EXCEPTION  1004

#endif