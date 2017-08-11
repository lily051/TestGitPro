#pragma once
#include "csclient/CsClientRecord.pb.h"
#include "fui\fbutton.h"
#include "CMSDataOperate.h"
#include "PlaybackCtrlDlg.h"

// CBusinessInfoDlg �Ի���

class CBusinessInfoDlg : public CFDialog
{
	DECLARE_DYNAMIC(CBusinessInfoDlg)

public:
	CBusinessInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBusinessInfoDlg();

// �Ի�������
	enum { IDD = IDD_DLG_BUSINESS_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	//ҵ����Ϣ
	CFButton m_btnBusinessInfo;
	CFStatic m_staticBusinessInfo;	
	CFStatic m_staticBusinessId;		//ҵ�񵥺�
	CFEdit m_editBusinessId;			
	CFStatic m_staticBusinessLocation;	//ҵ������
	CTreeComboBox m_comboBusinessLocation;  
	CFStatic m_staticCustomerName;		//�ͻ�����
	CFEdit m_editCustomerName;			
	CFStatic m_staticCustomerPhone;		//�ͻ��绰
	CFEdit m_editCustomerPhone;			
	CFStatic m_staticIDType;					//֤������
	CFComboBox m_comboIDType;
	CFStatic m_staticIDNumber;				//֤������
	CFEdit m_editIDNumber;

	//��Ʒ��Ϣ
	CFButton m_btnProductInfo;
	CFStatic m_staticProductInfo;				
	CFStatic m_staticProductType;			//��Ʒ����
	CTreeComboBox m_comboProductType;
	CFStatic m_staticProductName;			//��Ʒ����
	CFComboBox m_comboProductName;
	CFStatic m_staticProductCode;			//��Ʒ����
	CFEdit m_editProductCode;
	CFStatic m_staticIssuer;						//���л���
	CFEdit m_editIssuer;
	CFStatic m_staticPurchaseAmount;	//������
	CFEdit m_editPurchaseAmount;

	//¼����Ϣ
	CFButton m_btnRecordInfo;
	CFStatic m_staticRecordInfo;
	CFButton m_btnAddRecord;
	CFListCtrl m_listRecords;

	CFTreeCtrlEx *m_pRegionTree;		//�����б�������
	CStringArray m_ArrayRegionId;  //�����ID�б�
	CStringArray m_ArrayRegionPId; //�����PID�б�
	CStringArray m_ArrayRegionName; //����������б�
	CStringArray m_ArrayProductTypeId;  //��Ʒ����ID�б�
	CStringArray m_ArrayProductTypePId; //��Ʒ����PID�б�
	CStringArray m_ArrayProductTypeName;	//��Ʒ���������б�
	CStringArray m_ArrayProductId;	//��ƷID�б�
	CStringArray m_ArrayProductPId;	//��ƷPID�б�
	CStringArray m_ArrayProductName;	//��Ʒ�����б�
	CMap<int, int , CString, CString> m_IDMap;	//֤������

	//��������
	void LoadRegion(HTREEITEM hParentItem = TVI_ROOT, int nParentId = 0);
	//չ����
	void ExpandBranch(HTREEITEM hItem, CTreeCtrl& tree, BOOL bExpand =TRUE);
	//����ڵ㵽��
	void InsertNode(int nType, HTREEITEM hParantNode, int nPId);

	//��Ʒ����
	void LoadProductType(HTREEITEM hParentItem, int nParentId);

	//��ȡ��Ʒ����
void LoadProduct(CString & strTempName, HTREEITEM hParentItem, int nParentId);	

private:
	cms_8120::RecordsInfo m_businessInfo;

public:
	CFButton m_btnSave;
	afx_msg void OnBnClickedBtnSave();
	CFButton m_btnCancel;
	afx_msg void OnBnClickedBtnUpload();

	CArray<CString> m_importFileArray;	//������Ƶ�б�
	BOOL InsertRec(const RECOED_FILE_INFO &recordFile);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	CPlaybackCtrlDlg *m_pPlaybackDlg;

private:
	// ����(����)Id
	int m_nRegionId;
	// ҵ�񵥺�(ҵ����ˮ��)
	CString m_strBusinessId;
	//��ƷId
	int m_nProductId;
	//����������
	CTreeCtrl * m_treeRegion;
	//��Ʒ����������
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
