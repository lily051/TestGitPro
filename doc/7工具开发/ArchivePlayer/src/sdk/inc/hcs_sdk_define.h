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

/** @ struct [HCS_StreamFileInfo]
 *  @ brief  ��Ƶ�ļ���Ϣ�ṹ��
 */
typedef struct 
{
	char				file_key[HCS_FILE_KEY_LEN];		// �ļ���
	unsigned long long	file_size;						// �ļ���С,��λ:�ֽ�
	HCS_Time			begin_time;						// �ļ���ʼʱ��
	HCS_Time			end_time;						// �ļ�����ʱ��

}HCS_StreamFileInfo;

/** @ struct [HCS_StreamFileWriteInfo]
 *  @ brief  ��Ƶ�ļ�����д��Ϣ�ṹ��
 */
typedef struct 
{
	char file_key[HCS_FILE_KEY_LEN];					// �ļ���
	char pool_id[HCS_POOL_ID_LEN];						// ��Դ��ID[���֧��32���ַ�]
	unsigned int replication;							// ������[0-������ 1-1������ 2-˫������]
	char rep_pool_id[HCS_POOL_ID_LEN];					// ������Դ��ID[���֧��32���ַ������������Ϊ0ʱ��Ч]
	char rec_head[HCS_HEAD_LEN];						// ¼��ͷ
	unsigned int head_size;								// ¼��ͷ����

}HCS_StreamFileWriteInfo;
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

#define HCS_MAX_POLYGON_VERTEX       10					// �������󶥵���
#define HCS_MAX_RULE_NUM             8					// ���֧�ְ�������
#define HCS_MAX_RULE_NUM_EX          16					// ��չ�������֧��ʮ��������
#define HCS_MAX_TARGET_NUM           30					// ���ͬʱ���30���ƶ�Ŀ��
#define HCS_MAX_SHIELD_NUM           4					// ����ĸ���������
#define HCS_MAX_PARAM_NUM            200				// ���ؼ���������
#define HCS_MAX_AUX_AREA_NUM         16					// �������������Ŀ


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
	unsigned int  rule_num;							// �����й�������
	HCS_Rule      rule[HCS_MAX_RULE_NUM];			// ��������

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


/* @ struct [HCS_SmartSearchRet]
 * @ brief �����������
 */
typedef struct
{
	HCS_Time	start_time;						// �ƶ���ⱨ����ʼ��ʱ��
	HCS_Time	end_time;						// �¼�ֹͣ��ʱ��
	char		res[64];

}HCS_SmartSearchRet;


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

}

#endif // HCS_SDK_DEFINE_H_
