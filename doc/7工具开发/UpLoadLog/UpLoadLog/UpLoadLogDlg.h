// UpLoadLogDlg.h : ͷ�ļ�
//

#pragma once


// CUpLoadLogDlg �Ի���
class CUpLoadLogDlg : public CDialog
{
// ����
public:
	UploadThread *m_upthread;
	char zipfile[MAX_PATH];
	char file[MAX_PATH];
public:
	CUpLoadLogDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UPLOADLOG_DIALOG };

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
	afx_msg void OnNcPaint();
	afx_msg LRESULT UserMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	void UpLoad();        //�ϴ��ļ�
	void SetUploadInfo(); //�����ϴ���Ϣ
	void GetExePath();    //��ȡ���򱾵�·��
protected:
	virtual void OnCancel();
};
