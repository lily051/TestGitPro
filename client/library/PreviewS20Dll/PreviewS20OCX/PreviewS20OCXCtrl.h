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
	//开始预览
	LONG StartPreview(LPCTSTR strXML);
	//停止预览
	LONG StopPreview(LONG lWndNum);
	//开始录像
	LONG StartRecord(LONG lWndNum);
	//停止录像
	BSTR StopRecord(LONG lWndNum);
	//设置OSD信息
	LONG SetShowString(LPCTSTR strXML);
	//设置配置信息
	LONG SetConfigData(LPCTSTR strXML);
	//设置可信任站点
	LONG SetTrustedSite(LPCTSTR strXML);
	//获取USB监控点信息
	BSTR GetAllUSBCameraInfo();

public:
	/*
	*   @fn         ParseXml4Preview
	*   @brief      将XML格式解码为PREVIEW_CAMINFO结构
	*   @param[IN]  strXml:预览信息
	*   @param[OUT] struPrevCamInfo:预览信息
	*   @return     void
	*/
	BOOL ParseXml4Preview(PREVIEW_CAMINFO &struPrevCamInfo, const CString &strXml);
	
	/** @fn         SerialRecordFile4Xml
	*   @brief      将RECOED_FILE_INFO结构序列化为XML格式
	*   @param[IN]  recoedFile:录制文件信息
	*   @param[OUT] strXml:XML格式信息
	*   @return     void
	*/
	void SerialRecordFile4Xml(const RECOED_FILE_INFO& recoedFile, CString& strXml);

	/** @fn         NotifyRecordFileInfo
	*   @brief      外部通知文件录制完成信息
	*   @param[IN]  recoedFile:录制文件信息
	*   @return     void
	*/
	void NotifyRecordFileInfo(const RECOED_FILE_INFO &recoedFile);

	/*
	*	@fn	ReadCard
    *	@brief  读卡执行函数
    *	@param  Input:	无
    *           Output: 无
    *           InOut:  无
    *	@return	
    */
    void ReadCard();

protected:
	LONG m_lPreviewMode;               //布局模式
	LONG m_lSkinType;                 //皮肤类型
	LONG m_lBtnRights;                //控件权限
	BOOL m_bManualStop;	              //是否为手动关闭录像

	void OnlLayoutModeChanged(void);
	void OnlSkinTypeChanged(void);
	void OnlBtnRightsChanged(void);


	//获取录像文件回调函数
	void RecordFile(LPCTSTR lpstrXML)
	{
		// 如果是手动停止录像，就不用在进行事件提醒了
		if (!m_bManualStop)
		{
			FireEvent(dispidRecordFile, EVENT_PARAM(VTS_BSTR), lpstrXML);
		}
		m_bManualStop = FALSE;
	}

private:
    ///////////////////////////////////身份证读卡器begin////////////////////////////////////////////
    BOOL m_bInitSucc;             //初始化成功
    volatile bool m_bStopThread;  //开始线程
    HANDLE m_hReadCardThread;     //读卡线程
    CString m_strFilePath;        //照片存储路径
    CString m_strIdCardInfo;//身份证信息存储
	/*
	*	@fn	Start
    *	@brief	开启读卡线程
    *	@param  Input:	无
    *           Output: 无
    *           InOut:  无
    *	@return	
    */
    bool Start();

    /*
	*	@fn	Stop
    *	@brief	停止读卡线程，断开设备连接
    *	@param  Input:	无
    *           Output: 无
    *           InOut:  无
    *	@return	
    */
    void Stop();


    //身份证信息消息处理函数
    LRESULT OnFireFingerPrint(WPARAM wParam, LPARAM lParam);

	/*
	*	@fn	InitializeCardReader
    *	@brief	初始化函数，连接发卡器，并开启读卡线程
    *	@param  Input:	无
    *           Output: 无
    *           InOut:  无
    *	@return	初始化结果，0 成功，-1 连接发卡器失败 -2 开启读卡线程失败
    */
    LONG InitializeReader(LPCTSTR lpFilePath); 

    /**	@fn	UnInitializeCardReader
    *	@brief	反初始化函数，停止读卡线程并断开设备连接
    *	@param  Input:	无
    *           Output: 无
    *           InOut:  无
    *	@return	反初始化结果，0 成功
    */
    LONG UnInitializeReader(); // 关闭读卡线程，反初始化发卡机

    /**	@fn	FireCardSerialNo
    *	@brief	抛送身份证信息
    *	@param  Input:	lpName  姓名
    *	@param  Input:	lpIDCardNo  身份证号
    *	@param  Input:	lpBirthday  出生日期
    *	@param  Input:	lpSex  性别
    *	@param  Input:	lpNation  民族
    *	@param  Input:	lpAddress  住址
    *	@param  Input:	lpBeginDate  有效期开始日期
    *	@param  Input:	lpEndDate  有效期结束日期
    *	@param  Input:	lpGovernment  签发相关
    *           Output: 无
    *           InOut:  无
    *	@return	
    */
    void FireIDCardInfo(LPCTSTR lpName, LPCTSTR lpIDCardNo, LPCTSTR lpBirthday, 
                        LPCTSTR lpSex, LPCTSTR lpNation, LPCTSTR lpAddress, 
                        LPCTSTR lpBeginDate, LPCTSTR lpEndDate, LPCTSTR lpGovernment); 

    ///////////////////////////////////身份证读卡器end////////////////////////////////////////////
};

extern CPreviewS20OCXCtrl *g_pCtrl;
