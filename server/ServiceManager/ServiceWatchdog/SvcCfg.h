/**
*   @file   SvcCfg.h
*   @note   HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief  服务配置界面
*  
*   @author mzt
*   @date   2013/07/01
*
*   @note   <create>    <2013/07/01>
*
*   @warning
*/

#pragma once
#include "resource.h"
//#include "afxwin.h"
#include "./Utilities/MyIPAddressCtrl.h"
#include "./CenterProcessorCfg.h"
#include "./CollectorProcessorCfg.h"

// CSvcCfg 对话框

class CSvcCfg : public CDialog
{
	DECLARE_DYNAMIC(CSvcCfg)

public:
	CSvcCfg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSvcCfg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    DECLARE_MESSAGE_MAP()

public:
	/* 画刷 */
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	/** @fn       BOOL CSvcCfg::InitDlg(void)
	*  @brief    初始化界面
	*  @param    void
	*  @return:  BOOL：TRUE:操作成功 ; FALSE:操作失败
	*/
	BOOL InitDlg(int nProcessor);

	BOOL HideDlg(int nProcessor);

	BOOL CheckIsChanged(void);

private:

    /** @fn       BOOL CSvcCfg::CheckIPIsValid(const char* pStrIP)
     *  @brief    判断IP地址是否合法
     *  @param    (const char*) pStrIP [IN] : IP地址 
     *  @return:  BOOL：TRUE:合法 ; FALSE:非法
     */
    BOOL CheckIPIsValid(const char* pStrIP);

    /** @fn       CString CSvcCfg::GetLocalIPAddr(void)
     *  @brief    获取本机IP地址
     *  @param    void
     *  @return:  CString："":获取失败 ; 其他:本地的IP地址
     */
    CString GetLocalIPAddr(void);

	CBrush m_brushBkgnd; ///< 画刷
	CCenterProcessorCfg m_dlgCenterProcessorCfg;
	CCollectorProcessorCfg m_dlgCollectorProcessorCfg;

	BOOL m_bInit;
};
