#pragma once
#include "../resource.h"
#include "FDialog.h"
#include "FButton.h"

// CMsgBoxDlg 对话框

class CMsgBoxDlg : public CFDialog
{
	DECLARE_DYNAMIC(CMsgBoxDlg)

public:
	CMsgBoxDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMsgBoxDlg();

// 对话框数据
	enum { IDD = IDD_DLG_MESSAGEBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg void OnBtnClicked(UINT nID);

private:
    // 标题
    CString m_strCaption;
    // 提示文字
    CString m_strText;
    // 提示文字字体 
    CString m_strTextFont;
    // 按钮类型
    UINT m_uBtnType;

    // 图标
    Image *m_pImgIcon;
    // 背景图
    Image *m_pImgBtnBkg;
    
    // 控件：按钮
    CFButton m_btnOK;
    CFButton m_btnCancel;
    CFButton m_btnYes;
    CFButton m_btnNo;
    CFButton m_btnRetry;

    CBrush m_brBkg;
private:
    // 初始化对话框
    void InitDlg();
    // 根据传入参数初始化按钮
    void InitBtn();
    // 根据传入的提示文字调整对话框的大小
    void CalcSize();

    // 对话框背景
    void DrawBkg(CDC *pDC);
    // 提示图标
    void DrawImg(CDC *pDC);
    // 提示文字
    void DrawMsg(CDC *pDC);
    // 绘制文字，支持设置行距
    void DrawMultLineText(CDC* pDC, CRect& rect, int nRowDis, UINT nFromat, CString strText);

public:
    // 提示框属性设置
    void SetText(const CString& strCaption, const CString& strText, UINT uType);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
