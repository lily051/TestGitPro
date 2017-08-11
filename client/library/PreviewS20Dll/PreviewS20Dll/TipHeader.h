#pragma once

typedef enum _Enum_Tip_Msg
{
    TIP_MSG_NORMAL     = 0x00000001,   // ��ͨ������Ϣ
    TIP_MSG_HYPERLINKS = 0x00000002,   // �����ӵ�ַ��Ϣ
};

typedef struct tagTipDlgParam 
{
	unsigned uMoveSpeed;/*��ʾ�������ٶ�, ��ֵԽ���ƶ�Խ��*/
	unsigned uBlinkTime;/*��˸����*/
	unsigned uHideTime; /*������ʾ��ʱ��*/
	BYTE byShowMode;    /*��ʾ�����ģʽ 0:�Զ� 1:�·� 2:�ҷ�*/
	BYTE byAlphaMax;    /*���͸���� 1~255*/
	BYTE byAlphaMin;    /*��С͸���� 1~255*/
    unsigned uMsgType;  /*0:��ͨ������Ϣ 1:�����ӵ�ַ*/

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


bool InitTipDlg(const int nMaxDlgNum = 2/*һ�γ���������ʾ�����Ŀ*/);
bool ShowTipMsg(MSG_DETAIL &msgDetail); /*��Ϣ�������봦�����*/
bool DestoryTipDlg();                   /*�ر���ʾ��, �����Ϣ����*/