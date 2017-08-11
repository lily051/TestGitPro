#pragma once

#include "PreviewDlg.h"

class CPreviewS20OCXCtrl : public COleControl
{
	DECLARE_DYNCREATE(CPreviewS20OCXCtrl)
private:
	CPreviewDlg * m_pPreviewDlg;

// ���캯��
public:
	CPreviewS20OCXCtrl();

// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// ʵ��
protected:
	~CPreviewS20OCXCtrl();

	DECLARE_OLECREATE_EX(CPreviewS20OCXCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(CPreviewS20OCXCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CPreviewS20OCXCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(CPreviewS20OCXCtrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
public:
	enum {
		dispidStopRecordByXmlEx = 13L,
		dispidStopRecordByXml = 12L,
		dispidEnableRecordEx = 11L,
		dispidEnableRecord = 10L,
		dispidStopRecordByWndNumXML = 9L,
		dispidOnRecordFileInfo = 1L,
		dispidlSkinType = 8,
		dispidlLayoutMode = 7,
		dispidStopAllPreview = 6L,
		dispidStopRecord = 5L,
		dispidStartRecord = 4L,
		dispidStopPreview = 3L,
		dispidStartPlayByWndNum = 2L,
		dispidStartPreview = 1L
	};

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	LONG StartPreview(LPCTSTR strXML);
	LONG StopPreview(void);
	LONG StartRecord(LONG lWndNum);
	BSTR StopRecord(LONG lWndNum);
	BOOL m_bManualStop;	// �Ƿ�Ϊ�ֶ��ر�¼��
	LONG StopAllPreview(void);

public:
	// 
	void ParseXml4Preview(PREVIEW_CAMINFO &struPrevCamInfo, const CString &strXml);
	
	/** @fn         SerialRecordFile4Xml
	*   @brief      ��RECOED_FILE_INFO�ṹ���л�ΪXML��ʽ
	*   @param[IN]  recoedFile:¼���ļ���Ϣ
	*   @param[OUT] strXml:XML��ʽ��Ϣ
	*   @return     void
	*/
	void SerialRecordFile4Xml(const RECOED_FILE_INFO &recoedFile, CString& strXml);

	/** @fn         NotifyRecordFileInfo
	*   @brief      �ⲿ֪ͨ�ļ�¼�������Ϣ
	*   @param[IN]  recoedFile:¼���ļ���Ϣ
	*   @return     void
	*/
	void NotifyRecordFileInfo(const RECOED_FILE_INFO &recoedFile);

protected:
	void OnlLayoutModeChanged(void);
	LONG m_lLayoutMode;
	void OnlSkinTypeChanged(void);
	LONG m_lSkinType;

public:
	virtual BOOL IsInvokeAllowed(DISPID /*dispid*/);

protected:
	void OnRecordFileInfo(LPCTSTR lpstrXML)
	{
		// ������ֶ�ֹͣ¼�񣬾Ͳ����ڽ����¼�������
		if (!m_bManualStop)
		{
			FireEvent(dispidOnRecordFileInfo, EVENT_PARAM(VTS_BSTR), lpstrXML);
		}
		m_bManualStop = FALSE;
	}
	LONG StopRecordByXml(LPCTSTR strXML);
	LONG StopRecordByXmlEx(LPCTSTR strXml);
};

extern CPreviewS20OCXCtrl *g_pCtrl;
