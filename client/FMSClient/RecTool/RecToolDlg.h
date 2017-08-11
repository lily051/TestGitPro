
// RecToolDlg.h : ͷ�ļ�
//

#pragma once
#include <set>
#include "FUi/FDialog.h"
#include "PreviewCtrlDlg.h"
#include "Fui/flistctrl.h"
#include "PlaybackCtrlDlg.h"
#include "RecNameInfoDlg.h"
#include "BottomDlg.h"
#include "EditFileNameDlg.h"
#include "Fui/fstatic.h"
#include "ListInfoDlg.h"


// CRecToolDlg �Ի���
class CRecToolDlg : public CFDialog
{
// ����
public:
	CRecToolDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CRecToolDlg();

// �Ի�������
	enum { IDD = IDD_RECTOOL_DIALOG };

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void LoadSkin(void);

private:
	//Ԥ������
	CPreviewCtrlDlg * m_lpPreviewCtrlDlg;
	//¼�����ƴ���
	CRecNameInfoDlg * m_lpRecNameInfoDlg;
	//�طŴ���
	CPlaybackCtrlDlg *m_lpPlaybackCtrlDlg;
	//״̬��
	CBottomDlg * m_lpBottomDlg;
	//�ļ��б�
	CListInfoDlg * m_lpListInfoDlg;
public:
	void InitCtrl(void);
	void MoveCtrl(void);

	//����¼����ʾ֪ͨ��Ϣ
	void NotifyRecordFileInfo(const RECOED_FILE_INFO &recoedFile);

	BOOL StartPreview(void);

	bool m_bIsRecording;

	typedef struct _tagItemData
	{
		CString strFileName;
	}ITEMDATA, *LPITEMDATA;

	BOOL InsertRec(const RECOED_FILE_INFO &recordFile);
	BOOL InsertPic(const RECOED_FILE_INFO &recordFile);
	CFListCtrl m_listRecords;
	CArray<RECOED_FILE_INFO> m_PicFileArray;  //ͼƬ�б�
	CArray<RECOED_FILE_INFO> m_RecFileArray; //¼���б�
	CStringArray m_ArrayFileList;	//�ļ��б�
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

public:
    CFStatic m_staticBlank1;
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

extern CRecToolDlg * g_pRecMgrDlg;
