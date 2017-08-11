/**	@file VoiceDialogue.h
 *	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief �����Խ��ӿ�
 *
 *	@author		����
 *	@date		2011/10/09
 *
 *	@note �����Խ��������ɼ��Ͳ��ſ��ƽӿ�
 *	@note ��ʷ��¼��
 *	@note V1.0.0  �����ļ�
 */
#ifndef		_VOICE_DIALOGUE_
#define		_VOICE_DIALOGUE_

#ifdef VOICEDIALOGUE_EXPORTS
#define VOICEDIALOGUE_API __declspec(dllexport)
#else
#define VOICEDIALOGUE_API __declspec(dllimport)
#endif


/**	@enum EN_AUDIO_TYPE
 *  @brief �����ɼ������ʽ��
 *	ÿһ��ֻ��ռ�ò�ͬ�Ķ�����λ
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
 *	@brief �û��������ݻص���������
 *	@param[in] nAudioType �������� EN_AUDIO_TYPE��ֵ
 *	@param[in] pAudioBuf ��������
 *	@param[in] nAudioLen ��������
 *	@param[in] nUserData �û�����
 *	@return	��
 */
typedef void (__stdcall *HIK_AudioDataCallBack)(int nAudioType, void* pAudioBuf, unsigned int nAudioLen, int nUserData);

/**	@fn	VOICEDIALOGUE_API	bool CALLBACK HIK_AUDIO_StartPlay(int nAudioType)
 *	@brief ��ʼ�������ݵĲ���
 *	@param[in] nAudioType ������������ EN_AUDIO_TYPE ��ֵ
 *	@return	�����Ƿ�ɹ�
 */
VOICEDIALOGUE_API	bool CALLBACK	HIK_AUDIO_StartPlay(int nAudioType);

/**	@fn	VOICEDIALOGUE_API	bool CALLBACK HIK_AUDIO_StopPlay()
 *	@brief ֹͣ�����Խ������ݲ���
 *	@return	�����Ƿ�ɹ�
 */
VOICEDIALOGUE_API	bool CALLBACK	HIK_AUDIO_StopPlay();

/**	@fn	VOICEDIALOGUE_API	bool CALLBACK HIK_AUDIO_IputAudioData(char* pAduioBuf, unsigned int nLen)
 *	@brief ������Ҫ���ŵ���������
 *	@param[in] pAduioBuf ��������
 *	@param[in] nLen �������ݳ���
 *	@return	�����Ƿ�ɹ�
 */
VOICEDIALOGUE_API	bool CALLBACK	HIK_AUDIO_IputAudioData(char* pAduioBuf, unsigned int nLen);

/**	@fn	VOICEDIALOGUE_API	bool CALLBACK HIK_AUDIO_SetAduioVolume(WORD wVolume)
 *	@brief �����������ŵ�������С
 *	@param[in] wVolume ����ֵ
 *	@return	�����Ƿ�ɹ�
 */
VOICEDIALOGUE_API	bool CALLBACK	HIK_AUDIO_SetAduioVolume(WORD wVolume);


/**	@fn	VOICEDIALOGUE_API	bool CALLBACK HIK_AUDIO_StartAudioIn(int nAudioType)
 *	@brief ��ʼ�����ɼ�
 *	@param[in] nAudioType �������� ��ΪEN_AUDIO_TYPEֵ�ý���
 *	@return	�����Ƿ�ɹ�
 */
VOICEDIALOGUE_API	bool CALLBACK	HIK_AUDIO_StartAudioIn(int nAudioType);

/**	@fn	VOICEDIALOGUE_API	bool CALLBACK HIK_AUDIO_StopAudioIn()
 *	@brief ֹͣ�����ɼ�
 *	@return	�����Ƿ�ɹ�
 */
VOICEDIALOGUE_API	bool CALLBACK	HIK_AUDIO_StopAudioIn();

/**	@fn	VOICEDIALOGUE_API   bool CALLBACK HIK_AUDIO_SetAudioInCallBack(HIK_AudioDataCallBack pCbf, int nUserData)
 *	@brief ���������ɼ����û��ص���������Ϣ
 *	@param[in] pCbf �ص�����
 *	@param[in] nUserData �û�����
 *	@return	�����Ƿ�ɹ�
 */
VOICEDIALOGUE_API   bool CALLBACK	HIK_AUDIO_SetAudioInCallBack(HIK_AudioDataCallBack pCbf, int nUserData);


#ifdef __cplusplus
}
#endif

#endif
