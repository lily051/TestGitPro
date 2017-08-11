#pragma once
#include "afxcmn.h"
#include "afxdtctl.h"
#include "hcs_sdk.h"
#include <string>
// CVideoActiveCloud 对话框

class CVideoActiveCloud : public CPropertyPage
{
	DECLARE_DYNAMIC(CVideoActiveCloud)

public:
	CVideoActiveCloud();
	virtual ~CVideoActiveCloud();

// 对话框数据
	enum { IDD = IDD_DIALOG_ACTIVE_CLOUD };

protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
private:
    CIPAddressCtrl m_ctrlDevIp;
    CString m_csUser;
    CString m_csPort;
    CString m_csPassWord;
    CString m_csChanNum;
    CString m_csDevName;

    CString m_csCodeID;//编码器ID
    CString m_csCodeName;//编码器名称
    CString m_csStreamType;//取流类型[0-设备取流，1-URL取流]
    CString m_csPoolID;

    CDateTimeCtrl m_DateStart;
    CDateTimeCtrl m_TimeStart;
    CDateTimeCtrl m_DateEnd;
    CDateTimeCtrl m_TimeEnd;
    unsigned long long m_nLogID;//登录句柄
    unsigned long long m_taskID;
public:
    afx_msg void OnBnClickedButtonStartRecord();

    void SetLoginID(unsigned long long nLogID){m_nLogID = nLogID;}

    
    afx_msg void OnBnClickedButtonSetVideoplan();
    afx_msg void OnBnClickedButtonGetVideostatus();
    afx_msg void OnBnClickedButtonStopRecord();
    afx_msg void OnBnClickedButtonDelVideoplan();
    afx_msg void OnBnClickedButtonQuryVideoplan();
    afx_msg void OnBnClickedButtonSetHisplan();
    afx_msg void OnBnClickedButtonDelHisplan();
    afx_msg void OnBnClickedButtonGetHisplan();
    afx_msg void OnBnClickedButtonCurplanQuery();
private:
    CListCtrl m_ListVideoPlan;
    int m_ItemIndexInfo;
public:
    afx_msg void OnBnClickedButtonHisplanQuery();
    afx_msg void OnBnClickedButtonEventRecord();
};
