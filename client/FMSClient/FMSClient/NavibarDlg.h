#pragma once

#define MODULE_ADD      1
#define MODULE_DEL      2
#define MODULE_HEAD     3
// CNavibarDlg �Ի���

class CNavibarDlg : public CDialog
{
	DECLARE_DYNAMIC(CNavibarDlg)

public:
	CNavibarDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNavibarDlg();

// �Ի�������
	enum { IDD = IDD_DLG_NAVIBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	//�����ť��˸<ComponentID, times>
	std::map<int, int> m_mapBlink;
	//CRITICAL_SECTION m_csMapBlink;

	//bool m_isDraging;		//�Ƿ�����ק״̬
	//bool m_isBtnDown;	//��ť�Ƿ���
	//DWORD m_nPushTime;	//��ť���µ�ʱ��
	//int m_nDragComponentID;	//����ק�����ID
	//int m_nCurComponentID;	//��ǰ��ʾ�����ID

	Image * m_pImgBkg;
	CImageList m_imgListDrag;  //��קʱ��ͼ��
	CFButton * m_pBtnHome;
	//���ఴť
	CFButton *m_pBtnMore;
	

public:
	void ModuleOper(int nOper, int nComponentID);

	//����һ��ģ��İ�ť��nComponentID: ģ��ID
	void _AddModule(int nComponentID, bool bSecondWnd);
	
	void _DelModule(int nComponentID);

	//�����ص�ģ����ǰ�ŵ���ҳλ��
	void HeadModule(int nComponent);

	//����ģ�鰴ťͼ��
	CString GetModuleIconName(const int nComponent);

	//���ݴ��ڴ�С������ť
	void LayoutBtn(BOOL bResize);
	
	//��ȡ��ťָ��
	CButton * GetBtnByComponentID(const int nComponentID);

	//����Ƿ���ʾ
	BOOL IsComponentShow(const int nComponentID);

	//����Ƿ���ʾ�ڵڶ�����
	BOOL IsComSecondWnd(const int nComponentID);

	//�ı䰴ťѡ��״̬
	void ChangeBtnHover(const int nComponentID);

	//�жϵ���İ�ť
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
