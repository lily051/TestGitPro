#pragma once

typedef enum _Enum_Tip_Msg
{
    TIP_MSG_NORMAL     = 0x00000001,   // 普通文字消息
    TIP_MSG_HYPERLINKS = 0x00000002,   // 超链接地址消息
};

typedef struct tagTipDlgParam 
{
	unsigned uMoveSpeed;/*提示框上升速度, 数值越大移动越慢*/
	unsigned uBlinkTime;/*闪烁次数*/
	unsigned uHideTime; /*渐隐提示框时间*/
	BYTE byShowMode;    /*提示框出现模式 0:自动 1:下方 2:右方*/
	BYTE byAlphaMax;    /*最大透明度 1~255*/
	BYTE byAlphaMin;    /*最小透明度 1~255*/
    unsigned uMsgType;  /*0:普通文字信息 1:超链接地址*/

	tagTipDlgParam()
	{
		uMoveSpeed = 500;
		uBlinkTime = 3;
		uHideTime = 3000;
		byShowMode = 0;
		byAlphaMax = 255;
		byAlphaMin = 100;
        uMsgType = 0;
	}
}TIP_DLG_PARAM;

typedef struct tagMsgDetail
{
    TIP_DLG_PARAM tdp;
    CString strCaption;
    CString strMsg;
}MSG_DETAIL;


bool InitTipDlg(const int nMaxDlgNum = 2/*一次出现最多的提示框的数目*/);
bool ShowTipMsg(MSG_DETAIL &msgDetail); /*消息将被加入处理队列*/
bool DestoryTipDlg();                   /*关闭提示框, 清空消息队列*/