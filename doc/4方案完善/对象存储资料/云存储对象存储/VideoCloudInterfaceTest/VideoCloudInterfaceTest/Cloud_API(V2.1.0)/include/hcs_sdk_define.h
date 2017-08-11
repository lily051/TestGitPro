/** @file   hcs_sdk_define.h
 *  @note   Hangzhou Hikvision System Technology Co., Ltd. All Rights Reserved.
 *  @brief  
 *
 *  @author Jin Shengpeng
 *  @date   2015/01/28
 *
 *  @note   ��ʷ��¼��
 *  @note   V1.0.0.0 ����
 */

#ifndef HCS_SDK_DEFINE_H_
#define HCS_SDK_DEFINE_H_

#if (defined(_WIN32) || defined(_WIN64))
#  if defined(HCS_SDK_EXPORTS)
#    define HCS_SDK_EXTERN extern "C" __declspec(dllexport)
#  else
#    define HCS_SDK_EXTERN extern "C" __declspec(dllimport)
#  endif
#  define HCS_SDK_API
#elif defined(__linux__)
#  define HCS_SDK_EXTERN extern "C"
#  define HCS_SDK_API
#else
#  define HCS_SDK_EXTERN
#  define HCS_SDK_API
#endif


#define HCS_OK									0		// �ɹ�
#define HCS_ERROR								1		// ʧ��
#define HCS_DEVICE_IP_LEN						256		// ����ǰ���豸��IP��ַ��Ϣ����
#define HCS_IP_LEN								40		// ����IP��ַ��Ϣ����
#define HCS_FILE_KEY_LEN						260		// ��Ƶ�ļ�key����
#define HCS_RESERVED_DATA_LEN					64		// ����������󳤶�
#define HCS_CAMERA_ID_LEN						64		// ������ID����
#define HCS_RECORD_TYPE_LEN						32		// ����¼�����ͳ���
#define HCS_CAMERA_NAME_LEN						64		// ���ı��������Ƴ���
#define HCS_DEV_NAME_LEN						64		// �豸������󳤶�
#define HCS_USERNAME_LEN						32		// �û�������󳤶�
#define HCS_POOLUSER_COUNT						60		// ��Դ�ذ󶨵�����û�����
#define HCS_PASSWORD_LEN						32		// �������󳤶�
#define HCS_DAYS_PER_WEEK						7		// �ƴ洢¼��ƻ�֧�ֵ��������
#define HCS_PLAN_SEGMENT_COUNT					8		// �ƴ洢¼��ƻ�֧�ֵ�������
#define HCS_STREAM_URL_LEN						512		// ���ȡ��URL�ĳ���
#define HCS_POOL_ID_LEN							32		// �����Դ��ID����
#define HCS_POOL_NAME_LEN						64		// �����Դ��������
#define HCS_STORDEV_ID_LEN						64		// �洢�豸ID����
#define HCS_MAX_ABILITY							100		// �����������Ŀ
#define HCS_MAX_REPLICATION_NUM					2		// ���������Ŀ
#define HCS_IP_PORT_LEN							64		// IP��ַ�Ӷ˿ں���󳤶�
#define HCS_DESC_LEN							16		// �����ƴ洢֧�ֵ��豸����������Ϣ��󳤶�
#define HCS_SUP_DEV_NUM							500		// �����ƴ洢֧���豸�����������
#define HCS_HEAD_LEN							64		// �����Ƶͷ����
#define HCS_STREAM_INFO_SIZE					1472	// ȡ����ϢUNION����
#define HCS_PICTURE_URL_LEN						256		// ����ͼƬURL����
#define HCS_DATA_HOURS							24		// һ��24Сʱ
#define HCS_MONTH_DAYS							31		// һ�������31��
#define HCS_TAG_LEN								128		// ��ע��󳤶�
#define HCS_MAX_RECPLAN_ABILITY					8		// ¼��ƻ������������Ŀ
#define HCS_MAX_FILE_URL_LEN					1024    // �����Ƶ�ļ�URL����

#define HCS_MAX_VIDEO_KEY_LEN					1024	// ��������ȡkey����


/** @ enum  [HCS_AUX_SERVICE]
 *  @ brief �豸����ʱʹ�õĸ��ӷ�������ý�塢VAG��
 */
typedef enum
{
	VTDU	= 0,										// ��ý�����
	VAG		= 1											// �豸�������

}HCS_AUX_SERVICE;


/** @ enum  [HCS_RecDataType]
 *  @ brief ¼����������
 */
typedef enum
{
	HCS_RECDATA_REALTIME	= 1,						// ʵʱ��
	HCS_RECDATA_HISTORICAL	= 2							// ��ʷ��

}HCS_RecDataType;

/** @ enum  [HCS_DataType]
 *  @ brief ��������
 */
typedef enum
{
	HCS_DATATYPE_RECORD	= 1,							// ¼������
	HCS_DATATYPE_STREAMFILE	= 2							// ��Ƶ�ļ�����

}HCS_DataType;


/** @ enum  [HCS_IFrameType]
 *  @ brief I֡����
 */
typedef enum 
{
	HCS_IFRAME_REAL	= 0,								// ��ʵI֡
	HCS_IFRAME_FAKE	= 1,								// �Զ���I֡
	HCS_IFRAME_SMART264	= 2								// smart 264��

}HCS_IFrameType;


/** @ enum  [HCS_FrameType]
 *  @ brief ֡����
 */
typedef enum
{
	HCS_FRAME_TYPE_NORMAL = 1,							// ��ͨ֡
	HCS_FRAME_TYPE_REFRESH = 2,							// ˢ��֡
	HCS_FRAME_TYPE_IFRAME = 3							// �ؼ�֡(����֡)

}HCS_FrameType;


/** @ enum  [HCS_PlaybackMode]
 *  @ brief �ط�ģʽ
 */
typedef enum
{
	HCS_PBMODE_NORMAL	= 1,							// ˳��ط�
	HCS_PBMODE_IFRAME	= 2,							// ����I֡�ط�
	HCS_PBMODE_REVERSE	= 3								// ����ط�

}HCS_PlaybackMode;


/** @ enum  [HCS_SRLockType]
 *  @ brief ��������
 */
typedef enum
{
	HCS_SRLOCK_TYPE_All      = 0,						// ȫ��������������¼���û��������¼��
	HCS_SRLOCK_TYPE_UNLOCKED = 1,						// û������(���ԶԵ�ǰ¼��ִ�����в���)
	HCS_SRLOCK_TYPE_LOCKED   = 2						// ����(��ǰ¼�������ǣ������Իطš����غ�ɾ������)

}HCS_SRLockType;


/** @ enum  [HCS_PicType]
 *  @ brief ͼƬ����
 */
typedef enum
{
	HCS_PICTYPE_JPG = 1,								// JPGͼƬ
	HCS_PICTYPE_BMP = 2,								// BMPͼƬ
	HCS_PICTYPE_PNG = 3									// PNGͼƬ

}HCS_PicType;


/** @ enum  [HCS_PicZoomType]
 *  @ brief ͼƬ��������
 */
typedef enum
{
	HCS_PICZOOMTYPE_NONE	= 0,						// ������
	HCS_PICZOOMTYPE_BYSCALE = 1,						// ����������
	HCS_PICZOOMTYPE_BYSIZE  = 2,						// ����С����
	HCS_PICZOOMTYPE_BYLW	= 3							// ����������

}HCS_PicZoomType;

/* @ struct [HCS_Time]
 * @ brief  ʱ��ṹ����
 */
typedef struct
{
	unsigned int year;									// ��
	unsigned int month;									// ��
	unsigned int day;									// ��
	unsigned int hour;									// ʱ
	unsigned int minute;								// ��
	unsigned int second;								// ��
	unsigned int millisecond;							// ����
	char		 reserved[8];							// �����ֽ�

}HCS_Time;

/** @ struct [HCS_Version]
 *  @ brief �ƴ洢�汾��Ϣ
 */
typedef struct 
{
	int		major_version;								// ���汾��
	int		sub_version;								// �Ӱ汾��
	int		minor_version;								// �ΰ汾��

}HCS_Version;


/** @ struct [HCS_PoolInfo]
 *  @ brief  ��Դ����Ϣ
 */
typedef struct
{
	char				pool_id[HCS_POOL_ID_LEN];		// ��Դ��ID	
	char				pool_name[HCS_POOL_NAME_LEN];	// ��Դ������[��֧������]
	unsigned char		pool_type;						// ��Դ������[0-ͨ�ó�  1-��Ƶ��  3-ͼƬ��  4-�ļ���]
	int					pool_cycle;						// ��Դ�ظ�������[��λ����]
	unsigned char		disp_policy;					// ��ɢ����[0-����  1-��ɢ]
	unsigned char		cov_policy;						// ���ǲ���[0-������  1-����������  2-���ڸ���]
	unsigned char		lock_limit;						// ��ǰ��Դ��֧�ֵ�������������ٷֱ�[0% - 30%]
	unsigned long long	pool_size;						// ��Դ�ش�С[��λ: MB]
	unsigned char		trans_type;						// ת������0��ת�룬1����֡ת��	
	unsigned int		trans_cycle;					// ԭʼ�������ڣ���λ����
	unsigned char		pool_status;					// ��Դ��״̬1������2������3����������4����ǰ����
	unsigned long long	free_size;						// ��Դ��ʣ��ռ�[��λ: MB]
	HCS_Time			begin_time;						// д�����ݵĿ�ʼʱ��[��һ��д�����ݵ�ʱ��]
	HCS_Time			cov_time;						// �������ݸ��ǵ�ʱ���
	HCS_Time			trans_time;						// ת��ʱ���
	unsigned int		domain_id;						// ��ID;��Դ��������ID
	int					user_count;						// ���û�����
	char				username[HCS_POOLUSER_COUNT][HCS_USERNAME_LEN];			// �󶨵��û���,δ����ʱ����

}HCS_PoolInfo;


/** @struct [HCS_SupDevInfo]
 *  @brief �豸�������ݽṹ����
 */
typedef struct 
{
	int		dev_type;									// �豸����
	char	dev_desc[HCS_DESC_LEN];						// �豸����������Ϣ

}HCS_SupDevInfo;

/** @ struct [HCS_Ability]
 *  @ brief  �ƴ洢������
 */
typedef struct 
{
	int				dev_count;							// �ƴ洢֧�ֵ��豸����
	HCS_SupDevInfo	dev_list[HCS_SUP_DEV_NUM];			// �ƴ洢֧�ֵ��豸�б�
	char			res[4];								// �����ֶΣ���Ϊ0 

}HCS_Ability;

/** @ struct [HCS_RecordInfo]
 *  @ brief  ¼����Ϣ
 */
typedef struct
{
	char				camera_id[HCS_CAMERA_ID_LEN];	// ������ID
	HCS_Time			begin_time;						// ¼��ʼʱ��
	HCS_Time			end_time;						// ¼�����ʱ��
	unsigned char		type;							// ��������1-¼��  2-��Ƶ�ļ�
	unsigned short      record_type;					// ¼������
	unsigned char		lock;							// ¼������״̬[0: ȫ������(����������¼���û��������¼��)��1: û��������2: ����]
	unsigned char		trans_type;						// ת������[0: ԭʼ��1: I֡]
	unsigned int		size;							// ¼���С[��λ: �ֽ�]
	unsigned int		unit_count;						// ���ڵ�Ԫ����ͼƬ����I֡���͸�������
	char				pool_id[HCS_POOL_ID_LEN];		// ��Դ��ID 
	char				vs_ip[HCS_IP_PORT_LEN];			// �洢������IP�˿� ��ʽ��CVSIP:PORT1:PORT2:PORT3:PORT4
	char				device_id[HCS_STORDEV_ID_LEN];	// �洢�豸ID
	unsigned int		block_id;						// �洢��ID
	unsigned int		segment_id;						// �洢��ID
	unsigned char		status;							// ��ǰ¼�����״̬[0: ������1: VS�����ߡ�2: �豸�����á�3: �豸������]
	char				reserved[HCS_RESERVED_DATA_LEN];// �����ֶ�

}HCS_RecordInfo;

/** @ struct [HCS_AccessInfo]
 *  @ brief  ¼��ƻ�������Ϣ
 */
typedef struct 
{
	int type;											// ¼��ƻ����ͣ�1-ʵʱ��¼��ƻ�,2-��ʷ��¼��ƻ�
	int max_access_count;								// �����������ƴ洢���Ƶ����¼��ƻ�������
	int current_access_count;							// ��ǰ���������ƴ洢��ǰʵ�ʽ����¼��ƻ���
	int current_enable_access_count;					// ��ǰ�ɽ��������ƴ洢��ǰʵ�ʿɽ����¼��ƻ���
	int res[4];											// ����

}HCS_AccessInfo;

/** @ struct [HCS_RecPlanAbility]
 *  @ brief  ¼��ƻ�����������
 */
typedef struct 
{
	int				count;								// ¼��ƻ���������������Ĭ��֧��2����ʵʱ��¼��ƻ�����ʷ��¼��ƻ�
	HCS_AccessInfo  accss_info[HCS_MAX_RECPLAN_ABILITY];// ¼��ƻ�������Ϣ

}HCS_RecPlanAbility;

/** @ struct [HCS_Period]
 *  @ brief  ʱ�����Ϣ
 */
typedef struct 
{
	char ip[HCS_IP_LEN];								// IP��ַ
	int  online;										// ����״̬��0�����ߣ�1-����

}HCS_VtduInfo;

/** @ struct [HCS_Period]
 *  @ brief  ʱ�����Ϣ
 */
typedef struct 
{
	int start_hour;										// ��ʼʱ��Сʱ��
	int start_min;										// ��ʼʱ�������
	int stop_hour;										// ����ʱ��Сʱ��
	int stop_min;										// ����ʱ�������

}HCS_Period;

/** @ struct [HCS_Endpoint]
 *  @ brief  �����Ͽ����ӵĶ˵�
 */
typedef struct 
{
	char	ip[HCS_DEVICE_IP_LEN];						// IP��ַ
	int		port;										// �˿�
	char	username[HCS_USERNAME_LEN];					// �û���
	char	password[HCS_PASSWORD_LEN];					// ����

}HCS_Endpoint;

/** @ struct [HCS_DevInfo]
 *  @ brief  ¼��ƻ�ִ��ʱ����Ϣ
 */
typedef struct 
{
	char			name[HCS_DEV_NAME_LEN];				// �豸����
	HCS_Endpoint	endpoint;							// �豸������Ϣ
	unsigned int	protocol;							// ����Э��
	char			reserved[8];

}HCS_DevInfo;

/** @ struct [HCS_AuxServiceInfo]
 *  @ brief  ¼��ƻ�ִ��ʱ���ӷ�����Ϣ
 */
typedef struct 
{
	HCS_AUX_SERVICE type;								// ��������
	HCS_Endpoint	endpoint;							// ���������Ϣ
	char			reserved[8];

}HCS_AuxServiceInfo;

/** @ struct [HCS_ServiceVTDU]
 *  @ brief  VTDU������Ϣ�ṹ��
 */
typedef struct 
{
	HCS_Endpoint	endpoint;							// ���������Ϣ
	char			reserved[8];

}HCS_ServiceVTDU;

/** @ struct [HCS_ServiceVAG]
 *  @ brief  VAG������Ϣ�ṹ��
 */
typedef struct 
{
	HCS_Endpoint	endpoint;							// ���������Ϣ
	int				vag_trans_stream;					// �Ƿ��VAGת�� 0 - ǿ�Ʋ�ת�� 1 - ��VAGת��
	char			reserved[8];

}HCS_ServiceVAG;

/** @ struct [HCS_RecPlanDetail]
 *  @ brief  ¼��ƻ�ִ��ʱ��ϸ��
 */
typedef struct 
{
	int			record;									// �Ƿ�¼��[0-��¼��  1-¼��]
	int			stream_type;							// ��������[0-������  1-������]
	int			record_allday[HCS_DAYS_PER_WEEK];		// �Ƿ�ȫ��¼��
	HCS_Period	record_time[HCS_DAYS_PER_WEEK][HCS_PLAN_SEGMENT_COUNT];	// ¼��ƻ�ʱ���
	int			pre_record;								// Ԥ¼ʱ��[0-��Ԥ¼  ����ֵΪԤ¼ʱ��(��λ��)]
	char		replication;							// �������[0-������ 1-1������ 2-˫������]
	char		reserved[4];

}HCS_RecPlanDetail;


/** @ struct [HCS_RecSched]
 *  @ brief  ʱ���¼����Ϣ
 */
typedef struct 
{
	HCS_Period  rec_time;								// ¼��ʱ��
	int			rec_type;								// ¼������[0:��ʱ¼��1:�ƶ����]
	char		reserved[8];

}HCS_RecSched;

/** @ struct [HCS_RecDay]
 *  @ brief  ȫ��¼����Ϣ
 */
typedef struct 
{
	int			all_day_record;							// �Ƿ�ȫ��¼��[0-��ȫ��¼��  1-ȫ��¼��]
	int			rec_type;								// ¼������[0:��ʱ¼��1:�ƶ����]
	char		reserved[8];

}HCS_RecDay;

/** @ struct [HCS_RecPlanDetail_V20]
 *  @ brief  ¼��ƻ�ִ��ʱ��ϸ��,֧��¼������
 */
typedef struct 
{
	int				record;									// �Ƿ�¼��[0-��¼��  1-¼��]
	int				defence;								// �Ƿ񲼷�[0-������  1-����]
	int				stream_type;							// ��������[0-������  1-������]
	HCS_RecDay		record_allday[HCS_DAYS_PER_WEEK];		// �Ƿ�ȫ��¼��
	HCS_RecSched	record_time[HCS_DAYS_PER_WEEK][HCS_PLAN_SEGMENT_COUNT];	// ¼��ƻ�ʱ���
	int				pre_record;								// Ԥ¼ʱ��[0-��Ԥ¼  ����ֵΪԤ¼ʱ��(��λ��)]
	char			replication;							// �������[0-������ 1-1������ 2-˫������]
	char			reserved[4];

}HCS_RecPlanDetail_V20;


/** @ struct [HCS_StreamDev]
 *  @ brief  ȡ���豸��Ϣ
 */
typedef struct 
{
	HCS_DevInfo			device;							// ǰ���豸��Ϣ
	int					channel;						// ͨ����
	HCS_ServiceVTDU		svc_vtdu;						// VTDU����
	HCS_ServiceVAG		svc_vag;						// VAG����
	HCS_AuxServiceInfo	svc_reserved;					// ���ӷ�����
	char				reserved[HCS_RESERVED_DATA_LEN];// �����ֶ�

}HCS_StreamDev;

/** @ struct [HCS_StreamUrl]
 *  @ brief  ȡ��URL��Ϣ,���ͨ����ý����ǰ���豸ȡ�����û����������ǵ�¼ǰ���豸���û���������
 *           ����ǹ�VAGȡ�����û���������VAG���û��������롣
 */
typedef struct 
{
	char				username[HCS_USERNAME_LEN];		// �û���
	char				password[HCS_PASSWORD_LEN];		// ����  
	char				stream_url[HCS_STREAM_URL_LEN];	// ȡ��URL
	char				reserved[HCS_RESERVED_DATA_LEN];// �����ֶ�

}HCS_StreamUrl;

/** @ union [HCS_StreamInfo]
 *  @ brief  ¼��ƻ�ȡ����Ϣ
 */
typedef union 
{
	char				data[HCS_STREAM_INFO_SIZE];		// ���ϵ�����
	HCS_StreamDev       stream_dev;						// ȡ���豸��Ϣ
	HCS_StreamUrl       stream_url;						// ȡ��URL��Ϣ

}HCS_StreamInfo;

/** @ struct [HCS_RecordPlan]
 *  @ brief  ¼��ƻ��ṹ
 */
typedef struct 
{
	char				camera_id[HCS_CAMERA_ID_LEN];		// ������ID�����֧��64���ַ�
	char				camera_name[HCS_CAMERA_NAME_LEN];	// ����������

	int					stream_mode;						// ȡ������[0-�豸ȡ����1-URLȡ��]
	HCS_StreamInfo		stream_info;						// ȡ����Ϣ

	char				pool_id[HCS_POOL_ID_LEN];			// ��Դ��ID
	char				rep_pool_id[HCS_POOL_ID_LEN];		// ������Դ��ID,�����������0ʱ��Ч
	HCS_RecPlanDetail	detail;								// ¼��ƻ�ִ��ʱ����Ϣϸ��
	char				reserved[HCS_RESERVED_DATA_LEN];	// �����ֶ�

}HCS_RecordPlan;


/** @ struct [HCS_RecordPlan_V20]
 *  @ brief  ¼��ƻ��ṹ,֧��¼����������
 */
typedef struct 
{
	char					camera_id[HCS_CAMERA_ID_LEN];		// ������ID�����֧��64���ַ�
	char					camera_name[HCS_CAMERA_NAME_LEN];	// ����������

	int						stream_mode;						// ȡ������[0-�豸ȡ����1-URLȡ��]
	HCS_StreamInfo			stream_info;						// ȡ����Ϣ

	char					pool_id[HCS_POOL_ID_LEN];			// ��Դ��ID
	char					rep_pool_id[HCS_POOL_ID_LEN];		// ������Դ��ID,�����������0ʱ��Ч
	HCS_RecPlanDetail_V20	detail;								// ¼��ƻ�ִ��ʱ����Ϣϸ��
	char					reserved[HCS_RESERVED_DATA_LEN];	// �����ֶ�

}HCS_RecordPlan_V20;



/** @ struct [HCS_Date]
 *  @ brief  ���ڽṹ��
 */
typedef struct 
{
	int year;
	int month;
	int day;

}HCS_Date;


/** @ struct [HCS_HisRecPlanDetail]
 *  @ brief  ��ʷ��¼�ƻ�ʱ����ϸ��Ϣ
 */
typedef struct 
{	
	int				history_stream_allday[HCS_DAYS_PER_WEEK];						// �Ƿ�¼ȫ������
	HCS_Period		history_stream_time[HCS_DAYS_PER_WEEK][HCS_PLAN_SEGMENT_COUNT];	// ��¼����ʱ���
	
	int				replenish_allday[HCS_DAYS_PER_WEEK];							// �Ƿ�ȫ��ִ�в�¼
	HCS_Period		replenish_time[HCS_DAYS_PER_WEEK][HCS_PLAN_SEGMENT_COUNT];		// ��¼ִ��ʱ���

	HCS_Date		date;															// ��¼��ʼ����,ȫ0��ʾ���·��ƻ����쿪ʼ��¼													// �Ƿ�¼��

	char			reserved[HCS_RESERVED_DATA_LEN];								// �����ֶ�

}HCS_HisRecPlanDetail;


/** @ struct [HCS_HisRecPlan]
 *  @ brief  ��ʷ��¼��ƻ��ṹ
 */
typedef struct 
{	
	char					camera_id[HCS_CAMERA_ID_LEN];							// ������ID�����֧��64���ַ�
	char					camera_name[HCS_CAMERA_NAME_LEN];						// ����������
	
	int						stream_mode;											// ȡ������[0-�豸ȡ����1-URLȡ��]
	HCS_StreamInfo			stream_info;											// ȡ����Ϣ

	char					pool_id[HCS_POOL_ID_LEN];								// ��Դ��ID
	int						replication;											// �������[0-������ 1-1������ 2-˫������]
	char					rep_pool_id[HCS_POOL_ID_LEN];							// ������Դ��ID,�����������0ʱ��Ч

	HCS_HisRecPlanDetail	detail;													// ��ʷ��¼��ƻ�ִ��ʱ����Ϣϸ��
	char					reserved[HCS_RESERVED_DATA_LEN];						// �����ֶ�

}HCS_HisRecPlan;


/** @ struct [HCS_TaskInfo]
 *  @ brief  ������Ϣ�ṹ��
 */
typedef struct 
{
	unsigned long long	task_id;						// ����ID
	int					task_type;						// ��������[1-����,2-�ֶ�¼��3-�ֶ���¼]
	int					task_status;					// ����״̬[1-����ִ�� 2-�쳣 3-�������]
	float				task_speed;						// �������,�ٷֱ�[0-100]
	char				res[HCS_RESERVED_DATA_LEN];		// ����

}HCS_TaskInfo;

/** @ struct [HCS_FileIndexData]
 *  @ brief  �����ļ�ÿһ��ĸ�ʽ
 */
typedef struct
{
	unsigned int		frame_type;				// ֡������[1-I֡]
	unsigned int		frame_sub_type;         // ֡������[0-��ͨI֡��1-ˢ��֡����P֡��]
	unsigned int		frame_rate;				// ֡��
	unsigned int		frame_num;				// ֡��[��1��ʼ]
	HCS_Time			frame_abs_time;         // ֡����ʱ���
	unsigned int		frame_rel_time;         // ֡���ʱ���[��λ������]
	unsigned long long	frame_offset;			// ֡�ļ�ƫ�ƣ��ֽ�
	unsigned int		frame_size;				// ֡����,�ֽ�
	unsigned int		res;					// Ԥ���ֶ�

}HCS_FileIndexData;


/** @ struct [HCS_VideoFileInfo]
 *  @ brief  ��Ƶ�ļ���Ϣ�ṹ��
 */
typedef struct 
{
	char				file_key[HCS_FILE_KEY_LEN];		// �ļ���
	unsigned long long	file_size;						// �ļ���С,��λ:�ֽ�
	HCS_Time			begin_time;						// �ļ���ʼʱ��
	HCS_Time			end_time;						// �ļ�����ʱ��
	unsigned int		index_count;					// ��������
	unsigned int		frame_rate;						// ֡��
	unsigned int		res[8];							// Ԥ��

}HCS_VideoFileInfo;

/** @ struct [HCS_ThirdVideoFileInfo]
 *  @ brief  ��������Ƶ�ļ���Ϣ�ṹ�壬����������Ϊ�Ǻ�����Ǳ�׼H.264��ʽ�ļ�ʱ����
 */
typedef struct  
{
	unsigned int		head_len;						// �ļ�ͷ����
	HCS_Time			begin_time;						// �ļ��Ŀ�ʼʱ��,��λ����
	HCS_Time			end_time;						// �ļ��Ľ���ʱ��,��λ����
	unsigned long long	file_len;						// �ļ��ĳ���
	char				res[HCS_RESERVED_DATA_LEN];		// �����ֶ�

}HCS_ThirdVideoFileInfo;

/** @ struct [HCS_VideoFileWriteInfo]
 *  @ brief  ��Ƶ�ļ���Ϣ�ṹ��
 */
typedef struct  
{
	char					file_path[HCS_FILE_KEY_LEN];		// �ļ�·��������С��260��
	char					camera_id[HCS_CAMERA_ID_LEN];		// ��Ƶ�ļ���Ӧ������ID[δ֪�ɲ���]
	unsigned int			replication;						// ������[0-������ 1-1������ 2-˫������]
	char					pool_id[HCS_POOL_ID_LEN];			// ��Դ��ID[���֧��32���ַ�]
	char					rep_pool_id[HCS_POOL_ID_LEN];		// ������Դ��ID[���֧��32���ַ�]�����������Ϊ0ʱ��Ч
	unsigned int			file_type;							// �ļ����ͣ�4--�����ļ���
	HCS_ThirdVideoFileInfo	third_file_info;					// ��������Ƶ�ļ���Ϣ�ṹ����Ϣ
	char					res[HCS_RESERVED_DATA_LEN];			// �����ֶ�

}HCS_VideoFileWriteInfo;


/** @ struct [HCS_RecDetailInfo]
 *  @ brief  ¼�����
 */
typedef struct 
{
	int						enable;													// �Ƿ�ʹ�ô˽ṹ��Ϣ[0-��ʹ�ã���ʹ��¼��ƻ�ģ���е���Ϣ,1-ʹ��]
	char					user_name[HCS_USERNAME_LEN];							// �豸�û���
	char					password[HCS_PASSWORD_LEN];								// �豸�û�����
	char					stream_url[HCS_STREAM_URL_LEN];							// ȡ��URL
	char					pool_id[HCS_POOL_ID_LEN];								// ��Դ��ID
	int						replication;											// �������[0-������ 1-1������]
	char					rep_pool_id[HCS_POOL_ID_LEN];							// ������Դ��ID,�����������0ʱ��Ч
	char					reserved[HCS_RESERVED_DATA_LEN];						// �����ֶ�

}HCS_RecDetailInfo;

/** @ struct [HCS_RealRecInfo]
 *  @ brief  �ֶ�¼�����
 */
typedef struct 
{
	char				camera_id[HCS_CAMERA_ID_LEN];	// ������ID
	unsigned int		rec_type;						// ¼������[0-65535]
	HCS_RecDetailInfo	detail;							// ȡ��¼����ϸ����
	char				res[HCS_RESERVED_DATA_LEN];		// �����ֶ�

}HCS_RealRecInfo;

/** @ struct [HCS_HisRecInfo]
 *  @ brief  �ֶ���¼����
 */
typedef struct 
{
	char				camera_id[HCS_CAMERA_ID_LEN];	// ������ID
	unsigned int		rec_type;						// ¼������[0-65535]
	HCS_Time			begin_time;						// ¼��ʼʱ��
	HCS_Time			end_time;						// ¼�����ʱ��
	HCS_RecDetailInfo	detail;							// ȡ��¼����ϸ����
	char				res[HCS_RESERVED_DATA_LEN];		// �����ֶ�

}HCS_HisRecInfo;

/* @ struct [HCS_HighSpeedSegInfo]
 * @ brief �����������ݶνṹ
 */
typedef struct
{
	int					index;							// ���ݶα��
	int					segment_id;						// �洢���ݵ����ݶ�ID,���ƴ洢�ڲ�����
	HCS_Time			begin_time;						// ��ǰ�洢���¼��ʼʱ��
	HCS_Time			end_time;						// ��ǰ�洢���¼�����ʱ��
	int					size;							// ��ǰ�洢�����ݵ��ܳ���,��λ:�ֽ�
	int					complete_len;					// �Ѿ��������ݵĳ���,��λ:�ֽ�
	int					record_type;					// ¼����������
	char				vs_ip[HCS_IP_PORT_LEN];			// ���ݶ�����vs ip

}HCS_HighSpeedSegInfo;

/** @ struct [HCS_GetHeadInfo]
 *  @ brief  ��ȡ¼��ͷ�ṹ��
 */
typedef struct 
{
	char					key[HCS_MAX_VIDEO_KEY_LEN];		// ���ݶ�ȡkey,�����¼������������ID���������Ƶ�ļ�������ļ�key
	HCS_Time				begin_time;						// ��ʼʱ��
	HCS_Time				end_time;						// ����ʱ��
	unsigned int			data_type;						// ��������[0-¼��,1-��Ƶ�ļ�]
	char					reserved[HCS_RESERVED_DATA_LEN];// �����ֶ�

}HCS_GetHeadInfo;


/** @ struct [HCS_DataReadInfo]
 *  @ brief  ���ݶ�ȡ�����ṹ��
 */
typedef struct 
{
	char					key[HCS_MAX_VIDEO_KEY_LEN];		// ���ݶ�ȡkey,�����¼������������ID���������Ƶ�ļ�������ļ�key
	HCS_Time				begin_time;						// ��ʼʱ��
	HCS_Time				end_time;						// ����ʱ��
	unsigned int			data_type;						// ��������[0-¼��,1-��Ƶ�ļ�]
	char					rec_type[HCS_RECORD_TYPE_LEN];	// ¼������[""��ʾȫ��,¼���������ʹ�á�|���ָ����0|2|3,���֧��32���ַ�����Ƶ�ļ�ʱ���ֶ���Ч]
	unsigned int			read_type;						// ��ȡ����[1-�����طţ�2-����I֡�ط�,3-����طţ�4-����]
	char					reserved[HCS_RESERVED_DATA_LEN];// �����ֶ�

}HCS_DataReadInfo;


/** @ struct [HCS_VideoBaseInfo]
 *  @ brief  ��ȡ��Ƶ�ļ���Ϣ�ṹ��
 */
typedef struct 
{
	char					file_key[HCS_FILE_KEY_LEN];		// ��Ƶ�ļ�key
	char					reserved[HCS_RESERVED_DATA_LEN];// �����ֶ�

}HCS_VideoBaseInfo;


/** @ struct [HCS_VideoReadInfo]
 *  @ brief  ���ݶ�ȡ�����ṹ��
 */
typedef struct 
{
	char					file_key[HCS_MAX_VIDEO_KEY_LEN];// ��Ƶ�ļ�key
	HCS_Time				begin_time;						// ��ʼʱ��
	HCS_Time				end_time;						// ����ʱ��
	unsigned int			read_type;						// ��ȡ����[1-�����طţ�2-����I֡�ط�,3-����طţ�4-����]
	char					reserved[HCS_RESERVED_DATA_LEN];// �����ֶ�

}HCS_VideoReadInfo;


/** @ struct [HCS_HighSpeedDownloadInfo]
 *  @ brief  �������ز����ṹ��
 */
typedef struct 
{
	HCS_DataReadInfo		data_info;						// ����������Ϣ
	unsigned int			type;							// ��������[0-���أ�1-����]
	unsigned int			concurrency;					// ������[1-10]
	unsigned int			max_segment_count;				// �û�����Ķ��������Ԫ�ظ���
	HCS_HighSpeedSegInfo*	segment_infos;					// ����ʱ����ȡ�����ݿ���Ϣ�������������ʱ��ƽ̨����������ص�������Ϣ�����룩
	int*					segment_count;					// ����ʱ����ȡ�����ݿ�����������������ʱ��ƽ̨����������ص����ݸ��������룩
	char					reserved[HCS_RESERVED_DATA_LEN];// �����ֶ�

}HCS_HighSpeedDownloadInfo;


/* @ struct [HCS_RecFrameExtractInfo]
 * @ brief ��֡��ʾ��Ϣ
 */
typedef struct
{
	char			camera_id[HCS_CAMERA_ID_LEN];		// ������ID�����֧��64���ַ�
	char			record_type[HCS_RECORD_TYPE_LEN];	// ¼������ [""��ʾȫ��,¼���������ʹ�á�|���ָ����0|2|3,���֧��32���ַ�]
	HCS_Time		begin_time;							// ¼��ʼʱ��
	HCS_Time		end_time;							// ¼�����ʱ��
	int				time_sikp;							// ��ȡͼƬ��ʱ���� [��λΪ��]
	HCS_PicType		pic_type;							// ͼƬ����
	int				pic_width;							// ͼƬ�߶�
	int				pic_hight;							// ͼƬ���
	char			res[HCS_RESERVED_DATA_LEN];			// �����ֶ�

}HCS_RecFrameExtractInfo;

/* @ struct [HCS_RecDayInfo]
 * @ brief ������ѯ��Ϣ
 */
typedef struct
{
	int				rec_set[HCS_DATA_HOURS];			// ��ѯ����ÿ��Сʱ��¼����Ϣ [�����±�0 - 23����ʾ1 - 24��Сʱ��0: ��ʾ��ǰСʱ��û��¼����Ϣ��1: ��ʾ��ǰСʱ����¼����Ϣ]
	char			res[HCS_RESERVED_DATA_LEN];			// �����ֶ�

}HCS_RecDayInfo;


/* @ struct [HCS_RecMonthInfo]
 * @ brief ������ѯ��Ϣ
 */
typedef struct
{
	int				rec_set[HCS_MONTH_DAYS];			// ��ѯ��ǰ�·�ÿ�յ�¼����Ϣ [�����±�0 - 30����ʾ1 - 31�ţ�0: ��ʾ��ǰ��û��¼����Ϣ��1: ��ʾ��ǰ����¼����Ϣ]
	char			res[HCS_RESERVED_DATA_LEN];			// �����ֶ�

}HCS_RecMonthInfo;


/* @ struct [HCS_TagInfo]
 * @ brief ��ע��Ϣ
 */
typedef struct
{
	HCS_Time		time;								// ��עʱ���
	int				type;								// ��ע���ͣ�������
	char			tag[HCS_TAG_LEN];					// ��ע����
	char			res[HCS_RESERVED_DATA_LEN];			// �����ֶ�

}HCS_TagInfo;


/******************************************************************************
*                                                                            *
*                               ���ܼ������ݽṹ									  *
*                                                                            *
******************************************************************************/

#define HCS_MAX_POLYGON_VERTEX			10					// �������󶥵���
#define HCS_MAX_RULE_NUM				8					// ���֧�ְ�������
#define HCS_MAX_RULE_NUM_EX				16					// ��չ�������֧��ʮ��������
#define HCS_MAX_TARGET_NUM				30					// ���ͬʱ���30���ƶ�Ŀ��
#define HCS_MAX_SHIELD_NUM				4					// ����ĸ���������
#define HCS_MAX_PARAM_NUM				200					// ���ؼ���������
#define HCS_MAX_AUX_AREA_NUM			16					// �������������Ŀ


#define HCS_SEARCH_EVENT_INFO_LEN		800					// �����¼���Ϣ����
#define HCS_MAX_ANALOG_CHANNUM			32
#define HCS_MAX_IP_CHANNEL				32
#define HCS_MAX_CHANNUM							    ( HCS_MAX_ANALOG_CHANNUM + HCS_MAX_IP_CHANNEL )

#define HCS_STREAM_ID_LEN				32
#define HCS_MAX_POS_KEYWORDS_NUM		3					// ֧�ֹؼ��ֲ�������
#define HCS_MAX_POS_KEYWORD_LEN			128					// ÿ���ؼ��ֳ���
#define HCS_SEARCH_CASE_NO_LEN			56
#define HCS_SEARCH_CASE_NAME_LEN		100
#define HCS_SEARCH_LITIGANT_LEN			32
#define HCS_SEARCH_CHIEF_JUDGE_LEN		32

#define HCS_MAX_CHANNUM_V40				512
#define HCS_NAME_LEN					32      //�û�������
#define HCS_CASE_NO_RET_LEN				52
#define HCS_CASE_NAME_RET_LEN			64
#define HCS_LITIGANT_RET_LEN			24
#define HCS_CHIEF_JUDGE_RET_LEN			24


/** @ struct [HCS_PointF]
 *  @ brief  ��(����) 
 */
typedef struct
{
	float x;
	float y;

}HCS_PointF;

/** @ struct [HCS_VectorF]
 *  @ brief  ʸ��
 */
typedef struct
{
	HCS_PointF   start_point;							// ���
	HCS_PointF   end_point;								// �յ�

}HCS_VectorF;


/** @ struct [HCS_PolygonF]
 *  @ brief  �����(����)
 */
typedef struct
{
	unsigned int  vertex_num;							// ������
	HCS_PointF    point[HCS_MAX_POLYGON_VERTEX];		// ����

}HCS_PolygonF;

/** @ struct [HCS_Shield]
 *  @ brief  ��������
 */
typedef struct
{
	unsigned char     id;								// ����id����1��ʼ
	unsigned char     enable;							// �Ƿ񼤻�
	unsigned char     reserved[2];						// �����ֽ�
	HCS_PolygonF      polygon;							// ��������Ӧ�Ķ��������

}HCS_Shield;

/** @ struct [HCS_ShieldList]
 *  @ brief  ����������
 */
typedef struct
{
	unsigned int  shield_num;
	HCS_Shield    shield[HCS_MAX_SHIELD_NUM];

}HCS_ShieldList;


/** @ struct [HCS_RectF]
 *  @ brief  ����(����)
 */
typedef struct
{
	float x;											// �������Ͻ�X������
	float y;											// �������Ͻ�Y������
	float width;										// ���ο��
	float height;										// ���θ߶�

}HCS_RectF;

/** @ enum   [HCS_SizeMode]
 *  @ brief  �ߴ�����
 */
typedef enum
{
	HCS_IMAGE_PIX_MODE,									// �������ش�С���ã���λpixel
	HCS_REAL_WORLD_MODE,								// ����ʵ�ʴ�С���ã���λm
	HCS_DEFAULT_MODE,									// ȱʡģʽ
	HCS_IMAGE_PIX_AREA_MODE,							// �������ģʽ
	HCS_REAL_WORLD_AREA_MODE							// ʵ�����ģʽ

}HCS_SizeMode;


/** @ struct [HCS_SizeFilter]
 *  @ brief  Ŀ��ߴ�������ṹ��
 */
typedef struct
{
	unsigned int   enable;								// �Ƿ񼤻�ߴ������
	HCS_SizeMode   mode;								// ����������:       
	HCS_RectF      min_rect;							// ��СĿ���        
	HCS_RectF      max_rect;							// ���Ŀ���

}HCS_SizeFilter;


/** @ enum  [HCS_CrossDirection]
 *  @ brief  ���߷�������
 */
typedef enum
{
	HCS_BOTH_DIRECTION,									// ˫��
	HCS_LEFT_TO_RIGHT,									// ��������
	HCS_RIGHT_TO_LEFT									// ��������

}HCS_CrossDirection;


/** @ struct [HCS_TraversePlane]
 *  @ brief  ���߲���
 */
typedef struct
{
	HCS_CrossDirection cross_direction;					// ��Խ���� 

}HCS_TraversePlane;


/** @ struct [HCS_ReachHeightEx]
 *  @ brief  ���߲���
 */
typedef struct
{
	HCS_CrossDirection cross_direction;					// ��Խ���� 

}HCS_ReachHeightEx;


/** @ struct [HCS_Intrusion]
 *  @ brief  �������ֲ���
 */
typedef struct
{
	unsigned int delay; 

}HCS_Intrusion;


/** @ struct [HCS_Loiter]
 *  @ brief  �ǻ�����
 */
typedef struct
{
	unsigned int delay; 

}HCS_Loiter;


/** @ struct [HCS_LeftTake]
 *  @ brief  ��Ʒ��ȡ���ò���
 */
typedef struct
{
	unsigned int delay; 

}HCS_LeftTake;


/** @ struct [HCS_StickUp]
 *  @ brief  ����ճ������
 */
typedef struct
{
	unsigned int delay; 

}HCS_StickUp;


/** @ struct [HCS_InstallScanner]
 *  @ brief  ����������
 */
typedef struct
{
	unsigned int delay; 

}HCS_InstallScanner;


/** @ struct [HCS_Parking]
 *  @ brief  ͣ������
 */
typedef struct
{
	unsigned int delay; 

}HCS_Parking;


/** @ struct [HCS_AbnormalFace]
 *  @ brief  �쳣��������
 */
typedef struct
{
	unsigned int delay;
	unsigned int mode;

}HCS_AbnormalFace;


/** @ struct [HCS_ToiletTarry]
 *  @ brief  ��޳�ʱ����
 */
typedef struct
{
	unsigned int delay; 

}HCS_ToiletTarry;


/** @ struct [HCS_YaryTarry]
 *  @ brief  �ŷ糡��������
 */
typedef struct
{
	unsigned int delay; 

}HCS_YaryTarry;


/** @ struct [HCS_OverTime]
 *  @ brief  ������ʱ����
 */
typedef struct
{
	unsigned int delay;

}HCS_OverTime;


/** @ struct [HCS_Run]
 *  @ brief  �����ƶ�����
 */
typedef struct
{
	float         speed;								// �ٶ�
	unsigned int  delay;								// �����ӳ�
	HCS_SizeMode  mode;									// ����ģʽ,ֻ֧������ģʽ��ʵ��ģʽ����

}HCS_Run;


/** @ struct [HCS_HighDensity]
 *  @ brief  ��Ա�ۼ�����
 */
typedef struct
{
	float        alert_density;							// ��Ա�ۼ������ܶ�, ��Χ: [0.1, 1.0]
	unsigned int delay;									// ʱ�䣬�ﵽ��ʱ��󷢳�ӵ������

}HCS_HighDensity;   


/** @ enum  [HCS_ViolentMode]
 *  @ brief �����˶�ģʽ
 */
typedef enum
{
	HCS_VIOLENT_VIDEO_MODE,								// ����Ƶģʽ
	HCS_VIOLENT_VIDEO_AUDIO_MODE,						// ����Ƶ����ģʽ
	HCS_VIOLENT_AUDIO_MODE								// ����Ƶģʽ

}HCS_ViolentMode;


/** @ struct [HCS_Violent]
 *  @ brief  �����˶�����
 */
typedef struct
{
	unsigned int        delay;
	HCS_ViolentMode     mode;

}HCS_Violent;

/** @ struct [HCS_FlowCounter]
 *  @ brief  ������ͳ�ƽ��뷽�����
 */
typedef struct
{
	HCS_VectorF direction;

}HCS_FlowCounter;


/** @ enum   [HCS_LeavePosMode]
 *  @ brief  ֵ��״̬ģʽ
 */
typedef enum
{
	HCS_LEAVE_POSITION_LEAVE_MODE        = 0x01,		// ���ģʽ
	HCS_LEAVE_POSITION_SLEEP_MODE        = 0x02,		// ˯��ģʽ
	HCS_LEAVE_POSITION_LEAVE_SLEEP_MODE  = 0x04			// ���˯��ģʽ

}HCS_LeavePosMode;


/** @ enum   [HCS_LeavePosPersonMode]
 *  @ brief  ֵ������ģʽ
 */
typedef enum
{
	HCS_LEAVE_POSITION_SINGLE_MODE,						// ����ֵ��ģʽ
	HCS_LEAVE_POSITION_DOUBLE_MODE						// ˫��ֵ��ģʽ

}HCS_LeavePosPersonMode;


/** @ enum   [HCS_LeavePosChannelType]
 *  @ brief  ����ͨ������
 */
typedef enum
{
	HCS_LEAVE_POSITION_INDEPENDENT,						// ����ͨ��
	HCS_LEAVE_POSITION_MASTER,							// ��ͨ��
	HCS_LEAVE_POSITION_SLAVE							// ��ͨ��

}HCS_LeavePosChannelType;

/** @ struct [HCS_LeavePost]
 *  @ brief  ��ڼ�ⱨ������
 */
typedef struct
{
	HCS_LeavePosMode           mode;					// ���״̬ģʽ
	unsigned int               leave_delay;				// ���˱���ʱ��
	unsigned int               static_delay;			// ˯������ʱ��
	HCS_LeavePosPersonMode     peo_mode;				// �������ģʽ
	HCS_LeavePosChannelType    chan_type;				// ͨ�����ԣ�����ͨ������ͨ������ͨ��

}HCS_LeavePost;


/** @ struct [HCS_FallDown]
 *  @ brief  ���ؼ�����
 */
typedef struct
{
	unsigned int delay;

}HCS_FallDown;


/** @ struct [HCS_AudioAbnormal]
 *  @ brief  ��ǿͻ�����
 */
typedef struct
{
	unsigned int  decibel;							// ����ǿ��
	unsigned char audio_mode;						// �������ģʽ��0�����������ȼ�⣻1�����÷ֱ���ֵ���

}HCS_AudioAbnormal;


/** @ enum   [HCS_GetUpMode]
 *  @ brief  �𴲹���ģʽ
 */
typedef enum
{
	HCS_GET_UP_OVER_BED_MODE  = 0x1,				// ��ͨ��ģʽ
	HCS_GET_UP_AREA_MOVE_MODE = 0x2,				// �ߵ���ģʽ
	HCS_GET_UP_SITTING_MODE   = 0x3					// ��������ģʽ

}HCS_GetUpMode;

/** @ struct [HCS_GetUp]
 *  @ brief  ������
 */
typedef struct
{
	HCS_GetUpMode mode;								// ������ģʽ

}HCS_GetUp;


/** @ struct [HCS_Static]
 *  @ brief  ����
 */
typedef struct
{
	int delay;

}HCS_Static;


/** @ struct [HCS_BvFallDown]
 *  @ brief  ˫Ŀ���ع���
 */
typedef struct
{
	float        height;							// ���ظ߶���ֵ
	unsigned int delay;								// ����ʱ��

}HCS_BvFallDown; 


/** @ struct [HCS_BvStandUp]
 *  @ brief  ˫Ŀվ��������
 */
typedef struct
{
	float        height;							// վ���߶���ֵ
	unsigned int delay;								// վ��ʱ��

}HCS_BvStandUp; 


/** @ enum   [HCS_ComparkMode]
 *  @ brief  ˫Ŀ������⼰�����ڡ�С�ڡ����ڡ��������ж�ģʽ
 */
typedef enum
{
	HCS_MORE_MODE,									// ����ģʽ
	HCS_LESS_MODE,									// С��ģʽ
	HCS_EQUAL_MODE,									// ����ģʽ
	HCS_UNEQUAL_MODE								// ������ģʽ

}HCS_ComparkMode;


/** @ struct [HCS_BvPepleNum]
 *  @ brief  ˫Ŀ����������
 */
typedef struct
{
	unsigned int     people_num;					// ����
	unsigned int     people_state;					// ������״̬,1:���� 0:����
	HCS_ComparkMode  mode;							// ����С��ģʽ
	unsigned int     delay;							// ��ʱ

}HCS_BvPepleNum;


/** @ struct [HCS_BvPeopleDistance]
 *  @ brief  ˫Ŀ��������
 */
typedef struct
{
	float            distance;						// ��Ŀ����
	HCS_ComparkMode  mode;							// ����С��ģʽ
	unsigned int     delay;							// ��ʱ

}HCS_BvPeopleDistance;


/** @ union [HCS_RuleParamSets]
 *  @ brief �������������
 */
typedef union
{
	int                      param[6];				// ����
	HCS_TraversePlane        traverse_plane;		// ���߲���  
	HCS_ReachHeightEx        reach_height_ex;		// �ʸ߲���
	HCS_Intrusion            intrusion;				// �������ֲ���
	HCS_Loiter               loiter;				// �ǻ�����
	HCS_LeftTake             left_take;				// ��Ʒ��ȡ���ò���
	HCS_Parking              parking;				// ͣ������
	HCS_Run                  run;					// �����ƶ�����
	HCS_HighDensity          high_density;			// ��Ա�ۼ�����
	HCS_Violent              violent;				// �����˶�����
	HCS_AbnormalFace         abnormal_face;			// �쳣��������
	HCS_OverTime             over_time;				// ������ʱ
	HCS_StickUp              stick_up;				// ����ճ������
	HCS_InstallScanner       insert_scanner;		// ��װ������
	HCS_FlowCounter          flow_counter;			// ������ͳ�ƽ��뷽��
	HCS_LeavePost            leave_post;			// ��ڼ�ⱨ��ʱ��
	HCS_FallDown             fall_down;				// ���ؼ�����
	HCS_AudioAbnormal        audio_abnormal;		// �����쳣
	HCS_GetUp                get_up;				// ���������
	HCS_YaryTarry            yard_tarry;			// �ŷ糡����ʱ��
	HCS_ToiletTarry          toilet_tarry;			// ��޳�ʱʱ��
	HCS_Static               stic;					// ����
	HCS_BvFallDown           bv_fall_down;			// ˫Ŀ����
	HCS_BvStandUp            bv_stand_up;			// ˫Ŀվ��
	HCS_BvPepleNum           bv_people_num;			// ˫Ŀ���� 
	HCS_BvPeopleDistance     bv_people_distance;	// ˫Ŀ���

}HCS_RuleParamSets;

/** @ enum [HCS_RuleType]
 *  @ brief ������������
 */
typedef enum
{
	HCS_LINE_RULE,									// �߹��� 
	HCS_REGION_RULE,								// �������
	HCS_LINE_EX_RULE								// ���߹���

}HCS_RuleType;


/** @ enum  [HCS_EventType]
 *  @ brief �¼�����
 */
typedef enum
{ 
	HCS_TRAVERSE_PLANE  = 0x01,						// ��Խ������
	HCS_ENTER_AREA      = 0x02,						// ��������
	HCS_EXIT_AREA       = 0x04,						// �뿪����
	HCS_INTRUSION       = 0x08,						// ��������
	HCS_LOITER          = 0x10,						// �ǻ�
	HCS_LEFT_TAKE       = 0x20,						// ��Ʒ��ȡ����
	HCS_PARKING         = 0x40,						// ͣ��
	HCS_RUN             = 0x80,						// �����ƶ�
	HCS_HIGH_DENSITY    = 0x100,					// ��Ա�ۼ�
	HCS_FLOW_COUNTER    = 0x200,					// ������
	HCS_VIOLENT_MOTION  = 0x400,					// �����˶�
	HCS_TRAIL           = 0x800,					// β��
	HCS_LEFT            = 0x1000,					// ��Ʒ����
	HCS_TAKE            = 0x2000,					// ��Ʒ��ȡ
	HCS_ABNORMAL_FACE   = 0x4000,					// �쳣����
	HCS_GET_UP          = 0x8000,					// ������
	HCS_REACH_HEIGHT    = 0x10000,					// �ʸ߼��
	HCS_LEAVE_POSITION  = 0x20000,					// ��ڼ��
	HCS_FACE_CAPTURE	= 0x40000,					// ��������
	HCS_FALL_DOWN       = 0x80000,					// ���ؼ��
	HCS_AUDIO_ABNORMAL  = 0x100000,					// �����쳣���
	HCS_MULTI_FACES     = 0x200000,					// ��������
	HCS_TOILET_TARRY    = 0x400000,					// ��޳�ʱ
	HCS_YARD_TARRY      = 0x800000,					// �ŷ糡����
	HCS_REACH_HEIGHT_EX = 0x1000000,				// �ʸ߼��(����)
	HCS_HUMAN_ENTER     = 0x10000000,				// ��Ա����
	HCS_OVER_TIME       = 0x20000000,				// ������ʱ
	HCS_STICK_UP        = 0x40000000,				// ����ճ��              
	HCS_INSTALL_SCANNER = 0x80000000				// ��װ������   

}HCS_EventType;


/** @ struct [HCS_RuleInfo]
 *  @ brief  �򻯵Ĺ�����Ϣ, ��������Ļ�����Ϣ
 */
typedef struct
{
	unsigned char         id;						// ������������ id
	unsigned char         rule_type;				// �������ͣ�0: line; 1: region;
	unsigned char         reserved[6];				// �����ֽ�
	unsigned int          event_type;				// �����¼�����
	HCS_RuleParamSets     param;					// �������
	HCS_PolygonF          polygon;					// �����Ӧ�Ķ��������

}HCS_RuleInfo;


/** @ struct [HCS_RuleParam]
 *  @ brief  �������
 */
typedef struct
{
	unsigned int          sensitivity;				// ���������Ȳ�������Χ[1,5]
	unsigned int          reserved[4];				// Ԥ���ֽ�
	HCS_RuleParamSets     param;					// �������
	HCS_SizeFilter        size_filter;				// �ߴ������	

}HCS_RuleParam;


/** @ struct [HCS_Rule]
 *  @ brief  �������
 */
typedef struct
{
	unsigned char      id;							// ����id
	unsigned char      enable;						// �Ƿ񼤻�;
	unsigned char      rule_type;					// �������ͣ�0: line; 1: region;
	unsigned char      target_type;					// ����Ŀ�����ͣ�2: Human; 1: Vehicle; 0: anything
	unsigned int       event_type;					// �����¼�����

	unsigned char      reset_counter;				// ����ͳ�����ñ��
	unsigned char      update_rule;					// ������±��
	unsigned char      enable_counter;				// �Ƿ����¼�������
	unsigned char      reserved[5];					// �����ֽ�,��һ���ֽ�������ʾʹ���µ��¼����ͻ����ϵģ�0���ϵģ�1���µ�;

	HCS_RuleParam      rule_param;					// �������
	HCS_PolygonF       polygon;						// �����Ӧ�Ķ��������

}HCS_Rule;


/** @ struct [HCS_RuleList]
 *  @ brief  �����������
 */
typedef struct
{
	unsigned int	rule_num;						// �����й�������
	HCS_Rule		rule[HCS_MAX_RULE_NUM];			// ��������

}HCS_RuleList;


/** @ struct [HCS_RuleListEx]
 *  @ brief  ��չ�����������
 */
typedef struct
{
	unsigned int  rule_num;							// �����й�������
	HCS_Rule      rule[HCS_MAX_RULE_NUM_EX];		// ��������

}HCS_RuleListEx;


/* @ struct [HCS_SmartSearchParam]
 * @ brief  ���ܼ�������
 */
typedef struct
{
	char			camera_id[HCS_CAMERA_ID_LEN];	//������id
	HCS_Time		beg_time;						//¼��ʼʱ��
	HCS_Time		end_time;						//¼�����ʱ��
	HCS_RuleList	rule_list;						//���������б�
	char			res[HCS_RESERVED_DATA_LEN];		//����

}HCS_SmartSearchParam;


/* @ struct [HCS_SmartSearchParam_V20]
 * @ brief  ���ܼ�������(֧����ǰ�Ӻ�ʱ������)
 */
typedef struct
{
	char			camera_id[HCS_CAMERA_ID_LEN];	//������id
	HCS_Time		beg_time;						//¼��ʼʱ��
	HCS_Time		end_time;						//¼�����ʱ��
	unsigned int	pre_time;						//���ܱ�����ǰʱ�� ��λ:��
	unsigned int	delay_time;						//���ܱ����ӳ�ʱ�� ��λ:��		
	HCS_RuleList	rule_list;						//���������б�
	char			res[HCS_RESERVED_DATA_LEN];		//����

}HCS_SmartSearchParam_V20;

/* @ struct [HCS_SmartSearchRet]
 * @ brief �����������
 */
typedef struct
{
	HCS_Time	start_time;						// �ƶ���ⱨ����ʼ��ʱ��
	HCS_Time	end_time;						// �¼�ֹͣ��ʱ��
	char		res[64];

}HCS_SmartSearchRet;



// ����Ϣ - 72�ֽڳ�
typedef struct 
{
	int		size;
	char	id[HCS_STREAM_ID_LEN];  //ID����
	int		channel;                //�����豸ͨ��������0xffffffffʱ����ʾ������
	char	res[32];                //����

}StreamInfo;

/* @ struct [HCS_SmartEventSearchParam]
 * @ brief  �����¼���������
 */
typedef struct
{
	int			maior_type;						//0-�ƶ���⣬1-��������, 2-�����¼� 5-pos¼��
	int			minor_type;						//����������- ���������ͱ仯��0xffff��ʾȫ��
	char		camera[HCS_CAMERA_ID_LEN];		//�����ı�����
	HCS_Time	start_time;						//�����Ŀ�ʼʱ�䣬ֹͣʱ��: ͬʱΪ(0, 0) ��ʾ�������ʱ�俪ʼ���������ǰ���4000���¼�
	HCS_Time	end_time;						//�����Ľ���ʱ��
	int			lock_type;						// 0xff-ȫ����0-δ����1-����
	union
	{
		char				len[HCS_SEARCH_EVENT_INFO_LEN];

		struct//��������
		{
			short			alarm_info_num[128];			//��������ţ���ֵ��ʾ,���ý��������У�0xffff��ʾ������Ч
			char			res[544];						//����

		}AlarmParam;

		struct//�ƶ����
		{
			short			motion_channel[HCS_MAX_CHANNUM];//�ƶ����ͨ������ֵ��ʾ ,���ý��������У�0xffff��ʾ������Ч
			char			res[672];						//����

		}MotionParam;

		struct //��Ϊ���� 
		{
			short			channel[HCS_MAX_CHANNUM];		//��Ϊ������Ӧ��ͨ������ֵ��ʾ,���ý��������У�0xffff��ʾ������Ч
			char			rule_id;						//��Ϊ�������ͣ�����0xff��ʾȫ������0��ʼ
			char			res[671];    

		}VcaParam;

		struct//��Ѷ�¼���������
		{
			char			room_index;						//��Ѷ�ұ��,��1��ʼ
			char			res[799];						//����

		}InquestParam;

		struct//�������������� ��ͨ���Ű�ֵ��ʾ
		{
			char			all;							//����ȫ��ͨ����0-�񣬴�ʱdwChanNo������Ч��//1-����ȫ��ͨ������ʱdwChanNo������Ч��
			char			res1[3];
			short			channel[HCS_MAX_CHANNUM];		// ����ͨ����,��ֵ��ʾ��0xffff��Ч���Һ�������Ҳ��ʾ��Чֵ
			char			res[668];

		}VCADetect;

		struct
		{
			StreamInfo		id_info;						// ��id��Ϣ��72�ֽڳ�
			int				cmd_type;						// �ⲿ�������ͣ�NVR�����ƴ洢ʹ��
			char			backup_volume_num;				//�浵��ţ�CVRʹ��
			char			res[723];

		}StreamIDParam;

		struct //pos¼��
		{
			short			channel[HCS_MAX_CHANNUM];		//ͨ������ֵ��ʾ,���������У�����0xffffʱ��ʾ�������ֵ��Ч
			char			all_channel;					//�Ƿ����ȫ��ͨ����0-�񣬴�ʱwChannel��Ч��1-ȫ��ͨ������ʱwChannel��Ч
			char			case_sensitive;					//0-�����ִ�Сд�� 1-���ִ�Сд
			char			combinate_mode;					//�ؼ�����Ϸ�ʽ��0-��1-��
			char			res1;							//����
			char			key_word[HCS_MAX_POS_KEYWORDS_NUM][HCS_MAX_POS_KEYWORD_LEN]; //�ؼ��ֲ���ʱ������
			char			res2[284];						//����

		}PosAlarm;

		struct
		{
			char			case_index[HCS_SEARCH_CASE_NO_LEN];			//�������
			char			case_name[HCS_SEARCH_CASE_NAME_LEN];		//��������
			char			litigant1[HCS_SEARCH_LITIGANT_LEN];			//������1
			char			lLitigant2[HCS_SEARCH_LITIGANT_LEN];		//������2
			char			chief_judge[HCS_SEARCH_CHIEF_JUDGE_LEN];	//���г�
			char			case_type;
			char			res[47];

		}TrialParam;

	}SeniorParam;

	char			res[HCS_RESERVED_DATA_LEN];				//����

}HCS_SmartEventSearchParam;


//���������
typedef struct
{	
	HCS_VectorF				plane_bottom;					//������ױ�
	int						cross_direction;				//��Խ����: 0-˫��1-�����ң�2-���ҵ���
	char					sensitivity;                    //�����Ȳ�������Χ[1,5]
	char					plane_height;                   //������߶�
	char					dtection_target;				//0-����Ŀ�꣬1-�ˣ�2-��
	char					res[37];						//����

}HCS_Event_TraversePlan;

//����/�뿪�������
typedef struct
{
	HCS_PolygonF			region;//����Χ
	char					sensitivity;					//�����Ȳ�������Χ[1,5]
	char					detection_target;				//0-����Ŀ�꣬1-�ˣ�2-��
	char					res[6];

}HCS_Event_Area;

//���ݱ����ӳ�ʱ������ʶ�����д�ͼƬ�����������IO����һ�£�1�뷢��һ����
//���ֲ���
typedef struct
{
	HCS_PolygonF			region;							//����Χ
	short					duration;						//��Ϊ�¼�����ʱ����ֵ: 1-120�룬����5�룬�ж�����Ч������ʱ��  ��ATMϵͳ�д����ļ���ֵΪ 1-1000��
	char					sensitivity;					//�����Ȳ�������Χ[1-100]
	char					rate;							//ռ�ȣ�����������δ����Ŀ��ߴ�Ŀ��ռ��������ı��أ���һ��Ϊ����
	char					detection_target;				//0-����Ŀ�꣬1-�ˣ�2-��
	char					res[3];							//����

}HCS_Event_Intrusion;

//�ǻ�����
typedef struct
{
	HCS_PolygonF			region;							//����Χ
	short					duration;						//����ʱ����ֵ��1-120�룬����10��
	char					sensitivity;					//�����Ȳ�������Χ[1,5]
	char					res[5];

}HCS_Event_Loiter;

//��Ʒ����/��Ʒ��ȡ����
typedef struct
{
	HCS_PolygonF			region;							//����Χ
	short					duration;						//����ʱ����ֵ��1-120�룬����10��
	char					sensitivity;					//�����Ȳ�������Χ[1,5]
	char					res[5];

}HCS_Event_take_left;

//ͣ������
typedef struct
{
	HCS_PolygonF			region;							//����Χ
	short					duration;						//����ʱ����ֵ��1-100�룬����10��
	char					sensitivity;					//�����Ȳ�������Χ[1,5]
	char					res[5];

}HCS_Event_Parking;

//�����ƶ�����
typedef struct
{
	HCS_PolygonF			region;							//����Χ
	float					run_distance;					//�˿����ƶ�������, ��Χ: [0.1, 1.00] ����ģʽ ʵ��ģʽ(1,20)m/s
	char					sensitivity;					//�����Ȳ�������Χ[1,5]
	char					mode;							// 0 ����ģʽ  1 ʵ��ģʽ
	char					detection_target;				//0-����Ŀ�꣬1-�ˣ�2-��
	char					res;

}HCS_Event_Run;

//��Ա�ۼ�����
typedef struct
{
	HCS_PolygonF			region;							//����Χ
	float					density;						//�ۼ�����, ��Χ: [0.1, 1.0]
	char					sensitivity;					//�����Ȳ�������Χ[1,5]
	char					res;							// �����ֽ�
	short					duration;						// ������Ա�ۼ�����������ֵ 20-360s

}HCS_Event_High_Density; 

//�����˶�����
typedef struct
{
	HCS_PolygonF			region;							//����Χ
	short					duration;						//���������˶�������ֵ��1-50��
	char					sensitivity;					//�����Ȳ�������Χ[1,5]
	char					mode;							//0-����Ƶģʽ��1-����Ƶ����ģʽ��2-����Ƶģʽ
	char					res[4];							//����

}HCS_Event_Violent_motion; 

// �ʸ߲���
typedef struct
{
	HCS_VectorF				vca_line;						//�ʸ߾�����
	short					duration;						//�����ʸ߱�����ֵ��1-120��
	char					res[6];							// �����ֽ�

}HCS_Event_Reach_Hight;

// �𴲲���
typedef struct
{
	HCS_PolygonF			region;							//����Χ
	short					duration;						//�����𴲱�����ֵ1-100 ��
	char					mode;							//������ģʽ,0-��ͨ��ģʽ,1-�ߵ���ģʽ,2-��ͨ����������ģʽ
	char					sensitivity;					//�����Ȳ�������Χ[1,10]
	char					res[4];							//�����ֽ�

}HCS_Event_Get_Up;

// ��Ʒ����
typedef struct
{
	HCS_PolygonF			region;							// ����Χ
	short					duration;						// ������Ʒ����������ֵ 10-100��
	char					sensitivity;					// �����Ȳ�������Χ[1,5] 
	char					res[5];							// �����ֽ�

}HCS_Event_Left;

// ��Ʒ��ȡ
typedef struct
{
	HCS_PolygonF			region;							// ����Χ
	short					duration;						// ������Ʒ��ȡ������ֵ10-100��
	char					sensitivity;					// �����Ȳ�������Χ[1,5] 
	char					res[5];							// �����ֽ�

}HCS_Event_Take;

typedef struct
{
	HCS_PolygonF			region;							// ����Χ
	short					duration;						// ��������ʱ����ֵ 4s-60000s
	char					res[6];							// �����ֽ�

}HCS_Event_OverTime;

typedef struct
{
	int						res[23];						//�����ֽ�

}HCS_Event_Enter;

//��ֽ������
typedef struct
{
	HCS_PolygonF			region;							//����Χ
	short					duration;						//����ʱ����ֵ��4-60�룬����10��
	char					sensitivity;					//�����Ȳ�������Χ[1,5]
	char					res[5];

}HCS_Event_Stick_Up; 

//����������
typedef struct
{
	HCS_PolygonF			region;							//����Χ
	short					duration;						//��������ʱ�䣺4-60��
	char					sensitivity;					//�����Ȳ�������Χ[1,5]
	char					res[5];

}HCS_Event_Scanner; 

//����¼�
typedef struct
{
	HCS_PolygonF			region;							//����Χ
	short					leave_delay;					//���˱���ʱ�䣬��λ��s��ȡֵ1-1800
	short					static_delay;					//˯������ʱ�䣬��λ��s��ȡֵ1-1800
	char					mode;							//ģʽ��0-����¼���1-˯���¼���2-���˯���¼�
	char					person_type;					//ֵ���������ͣ�0-����ֵ�ڣ�1-˫��ֵ��
	char					res[2];							//����

}HCS_Event_LEAVE_POSITION;

//β�����
typedef struct
{
	HCS_PolygonF			region;							//����Χ
	short					res;							//����
	char					sensitivity;					//�����Ȳ�������Χ[1,5]
	char					res2[5];

}HCS_Event_Trail;

//���ز���
typedef struct
{
	HCS_PolygonF			region;							//����Χ
	short					duration;						//�����¼���ֵ 1-60
	char					sensitivity;					//�����Ȳ�������Χ[1,5]
	char					height_threshold;				//�߶���ֵ����Χ[0,250]��Ĭ��90����λ������
	char					res[4];

}HCS_Event_Fall_Down;

//����
typedef struct
{
	HCS_PolygonF			region;							//����Χ
	char					sensitivity;					//�����Ȳ�������Χ[1,100]
	char					height_threshold;				//�߶���ֵ����Χ[0,250]��Ĭ��130����λ������
	short					duration;						//�����¼���ֵ[1,3600]��Ĭ��2����λ����
	char					res[4];							//����

}HCS_Event_Standup;


//�����仯
typedef struct
{
	HCS_PolygonF			region;							//����Χ
	char					sensitivity;					//�����Ȳ�������Χ[1,100]
	char					people_num_threshold;			//������ֵ����Χ[0,5]��Ĭ��1 
	char					detect_mode;					//��ⷽʽ����������ֵ��Ƚϡ�1-���ڣ�2-С�ڣ�3-���ڣ�4-������
	char					none_state_effective;			//����״̬�Ƿ���Ч��0-��Ч��1-��Ч
	short					duration;						//����ʱ����ֵ[1,3600]��Ĭ��2����λ����
	char					res[2];							//����

}HCS_Event_PeopleNum_Change;

//���仯
typedef struct
{
	HCS_PolygonF			region;							//����Χ
	float					spacing_threshold;				//�����ֵ����Χ[0,10.0]��Ĭ��1.0����λ����
	char					sensitivity;					//�����Ȳ�������Χ[1,100]
	char					detect_mode;					//��ⷽʽ��������ֵ��Ƚϡ�1-���ڣ�2-С��
	short					duration;						//����ʱ����ֵ[1,3600]��Ĭ��2����λ����

}HCS_Event_Spacing_Change;


//��ǿͻ�����
typedef struct
{
	short					decibel;						//����ǿ��
	char					sensitivity;					//�����Ȳ�������Χ[1,100] 
	char					audio_mode;						//�������ģʽ��0-�����ȼ�⣬1-�ֱ���ֵ��⣬2-��������ֱ���ֵ��� 
	char					enable;							//ʹ�ܣ��Ƿ���(��ǿͻ�䣬����)
	char					threshold;						//������ֵ[1,100]
	char					res[54];						//����   

}HCS_Event_Audio_Abnormal;

//��ǿ����
typedef struct
{
	char					sensitivity;					//�����Ȳ�������Χ[1,100] 
	char					enable;							//ʹ�ܣ��Ƿ���(��ǿͻ�䣬����)
	char					res[6];    

}HCS_Event_Steep_Drop;


typedef struct
{
	HCS_PolygonF			region;							//����Χ
	short					delay;							//��޳�ʱʱ��[1,3600]����λ����
	char					res[6];    

}HCS_Event_Toiler_Tarry;

typedef struct
{
	HCS_PolygonF			region;							//����Χ
	short					delay;							//�ŷ糡����ʱ��[1,120]����λ����
	char					res[6];   

}HCS_Event_Yard_Tarry;

typedef struct
{
	HCS_PolygonF			region;							//�ʸ�����
	int						cross_direction;				//��Խ����: 0-˫��1-������2-���ҵ���
	char					res[4];							//�����ֽ�

}HCS_Event_AdvReach_Height;

typedef struct
{
	HCS_PolygonF			region;							//����������
	int						cross_direction;				//��Խ����: 0-˫��1-������2-���ҵ���
	char					sensitivity;					//�����Ȳ�������Χ[1,5] 
	char					res[3];							//�����ֽ�

} HCS_Event_AdvTraverse_Plane;

typedef struct
{
	HCS_PolygonF			region;							//����Χ
	short					duration;						//��Ϊ�¼�����ʱ����ֵ: 1-10�룬����1�룬�ж�����Ч������ʱ��
	char					sensitivity;					//�����Ȳ�������Χ[1-100]
	char					alarm_state;					//ֻ���ֶν��治��ʾ��0-����,1-������ʼ,2-�������� 
	char					tracking_mode ;					//����ģʽ��0-�Զ�(Ĭ��) , 1-ˮƽ, 2-��ֱ
	char					zoom_mode;						//�䱶ģʽ, 0-�̶�(Ĭ��), 1-�Զ�
	char					zoom_over;						//0-����,1-�䱶��λ(�����ϴ�¼����������Ϊ�л������ж�����)
	char					res;

}HCS_Event_Lecture;

typedef struct
{
	HCS_PolygonF			region;							//����Χ
	char					sensitivity;					//�����Ȳ�������Χ[1-100]
	char					alarm_state;					//ֻ���ֶν��治��ʾ��0-����,1-������ʼ,2-��������
	char					zoom_over;						//0-����,1-�䱶��λ(�����ϴ�¼����������Ϊ�л������ж�����)
	char					res[5];							//����

}HCS_Event_Answer;

//��������ṹ��
typedef struct
{   
	char					rule_id;						//������� 0-��ʾ��
	char					res[3];							//����

}HCS_Event_Relate_Rule_Param;

typedef struct
{
	char							rule_sequence;			//���򴥷�˳��0-˳������򴥷���1-˳�򴥷�
	char							res1[7];
	int								min_interval;			//��Сʱ��������λ����
	int								max_interval;			//���ʱ��������λ����
	HCS_Event_Relate_Rule_Param		struRule1Raram;			//����1
	HCS_Event_Relate_Rule_Param		struRule2Raram;			//����2
	char							res2[36];

} HCS_Event_Combined_Rule;

//�����¼�����
typedef union 
{
	int								len[23];				//����
	HCS_Event_TraversePlan			traverse_plane;			//��Խ��������� 
	HCS_Event_Area    				area;					//����/�뿪�������
	HCS_Event_Intrusion				intrusion;				//���ֲ���
	HCS_Event_Loiter				loiter;					//�ǻ�����
	HCS_Event_take_left				take_left;				//��Ʒ����/��Ʒ��ȡ����
    HCS_Event_Parking				parking;				//ͣ������
	HCS_Event_Run					run;					//�����ƶ�����  *
	HCS_Event_High_Density			high_density;			//��Ա�ۼ�����  *
	HCS_Event_Violent_motion		violent_motion;			//�����˶�
	HCS_Event_Reach_Hight			reach_hight;			//�ʸ�
	HCS_Event_Get_Up				get_up;					//��
	HCS_Event_Left					left;					//��Ʒ����
	HCS_Event_Take					take;					// ��Ʒ��ȡ
	HCS_Event_Enter					human_enter;			//��Ա����
	HCS_Event_OverTime				over_time;				//������ʱ
	HCS_Event_Stick_Up 				stick_up;				//��ֽ��
	HCS_Event_Scanner 				scanner;				//���������� 	
	HCS_Event_LEAVE_POSITION		leave_pos;				//��ڲ���
	HCS_Event_Trail					trail;					//β�����
	HCS_Event_Fall_Down				fall_down;				//���ز���
	HCS_Event_Audio_Abnormal		audio_abnormal;			//��ǿͻ��
	HCS_Event_AdvReach_Height		adv_reach_height;		//�����ʸ߲���
	HCS_Event_Toiler_Tarry			toilet_tarry;			//��޳�ʱ����
	HCS_Event_Yard_Tarry			yard_tarry;				//�ŷ糡��������
	HCS_Event_AdvTraverse_Plane		adv_traverse_plane;		//���߾��������
	HCS_Event_Lecture				lecture;				//�ڿ��¼�
	HCS_Event_Answer				answer;					//�ش������¼�
	HCS_Event_Standup				stand_up;				//��������
	HCS_Event_PeopleNum_Change		people_num_change;		//�����仯����
	HCS_Event_Spacing_Change		spacing_change;			//���仯����
	HCS_Event_Combined_Rule			combined_rule;			//��Ϲ������

}HCS_SmartEventUnion;


/* @ struct [HCS_SmartEventSearchResult]
 * @ brief �����¼��������
 */
typedef struct
{
	int			maior_type;									//0-�ƶ���⣬1-��������, 2-�����¼� 5-pos¼��
	int			minor_type;									//����������- ���������ͱ仯��0xffff��ʾȫ��
	HCS_Time	start_time;									//�¼���ʼ��ʱ��
	HCS_Time	end_time;									//�¼���ʼ��ʱ��
	short		chanel[HCS_MAX_CHANNUM_V40/*512*/];			//������ͨ���ţ�0xffff��ʾ������Ч
	union		
	{
		char				len[800];						//�����峤��

		struct//����������
		{
			int				alarm_info_num;					//���������
			char			res[796];

		}AlarmRet;

		struct//�ƶ������
		{
			int				motion_channel;					//�ƶ����ͨ��
			char			res[796];

		}MotionRet;

		struct//��Ϊ�������  
		{
			int				channel;						//�����¼���ͨ����
			char			rule_id;						//����ID
			char			res1[3];						//����
			char			rule_name[HCS_NAME_LEN];		//��������
			HCS_SmartEventUnion smart_event;				//��Ϊ�¼�����
			char			res2[668];						//����

		}VcaRet;

		struct//��Ѷ�¼�
		{
			char			room_index;						//��Ѷ�ұ��,��1��ʼ
			char			drive_index;					//��¼�����,��1��ʼ
			char			res1[6];						//����
			int				segment_number;					//��Ƭ���ڱ�����Ѷ�е����,��1��ʼ 
			short			segment_size;					//��Ƭ�ϵĴ�С, ��λM 
			short			segment_status;					//��Ƭ��״̬ 0 ��¼������1 ��¼�쳣��2 ����¼��Ѷ
			char			res2[784];						//����

		}InquestRet;

		struct //��id¼���ѯ���
		{
			int				record_type;					//¼������ 0-��ʱ¼�� 1-�ƶ���� 2-����¼�� 3-����|�ƶ���� 4-����&�ƶ���� 5-����� 6-�ֶ�¼�� 7-�𶯱��� 8-�������� 9-���ܱ��� 10-�ش�¼��
			int				record_len;						//¼���С
			char			lock;							//������־ 0��û���� 1������
			char			frame_type;						//0���ǳ�֡¼�� 1����֡¼��
			char			res1[2];
			char			file_name[HCS_NAME_LEN];		//�ļ���
			int				file_index;						// �浵���ϵ��ļ�����
			char			res2[752];

		}StreamIDRet;

		struct//POS¼���ѯ���
		{
			int				channel;						//��������pos�¼���ͨ��
			char			res[796];

		}PosRet;

		struct
		{
			char			room_index;						//��Ѷ�ұ��,��1��ʼ
			char			drive_index;					//��¼�����,��1��ʼ
			short			segment_size;					//��Ƭ�ϵĴ�С, ��λM
			int				segment_index;					//��Ƭ���ڱ�����Ѷ�е����,��1��ʼ
			char			segment_status;					//��Ƭ��״̬��0-��¼������1-��¼�쳣��2-����¼��Ѷ
			char			case_type;						//�������ͣ�0-ȫ����1-���°�����2-���°���
			char			res1[2];
			char			case_number[HCS_CASE_NO_RET_LEN];	//�������
			char			case_name[HCS_CASE_NAME_RET_LEN];	//�������ƣ�
			char			litigant1[HCS_LITIGANT_RET_LEN];	//������1��
			char			litigant2[HCS_LITIGANT_RET_LEN];	//������2��
			char			chief_judge[HCS_CHIEF_JUDGE_RET_LEN];//���г�
			char			res2[600];

		}TrialRet;

	}SeniorRet;

	char					res[HCS_RESERVED_DATA_LEN];		//����

}HCS_SmartEventSearchResult;


/* @ struct [HCS_SmartSearchRet]
 * @ brief ������Ϣ
 */
typedef struct
{
	HCS_RectF	plate;                          // ����λ�ã�ע�������x,y��ŵ��ǳ��Ƶ���������(���ڽ�ģ)
	float		angle;                          // ������б�Ƕ�[���ڽ�ģ]
	int			color;							// ������ɫ[������ɫʹ�����ֱ�ʾ��1 - ����2 - �ơ�3 - �ס�4 - �ڡ�5 - ��]
	char		res[64];						// ��������

}HCS_PlateInfo;


/* @ struct [HCS_PicModel]
 * @ brief ����ͼƬ��Ϣ���ݽṹ����
 */
typedef struct 
{
	bool			use_model;					// �Ƿ����ó�������ģ��
	HCS_PlateInfo	plate_info;					// ��������ģ��
	char*			pic_buf;					// ͼƬ������Ϣ
	unsigned int	buf_len;					// ͼƬ���ݳ���
	float			similarity;					// ��ѯͼƬ�����ƶ�[0 - 1]
	char			res[64];					// ��������

}HCS_PicModel;

/* @ struct [HCS_SBPRet]
 * @ brief ��ͼ��ͼ���
 */
typedef struct
{
	char		camera_id[HCS_CAMERA_ID_LEN];	//������id
	char		url[HCS_PICTURE_URL_LEN];		//����ͼƬURL
	float		similarity;						//���ƶ�
	char		res[64];

}HCS_SBPRet;





extern "C"
{
	/** @ fn     RecDownloadCb
	 *  @ brief  ¼�����ػص�����
	 *  @ param  handle		- [in] ¼�����ز������
	 *  @ param  data		- [in] ¼������
	 *  @ param  data_size	- [in] ���ݴ�С
	 *  @ param  flag		- [in] ʣ������[0-���޸�������  1-����δ�ص�����]
	 *  @ param  error_code	- [in] ����������
	 *  @ param  user_data	- [in] �û��Զ�������
	 *  @ return void
	 */
	typedef void (*RecDownloadCb)(int handle, const char* data, int data_size, int flag, int error_code, void* user_data);

	/**@ fn     AttachedDownloadCb
	*  @ brief  �������ػص�����
	*  @ param  handle		- [in] �������ز������
	*  @ param  data		- [in] ��������
	*  @ param  data_size	- [in] ���ݴ�С
	*  @ param  begin_time	- [in] ��ʼʱ�䣨���룩
	*  @ param  end_time	- [in] ����ʱ�䣨���룩
	*  @ param  type		- [in] �������ͣ�10 ��������
	*  @ param  flag		- [in] ʣ������[0-���޸�������  1-����δ�ص�����]
	*  @ param  error_code	- [in] ����������
	*  @ param  user_data	- [in] �û��Զ�������
	*  @ return void
	*/
	typedef void (*AttachedDownloadCb)(int handle, const char* data, int data_size, HCS_Time begin_time,HCS_Time end_time,int type,int flag, int error_code, void* user_data);

	/**@ fn     RecHighSpeedDownloadCb
	*  @ brief  ¼��������ػص�����
	*  @ param  handle		- [in] ¼��������ز������
	*  @ param  data		- [in] ¼������
	*  @ param  data_size	- [in] ���ݴ�С
	*  @ param  index		- [in] ¼�����ݶ����,[indexΪ-1,flagΪ0��ʾ��������������]
	*  @ param  flag		- [in] ��ǰ��ʣ������[0-���޸�������  1-����δ�ص�����]
	*  @ param  error_code	- [in] ����������
	*  @ param  user_data	- [in] �û��Զ�������
	*  @ return void
	*/
	typedef void (*RecHighSpeedDownloadCb)(int handle, const char* data, int data_size, int index,int flag, int error_code, void* user_data);

   /** @ fn    RecFrameExtractCb
	*  @ brief ��֡��ʾ�ص�����
	*  @ param handle       - [in] ��ȡ��֡��ʾ�ľ��
	*  @ param data			- [in] ��֡��ʾ���� 
	*  @ param data_size	- [in] ��֡��ʾ���ݴ�С
	*  @ param pic_type		- [in] ͼƬ����
	*  @ param hcs_time  	- [in] ��ȡͼƬ��ʱ��
	*  @ param frame_width  - [in] ��֡��ʾ���[ͼƬ���]
	*  @ param frame_height - [in] ��֡��ʾ�߶�[ͼƬ�߶�]
	*  @ param flag         - [in] ��ǰ��ʣ������[0-���޸�������  1-����δ�ص�����]
	*  @ param error_code	- [in] ����������
	*  @ param user_data    - [in] �û��Զ������� 
	*  @ return void
	*/
	typedef void (*RecFrameExtractCb)(int handle, const char* data, long data_size, HCS_PicType pic_type, HCS_Time hcs_time,int frame_width, int frame_height, int flag,  int error_code, void* user_data);


	/** @ fn     PicDownloadCb
	 *  @ brief  ͼƬ���ػص�����
	 *  @ param  handle		- [in] ͼƬ���ز������
	 *  @ param  data		- [in] ͼƬ����
	 *  @ param  data_size	- [in] ͼƬ���ݴ�С
	 *  @ param  pic_type	- [in] ͼƬ����
	 *  @ param  flag		- [in] ʣ������[0-���޸�������  ����0-ʣ��Ĵ��ص���Ŀ��]
	 *  @ param  error_code	- [in] ����������
	 *  @ param  user_data	- [in] �û��Զ�������
	 *  @ return void
	 */
	typedef void (*PicDownloadCb)(int handle, const char* data, int data_size, int pic_type, int flag, int error_code, void* user_data);

	/**@ fn     TaskNotifyCb
	*  @ brief  ����֪ͨ�ص�����
	*  @ param  info		- [in] ������Ϣ
	*  @ param  flag		- [in] ʣ������[0-���޸�������  ����0-ʣ��Ĵ��ص���Ŀ��]
	*  @ param  error_code	- [in] ����������
	*  @ param  user_data	- [in] �û��Զ�������
	*  @ return void
	*/
	typedef void (*TaskNotifyCb)(HCS_TaskInfo *info, int flag, int error_code, void* user_data);

	/**@ fn     UploadFileSpeedCb
	*  @ brief  �첽�ϴ��ļ����Ȼص�����
	*  @ param  handle		- [in] �����ļ��ϴ����
	*  @ param  file_speed	- [in] �ļ��ϴ�����[0-100]
	*  @ param  error_code	- [in] ����������
	*  @ param  user_data	- [in] �û��Զ�������
	*  @ return void
	*/
	typedef void (*UploadFileSpeedCb)(int handle, int file_speed, int error_code, void* user_data);

}

#endif // HCS_SDK_DEFINE_H_
