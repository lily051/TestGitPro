#pragma once
#include "plaympeg4.h"
#include "NVRAPIClient.h"
#include "afxwin.h"

#define POOLSIZE 1024*1024

typedef struct _Play_Info{
    unsigned int m_nLoginID;
    char CamerID[256];
    time_t startTime ;
    time_t endTime;
    _Play_Info()
    {
        m_nLoginID = 0;
        startTime = 0;
        endTime = 0;
    }

}Play_Info;
//Play_Info m_StructPlayInfo;
// PlayMp4Cvr 对话框

class PlayMp4Cvr : public CDialog
{
	DECLARE_DYNAMIC(PlayMp4Cvr)

public:
	PlayMp4Cvr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PlayMp4Cvr();
    virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_PLAY };

    //static Play_Info GetPlayInfo(){return m_StructPlayInfo;}

protected:
    HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonPlayStart();
    //static void CALLBACK HK_FILEPLAY(unsigned int playhandle,BYTE *data,DWORD datalen,NVR_DWORD user);
    static Play_Info m_StructPlayInfo;
    UINT PlayHandle;
    CStatic m_PlayShow;
    HANDLE m_Event;

    
};
