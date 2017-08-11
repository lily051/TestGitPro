#pragma once
#include "csclient/CsClientRecord.pb.h"
#include "fui\fbutton.h"
#include "CMSDataOperate.h"
#include "PlaybackCtrlDlg.h"

// CBusinessInfoDlg 对话框

class CBusinessInfoDlg : public CFDialog
{
	DECLARE_DYNAMIC(CBusinessInfoDlg)

public:
	CBusinessInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBusinessInfoDlg();

// 对话框数据
	enum { IDD = IDD_DLG_BUSINESS_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	void InitCtrl(void);
	void MoveCtrl(void);
	void InitData(void);
	void SetDialogData(const cms_8120::RecordsInfo& recordInfo);
	void SetEditEnable(BOOL bEnable);
	void LoadIDType(void);

protected:
	//业务信息
	CFButton m_btnBusinessInfo;
	CFStatic m_staticBusinessInfo;	
	CFStatic m_staticBusinessId;		//业务单号
	CFEdit m_editBusinessId;			
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
	CFStatic m_staticProductType;			//产品类型
	CTreeComboBox m_comboProductType;
	CFStatic m_staticProductName;			//产品名称
	CFComboBox m_comboProductName;
	CFStatic m_staticProductCode;			//产品编码
	CFEdit m_editProductCode;
	CFStatic m_staticIssuer;						//发行机构
	CFEdit m_editIssuer;
	CFStatic m_staticPurchaseAmount;	//购买金额
	CFEdit m_editPurchaseAmount;

	//录像信息
	CFButton m_btnRecordInfo;
	CFStatic m_staticRecordInfo;
	CFButton m_btnAddRecord;
	CFListCtrl m_listRecords;

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

	//加载区域
	void LoadRegion(HTREEITEM hParentItem = TVI_ROOT, int nParentId = 0);
	//展开树
	void ExpandBranch(HTREEITEM hItem, CTreeCtrl& tree, BOOL bExpand =TRUE);
	//插入节点到树
	void InsertNode(int nType, HTREEITEM hParantNode, int nPId);

	//产品类型
	void LoadProductType(HTREEITEM hParentItem, int nParentId);

	//获取产品名称
void LoadProduct(CString & strTempName, HTREEITEM hParentItem, int nParentId);	

private:
	cms_8120::RecordsInfo m_businessInfo;

public:
	CFButton m_btnSave;
	afx_msg void OnBnClickedBtnSave();
	CFButton m_btnCancel;
	afx_msg void OnBnClickedBtnUpload();

	CArray<CString> m_importFileArray;	//本地视频列表
	BOOL InsertRec(const RECOED_FILE_INFO &recordFile);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	CPlaybackCtrlDlg *m_pPlaybackDlg;

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
	afx_msg void OnCbnDropdownComboProductname();
	afx_msg void OnCbnSelchangeComboProductname();
	BOOL GetRegionId();
	BOOL GetProductId();
	int GetProductIdByName(CString& strProductName);
	int GetProductTypeIdByName(CString& strProductTypeName);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	 CToolTipCtrl m_ToolTips;  
	 afx_msg void OnMouseLeave();
	 int m_nLastHover;
};
