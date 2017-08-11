#pragma once
#include "atltypes.h"


// CSplashDlg �Ի���

class CSplashDlg : public CDialog
{
	DECLARE_DYNAMIC(CSplashDlg)

public:
	CSplashDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSplashDlg();

// �Ի�������
	enum { IDD = IDD_DLG_SPLASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

	//���ó�ʼ����
	void SetText(const CString& strTitle, const CString& strVer, const CString& strText);
	//���ö�̬����
	void SetText(const CString& strText);
	//��ʾ����
	void Show();
	//���ؽ���
	void Hide();
	//ˢ�½���
	void SplashStep(CDC* pDC);
	//������ͼ
	void DrawBkg(CDC* pDC);
	//ˢ�½����̺߳���
	void ThrobberProc();

private:
	Image *m_pImgBkg;
	CRect m_rcWnd;
	HANDLE m_hExitThread;
	HANDLE m_hThread;
	CString m_strTitle;
	CString m_strText;
	CString m_strVer;
	
};
