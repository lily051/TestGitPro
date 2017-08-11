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

#define AEUSB_ERROR_UNKNOWN		-1//未知错误
#define AEUSB_OK				0//成功
#define AEUSB_LOGINFAIL			1//登录失败,用户名或密码错误
#define AEUSB_ERROR_CMD			2//命令不识别
#define AEUSB_ERROR_JSON		3//json串错误
#define AEUSB_ERROR_CHAN		4//通道号错误
#define AEUSB_CAP_FAIL			5//抓图失败
#define AEUSB_GET_ABILITY_FAIL	6//获取能力集失败
#define AEUSB_SET_PARAM_FAIL	7//设置参数失败
#define AEUSB_GET_PARAM_FAIL	8//获取参数失败
#define AEUSB_UNKNOW_CMD		9//未知指令
#define AEUSB_TIMEOUT			258//超时

//接口导出

typedef void*			USB_HANDLE;
typedef void*			USB_DEVICE;
typedef int				STREAM_HANDLE;

struct usb_simple_device
{
	char		szFileName[FILE_PATH_MAX];//路径
	char		szDescriptor[FILE_PATH_MAX];//描述符
	char		szExtRes[EXT_INFO_MAX];//附加信息.暂定为json串格式
	USB_DEVICE	pDev;//设备

	usb_simple_device* pRev;//前一个
	usb_simple_device* pNext;//后一个
};

typedef struct
{
	char*		pCondBuf;///参数数据
	int			nCondBufLen;//参数数据长度
	char		szExtRes[EXT_INFO_MAX];//扩展信息
} DevSetParams;

typedef struct
{
	char*		pCondBuf;//参数数据
	int			nCondBufLen;//参数数据长度
	char* 		pOutBuf;//返回的数据结果
	int			nOutBufLen;//返回的数据结果的指定长度
	int*		nOutRetLen;//返回的数据结果的实际长度
	char		szExtRes[EXT_INFO_MAX];//扩展信息
} DevGetParams;

typedef struct
{
	char*		pCondBuf;//参数数据
	int			nCondBufLen;//参数数据长度
	char* 		pOutBuf;//返回的数据结果
	int			nOutBufLen;//返回的数据结果的指定长度
	int*		nOutRetLen;//返回的数据结果的实际长度
	char		szExtRes[EXT_INFO_MAX];//扩展信息
} DevAbilityParams;

typedef struct
{
	int			nChannel;//通道号
	char*		pPicBuff;//返回的图片数据
	int			nInBufLen;//返回的图片数据 指定长度,一般不会超过2M
	int*		nOutBufLen;//返回的图片数据 实际长度
	char		szExtRes[EXT_INFO_MAX];//扩展信息
} DevCapPicture;

typedef struct
{
	int			nChannel;//通道号
	int			nType;//主流or子流 0-main, 1-sub
	int			nErrorCode;//失败对应错误码
	char		szExtRes[EXT_INFO_MAX];//扩展信息
} DevRealPlay;

/**
 * 设备状态回调函数
 * @param[out] pHandle-会话句柄
 * @param[out] nStatus-登录状态
 * @param[out] pUser-AEUSB_SetDevStatusCallBack时设置的user
 */
typedef void (*tfnOnDevStatus)(USB_HANDLE pHandle, int nStatus, void* pUser);
/**
 * 实时预览回调函数
 * @param[out] hRealPlayHandle-实时预览的句柄,由AEUSB_StartRealPlayStream返回
 * @param[out] nDataType- 0-head, 1-video data
 * @param[out] pBuffer-视频数据
 * @param[out] nRetLen-视频数据大小
 * @param[out] pUser-AEUSB_StartRealPlayStream时设置的user
 */
typedef void (*tfnRecvRealPlay)(STREAM_HANDLE hRealPlayHandle, int nDataType, char* pBuffer, int nRetLen, void* pUser);

/**
 * 资源初始化
 * @return: AEUSB_OK - succ, 其他-错误码
 */
AE_USB_EXTERN int AEUSB_Initialize();

/**
 * 资源反初始化
 * @return: AEUSB_OK - succ, 其他-错误码
 */
AE_USB_EXTERN int AEUSB_Finalize();

/**
 * 获取所有USB设备
 * @return: 检测到的设备个数
 * @param[in] pDevList-设备列表, 重新获取列表后, 之前的指针不在有效
 */
AE_USB_EXTERN int AEUSB_GetUsbDevList(usb_simple_device*& pDevList);

/**
 * 设备登录
 * @return: 会话句柄,用于以后操作设备, 失败返回NULL
 * @param[in] pDev-设备,获取设备列表时可以拿到
 * @param[in] pUser-用户名
 * @param[in] nUserLen-用户名长度
 * @param[in] pPwd-密码
 * @param[in] nPwdLen-密码长度
 */
AE_USB_EXTERN USB_HANDLE AEUSB_LogIn(USB_DEVICE pDev, char* pUser, int nUserLen, char* pPwd, int nPwdLen);

/**
 * 登录状态的回调.用于自动掉线等异常情况下,状态的通知
 * @param[in] pHandle-登录时返回的会话句柄
 * @param[in] pfn-状态回调函数
 * @param[in] pUser-用户
 */
AE_USB_EXTERN void AEUSB_SetDevStatusCallBack(USB_HANDLE pHandle, tfnOnDevStatus pfn, void* pUser);

/**
 * 设备登出
 * @return: AEUSB_OK - succ, 其他-错误码
 * @param[in] pDev-需要登出的设备
 */
AE_USB_EXTERN int AEUSB_LogOut(USB_HANDLE pHandle);

/**
 * 设置参数
 * @return: AEUSB_OK - succ, 其他-错误码
 * @param[in] pHandle-会话句柄
 * @param[in] pSetParams-参数结构体,具体见结构体注释
 */
AE_USB_EXTERN int AEUSB_SetParams(USB_HANDLE pHandle, DevSetParams* pSetParams);


/**
 * 获取参数
 * @return: AEUSB_OK - succ, 其他-错误码
 * @param[in] pHandle-会话句柄
 * @param[in] pGetParams-参数结构体,具体见结构体注释
 */
AE_USB_EXTERN int AEUSB_GetParams(USB_HANDLE pHandle, DevGetParams* pGetParams);

/**
 * 获取能力集
 * @return: AEUSB_OK - succ, 其他-错误码
 * @param[in] pHandle-会话句柄
 * @param[in] pAbilityParams-参数结构体,具体见结构体注释
 */
AE_USB_EXTERN int AEUSB_GetAbility(USB_HANDLE pHandle, DevAbilityParams* pAbilityParams);

/**
 * 抓图
 * @return: AEUSB_OK - succ, 其他-错误码
 * @param[in] pHandle-会话句柄
 * @param[in] pDevCapPic-参数结构体,具体见结构体注释
 */
AE_USB_EXTERN int AEUSB_CapPicture(USB_HANDLE pHandle, DevCapPicture* pDevCapPic);

/**
 * 开始预览
 * @return: STREAM_HANDLE 失败返回-INVALID_STREAM_HANDLE,DevRealPlay里有对应错误码, 成功返回 >= 0
 * @param[in] pRealPlay-参数结构体,具体见结构体注释
 * @param[in] pfnRecvRealPlay-视频数据回调函数
 * @param[in] pUser-用户
 */
AE_USB_EXTERN STREAM_HANDLE AEUSB_StartRealPlay(USB_HANDLE pHandle, DevRealPlay* pRealPlay, tfnRecvRealPlay pfnRecvRealPlay, void* pUser);

/**
 * 停止预览
 * @return: AEUSB_OK - succ, 其他-错误码
 * @param[in] pHandle-会话句柄
 * @param[in] hRealPlayHandle-预览句柄
 */
AE_USB_EXTERN int AEUSB_StopRealPlay(USB_HANDLE pHandle, STREAM_HANDLE hRealPlayHandle);

#ifdef __cplusplus
USB_EXTERN_C_END
#endif

#endif //USB_INTERFACE