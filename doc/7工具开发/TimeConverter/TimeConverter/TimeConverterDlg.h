
// TimeConverterDlg.h : ͷ�ļ�
//

#pragma once


// CTimeConverterDlg �Ի���
class CTimeConverterDlg : public CDialog
{
// ����
public:
	CTimeConverterDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TIMECONVERTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CString m_Date;
    ULONGLONG m_TimeStamp;
    afx_msg void OnEnKillfocusDateEdit();
    afx_msg void OnEnKillfocusTimestampEdit();
};
