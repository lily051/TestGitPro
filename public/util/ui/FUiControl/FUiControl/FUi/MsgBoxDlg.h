#pragma once
#include "../resource.h"
#include "FDialog.h"
#include "FButton.h"

// CMsgBoxDlg �Ի���

class CMsgBoxDlg : public CFDialog
{
	DECLARE_DYNAMIC(CMsgBoxDlg)

public:
	CMsgBoxDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMsgBoxDlg();

// �Ի�������
	enum { IDD = IDD_DLG_MESSAGEBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg void OnBtnClicked(UINT nID);

private:
    // ����
    CString m_strCaption;
    // ��ʾ����
    CString m_strText;
    // ��ʾ�������� 
    CString m_strTextFont;
    // ��ť����
    UINT m_uBtnType;

    // ͼ��
    Image *m_pImgIcon;
    // ����ͼ
    Image *m_pImgBtnBkg;
    
    // �ؼ�����ť
    CFButton m_btnOK;
    CFButton m_btnCancel;
    CFButton m_btnYes;
    CFButton m_btnNo;
    CFButton m_btnRetry;

    CBrush m_brBkg;
private:
    // ��ʼ���Ի���
    void InitDlg();
    // ���ݴ��������ʼ����ť
    void InitBtn();
    // ���ݴ������ʾ���ֵ����Ի���Ĵ�С
    void CalcSize();

    // �Ի��򱳾�
    void DrawBkg(CDC *pDC);
    // ��ʾͼ��
    void DrawImg(CDC *pDC);
    // ��ʾ����
    void DrawMsg(CDC *pDC);
    // �������֣�֧�������о�
    void DrawMultLineText(CDC* pDC, CRect& rect, int nRowDis, UINT nFromat, CString strText);

public:
    // ��ʾ����������
    void SetText(const CString& strCaption, const CString& strText, UINT uType);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
