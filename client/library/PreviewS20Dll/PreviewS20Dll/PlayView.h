/********************************************************************
    Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.	
	created:	2009/10/09   21:07 
	filename:	playview.h
	author:		GuoLinlin
    Description:  
	Modification History:      
	<version>   <time>        <author>        <desc>	
	   1.0      2009-10-9    GuoLinlin       Created	   
	   1.0      2009-10-9    GuoLinlin       Modify	
	   3.0      2012-4-18    nobody			 Modify	
*********************************************************************/
#pragma once
#include "RealPlayViewWnd.h"
#include "PlayView_WndArrangeOper.h"

#define  PREVIEW_WND_MAXNUM	1 //���Ŵ��������

class CPlayView : public CWnd,public CPlayView_WndArrangeOper
{
	DECLARE_DYNAMIC(CPlayView)

public:
	CPlayView();
	virtual ~CPlayView();

protected:
	DECLARE_MESSAGE_MAP()

private:
	int m_nStartWndIndex;									//��ʼ����������
	int m_nMaxWndIndex;										//��󴰿�������
	int m_nFocusWndIndex;									//���㴰���������
    BOOL m_bFullScreen;                                     //�Ƿ�ȫ��״̬
	CRealPlayViewWnd *m_pRealPlayWnd[PREVIEW_WND_MAXNUM];	//���Ŵ���

public:
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//��������
	BOOL CreateWnd(const RECT & rect, 
		CWnd * pParent, unsigned int uiID,unsigned int uiStartWndIndex = 0,
		unsigned int uiMaxWndNum = PREVIEW_WND_MAXNUM);

	//��������Դ
	void CreateChildWnd();

	//��������Դ
	void DestroyChildWnd(void);

	//���ý��㴰��
	void SetChildWndZoom(int nWndIndex);

	//�������
	BOOL SetChildWndEnlarge();

	//�����ڱ߿򣬽��㴰�ڱ߿��Ǻ�ɫ
	void DrawOutputBorder(void);

	//��ʼ��Ԥ������λ��
	void InitPose(void);

	//���û���ָ�ģʽ
	BOOL SetLayoutMode(WORD wMode);

	//��ȡ������ڴ��ڵ�������
	int GetHoverWndIndex(void);

	//���㴰��Ԥ��
	BOOL StartPreview(const PREVIEW_CAMINFO &struPreviewInfo);

	//���ͣ������Ԥ��
	BOOL StartPreviewMouseHover(const PREVIEW_CAMINFO &struPreviewInfo);

	//ָ������Ԥ��
	BOOL StartPreviewByWndIndex(UINT nWndIndex, const PREVIEW_CAMINFO &struPreviewInfo, 
		int nPresetIndex = -1, BOOL bWriteLog = FALSE);

	//ָ������ֹͣԤ��
	BOOL StopPreviewByWndIndex(UINT nWndIndex, int nPresetIndex = -1);

	//ֹͣԤ��
	void StopPreview(void);

	//��ȡ���㴰�ڼ�ص�ID
	int GetCamIDFromFocusWnd();

	//��ȡ���㴰��ָ��
	CWnd* GetFocusWndPointer();

	//Ԥ�����ڹ�������ʾ����
	void ShowToolBar(int nWndIndex);

    /** @fn         IsWndPreview
     *  @brief      ���ָ�������Ƿ�����Ԥ��
     *  @param[IN]  uWndIndex: ��������
     *  @return     TRUE: ����Ԥ����FALSE: ����
     */
    BOOL IsWndPreview(UINT uWndIndex);

    /** @fn         BwEnforceStopPreview
     *  @brief      ������Ʒ�����ǿ�ƹر�ָ����ص��Ԥ��
     *  @param[IN]  nCamID: ָ����ص�
     *  @return     void
     */
    void BwEnforceStopPreview(int nCamID);

    ///** @fn         BwReRequestPreview
    // *  @brief      �ͻ��������ϴ�����Ʒ�������������������Ԥ���ļ�ص�
    // *  @return     void
    // */
    //void BwReRequestPreview();

    /** @fn         CheckCameraIsPlaying
     *  @brief      ���ָ����ص��Ƿ�����Ԥ��
     *  @param[IN]  nCamID: ָ����ص�ID
     *  @return     TRUE: �У�FALSE: ��
     */
    BOOL CheckCameraIsPlaying(int nCamID);

    /** @fn         SetFullScreenStatus
     *  @brief      ����ȫ��״̬
     *  @param[IN]  bFullScreen: �Ƿ�ȫ��
     *  @return     void
     */
    void SetFullScreenStatus(BOOL bFullScreen = FALSE);

	//��ʼ¼��
	BOOL StartRecord();

	//ֹͣ¼��
	BOOL StopRecord();

	//������Ƶ
    BOOL OpenSound();

	//�ر���Ƶ
	BOOL CloseSound();

	//��ȡ¼���ļ���Ϣ
	BOOL GetRecordFile(RECOED_FILE_INFO& recordFile);

	BOOL SetEventFunc(PV_EVENT_FUNC Func);

	void SetFirstWndWave(CScope *pVideoWav, CScope *pAudioWav);

	//����OSD��ʾ��Ϣ
	BOOL SetShowString(const CString &strBusinessInfo);

	void OnToolBarExecute(int nBtnType);
};