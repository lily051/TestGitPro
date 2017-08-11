// PreviewS20Dll.h : PreviewS20Dll DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "CustomLayoutData.h"
#include "TipHeader.h"



// CPreviewS20DllApp
// 有关此类实现的信息，请参阅 PreviewS20Dll.cpp
//

class CPreviewS20DllApp : public CWinApp
{
public:
	CPreviewS20DllApp();

// 重写
public:
	virtual BOOL InitInstance();
    virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()

public:

    /** @fn         GetUIColor
    *  @brief      获取指定类型颜色值
    *  @param[IN]  nClrType: 颜色类型，参见_ENUM_UI_COLOR_TYPE
    *  @return     色值
    */
    COLORREF GetUIColor(int nClrType);

    /** @fn         ShowCustomMsg
    *  @brief      显示自定义消息
    *  @param[IN]  strCaption: 消息标题
    *  @param[IN]  strContent: 消息内容
    *  @param[IN]  nMsgType: 消息类型(0:普通文字消息 1:超链接消息)
    *  @return     void
    */
    void ShowCustomMsg(const CString &strCaption, const CString &strContent, int nMsgType = 0);

    /** @fn         GetOperateRight
    *  @brief      获取用户对应的操作权限
    *  @param[IN]  nOperType: 权限类型
    *  @return     TRUE: 有，FALSE: 无
    */
    BOOL GetOperateRight(int nOperType);

    /** @fn         GetLocalCfg
    *  @brief      获取预览本地参数配置
    *  @param[IN]  stLocalCfg: 参数配置信息
    *  @return     void
    */
    void GetLocalCfg(LOCAL_CFG &stLocalCfg);

public:
    int m_nUIStyle;                             ///< 界面风格
    HINSTANCE m_hResInstance;                   ///< 资源句柄
    CWnd * m_pParentWnd;                        ///< 父窗口
    CString m_strModulePath;                    ///< 模块目录
    CString m_strLocalIPAddr;                   ///< 本地IP地址
    LOCAL_CFG m_stLocalCfg;                     ///< 预览本地参数配置
    PREVIEW_PARAM m_stPreviewParam;             ///< 预览参数
	CString m_strSavePath;                      ///录像文件保存路径
	int m_nPreviewModel;
	LONG m_nBtnRights;                          //按钮功能权限
    BOOL m_bUSBExit;
	LONG m_lUSBPort;
    CCustomLayoutData m_customLayoutData;       ///< 分割模式管理
};

extern CPreviewS20DllApp theApp;
