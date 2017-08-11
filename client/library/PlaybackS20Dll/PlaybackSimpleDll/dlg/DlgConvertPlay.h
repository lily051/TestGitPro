#pragma once
#include "../ui/uiBase/SkinButton.h"
#include "../ui/uiBase/Label.h"
#include "afxwin.h"
#include "../ui/uiBase/ButtonST_Skin.h"
// CDlgConvertPlay 对话框
#include <list>
typedef struct _stru_AbilityDesc
{
	int nValue;
	CString strDesc;
}STRU_ABILITYDESC;

typedef struct _stru_Ability
{
	STRU_ABILITYDESC struRelolution;				//分辨率
	std::list<STRU_ABILITYDESC> listAbilityFrame;   //帧率
	std::list<STRU_ABILITYDESC> listAbilityBitrate; //码率
}STRU_ABILITY;

enum
{
	Type_Reso = 0,
	Type_Frame,
	Type_Bit,
};

class CDlgConvertPlay : public CDialog
{
	DECLARE_DYNAMIC(CDlgConvertPlay)

public:
	CDlgConvertPlay(BOOL bConvert,CONVERTPLAY_PARAM stConvertPlay
        , int nXPos, int nYPos, CString strXml, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgConvertPlay();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONVERT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbResolution;
	CComboBox m_cmbVedioBitrate;
	CComboBox m_cmbVedioFrameRate;
	CSkinButton m_btnOK;
	CSkinButton m_btnCancel;
    CLabel m_sttTitle;
    CButtonST_Skin m_btnClose;
    CLabel m_sttRes;
    CLabel m_sttBit;
    CLabel m_sttFrame;
    CStatic m_line1;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboResolution();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg LRESULT OnNcHitTest(CPoint point);
    afx_msg void OnBnClickedBtnClose();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
    int m_nXPos;
    int m_nYPos;
	CString m_strXml;
	std::map<int, STRU_ABILITY> m_mapAbility;
	void InitCtrl();
    void InitPos();
	void InitData();
	void ParseXml();
	void ParseString(int nType, CString strXml, std::list<STRU_ABILITYDESC>& listAbility);
	CString GetFrameDesc(int nValue);
	CString GetBitrateDesc(int nValue);

public:
	BOOL m_bConvert;
	CONVERTPLAY_PARAM m_stConvertPlay;
};
