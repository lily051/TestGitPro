
// UpdateDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CUpdateDlg �Ի���
class CUpdateDlg : public CDialog
{
// ����
public:
	CUpdateDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UPDATE_DIALOG };

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
public:
	CListCtrl m_StoreItems;
	queue<CString> download_list;
	void InitCtr(void);
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnDownload();
	void PushList(void);
	void WriteConfig(int i,struct fileStruct &fileInfo);
	void DeleteConfig( );
	void FindFiles(void);
	void GetAllFileInfo(void);
	void WriteCommon(void);
	BOOL GetFileVersion(LPCTSTR strFile, CString& strVersion,CString &filename,CString &description);   
	bool StartConnect(void);

	//afx_msg void OnBnClickedButtonCon();
	CString GeTPath(void);
};

typedef struct fileStruct
{
	CString Name;//=replace2.dll
//# �ļ���С
	ULONGLONG filesize;//=246
//# �ļ����ص�ַ
	CString	URL;//=http://127.0.0.1/download/AutoUpdate/replace2.dll
//# �ļ�ѹ����ʽ
	CString CompressMethod;//=None
//# �ļ�ժҪ�룬�ñ�׼MD5�㷨�õ�
	CString	Hash;//=E2DC119208AB13BB254E0DD7DEC04466
//# �ļ���װ��·��������ڰ�װĿ¼
	CString Subcatalog;//=1\//
}fileStruct;

