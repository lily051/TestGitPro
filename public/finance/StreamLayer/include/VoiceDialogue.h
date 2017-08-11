/**	@file VoiceDialogue.h
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief 语音对讲接口
 *
 *	@author		何文
 *	@date		2011/10/09
 *
 *	@note 语音对讲的声音采集和播放控制接口
 *	@note 历史记录：
 *	@note V1.0.0  创建文件
 */
#ifndef		_VOICE_DIALOGUE_
#define		_VOICE_DIALOGUE_

#ifdef VOICEDIALOGUE_EXPORTS
#define VOICEDIALOGUE_API __declspec(dllexport)
#else
#define VOICEDIALOGUE_API __declspec(dllimport)
#endif


/**	@enum EN_AUDIO_TYPE
 *  @brief 语音采集编码格式。
 *	每一项只能占用不同的二进制位
 */
typedef enum
{
	AUDIO_TYPE_G711 = 0x0001,
	AUDIO_TYPE_G722 = 0x0002,
}EN_AUDIO_TYPE;


#ifdef __cplusplus
extern "C" {
#endif


/**	@fn	typedef void (__stdcall * HIK_AudioDataCallBack(int nAudioType, void* pAudioBuf, unsigned int nAudioLen, int nUserData)
 *	@brief 用户语音数据回调函数定义
 *	@param[in] nAudioType 语音类型 EN_AUDIO_TYPE的值
 *	@param[in] pAudioBuf 语音数据
 *	@param[in] nAudioLen 语音长度
 *	@param[in] nUserData 用户数据
 *	@return	无
 */
typedef void (__stdcall *HIK_AudioDataCallBack)(int nAudioType, void* pAudioBuf, unsigned int nAudioLen, int nUserData);

/**	@fn	VOICEDIALOGUE_API	bool CALLBACK HIK_AUDIO_StartPlay(int nAudioType)
 *	@brief 开始语音数据的播放
 *	@param[in] nAudioType 语音数据类型 EN_AUDIO_TYPE 的值
 *	@return	播放是否成功
 */
VOICEDIALOGUE_API	bool CALLBACK	HIK_AUDIO_StartPlay(int nAudioType);

/**	@fn	VOICEDIALOGUE_API	bool CALLBACK HIK_AUDIO_StopPlay()
 *	@brief 停止语音对讲的数据播放
 *	@return	操作是否成功
 */
VOICEDIALOGUE_API	bool CALLBACK	HIK_AUDIO_StopPlay();

/**	@fn	VOICEDIALOGUE_API	bool CALLBACK HIK_AUDIO_IputAudioData(char* pAduioBuf, unsigned int nLen)
 *	@brief 插入需要播放的语音数据
 *	@param[in] pAduioBuf 语音数据
 *	@param[in] nLen 语音数据长度
 *	@return	操作是否成功
 */
VOICEDIALOGUE_API	bool CALLBACK	HIK_AUDIO_IputAudioData(char* pAduioBuf, unsigned int nLen);

/**	@fn	VOICEDIALOGUE_API	bool CALLBACK HIK_AUDIO_SetAduioVolume(WORD wVolume)
 *	@brief 设置语音播放的声音大小
 *	@param[in] wVolume 声音值
 *	@return	操作是否成功
 */
VOICEDIALOGUE_API	bool CALLBACK	HIK_AUDIO_SetAduioVolume(WORD wVolume);


/**	@fn	VOICEDIALOGUE_API	bool CALLBACK HIK_AUDIO_StartAudioIn(int nAudioType)
 *	@brief 开始语音采集
 *	@param[in] nAudioType 语音类型 可为EN_AUDIO_TYPE值得交集
 *	@return	操作是否成功
 */
VOICEDIALOGUE_API	bool CALLBACK	HIK_AUDIO_StartAudioIn(int nAudioType);

/**	@fn	VOICEDIALOGUE_API	bool CALLBACK HIK_AUDIO_StopAudioIn()
 *	@brief 停止语音采集
 *	@return	操作是否成功
 */
VOICEDIALOGUE_API	bool CALLBACK	HIK_AUDIO_StopAudioIn();

/**	@fn	VOICEDIALOGUE_API   bool CALLBACK HIK_AUDIO_SetAudioInCallBack(HIK_AudioDataCallBack pCbf, int nUserData)
 *	@brief 设置语音采集的用户回调函数和信息
 *	@param[in] pCbf 回调函数
 *	@param[in] nUserData 用户数据
 *	@return	操作是否成功
 */
VOICEDIALOGUE_API   bool CALLBACK	HIK_AUDIO_SetAudioInCallBack(HIK_AudioDataCallBack pCbf, int nUserData);


#ifdef __cplusplus
}
#endif

#endif
