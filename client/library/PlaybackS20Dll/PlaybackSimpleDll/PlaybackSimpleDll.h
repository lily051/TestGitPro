// PlaybackSimpleDll.h : PlaybackSimpleDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "DeviceManager.h"
#include "PlaybackDef.h"
#include "PlaybackMain.h"

// CPlaybackSimpleDllApp
// �йش���ʵ�ֵ���Ϣ������� PlaybackSimpleDll.cpp
//
#define FLAG_PLAYTYPE 100
class CPlaybackSimpleDllApp : public CWinApp
{
public:
	CPlaybackSimpleDllApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

public:
	int m_nSkinType;						// Ƥ������.
	HINSTANCE m_hResInstance;				// ��Դ���.
	HINSTANCE m_hOldInstance;				// ԭ������Դ���.
	HINSTANCE m_hIEFrame;					// ieframe.dll���ؾ��.
	CString	m_strModulePath;				// ����·��.
	PLAYBACK_PARAM m_stPlaybackParam;       // �ͻ��˲�����Ϣ.
	CDeviceManager m_manDevice;				// �豸������.
	DWORD m_dwStyle;						// ������.
	BOOL m_bIsIEProtected;					// �Ƿ���IE����ģʽ.
	PLAY_LAYOUT_WND m_emLayoutStyle;		// �طŻ���ָ�ģʽ.
	BOOL m_bBwControl;                      //�Ƿ���Ҫ�������
	BOOL m_bBwMode;							// �Ƿ��Ǵ������ģʽ.
	int m_nBWUserID;						// �������ID.
	PBSDUserInfo m_stDataInfo;                 //�ͻ��˵�¼��Ϣ
	LPVOID m_pClientVoid;                   //�ͻ��˶��������
	PBSDCmsData m_stCmsInfo;				// CMS���ݣ��������ʹ��.
	CString m_strLocalIPAddr;				// ����IP��ַ.
	CPlaybackMain * m_pPlaybackMain;        //�ط�����Ի���ָ��
	PBSDClientData m_stClientData;			// �ͻ���������Ϣ.
	LONG m_lBwControl;                       //֧�ִ������
	// �Ƿ��ʼ������.
	BOOL m_bInitProx;
	// �Ƿ��ʼ��PCNVR.
	BOOL m_bInitNvr;
	// �Ƿ��ʼ��bdp.
	BOOL m_bInitBdp;
	// �Ƿ��ʼ��FinAudio.
	BOOL m_bInitFinAudio;
	// ����Ȩ��
	PBSDRights m_stRights;

private:
    // SDK��ʼ��,�����ʼ������ֵ+100
    std::set<int> m_setSDKInit;

    CRITICAL_SECTION m_csSDK;
public:
	// ��ʼ��������Ϣ.
	void InitConfigInfo();
	//��ȡ��ص�����Ȩ��
	BOOL IsCamDownloadRight(DWORD dwRight);
	// ��ȡ��ص�ץͼȨ��.
	BOOL IsCamCaptureRight(DWORD dwRight);
	// ��ȡ��ص����Ȩ��.
	BOOL IsCamSaveRight(DWORD dwRight);
	//��ȡ����ڲ�Ȩ��
	BOOL GetTabRight(int& nTabRight);
	// �Ƿ�������ģ��.
	BOOL HasDownloadModule();
	// �Ƿ񱾵ػط�.
	BOOL HasLocalPlayback();
	// ��ȡIE�ͼ���·��.
	BOOL GetIESafePath(const CString& strFileName, CString& strTempPath, CString& strSelectPath, HANDLE& hState, IE_SAVE_FILETYPE emFileType, ENUM_PIC_TYPE emPicType = PIC_TYPE_BMP);
	// ����IE�ļ�.
	BOOL SaveIEFile(const CString& strPath, const HANDLE& hState);
	// ȡ������IE�ļ�.
	BOOL CancelIEFile(const HANDLE& hState);

	//��ȡ��ص�����Ȩ��
	virtual int ExitInstance();

    BOOL InitSDK(ENUM_DEVICE_TYPE emDevType, ENUM_PLAY_TYPE emPlayType);
    void UnInitSDK();

private:
	// ��ȡ�ļ������ַ���.
	void GetFileFilter(IE_SAVE_FILETYPE emFileType, ENUM_PIC_TYPE emPicType, CString& strFilter, CString& strExt);
};

extern CPlaybackSimpleDllApp theApp;