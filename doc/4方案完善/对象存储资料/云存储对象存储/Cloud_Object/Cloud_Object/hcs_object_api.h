/** @file   hcs_object_api.h
*  @note	HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*  @brief	API Interface H File
*  @author  xiaweiqiang@hikvision.com.cn
*  @date	2015-01-04
*
*  @change log
*
*/

#ifndef _HCS_OBJECT_API_
#define _HCS_OBJECT_API_

#define HCS_OBJECT_API_EXPORTS

#if (defined _WIN32 || defined _WIN64)
#ifdef HCS_OBJECT_API_EXPORTS
#define HCS_OBJECT_API __declspec(dllexport)
#else
#define HCS_OBJECT_API __declspec(dllimport)
#endif
#else
#define HCS_OBJECT_API
#endif

/**
 * ��ʾhik����洢����ķ��ʿ����б�Access Control List, ACL),
 */
enum HCSAccessACL
{
	PUBLIC_READ_WRITE = 0,      //�ܶ���д
	PUBLIC_READ,				//ֻ��
	PRIVATE,					//˽��
};


/**
 * ����bucketʱ,��ָ��bucket�Ĵ洢����
 Ĭ�ϲ������汾�Ź���,��bucket�µ�����objectֻ��һ���汾
 ����������bucket�İ汾�ź�,ͬһ��bucket��ͬһ��key���Զ�Ӧ���object��ÿ��object��version��ͬ
 �ҿ�����ֻ�ܶ��Ժ���д������������
 �汾�Ź����ܹر�,ֻ�ܹ���,������֮����д������ֻ��һ���汾,��ǰд�����ݲ���
 */
enum HCSBucketVersionManage
{
	UNVERSIONED = 0,			//�������汾�Ź���
	VERSION_ENABLED,			//�����汾�Ź������
	VERSION_SUSPENDED,			//����汾�Ź������
};


/**
 * ��ȡobject�б������Ϣʱ,object��״̬����ʾ����Ϣ
 */
enum HCSObjectStatus
{
	STATUS_NORMAL = 0,			//����
	STATUS_UPLOADING,			//�����ϴ�
	STATUS_COPYING,				//���ڿ���
	STATUS_DELETED,				//��ɾ��
	STATUS_COPY_FAILED,			//����ʧ��
	STATUS_UPLOAD_ERROR,		//�ϴ�ʧ��
};


/**
 * �û��ϴ���ʽ�ļ�(��Ҫ����Ƶ�ļ�)ʱָ���Ķ���
 */
enum HCSStreamFileAct
{
	ACT_NOACT = 0,              //�޶���
	ACT_FRAME_ANALYZE,			//��Ҫ�ƴ洢��֡����
	ACT_TRANSCODE_FILE,			//��Ҫ�ƴ洢ת��
	ACT_ANALYZE_TRANSCODE,		//��Ҫ�ƴ洢֡������ת��
};


/**
 * �����װ��ʽ
 */
enum HCSOutputPacketType
{
	TYPE_SOURCE = 0,			//����ԭ��ʽ����
	TYPE_HIK,					//������װ
	TYPE_MPEG2_PS,				//MPEG2-PS
	TYPE_MPEG2_TS,				//MPEG2-TS
	TYPE_RTP,					//RTP
	TYPE_MP4_3GPP_MOV,			//MP4/3GPP/MOV
	TYPE_AVI,					//AVI
};


/**
 * �����Ƶ��װ��ʽ
 */
enum HCSVideoEncodeFormat
{
	VIDEO_SOURCE = 0,			//����ԭ��
	VIDEO_HIK264,				//����264
	VIDEO_MPEG2,				//mpeg2
	VIDEO_MPEG4,				//mpeg4
	VIDEO_H264,					//H264
	VIDEO_MJPEG,				//MJPEG
	VIDEO_MPEG4V1_V2_V3,		//MS_MPEG4V1/2/3
	VIDEO_WMV2,					//WMV2
};


/**
 * �����Ƶ��װ��ʽ
 */
enum HCSAudioEncodeFormat
{
	AUDIO_SOURCE = 0,			//����ԭ��
	AUDIO_MPGA,					//mpga
	AUDIO_AAC,					//AAC
	AUDIO_G711,					//G.711
	AUDIO_G722,					//G.722
	AUDIO_G7231,				//G.7231
	AUDIO_G726,					//G.726
	AUDIO_G729,					//G.729
	AUDIO_AMR_NB,				//AMR_NB
	AUDIO_WNAV2,				//WNAV2
	AUDIO_PCM,					//PCM
};


/**
 * ����ͼƬĿ���ʽ
 */
enum HCSImageOutputFormat
{
	FOAMAT_SOURCE = 0,//����ԭ�и�ʽ
	FORMAT_JPG,       //ת��ΪJPG
	FORMAT_PNG,       //ת��ΪPNG
	FORMAT_BMP,       //ת��ΪBMP
};


/**
 * ��ʱ���������Ƶ���ݵ������Ƶ��װ��ʽ
 */
enum HCSVideoOutPutFormat
{
	OUTPUT_VIDEO_SOURCE = 0,			//����ԭ��
	OUTPUT_VIDEO_HIK264,				//����264
	OUTPUT_VIDEO_PS,				    //ps
	OUTPUT_VIDEO_TX,				    //tx
	OUTPUT_VIDEO_RTP,					//rtp
	OUTPUT_VIDEO_MPEG4,					//MPEG4
};

/**
 * �û�ע��󣬷��ص��û���ʶָ��
 */
typedef struct _HCSClient 
{
	char *endpoint;				//�ƴ洢��������ڵ�ַ,������������д��������������дIP:PORT
	char *access_key;			//�ƴ洢���صĹ�Կ��Ϣ
	char *secret_key;			//�ƴ洢���ص�˽Կ��Ϣ
}HCSClient;


/**
 * Ԫ���ݼ�ֵ�Խṹ��
 */
typedef struct _KeyValuePair
{
	char *key;
	char *value;
}KeyValuePair;


/**
 * �ڻ�ȡBucket�б�ʱ���ص�Bucket��Ϣ
 */
typedef struct _HCSBucketInfo
{
	char *create_date;							//Bucket�Ĵ���ʱ��
	char *bucket_name;							//Bucket������
	char *owner;								//Bucket��������
	char *region;								//Bucket���ڵ�������Ϣ
	HCSAccessACL acl;							//Bucket��ACL
	HCSBucketVersionManage version_policy;		//Bucket�Ķ�汾�������
	int life_cycle;								//Bucket����������,��λΪ��(��δ����,ֵΪ-1,��ֵ��bucket�����汾����֮��������)
	int trash_del_cycle;						//Bucket�Ļ���վ����,��λΪ��(��δ����,ֵΪ0,��ֵ��ʾ��object�洢���ڵ���֮�����ڻ���վ�д洢��ʱ��)
	long long bucket_size;						//Bucket������,��λΪGB
	double free_size;				            //Bucket��ʣ������,��λΪGB
}HCSBucketInfo;


/**
 * Object���û��Զ���Ԫ����
 */
typedef struct _HCSObjectUserDefineMetadata
{
	int count;
	KeyValuePair *key_value_pair;
}HCSObjectUserDefineMetadata;


/**
 * Object��ϵͳ����Ԫ����,��object��С���洢����
 */
typedef struct _HCSObjectSystemDefineMetadata
{
	int count;
	KeyValuePair *key_value_pair;
}HCSObjectSystemDefineMetadata;


/**
 * Object������Ԫ������Ϣ
 */
typedef struct _HCSObjectMetadata
{
	HCSObjectUserDefineMetadata *user_def;		//�û��Զ���Ԫ���ݼ���
	HCSObjectSystemDefineMetadata *sys_def;		//ϵͳ����Ԫ���ݼ���
}HCSObjectMetadata;


/**
 * Object�ļ�Ҫ��Ϣ
 */
typedef struct _HCSObjectSummary
{
	char *key;					//Object������
	char *version_id;			//Object��Ӧ�İ汾ID
	int status;					//Object��Ӧ��hcs�е�״̬
	HCSAccessACL acl;			//Object��Ӧ��ACLֵ
	char *storage_class;		//Object�Ĵ洢����
	char *create_time;			//Object��Ӧ�Ĵ���ʱ��
	char *user;					//������object���û���
	long long size;				//Object�Ĵ�С
}HCSObjectSummary;


/**
 * Object���б�,�ڻ�ȡObject�б�ʱ����
 */
typedef struct _HCSObjectList
{
	char *bucket_name;			//Object���ڵ�Bucket����
	char *delimiter;			//ָ���ķ����
	char *prefix;				//ָ����Object��ǰ׺
	int   max_keys;				//ָ��һ�η��ص��������
	char *marker;				//ָ��Object��key�Ŀ�ʼ��־

	bool is_truncated;			//��ʶ�Ƿ���δ��ʾ��Object
	char *next_marker;			//������һ���ϵ�marker,���û�������delimiter,��is_truncatedΪtrueʱ����ֵ
	
	long summaries_count;		 //��������prefix��Object����
	HCSObjectSummary **summaries;//��������prefix��Object��Ϣ
	
	long common_prefix_count;	 //ָ��ǰ׺�ҵ�һ�γ���delimiter֮���object����
	char **common_prefixes;      //ָ��ǰ׺�ҵ�һ�γ���delimiter֮���object key����
}HCSObjectList;


/**
 * ����Object��������ṹ��
 */
typedef struct _HCSGetObjectRequest
{
	char *bucket_name;			  //Object���ڵ�Bucket����
	char *key;					  //Object����
	char *version_id;			  //object��Ӧ�İ汾��Ϣ,ΪNULL���ȡ����Object����
	long long start_pos;		  //��Ҫ��ȡ�Ŀ�ʼƫ��,��λΪ�ֽ�
	long long end_pos;            //��Ҫ��ȡ�Ľ���ƫ��λ��,��λΪ�ֽ�
}HCSGetObjectRequest;


/**
 * ��ȡObject�б���������ṹ��
 */
typedef struct _HCSListObjectRequest
{
	char *delimiter;			  //�ָ���
	char *marker;                 //��ʶ��ʾ�����ﷵ���б�
	unsigned int max_keys;        //���Ƶ���󷵻ؼ�¼��
	char *prefix;                 //����object���Ƶ�ǰ׺(�� aaa.txt,��ǰ׺���ó�a��aa��aaa��aaa.txt�ᱻ����)
}HCSListObjectRequest;


/**
 * ����Multipart�ϴ���Part�ķ��ؽ����Ϣ
 */
typedef struct _HCSMultipartUploadPart
{
	int part_number;        //Part��ʶ����
	int part_size;			//Part��Ӧ�Ĵ�С,���ֽ�Ϊ��λ
}HCSMultipartUploadPart;


/**
 * �������һ��Multipart�ϴ��¼����������
 */
typedef struct _HCSCompleteMultipartUploadRequest
{
	char *bucket_name;						//Bucket ����
	char *key;								//Object ����
	HCSMultipartUploadPart *part_info;      //��ʶ�ϴ�Part�Ľ��
	char *upload_id;						//��ʶMultipart�ϴ��¼���uploadId
	int part_info_num;						//part_info�ĸ���
}HCSCompleteMultipartUploadRequest;


/**
 * Object��Ӧ��ʱ��ε��б�,����ʽ�������͵�objectʹ�øýӿڲ���Ч
 */
typedef struct _HCSTimeSegmentInfo
{
	long long  size;				//�����ѯ������ʱ��εĴ�С��������Ϊ1��Ϊ�ܴ�С�����Ϊ0����ʾΪÿ���εĴ�С
	long long start_time;		 //�����ѯ������ʱ��εĿ�ʼʱ��
	long long end_time;		     //�����ѯ������ʱ��εĽ���ʱ��
}HCSTimeSegmentInfo;


/**
 * Object��Ӧ��ʱ��ε��б�,����ʽ�������͵�objectʹ�øýӿڲ���Ч
 */
typedef struct _HCSStreamTimeSegmentList
{
	long segment_count;		 //�������㿪ʼʱ�������ʱ���ʱ�������
	HCSTimeSegmentInfo *segment_list;
}HCSStreamTimeSegmentList;


/*typedef struct _HCSTimeSegmentInfoEx
{
	long long  size;				//�����̵Ĵ�С
	long long start_time;		 
	long long end_time;		     //�����ѯ������ʱ��εĽ���ʱ��
}HCSTimeSegmentInfoEx;

*/
/**
 * Object��Ӧ��ʱ��ε��б�,����ʽ�������͵�objectʹ�øýӿڲ���Ч
 */
typedef struct _HCSStreamTimeSegmentListEx
{
	long segment_count;		 //�������㿪ʼʱ�������ʱ���ʱ������� ���Ϊ1,��ʾֻ��һ����
	HCSTimeSegmentInfo *segment_list;//���Ϊ1������ֻ�洢һ���Σ��ҶεĴ�СΪ���жε��ܺͣ�
}HCSStreamTimeSegmentListEx;


/**
 * �·�ת��Ĳ�������
 */
typedef struct _HCSTranscodeParam
{
	char* output_bucket;//ת��֮����������bucket
	char* output_key;	//ת��֮����������key
	int packet_type;	//�ο�HCSOutputPacketType����ֵ˵��
	int video_format;	//�ο�HCSVideoEncodeFormat����ֵ˵��
	int audio_format;	//�ο�HCSAudioEncodeFormat����ֵ˵��
	int frame_width;	//���֡��
	int frame_height;	//���֡��
}HCSTranscodeParam;


/**
 * ת���������
 */
typedef struct _HCSTranscodeProgress
{
	char* output_bucket;//ת��֮����������bucket
	char* output_key;	//ת��֮����������key
	int status;	        //ת��״̬
	int progress;	    //ת�����
}HCSTranscodeProgress;



/**
 * ȡ���豸��Ϣ�ṹ��
 */
typedef struct _HCSStreamDevInfo
{
	char *user_name;
	char *passwd;
	char *url;
	int reserved1;
	char *reserved2;
}HCSStreamDevInfo;



#ifdef __cplusplus
extern "C"
{
#endif

    /** 
	*  ���ݿ�ʼʱ�����ʱ��������ʽ���ݻص���������
	*  @param buff[out]         ¼������
	*  @param bufflen[out]      ���ݳ��� 
	*  @param offset[out]       ��ǰ���������������������е�ƫ��
	*  @param error[out]        ����0: ��ǰ���سɹ�; С��0: ��ʾ���س��ִ���,ֵΪ������������
	*  @param pUser[out]        �û��Զ������� 
	*  @@return ����0: �����Ѵ���; С��0: ����δ����
	*/
	typedef int (*DownloadStreamDataCb)(const char* buff, int bufflen, long long offset, int error, void* p_user);
	
	  /** 
	*  ��ͨ�ļ�����
	*  @param buff[out]         ¼������
	*  @param bufflen[out]      ���ݳ��� 
	*  @param offset[out]       ��ǰ���������������������е�ƫ��
	*  @param error[out]        ����0: ��ǰ���سɹ�; С��0: ��ʾ���س��ִ���,ֵΪ������������
	*  @param pUser[out]        �û��Զ������� 
	*  @@return ����0: �����Ѵ���; С��0: ����δ����
	*/
	typedef int (*DownloadNormalDataCb)(const char* buff, int bufflen,int error, void* p_user);
	/**
	 * HCSClient �Ĺ��캯��
	 * @param access_id [in]  �û�ע��Ķ���洢�����û���
	 * @param access_key [in] �û�ע��Ķ���洢��������
	 * @param endpoint [in]   �ƴ洢�����������IP:PORT����www.hcs.com��192.168.1.1:80
	 * @return ����һ����ʶ���û���ݵĽṹָ��
	 * @retval �ǿ� ��ʾ�ɹ�
	 * @retval NULL ��ʾʧ��
	 */
	HCS_OBJECT_API HCSClient *ClientInitialize(const char *access_key, const char *secret_key, const char *endpoint);

	/**
	 * HCSClient ��������
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @retval void
	 * @pre client ����ʹ��ClientInitialize�ķ���ֵ
	 */
	HCS_OBJECT_API void ClientFinalize(HCSClient *client);

	/**
	 * ���� Bucket
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ������Bucket������
	 * @param region [in] Bucket�����������Ҫ�洢������,Ŀǰֻ֧��"LOCAL-1"
	 * @param bucket_size [in] Bucket������,��λΪGB
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ��0  ��ʾʧ��
	 */
	HCS_OBJECT_API int CreateBucket(HCSClient *client, const char *bucket_name, const char* region, long long bucket_size);
	
    /**
	 * ����bucket������
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ���õ�Bucket������
	 * @param bucket_size [in] �޸�֮�������ֵ,��λΪGB
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ��0  ��ʾʧ��
	 */
	HCS_OBJECT_API int SetBucketSize(HCSClient *client, const char *bucket_name, long long bucket_size);

	/**
	 * ����bucket�İ汾�Ź������
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ���õ�Bucket������
	 * @param policy [in] �޸�֮��İ汾�Ź������
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ��0  ��ʾʧ��
	 */
	HCS_OBJECT_API int SetBucketVersionPolicy(HCSClient *client, const char *bucket_name, HCSBucketVersionManage policy);

	/**
	 * ����bucket��object��������
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ���õ�Bucket������
	 * @param expiration [in] bucket�µ�object��������,��λΪ��,Ĭ�ϲ�����,����Ϊ-1���ʾ��������,����Ӧ��bucket�����˰汾�����������øù����ƴ洢�᷵�ض�Ӧ����
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ��0  ��ʾʧ��
	 */
	HCS_OBJECT_API int SetBucketLifecycle(HCSClient *client, const char *bucket_name, int expiration);

	/**
	 * ����bucket�»���վ�Ĵ洢����,��ʾbucket�µ�object�Ĵ洢���ڵ���֮������ʱ�ڻ���վ�д洢��ʱ��
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ���õ�Bucket������
	 * @param expiration [in] bucket�µĻ���վ�Ĵ洢����,��λΪ��,Ĭ����0,��ʾ����������վ����(bucket�洢���ڵ��ں�object����������ɾ��)
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ��0  ��ʾʧ��
	 */
	HCS_OBJECT_API int SetBucketTrashDelcycle(HCSClient *client, const char *bucket_name, int expiration);

	/**
	 * ɾ�� Bucket,��bucket����û���κ�objectʱ����ɾ��
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫɾ����Bucket������
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ��0  ��ʾʧ��
	 */
	HCS_OBJECT_API int DeleteBucket(HCSClient *client, const char *bucket_name);

	/**
	 * ����������ӵ�е����� Bucket ���б�
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param buckets_number [out] ����Bucket������
	 * @buckets_list [out] ����һ��HCSBucketInfo��˫��ָ�룬��ͨ��buckets_num������,�������֮���û����ͷŸò����ڴ�
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int ListBuckets(HCSClient *client, int *buckets_num, HCSBucketInfo **buckets_list);

	/** FreeHCSBucketInfo
	 *  �ͷſ��ж�̬�������Դ
	 *  @param client[in] �ͻ����Ӿ��
	 *  @param buckets_num[in out] ��Դ����
	 *  @param buckets_info[in out] ���ͷŵ���Դ
	 *  @return �ɹ����� HCS_OK�� ʧ�ܷ��ش�����
	 */
	HCS_OBJECT_API int FreeHCSBucketInfo(HCSClient *client, int *buckets_num, HCSBucketInfo** buckets_info);

	/**
	 * ����ָ�� Bucket �� Access Control List(ACL)
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name Ҫ����Ȩ�޵�Bucket������
	 * @param acl Ҫ���õ�Ȩ��
	 * @retval 0     �ɹ�
	 * @retval ����  ʧ��
	 */
	HCS_OBJECT_API int SetBucketAcl(HCSClient *client, const char *bucket_name, HCSAccessACL acl);

	/**
	 * ���ظ��� Bucket �� ��ϸ��Ϣ
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ��ȡ��Bucket����
	 * @param bucket_info [out] ���ص�Bucket����ϸ��Ϣ,��ȡ����û����ͷŸò����ڴ�
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int GetBucketInfo(HCSClient *client, const char *bucket_name, HCSBucketInfo **bucket_info);

	/**
	 * �ϴ�ָ����object��ָ���� Bucket,ע��ýӿ�ֻ���ϴ�С��100M���ļ�,����100M���ļ�������÷�Ƭ�ϴ��ӿ�
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ�ϴ�����Bucket����
	 * @param key [in] Ҫ�ϴ�������������ʾ��Object������
	 * @param file_handle [in] Ҫ�ϴ����ļ����
	 * @param metadata [in] ��ʶ�û��Զ����object ��һЩԪ������Ϣ
	 * @param storage_class [in] ��ʶobject�Ĵ洢����,Ŀǰֻ֧��"STANDARD"
	 * @param object_type [in] ��ʶobject������,��"application/xml",��ֵΪ����Ĭ��Ϊ"application/octet-stream"
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int PutObjectFromFile(HCSClient *client, const char *bucket_name, const char *key, FILE *file_handle, HCSObjectUserDefineMetadata *metadata, const char* storage_class, const char* object_type);

	/**
	 * �ϴ�ָ����object��ָ���� Bucket
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ�ϴ�����Bucket����
	 * @param key [in] Ҫ�ϴ�������������ʾ��Object������
	 * @param data [in] Ҫ�ϴ�������
	 * @param data_len [in] Ҫ�ϴ������ݵĳ���
	 * @param metadata [in] ��ʶobject ��һЩԪ��Ϣ
	 * @param storage_class [in] ��ʶobject�Ĵ洢����,Ŀǰֻ֧��"STANDARD"
	 * @param object_type [in] ��ʶobject������,��"application/xml",��ֵΪ����Ĭ��Ϊ"application/octet-stream"
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int PutObjectFromBuffer(HCSClient *client, const char *bucket_name, const char *key, void *data, long long data_len, HCSObjectUserDefineMetadata *metadata, const char* storage_class, const char* object_type);

	/**
	 * ����ָ�� object �� Access Control List(ACL)
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name Ҫ����Ȩ�޵�Bucket������
	 * @param key Ҫ���õ�object��keyֵ
	 * @retval version_id     ��Ҫ���õ�object�Ķ�Ӧ�汾��,������ΪNULL,���ʾ���ĵ�ǰ���°汾��ACLֵ
	 * @param acl Ҫ���õ�Ȩ��
	 * @retval ����  ʧ��
	 */
	HCS_OBJECT_API int SetObjectAcl(HCSClient *client, const char *bucket_name, const char *key, const char *version_id, HCSAccessACL acl);

	/**
	 * ���ظ��� object �����м�Ҫ��Ϣ,��bucket�İ汾��������Ѿ���,��Ҳ�᷵�ض�Ӧ��ʷ�汾��Ϣ
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ��ȡ��Bucket����
	 * @param key Ҫ��ȡ��object��keyֵ
	 * @param list_count ��ȡ�ɹ���object����
	 * @retval summary     ��ȡ֮���ժҪ��Ϣ,��ȡ���֮���û���Ҫ�ͷŸ�ָ��ָ��������ڴ�
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int ListObjectSummary(HCSClient *client, const char *bucket_name, const char *key, int *list_count, HCSObjectSummary **summary);

	/**
	 * ��ʾ�����ͷſ��ж�̬�����HCSObjectSummary��Դ
	 * @param list_count[in out] ����ĸ���
	 * @param summary[in out] ����Ĵ�ŵ�ַ��ָ��
	 * @return �ɹ�����HCS_OK�� ʧ�ܷ��ش�����
	 */
	HCS_OBJECT_API int FreeHCSObjectSummary(HCSClient *client, int *list_count, HCSObjectSummary **summary);

	/**
	 * ���������÷���bucket���������object��Ϣ,�ýӿ�ֻ�������°汾object��Ϣ,������ȡobject����ʷ����,�����ListObjectSummary�ӿ�
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ��ȡ��Bucket����
	 * @param list_params [in] ��ȡ�б�ʱ���õĲ���
	 * @retval summary     ��ȡ֮���ժҪ��Ϣ,��ȡ���֮���û���Ҫ�ͷŸ�ָ��ָ��������ڴ�
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int ListObjects(HCSClient *client, const char *bucket_name, HCSListObjectRequest *list_params, HCSObjectList **list);

	/**
	 * ��ʾ�����ͷſ��ж�̬�����HCSObjectList��Դ
	 * @param client �û����Ӿ��
	 * @param summary[in out] ����Ĵ�ŵ�ַ��ָ��
	 * @return �ɹ�����HCS_OK�� ʧ�ܷ��ش�����
	 */
	HCS_OBJECT_API int FreeHCSObjectList(HCSClient *client, HCSObjectList **summary);

	/**
	 * ���� Object ��Ԫ����
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name Ҫ��ѯ��Object���ڵ�Bucket����
	 * @param key Ҫ��ѯ��Object����
	 * @param version_id Ҫ��ȡ��object��Ӧ�İ汾��,������ΪNULL,�򷵻�����object��metadata
	 * @param metadata [out] ���ص�objectԪ������Ϣ,��ȡ��������0���û����ͷŸ��ڴ�
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int GetObjectMetadata(HCSClient *client, const char *bucket_name, const char *key, const char *version_id, HCSObjectMetadata **metadata);

	/**
	 * ��ʾ�����ͷſ��ж�̬�����HCSObjectMetadata��Դ
	 * @param client �û����Ӿ��
	 * @param metadata[in out] ���ͷŵ�Ԫ������Դ
	 * @return �ɹ�����HCS_OK�� ʧ�ܷ��ش�����
	 */
	HCS_OBJECT_API int FreeHCSObjectMetadata(HCSClient *client, HCSObjectMetadata **metadata);


	/**
	 * ��ָ���� Bucket �е���ָ����Object��Ŀ���ļ�
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param request [in] ��Ҫ��ȡ��object��һЩ��������
	 * @param file_handle [in] Ҫ���Object���ļ�ָ��
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int AsyncGetObject(HCSClient *client,HCSGetObjectRequest *request, DownloadNormalDataCb download_callbk,void *p_user);


	/**
	 * ��ָ���� Bucket �е���ָ����Object��Ŀ���ļ�
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param request [in] ��Ҫ��ȡ��object��һЩ��������
	 * @param file_handle [in] Ҫ���Object���ļ�ָ��
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int GetObjectToFile(HCSClient *client,HCSGetObjectRequest *request, void *file_handle);

	/**
	 * ��ָ����Bucket�е���ָ����Object ���ڴ�
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param request [in] ��Ҫ��ȡ��object��һЩ��������
	 * @param output [in] Ҫ���Object���ڴ��ַ,���ڴ���hcs����,�ɹ������û��ͷ�
	 * @param output_len [out] ����ֵΪ��������С�����ֵΪʵ�ʷ��ص����ݳ���
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int GetObjectToBuffer(HCSClient *client, HCSGetObjectRequest *request, void **output, unsigned int *output_len);

	/**
	 * ��ʾ�����ͷſ��ж�̬�����buffer��Դ
	 * @param client[in] �û����Ӿ��
	 * @param output[in out] ���ͷŵ�buffer��Դ
	 * @return �ɹ�����HCS_OK�� ʧ�ܷ��ش�����
	 */
	HCS_OBJECT_API int FreeBuffer(HCSClient *client, void **output);

	/**
	 * ����һ�����ƴ洢���Ѿ����ڵ� Object ������һ�� Object
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param source_bucket_name ��������Object���ڵ�Bucket������
	 * @param source_key ��������Object������
	 * @param destination_bucket_name Ŀ��Object���ڵ�Bucket������
	 * @param destination_key Ŀ��Object������
	 * @param metadata Ŀ��Object��Ԫ���ݶ���
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int CopyObject(HCSClient *client, const char *source_bucket_name, const char *source_key, const char *destination_bucket_name, const char *destination_key, HCSObjectUserDefineMetadata *metadata);

	/**
	 * ɾ��ָ���� Object
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫɾ��Object���ڵ�Bucket������
	 * @param key [in] Ҫɾ��Object������
	 * @param version_id [in] Ҫɾ��Object�İ汾ID��ΪNULL���ʾɾ�����°汾(����ͨ��list_object��ã�����ͨ��get_object�е�metadata�л��)
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int DeleteObjectByName(HCSClient *client, const char *bucket_name, const char *key, const char *version_id);

	/**
	 * ��ʼ��һ��multipart�ϴ�����
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] bucket������
	 * @param key [in]  object��keyֵ
	 * @param metadata [in]  object���û��Զ���Ԫ����
	 * @param storage_class [in]  object�Ĵ洢����,Ŀǰֻ֧��"STANDARD"
	 * @param object_type [in] ��ʶobject������,��"application/xml",��ֵΪ����Ĭ��Ϊ"application/octet-stream"
	 * @param error_code [out]  �ӿڷ��ش�����. 0:�ɹ� ����ֵ:ʧ��
	 * @retval �ǿ� ��ʾ�ɹ����ҷ�������ID
	 * @retval NULL ��ʾʧ��
	 */
	HCS_OBJECT_API char *InitiateMultipartUpload(HCSClient *client, const char *bucket_name, const char *key, HCSObjectUserDefineMetadata *metadata, const char* storage_class, const char* object_type, int *error_code);

	/**
	 * �ϴ�һ���ֿ�(part  ��Դ����)
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] bucket������
	 * @param key [in]  object��keyֵ
	 * @param task_id [in] initiate_multipart_upload����
	 * @param part_index [in] Ҫ�ϴ���part����
	 * @param part_data [in] Ҫ�ϴ���part������(�Ƽ�2M��С����Ϊ���һƬ����������)
	 * @param part_data_len [in] part���ݵĳ���
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int UploadPart(HCSClient *client, const char *bucket_name, const char *key, const char *task_id, int part_index, void *part_data, int part_data_len);

	/**
	 * ��ֹһ��multipart�ϴ�����
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] bucket������
	 * @param key [in]  object��keyֵ
	 * @param task_id [in] initiate_multipart_upload����
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int AbortMultipartUpload(HCSClient *client, const char *bucket_name, const char *key, const char *task_id);

	/**
	 * �ͷ�multipart�ϴ�����
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param task_id [in] initiate_multipart_upload����
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int MultipartUploadFinalize(HCSClient *client, HCSCompleteMultipartUploadRequest *request);

	/**
	 * ��ȡ�Ѿ��ϴ��ɹ��ķ�Ƭ��Ϣ
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] bucket������
	 * @param key [in]  object��keyֵ
	 * @param task_id [in] initiate_multipart_upload����
	 * @param result [out] ���з�Ƭ��Ϣ������,��hcs��ֵ�����ڴ�,�ɵ������ͷ��ڴ�(free)
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int ListPart(HCSClient *client, const char *bucket_name, const char *key, const char *task_id, HCSCompleteMultipartUploadRequest **result);

	/**
	 * ��ʾ�����ͷſ��ж�̬�����HCSCompleteMultipartUploadRequest��Դ
	 * @param client[in] �û����Ӿ��
	 * @param result[in] ���ͷŵ�HCSCompleteMultipartUploadRequest��Դ
	 * @return �ɹ�����HCS_OK�� ʧ�ܷ��ش�����
	 */
	HCS_OBJECT_API int FreeHCSCompleteMultipartUploadRequest(HCSClient *client, HCSCompleteMultipartUploadRequest **result);

    /**
	 * object����׷��
	 * @param client[in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ�ϴ�����Bucket����
	 * @param object_key [in] Ҫ�ϴ�������������ʾ��Object������
	 * @param data [in] Ҫ�ϴ�������
	 * @param data_len [in] Ҫ�ϴ������ݵĳ��ȣ�����С��0 ���� ����16MBʱ�򣬺�����ֱ�ӷ��� INVALID_ARGUMENT
	 * @param position[in] ������������object�еĿ�ʼλ��,��λΪ�ֽ�
	 * @param next_position[out] �´��û�Ӧ�ôӺδ���ʼִ��append����
	 * @return �ɹ�����HCS_OK�� ʧ�ܷ��ش����롣�ɹ� ���� ������Ϊ PositionNotEqualToLength ʱ��next_position��ʾ���´�append����ȷposition��
	 */
	HCS_OBJECT_API int AppendObject(HCSClient* client, const char* bucket_name, const char* object_key, const char* data, int data_len, long long position, long long* next_position);

	/**
	 * ��һ����ʽ���,�ýӿ������Ƶ�����ϴ�Ӧ��
	 * @param client[in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ�ϴ�����Bucket����
	 * @param object_key [in] Ҫ�ϴ�������������ʾ��Object������
	 * @param stream_handle [out] �ƴ洢���ص������,���ڴ��ɵ���������,�������һ��������64���ַ����ַ���
	 * @return �ɹ�����HCS_OK�� ʧ�ܷ��ش�����
	 */
	HCS_OBJECT_API int OpenStream(HCSClient* client, const char* bucket_name, const char* object_key, char* stream_handle);

	/**
	 * ��ʼ�ϴ�������
	 * @param client[in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ�ϴ�����Bucket����
	 * @param object_key [in] Ҫ�ϴ�������������ʾ��Object������
	 * @param stream_handle [in] �ƴ洢���ص������,�ò����ɵ���OpenStream�ӿڻ��
	 * @param stream_data [in] ��Ҫ�ϴ�������
	 * @param data_len [in] ��Ҫ�ϴ������ݵĳ��ȣ�����С��0 ���� ����16MBʱ�򣬺�����ֱ�ӷ��� INVALID_ARGUMENT
	 * @return �ɹ�����HCS_OK�� ʧ�ܷ��ش�����
	 */
	HCS_OBJECT_API int UploadStreamData(HCSClient* client, const char* bucket_name, const char* object_key, const char* stream_handle, const char* stream_data, int data_len);

	/**
	 * �ر�һ����ʽ���,���û�ֹͣ��ʽ�ļ�д��ʱ����
	 * @param client[in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ�ϴ�����Bucket����
	 * @param object_key [in] Ҫ�ϴ�������������ʾ��Object������
	 * @param stream_handle [in] �����,�ò����ɵ���OpenStream�ӿڻ��
	 * @return �ɹ�����HCS_OK�� ʧ�ܷ��ش�����
	 */
	HCS_OBJECT_API int CloseStream(HCSClient* client, const char* bucket_name, const char* object_key, const char* stream_handle);

	/**
	 * ͨ��ʱ��η�Χ������object�����е�ʱ�����Ϣ
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ��ȡ��Bucket����
	 * @param object_key [in] Ҫ��ȡ��object����
	 * @param start_time [in] Ҫ��ȡ��object�Ŀ�ʼʱ��
	 * @param end_time [in] Ҫ��ȡ��object�Ľ���ʱ��
	 * @param segment_list [out] ��ȡ��������ʱ�����Ϣ����ȡ���֮���û���Ҫ�ͷŸ�ָ��ָ��������ڴ�
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int QueryObjectTimesegment(HCSClient *client, const char *bucket_name, const char *object_key, long long start_time, long long end_time, HCSStreamTimeSegmentList **segment_list);
	/**
	 * ͨ��ʱ��η�Χ������object�����е�ʱ�����Ϣ
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ��ȡ��Bucket����
	 * @param object_key [in] Ҫ��ȡ��object����
	 * @param start_time [in] Ҫ��ȡ��object�Ŀ�ʼʱ��
	 * @param end_time [in] Ҫ��ȡ��object�Ľ���ʱ��
	 * @param segment_list [out] ��ȡ��������ʱ�����Ϣ����ȡ���֮���û���Ҫ�ͷŸ�ָ��ָ��������ڴ�
	 * @param  flag [int] �������ͣ�1����һ����0���ض����
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int QueryObjectTimesegmentEx(HCSClient *client, const char *bucket_name, const char *object_key, long long start_time, long long end_time, HCSStreamTimeSegmentListEx **segment_list,int flag);
    /**
	 * ��ʾ�ͷſ��ж�̬�����HCSStreamTimeSegmentList��Դ
	 * @param client[in] �û����Ӿ��
	 * @param segment_list[in] ���ͷŵ�HCSStreamTimeSegmentList��Դ
	 * @return �ɹ�����HCS_OK�� ʧ�ܷ��ش�����
	 */
	HCS_OBJECT_API int FreeStreamTimeSegmentList(HCSClient *client, HCSStreamTimeSegmentList **segment_list);
	HCS_OBJECT_API int FreeStreamTimeSegmentListEx(HCSClient *client, HCSStreamTimeSegmentListEx **segment_list);

	/**
	 * ͨ��ʱ��η�Χ����Object����
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ��ȡ��Bucket����
	 * @param object_key [in] Ҫ��ȡ��object����
	 * @param start_time [in] Ҫ��ȡ��object�Ŀ�ʼʱ��
	 * @param end_time [in] Ҫ��ȡ��object�Ľ���ʱ��
	 * @param segment_list [out] ��ȡ��������ʱ�����Ϣ����ȡ���֮���û���Ҫ�ͷŸ�ָ��ָ��������ڴ�
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int DownloadStreamData(HCSClient *client, const char *bucket_name, const char *object_key, long long start_time, long long end_time, HCSVideoOutPutFormat output_format, DownloadStreamDataCb download_callbk, void *p_user);

	/**
	 * �·�ת������ӿ�
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ��ȡ��Bucket����
	 * @param object_key [in] Ҫ��ȡ��object����
	 * @param param_list [in] ����������б�
	 * @param param_count [in] ����������б��Ԫ�ظ���
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int SendTranscodeTask(HCSClient *client, const char *bucket_name, const char *object_key, HCSTranscodeParam *param_list, int param_count);

	/**
	* �·�ת������ӿ�
	* @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	* @param bucket_name [in] Ҫ��ȡ��Bucket����
	* @param object_key [in] Ҫ��ȡ��object����
	* @param param_list [in] ����������б�
	* @param param_count [in] ����������б��Ԫ�ظ���
	* @retval 0	��ʾ�ɹ�
	* @retval ���� ��ʾʧ��
	*/
	HCS_OBJECT_API int SendTranscodeTaskEx(HCSClient *client, const char *bucket_name, const char *object_key,const  char *callback_ip_port, HCSTranscodeParam *param_list, int param_count);
	/**
	 * ɾ��ת��ӿ�
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫɾ��ת���Bucket���ƣ���ֵ���·�ת�������е�Դbucket
	 * @param object_key [in] Ҫɾ��ת���object����,��ֵ���·�ת�������е�Դkey
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int DeleteTranscodeTask(HCSClient *client, const char *bucket_name, const char *object_key);

	/**
	 * ��ѯת��״̬����Ƚӿ�
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ��ȡ��Bucket����
	 * @param object_key [in] Ҫ��ȡ��object����,��ֵ�����·�ת����������е�output_key
	 * @param output_key [in] ת��֮��Ķ����Ӧ��key
	 * @param progress [out] ��key��Ӧ��ת������б�ṹ��
	 * @param count [out] �����б�ṹ������
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	
	HCS_OBJECT_API int QueryTranscodeTaskProgress(HCSClient *client, const char *bucket_name, const char *object_key, HCSTranscodeProgress** progress, int* count);

	/**
	 * ��ʾ�ͷſ��ж�̬�����HCSTranscodeProgress��Դ
	 * @param client[in] �û����Ӿ��
	 * @param progress[in out] ���ͷŵ�HCSTranscodeProgress��Դ�б�
	 * @param count[in out] ���ͷ�HCSTranscodeProgress�ṹ��ĸ���
	 * @return �ɹ�����HCS_OK�� ʧ�ܷ��ش�����
	 */
	HCS_OBJECT_API int FreeTranscodeProgress(HCSClient *client, HCSTranscodeProgress** progress, int* count);

	/**
	 * ָ���ƴ洢���տ�߻��߸�ʽ����ͼƬ����
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Ҫ��ȡ��Bucket����
	 * @param object_key [in] Ҫ��ȡ��object����
	 * @param dest_width [in] Ҫ���ص�ͼƬ���ݵĿ�
	 * @param dest_high [in] Ҫ���ص�ͼƬ���ݵĸ�
	 * @param change_format [in] Ҫ���ص�ͼƬ���ݵĸ�ʽ
	 * @param image_data [out] �ƴ洢���صľ����ͼƬ����,�ڴ����û��ͷ�
	 * @param image_data_len [out] �ƴ洢���ص�ͼƬ���ݵĳ���
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int GetImageDataWithWidthAndHigh(HCSClient *client, const char *bucket_name, const char *object_key, int dest_width, int dest_high, HCSImageOutputFormat change_format, char **image_data, unsigned int *image_data_len);

	/**
	 * �·������ֶ�¼������
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] �ֶ�¼��洢��Bucket����
	 * @param object_key [in] �ֶ�¼��洢��object����
	 * @param param [in] ȡ���豸��Ϣ
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int StartManualRecord(HCSClient *client, const char *bucket_name, const char *object_key, HCSStreamDevInfo *param);
	/**
	 * �·������ֶ�¼������
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] �ֶ�¼��洢��Bucket����
	 * @param object_key [in] �ֶ�¼��洢��object����
	 * @param callback_ip [in] �ֶ�¼��洢���·���ip
	 * @param param [in] ȡ���豸��Ϣ
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int StartManualRecordEx(HCSClient *client, const char *bucket_name, const char *object_key,const  char *callback_ip_port, HCSStreamDevInfo *param);	

	/**
	 * �·������ֶ���¼����,��Ҫ�����ʷ����
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] �ֶ���¼���������洢��Bucket����
	 * @param object_key [in] �ֶ���¼���������洢��object����
	 * @param param [in] ȡ���豸��Ϣ
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int StartManualHistoryRecord(HCSClient *client, const char *bucket_name, const char *object_key, HCSStreamDevInfo *param);

	/**
	 * �·������ֶ���¼����,��Ҫ�����ʷ����
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] �ֶ���¼���������洢��Bucket����
	 * @param object_key [in] �ֶ���¼���������洢��object����
	 * @param param [in] ȡ���豸��Ϣ
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int StartManualHistoryRecordEx(HCSClient *client, const char *bucket_name, const char *object_key, const  char *callback_ip_port,HCSStreamDevInfo *param);
	/**
	 * �·�ֹͣ¼������,�ýӿڿ���ֹͣ�ֶ�¼������ֶ���¼
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Bucket����
	 * @param object_key [in] object����
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int StopRecord(HCSClient *client, const char *bucket_name, const char *object_key);

	/**
	 * ��ѯ¼��״̬,�ýӿڿ��Բ�ѯ¼������ֶ���¼����״̬
	 * @param client [in] ��ʶһ��HCSClient�Ľṹָ��
	 * @param bucket_name [in] Bucket����
	 * @param object_key [in] object����
	 * @param status [out] ���ظ��û���״ֵ̬
	 * @retval 0    ��ʾ�ɹ�
	 * @retval ���� ��ʾʧ��
	 */
	HCS_OBJECT_API int QueryRecordStatus(HCSClient *client, const char *bucket_name, const char *object_key, int* status);

    /**
	 * ��ȡhcs����ӿڰ汾����Ϣ
	 * @retval �ǿ�    ��ʾ�ɹ�
	 * @retval ����    ��ʾʧ��
	 */
	HCS_OBJECT_API const char * const GetVersions();

#ifdef __cplusplus
}//end of extern "C"


#endif

#endif //_HCS_OBJECT_API_
