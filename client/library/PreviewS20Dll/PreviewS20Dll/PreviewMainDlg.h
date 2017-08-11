#pragma once

#include "PlayViewWnd.h"
#include "PreviewS20API.h"


// CPreviewMainDlg �Ի���

class CPreviewMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CPreviewMainDlg)

public:
	CPreviewMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPreviewMainDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PREVIEW_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

public:

    /*
	*  @fn         CreateChildWnd
    *  @brief      ����Ԥ���Ӵ���
    *  @return     void
    */
    void CreateChildWnd();

    /*
	*  @fn         DeleteChildWnd
    *  @brief      ����Ԥ���Ӵ���
    *  @return     void
    */
    void DeleteChildWnd();

    /*
	*  @fn         SetPreviewLayoutMode
    *  @brief      ����Ԥ�����ڻ���ָ�ģʽ
    *  @param[IN]  nLayoutMode: ����ָ�ģʽ
    *  @return     TRUE: �ɹ� FALSE: ʧ��
    */
    BOOL SetPreviewLayoutMode(int nLayoutMode);

    /** @fn         StartPlayOnSelWnd
    *  @brief      ��ѡ�еĴ����п�ʼԤ��
    *  @param[IN]  struPreviewInfo: Ԥ��������Ϣ
    *  @return     TRUE: �ɹ� FALSE: ʧ��
    */
    BOOL StartPreview(const PREVIEW_CAMINFO &struPreviewInfo);

    /** @fn         SetWinPos
    *  @brief      ����Ԥ������λ��
    *  @return     void
    */
    void SetWinPos();

    /*
	*  @fn         StopPreview
    *  @brief      ֹͣ����Ԥ��
    *  @return     TRUE: �ɹ� FALSE: ʧ��
    */
    BOOL StopPreview();

	//��ʼ¼��
	BOOL StartRecord();

	//ֹͣ¼��
	BOOL StopRecord();

	//��ȡ¼���ļ���Ϣ
	BOOL GetRecordFile(RECOED_FILE_INFO& recordFile);

	BOOL SetEventFunc(PV_EVENT_FUNC Func);

	//����OSD��ʾ��Ϣ
	BOOL SetShowString(const CString &strBusinessInfo);

	//������
	BOOL OpenSound();
private:
    BOOL m_bIsWndInit;
    CPlayViewWnd *m_pPlayviewWnd;      // Ԥ����Ƶ����

};
