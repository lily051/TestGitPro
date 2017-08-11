/*****************************************************************************  
*    @file     MsgBox.h
*    @note     HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*    @brief    信息提示对话框
*    @author   刘大煜
*    @date     2011-11-24
*    @note     历史记录：
*    @note     v1.0.0    创建
*    @warning  NULL
******************************************************************************/
#ifdef MSGBOX_EXPORTS
#define MSGBOX_API __declspec(dllexport)
#else
#define MSGBOX_API __declspec(dllimport)
#endif

//标识带有倒计时的MSGBOX
#define MSGBOX_TIMER 1
//消息对话框的分类
enum MSGBOX_CUSTOM_DEFINE
{
	MSGBOX_TYPE_WARNING			= 1,    //警告 提示
	MSGBOX_TYPE_MAKESURE		= 2,    //确认 提示
	MSGBOX_TYPE_ERROR			= 4,    //错误 提示
	MSGBOX_TYPE_YESNO			= 8,    //是否 提示
	MSGBOX_TYPE_INFORMATION		= 16,   //信息 提示
	MSGBOX_TYPE_TIMER			= 32	//倒计时 提示
};

enum SKIN_COLOR
{
    MSGBOX_SKIN_RED = 0,
    MSGBOX_SKIN_BLUE = 1
};

/*************************************************
Function:       MsgBoxApi_Show
Description:	根据类型和字符串显示信息提示窗口
Input:          czContent:信息内容
                nType:提示框类型
				bTimer: 0表示默认的提示框; 1:表示倒计时提示框
Output:
return:			信息提示框DoModel返回值
*************************************************/
MSGBOX_API INT_PTR MsgBoxApi_Show(wchar_t* czContent, MSGBOX_CUSTOM_DEFINE nType, BOOL bTimer = 0);

/*************************************************
Function:       MsgBoxApi_SetSkinColor
Description:	设置提示框皮肤颜色，该设置影响所有使用到该dll的模块
Input:          int nSkinColor 0-红黑，1-蓝白,见enum SKIN_COLOR定义
return:			int 原来的皮肤颜色设置
*************************************************/
MSGBOX_API int MsgBoxApi_SetSkinColor(int nSkinColor);