#pragma once
/**	@file    PlaybackDef.h
*	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief   Playback���ݶ�����
*
*	@author	 yudan
*	@date	 2012/06/20
*	@note   
*	@note    ��ʷ��¼��
*	@note    V1.0  create at 2012/06/20 by yudan
*/
//lint -library
/***************DEFINE*************************************************/
//color
#define UI_COLOR_BKGND           RESOURCELOAD_COLOR(IDS_UI_COLOR_BKGND)   //�ط����屳��ɫ
#define UI_WND_BKGND             RESOURCELOAD_COLOR(IDS_UI_WND_BKGND)   //TabҳWnd����ɫ
#define UI_WNDSPACE_BKGND        RESOURCELOAD_COLOR(IDS_UI_WND_BKGND)   //���ڿؼ�space����ɫ
#define UI_LISTHEADER_COLOR      RESOURCELOAD_COLOR(IDS_UI_WND_BKGND)   //list Ctrlͷ����ɫ
#define UI_LISTITEMF_COLOR       RESOURCELOAD_COLOR(IDS_UI_LISTITEMF_COLOR)   //list������ɫ
#define UI_LISTITEMS_COLOR       RESOURCELOAD_COLOR(IDS_UI_LISTITEMS_COLOR)   //list˫����ɫ
#define UI_LISTSELECT_COLOR      RESOURCELOAD_COLOR(IDS_UI_LISTSELECT_COLOR)   //listѡ����ɫ
#define UI_TEXT_COLOR            RESOURCELOAD_COLOR(IDS_UI_TEXT_COLOR)         //�ı�ɫ
#define UI_TREE_BKCOLOR          RESOURCELOAD_COLOR(IDS_UI_TREE_BKCOLOR)   //���ı���ɫ,listͨ��  
#define UI_TREE_TEXT_COLOR_153	 RGB(153,153,153)     //�����ı���ɫ
#define UI_PLAYWND_COLOR         RESOURCELOAD_COLOR(IDS_UI_PLAYWND_COLOR)      //��Ƶ���ڱ���ɫ
#define UI_PLAYWND_SPACE         RESOURCELOAD_COLOR(IDS_UI_PLAYWND_SPACE)      //��Ƶ���ڷָ�ɫ
#define UI_PLAYWNDSEL_FRAME      RESOURCELOAD_COLOR(IDS_UI_PLAYWNDSEL_FRAME)       //��Ƶ����ѡ�б߿�
#define UI_PLAYWND_TITLE_COLOR   RESOURCELOAD_COLOR(IDS_UI_PLAYWND_TITLE_COLOR)	   // ��Ƶ���ڱ�����ɫ.

#define UI_PLAYBAR_TITLEBK       RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_TITLEBK)
#define UI_PLAYBAR_RECORDBK      RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_RECORDBK)
#define UI_PLAYBAR_TITLESPACE    RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_TITLESPACE)
#define UI_PLAYBAR_CURTIME       RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_CURTIME)
#define UI_PLAYBAR_SELECT        RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_SELECT)
#define UI_PLAYBAR_BK            RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_TITLEBK)
#define UI_PLAYBAR_MINPEN        RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_MINPEN)
#define UI_PLAYBAR_LEFT          RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_TITLEBK)
#define UI_PLAYBAR_ELEMENT       RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_ELEMENT)
#define UI_PLAYBAR_TOP           RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_TITLEBK)
#define UI_PLAYBAR_LIGHT         RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_LIGHT)   //��������ɫ
#define UI_PLAYBAR_RECORDTIMEING RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_RECORDTIMEING)
#define UI_PLAYBAR_RECORDMANUAL  RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_RECORDMANUAL)
#define UI_PLAYBAR_RECORDALARM   RESOURCELOAD_COLOR(IDS_UI_PLAYBAR_RECORDALARM)

//size
#define UI_TREE_ITEM_HEIGHT	18	                 //����ÿ��ĸ߶�

// max file count.
#define MAX_FILE_COUNT 4000

//define
#define MAX_PLAYWND_NUM 16                       //���طŴ�������
#define MAX_SUBSERACHWND_NUM 16                  //���ֶ�������������
#define GROUPNAME_MAXLEN 32                      //��������󳤶�
#define LOCAL_MAX_FILENUM 1000                   //���ػط�list�����Ƶ����
#define FLOATWND_CLOSE_TIME 4                    //����������ʾʱ��
#define TREEDLG_WIDTH 259                        //�����ڿ��
#define DOWNLOADTITLE_WIDTH 200                  //���ش��ڿ��

#define MSG_PLAYBACK_CONTROL WM_USER + 1001      //�طſ��ư�ť��Ӧ��Ϣ
#define MSG_PLAYOSD_GET WM_USER + 1003           //��ȡ�ط�OSDʱ��
#define MSG_DB_VEDIO (WM_USER + 1004)              //˫����Ƶ�Ŵ���Ϣ
#define MSG_WND_FULLSCREEN (WM_USER + 1005)        //����ȫ����Ϣ
#define MSG_DATA_RESET WM_USER + 1006            //���ݸı���Ϣ
#define MSG_SELECT_VEDIO (WM_USER + 1007)          //����ѡ����Ϣ
#define MSG_PLAYSTATE_UPDATE (WM_USER + 1008)      //�ط�״̬�ı���Ϣ
#define MSG_MENU_CALLBACK (WM_USER + 1009)		 //�˵��¼�

#define PB_SPEED_ONESIXTEEN  0                   // 1/16
#define PB_SPEED_ONEEIGHTH   1                   // 1/8
#define PB_SPEED_QUARTER   2                     // 1/4
#define PB_SPEED_HALF   3                        // 1/2d
#define PB_SPEED_NORMAL   4                      // 1
#define PB_SPEED_TWO   5                         // 2
#define PB_SPEED_FOUR   6                        // 4
#define PB_SPEED_EIGHT   7                       // 8
#define PB_SPEED_SIXTEEN   8                     // 16
#define PB_PAGE_SIZE   30                        //ҳ��С
#define PB_MAX_SPEED_RANGE PB_PAGE_SIZE * PB_SPEED_SIXTEEN    //��󲥷��ٶ�
#define PB_MIN_SPEED_RANGE PB_SPEED_ONESIXTEEN                //��С�����ٶ�

//msg
#define MSG_TOOLBAR_BTN_CLICK  (WM_USER + 2001)    //��������ť�����Ӧ��Ϣ
#define MSG_TREENODE_DBCLICK   (WM_USER + 2002)    //���ڵ�˫����Ϣ
#define MSG_TREENODE_DRAG      (WM_USER + 2003)    //���ڵ���ק��Ϣ
#define MSG_RECORDFILE_PLAY    WM_USER + 2004    //�ļ��ط���Ϣ
#define MSG_RECORDFILE_DRAG    WM_USER + 2005    //�ļ���ק��Ϣ
#define MSG_RECORDFILE_DOWNLOAD (WM_USER+ 2006)    //�ļ�������Ϣ
#define MSG_RECORD_DOWNLOAD_NOTIFY (WM_USER + 2007)//�ļ�����֪ͨ
#define MSG_TOOLBAR_UPDATE     (WM_USER + 2017)    //������ˢ����Ϣ
#define MSG_FULLSCREEN_CLICK   WM_USER + 2018    //ȫ����ť�����Ϣ
#define MSG_LOCALVEDIO_PLAY    (WM_USER + 2029)    //�����ļ�����֪ͨ��Ϣ
#define MSG_LOCALVEDIO_OTHER   WM_USER + 2030    //���ز����ļ��л���Ϣ
#define MSG_FLOATWND_CLOST     (WM_USER + 2041)    //�������ڹر�֪ͨ��Ϣ
#define MSG_RECORDDLG_SHOW     WM_USER + 2042    //�ļ�����չʾ������Ϣ
#define MSG_DOWNLIST_SELECT    (WM_USER + 2053)    //����list�л���Ϣ
#define MSG_DOWNLOADTASK_DEL   (WM_USER + 2054)    //��������ɾ����Ϣ
#define MSG_DOWNLOADTASK_FAIL  (WM_USER + 2055)    //��������ʧ����Ϣ
#define MSG_DOWNLOADTASK_SUC   (WM_USER + 2056)    //��������ɹ���Ϣ
#define MSG_DOWNLOADTASK_REDOWN (WM_USER + 2095)   //������������������Ϣ
#define MSG_DOWNLOADFILE_PLAY  (WM_USER + 2096)	 // �������سɹ����ļ�.
#define MSG_PLAYBACK_NEXT_FILE (WM_USER + 2097)	 // �����ļ�����,������һ���ļ�.
#define MSG_BWCONTROL_STOPPLAY (WM_USER + 2098)	 // ���������Ϣ,ֹͣ�ط�.
#define MSG_GROUPCFG_CHANGE (WM_USER + 2099)		 // �طŷ����Զ�ˢ����Ϣ.
#define MSG_BWCONTROL_RECONNECT WM_USER + 2100	 // ����������Ϣ.
#define MSG_PLAYBACK_PLAYING (WM_USER + 2101)		 // �طſ�����Ϣ-���ڻط���.
#define MSG_PLAYBACK_PLAY_SUCCESS (WM_USER + 2102) // �طſ�����Ϣ-�طųɹ�.
#define MSG_PLAYBACK_PLAY_FAIL (WM_USER + 2103)    // �طſ�����Ϣ-�ط�ʧ��.
#define MSG_PLAYBACK_BW_LIMIT (WM_USER + 2104)	 // �طſ�����Ϣ-���������������.
#define MSG_PLAYBACK_STOPPLAY (WM_USER + 2105)	 // �طſ�����Ϣ-ֹͣ�ط�.
#define MSG_PLAYBACK_FIND_FAIL (WM_USER + 2106)	 // �طſ�����Ϣ-�����ļ�ʧ��.
#define MSG_PLAYBACK_FIND_SUCCESS (WM_USER + 2107) // �طſ�����Ϣ-�����ļ��ɹ�.
#define MSG_FILELIST_CHANGE (WM_USER + 2108)		 // �ļ��б��л���ʾ��Ϣ.
#define MSG_PLAYBACK_LOCATEPLAY_SUCCESS (WM_USER + 2109)  // �طſ�����Ϣ-���ûطŽ��ȳɹ�.
#define MSG_PLAYBACK_LOCATEPLAY_FAIL (WM_USER + 2110)     // �طſ�����Ϣ-���ûطŽ���ʧ��.
#define MSG_PLAYBACK_BWRECONNECT_STOPPLAY (WM_USER + 2111) // �طſ�����Ϣ-������Ʒ���������ֹͣ�ط�.
#define MSG_MOUSEEVENT_MINITOOLBAR_LEAVE (WM_USER + 2112) // ����뿪��������Ϣ.
#define MSG_MOUSEEVENT_MINITOOLBAR_MOVE (WM_USER + 2113)  // �����빤������Ϣ.
#define MSG_MINITOOLBAR_EXPAND_CLOSE (WM_USER + 2114)		// ��չ�������ر���Ϣ.
#define MSG_MINITOOLBAR_OPENSOUND WM_USER + 2115		// �ر��������ڵ���Ƶ.
#define MSG_MINITOOLBAR_DOWNLOAD (WM_USER + 2116)			// ��ʱ������.
#define MSG_UPDATE_SYNBTN_STATE WM_USER + 2117			// ����ͬ���طŰ�ť.
#define MSG_CHECK_SPACE_NOTENOUGH (WM_USER + 2118)		// ���̿ռ䲻��.
#define MSG_PLAYBACK_FIND_NO_FILE (WM_USER + 2119)		// �طſ�����Ϣ-�����ļ��ɹ�,���ļ�����Ϊ0. 
#define MSG_PLAYBACK_SPEED_CHANGE (WM_USER + 2120)		// �ط��ٶȸı���Ϣ.
#define MSG_PLAYBACK_SINGLEFRAME (WM_USER + 2121)			// ��֡�ط���Ϣ.
#define MSG_UPDATE_REVERSEBTN_STATE WM_USER + 2122		// ���µ��Ű�ť.
#define MSG_INVALIDATE_WINDOW (WM_USER + 2123)					// ˢ�´�����Ϣ.
#define MSG_TOOLBAR_OPERATOR    (WM_USER + 2124)    
#define MSG_PLAYBACK_STATE  (WM_USER + 2025)            //����״̬

//timer
#define	IDT_LOCAL_PLAYBACK_POS_TIMER    WM_USER + 3004	 //���ػطŽ��ȼ�ʱ��ID
#define	IDT_LOCAL_PLAYBACK_TIME_TIMER   WM_USER + 3005	 //���ػط�ʱ���ʱ��ID
#define IDT_SYNPLAY_TIMER               WM_USER + 3006   //ͬ���طŶ�ʱ��
#define IDT_VOLUMNWNDCLOSE_TIMER        WM_USER + 3007   //���������Զ��رն�ʱ��
#define IDT_CAPTUREDLGCLOSE_TIME        WM_USER + 3008   //ץͼչʾ�����Զ��رն�ʱ��


//string
//¼������
//[����]//¼���ļ�����0xff��ȫ����0����ʱ¼��,1-�ƶ���� ��2������������
//3-����|�ƶ���� 4-����&�ƶ���� 5-����� 6-�ֶ�¼��
#define		STR_PBL_RECORD_TYPE				     _T("¼������:")
#define		STR_PBL_RECORD_TYPE_ALL				 _T("ȫ��")
#define		STR_PBL_RECORD_TYPE_TIMING			 _T("��ʱ¼��")
#define		STR_PBL_RECORD_TYPE_MOVE			 _T("�ƶ����")
#define		STR_PBL_RECORD_TYPE_ALARM_TRIGER	 _T("��������")
#define		STR_PBL_RECORD_TYPE_ALARM_OR_MOVE	 _T("����|�ƶ����")
#define		STR_PBL_RECORD_TYPE_ALARM_AND_MOVE	 _T("����&�ƶ����")
#define		STR_PBL_RECORD_TYPE_CMD_TRIGER		 _T("�����")
#define		STR_PBL_RECORD_TYPE_MANUAL_RECORD	 _T("�ֶ�¼��")
#define		STR_PBL_RECORD_TYPE_BACKUP			 _T("��������¼��")
#define		STR_PBL_RECORD_TYPE_EVENT			 _T("��������¼��")


/***************ENUM***************************************************/

// Ƥ������.
typedef enum _EnumSkinType
{
	SKIN_RED = 0,		// ��ɫ.
	SKIN_BLUE = 1,		// ��ɫ.
}EnumSkinType;

// ����ָ�ģʽ.
typedef enum _PLAY_LAYOUT_WND
{
	LAYOUT_WND_1 = 1,                     //һ�������
	LAYOUT_WND_4 = 4,                     //�Ļ������
	LAYOUT_WND_9 = 9,                     //�Ż������
	LAYOUT_WND_16= 16,                    //ʮ���������
}PLAY_LAYOUT_WND;

//������ϢĿ��ID
enum COMMON_SERVER_ID
{
	DB_CMS = 1,            //��������
	CLIENT_SELF,           //�ͻ�������
};

//������Ϣ����
typedef enum
{
	ALM_REQ_MSG_WAITING_RSP = 0, //�ȴ�ͬ������
	ALM_REQ_MSG_ASYS_RSP,        //�첽����
	ALM_REQ_MSG_NO_RSP           //����Ҫ����
}MSG_REQ_TYPE;

enum MAIN_TAB_ITEM				 //TABҳID
{
	TAB_ITEM_PLAYBACK = 0,       //�طŷ�ҳ
	TAB_ITEM_DOWNLOAD,           //����ҳ
	//TAB_ITEM_LABLE,            //¼���ǩ
	TAB_ITEM_SUBSEARCH,          //�ֶμ���
	TAB_ITEM_LOCALPLAYBACK,      //���ػط�
	TAB_ITEM_CONFIG,             //����
};

enum DOWNLOAD_TAB_ITEM
{
	TAB_ITEM_DOWNLOAD_BY_FILE = 0,		// ���ļ�����.
	TAB_ITEM_DOWNLOAD_BY_TIME,			// ��ʱ������.
};

enum TREENODE_TYPE				 //���ڵ�����
{
	NODE_CTRLCELL		= 1,		  //���Ƶ�Ԫ
	NODE_REGION			= 2,		  //����
	NODE_CAMERA			= 3,          //��ص�
	NODE_GROUPROOT		= 4,		  //������Ƶ�Ԫ
	NODE_GROUP			= 5,          //����
};

enum TREENODE_IMAGE
{
	TREEIMAGE_CTRLCELL  = 0,          //���Ƶ�Ԫ
	TREEIMAGE_REGION,                 //����
	TREEIMAGE_CAMERA,                 //��ص�
	TREEIMAGE_CAMERASEL,              //��ǰѡ�еļ�ص�
};

enum PLAYCONTROL_ID
{
	PLAYBACK_WND_ID = WM_USER + 1001,            //�طŴ���ID
	PLAYBACK_TITLE_ID = WM_USER + 1101,          //�طű��ⴰ��ID
	PLAYBACK_VEDIO_ID,                           //�ط���Ƶ����ID
	PLAYBACK_MINITOOL_ID,						 // �طŹ��ߴ���ID.
	PLAYBACK_ID,                                 //�ط�ID
	PLAYBACK_CONTROL_ID,                         //���ƴ���ID
	PLAYBACK_TIMEBAR,                            //����ʱ��ؼ�

	BTN_PLAY_ID = WM_USER + 1110,                //����
	BTN_PAUSE_ID,                                //��ͣ
	BTN_STOP_ID,                                 //ֹͣ
	BTN_SLOW_ID,                                 //����
	BTN_NOMORL_ID,                               //��������
	BTN_FAST_ID,                                 //���
	BTN_SAVEFILE_ID,                             //����
	BTN_VOICE_ID,                                //��Ƶ
	BTN_VOLUMESHOW_ID,                           //�����ؼ���ʾ����
	BTN_CAPTURE_ID,                              //��ͼ
	BTN_DOWNLOAD_ID,                             //����
	BTN_CONFIG_ID,                               //����

	STT_VOLUMN_ID,                               //����

};

enum DOWNLOAD_ID
{
	DOWNLOAD_WND_ID = WM_USER + 1201,            //���ش���ID
	TASK_LIST_ID,                                //�����б�ID
	DATA_START_ID,                               //��ʼʱ��ID
	DATA_STOP_ID,                                //����ʱ��ID
	BTN_ADD_TASK,                                //�������ID
	BTN_CLEAR_ID,                                //�������ID
	BTN_DEL_ID,                                  //ɾ������ID
	BTN_STARTDOWNLOAD_ID,                        //��ʼ����ID
	BTN_STOPDOWNLOAD_ID,                         //ֹͣ����ID
	BTN_EXIT_ID,                                 //�˳�

};

enum TOOLBAR_CTRL_ID
{
	TOOL_GROUPCFG_ID = WM_USER + 1300,           //��������
	TOOL_PLAY_ID = WM_USER + 1301,               //����
	TOOL_PAUSE_ID,                               //��ͣ
	TOOL_STOP_ID,                                //ֹͣ
	TOOL_STOPALL_ID,                             //ȫ��ֹͣ
	TOOL_CAPTURE_ID,                             //ץͼ
	TOOL_SAVEDATA_ID,                            //��������
	TOOL_FRAME_ID,                               //��֡
	TOOL_SOUND_ID,                               //��Ƶ
	TOOL_DOWNLOAD_ID,                            //ʱ������
	TOOL_VOLUMN_ID,                              //����չ����ť
	TOOL_FULLSCREEN_ID,                          //ȫ��

	TOOL_SLIDER_SPEED,                           //�ٶȽ�����
	TOOL_SLIDER_POS,                             //���ػطŽ�����
	TOOL_SLIDER_VOLUMN,                          //��Ƶ������
	TOOL_CHECKBOX_VOICE,                         //��������

	TOOL_PRE_ID,                                 //ǰһ��
	TOOL_NEXT_ID,                                //��һ��

	TOOL_TIMESTA_ID,                             //���ػط�ʱ����ʾ

	TOOL_EXPAND_ID,								 // ��չ��ť.
	
	TOOL_MINI_SLOW_ID,							 // ����.
	TOOL_MINI_FAST_ID,							 // ����.
};

enum WND_ID
{
	WND_PLAYVIEW_ID = WM_USER + 1401,            //��Ƶ����
	WND_TOOLBAR_ID,                              //�طŹ�����
	WND_PLAYBARTIMECTRL_ID,                      //¼����
};

enum CONTROL_MSG
{
	MSG_PLAY = 1,                                //����
	MSG_PAUSE,                                   //��ͣ
	MSG_STOP,                                    //ֹͣ
	MSG_SPEED,                                   //�ٶ�
	MSG_SAVE,                                    //����
	MSG_VOICE,                                   //����
	MSG_VOLUME,                                  //����
	MSG_CAPTURE,                                 //��ͼ
	MSG_DOWNLOAD,                                //����
	MSG_CONFIG,                                  //����
};

enum CTRL_CUSTOMSIZE
{
	CTRL_BIGBTN_HEIGHT			= 50,										//�Զ��尴ť��С
	CTRL_EDIT_NORMAL_WIDTH		= 25,										//�༭��
	CTRL_TREE_NORMAL_WIDTH		= 245,										//���ؼ�
	CTRL_TREE_NODE_HEIGHT		= 20,
	CTRL_TREE_SMALL_WIDTH		= 200,		
	CTRL_SPACE_LITTLE			= 2,										//�ؼ����
	CTRL_SPACE_NORMAL			= 5,
	CTRL_BORD_LITTLE			= 3,										//ϸ�߿�
	CTRL_PREVIEW_TOOLBAR_HEIGHT	= 39										//Ԥ���������߶�
};

enum IE_SAVE_FILETYPE
{
	SAVE_FILETYPE_PIC = 0,						// ͼƬ.
	SAVE_FILETYPE_REC = 1,						// ¼��.
};

typedef enum _BWControlType
{
	BWCONTROL_NOCONTROL = 0,				// û�п���.
	BWCONTROL_CROWDING,						// �߼���ռ.
	BWCONTROL_RECONNECT,					// ��������.
	SHOW_ERROR_MSG,							// ������Ϣ.
}BWControlType;

// ��ʾ����.
typedef enum _ShowProportion
{
	SHOW_PROPORTION_ORIGINAL = 0,					// ���.
	SHOW_PROPORTION_4_3,							// 4:3.
	SHOW_PROPORTION_16_9,							// 16:9.
}ShowProportion;

/***************STRUCT*************************************************/

//�طŲ���
typedef struct _stPlayParam
{
	int nChannel;					   // ͨ����.
	char szStreamID[32];               //��ID,CVRʱ��Ч
	ENUM_PLAY_TYPE emPlayType;         //�طŷ�ʽ
	ENUM_RECORD_TYPE emRecordType;     //¼������
	PB_TIME startTime;                 //��ʼʱ��
	PB_TIME stopTime;                  //����ʱ��
	BOOL bByCardNum;				   // �Ƿ񰴿��Ų�ѯ.
    CString strCardNum;				   // ����.
	_stPlayParam()
	{
		nChannel = 0;
		szStreamID[0] = '\0'; 
		emPlayType = PB_TYPE_DVR;
		emRecordType = RECORD_TYPE_ALL;
		bByCardNum = FALSE;
		strCardNum = _T("");
	}
}PLAY_PARAM;

// ��������.
typedef struct _stDownloadType
{
	DOWNLOAD_FILE_INFO downloadFileParam;
	DOWNLOAD_TIME_INFO downloadTimeParam;
}DownloadType;

//����������Ϣ
typedef struct _stDownloadInfo
{
	int nCameraID;         //��ص�ID
	int nChannel;			// ͨ����.
	char szCameraName[255];     //��ص�����
	ENUM_PLAY_TYPE emDownType;  //���ط�ʽ
	BOOL bByTime;            //�Ƿ�ʱ������
	PB_LOGININFO stLoginInfo;	// ��¼��Ϣ.
	DWORD dwRev;			 // Ԥ���ֶ�.
	DownloadType unDownloadParam;	// ����������Ϣ.
	UINT nPos;               //���ؽ���
	BOOL bDownload;          //�Ƿ�������
	_stDownloadInfo()
	{
		nCameraID = -1;
        nChannel = -1;
		dwRev = 0;
		szCameraName[0] = '\0';
		emDownType = PB_TYPE_DVR;
		bByTime = FALSE;
		bDownload = FALSE;
		nPos = 0;
	}
	BOOL operator == (_stDownloadInfo & st)
	{
		if (nCameraID != st.nCameraID || emDownType != st.emDownType || bByTime != st.bByTime  ||  dwRev != st.dwRev)
		{
			return FALSE;
		}
		if (bByTime)
		{
			if (unDownloadParam.downloadTimeParam.lChannel != st.unDownloadParam.downloadTimeParam.lChannel || 
					unDownloadParam.downloadTimeParam.stStartTime != st.unDownloadParam.downloadTimeParam.stStartTime || 
						unDownloadParam.downloadTimeParam.stStopTime != st.unDownloadParam.downloadTimeParam.stStopTime)
			{
				return FALSE;
			}
		}
		else
		{
			if (strcmp(unDownloadParam.downloadFileParam.stFileInfo.szFileName, st.unDownloadParam.downloadFileParam.stFileInfo.szFileName) != 0)
			{
				return FALSE;
			}
		}

		return TRUE;
	}
}DOWNLOAD_INFO;

//�طű�ǩ��Ϣ
typedef struct _stPlaybackLabelInfo
{
	int nID;
	int nCameraID;                     //��ص�ID
	CString strCameraName;             //��ص�����
	CString strFileName;               //�ļ�����
	CString strStartTime;              //��ʼʱ��
	CString strStopTime;               //����ʱ��
	ENUM_PLAY_TYPE emPlayType;         //����Դ
	CString strLabelInfo;              //��ǩ��Ϣ
	CString strLabelTime;              //��ǩʱ��
	CString strLabelUser;              //��ǩ�����û�
}PLAYBACK_LABELINFO;

//������Դ��ϸ��Ϣ��
typedef struct _stGroupResDetail
{
	int nGroupID;
	int nCameraID;
	CString	strCamName;
	CString strRegionName;
}GROUP_RES;

// ����̬����.
typedef struct _TaskDynData
{
	LPVOID pObject;			// ����ָ��.
}TaskDynData, *LPTaskDynData;

// ����ʱ�䷶Χ�ṹ��.
typedef struct _ReverseTimeData
{
	PB_TIME tStart[MAX_PLAYWND_NUM];
	PB_TIME tStop[MAX_PLAYWND_NUM];
}ReverseTimeData, *LPReverseTimeData;
// �����̿ռ��߳���Ϣ.
typedef struct _ThreadDataCheckSpace
{
	CString strFilePath;				// �ļ�·��.
	LPVOID pParam;						// �Ի���ָ��.
}ThreadDataCheckSpace, *LPThreadDataCheckSpace;

// �����Ϣ.
typedef struct _FrameMsgData
{
	int nCmdID;							// ��Ϣ����.
	std::string sMsg;					// ��Ϣ������.
}FrameMsgData, *LPFrameMsgData;

// �ط���һ���ļ��ṹ����Ϣ.
typedef struct _OrderPlayData
{
	int nCameraID;						// ��ص�ID.
	CString strCamName;					// ��ص�����.
	int nChannel;						// ͨ����. 
	char szStreamID[32];                // ��ID
	PB_FILEINFO stFileInfo;				// �ļ���Ϣ. 
	PB_LOGININFO stLoginInfo;			// ��¼��Ϣ. 
	int nWndID;							// ���ں�.
	DWORD dwRight;						// �ط�Ȩ��.
	DWORD dwRev;						// ������Ϣ.
}OrderPlayData, *LPOrderPlayData;