#pragma once

#define XML_HEAD                    _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n")
#define XML_NODE_ROOT               _T("ROOT")
#define XML_NODE_LOG_PATH           _T("PARAM_LOG_PATH")
#define XML_NODE_CODE               _T("CODE")
#define XML_NODE_DEV_TYPE           _T("DEV_TYPE")
#define XML_NODE_DEV_IP             _T("DEV_IP")
#define XML_NODE_DEV_PORT           _T("DEV_PORT")
#define XML_NODE_DEV_USER           _T("DEV_USER")
#define XML_NODE_DEV_PWD            _T("DEV_PWD")
#define XML_NODE_DEV_DEV_CHANNEL    _T("DEV_CHANNEL")
#define XML_NODE_DEV_SHOW_IP_MODE   _T("DEV_SHOW_IP_MODE")
#define XML_NODE_CHANNEL            _T("CHANNEL")
#define XML_NODE_CHANNEL_NUM        _T("channel")
#define XML_NODE_CHANNEL_ENABLE     _T("enable")
#define XML_NODE_DISK_INFO          _T("DISK_INFO")
#define XML_NODE_DISK_CAPACITY      _T("CAPACITY")
#define XML_NODE_DISK_FREESPACE     _T("FREESPACE")
#define XML_NODE_DISK_RECYCLING     _T("RECYCLING")
#define XML_NODE_DISK_HDNO          _T("HDNO")
#define XML_NODE_DISK_HDSTATUS      _T("HDSTATUS")
#define XML_NODE_DISK_HDADDR        _T("HDADDR")
#define XML_NODE_DISK_HDTYPE        _T("HDTYPE")
#define XML_NODE_DISK_HDDRIVER      _T("HDDRIVER")
#define XML_NODE_NTP_SERVER         _T("NTP_SERVER")
#define XML_NODE_NTP_PORT           _T("NTP_PORT")
#define XML_NODE_NTP_INTERVAL       _T("NTP_INTERVAL")
#define XML_NODE_NTP_ENABLE         _T("ENABLE_NTP")


// RPConfigCtrl.h : CRPConfigCtrl ActiveX �ؼ����������


// CRPConfigCtrl : �й�ʵ�ֵ���Ϣ������� RPConfigCtrl.cpp��


class CRPConfigCtrl : public COleControl
{
	DECLARE_DYNCREATE(CRPConfigCtrl)

// ���캯��
public:
	CRPConfigCtrl();

// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

    
// ʵ��
protected:
	~CRPConfigCtrl();

	DECLARE_OLECREATE_EX(CRPConfigCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(CRPConfigCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CRPConfigCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(CRPConfigCtrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()
	
// �¼�ӳ��
	DECLARE_EVENT_MAP()

    afx_msg void AboutBox();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

// ���Ⱥ��¼� ID
public:
	enum {
        dispidRP_GetVersion = 4L,
        dispidRP_Function1 = 3L,
        dispidRP_UninitLib = 2L,
        dispidRP_InitLib = 1L
    };
protected://����ӿ�
    /**	@fn	    RP_InitLib
    *	@brief	��ʼ����
    *	@param  [in] xmlParam��������
    *	@return	0�ɹ���-1ʧ��
    */
    LONG RP_InitLib(void);

    /**	@fn	    RP_UninitLib
    *	@brief	����ʼ����
    *	@param  [in] 
    *	@return	0�ɹ���-1ʧ��
    */
    LONG RP_UninitLib(void);

    /**	@fn	    RP_Function1
    *	@brief	���ܺ���1
    *	@param  [in] xmlParam��������
    *	@return	��ʧ��;�ɹ����ؽ��XML����
    */
    BSTR RP_Function1(LPCTSTR xmlParam);
private:
    BOOL            m_bInit;//�Ƿ񱻳�ʼ����
    INIT_LIB_PARAM  m_initLibParam;
private://XMLת������
    /**	@fn	    ParseInitLibParam
    *	@brief	ת����ʼ������
    *	@param  [in] xmlParam��������
    *   @param  [out] initLibParamת����Ĳ�����Ϣ
    *	@return	TRUE�ɹ���FALSEʧ��
    */
    BOOL ParseInitLibParam(LPCTSTR xmlParam, INIT_LIB_PARAM & initLibParam);

    /**	@fn	    ParseFunParam
    *	@brief	ת����������
    *	@param  [in] xmlParam��������
    *   @param  [out] funParam��������
    *	@return	TRUE�ɹ���FALSEʧ��
    */
    BOOL ParseFunParam(LPCTSTR xmlParam, FUN_PARAM & funParam);

private://ͳһ�ӿ�
    //ͳһ�ӿ�////////////////////////////////////////////////////////////////////////
    /**	@fn	    GetDevInfo
    *	@brief	��ȡ�豸����
    *	@param  [in] funParam������Ϣ
    *	@return	��ʧ��;�ɹ����ؽ��XML����
    */
    BSTR GetDevInfo(const FUN_PARAM & funParam);

    /**	@fn	    GetChannelName
    *	@brief	��ȡͨ������
    *	@param  [in] funParam������Ϣ
    *	@return	��ʧ��;�ɹ����ؽ��XML����
    */
    BSTR GetChannelName(const FUN_PARAM & funParam);

private://�����ӿ�////////////////////////////////////////////////////////////////////////
    /**	@fn	    HIK_GetDevInfo
    *	@brief	��ȡ�豸����[�����豸]
    *	@param  [in] funParam������Ϣ
    *	@return	��ʧ��;�ɹ����ؽ��XML����
    */
    BSTR HIK_GetDevInfo(const FUN_PARAM & funParam);

    /**	@fn	    HIK_GetChannelName
    *	@brief	��ȡͨ������[�����豸]
    *	@param  [in] funParam������Ϣ
    *	@return	��ʧ��;�ɹ����ؽ��XML����
    */
    BSTR HIK_GetChannelName(const FUN_PARAM & funParam);

	//CVR�ӿ�>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	BSTR CVR_GetDevInfo(const FUN_PARAM & funParam);

	//�ƴ洢�ӿ�>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	BSTR CLOUD_GetDevInfo(const FUN_PARAM & funParam);

private:
    BSTR ParseDevInfoResult(const RP_DEV_INFO & rpDevInfo);
    BSTR SetDevNTPConfig(const FUN_PARAM & funParam);
protected:
    BSTR RP_GetVersion(void);
public:
    virtual BOOL IsInvokeAllowed(DISPID /*dispid*/);
    
};

