#pragma once
#include ".\fui\fstatic.h"
#include ".\fui\fedit.h"
#include ".\fui\flistctrl.h"
#include ".\fui\fbutton.h"
#include ".\fui\fcombobox.h"
#include "PreviewCtrlDlg.h"
#include "PlaybackCtrlDlg.h"
#include "RecNameInfoDlg.h"
#include "HistoryListDlg.h"
#include ".\FUi\TreeComboBox.h"
#include "utils\ui\MyFEdit.h"

#include "afxcoll.h"
#include "afxwin.h"
#include "afxcmn.h"

#define CAM_TYPE_IPC	1
#define CAM_TYPE_USB	2

// CBusinessManageDlg 对话框

class CBusinessManageDlg : public CFDialog
{
	DECLARE_DYNAMIC(CBusinessManageDlg)

public:
	CBusinessManageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBusinessManageDlg();

// 对话框数据
	enum { IDD = IDD_DLG_BUSINESSMANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	//业务信息
	CFButton m_btnBusinessInfo;
	CFStatic m_staticBusinessInfo;	
	CFStatic m_staticBusinessId;		//业务单号
	CFEdit m_editBusinessId;			
	CFButton m_btnHistoryList;			//历史业务单
	CFStatic m_staticBusinessLocation;	//业务网点
	CTreeComboBox m_comboBusinessLocation;  
	CFStatic m_staticCustomerName;		//客户姓名
	CFEdit m_editCustomerName;			
	CFStatic m_staticCustomerPhone;		//客户电话
	CFEdit m_editCustomerPhone;			
	CFStatic m_staticIDType;					//证件类型
	CFComboBox m_comboIDType;
	CFStatic m_staticIDNumber;				//证件号码
	CFEdit m_editIDNumber;

	//产品信息
	CFButton m_btnProductInfo;
	CFStatic m_staticProductInfo;
	CFButton m_btnWords;					//产品话术
	CFStatic m_staticProductType;			//产品类型
	CTreeComboBox m_comboProductType;
	CFStatic m_staticProductName;			//产品名称
	CFComboBox m_comboProductName;
	CFStatic m_staticProductCode;			//产品编码
	CMyFEdit m_editProductCode;
	CFStatic m_staticIssuer;						//发行机构
	CFEdit m_editIssuer;
	CFStatic m_staticPurchaseAmount;	//购买金额
	CFEdit m_editPurchaseAmount;

	//业务员信息
	CFButton m_btnSalesmanInfo;
	CFStatic m_staticSalesmanInfo;
	CFStatic m_staticSalesmanName1;	//业务员姓名
	CFStatic m_staticSalesmanName2;
	CFStatic m_staticSalesmanId1;			//业务员工号
	CFStatic m_staticSalesmanId2;

	CFListCtrl m_listRecords;			//录像列表
	CFStatic m_staticRecList;
	CFButton m_btnUpLoad;		//添加本地文件

	CFButton m_btnPrint;				//打印
	CFButton m_btnSubmit;			//提交
	CFButton m_btnReset;			//重置

	CFButton m_btnShowOSD;		//OSD
	
	//线条
	CFStatic m_staticBlank1;
	CFStatic m_staticBlank2;
	CFStatic m_staticBlank3;
	CFStatic m_staticBlank4;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

public:
	//初始化
	void InitCtrl(void);
	//页面布局
	void MoveCtrl(void);
	//初始化数据
	void InitData(void);
	//重置数据
	void ResetData(void);
	//开始预览
	BOOL StartPreview();
	//生成业务单号
	BOOL GenerateBusinessId(void);
	//加载区域
	void LoadRegion(HTREEITEM hParentItem = TVI_ROOT, int nParentId = 0);
	//加载证件类型
	void LoadIDType(void);
	//加载产品类型
	void LoadProductType(HTREEITEM hParentItem = TVI_ROOT, int nParentId = 0);
	//加载产品名称
	void LoadProduct(CString & strTempName, HTREEITEM hParentItem = TVI_ROOT, int nParentId = 0);
	//根据产品编码获取产品信息
	void GetProductInfoByCode(CString strCode);
	//插入节点到树
	void InsertNode(int nType, HTREEITEM hParantNode, int nPId);
	//处理录像提示通知信息
	void NotifyRecordFileInfo(const RECOED_FILE_INFO &recoedFile);
	//插入图片到文件列表
	BOOL InsertPic(const RECOED_FILE_INFO &recordFile);
	//插入录像到文件列表
	BOOL InsertRec(const RECOED_FILE_INFO &recordFile);

	BOOL StartSound(void);

public:
	//预览窗口
	CPreviewCtrlDlg * m_lpPreviewCtrlDlg;
	//录像名称窗口
	CRecNameInfoDlg * m_lpRecNameInfoDlg;
	//回放窗口
	CPlaybackCtrlDlg *m_lpPlaybackCtrlDlg;
	//历史业务单对话框
	CHistoryListDlg * m_pHistoryListDlg;	

	//bool m_bIsInit;
	
	CFTreeCtrlEx *m_pRegionTree;		//区域列表下拉树
	CStringArray m_ArrayRegionId;  //区域表ID列表
	CStringArray m_ArrayRegionPId; //区域表PID列表
	CStringArray m_ArrayRegionName; //区域表名字列表
	CStringArray m_ArrayProductTypeId;  //产品类型ID列表
	CStringArray m_ArrayProductTypePId; //产品类型PID列表
	CStringArray m_ArrayProductTypeName;	//产品类型名字列表
	CStringArray m_ArrayProductId;	//产品ID列表
	CStringArray m_ArrayProductPId;	//产品PID列表
	CStringArray m_ArrayProductName;	//产品名称列表
	CMap<int, int , CString, CString> m_IDMap;	//证件类型
	CArray<CString> m_importFileArray;	//本地视频列表
	CArray<RECOED_FILE_INFO> m_PicFileArray;  //图片列表
	CArray<RECOED_FILE_INFO> m_RecFileArray; //录像列表
	CStringArray m_ArrayFileList;		//图片和录像列表

	//展开树
	void ExpandBranch(HTREEITEM hItem, CTreeCtrl& tree, BOOL bExpand =TRUE);
	//选择产品
	LRESULT OnSelectProduct(WPARAM wparam, LPARAM lparam);
	//添加本地文件
	afx_msg void OnBnClickedBtnUpload();
	//产品下拉
	afx_msg void OnCbnDropdownComboProductname();
	//历史业务单按钮
	afx_msg void OnBnClickedBtnHistorylist();
	//选择产品名称
	afx_msg void OnCbnSelchangeComboProductname();
	//提交按钮
	afx_msg void OnBnClickedBtnSubmit();
	//获取区域Id
	BOOL GetRegionId(void);
	//获取产品Id
	BOOL GetProductId(void);
	//重置按钮
	afx_msg void OnBnClickedBtnReset();
	//产品话术按钮
	afx_msg void OnBnClickedBtnWords();
	//选择历史业务单
	LRESULT OnSelectHisClientInput(WPARAM wParam, LPARAM lParam);
	//打印
	afx_msg void OnBnClickedBtnPrint();

private:
	// 区域(网点)Id
	int m_nRegionId;
	// 业务单号(业务流水号)
	CString m_strBusinessId;
	//产品Id
	int m_nProductId;
	//区域下拉树
	CTreeCtrl * m_treeRegion;
	//产品类型下拉树
	CTreeCtrl * m_treeProductType;
	HTREEITEM m_hRoot;
	
public:
	bool m_bIsRecording;
	bool m_bIsHistory;
	//业务单主键id
	int m_nBusinessIndex;
	typedef struct _tagItemData
	{
		CString strFileName;
	}ITEMDATA, *LPITEMDATA;
	CListBox m_listBoxProductCode;
	afx_msg void OnEnChangeEditProductcode();
	afx_msg void OnLbnSelchangeListboxProductcode();
	//根据产品名称获取产品id
	int GetProductIdByName(CString& strProductName);
	//根据产品类型名称获取产品类型id
	int GetProductTypeIdByName(CString& strProductTypeName);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	CToolTipCtrl m_toolTips;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CFComboBox m_comboBusinessId;
	afx_msg void OnCbnDropdownComboBusinessid();
	void SetEnable(BOOL bEnable);
};

extern CBusinessManageDlg * g_pBusinessMgrDlg;