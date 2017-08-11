/*****************************************************************************  
*    @file     MsgBox.h
*    @note     HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*    @brief    ��Ϣ��ʾ�Ի���
*    @author   ������
*    @date     2011-11-24
*    @note     ��ʷ��¼��
*    @note     v1.0.0    ����
*    @warning  NULL
******************************************************************************/
#ifdef MSGBOX_EXPORTS
#define MSGBOX_API __declspec(dllexport)
#else
#define MSGBOX_API __declspec(dllimport)
#endif

//��ʶ���е���ʱ��MSGBOX
#define MSGBOX_TIMER 1
//��Ϣ�Ի���ķ���
enum MSGBOX_CUSTOM_DEFINE
{
	MSGBOX_TYPE_WARNING			= 1,    //���� ��ʾ
	MSGBOX_TYPE_MAKESURE		= 2,    //ȷ�� ��ʾ
	MSGBOX_TYPE_ERROR			= 4,    //���� ��ʾ
	MSGBOX_TYPE_YESNO			= 8,    //�Ƿ� ��ʾ
	MSGBOX_TYPE_INFORMATION		= 16,   //��Ϣ ��ʾ
	MSGBOX_TYPE_TIMER			= 32	//����ʱ ��ʾ
};

enum SKIN_COLOR
{
    MSGBOX_SKIN_RED = 0,
    MSGBOX_SKIN_BLUE = 1
};

/*************************************************
Function:       MsgBoxApi_Show
Description:	�������ͺ��ַ�����ʾ��Ϣ��ʾ����
Input:          czContent:��Ϣ����
                nType:��ʾ������
				bTimer: 0��ʾĬ�ϵ���ʾ��; 1:��ʾ����ʱ��ʾ��
Output:
return:			��Ϣ��ʾ��DoModel����ֵ
*************************************************/
MSGBOX_API INT_PTR MsgBoxApi_Show(wchar_t* czContent, MSGBOX_CUSTOM_DEFINE nType, BOOL bTimer = 0);

/*************************************************
Function:       MsgBoxApi_SetSkinColor
Description:	������ʾ��Ƥ����ɫ��������Ӱ������ʹ�õ���dll��ģ��
Input:          int nSkinColor 0-��ڣ�1-����,��enum SKIN_COLOR����
return:			int ԭ����Ƥ����ɫ����
*************************************************/
MSGBOX_API int MsgBoxApi_SetSkinColor(int nSkinColor);