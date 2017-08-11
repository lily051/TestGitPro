#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "../ui/uiBase/TransButton.h"
#include "../ui/uiBase/SkinListCtrl.h"


// CDownFailedDlg �Ի���

class CDownFailedDlg : public CDialog
{
	DECLARE_DYNAMIC(CDownFailedDlg)

public:
	CDownFailedDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDownFailedDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DOWNFAILED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bMove;

	CRect m_rcTitle;

	CDC m_dcMem;
	HBITMAP m_hBitmap;
	int m_nBitmapWidth;  //����ͼƬ���
	int m_nBitmapHeight;  //����ͼƬ�߶�

public:
	std::map<LONG, DOWNLOAD_INFO> m_mapDownInfo;  //������Ϣ

private:
	/** @fn InitCtrl
	*   @brief ��ʼ���ؼ�
	*   @param[in]  NULL
	*   @param[out] NULL
	*   @return NULL
	*/
	void InitCtrl();

	/** @fn MoveCtrl
	*   @brief �ƶ��ؼ�
	*   @param[in]  NULL
	*   @param[out] NULL
	*   @return NULL
	*/
	void MoveCtrl();

public:

	//����һ����¼
	BOOL AddDownloadInfo(LONG lSeq, DOWNLOAD_INFO& stDownloadInfo, DWORD dwError);

	/** @fn TransparentBk
	*   @͸����ͼ 
	*   @param[in] pDc: �ӿؼ�pDc
	*   @param[in] pWnd: �ӿؼ���� 
	*   @return NULL
	*/
	void TransparentBk(CDC * pDc, CWnd * pWnd);

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CTransButton m_btnDelete;
	CTransButton m_btnClear;
	CTransButton m_btnReDownload;
	CSkinListCtrl m_listDownFailed;
	afx_msg void OnBnClickedButtonRedown();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnPaint();
    afx_msg void OnLvnItemchangedListDownloadfailed(NMHDR *pNMHDR, LRESULT *pResult);
};
