#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "../ui/uiBase/TransButton.h"
#include "../ui/uiBase/SkinListCtrl.h"
#include "DownloadOper.h"

#define TIMER_ID_GETDOWNLOADPOS WM_USER + 1101
#define MSG_UPDATEBTN (WM_USER + 1102)

class CDownloadingDlg : public CDialog
{
	DECLARE_DYNAMIC(CDownloadingDlg)

public:
	CDownloadingDlg(HANDLE hDlg, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDownloadingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DOWNLOADING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bMove;
	HANDLE m_hDlg;
	CRect m_rcTitle;

	CDC m_dcMem;
	HBITMAP m_hBitmap;
	int m_nBitmapWidth;  //����ͼƬ���
	int m_nBitmapHeight;  //����ͼƬ�߶�

	LONG m_lDowningSeq;          //��ǰ�������ص���Ϣ����
	std::map<LONG, DOWNLOAD_INFO> m_mapDownInfo;  //������Ϣ
	CRITICAL_SECTION m_csDownlist;  //������Ϣ�ź���
	HANDLE m_hDownload;          //��ǰ���ؾ��
	int m_nCamID;				 // ��ǰ��ص�ID.
	DWORD m_dwTimeBegin;		 // ��ǰ����¼��Ŀ�ʼʱ��.
	DWORD m_dwTimeEnd;			 // ��ǰ����¼��Ľ���ʱ��.
	DWORD m_dwRev;				 // Ԥ���ֶ�.
	CDownloadOper m_opeDownload; // ���ؿ�����.
public:
	BOOL m_bDownloading;         //��������
	BOOL m_bWorking;             //�����߳����ڹ���
	HANDLE m_hDownloadThread;    //�����߳�

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
	/** @fn TransparentBk
	*   @͸����ͼ 
	*   @param[in] pDc: �ӿؼ�pDc
	*   @param[in] pWnd: �ӿؼ���� 
	*   @return NULL
	*/
	void TransparentBk(CDC * pDc, CWnd * pWnd);

	//�����߳�
	static void DownloadThreadFun(LPVOID lpvoid);

	//��ʼ����
	BOOL StartDownload();
	//���ص�������
	BOOL DownloadSingleTask(LONG lSeq, DOWNLOAD_INFO& stDownloadInfo);
	//ֹͣ����
	void StopDownload();
	//�¼�֪ͨ����
	BOOL CDownloadingDlg::NotifyStartDownload();

	//����һ����¼
	BOOL AddDownloadInfo(LONG lSeq, DOWNLOAD_INFO &stDownloadInfo);
	//ɾ��ѡ�еļ�¼
	void DelSelectInfo();
	//����б�
	void ClearList();
	// ��ȡ����״̬��Ϣ.
	BOOL GetPlaybackInfo(PlaybackStatData& stData);
	// ���̿ռ䲻��.
	HRESULT OnMsgCheckSpaceNotEnough(WPARAM wParam, LPARAM lParam);
    //���²˵�����ť״̬
    LRESULT UpdateMenuButtonState(WPARAM wParam, LPARAM lParam);
    //����ʧ����Ϣ
    LRESULT OnMsgDownloadFailed(WPARAM wParam, LPARAM lParam);
    //�������
    void ReSetListIndex();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CTransButton m_btnStartDownload;
	CTransButton m_btnStopDownload;
	CTransButton m_btnDelete;
	CTransButton m_btnClear;
	CSkinListCtrl m_listDownloading;
	afx_msg void OnBnClickedButtonStartdownload();
	afx_msg void OnBnClickedButtonStopdownload();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonClear();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnLvnItemchangedListDownloading(NMHDR *pNMHDR, LRESULT *pResult);
};
