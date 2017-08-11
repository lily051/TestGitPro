#ifndef USB_INTERFACE
#define USB_INTERFACE

#include <list>

#if (defined _WIN32 || defined _WIN64)
#   ifdef USB_INTERFACE_EXPORTS
#       define AE_USB_EXTERN __declspec(dllexport)
#   else
#       define AE_USB_EXTERN __declspec(dllimport)
#   endif
#   define AE_USB_API __stdcall
#else
#   ifdef __linux__
#       define DAG_EXTERN
#   endif
#   define DAG_API
#endif

#ifdef __cplusplus
#   define USB_EXTERN_C_BEGIN extern "C" {
#   define USB_EXTERN_C_END }
#endif

#ifdef __cplusplus
USB_EXTERN_C_BEGIN
#endif

#define FILE_PATH_MAX			512
#define EXT_INFO_MAX			64
#define USB_SDK_OK				0
#define INVALID_STREAM_HANDLE	-1

#define AEUSB_ERROR_UNKNOWN		-1//δ֪����
#define AEUSB_OK				0//�ɹ�
#define AEUSB_LOGINFAIL			1//��¼ʧ��,�û������������
#define AEUSB_ERROR_CMD			2//���ʶ��
#define AEUSB_ERROR_JSON		3//json������
#define AEUSB_ERROR_CHAN		4//ͨ���Ŵ���
#define AEUSB_CAP_FAIL			5//ץͼʧ��
#define AEUSB_GET_ABILITY_FAIL	6//��ȡ������ʧ��
#define AEUSB_SET_PARAM_FAIL	7//���ò���ʧ��
#define AEUSB_GET_PARAM_FAIL	8//��ȡ����ʧ��
#define AEUSB_UNKNOW_CMD		9//δָ֪��
#define AEUSB_TIMEOUT			258//��ʱ

//�ӿڵ���

typedef void*			USB_HANDLE;
typedef void*			USB_DEVICE;
typedef int				STREAM_HANDLE;

struct usb_simple_device
{
	char		szFileName[FILE_PATH_MAX];//·��
	char		szDescriptor[FILE_PATH_MAX];//������
	char		szExtRes[EXT_INFO_MAX];//������Ϣ.�ݶ�Ϊjson����ʽ
	USB_DEVICE	pDev;//�豸

	usb_simple_device* pRev;//ǰһ��
	usb_simple_device* pNext;//��һ��
};

typedef struct
{
	char*		pCondBuf;///��������
	int			nCondBufLen;//�������ݳ���
	char		szExtRes[EXT_INFO_MAX];//��չ��Ϣ
} DevSetParams;

typedef struct
{
	char*		pCondBuf;//��������
	int			nCondBufLen;//�������ݳ���
	char* 		pOutBuf;//���ص����ݽ��
	int			nOutBufLen;//���ص����ݽ����ָ������
	int*		nOutRetLen;//���ص����ݽ����ʵ�ʳ���
	char		szExtRes[EXT_INFO_MAX];//��չ��Ϣ
} DevGetParams;

typedef struct
{
	char*		pCondBuf;//��������
	int			nCondBufLen;//�������ݳ���
	char* 		pOutBuf;//���ص����ݽ��
	int			nOutBufLen;//���ص����ݽ����ָ������
	int*		nOutRetLen;//���ص����ݽ����ʵ�ʳ���
	char		szExtRes[EXT_INFO_MAX];//��չ��Ϣ
} DevAbilityParams;

typedef struct
{
	int			nChannel;//ͨ����
	char*		pPicBuff;//���ص�ͼƬ����
	int			nInBufLen;//���ص�ͼƬ���� ָ������,һ�㲻�ᳬ��2M
	int*		nOutBufLen;//���ص�ͼƬ���� ʵ�ʳ���
	char		szExtRes[EXT_INFO_MAX];//��չ��Ϣ
} DevCapPicture;

typedef struct
{
	int			nChannel;//ͨ����
	int			nType;//����or���� 0-main, 1-sub
	int			nErrorCode;//ʧ�ܶ�Ӧ������
	char		szExtRes[EXT_INFO_MAX];//��չ��Ϣ
} DevRealPlay;

/**
 * �豸״̬�ص�����
 * @param[out] pHandle-�Ự���
 * @param[out] nStatus-��¼״̬
 * @param[out] pUser-AEUSB_SetDevStatusCallBackʱ���õ�user
 */
typedef void (*tfnOnDevStatus)(USB_HANDLE pHandle, int nStatus, void* pUser);
/**
 * ʵʱԤ���ص�����
 * @param[out] hRealPlayHandle-ʵʱԤ���ľ��,��AEUSB_StartRealPlayStream����
 * @param[out] nDataType- 0-head, 1-video data
 * @param[out] pBuffer-��Ƶ����
 * @param[out] nRetLen-��Ƶ���ݴ�С
 * @param[out] pUser-AEUSB_StartRealPlayStreamʱ���õ�user
 */
typedef void (*tfnRecvRealPlay)(STREAM_HANDLE hRealPlayHandle, int nDataType, char* pBuffer, int nRetLen, void* pUser);

/**
 * ��Դ��ʼ��
 * @return: AEUSB_OK - succ, ����-������
 */
AE_USB_EXTERN int AEUSB_Initialize();

/**
 * ��Դ����ʼ��
 * @return: AEUSB_OK - succ, ����-������
 */
AE_USB_EXTERN int AEUSB_Finalize();

/**
 * ��ȡ����USB�豸
 * @return: ��⵽���豸����
 * @param[in] pDevList-�豸�б�, ���»�ȡ�б��, ֮ǰ��ָ�벻����Ч
 */
AE_USB_EXTERN int AEUSB_GetUsbDevList(usb_simple_device*& pDevList);

/**
 * �豸��¼
 * @return: �Ự���,�����Ժ�����豸, ʧ�ܷ���NULL
 * @param[in] pDev-�豸,��ȡ�豸�б�ʱ�����õ�
 * @param[in] pUser-�û���
 * @param[in] nUserLen-�û�������
 * @param[in] pPwd-����
 * @param[in] nPwdLen-���볤��
 */
AE_USB_EXTERN USB_HANDLE AEUSB_LogIn(USB_DEVICE pDev, char* pUser, int nUserLen, char* pPwd, int nPwdLen);

/**
 * ��¼״̬�Ļص�.�����Զ����ߵ��쳣�����,״̬��֪ͨ
 * @param[in] pHandle-��¼ʱ���صĻỰ���
 * @param[in] pfn-״̬�ص�����
 * @param[in] pUser-�û�
 */
AE_USB_EXTERN void AEUSB_SetDevStatusCallBack(USB_HANDLE pHandle, tfnOnDevStatus pfn, void* pUser);

/**
 * �豸�ǳ�
 * @return: AEUSB_OK - succ, ����-������
 * @param[in] pDev-��Ҫ�ǳ����豸
 */
AE_USB_EXTERN int AEUSB_LogOut(USB_HANDLE pHandle);

/**
 * ���ò���
 * @return: AEUSB_OK - succ, ����-������
 * @param[in] pHandle-�Ự���
 * @param[in] pSetParams-�����ṹ��,������ṹ��ע��
 */
AE_USB_EXTERN int AEUSB_SetParams(USB_HANDLE pHandle, DevSetParams* pSetParams);


/**
 * ��ȡ����
 * @return: AEUSB_OK - succ, ����-������
 * @param[in] pHandle-�Ự���
 * @param[in] pGetParams-�����ṹ��,������ṹ��ע��
 */
AE_USB_EXTERN int AEUSB_GetParams(USB_HANDLE pHandle, DevGetParams* pGetParams);

/**
 * ��ȡ������
 * @return: AEUSB_OK - succ, ����-������
 * @param[in] pHandle-�Ự���
 * @param[in] pAbilityParams-�����ṹ��,������ṹ��ע��
 */
AE_USB_EXTERN int AEUSB_GetAbility(USB_HANDLE pHandle, DevAbilityParams* pAbilityParams);

/**
 * ץͼ
 * @return: AEUSB_OK - succ, ����-������
 * @param[in] pHandle-�Ự���
 * @param[in] pDevCapPic-�����ṹ��,������ṹ��ע��
 */
AE_USB_EXTERN int AEUSB_CapPicture(USB_HANDLE pHandle, DevCapPicture* pDevCapPic);

/**
 * ��ʼԤ��
 * @return: STREAM_HANDLE ʧ�ܷ���-INVALID_STREAM_HANDLE,DevRealPlay���ж�Ӧ������, �ɹ����� >= 0
 * @param[in] pRealPlay-�����ṹ��,������ṹ��ע��
 * @param[in] pfnRecvRealPlay-��Ƶ���ݻص�����
 * @param[in] pUser-�û�
 */
AE_USB_EXTERN STREAM_HANDLE AEUSB_StartRealPlay(USB_HANDLE pHandle, DevRealPlay* pRealPlay, tfnRecvRealPlay pfnRecvRealPlay, void* pUser);

/**
 * ֹͣԤ��
 * @return: AEUSB_OK - succ, ����-������
 * @param[in] pHandle-�Ự���
 * @param[in] hRealPlayHandle-Ԥ�����
 */
AE_USB_EXTERN int AEUSB_StopRealPlay(USB_HANDLE pHandle, STREAM_HANDLE hRealPlayHandle);

#ifdef __cplusplus
USB_EXTERN_C_END
#endif

#endif //USB_INTERFACE