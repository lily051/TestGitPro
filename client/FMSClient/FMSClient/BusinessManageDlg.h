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

// CBusinessManageDlg �Ի���

class CBusinessManageDlg : public CFDialog
{
	DECLARE_DYNAMIC(CBusinessManageDlg)

public:
	CBusinessManageDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBusinessManageDlg();

// �Ի�������
	enum { IDD = IDD_DLG_BUSINESSMANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	//ҵ����Ϣ
	CFButton m_btnBusinessInfo;
	CFStatic m_staticBusinessInfo;	
	CFStatic m_staticBusinessId;		//ҵ�񵥺�
	CFEdit m_editBusinessId;			
	CFButton m_btnHistoryList;			//��ʷҵ��
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
	CFButton m_btnWords;					//��Ʒ����
	CFStatic m_staticProductType;			//��Ʒ����
	CTreeComboBox m_comboProductType;
	CFStatic m_staticProductName;			//��Ʒ����
	CFComboBox m_comboProductName;
	CFStatic m_staticProductCode;			//��Ʒ����
	CMyFEdit m_editProductCode;
	CFStatic m_staticIssuer;						//���л���
	CFEdit m_editIssuer;
	CFStatic m_staticPurchaseAmount;	//������
	CFEdit m_editPurchaseAmount;

	//ҵ��Ա��Ϣ
	CFButton m_btnSalesmanInfo;
	CFStatic m_staticSalesmanInfo;
	CFStatic m_staticSalesmanName1;	//ҵ��Ա����
	CFStatic m_staticSalesmanName2;
	CFStatic m_staticSalesmanId1;			//ҵ��Ա����
	CFStatic m_staticSalesmanId2;

	CFListCtrl m_listRecords;			//¼���б�
	CFStatic m_staticRecList;
	CFButton m_btnUpLoad;		//��ӱ����ļ�

	CFButton m_btnPrint;				//��ӡ
	CFButton m_btnSubmit;			//�ύ
	CFButton m_btnReset;			//����

	CFButton m_btnShowOSD;		//OSD
	
	//����
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
	//��ʼ��
	void InitCtrl(void);
	//ҳ�沼��
	void MoveCtrl(void);
	//��ʼ������
	void InitData(void);
	//��������
	void ResetData(void);
	//��ʼԤ��
	BOOL StartPreview();
	//����ҵ�񵥺�
	BOOL GenerateBusinessId(void);
	//��������
	void LoadRegion(HTREEITEM hParentItem = TVI_ROOT, int nParentId = 0);
	//����֤������
	void LoadIDType(void);
	//���ز�Ʒ����
	void LoadProductType(HTREEITEM hParentItem = TVI_ROOT, int nParentId = 0);
	//���ز�Ʒ����
	void LoadProduct(CString & strTempName, HTREEITEM hParentItem = TVI_ROOT, int nParentId = 0);
	//���ݲ�Ʒ�����ȡ��Ʒ��Ϣ
	void GetProductInfoByCode(CString strCode);
	//����ڵ㵽��
	void InsertNode(int nType, HTREEITEM hParantNode, int nPId);
	//����¼����ʾ֪ͨ��Ϣ
	void NotifyRecordFileInfo(const RECOED_FILE_INFO &recoedFile);
	//����ͼƬ���ļ��б�
	BOOL InsertPic(const RECOED_FILE_INFO &recordFile);
	//����¼���ļ��б�
	BOOL InsertRec(const RECOED_FILE_INFO &recordFile);

	BOOL StartSound(void);

public:
	//Ԥ������
	CPreviewCtrlDlg * m_lpPreviewCtrlDlg;
	//¼�����ƴ���
	CRecNameInfoDlg * m_lpRecNameInfoDlg;
	//�طŴ���
	CPlaybackCtrlDlg *m_lpPlaybackCtrlDlg;
	//��ʷҵ�񵥶Ի���
	CHistoryListDlg * m_pHistoryListDlg;	

	//bool m_bIsInit;
	
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
	CArray<CString> m_importFileArray;	//������Ƶ�б�
	CArray<RECOED_FILE_INFO> m_PicFileArray;  //ͼƬ�б�
	CArray<RECOED_FILE_INFO> m_RecFileArray; //¼���б�
	CStringArray m_ArrayFileList;		//ͼƬ��¼���б�

	//չ����
	void ExpandBranch(HTREEITEM hItem, CTreeCtrl& tree, BOOL bExpand =TRUE);
	//ѡ���Ʒ
	LRESULT OnSelectProduct(WPARAM wparam, LPARAM lparam);
	//��ӱ����ļ�
	afx_msg void OnBnClickedBtnUpload();
	//��Ʒ����
	afx_msg void OnCbnDropdownComboProductname();
	//��ʷҵ�񵥰�ť
	afx_msg void OnBnClickedBtnHistorylist();
	//ѡ���Ʒ����
	afx_msg void OnCbnSelchangeComboProductname();
	//�ύ��ť
	afx_msg void OnBnClickedBtnSubmit();
	//��ȡ����Id
	BOOL GetRegionId(void);
	//��ȡ��ƷId
	BOOL GetProductId(void);
	//���ð�ť
	afx_msg void OnBnClickedBtnReset();
	//��Ʒ������ť
	afx_msg void OnBnClickedBtnWords();
	//ѡ����ʷҵ��
	LRESULT OnSelectHisClientInput(WPARAM wParam, LPARAM lParam);
	//��ӡ
	afx_msg void OnBnClickedBtnPrint();

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
	bool m_bIsRecording;
	bool m_bIsHistory;
	//ҵ������id
	int m_nBusinessIndex;
	typedef struct _tagItemData
	{
		CString strFileName;
	}ITEMDATA, *LPITEMDATA;
	CListBox m_listBoxProductCode;
	afx_msg void OnEnChangeEditProductcode();
	afx_msg void OnLbnSelchangeListboxProductcode();
	//���ݲ�Ʒ���ƻ�ȡ��Ʒid
	int GetProductIdByName(CString& strProductName);
	//���ݲ�Ʒ�������ƻ�ȡ��Ʒ����id
	int GetProductTypeIdByName(CString& strProductTypeName);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	CToolTipCtrl m_toolTips;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CFComboBox m_comboBusinessId;
	afx_msg void OnCbnDropdownComboBusinessid();
	void SetEnable(BOOL bEnable);
};

extern CBusinessManageDlg * g_pBusinessMgrDlg;