
// ResPackerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "zlibHelper.h"
#include "PicPreviewDlg.h"
#include "ColorListCtrl.h"


// CResPackerDlg �Ի���
class CResPackerDlg : public CDialog
{
// ����
public:
	CResPackerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RESPACKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	BOOL PackWithzlib(CString& strPath);


	//////////////////////////////////////////////////////////////////////////
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CTreeCtrl m_trRes;
	CPicPreviewDlg m_dlgPicPreview;
	CColorListCtrl m_lcLog;

	CString m_strResPath;
	CString m_strZipRootPath;	// zip�ļ��ı���·��
	CImageList m_imageList;

	zlibHelper m_zlib;
	CString m_strPassword;

	RC_INFO m_rcInfoList;

    std::vector<CString> m_vecTempResPath;

	// �˵�����ļ���������Դ����������
	CMenu m_menu;
	afx_msg void OnMenuOpen();
    afx_msg void OnMenuNew();
	afx_msg void OnMenuSaveRed();
    afx_msg void OnMenuSaveBlue();
    afx_msg void OnMenuExtract();
	afx_msg void OnMenuPassword();

	BOOL m_bInit;
	void InitCtrl();
	void MoveCtrl();
	void AddLog(const CString& strMsg, int nLevel = 0);
	// ������Դ��
	void BuildRcTree(const RC_INFO& rcInfo, HTREEITEM hItem = TVI_ROOT);
	//
	void ListRes(RC_INFO& rcInfo);
	// 
	BOOL SaveRc(const int nSkinType = 0);
	// 
	BOOL AddRes2Zip(zlibHelper& zlib, RC_INFO& rcInfo);

	// �Զ�����Ϣ������Դ�ļ�
	afx_msg LRESULT OnMsgOpenRc(WPARAM, LPARAM);
	// �Զ�����Ϣ�������Դ�ļ���
	afx_msg LRESULT OnMsgAddRes(WPARAM, LPARAM);
	// 
	afx_msg void OnTvnSelchangedTreeRes(NMHDR *pNMHDR, LRESULT *pResult);
    
    afx_msg void OnClose();
};
