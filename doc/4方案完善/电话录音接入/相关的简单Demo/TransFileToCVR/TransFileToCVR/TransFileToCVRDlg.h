
// TransFileToCVRDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include <iostream>
#include <vector>
#include "afxdtctl.h"
#include "NVRAPIClient.h"
#include "PlayMp4Cvr.h"
using namespace std;
typedef unsigned int bn_handle_t;

class CTransFileToCVRDlg;
#define BUFFSIZE   1024*1024
#define MAX_LEN    256
#define IDM_GET_FILE_SIZE WM_USER + 1
#define IDM_GET_PLAY_MP4 WM_USER + 2
// CTransFileToCVRDlg �Ի���
class CTransFileToCVRDlg : public CDialog
{
// ����
public:
	CTransFileToCVRDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TRANSFILETOCVR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
    CIPAddressCtrl m_strIPAddressCtrl;
    CString CvrDeviceIp;
    volatile bool m_bLogin;
    volatile bn_handle_t m_nLoginID;
    volatile bn_handle_t m_nStartRecordHandle;
    volatile bn_handle_t m_nPlayHandleLoad;

    CDateTimeCtrl m_tDateStart;
    CDateTimeCtrl m_tTimeStart;
    CDateTimeCtrl m_tDataEnd;
    CDateTimeCtrl m_tTimeEnd;
    
    HANDLE m_SendHandle;
    CArray<CString, CString> ary_filename;//��CString�������ļ���·��
    CArray<CString, CString> ary_fileTitle;//����ļ��ı���

public:
    //void CALLBACK GetResult(unsigned int logInID,const char *CamerID,unsigned int beginTime,unsigned int endTime,const char *Alarmtype,const char*desc,int desclen,NVR_DWORD duser);
    DWORD WINAPI StartSendFileToCVR(LPVOID LPARAM);
    afx_msg void OnBnClickedButtonLogin();
    afx_msg void OnBnClickedButtonSelectFile();
    afx_msg void OnBnClickedButtonTransToCvr();
    afx_msg void OnBnClickedButtonFindFile();
    afx_msg void GetSelectFileSize();
    afx_msg void StartPlay();
    CListCtrl m_listFindFile;
    int m_nCountList;
    char m_FileName[MAX_LEN];
    volatile bool m_bLoading;
    volatile bool m_bLoadingReverse;

    HMENU hMenu;
    //CMenu popMenu;

    PlayMp4Cvr dlg;
    Play_Info m_StructPlayInfo;
public:
    afx_msg void OnBnClickedButtonDownloade();
    afx_msg void OnBnClickedButtonRecordDel();
    afx_msg void OnBnClickedButtonStopLoad();
    afx_msg void OnBnClickedButtonDownloadeReverse();
    afx_msg void OnBnClickedButtonStopLoadRe();
    afx_msg void OnNMRClickListFindFile(NMHDR *pNMHDR, LRESULT *pResult);
};
