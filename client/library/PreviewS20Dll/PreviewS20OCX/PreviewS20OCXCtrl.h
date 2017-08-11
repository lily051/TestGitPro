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
	enum 
	{
		dispidlBtnRights     = 11L,
		dispidSetTrustedSite = 10L,
		dispidGetAllUsbCameraInfo = 9L,
		dispidlSkinType     = 8L,
		dispidlLayoutMode   = 7L,
		dispidFireIDCardInfo= 2L,
		dispidSetConfigData = 6L,
		dispidSetShowString = 5L,
		dispidRecordFile    = 1L,
		dispidStopRecord    = 4L,
		dispidStartRecord   = 3L,
		dispidStopPreview   = 2L,
		dispidStartPreview  = 1L
	};

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);


protected:
	//��ʼԤ��
	LONG StartPreview(LPCTSTR strXML);
	//ֹͣԤ��
	LONG StopPreview(LONG lWndNum);
	//��ʼ¼��
	LONG StartRecord(LONG lWndNum);
	//ֹͣ¼��
	BSTR StopRecord(LONG lWndNum);
	//����OSD��Ϣ
	LONG SetShowString(LPCTSTR strXML);
	//����������Ϣ
	LONG SetConfigData(LPCTSTR strXML);
	//���ÿ�����վ��
	LONG SetTrustedSite(LPCTSTR strXML);
	//��ȡUSB��ص���Ϣ
	BSTR GetAllUSBCameraInfo();

public:
	/*
	*   @fn         ParseXml4Preview
	*   @brief      ��XML��ʽ����ΪPREVIEW_CAMINFO�ṹ
	*   @param[IN]  strXml:Ԥ����Ϣ
	*   @param[OUT] struPrevCamInfo:Ԥ����Ϣ
	*   @return     void
	*/
	BOOL ParseXml4Preview(PREVIEW_CAMINFO &struPrevCamInfo, const CString &strXml);
	
	/** @fn         SerialRecordFile4Xml
	*   @brief      ��RECOED_FILE_INFO�ṹ���л�ΪXML��ʽ
	*   @param[IN]  recoedFile:¼���ļ���Ϣ
	*   @param[OUT] strXml:XML��ʽ��Ϣ
	*   @return     void
	*/
	void SerialRecordFile4Xml(const RECOED_FILE_INFO& recoedFile, CString& strXml);

	/** @fn         NotifyRecordFileInfo
	*   @brief      �ⲿ֪ͨ�ļ�¼�������Ϣ
	*   @param[IN]  recoedFile:¼���ļ���Ϣ
	*   @return     void
	*/
	void NotifyRecordFileInfo(const RECOED_FILE_INFO &recoedFile);

	/*
	*	@fn	ReadCard
    *	@brief  ����ִ�к���
    *	@param  Input:	��
    *           Output: ��
    *           InOut:  ��
    *	@return	
    */
    void ReadCard();

protected:
	LONG m_lPreviewMode;               //����ģʽ
	LONG m_lSkinType;                 //Ƥ������
	LONG m_lBtnRights;                //�ؼ�Ȩ��
	BOOL m_bManualStop;	              //�Ƿ�Ϊ�ֶ��ر�¼��

	void OnlLayoutModeChanged(void);
	void OnlSkinTypeChanged(void);
	void OnlBtnRightsChanged(void);


	//��ȡ¼���ļ��ص�����
	void RecordFile(LPCTSTR lpstrXML)
	{
		// ������ֶ�ֹͣ¼�񣬾Ͳ����ڽ����¼�������
		if (!m_bManualStop)
		{
			FireEvent(dispidRecordFile, EVENT_PARAM(VTS_BSTR), lpstrXML);
		}
		m_bManualStop = FALSE;
	}

private:
    ///////////////////////////////////���֤������begin////////////////////////////////////////////
    BOOL m_bInitSucc;             //��ʼ���ɹ�
    volatile bool m_bStopThread;  //��ʼ�߳�
    HANDLE m_hReadCardThread;     //�����߳�
    CString m_strFilePath;        //��Ƭ�洢·��
    CString m_strIdCardInfo;//���֤��Ϣ�洢
	/*
	*	@fn	Start
    *	@brief	���������߳�
    *	@param  Input:	��
    *           Output: ��
    *           InOut:  ��
    *	@return	
    */
    bool Start();

    /*
	*	@fn	Stop
    *	@brief	ֹͣ�����̣߳��Ͽ��豸����
    *	@param  Input:	��
    *           Output: ��
    *           InOut:  ��
    *	@return	
    */
    void Stop();


    //���֤��Ϣ��Ϣ������
    LRESULT OnFireFingerPrint(WPARAM wParam, LPARAM lParam);

	/*
	*	@fn	InitializeCardReader
    *	@brief	��ʼ�����������ӷ������������������߳�
    *	@param  Input:	��
    *           Output: ��
    *           InOut:  ��
    *	@return	��ʼ�������0 �ɹ���-1 ���ӷ�����ʧ�� -2 ���������߳�ʧ��
    */
    LONG InitializeReader(LPCTSTR lpFilePath); 

    /**	@fn	UnInitializeCardReader
    *	@brief	����ʼ��������ֹͣ�����̲߳��Ͽ��豸����
    *	@param  Input:	��
    *           Output: ��
    *           InOut:  ��
    *	@return	����ʼ�������0 �ɹ�
    */
    LONG UnInitializeReader(); // �رն����̣߳�����ʼ��������

    /**	@fn	FireCardSerialNo
    *	@brief	�������֤��Ϣ
    *	@param  Input:	lpName  ����
    *	@param  Input:	lpIDCardNo  ���֤��
    *	@param  Input:	lpBirthday  ��������
    *	@param  Input:	lpSex  �Ա�
    *	@param  Input:	lpNation  ����
    *	@param  Input:	lpAddress  סַ
    *	@param  Input:	lpBeginDate  ��Ч�ڿ�ʼ����
    *	@param  Input:	lpEndDate  ��Ч�ڽ�������
    *	@param  Input:	lpGovernment  ǩ�����
    *           Output: ��
    *           InOut:  ��
    *	@return	
    */
    void FireIDCardInfo(LPCTSTR lpName, LPCTSTR lpIDCardNo, LPCTSTR lpBirthday, 
                        LPCTSTR lpSex, LPCTSTR lpNation, LPCTSTR lpAddress, 
                        LPCTSTR lpBeginDate, LPCTSTR lpEndDate, LPCTSTR lpGovernment); 

    ///////////////////////////////////���֤������end////////////////////////////////////////////
};

extern CPreviewS20OCXCtrl *g_pCtrl;
