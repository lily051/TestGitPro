#pragma once
#include "afxwin.h"
#include "../ui/uiBase/TitleBtn.h"
#include "../ui/uiBase/IconStatic.h"


// CDownloadTitleDlg �Ի���

class CDownloadTitleDlg : public CDialog
{
	DECLARE_DYNAMIC(CDownloadTitleDlg)

public:
	CDownloadTitleDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDownloadTitleDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DOWNLOADTITLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bMove;
	CBrush m_bkBrush;

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
	/** @fn SetStaText
	*   @brief ����static�ؼ�����
	*   @param[in]  NULL
	*   @param[out] NULL
	*   @return NULL
	*/
	void SetStaText();

public:
	int m_nDowningCount;      //�������ص��ļ�����
	int m_nDownSuccessCount;  //���سɹ����ļ�����
	int m_nDownFailedCount;   //����ʧ�ܵ��ļ�����

public:
	CTitleButton m_btnTitle;
	CIconStatic m_staDownloading;
	CIconStatic m_staDownSuccess;
	CIconStatic m_staDownFailed;

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedStaticDownloading();
	afx_msg void OnStnClickedStaticDownsuccess();
	afx_msg void OnStnClickedStaticDownfailed();
	afx_msg void OnPaint();
};
