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


// RPConfigCtrl.h : CRPConfigCtrl ActiveX 控件类的声明。


// CRPConfigCtrl : 有关实现的信息，请参阅 RPConfigCtrl.cpp。


class CRPConfigCtrl : public COleControl
{
	DECLARE_DYNCREATE(CRPConfigCtrl)

// 构造函数
public:
	CRPConfigCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

    
// 实现
protected:
	~CRPConfigCtrl();

	DECLARE_OLECREATE_EX(CRPConfigCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CRPConfigCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CRPConfigCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CRPConfigCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()
	
// 事件映射
	DECLARE_EVENT_MAP()

    afx_msg void AboutBox();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

// 调度和事件 ID
public:
	enum {
        dispidRP_GetVersion = 4L,
        dispidRP_Function1 = 3L,
        dispidRP_UninitLib = 2L,
        dispidRP_InitLib = 1L
    };
protected://对外接口
    /**	@fn	    RP_InitLib
    *	@brief	初始化库
    *	@param  [in] xmlParam参数报文
    *	@return	0成功，-1失败
    */
    LONG RP_InitLib(void);

    /**	@fn	    RP_UninitLib
    *	@brief	反初始化库
    *	@param  [in] 
    *	@return	0成功，-1失败
    */
    LONG RP_UninitLib(void);

    /**	@fn	    RP_Function1
    *	@brief	功能函数1
    *	@param  [in] xmlParam参数报文
    *	@return	空失败;成功返回结果XML报文
    */
    BSTR RP_Function1(LPCTSTR xmlParam);
private:
    BOOL            m_bInit;//是否被初始化过
    INIT_LIB_PARAM  m_initLibParam;
private://XML转换函数
    /**	@fn	    ParseInitLibParam
    *	@brief	转换初始化参数
    *	@param  [in] xmlParam参数报文
    *   @param  [out] initLibParam转换后的参数信息
    *	@return	TRUE成功，FALSE失败
    */
    BOOL ParseInitLibParam(LPCTSTR xmlParam, INIT_LIB_PARAM & initLibParam);

    /**	@fn	    ParseFunParam
    *	@brief	转换函数参数
    *	@param  [in] xmlParam参数报文
    *   @param  [out] funParam函数参数
    *	@return	TRUE成功，FALSE失败
    */
    BOOL ParseFunParam(LPCTSTR xmlParam, FUN_PARAM & funParam);

private://统一接口
    //统一接口////////////////////////////////////////////////////////////////////////
    /**	@fn	    GetDevInfo
    *	@brief	获取设备名称
    *	@param  [in] funParam参数信息
    *	@return	空失败;成功返回结果XML报文
    */
    BSTR GetDevInfo(const FUN_PARAM & funParam);

    /**	@fn	    GetChannelName
    *	@brief	获取通道名称
    *	@param  [in] funParam参数信息
    *	@return	空失败;成功返回结果XML报文
    */
    BSTR GetChannelName(const FUN_PARAM & funParam);

private://海康接口////////////////////////////////////////////////////////////////////////
    /**	@fn	    HIK_GetDevInfo
    *	@brief	获取设备名称[海康设备]
    *	@param  [in] funParam参数信息
    *	@return	空失败;成功返回结果XML报文
    */
    BSTR HIK_GetDevInfo(const FUN_PARAM & funParam);

    /**	@fn	    HIK_GetChannelName
    *	@brief	获取通道名称[海康设备]
    *	@param  [in] funParam参数信息
    *	@return	空失败;成功返回结果XML报文
    */
    BSTR HIK_GetChannelName(const FUN_PARAM & funParam);

	//CVR接口>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	BSTR CVR_GetDevInfo(const FUN_PARAM & funParam);

	//云存储接口>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	BSTR CLOUD_GetDevInfo(const FUN_PARAM & funParam);

private:
    BSTR ParseDevInfoResult(const RP_DEV_INFO & rpDevInfo);
    BSTR SetDevNTPConfig(const FUN_PARAM & funParam);
protected:
    BSTR RP_GetVersion(void);
public:
    virtual BOOL IsInvokeAllowed(DISPID /*dispid*/);
    
};

