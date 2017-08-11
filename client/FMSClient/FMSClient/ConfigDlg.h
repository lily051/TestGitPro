#pragma once
#include "fui\fbutton.h"
#include "fui\fstatic.h"
#include "fui\fedit.h"
#include "fui\fcombobox.h"
#include "csclient\CsSysconfig.pb.h"
#include "fui\fbutton.h"
#include "fui\fbutton.h"

#define MAX_RECORD_TIME		6100		//���¼��ʱ��
#define UPLOAD_TIME_START	6190		//�ϴ���ʼʱ��
#define UPLOAD_TIME_END		6191		//�ϴ�����ʱ��




// CConfigDlg �Ի���

class CConfigDlg : public CFDialog
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConfigDlg();

// �Ի�������
	enum { IDD = IDD_DLG_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	CFButton m_btnBasicParm;
	CFStatic m_staticBasicParm;
	CFStatic m_staticDefaultFolder;
	CFEdit m_editDefaultFolder;
	CFButton m_btnChangeDir;
	CFButton m_btnOpenFolder;
	CFButton m_btnSalesmanInfo;
	CFStatic m_staticSalesmanInfo;
	CFStatic m_staticDefaultIPC;
	CFComboBox m_comboDefaultIPC;
	CFStatic m_staticRecTime;
	CFEdit m_editRecTime;
	CFStatic m_staticAudioSet;
	CFButton m_checkOpenAduio;
	CFButton m_checkCloseAudio;
	CFButton m_btnUploadParm;
	CFStatic m_staticUploadParm;
	CFStatic m_staticUploadTime;
	CFEdit m_editUploadTimeStart;
	CFStatic m_staticAnd;
	CFEdit m_editUploadTimeEnd;

	void InitCtrl(void);
	void MoveCtrl(void);
	void InitData(void);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void CConfigDlg::GetLocalConfig();
	BOOL GetSystemConfig(void);
	CString m_strUploadStart;
	CString m_strUploadEnd;
	CFButton m_btnOK;
	CFButton m_btnCancel;
	afx_msg void OnBnClickedCheckOpen();
	afx_msg void OnBnClickedCheckClose();
	afx_msg void OnBnClickedBtnChangeDir();
	afx_msg void OnBnClickedBtnOpenFolder();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnDropdownComboDefaultIpc();
	CString m_strMaxRecordTime;
	CFButton m_btnSystemInfo;
	CFStatic m_staticSystemInfo;
	CFStatic m_staticStartupSet;
	CFButton m_checkStartup;
	CFStatic m_staticClose;
	CFButton m_checkExit;
	CFButton m_checkTray;
	afx_msg void OnBnClickedCheckExit();
	afx_msg void OnBnClickedCheckTray();
	CMap<int, int , CString, CString> m_USBMap;	//USB ����&����
	BOOL EnumAllUSB(void);
	int m_nCamIndex;
	CString m_strCamName;
	afx_msg void OnBnClickedReset();
	BOOL SetAutoRun(bool bRun);

public:
	//��ȡ��ǰ����USB�������
	int GetUSBCamCount(void);
	CFStatic m_staticMinute;
};
