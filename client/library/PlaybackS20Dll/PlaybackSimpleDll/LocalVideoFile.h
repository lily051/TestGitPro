#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "./ui/uiBase/BtnST_EX.h"
#include "./ui/uiBase/SkinListCtrl.h"
#include "./ui/uiBase/ButtonST_Skin.h"
#include "./ui/uiBase/TitleBtn.h"
#include "./ui/uiBase/xSkinButton.h"


// CLocalVideoFile �Ի���

class CLocalVideoFile : public CDialog
{
	DECLARE_DYNAMIC(CLocalVideoFile)

public:
	CLocalVideoFile(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLocalVideoFile();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOCALVIDEOFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	// ��ʼ���ؼ�
	void InitCtrl();
	// �ƶ��ؼ�
	void MoveCtrl();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonOpenpath();
	afx_msg void OnNMDblclkTreeVideolist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonRefresh();

private:
	CImageList m_ImageList;
	CStatic m_stVideoPath;
	CEdit m_editVideoPath;
	CButtonST_EX m_btnVideoPath;
	CStatic m_stVideoType;
	CComboBox m_cmbVideoType;
	CTreeCtrl m_treeVideoList;
	CxSkinButton m_btnRefresh;

	CString  m_csFilePath;
	int m_nVideoType;

public:
	enum ENUM_VEDIOFILE_ITEM_TYPE
	{
		typeDirectory = 0,
		typeFile,
	};
    CString m_strEditPath;
    std::vector<CString> m_strVEditPath;
	//��ȡ�ļ�·��
	void GetFileInfo(int &nVideoType, CString &csFilePath);
	// ���ػط�.
	void LocalPlayback(const int nVideoType, const CString csFilePath);

private:
	//������Դ��
	void GetPathItem(CString strPath, HTREEITEM hRegionItem = NULL);
    void SetPathItem(std::vector<CString> strPath);
    void InitRecoredPath(const CString strFilePath, std::vector<CString>& sttVFilePath);
};
