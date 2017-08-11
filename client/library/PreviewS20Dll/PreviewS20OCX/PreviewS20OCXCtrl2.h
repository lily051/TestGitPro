#pragma once

#include "PreviewDlg.h"

class CPreviewS20OCXCtrl : public COleControl
{
	DECLARE_DYNCREATE(CPreviewS20OCXCtrl)
private:
	CPreviewDlg * m_pPreviewDlg;

// 构造函数
public:
	CPreviewS20OCXCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CPreviewS20OCXCtrl();

	DECLARE_OLECREATE_EX(CPreviewS20OCXCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CPreviewS20OCXCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CPreviewS20OCXCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CPreviewS20OCXCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
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
	BOOL m_bManualStop;	// 是否为手动关闭录像
	LONG StopAllPreview(void);

public:
	// 
	void ParseXml4Preview(PREVIEW_CAMINFO &struPrevCamInfo, const CString &strXml);
	
	/** @fn         SerialRecordFile4Xml
	*   @brief      将RECOED_FILE_INFO结构序列化为XML格式
	*   @param[IN]  recoedFile:录制文件信息
	*   @param[OUT] strXml:XML格式信息
	*   @return     void
	*/
	void SerialRecordFile4Xml(const RECOED_FILE_INFO &recoedFile, CString& strXml);

	/** @fn         NotifyRecordFileInfo
	*   @brief      外部通知文件录制完成信息
	*   @param[IN]  recoedFile:录制文件信息
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
		// 如果是手动停止录像，就不用在进行事件提醒了
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
