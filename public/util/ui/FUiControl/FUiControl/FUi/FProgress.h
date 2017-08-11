#pragma once
#include "FuiBase.h"

// CFProgress

class AFX_EXT_CLASS CFProgress : public CProgressCtrl
{
	DECLARE_DYNAMIC(CFProgress)

public:
	CFProgress();
	virtual ~CFProgress();

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();

private:
    // ����ͼƬ
    Gdiplus::Image *m_pImgBkg;
    // �ؼ�����
    CString m_strObjectName;
    // ������ɫ
    COLORREF m_clrText;
    // ��ʾ����
    CString m_strTextFontName;
    // �Զ�����ʾ����
    CString m_strText;

public:
    // ���ÿؼ�����
    void SetObjectName(LPCTSTR szObjectName);

    // ������ʾ���ֵ���ɫ
    void SetTextColor(COLORREF clrText);

    // �Զ�����ʾ������
    void SetText(LPCTSTR szText);

    // ��������
    void SetTextFont(const CString& strFontName);
};


