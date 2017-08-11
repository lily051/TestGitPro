/**
*   @file   SvcCfg.h
*   @note   HangZhou Hikvision System Technology Co.,Ltd. All Right Reserved.
*   @brief  �������ý���
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

// CSvcCfg �Ի���

class CSvcCfg : public CDialog
{
	DECLARE_DYNAMIC(CSvcCfg)

public:
	CSvcCfg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSvcCfg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    DECLARE_MESSAGE_MAP()

public:
	/* ��ˢ */
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	/** @fn       BOOL CSvcCfg::InitDlg(void)
	*  @brief    ��ʼ������
	*  @param    void
	*  @return:  BOOL��TRUE:�����ɹ� ; FALSE:����ʧ��
	*/
	BOOL InitDlg(int nProcessor);

	BOOL HideDlg(int nProcessor);

	BOOL CheckIsChanged(void);

private:

    /** @fn       BOOL CSvcCfg::CheckIPIsValid(const char* pStrIP)
     *  @brief    �ж�IP��ַ�Ƿ�Ϸ�
     *  @param    (const char*) pStrIP [IN] : IP��ַ 
     *  @return:  BOOL��TRUE:�Ϸ� ; FALSE:�Ƿ�
     */
    BOOL CheckIPIsValid(const char* pStrIP);

    /** @fn       CString CSvcCfg::GetLocalIPAddr(void)
     *  @brief    ��ȡ����IP��ַ
     *  @param    void
     *  @return:  CString��"":��ȡʧ�� ; ����:���ص�IP��ַ
     */
    CString GetLocalIPAddr(void);

	CBrush m_brushBkgnd; ///< ��ˢ
	CCenterProcessorCfg m_dlgCenterProcessorCfg;
	CCollectorProcessorCfg m_dlgCollectorProcessorCfg;

	BOOL m_bInit;
};
