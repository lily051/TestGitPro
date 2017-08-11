#pragma once

#define MODULE_ADD      1
#define MODULE_DEL      2
#define MODULE_HEAD     3
// CNavibarDlg 对话框

class CNavibarDlg : public CDialog
{
	DECLARE_DYNAMIC(CNavibarDlg)

public:
	CNavibarDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNavibarDlg();

// 对话框数据
	enum { IDD = IDD_DLG_NAVIBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	typedef struct _tagBtn
	{
		bool bShow;
		bool bSecondWnd;
		int nComponentID;
		CFButton* pBtn;

		_tagBtn()
		{
			bShow = false;
			bSecondWnd = false;
			nComponentID = 0;
			pBtn = NULL;
		}
	}NAVI_BTN;
	std::list<NAVI_BTN> m_lsNaviBtn;
	CRITICAL_SECTION m_csNaviBtn;

	//组件按钮闪烁<ComponentID, times>
	std::map<int, int> m_mapBlink;
	//CRITICAL_SECTION m_csMapBlink;

	//bool m_isDraging;		//是否在拖拽状态
	//bool m_isBtnDown;	//按钮是否按下
	//DWORD m_nPushTime;	//按钮按下的时间
	//int m_nDragComponentID;	//被拖拽的组件ID
	//int m_nCurComponentID;	//当前显示的组件ID

	Image * m_pImgBkg;
	CImageList m_imgListDrag;  //拖拽时的图标
	CFButton * m_pBtnHome;
	//更多按钮
	CFButton *m_pBtnMore;
	

public:
	void ModuleOper(int nOper, int nComponentID);

	//增加一个模块的按钮，nComponentID: 模块ID
	void _AddModule(int nComponentID, bool bSecondWnd);
	
	void _DelModule(int nComponentID);

	//把隐藏的模块提前放到首页位置
	void HeadModule(int nComponent);

	//设置模块按钮图标
	CString GetModuleIconName(const int nComponent);

	//根据窗口大小调整按钮
	void LayoutBtn(BOOL bResize);
	
	//获取按钮指针
	CButton * GetBtnByComponentID(const int nComponentID);

	//组件是否显示
	BOOL IsComponentShow(const int nComponentID);

	//组件是否显示在第二窗口
	BOOL IsComSecondWnd(const int nComponentID);

	//改变按钮选择状态
	void ChangeBtnHover(const int nComponentID);

	//判断点击的按钮
	BOOL GetClickButton(const CPoint pt, NAVI_BTN & naviBtn);

	void DrawBkg(CDC *pDC);
	void ShowMore();

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnMsgAddModule(WPARAM, LPARAM);
};
