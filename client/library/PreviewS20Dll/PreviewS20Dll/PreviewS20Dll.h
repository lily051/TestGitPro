// PreviewS20Dll.h : PreviewS20Dll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "CustomLayoutData.h"
#include "TipHeader.h"



// CPreviewS20DllApp
// �йش���ʵ�ֵ���Ϣ������� PreviewS20Dll.cpp
//

class CPreviewS20DllApp : public CWinApp
{
public:
	CPreviewS20DllApp();

// ��д
public:
	virtual BOOL InitInstance();
    virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()

public:

    /** @fn         GetUIColor
    *  @brief      ��ȡָ��������ɫֵ
    *  @param[IN]  nClrType: ��ɫ���ͣ��μ�_ENUM_UI_COLOR_TYPE
    *  @return     ɫֵ
    */
    COLORREF GetUIColor(int nClrType);

    /** @fn         ShowCustomMsg
    *  @brief      ��ʾ�Զ�����Ϣ
    *  @param[IN]  strCaption: ��Ϣ����
    *  @param[IN]  strContent: ��Ϣ����
    *  @param[IN]  nMsgType: ��Ϣ����(0:��ͨ������Ϣ 1:��������Ϣ)
    *  @return     void
    */
    void ShowCustomMsg(const CString &strCaption, const CString &strContent, int nMsgType = 0);

    /** @fn         GetOperateRight
    *  @brief      ��ȡ�û���Ӧ�Ĳ���Ȩ��
    *  @param[IN]  nOperType: Ȩ������
    *  @return     TRUE: �У�FALSE: ��
    */
    BOOL GetOperateRight(int nOperType);

    /** @fn         GetLocalCfg
    *  @brief      ��ȡԤ�����ز�������
    *  @param[IN]  stLocalCfg: ����������Ϣ
    *  @return     void
    */
    void GetLocalCfg(LOCAL_CFG &stLocalCfg);

public:
    int m_nUIStyle;                             ///< ������
    HINSTANCE m_hResInstance;                   ///< ��Դ���
    CWnd * m_pParentWnd;                        ///< ������
    CString m_strModulePath;                    ///< ģ��Ŀ¼
    CString m_strLocalIPAddr;                   ///< ����IP��ַ
    LOCAL_CFG m_stLocalCfg;                     ///< Ԥ�����ز�������
    PREVIEW_PARAM m_stPreviewParam;             ///< Ԥ������
	CString m_strSavePath;                      ///¼���ļ�����·��
	int m_nPreviewModel;
	LONG m_nBtnRights;                          //��ť����Ȩ��
    BOOL m_bUSBExit;
	LONG m_lUSBPort;
    CCustomLayoutData m_customLayoutData;       ///< �ָ�ģʽ����
};

extern CPreviewS20DllApp theApp;
