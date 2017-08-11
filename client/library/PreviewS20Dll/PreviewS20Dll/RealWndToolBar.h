#pragma once
#include "UI/ButtonST_Skin.h"
#include "util/CToolTips.h"


#define BTN_ID_CAPTURE		MENU_ID_CAPTURE       //ץͼ
#define BTN_ID_RECORD		MENU_ID_RECORD        //¼�� 
#define BTN_ID_SOUND		MENU_ID_SOUND         //��Ƶ
#define BTN_ID_STREAM		MENU_ID_STREAMTYPE    //������
#define BTN_ID_OSD  		MENU_ID_OSD           //OSD��Ϣ
#define BTN_ID_4OSDINFO		MENU_ID_4OSD          //OSD4����ʾ
#define BTN_ID_8OSDINFO		MENU_ID_8OSD          //OSD8����ʾ
#define BTN_ID_STOP			MENU_ID_STOP_PREVIEW  //ֹͣԤ��

//���ϰ�ť����
#define PVWND_TOOLBAR_BTN_MAXNUM    5

// CRealWndToolBar �Ի���

class CRealWndToolBar : public CWnd
{
	DECLARE_DYNAMIC(CRealWndToolBar)

public:
	CRealWndToolBar(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRealWndToolBar();

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnToolBarBtnClk_Capture();
    afx_msg void OnToolBarBtnClk_Record();
    afx_msg void OnToolBarBtnClk_Sound();
    afx_msg void OnToolBarBtnClk_Stream();
	afx_msg void OnToolBarBtnClk_OSD();
    afx_msg void OnToolBarBtnClk_Stop();
	virtual void PreSubclassWindow();

private:
    CButtonST_Skin m_btnCapture;
    CButtonST_Skin m_btnRecord;
    CButtonST_Skin m_btnSound;
    CButtonST_Skin m_btnStreamType;
	CButtonST_Skin m_btnOsdInfo;
	CButtonST_Skin m_btnStop;

public:
	afx_msg void OnPaint();	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void CreateChildWnd();
	void InitCtrl();
	void InitPos();
	void OnBtnClick(UINT nID);
    void SetToolBarBtnTips();
    void CheckShowing4Btns();
};
